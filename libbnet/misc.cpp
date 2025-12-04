#include "network.hpp"

extern "C" {
#include <netdb.h>
}

int
protocol_by_name(char const* prot) noexcept {
	if (!prot || prot[0] == '\0') return (0);
	protoent const*	const	entry = ::getprotobyname(prot);

	return (entry->p_proto);
}