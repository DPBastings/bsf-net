#include <catch2/catch_test_macros.hpp>
#include "socket.hpp"
#include "address_ipv4.hpp"
#include "address_ipv6.hpp"
#include "acceptor_socket.hpp"

#include <iostream>
#include <string>

#include <netdb.h>

TEST_CASE("address ipv4") {
	using address = network::address<network::socket_domain::ipv4>;

	address	a0;
	address	a1 {443};
	address	a2 {"192.168.0.0", 443};
	REQUIRE(std::string(a0) == "0.0.0.0:0");
	REQUIRE(std::string(a1) == "0.0.0.0:443");
	REQUIRE(std::string(a2) == "192.168.0.0:443");
}

TEST_CASE("ipv4 socket default address", "[socket ipv4 address]") {
	using socket = network::socket<network::socket_domain::ipv4, network::socket_type::stream>;
 
	socket	s;

	char	hname[256];
	gethostname(hname, 256);
	std::cout << hname << std::endl;

	REQUIRE(std::string(s.address()) == "0.0.0.0:0");
}

TEST_CASE("ipv4 tcp socket protocol", "[socket ipv4 tcp protocol]") {
	network::socket<network::socket_domain::ipv4, network::socket_type::stream>	s;

	protoent*	ent = ::getprotobynumber(s.protocol());
	REQUIRE(strcmp(ent->p_name, "tcp") == 0);
}

TEST_CASE("ipv4 udp socket protocol", "[socket ipv4 udp protocol]") {
	network::socket<network::socket_domain::ipv4, network::socket_type::datagram>	s;

	protoent*	ent = ::getprotobynumber(s.protocol());
	REQUIRE(strcmp(ent->p_name, "udp") == 0);
}

TEST_CASE("ipv6 acceptor" "[acceptor ipv6 address]") {
	network::address<network::socket_domain::ipv6>	addr(1100);
	network::acceptor_socket<network::socket_domain::ipv6, network::socket_type::stream>	s(addr);

	REQUIRE(std::string(s.address()) == "[0:0:0:0:0:0:0:0]:1100");
}