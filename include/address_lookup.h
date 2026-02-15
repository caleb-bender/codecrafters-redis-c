#ifndef ADDRESS_LOOKUP_H
#define ADDRESS_LOOKUP_H
#include <stdint.h>

#define ADDRESS_INFOS_BY_ID_CAPACITY 100

typedef enum IPVersion : uint8_t {
    IP_ANY,
    IP_VERSION_4,
    IP_VERSION_6
} IPVersion;

typedef enum SocketType : uint8_t {
    UNKNOWN_SOCKET,
    TCP_SOCKET,
    UDP_SOCKET
} SocketType;

typedef enum IPAssignmentMode : uint8_t {
    CUSTOM_PROVIDED_IP,
    AUTO_ASSIGN_IP
} IPAssignmentMode;

typedef enum AddressInfoResultCode : uint8_t {
    ADDRESS_INFO_OK,
    ADDRESS_INFO_NOT_FOUND,
    ADDRESS_INFO_CAPACITY_EXCEEDED_PLEASE_CLEAR_STORE
} AddressInfoResultCode;

int network_lib__create_compatible_address_info(const char* hostname, const char* port,
    IPVersion version, SocketType socket_type, IPAssignmentMode ip_assignment_mode, AddressInfoResultCode* code
);

IPVersion network_lib__get_address_info_ip_version(int id, AddressInfoResultCode* code);

void network_lib__clear_address_info_store();

#endif //ADDRESS_LOOKUP_H