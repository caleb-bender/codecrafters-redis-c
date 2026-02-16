#include "test_pch.h"
#include "sockets.h"

UTEST(SocketTests, given_address_info_exists_when_creating_socket_then_descriptor_is_returned) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketResultCode socket_code;
    int socket_descriptor = network_lib__create_socket_descriptor(address_info_id, &socket_code);
    // Assert
    ASSERT_EQ(0, socket_descriptor);
    ASSERT_EQ(SOCKET_RESULT_OK, socket_code);
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, given_address_info_doesnt_exist_when_creating_socket_then_code_is_address_info_not_found) {
    // Arrange
    SocketResultCode socket_code;
    // Act
    int socket_descriptor = network_lib__create_socket_descriptor(0, &socket_code);
    // Assert
    ASSERT_EQ(-1, socket_descriptor);
    ASSERT_EQ(SOCKET_RESULT_ADDRESS_INFO_NOT_FOUND, socket_code);
    network_lib__clear_address_info_store();
}