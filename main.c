#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <target_ip> <start_port> <end_port>\n", argv[0]);
        return 1;
    }
scan_result_t result;

strncpy(result.target, argv[1], sizeof(result.target) - 1);
result.start_port = atoi(argv[2]);
result.end_port = atoi(argv[3]);
scan_ports(&result);

for (int i = 0; i < result.total_scanned; i++) {

    if (result.results[i].status == PORT_FILTERED)
        continue;   // 🚀 Skip filtered ports

    if (result.results[i].status == PORT_OPEN)
        printf("Port %d: OPEN\n", result.results[i].port);

    else if (result.results[i].status == PORT_CLOSED)
        printf("Port %d: CLOSED\n", result.results[i].port);
}
printf("\nTotal scanned: %d\n", result.total_scanned);
printf("Open ports: %d\n", result.open_count);
free(result.results);
}
