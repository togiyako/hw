#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>

struct {
    struct nlmsghdr nlh;
    struct ifinfomsg ifm;
} nl_req;

void request_link_list(int sock_fd) {
    memset(&nl_req, 0, sizeof(nl_req));

    nl_req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
    nl_req.nlh.nlmsg_type = RTM_GETLINK;

    nl_req.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nl_req.nlh.nlmsg_seq = 1;
    nl_req.nlh.nlmsg_pid = getpid();
    nl_req.ifm.ifi_family = AF_UNSPEC;

    if (send(sock_fd, &nl_req, nl_req.nlh.nlmsg_len, 0) < 0) {
        perror("send(RTM_GETLINK)");
    }
}

void parse_rtm_newlink(struct nlmsghdr *nlh, bool *initial_dump_done, time_t *last_msg_time) {
    struct ifinfomsg *ifimsg;
    struct rtattr *rta;
    int rta_len;
    char ifname[IF_NAMESIZE] = "";

    ifimsg = (struct ifinfomsg *)NLMSG_DATA(nlh);
    rta = IFLA_RTA(ifimsg);
    rta_len = IFLA_PAYLOAD(nlh);

    for (; RTA_OK(rta, rta_len); rta = RTA_NEXT(rta, rta_len)) {
        if (rta->rta_type == IFLA_IFNAME) {
            strncpy(ifname, (char *)RTA_DATA(rta), IF_NAMESIZE - 1);
            ifname[IF_NAMESIZE - 1] = '\0';
            break;
        }
    }

    if (strlen(ifname) == 0) {
        strcpy(ifname, "(unknown)");
    }

    if (!(*initial_dump_done)) {
        printf("Existing interface: %s (index: %d, flags: 0x%x)\n", 
               ifname, ifimsg->ifi_index, ifimsg->ifi_flags);
    } else {
        printf("\n*** New interface added ***\n");
        printf("Interface: %s (index: %d, flags: 0x%x)\n", 
               ifname, ifimsg->ifi_index, ifimsg->ifi_flags);
        printf("***************************\n");
        
        *last_msg_time = time(NULL);
    }
}

void parse_and_print_nlmsg(char *buf, ssize_t len, bool *initial_dump_done, time_t *last_msg_time) {
    struct nlmsghdr *nlh;

    for (nlh = (struct nlmsghdr *)buf; NLMSG_OK(nlh, len); nlh = NLMSG_NEXT(nlh, len)) {
        switch (nlh->nlmsg_type) {
            case NLMSG_DONE:
                if (!(*initial_dump_done)) {
                    *initial_dump_done = true;
                    printf("--- Finished fetching. Now listening for new interfaces ---\n");
                    *last_msg_time = time(NULL); 
                }
                break;

            case NLMSG_ERROR:
                {
                    struct nlmsgerr *err = (struct nlmsgerr *)NLMSG_DATA(nlh);
                    if (err->error == 0) {
                    } else {
                        fprintf(stderr, "Netlink error: %s\n", strerror(-err->error));
                    }
                }
                break;

            case RTM_NEWLINK:
                parse_rtm_newlink(nlh, initial_dump_done, last_msg_time);
                break;
            
            default:
                break;
        }
    }
}

int main() {
    int sock_fd;
    struct sockaddr_nl sa_nl;
    char buf[8192];
    ssize_t len;
    time_t last_msg_time = 0;
    bool initial_dump_done = false;

    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(sock_fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    if (fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL, O_NONBLOCK)");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    memset(&sa_nl, 0, sizeof(sa_nl));
    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_pid = getpid();
    sa_nl.nl_groups = RTMGRP_LINK;

    if (bind(sock_fd, (struct sockaddr*)&sa_nl, sizeof(sa_nl)) < 0) {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("--- Fetching existing interfaces ---\n");
    request_link_list(sock_fd);

    while (1) {
        
        if (initial_dump_done) {
            time_t current_time = time(NULL);
            if (difftime(current_time, last_msg_time) >= 10.0) {
                printf("listening for new interfaces\n");
                last_msg_time = current_time;
            }
        }

        len = recv(sock_fd, buf, sizeof(buf), 0);

        if (len < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                usleep(100000);
                continue;
            }
            perror("recv");
            break;
        }

        if (len > 0) {
            parse_and_print_nlmsg(buf, len, &initial_dump_done, &last_msg_time);
        }
    }

    close(sock_fd);
    return 0;
}

