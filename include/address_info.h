#ifndef ADDRESS_INFO_H
#define ADDRESS_INFO_H
#include <stdint.h>

#define ADDRESS_INFOS_BY_ID_CAPACITY 100

typedef enum IPVersion : uint8_t {
    IP_ANY,
    IP_VERSION_4,
    IP_VERSION_6
} IPVersion;

typedef enum SocketType : uint8_t {
    UNKNOWN_SOCKET = 4U,
    TCP_SOCKET,
    UDP_SOCKET
} SocketType;

typedef enum IPAssignmentMode : uint8_t {
    CUSTOM_PROVIDED_IP = 16U,
    AUTO_ASSIGN_IP
} IPAssignmentMode;

typedef enum AddressInfoResultCode : uint8_t {
    ADDRESS_INFO_OK = 32U,
    ADDRESS_INFO_NOT_FOUND,
    ADDRESS_INFO_CAPACITY_REACHED_PLEASE_CLEAR_STORE,
    ADDRESS_INFO_ID_OUT_OF_RANGE,
    ADDRESS_INFO_HOSTNAME_REQUIRED,
    ADDRESS_INFO_INCOMPATIBLE_IP_VERSION_GIVEN_HOSTNAME,
    ADDRESS_INFO_UNKNOWN_ERROR
} AddressInfoResultCode;

int network_lib__create_compatible_address_info(const char* hostname, const char* port,
    IPVersion version, SocketType socket_type, IPAssignmentMode ip_assignment_mode, AddressInfoResultCode* code
);

IPVersion network_lib__get_address_info_ip_version(int id, AddressInfoResultCode* code);

SocketType network_lib__get_address_info_socket_type(int id, AddressInfoResultCode* code);

void network_lib__clear_address_info_store();

int network_lib__pop_address_info_store();

#endif //ADDRESS_INFO_H