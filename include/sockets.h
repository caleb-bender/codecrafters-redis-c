#ifndef SOCKETS_H
#define SOCKETS_H
#include "address_info.h"

typedef enum SocketResultCode {
    SOCKET_RESULT_OK = 64U,
    SOCKET_RESULT_ADDRESS_INFO_NOT_FOUND
} SocketResultCode;

int network_lib__create_socket_descriptor(int address_info_id, SocketResultCode* code);

#endif // SOCKETS_H