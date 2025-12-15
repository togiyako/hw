
## About project ##

This project is an implementation of a multi-threaded HTTP web server in the C programming language. 
The server is designed to handle basic HTTP requests, support large file transfers, maintain logs, and work with configuration files.
The architecture is based on the “Process-per-Client” model.

Key features:
- HTTP Methods: support for GET (read), POST (upload), DELETE (delete).
- Concurrency: sandling multiple clients simultaneously via fork().
- Keep-Alive: support for persistent connections to reduce the overhead of TCP handshakes.
- Large Files: stream data transfer (stream I/O) without fully loading it into RAM.
- Configuration: flexible configuration via the server.conf file.
- Logging: detailed event logging with different levels (DEBUG, INFO, ERROR, FATAL).


##  Process Architecture
```mermaid
graph TD
    Client1[Client A] -->|TCP Connect| Master[Master Server Process]
    Client2[Client B] -->|TCP Connect| Master
    
    Master -- fork() --> Worker1[Worker Process PID: 101]
    Master -- fork() --> Worker2[Worker Process PID: 102]
    
    Worker1 <-->|HTTP Request/Response| Client1
    Worker2 <-->|HTTP Request/Response| Client2
    
    Worker1 -->|Read/Write| FS[(File System / Storage)]
    Worker2 -->|Read/Write| FS
    
    Worker1 -.->|Log| Logger[Server Log File]
```
## Request Processing Flow

```mermaid
sequenceDiagram
    participant Client
    participant Worker
    participant FileSystem

    Client->>Worker: HTTP Request (GET /index.html)
    Worker->>Worker: Parse Headers & Method
    
    alt Method is GET
        Worker->>FileSystem: Read File
        FileSystem-->>Worker: File Stream / 404 Error
        Worker->>Client: HTTP 200 OK + File Content
    else Method is POST
        Worker->>FileSystem: Create/Write File
        Client->>Worker: Stream Data Body
        Worker->>FileSystem: Save Chunked Data
        Worker->>Client: HTTP 201 Created
    else Method is DELETE
        Worker->>FileSystem: Remove File
        FileSystem-->>Worker: Success / Error
        Worker->>Client: HTTP 200 OK / 404 Not Found
    end
```
## Build and Run ##

Project compilation:

make

Starting the server:

./main

Cleaning:

make clean

## Testing ##

pytest -v ./tests/test_server.py
