#include "network.hpp"

#include <cerrno>
#include <cstring>

extern int errno;

using bsf::net::exception;

exception::exception(char const* msg) noexcept:
	_msg(msg), _errno(errno) {}

char const*
exception::what() const noexcept {
	return (_msg);
}

int
exception::error() const noexcept {
	return (_errno);
}