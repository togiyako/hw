#ifndef server_h
#define server_h
#include <netinet/in.h>
#define BUFFER_SIZE 16000

typedef enum {
    LOG_FATAL = 0,
    LOG_ERROR = 1,
    LOG_WARN  = 2,
    LOG_INFO  = 3,
    LOG_DEBUG = 4
} LogLevel;

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
    int backlog;
    char root_dir[256];
    char storage_dir[256];
    char ip_address[32];
    char log_file[256];
    int keep_alive_timeout;
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

void logger_init(const char *filename);
void log_message(LogLevel level, const char *file, int line, const char *format, ...);

#define LOG_FATAL(fmt, ...) log_message(LOG_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) log_message(LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  log_message(LOG_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  log_message(LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) log_message(LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif