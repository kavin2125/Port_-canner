#ifndef SCANNER_H
#define SCANNER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/* Port state enum */
typedef enum {
    PORT_CLOSED = 0,
    PORT_OPEN,
    PORT_FILTERED
} port_status_t;

/* Structure for single port result */
typedef struct {
    int port;
    port_status_t status;
} port_result_t;

/* Scan result structure */
typedef struct {
    char target[256];
    int start_port;
    int end_port;
    int total_scanned;
    int open_count;
    port_result_t *results;
} scan_result_t;


int validate_input(char *target, int start_port, int end_port);
void scan_ports(scan_result_t *target);

#endif
