#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "server.h"
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdarg.h>

static char LOG_FILE_PATH[256] = "";

void logger_init(const char *filename) {
    if (filename && strlen(filename) > 0) {
        strncpy(LOG_FILE_PATH, filename, sizeof(LOG_FILE_PATH) - 1);
    }
}

void log_message(LogLevel level, const char *file, int line, const char *format, ...) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    const char *level_str;
    const char *color_code;
    
    switch (level) {
        case LOG_FATAL: level_str = "FATAL"; color_code = "\033[1;31m"; break;
        case LOG_ERROR: level_str = "ERROR"; color_code = "\033[0;31m"; break;
        case LOG_WARN:  level_str = "WARN";  color_code = "\033[0;33m"; break;
        case LOG_INFO:  level_str = "INFO";  color_code = "\033[0;32m"; break;
        case LOG_DEBUG: level_str = "DEBUG"; color_code = "\033[0;36m"; break;
        default:        level_str = "UNKNOWN"; color_code = "\033[0m"; break;
    }
    const char *reset_code = "\033[0m";

    char message[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    printf("%s[%s] [%s] %s (%s:%d)%s\n", 
           color_code, time_str, level_str, message, file, line, reset_code);

    if (strlen(LOG_FILE_PATH) > 0) {
        FILE *f = fopen(LOG_FILE_PATH, "a");
        if (f) {
            fprintf(f, "[%s] [%s] %s (%s:%d)\n", time_str, level_str, message, file, line);
            fclose(f);
        }
    }
}

void send_response(int socket, HttpStatusCode status_code, char *content_type, char *body) {
    char header[BUFFER_SIZE];
    char *status_text;

    switch (status_code) {
        case HTTP_OK: status_text = "OK"; break;
        case HTTP_CREATED: status_text = "Created"; break;
        case HTTP_BAD_REQUEST: status_text = "Bad Request"; break;
        case HTTP_FORBIDDEN: status_text = "Forbidden"; break;
        case HTTP_NOT_FOUND: status_text = "Not Found"; break;
        case HTTP_INTERNAL_SERVER_ERROR: status_text = "Internal Server Error"; break;
        case HTTP_NOT_IMPLEMENTED: status_text = "Not Implemented"; break;
        case HTTP_SERVICE_UNAVAILABLE: status_text = "Service Unavailable"; break;
        default: status_text = "Unknown"; break;
    }

    int len = sprintf(header, 
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %lu\r\n"
        "\r\n",
        status_code, status_text, content_type, body ? strlen(body) : 0);

    if (write(socket, header, len) < 0) return;

    if (body) {
        write(socket, body, strlen(body));
    }
}

void send_file_stream(int socket, const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        LOG_ERROR("Cannot open file: %s", filepath);
        send_response(socket, HTTP_NOT_FOUND, "text/html", "<html><body><h1>404 Not Found</h1></body></html>");
        return;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0) {
        close(fd);
        return;
    }

    char header[1024];
    int len = sprintf(header, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        file_stat.st_size);
    
    if (write(socket, header, len) < 0) {
        close(fd);
        return;
    }

    char file_buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, file_buffer, sizeof(file_buffer))) > 0) {
        if (write(socket, file_buffer, bytes_read) < 0) {
            break;
        }
    }

    close(fd);
}

void handle_upload(int socket, long content_length, const char *filename, char *initial_data, int initial_len) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        LOG_ERROR("Failed to open file for writing: %s", filename);
        send_response(socket, HTTP_INTERNAL_SERVER_ERROR, "text/html", "<html><body><h1>500 Error</h1></body></html>");
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
        
        if (bytes_read < 0) {
             LOG_ERROR("Error reading socket during upload");
             break;
        }
        if (bytes_read == 0) {
             LOG_WARN("Client closed connection prematurely");
             break;
        }

        fwrite(buffer, 1, bytes_read, f);
        total_written += bytes_read;
    }

    fclose(f);
    LOG_INFO("File uploaded: %s (%ld bytes)", filename, total_written);
    send_response(socket, HTTP_CREATED, "text/plain", "File Uploaded Successfully");
}

struct Server server_Constructor(ServerConfig config, void (*launch)(struct Server *server)) {
    struct Server server;
    server.config = config;
    server.launch = launch;

    server.address.sin_family = AF_INET;
    server.address.sin_port = htons(config.port);
    
    if (inet_pton(AF_INET, config.ip_address, &server.address.sin_addr) <= 0) {
        LOG_ERROR("Invalid address: %s", config.ip_address);
        server.address.sin_addr.s_addr = INADDR_ANY; 
    }

    server.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server.socket < 0) {
        LOG_FATAL("Failed to initialize socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        LOG_FATAL("Bind failed on port %d", config.port);
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, config.backlog) < 0) {
        LOG_FATAL("Listen failed");
        exit(EXIT_FAILURE);
    }

    return server;
}

void launch(struct Server *server) {
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(server->address);

    logger_init(server->config.log_file);
    printf("=== SERVER STARTED on %s:%d ===\n", server->config.ip_address, server->config.port);

    while (1) {
        int new_socket = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);
        if (new_socket < 0) continue;

        pid_t pid = fork();

        if (pid < 0) {
        LOG_ERROR("Failed to fork process");
        send_response(new_socket, HTTP_SERVICE_UNAVAILABLE, "text/html", "<h1>503 Service Unavailable</h1>");
        close(new_socket);
        continue;
        }

        if (pid == 0) {
            close(server->socket);

            struct timeval tv;
            tv.tv_sec = server->config.keep_alive_timeout;
            tv.tv_usec = 0;
            setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

            while(1) {
                memset(buffer, 0, BUFFER_SIZE);

                ssize_t bytesRead = read(new_socket, buffer, BUFFER_SIZE - 1);
                if (bytesRead <= 0) break; 
                
                buffer[bytesRead] = '\0';

                char method[16] = {0}, path[256] = {0}, proto[16] = {0};
                if (sscanf(buffer, "%s %s %s", method, path, proto) < 2) break;

                LOG_INFO("[PID:%d] Request: %s %s", getpid(), method, path);

                if (strcmp(method, "GET") == 0) {
                    char file_path[512];
                    if (strcmp(path, "/") == 0)
                        snprintf(file_path, sizeof(file_path), "%s/index.html", server->config.root_dir);
                    else {
                        if (strstr(path, "..")) {
                            send_response(new_socket, HTTP_FORBIDDEN, "text/html", "<html><body><h1>403 Forbidden</h1></body></html>");
                            continue;
                        }
                        snprintf(file_path, sizeof(file_path), "%s%s", server->config.root_dir, path);
                    }
                    send_file_stream(new_socket, file_path);
                } 
                else if (strcmp(method, "POST") == 0) {
                    long content_len = 0;
                    char *len_str = strstr(buffer, "Content-Length: ");
                    if (len_str) sscanf(len_str, "Content-Length: %ld", &content_len);

                    char *body_start = strstr(buffer, "\r\n\r\n");
                    int initial_body_len = 0;
                    if (body_start) {
                        body_start += 4;
                        initial_body_len = bytesRead - (body_start - buffer);
                    } else {
                        body_start = NULL; 
                    }

                    if (content_len > 0) {
                        char save_path[512];
                        snprintf(save_path, sizeof(save_path), "%s/upload_%d_%ld.bin", 
                            server->config.storage_dir, getpid(), time(NULL));
                        
                        handle_upload(new_socket, content_len, save_path, body_start, initial_body_len);
                    } else {
                        send_response(new_socket, HTTP_BAD_REQUEST, "text/html", "<html><body><h1>400 No Content-Length</h1></body></html>");
                    }
                }
                else if (strcmp(method, "DELETE") == 0) {
                    char file_path[512];
                    if (strcmp(path, "/") == 0) {
                         send_response(new_socket, HTTP_BAD_REQUEST, "text/html", "<html><body><h1>Cannot delete root</h1></body></html>");
                    } else {
                        if (strstr(path, "..")) {
                            send_response(new_socket, HTTP_FORBIDDEN, "text/html", "<html><body><h1>403 Forbidden</h1></body></html>");
                            continue; 
                        }
                        snprintf(file_path, sizeof(file_path), "%s%s", server->config.root_dir, path);
                        if (remove(file_path) == 0) {
                            send_response(new_socket, HTTP_OK, "text/html", "<html><body><h1>File Deleted</h1></body></html>");
                        } else {
                            if (errno == ENOENT)
                                send_response(new_socket, HTTP_NOT_FOUND, "text/html", "<html><body><h1>404 Not Found</h1></body></html>");
                            else
                                send_response(new_socket, HTTP_FORBIDDEN, "text/html","<html><body><h1>403 Forbidden</h1></body></html>");
                        }
                    }
                }
                else {
                    send_response(new_socket, HTTP_NOT_IMPLEMENTED, "text/html", "<html><body><h1>501 Not Implemented</h1></body></html>");
                }
                if (strstr(buffer, "Connection: close")) {
                    break;
                }
            }
            close(new_socket);
            exit(0);
        } 
        else {
            close(new_socket);
        }
    }
}

ServerConfig load_config(const char *filename) {
    ServerConfig config;
    config.port = 8080;
    config.backlog = 10;
    config.keep_alive_timeout = 5;
    strcpy(config.root_dir, ".");
    strcpy(config.storage_dir, "./uploads");
    strcpy(config.ip_address, "0.0.0.0");
    strcpy(config.log_file, "");

    FILE *f = fopen(filename, "r");
    if (!f) return config;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        char key[50], val[200];
        if (sscanf(line, "%[^=]=%s", key, val) == 2) {
            if (strcmp(key, "port") == 0) config.port = atoi(val);
            if (strcmp(key, "root_dir") == 0) strcpy(config.root_dir, val);
            if (strcmp(key, "storage_dir") == 0) strcpy(config.storage_dir, val);
            if (strcmp(key, "ip") == 0) strcpy(config.ip_address, val);
            if (strcmp(key, "max_clients") == 0) config.backlog = atoi(val);
            if (strcmp(key, "log_file") == 0) strcpy(config.log_file, val);
            if (strcmp(key, "keep_alive_timeout") == 0) config.keep_alive_timeout = atoi(val);
        }
    }
    fclose(f);
    return config;
}