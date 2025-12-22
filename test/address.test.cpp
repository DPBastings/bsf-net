#include <catch2/catch_test_macros.hpp>
#include <libbsf/net/socket/socket_base.hpp>
#include <libbsf/net/address/address.hpp>
#include <libbsf/net/socket/acceptor.hpp>
#include <iostream>
#include <string>

extern "C" {
#include <netdb.h>
}

using namespace bsf::net;

TEST_CASE("address ipv4") {
	using Address = address::address<domain::ipv4>;

	Address const	a0{};
	REQUIRE(to_string(a0) == "0.0.0.0:0");

	Address const	a1{ 0, 443 };
	REQUIRE(to_string(a1) == "0.0.0.0:443");

	auto const	a2 = Address::from_string("192.168.0.0");
	REQUIRE(a2);
	REQUIRE(to_string(*a2) == "192.168.0.0:0");
}

TEST_CASE("ipv4 socket default address", "[socket ipv4 address]") {
	using Socket = socket::socket_base<domain::ipv4, socket::type::stream>;

	Socket	s;
	auto	addr = s.address();

	REQUIRE(!addr);
}

TEST_CASE("ipv6 acceptor" "[acceptor ipv6 address]") {
	using Address = address::address<domain::ipv6>;
	using Acceptor = socket::acceptor<domain::ipv6, socket::type::stream>;

	Address	addr{ address::traits<domain::ipv6>::default_host, 1100 };
	auto	s = Acceptor::make(addr, socket::config{}, 5);
	REQUIRE(s);

	auto	from = s->address();
	REQUIRE(from);
	REQUIRE(to_string(*from) == "[0:0:0:0:0:0:0:0]:1100");
}