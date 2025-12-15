import pytest
import subprocess
import time
import requests
import os
import shutil
import socket

TEST_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(TEST_DIR)
SERVER_BIN = os.path.join(PROJECT_ROOT, "main")
TEST_PORT = 8082
TEST_HOST = "127.0.0.1"
BASE_URL = f"http://{TEST_HOST}:{TEST_PORT}"
TEST_CONF_FILE = os.path.join(TEST_DIR, "server.conf")
TEST_UPLOAD_DIR = os.path.join(TEST_DIR, "uploads")
TEST_LOG_FILE = "server_test.log"

@pytest.fixture(scope="class", autouse=True)
def setup_server_class(request):
    config_content = f"""
port={TEST_PORT}
root_dir=.
storage_dir=./uploads
ip={TEST_HOST}
max_clients=10
log_file={TEST_LOG_FILE}
keep_alive_timeout=2
"""
    with open(TEST_CONF_FILE, "w") as f:
        f.write(config_content.strip())

    if os.path.exists(TEST_UPLOAD_DIR):
        shutil.rmtree(TEST_UPLOAD_DIR)
    os.makedirs(TEST_UPLOAD_DIR)
    
    with open(os.path.join(TEST_DIR, "index.html"), "w") as f:
        f.write("<h1>Unit Test Index</h1>")

    subprocess.run(["make"], cwd=PROJECT_ROOT, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    print(f"\n[Setup] Launching server on port {TEST_PORT}...")
    server_process = subprocess.Popen(
        [SERVER_BIN],
        cwd=TEST_DIR,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

    timeout = 2
    start_time = time.time()
    while time.time() - start_time < timeout:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        result = sock.connect_ex((TEST_HOST, TEST_PORT))
        sock.close()
        if result == 0:
            break
        time.sleep(0.1)

    yield

    print("\n[Teardown] Killing server...")
    server_process.terminate()
    server_process.wait()

    if os.path.exists(os.path.join(TEST_DIR, "index.html")):
        os.remove(os.path.join(TEST_DIR, "index.html"))
    if os.path.exists(TEST_CONF_FILE):
        os.remove(TEST_CONF_FILE)
    if os.path.exists(TEST_UPLOAD_DIR):
        shutil.rmtree(TEST_UPLOAD_DIR)

# ==========================================
#      POSITIVE SCENARIOS (Happy Path)
# ==========================================
class TestPositiveScenarios:
    
    def test_get_root_page(self):
        """[Positive]Getting the main page (index.html)."""
        response = requests.get(f"{BASE_URL}/")
        assert response.status_code == 200
        assert "<h1>Unit Test Index</h1>" in response.text

    def test_get_specific_file(self):
        """[Positive] Obtaining a specific file."""
        css_content = "body { color: red; }"
        with open(os.path.join(TEST_DIR, "style.css"), "w") as f:
            f.write(css_content)
            
        response = requests.get(f"{BASE_URL}/style.css")
        assert response.status_code == 200
        assert response.text == css_content
        
        os.remove(os.path.join(TEST_DIR, "style.css"))

    def test_upload_file_post(self):
        """[Positive] Uploading a file via POST."""
        for f in os.listdir(TEST_UPLOAD_DIR):
            os.remove(os.path.join(TEST_UPLOAD_DIR, f))

        binary_data = b'\xDE\xAD\xBE\xEF' * 100
        headers = {"Content-Length": str(len(binary_data))}
        
        response = requests.post(BASE_URL, data=binary_data, headers=headers)
        
        assert response.status_code == 201

        uploaded_files = os.listdir(TEST_UPLOAD_DIR)
        assert len(uploaded_files) == 1, f"Expected 1 file, found {len(uploaded_files)}: {uploaded_files}"
        
        filepath = os.path.join(TEST_UPLOAD_DIR, uploaded_files[0])
        file_size = os.path.getsize(filepath)
        
        assert file_size == len(binary_data), f"File size mismatch! Expected {len(binary_data)}, got {file_size}"

    def test_delete_existing_file(self):
        """[Positive] Deleting a file."""
        filename = "to_delete.txt"
        filepath = os.path.join(TEST_DIR, filename)
        with open(filepath, "w") as f:
            f.write("delete me")

        response = requests.delete(f"{BASE_URL}/{filename}")
        assert response.status_code == 200
        assert not os.path.exists(filepath)


# ==========================================
#      NEGATIVE SCENARIOS (Error Handling)
# ==========================================
class TestNegativeScenarios:

    def test_get_non_existent_file(self):
        """[Negative] 404 Not Found."""
        response = requests.get(f"{BASE_URL}/ghost_file.html")
        assert response.status_code == 404

    def test_delete_non_existent_file(self):
        """[Negative] Deleting a non-existent file."""
        response = requests.delete(f"{BASE_URL}/missing.txt")
        assert response.status_code == 404

    def test_delete_root_directory(self):
        """[Negative] Prohibition of root removal."""
        response = requests.delete(f"{BASE_URL}/")
        assert response.status_code == 400

    def test_method_not_implemented(self):
        """[Negative] 501 Not Implemented."""
        response = requests.put(f"{BASE_URL}/index.html", data="update")
        assert response.status_code == 501

    def test_directory_traversal_attack(self):
        """[Negative] Directory Traversal."""
        s = requests.Session()
        req = requests.Request('GET', f"{BASE_URL}/../Makefile")
        prepped = req.prepare()
        prepped.url = f"{BASE_URL}/../Makefile"
        
        response = s.send(prepped)
        assert response.status_code == 403

    def test_post_without_content_length(self):
        """[Negative] POST без Content-Length."""
        s = requests.Session()
        req = requests.Request('POST', BASE_URL)
        prepped = req.prepare()
        if 'Content-Length' in prepped.headers:
            del prepped.headers['Content-Length']
            
        response = s.send(prepped)
        assert response.status_code == 400