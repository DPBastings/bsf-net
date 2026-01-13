#include <catch2/catch_test_macros.hpp>
#include <libbsf/net/socket/stream.hpp>
#include <libbsf/net/address/unix.hpp>
#include <cstring>

using namespace bsf::net;

TEST_CASE("socket stream unix") {
	using Socket = socket::stream<domain::unix, 1024, 1024>;

	auto [ ra, rb ] = Socket::socket_t::make_pair(socket::config{
		.non_blocking = true,
	});
	Socket	a{ std::move(ra) };
	Socket	b{ std::move(rb) };

	static unsigned char const	to_send[2][5] = {
		"abcd", "efgh"
	};
	REQUIRE(a.put_try(to_send[0], 4) == 4);
	REQUIRE(b.put_try(to_send[1], 4) == 4);
	REQUIRE(a.send());
	REQUIRE(b.send());
	REQUIRE(a.recv());
	REQUIRE(b.recv());
	static unsigned char	to_receive[2][5]{};
	REQUIRE(a.get_try(to_receive[0], 4) == 4);
	REQUIRE(b.get_try(to_receive[1], 4) == 4);
	REQUIRE(std::strncmp(reinterpret_cast<char const*>(to_receive[0]), "efgh", 4) == 0);
	REQUIRE(std::strncmp(reinterpret_cast<char const*>(to_receive[1]), "abcd", 4) == 0);
}