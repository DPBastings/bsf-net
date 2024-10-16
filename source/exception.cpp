#include "network.hpp"

#include <cerrno>

extern int errno;

using network::exception;

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