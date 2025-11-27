#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

void print_ip_address(struct sockaddr *addr) {
    char ipstr[INET6_ADDRSTRLEN];
    void *addr_ptr;

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)addr;
        addr_ptr = &(ipv4->sin_addr);
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addr;
        addr_ptr = &(ipv6->sin6_addr);
    } else {
        return;
    }

    inet_ntop(addr->sa_family, addr_ptr, ipstr, sizeof(ipstr));
    printf("IP Address: %s\n", ipstr);
}

void print_hostname(struct sockaddr *addr, socklen_t addrlen) {
    char host[1024];

    int s = getnameinfo(addr, addrlen, host, sizeof(host), NULL, 0, NI_NAMEREQD);

    if (s == 0) {
        printf("Hostname: %s\n", host);
    } else {
        printf("Hostname: [Not found] (PTR record missing)\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname or ip>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input = argv[1];
    struct addrinfo hints, *result, *rp;
    int s;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICHOST;

    s = getaddrinfo(input, NULL, &hints, &result);

    if (s == 0) {
        
        printf("Detected input as IP address.\nResolving hostname...\n");

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            print_hostname(rp->ai_addr, rp->ai_addrlen);
        }
        freeaddrinfo(result);

    } else {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        s = getaddrinfo(input, NULL, &hints, &result);
        if (s != 0) {
            fprintf(stderr, "Could not resolve '%s': %s\n", input, gai_strerror(s));
            return EXIT_FAILURE;
        }

        printf("Detected input as Hostname.\nResolving IPs...\n");

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            print_ip_address(rp->ai_addr);
        }
        freeaddrinfo(result);
    }

    return EXIT_SUCCESS;
}