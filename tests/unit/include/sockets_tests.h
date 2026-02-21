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
    network_lib__close_all_sockets();
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
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_creating_socket_descriptors_past_capacity_then_code_is_capacity_reached) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    for (int i = 0; i < SOCKET_DESCRIPTORS_BY_ID_CAPACITY; i++) {
        int descriptor = network_lib__create_socket_descriptor(address_info_id, NULL);
        ASSERT_EQ(i, descriptor);
    }
    SocketResultCode socket_code;
    int descriptor = network_lib__create_socket_descriptor(address_info_id, &socket_code);
    // Assert
    ASSERT_EQ(-1, descriptor);
    ASSERT_EQ(SOCKET_RESULT_CAPACITY_REACHED_PLEASE_CLOSE_AT_LEAST_ONE_SOCKET, socket_code);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_closing_all_sockets_then_sockets_are_all_freed) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    for (int i = 0; i < SOCKET_DESCRIPTORS_BY_ID_CAPACITY; i++) {
        int descriptor = network_lib__create_socket_descriptor(address_info_id, NULL);
        ASSERT_EQ(i, descriptor);
    }
    network_lib__close_all_sockets();
    SocketResultCode socket_code;
    int descriptor = network_lib__create_socket_descriptor(address_info_id, &socket_code);
    // Assert
    ASSERT_EQ(0, descriptor);
    ASSERT_EQ(SOCKET_RESULT_OK, socket_code);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_creating_socket_and_checking_status_then_it_exists_but_is_closed) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    int socket_descriptor = network_lib__create_socket_descriptor(address_info_id, NULL);
    ASSERT_EQ(0, socket_descriptor);
    SocketStatusCode result = network_lib__socket_descriptor_status(socket_descriptor);
    // Assert
    ASSERT_EQ(SOCKET_STATUS_EXISTS_BUT_CLOSED, result);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_checking_socket_descriptor_status_with_nonexistent_id_then_it_returns_not_found_or_closed_code) {
    // Act
    SocketStatusCode result1 = network_lib__socket_descriptor_status(0);
    SocketStatusCode result2 = network_lib__socket_descriptor_status(-1);
    // Assert
    ASSERT_EQ(SOCKET_STATUS_NOT_FOUND_OR_CLOSED, result1);
    ASSERT_EQ(SOCKET_STATUS_NOT_FOUND_OR_CLOSED, result2);
}

UTEST(SocketTests, when_checking_socket_descriptor_status_after_closing_it_then_it_returns_not_found_or_closed_code) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    int socket_descriptor = network_lib__create_socket_descriptor(address_info_id, NULL);
    network_lib__close_last_socket();
    SocketStatusCode result = network_lib__socket_descriptor_status(socket_descriptor);
    // Assert
    ASSERT_EQ(SOCKET_STATUS_NOT_FOUND_OR_CLOSED, result);
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_creating_multiple_sockets_and_closing_once_then_second_is_freed_but_first_isnt) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    int first_socket_descriptor = network_lib__create_socket_descriptor(address_info_id, NULL);
    int second_socket_descriptor = network_lib__create_socket_descriptor(address_info_id, NULL);
    ASSERT_EQ(0, first_socket_descriptor);
    ASSERT_EQ(1, second_socket_descriptor);
    network_lib__close_last_socket();
    SocketStatusCode first_result = network_lib__socket_descriptor_status(first_socket_descriptor);
    SocketStatusCode second_result = network_lib__socket_descriptor_status(second_socket_descriptor);
    // Assert
    ASSERT_EQ(SOCKET_STATUS_EXISTS_BUT_CLOSED, first_result);
    ASSERT_EQ(SOCKET_STATUS_NOT_FOUND_OR_CLOSED, second_result);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_closing_last_socket_and_creating_new_socket_then_first_id_is_zero_and_no_error_occurs) {
    // Act
    network_lib__close_last_socket();
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, NULL);
    int socket_descriptor = network_lib__create_socket_descriptor(address_info_id, NULL);
    SocketStatusCode result = network_lib__socket_descriptor_status(0);
    // Assert
    ASSERT_EQ(0, socket_descriptor);
    ASSERT_EQ(SOCKET_STATUS_EXISTS_BUT_CLOSED, result);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}