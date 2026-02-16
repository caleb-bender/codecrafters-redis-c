#include "pch.h"
#include "sockets.h"


extern struct addrinfo* network_lib__get_bsd_address_info_results_using_id(int address_info_id, AddressInfoResultCode* code);

int network_lib__create_socket_descriptor(int address_info_id, SocketResultCode* code)  {
    AddressInfoResultCode addr_code;
    struct addrinfo* address_info = network_lib__get_bsd_address_info_results_using_id(address_info_id, &addr_code);
    if (address_info == NULL) {
        *code = SOCKET_RESULT_ADDRESS_INFO_NOT_FOUND;
        return -1;
    }
    *code = SOCKET_RESULT_OK;
    return 0;
}

