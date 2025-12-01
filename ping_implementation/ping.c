#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <getopt.h>

#define DEFAULT_PKT_SIZE 64
#define DEFAULT_TTL 64
#define RECV_TIMEOUT 2

volatile int keep_running = 1;

struct ping_pkt {
    struct icmphdr hdr;
    char msg[];
};

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void intHandler(int dummy) {
    keep_running = 0;
}

char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con) {
    struct hostent *host_entity;
    
    if ((host_entity = gethostbyname(addr_host)) == NULL) {
        return NULL;
    }

    addr_con->sin_family = host_entity->h_addrtype;
    addr_con->sin_port = htons(0);
    addr_con->sin_addr.s_addr = *(long *)host_entity->h_addr;

    return inet_ntoa(*(struct in_addr *)host_entity->h_addr);
}

void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_dom, char *ping_ip, int count, int ttl_val, int data_size) {
    
    int seq_id = 0;
    struct sockaddr_in r_addr;
    socklen_t addr_len;
    struct timespec time_start, time_end;
    long double rtt_msec = 0;

    if (setsockopt(ping_sockfd, IPPROTO_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0) {
        perror("Setting TTL failed");
        return;
    }

    struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;
    setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);

    int packet_len = sizeof(struct icmphdr) + data_size;
    struct ping_pkt *pckt = malloc(packet_len);
    if (!pckt) {
        perror("Malloc failed");
        return;
    }

    char *rbuffer = malloc(2048); 
    if (!rbuffer) {
        free(pckt);
        perror("Malloc buffer failed");
        return;
    }

    while (keep_running) {
        if (count != -1 && seq_id >= count) break;

        bzero(pckt, packet_len);
        pckt->hdr.type = ICMP_ECHO;
        pckt->hdr.un.echo.id = getpid();
        pckt->hdr.un.echo.sequence = seq_id++;
        
        for (int i = 0; i < data_size; i++)
            pckt->msg[i] = (i % 50) + '0';
        
        pckt->hdr.checksum = checksum(pckt, packet_len);

        printf("PING: sent %d bytes to %s (%s)\n", packet_len, ping_dom, ping_ip);

        clock_gettime(CLOCK_MONOTONIC, &time_start);
        
        if (sendto(ping_sockfd, pckt, packet_len, 0, (struct sockaddr*)ping_addr, sizeof(*ping_addr)) <= 0) {
            printf("\nPacket Sending Failed!\n");
        }

        addr_len = sizeof(r_addr);

        ssize_t bytes_recvd = recvfrom(ping_sockfd, rbuffer, 2048, 0, (struct sockaddr*)&r_addr, &addr_len);

        if (bytes_recvd <= 0) {
            printf("Timeout waiting for response. Terminating.\n");
            break;
        }

        clock_gettime(CLOCK_MONOTONIC, &time_end);

        struct iphdr *ip = (struct iphdr *)rbuffer;
        int ip_header_len = ip->ihl * 4;
        
        struct icmphdr *icmp = (struct icmphdr *)(rbuffer + ip_header_len);

        if (icmp->type == ICMP_ECHOREPLY && icmp->un.echo.id == getpid()) {
            
            double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
            rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + timeElapsed;

            int recv_ttl = ip->ttl;

            printf("PONG: received %ld bytes from %s (%s), ttl=%d, round-trip time %.2Lf ms\n", 
                   bytes_recvd - ip_header_len, ping_dom, ping_ip, recv_ttl, rtt_msec);
        }

        usleep(1000000);
    }

    free(pckt);
    free(rbuffer);
}

int main(int argc, char *argv[]) {
    int sockfd;
    char *ip_addr;
    struct sockaddr_in addr_con;
    char *hostname = NULL;

    int count = -1;
    int ttl = DEFAULT_TTL;
    int data_size = DEFAULT_PKT_SIZE;

    int opt;
    while ((opt = getopt(argc, argv, "c:t:s:")) != -1) {
        switch (opt) {
            case 'c': count = atoi(optarg); break;
            case 't': ttl = atoi(optarg); break;
            case 's': data_size = atoi(optarg); break;
            default:
                fprintf(stderr, "Usage: %s <hostname> [-c count] [-t ttl] [-s packet_size]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Expected hostname after options\n");
        fprintf(stderr, "Usage: %s [-c count] [-t ttl] [-s packet_size] <hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    hostname = argv[optind];

    if (data_size < 0 || data_size > 1024) data_size = DEFAULT_PKT_SIZE;

    ip_addr = dns_lookup(hostname, &addr_con);
    if (ip_addr == NULL) {
        printf("\nDNS lookup failed! Could not resolve hostname!\n");
        return 0;
    }

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Socket creation failed (Root privileges required?)");
        return 0;
    }

    signal(SIGINT, intHandler);

    send_ping(sockfd, &addr_con, hostname, ip_addr, count, ttl, data_size);
    
    close(sockfd);
    return 0;
}