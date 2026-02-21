#ifndef SOCKETS_H
#define SOCKETS_H
#include "address_info.h"

#define SOCKET_DESCRIPTORS_BY_ID_CAPACITY 100

typedef enum SocketResultCode {
    SOCKET_RESULT_OK = 64U,
    SOCKET_RESULT_ADDRESS_INFO_NOT_FOUND,
    SOCKET_RESULT_CAPACITY_REACHED_PLEASE_CLOSE_AT_LEAST_ONE_SOCKET
} SocketResultCode;

typedef enum SocketStatusCode {
    SOCKET_STATUS_EXISTS_AND_IS_OPEN = 128U,
    SOCKET_STATUS_EXISTS_BUT_CLOSED,
    SOCKET_STATUS_NOT_FOUND_OR_CLOSED
} SocketStatusCode;

int network_lib__create_socket_descriptor(int address_info_id, SocketResultCode* code);

SocketStatusCode network_lib__socket_descriptor_status(int socket_descriptor_id);

void network_lib__close_last_socket();

void network_lib__close_all_sockets();

#endif // SOCKETS_H