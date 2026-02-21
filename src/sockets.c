#include "pch.h"
#include "sockets.h"

static int open_socket_descriptors[SOCKET_DESCRIPTORS_BY_ID_CAPACITY] = {-1};

extern struct addrinfo* network_lib__get_bsd_address_info_results_using_id(int address_info_id, AddressInfoResultCode* code);
extern void network_lib__delete_address_info(int address_info_id);

int network_lib__open_socket(int address_info_id, SocketOpenResultCode* code)  {

    AddressInfoResultCode addr_code;
    struct addrinfo* address_info = network_lib__get_bsd_address_info_results_using_id(address_info_id, &addr_code);
    if (address_info == NULL) {
        if (code != NULL)
            *code = SOCKET_OPEN_RESULT_ADDRESS_INFO_NOT_FOUND;
        return -1;
    }
    int current_id = 0;
    while (current_id < SOCKET_DESCRIPTORS_BY_ID_CAPACITY) {
        if (open_socket_descriptors[current_id] != -1) {
            current_id++;
            continue;
        }
        int socket_descriptor = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
        open_socket_descriptors[current_id] = socket_descriptor;
        if (code != NULL) *code = SOCKET_OPEN_RESULT_OK;
        return current_id;
    }
    if (code != NULL) *code = SOCKET_OPEN_RESULT_CAPACITY_REACHED_PLEASE_CLOSE_AT_LEAST_ONE_SOCKET;
    return -1;
}

int network_lib__open_socket_and_delete_address_info(int address_info_id, SocketOpenResultCode* code) {
    int socket_descriptor_id = network_lib__open_socket(address_info_id, code);
    network_lib__delete_address_info(address_info_id);
    return socket_descriptor_id;
}

void network_lib__close_all_sockets() {
    for (int i = 0; i < SOCKET_DESCRIPTORS_BY_ID_CAPACITY; i++) {
        network_lib__close_socket(i);
    }
}

SocketStatusCode network_lib__socket_descriptor_status(int socket_descriptor_id) {
    if (open_socket_descriptors[socket_descriptor_id] == -1) {
        return SOCKET_STATUS_CLOSED;
    }
    return SOCKET_STATUS_OPEN;
}

void network_lib__close_socket(int socket_descriptor_id) {
    if (socket_descriptor_id < 0 || socket_descriptor_id >= SOCKET_DESCRIPTORS_BY_ID_CAPACITY) return;
    if (open_socket_descriptors[socket_descriptor_id] == -1) return;
    close(open_socket_descriptors[socket_descriptor_id]);
    open_socket_descriptors[socket_descriptor_id] = -1;
}

BindResultCode network_lib__bind_socket(int socket_descriptor_id) {
    return BIND_RESULT_UNKNOWN_ERROR;
}

