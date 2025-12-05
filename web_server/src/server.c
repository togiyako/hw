#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "server.h"
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void send_response(int socket, HttpStatusCode status_code, char *content_type, char *body) {
    char header[BUFFER_SIZE];
    char *status_text;

    switch (status_code) {
        case HTTP_OK: 
            status_text = "OK"; 
            break;
        case HTTP_CREATED: 
            status_text = "Created"; 
            break;
        case HTTP_BAD_REQUEST: 
            status_text = "Bad Request"; 
            break;
        case HTTP_FORBIDDEN: 
            status_text = "Forbidden"; 
            break;
        case HTTP_NOT_FOUND: 
            status_text = "Not Found"; 
            break;
        case HTTP_INTERNAL_SERVER_ERROR: 
            status_text = "Internal Server Error"; 
            break;
        case HTTP_NOT_IMPLEMENTED: 
            status_text = "Not Implemented"; 
            break;
        case HTTP_SERVICE_UNAVAILABLE: 
            status_text = "Service Unavailable"; 
            break;
        default:  
            status_text = "Unknown"; 
            break;
    }

    int len = sprintf(header, 
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %lu\r\n"
        "\r\n",
        status_code, status_text, content_type, body ? strlen(body) : 0);

    write(socket, header, len);

    if (body) {
        write(socket, body, strlen(body));
    }
}


char *read_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, f);
        buffer[length] = '\0';
    }
    fclose(f);
    return buffer;
}

void send_file_stream(int socket, const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        send_response(socket, HTTP_NOT_FOUND, "text/plain", "File not found");
        return;
    }

    struct stat file_stat;
    fstat(fd, &file_stat);

    char header[1024];
    int len = sprintf(header, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        file_stat.st_size);
    
    write(socket, header, len);

    off_t offset = 0;
    ssize_t sent_bytes = 0;
    while (offset < file_stat.st_size) {
        sent_bytes = sendfile(socket, fd, &offset, file_stat.st_size - offset);
        if (sent_bytes <= 0) {
            break;
        }
    }

    close(fd);
}

void handle_upload(int socket, long content_length, const char *filename, char *initial_data, int initial_len) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        send_response(socket, HTTP_INTERNAL_SERVER_ERROR, "text/plain", "Cannot open file");
        return;
    }

    long total_written = 0;

    if (initial_len > 0) {
        fwrite(initial_data, 1, initial_len, f);
        total_written += initial_len;
    }

    char buffer[4096];
    ssize_t bytes_read;

    while (total_written < content_length) {
        bytes_read = read(socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) break;

        fwrite(buffer, 1, bytes_read, f);
        total_written += bytes_read;
    }

    fclose(f);
    send_response(socket, HTTP_CREATED, "text/plain", "File Uploaded Successfully");
}

struct Server server_Constructor(ServerConfig config, void (*launch)(struct Server *server)) {
    struct Server server;
    server.config = config;
    server.launch = launch;

    server.address.sin_family = AF_INET;
    server.address.sin_port = htons(config.port);
    server.address.sin_addr.s_addr = INADDR_ANY;

    server.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server.socket < 0) {
        perror("Failed to initialize/connect to socket...\n");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket...\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, config.backlock) < 0) {
        perror("Failed to start listening...\n");
        exit(EXIT_FAILURE);
    }

    return server;
}

void launch(struct Server *server) {
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("=== WAITING FOR CONNECTION === \n");
        int addrlen = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);

        ssize_t bytesRead = read(new_socket, buffer, BUFFER_SIZE - 1);
        if (bytesRead <= 0) {
            close(new_socket);
            continue;
        }
        buffer[bytesRead] = '\0';

        char method[16], path[256], proto[16];

        if (sscanf(buffer, "%s %s %s", method, path, proto) < 2) {
            printf("Invalid request received\n");
            close(new_socket);
            continue;
        }
        printf("[%s] Request: %s\n", method, path);

        if (strcmp(method, "GET") == 0) {
            char file_path[512];
            if (strcmp(path, "/") == 0) {
                sprintf(file_path, "%s/index.html", server->config.root_dir);
            } else {
                sprintf(file_path, "%s%s", server->config.root_dir, path);
            }
 
            send_file_stream(new_socket, file_path);
        } 
        else if (strcmp(method, "POST") == 0) {
            long content_len = 0;

            char *len_str = strstr(buffer, "Content-Length: ");
            if (len_str) {
                sscanf(len_str, "Content-Length: %ld", &content_len);
            }

            char *body_start = strstr(buffer, "\r\n\r\n");
            int initial_body_len = 0;
            
            if (body_start) {
                body_start += 4;
                initial_body_len = bytesRead - (body_start - buffer);
            } else {
                body_start = buffer + bytesRead; 
            }

            if (content_len > 0) {
                char save_path[512];
                sprintf(save_path, "%s/uploaded_file_%d.bin", server->config.storage_dir, new_socket);
                
                printf("Uploading %ld bytes to %s...\n", content_len, save_path);

                handle_upload(new_socket, content_len, save_path, body_start, initial_body_len);
            } else {
                send_response(new_socket, HTTP_BAD_REQUEST, "text/plain", "Missing Content-Length");
            }
        }
        else {
             send_response(new_socket, HTTP_NOT_IMPLEMENTED, "text/plain", "Method not allowed");
        }
        
        close(new_socket);
    }
}

ServerConfig load_config(const char *filename) {
    ServerConfig config;
    config.port = 8080;
    config.backlock = 10;
    strcpy(config.root_dir, ".");
    strcpy(config.storage_dir, "./storage");

    FILE *f = fopen(filename, "r");
    if (!f) return config;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char key[50], val[200];
        if (sscanf(line, "%[^=]=%s", key, val) == 2) {
            if (strcmp(key, "port") == 0) config.port = atoi(val);
            if (strcmp(key, "root_dir") == 0) strcpy(config.root_dir, val);
            if (strcmp(key, "storage_dir") == 0) strcpy(config.storage_dir, val);
        }
    }
    fclose(f);
    return config;
}