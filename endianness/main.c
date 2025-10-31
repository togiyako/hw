#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>
 
// Please implement a runtime check, do not use the predefined macros
bool is_little_endian() {
    u_int32_t i = 1;
    return (*(uint8_t*)&i == 1);
}

static uint32_t swap_bytes_32(uint32_t val) {

    uint32_t byte1 = (val >> 0)  & 0xFF;
    uint32_t byte2 = (val >> 8)  & 0xFF;
    uint32_t byte3 = (val >> 16) & 0xFF;
    uint32_t byte4 = (val >> 24) & 0xFF;
    
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4 << 0);
}

uint32_t my_htonl(uint32_t hostint) {

    if(!is_little_endian()){
        return hostint;
    }
    else{
        return swap_bytes_32(hostint);
    }
}
 
uint32_t my_ntohl(uint32_t netint) {
    
    if(!is_little_endian()){
        return netint;
    }
    else{
        return swap_bytes_32(netint);
    }
}
 
int main() {
    uint32_t i = 1947;
    printf("%s %s\n", "Test 1: ", i == my_ntohl(my_htonl(i)) ? "Success" : "Failure");
    printf("%s %s\n", "Test 2: ", i == ntohl(my_htonl(i)) ? "Success" : "Failure");
    printf("%s %s\n", "Test 3: ", i == my_ntohl(htonl(i)) ? "Success" : "Failure");
 
    return 0;
}