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
        NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, &code
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
        NULL, "27653", IP_VERSION_6, TCP_SOCKET, AUTO_ASSIGN_IP, &code
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

UTEST(AddressLookupTests, when_creating_too_many_address_infos_then_result_code_is_capacity_exceeded_please_reset) {
    // Arrange
    AddressInfoResultCode code;
    for (int i = 0; i < ADDRESS_INFOS_BY_ID_CAPACITY; i++) {
        int id = network_lib__create_compatible_address_info(
            NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, &code
        );
        ASSERT_EQ(i, id);
    }
    int id = network_lib__create_compatible_address_info(
        NULL, "27653", IP_VERSION_4, TCP_SOCKET, AUTO_ASSIGN_IP, &code
    );
    ASSERT_EQ(-1, id);
    ASSERT_EQ(ADDRESS_INFO_CAPACITY_EXCEEDED_PLEASE_CLEAR_STORE, code);
    network_lib__clear_address_info_store();
}


#endif //ADDRESS_LOOKUP_TESTS_H