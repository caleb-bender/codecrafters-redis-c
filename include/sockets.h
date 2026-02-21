#ifndef SOCKETS_H
#define SOCKETS_H
#include "address_info.h"

#define SOCKET_DESCRIPTORS_BY_ID_CAPACITY 100

typedef enum SocketOpenResultCode {
    SOCKET_OPEN_RESULT_OK = 64U,
    SOCKET_OPEN_RESULT_ADDRESS_INFO_NOT_FOUND,
    SOCKET_OPEN_RESULT_CAPACITY_REACHED_PLEASE_CLOSE_AT_LEAST_ONE_SOCKET
} SocketOpenResultCode;

typedef enum SocketStatusCode {
    SOCKET_STATUS_OPEN = 128U,
    SOCKET_STATUS_CLOSED
} SocketStatusCode;

typedef enum BindResultCode {
    BIND_RESULT_OK = 192U,
    BIND_RESULT_SOCKET_DESCRIPTOR_NOT_FOUND,
    BIND_RESULT_ADDRESS_ALREADY_IN_USE,
    BIND_RESULT_UNKNOWN_ERROR
} BindResultCode;

typedef enum ReuseAddressOption {
    REUSE_ADDRESS_OPTION_OFF = 0U,
    REUSE_ADDRESS_OPTION_ON
} ReuseAddressOption;

int network_lib__open_socket(int address_info_id, SocketOpenResultCode* code);
int network_lib__open_socket_and_delete_address_info(int address_info_id, SocketOpenResultCode* code);

SocketStatusCode network_lib__socket_descriptor_status(int socket_descriptor_id);

void network_lib__close_all_sockets();

void network_lib__close_socket(int socket_descriptor_id);

BindResultCode network_lib__bind_socket(int socket_descriptor_id);

#endif // SOCKETS_H