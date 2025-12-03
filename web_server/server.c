#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "server.h"

void send_response(int socket, int status_code, char *content_type, char *body) {
    char header[BUFFER_SIZE];
    char *status_text;

    switch (status_code) {
        case 200: status_text = "OK"; break;
        case 400: status_text = "Bad Request"; break;
        case 403: status_text = "Forbidden"; break;
        case 404: status_text = "Not Found"; break;
        case 501: status_text = "Not Implemented"; break;
        case 503: status_text = "Service Unavailable"; break;
        default:  status_text = "Internal Server Error"; break;
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

struct Server server_Constructor(int domain, int port, int service, int protocol, int backlog, unsigned long interface, void (*launch)(struct Server *server)) {
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.port = port;
    server.protocol = protocol;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);
    if (server.socket < 0) {
        perror("Failed to initialize/connect to socket...\n");
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket...\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening...\n");
        exit(EXIT_FAILURE);
    }

    server.launch = launch;
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
        sscanf(buffer, "%s %s %s", method, path, proto);

        printf("Request: %s %s\n", method, path);

        if (strcmp(method, "GET") == 0) {
            char file_path[300];

            if (strcmp(path, "/") == 0) {
                sprintf(file_path, "index.html"); 
            } else {
                sprintf(file_path, "%s", path + 1);
            }

            char *file_content = read_file(file_path);
            
            if (file_content) {
                send_response(new_socket, 200, "text/html", file_content);
                free(file_content);
            } else {
                send_response(new_socket, 404, "text/html", "<html><body><h1>404 Not Found</h1></body></html>");
            }
        } 
        else if (strcmp(method, "POST") == 0) {
             send_response(new_socket, 200, "text/html", "<html><body><h1>POST Request Received</h1></body></html>");
        } 
        else if (strcmp(method, "DELETE") == 0) {
             send_response(new_socket, 200, "text/html", "<html><body><h1>DELETE executed (simulation)</h1></body></html>");
        }
        
        close(new_socket);
    }
}
