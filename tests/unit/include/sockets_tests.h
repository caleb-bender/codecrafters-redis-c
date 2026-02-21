#include "test_pch.h"
#include "sockets.h"

UTEST(SocketTests, given_address_info_exists_when_opening_socket_then_descriptor_is_returned) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    // Assert
    ASSERT_EQ(0, socket_descriptor);
    ASSERT_EQ(SOCKET_OPEN_RESULT_OK, socket_code);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, given_address_info_doesnt_exist_when_opening_socket_then_code_is_address_info_not_found) {
    // Arrange
    SocketOpenResultCode socket_code;
    // Act
    int socket_descriptor = network_lib__open_socket(0, &socket_code);
    // Assert
    ASSERT_EQ(-1, socket_descriptor);
    ASSERT_EQ(SOCKET_OPEN_RESULT_ADDRESS_INFO_NOT_FOUND, socket_code);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_opening_socket_descriptors_past_capacity_then_code_is_capacity_reached) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    for (int i = 0; i < SOCKET_DESCRIPTORS_BY_ID_CAPACITY; i++) {
        int descriptor = network_lib__open_socket(address_info_id, NULL);
        ASSERT_EQ(i, descriptor);
    }
    SocketOpenResultCode socket_code;
    int descriptor = network_lib__open_socket(address_info_id, &socket_code);
    // Assert
    ASSERT_EQ(-1, descriptor);
    ASSERT_EQ(SOCKET_OPEN_RESULT_CAPACITY_REACHED_PLEASE_CLOSE_AT_LEAST_ONE_SOCKET, socket_code);
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
        int descriptor = network_lib__open_socket(address_info_id, NULL);
        ASSERT_EQ(i, descriptor);
    }
    network_lib__close_all_sockets();
    SocketOpenResultCode socket_code;
    int descriptor = network_lib__open_socket(address_info_id, &socket_code);
    // Assert
    ASSERT_EQ(0, descriptor);
    ASSERT_EQ(SOCKET_OPEN_RESULT_OK, socket_code);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_opening_socket_then_status_is_open) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    // Assert
    SocketStatusCode status = network_lib__socket_descriptor_status(socket_descriptor);
    ASSERT_EQ(SOCKET_STATUS_OPEN, status);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_closing_socket_then_status_is_closed) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    network_lib__close_socket(socket_descriptor);
    // Assert
    SocketStatusCode status = network_lib__socket_descriptor_status(socket_descriptor);
    ASSERT_EQ(SOCKET_STATUS_CLOSED, status);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_opening_a_few_sockets_and_closing_middle_one_then_next_open_socket_reuses_closed_socket_descriptor) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int first_socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    int second_socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    int third_socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    network_lib__close_socket(second_socket_descriptor);
    int fourth_socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    // Assert
    ASSERT_EQ(second_socket_descriptor, fourth_socket_descriptor);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_opening_a_couple_sockets_and_closing_second_then_first_is_open_and_second_is_closed) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int first_socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    int second_socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    network_lib__close_socket(second_socket_descriptor);
    // Assert
    SocketStatusCode first_status = network_lib__socket_descriptor_status(first_socket_descriptor);
    SocketStatusCode second_status = network_lib__socket_descriptor_status(second_socket_descriptor);
    ASSERT_EQ(SOCKET_STATUS_OPEN, first_status);
    ASSERT_EQ(SOCKET_STATUS_CLOSED, second_status);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_opening_socket_then_address_info_still_exists_and_is_usable) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    // Assert
    network_lib__get_address_info_ip_version(address_info_id, &addr_code);
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_opening_socket_and_deleting_address_info_then_socket_descriptor_still_exists_but_address_info_does_not) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int socket_descriptor = network_lib__open_socket_and_delete_address_info(address_info_id, &socket_code);
    // Assert
    SocketStatusCode status = network_lib__socket_descriptor_status(socket_descriptor);
    ASSERT_EQ(SOCKET_STATUS_OPEN, status);
    network_lib__get_address_info_ip_version(address_info_id, &addr_code);
    ASSERT_EQ(ADDRESS_INFO_NOT_FOUND, addr_code);
    network_lib__get_address_info_socket_type(address_info_id, &addr_code);
    ASSERT_EQ(ADDRESS_INFO_NOT_FOUND, addr_code);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, given_address_info_does_not_exist_when_opening_socket_and_deleting_address_info_then_code_is_address_info_not_found) {
    // Arrange
    SocketOpenResultCode socket_code;
    // Act
    int socket_descriptor = network_lib__open_socket_and_delete_address_info(2, &socket_code);
    // Assert
    ASSERT_EQ(-1, socket_descriptor);
    ASSERT_EQ(SOCKET_OPEN_RESULT_ADDRESS_INFO_NOT_FOUND, socket_code);
    network_lib__close_all_sockets();
}

UTEST(SocketTests, given_address_info_id_is_out_of_range_when_opening_socket_and_deleting_address_info_then_code_is_address_info_not_found) {
    // Arrange
    SocketOpenResultCode socket_code;
    // Act
    int socket_descriptor = network_lib__open_socket_and_delete_address_info(28847737, &socket_code);
    // Assert
    ASSERT_EQ(-1, socket_descriptor);
    ASSERT_EQ(SOCKET_OPEN_RESULT_ADDRESS_INFO_NOT_FOUND, socket_code);
    network_lib__close_all_sockets();
}

UTEST(SocketTests, given_address_info_id_is_negative_when_opening_socket_and_deleting_address_info_then_code_is_address_info_not_found) {
    // Arrange
    SocketOpenResultCode socket_code;
    // Act
    int socket_descriptor = network_lib__open_socket_and_delete_address_info(-1, &socket_code);
    // Assert
    ASSERT_EQ(-1, socket_descriptor);
    ASSERT_EQ(SOCKET_OPEN_RESULT_ADDRESS_INFO_NOT_FOUND, socket_code);
    network_lib__close_all_sockets();
}

UTEST(SocketTests, when_closing_socket_using_negative_id_then_nothing_happens) {
    // Arrange
    AddressInfoResultCode addr_code;
    int address_info_id = network_lib__create_compatible_address_info(NULL, "27145", IP_ANY, TCP_SOCKET, AUTO_ASSIGN_IP, &addr_code);
    // Act
    ASSERT_EQ(ADDRESS_INFO_OK, addr_code);
    SocketOpenResultCode socket_code;
    int socket_descriptor = network_lib__open_socket(address_info_id, &socket_code);
    network_lib__close_socket(-1);
    // Assert
    SocketStatusCode status = network_lib__socket_descriptor_status(socket_descriptor);
    ASSERT_EQ(SOCKET_STATUS_OPEN, status);
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}

UTEST(SocketTests, when_closing_socket_using_out_of_range_id_then_nothing_happens) {
    // Arrange
    network_lib__close_socket(28847737);
    // Assert
    network_lib__close_all_sockets();
    network_lib__clear_address_info_store();
}