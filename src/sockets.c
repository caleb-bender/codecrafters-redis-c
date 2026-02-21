#include "pch.h"
#include "sockets.h"

static int socket_descriptors[SOCKET_DESCRIPTORS_BY_ID_CAPACITY];
static int current_id = -1;


extern struct addrinfo* network_lib__get_bsd_address_info_results_using_id(int address_info_id, AddressInfoResultCode* code);

int network_lib__create_socket_descriptor(int address_info_id, SocketResultCode* code)  {

    if (current_id + 1 >= SOCKET_DESCRIPTORS_BY_ID_CAPACITY) {
        if (code != NULL) *code = SOCKET_RESULT_CAPACITY_REACHED_PLEASE_CLOSE_AT_LEAST_ONE_SOCKET;
        return -1;
    }
    AddressInfoResultCode addr_code;
    struct addrinfo* address_info = network_lib__get_bsd_address_info_results_using_id(address_info_id, &addr_code);
    if (address_info == NULL) {
        if (code != NULL)
            *code = SOCKET_RESULT_ADDRESS_INFO_NOT_FOUND;
        return -1;
    }
    int socket_descriptor = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
    socket_descriptors[++current_id] = socket_descriptor;
    if (code != NULL) *code = SOCKET_RESULT_OK;
    return current_id;
}

void network_lib__close_all_sockets() {
    for (int i = 0; i <= current_id; i++) {
        close(socket_descriptors[i]);
    }
    current_id = -1;
}

SocketStatusCode network_lib__socket_descriptor_status(int socket_descriptor_id) {
    if (socket_descriptor_id < 0 || socket_descriptor_id > current_id) {
        return SOCKET_STATUS_NOT_FOUND_OR_CLOSED;
    }
    return SOCKET_STATUS_EXISTS_BUT_CLOSED;
}

void network_lib__close_last_socket() {
    if (current_id == -1) return;
    close(socket_descriptors[current_id]);
    current_id--;
}

