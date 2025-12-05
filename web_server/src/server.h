#ifndef server_h
#define server_h

#include <netinet/in.h>
#define BUFFER_SIZE 16000

typedef enum {
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_BAD_REQUEST = 400,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED = 501,
    HTTP_SERVICE_UNAVAILABLE = 503
} HttpStatusCode;

typedef struct{
    int port;
    int backlock;
    char root_dir[256];
    char storage_dir[256];
} ServerConfig;

struct Server {
    ServerConfig config;
    int socket;
    struct sockaddr_in address;

    void (*launch)(struct Server *server);
};

struct Server server_Constructor(ServerConfig config, void (*launch)(struct Server *server));
void launch(struct Server *server);
ServerConfig load_config(const char *filename);
void send_response(int socket, HttpStatusCode status_code, char *content_type, char *body);
void handle_upload(int socket, long content_length, const char *filename, char *initial_data, int initial_len);

#endif