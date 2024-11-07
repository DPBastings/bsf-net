#include <catch2/catch_test_macros.hpp>
#include "socket.hpp"
#include "address.hpp"
#include "acceptor_socket.hpp"

#include <iostream>
#include <string>

extern "C" {
#include <netdb.h>
}

using namespace network;

TEST_CASE("address ipv4") {
	using address = basic_address<socket_domain::ipv4>;

	address	a0;
	address	a1 {address::default_host, 443};
	address	a2 {"192.168.0.0"};
	REQUIRE(std::string(a0) == "0.0.0.0:0");
	REQUIRE(std::string(a1) == "0.0.0.0:443");
	REQUIRE(std::string(a2) == "192.168.0.0:0");
}

TEST_CASE("ipv4 socket default address", "[socket ipv4 address]") {
	using socket = basic_socket<socket_domain::ipv4, socket_type::stream>;
 
	socket	s;

	REQUIRE(std::string(s.address()) == "0.0.0.0:0");
}

TEST_CASE("ipv4 tcp socket protocol", "[socket ipv4 tcp protocol]") {
	basic_socket<socket_domain::ipv4, socket_type::stream>	s;

	protoent*	ent = ::getprotobynumber(s.protocol());
	REQUIRE(strcmp(ent->p_name, "tcp") == 0);
}

TEST_CASE("ipv4 udp socket protocol", "[socket ipv4 udp protocol]") {
	basic_socket<socket_domain::ipv4, socket_type::datagram>	s;

	protoent*	ent = ::getprotobynumber(s.protocol());
	REQUIRE(strcmp(ent->p_name, "udp") == 0);
}

TEST_CASE("ipv6 acceptor" "[acceptor ipv6 address]") {
	using address = basic_address<socket_domain::ipv6>;
	address	addr(address::default_host, 1100);
	basic_acceptor_socket<socket_domain::ipv6, socket_type::stream>	s(addr);

	REQUIRE(std::string(s.address()) == "[0:0:0:0:0:0:0:0]:1100");
}