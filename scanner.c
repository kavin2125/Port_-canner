#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include "scanner.h"
void scan_ports(scan_result_t *result);
port_status_t try_connect(const char *target, int port);

port_status_t try_connect(const char *target, int port)
{
    int sockfd;
    struct sockaddr_in addr;
    fd_set fdset;
    struct timeval timeout;
    int flags, res, so_error;
    socklen_t len;

    // 1️⃣ Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return PORT_CLOSED;

    // 2️⃣ Make socket non-blocking
    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    // 3️⃣ Setup address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, target, &addr.sin_addr) <= 0) {
        close(sockfd);
        return PORT_CLOSED;
    }

    // 4️⃣ Start connection
    res = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    if (res < 0) {
        if (errno != EINPROGRESS) {
            close(sockfd);
            return PORT_CLOSED;
        }
    }

    // 5️⃣ Wait with timeout
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);

    timeout.tv_sec = 0;   // 1 second timeout
    timeout.tv_usec = 200000;

    res = select(sockfd + 1, NULL, &fdset, NULL, &timeout);

    if (res == 1) {
        // 6️⃣ Check socket error
        len = sizeof(so_error);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);

        close(sockfd);

        if (so_error == 0)
            return PORT_OPEN;
        else
            return PORT_CLOSED;
    }
    else {
        close(sockfd);
        return PORT_FILTERED; // timeout
    }
}
void scan_ports(scan_result_t *result)
{
    int total_ports = result->end_port - result->start_port + 1;

    result->results = malloc(total_ports * sizeof(port_result_t));
    if (result->results == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
}
    result->total_scanned = 0;
    result->open_count = 0;

    for (int port = result->start_port; port <= result->end_port; port++)
    {
        port_status_t status = try_connect(result->target, port);

        result->results[result->total_scanned].port = port;
        result->results[result->total_scanned].status = status;

        if (status == PORT_OPEN)
            result->open_count++;

        result->total_scanned++;
    }
}
