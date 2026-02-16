#ifndef SOCKETS_H
#define SOCKETS_H
#include "address_info.h"

#define SOCKET_DESCRIPTORS_BY_ID_CAPACITY 100

typedef enum SocketResultCode {
    SOCKET_RESULT_OK = 64U,
    SOCKET_RESULT_ADDRESS_INFO_NOT_FOUND,
    SOCKET_RESULT_CAPACITY_REACHED_PLEASE_CLOSE_AT_LEAST_ONE_SOCKET
} SocketResultCode;

int network_lib__create_socket_descriptor(int address_info_id, SocketResultCode* code);

void network_lib__close_all_sockets();

#endif // SOCKETS_H