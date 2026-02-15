#include "pch.h"
#include "address_lookup.h"

#define STARTING_ADDRESS_INFOS_BY_ID_SIZE 10

static struct addrinfo** address_infos_by_id[STARTING_ADDRESS_INFOS_BY_ID_SIZE];
static int size_of_address_infos_by_id = STARTING_ADDRESS_INFOS_BY_ID_SIZE;
static int current_id = -1;

int network_lib__get_first_compatible_address_info(const char* hostname, const char* port,
    IPVersion version, SocketType socket_type, IPAssignmentMode ip_assignment_mode, AddressInfoResultCode* code) {

    struct addrinfo hints;
    struct addrinfo* results;
    create_addrinfo_hints_from(&hints, version, socket_type, ip_assignment_mode);
    int result_code = getaddrinfo(hostname, port, &hints, &results);
    if (result_code != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(result_code));
    }
    address_infos_by_id[++current_id] = results;
    *code = ADDRESS_INFO_OK;
    return current_id;
}

IPVersion network_lib__get_ip_version(int id, AddressInfoResultCode* code) {
    if (current_id == -1) {
        *code = ADDRESS_INFO_NOT_FOUND;
        return IP_ANY;
    }
    struct addrinfo* result = address_infos_by_id[id];
    *code = ADDRESS_INFO_OK;
    if (result->ai_family == AF_INET)
        return IP_VERSION_4;
    else if (result->ai_family == AF_INET6)
        return IP_VERSION_6;
        
    return IP_ANY;
}

void create_addrinfo_hints_from(struct addrinfo* hints, IPVersion version, SocketType socket_type, IPAssignmentMode ip_assignment_mode) {
    memset(hints, 0, sizeof(struct addrinfo));
    if (version == IP_VERSION_4) {
        hints->ai_family = AF_INET;
    }
    else if (version == IP_VERSION_6) {
        hints->ai_family = AF_INET6;
    }
    hints->ai_socktype = SOCK_STREAM; // TCP stream sockets
}

void network_lib__clear_address_info_store() {
    for (int i = 0; i < current_id + 1; i++) {
        freeaddrinfo(address_infos_by_id[i]);
    }
    current_id = -1;
}



