#include <catch2/catch_test_macros.hpp>
#include <libbsf/net/socket/socket_base.hpp>
#include <libbsf/net/address/address.hpp>

#include <iostream>
#include <string>

extern "C" {
#include <netdb.h>
}

using namespace bsf::net;

TEST_CASE("ipv4 tcp socket protocol", "[socket ipv4 tcp protocol]") {
	using Socket = socket::socket_base<domain::ipv4, socket::type::stream>;
	
	auto	s = Socket::make(socket::config{});
	REQUIRE(s);

	protoent*	ent = ::getprotobynumber(s->protocol());
	REQUIRE(strcmp(ent->p_name, "tcp") == 0);
}

TEST_CASE("ipv4 udp socket protocol", "[socket ipv4 udp protocol]") {
	using Socket
		= socket::socket_base<domain::ipv4, socket::type::datagram>;
		
	auto	s = Socket::make(socket::config{});
	REQUIRE(s);

	protoent*	ent = ::getprotobynumber(s->protocol());
	REQUIRE(strcmp(ent->p_name, "udp") == 0);
}

// TEST_CASE("ipv6 acceptor" "[acceptor ipv6 address]") {
// 	using address = address<domain::ipv6>;
// 	address	addr(address::default_host, 1100);
// 	basic_acceptor_socket<domain::ipv6, socket::type::stream>	s(addr);

// 	REQUIRE(std::string(s.address()) == "[0:0:0:0:0:0:0:0]:1100");
// }