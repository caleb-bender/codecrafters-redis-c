#ifndef ADDRESS_LOOKUP_TESTS_H
#define ADDRESS_LOOKUP_TESTS_H
#include "test_pch.h"
#include "address_lookup.h"

UTEST(AddressLookupTests, given_localhost_port_and_tcp_and_ip_version_4_when_creating_compatible_address_info_then_returned_struct_is_correct) {
    // Arrange
    AddressInfoResultCode code;
    // Act
    int address_info_id = network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, &code
    );
    // Assert
    ASSERT_EQ(0, address_info_id);
    ASSERT_EQ(ADDRESS_INFO_OK, code);
}

UTEST(AddressLookupTests, given_ip_version_4_when_looking_up_address_info_version_then_returned_version_is_correct) {
    // Arrange
    AddressInfoResultCode code;
    int address_info_id = network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    IPVersion version = network_lib__get_address_info_ip_version(address_info_id, &code);
    // Assert
    ASSERT_EQ(IP_VERSION_4, version);
    ASSERT_EQ(ADDRESS_INFO_OK, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_ip_version_6_when_looking_up_address_info_version_then_returned_version_is_correct) {
    // Arrange
    AddressInfoResultCode code;
    int address_info_id = network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_6, TCP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    IPVersion version = network_lib__get_address_info_ip_version(address_info_id, &code);
    // Assert
    ASSERT_EQ(IP_VERSION_6, version);
    ASSERT_EQ(ADDRESS_INFO_OK, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_address_info_doesnt_exist_when_looking_up_address_info_version_then_result_code_is_not_found) {
    // Arrange
    AddressInfoResultCode code;
    int address_info_id = 0;
    // Act
    IPVersion version = network_lib__get_address_info_ip_version(address_info_id, &code);
    // Assert
    ASSERT_EQ(IP_ANY, version);
    ASSERT_EQ(ADDRESS_INFO_NOT_FOUND, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, when_creating_too_many_address_infos_then_result_code_is_capacity_reached_please_reset) {
    // Arrange
    AddressInfoResultCode code;
    for (int i = 0; i < ADDRESS_INFOS_BY_ID_CAPACITY; i++) {
        int id = network_lib__create_compatible_address_info(
            NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, NULL
        );
        ASSERT_EQ(i, id);
    }
    int id = network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, &code
    );
    ASSERT_EQ(-1, id);
    ASSERT_EQ(ADDRESS_INFO_CAPACITY_REACHED_PLEASE_CLEAR_STORE, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_tcp_when_looking_up_address_info_socket_type_then_it_is_correct) {
    // Arrange
    AddressInfoResultCode code;
    int id = network_lib__create_compatible_address_info(
            NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    SocketType socket_type = network_lib__get_address_info_socket_type(id, &code);
    // Assert
    ASSERT_EQ(TCP_SOCKET, socket_type);
    ASSERT_EQ(ADDRESS_INFO_OK, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_udp_when_looking_up_address_info_socket_type_then_it_is_correct) {
    // Arrange
    AddressInfoResultCode code;
    int id = network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, UDP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    SocketType socket_type = network_lib__get_address_info_socket_type(id, &code);
    // Assert
    ASSERT_EQ(UDP_SOCKET, socket_type);
    ASSERT_EQ(ADDRESS_INFO_OK, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_address_info_doesnt_exist_when_looking_up_address_info_socket_type_then_code_is_not_found) {
    // Arrange
    AddressInfoResultCode code;
    // Act
    SocketType socket_type = network_lib__get_address_info_socket_type(0, &code);
    // Assert
    ASSERT_EQ(UNKNOWN_SOCKET, socket_type);
    ASSERT_EQ(ADDRESS_INFO_NOT_FOUND, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_address_info_exists_but_id_out_of_range_when_looking_up_address_info_ip_version_then_out_of_range_code) {
    // Arrange
    AddressInfoResultCode below_lower_bound_code;
    AddressInfoResultCode above_upper_bound_code;
    network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, UDP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    IPVersion lower_bound_version = network_lib__get_address_info_ip_version(-1, &below_lower_bound_code);
    IPVersion upper_bound_version = network_lib__get_address_info_ip_version(ADDRESS_INFOS_BY_ID_CAPACITY, &above_upper_bound_code);
    // Assert
    ASSERT_EQ(IP_ANY, lower_bound_version);
    ASSERT_EQ(IP_ANY, upper_bound_version);
    ASSERT_EQ(ADDRESS_INFO_ID_OUT_OF_RANGE, below_lower_bound_code);
    ASSERT_EQ(ADDRESS_INFO_ID_OUT_OF_RANGE, above_upper_bound_code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_address_info_exists_but_id_exceeds_current_id_when_looking_up_address_info_ip_version_then_code_is_not_found) {
    // Arrange
    AddressInfoResultCode code;
    network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, UDP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    IPVersion version = network_lib__get_address_info_ip_version(1, &code);
    // Assert
    ASSERT_EQ(IP_ANY, version);
    ASSERT_EQ(ADDRESS_INFO_NOT_FOUND, code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_address_info_exists_but_id_out_of_range_when_looking_up_address_info_socket_type_then_out_of_range_code) {
    // Arrange
    AddressInfoResultCode below_lower_bound_code;
    AddressInfoResultCode above_upper_bound_code;
    network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, UDP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    SocketType lower_bound_socket_type = network_lib__get_address_info_socket_type(-1, &below_lower_bound_code);
    SocketType upper_bound_socket_type = network_lib__get_address_info_socket_type(ADDRESS_INFOS_BY_ID_CAPACITY, &above_upper_bound_code);
    // Assert
    ASSERT_EQ(UNKNOWN_SOCKET, lower_bound_socket_type);
    ASSERT_EQ(UNKNOWN_SOCKET, upper_bound_socket_type);
    ASSERT_EQ(ADDRESS_INFO_ID_OUT_OF_RANGE, below_lower_bound_code);
    ASSERT_EQ(ADDRESS_INFO_ID_OUT_OF_RANGE, above_upper_bound_code);
    network_lib__clear_address_info_store();
}

UTEST(AddressLookupTests, given_address_info_exists_but_id_exceeds_current_id_when_looking_up_socket_type_ip_version_then_code_is_not_found) {
    // Arrange
    AddressInfoResultCode code;
    network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, UDP_SOCKET, AUTO_ASSIGN_IP, NULL
    );
    // Act
    SocketType socket_type = network_lib__get_address_info_socket_type(1, &code);
    // Assert
    ASSERT_EQ(UNKNOWN_SOCKET, socket_type);
    ASSERT_EQ(ADDRESS_INFO_NOT_FOUND, code);
    network_lib__clear_address_info_store();
}

#endif //ADDRESS_LOOKUP_TESTS_H