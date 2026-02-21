#include "pch.h"
#include "address_info.h"

#define EXISTS_AND_IN_BOUNDS 255U

static struct addrinfo* address_infos_by_id[ADDRESS_INFOS_BY_ID_CAPACITY];
static int current_id = -1;

uint8_t get_validation_code_using_id(uint8_t validation_code, int id, AddressInfoResultCode* code);
void create_addrinfo_hints_from(struct addrinfo* hints, IPVersion version, SocketType socket_type, IPAssignmentMode ip_assignment_mode);

int network_lib__create_compatible_address_info(const char* hostname, const char* port,
    IPVersion version, SocketType socket_type, IPAssignmentMode ip_assignment_mode, AddressInfoResultCode* code) {
    if (current_id + 1 == ADDRESS_INFOS_BY_ID_CAPACITY) {
        *code = ADDRESS_INFO_CAPACITY_REACHED_PLEASE_CLEAR_STORE;
        return -1;
    }
    else if (ip_assignment_mode == CUSTOM_PROVIDED_IP && hostname == NULL) {
        *code = ADDRESS_INFO_HOSTNAME_REQUIRED;
        return -1;
    }
    struct addrinfo hints;
    struct addrinfo* results;
    create_addrinfo_hints_from(&hints, version, socket_type, ip_assignment_mode);
    int result_code = getaddrinfo(hostname, port, &hints, &results);
    if (result_code != 0) {
        switch (result_code) {
            case -9:
                *code = ADDRESS_INFO_INCOMPATIBLE_IP_VERSION_GIVEN_HOSTNAME;
                return -1;
            default:
                *code = ADDRESS_INFO_UNKNOWN_ERROR;
                return -1;
        }
    }
    address_infos_by_id[++current_id] = results;
    if (code != NULL)
        *code = ADDRESS_INFO_OK;
    return current_id;
}

IPVersion network_lib__get_address_info_ip_version(int id, AddressInfoResultCode* code) {

    uint8_t validation_code = get_validation_code_using_id(IP_ANY, id, code);
    if (validation_code != EXISTS_AND_IN_BOUNDS) return validation_code;
    struct addrinfo* result = address_infos_by_id[id];
    *code = ADDRESS_INFO_OK;
    if (result->ai_family == AF_INET)
        return IP_VERSION_4;
    else if (result->ai_family == AF_INET6)
        return IP_VERSION_6;
    return IP_ANY;
}

SocketType network_lib__get_address_info_socket_type(int id, AddressInfoResultCode* code) {

    uint8_t validation_code = get_validation_code_using_id(UNKNOWN_SOCKET, id, code);
    if (validation_code != EXISTS_AND_IN_BOUNDS) return validation_code;
    struct addrinfo* result = address_infos_by_id[id];
    *code = ADDRESS_INFO_OK;
    if (result->ai_socktype == SOCK_STREAM) return TCP_SOCKET;
    else if (result->ai_socktype == SOCK_DGRAM) return UDP_SOCKET;
    return UNKNOWN_SOCKET;
}

void create_addrinfo_hints_from(struct addrinfo* hints, IPVersion version, SocketType socket_type, IPAssignmentMode ip_assignment_mode) {
    memset(hints, 0, sizeof(struct addrinfo));
    if (version == IP_VERSION_4) {
        hints->ai_family = AF_INET;
    }
    else if (version == IP_VERSION_6) {
        hints->ai_family = AF_INET6;
    }
    else if (version == IP_ANY) {
        hints->ai_family = AF_UNSPEC;
    }
    if (socket_type == TCP_SOCKET) {
        hints->ai_socktype = SOCK_STREAM;
    }
    else if (socket_type == UDP_SOCKET) {
        hints->ai_socktype = SOCK_DGRAM;
    }
}

void network_lib__clear_address_info_store() {
    for (int i = 0; i <= current_id; i++) {
        freeaddrinfo(address_infos_by_id[i]);
    }
    current_id = -1;
}

uint8_t get_validation_code_using_id(uint8_t validation_code, int id, AddressInfoResultCode* code) {
    if (id < 0 || id >= ADDRESS_INFOS_BY_ID_CAPACITY) {
        *code = ADDRESS_INFO_ID_OUT_OF_RANGE;
        return validation_code;
    }
    if (current_id == -1 || id > current_id) {
        *code = ADDRESS_INFO_NOT_FOUND;
        return validation_code;
    }
    return EXISTS_AND_IN_BOUNDS;
}


struct addrinfo* network_lib__get_bsd_address_info_results_using_id(int address_info_id, AddressInfoResultCode* code) {
    uint8_t validation_code = get_validation_code_using_id(0, address_info_id, code);
    if (validation_code != EXISTS_AND_IN_BOUNDS) return NULL;
    return address_infos_by_id[address_info_id];
}

int network_lib__pop_address_info_store() {
    if (current_id == -1) return -1;
    int popped_id = current_id;
    uint8_t validation_code = get_validation_code_using_id(0, popped_id, NULL);
    if (validation_code != EXISTS_AND_IN_BOUNDS) return -1;
    freeaddrinfo(address_infos_by_id[popped_id]);
    current_id--;
    return popped_id;
}



