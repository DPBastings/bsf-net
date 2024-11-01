#include <catch2/catch_test_macros.hpp>
#include "socket.hpp"
#include "address_ipv4.hpp"
#include "address_ipv6.hpp"
#include "acceptor_socket.hpp"

#include <iostream>
#include <string>

TEST_CASE("ipv4 socket", "[socket ipv4 address]") {
	network::socket<network::socket_domain::ipv4, network::socket_type::stream>	s;

	std::cout << std::string(s.address()) << std::endl;
	REQUIRE(true);
}

TEST_CASE("ipv6 acceptor" "[acceptor ipv6 address]") {
	network::address<network::socket_domain::ipv6>	addr(1100);
	network::acceptor_socket<network::socket_domain::ipv6, network::socket_type::stream>	s(addr);

	std::cout << std::string(s.address()) << std::endl;
	REQUIRE(true);
}