#include "handle.hpp"

extern "C" {
#include <unistd.h>
}

using network::handle;

// Basic operations

handle::handle(raw_type raw):
	_raw(raw) {}

handle::~handle() {
	close();
}

handle::handle(handle const& that):
	_raw(::dup(that._raw)) {
	if (!is_open()) {
		throw exception("dup");
	}
}

handle::handle(handle&& that) noexcept:
	_raw(that.release()) {}


handle&
handle::operator=(handle const& that) {
	if (this == &that) {
		return (*this);
	}
	if (!that.is_open()) {
		close();
		return (*this);
	}
	raw_type const duplicate = ::dup(that._raw);
	if (duplicate == _invalid_handle) {
		throw exception("dup");
	}
	close();
	_raw = duplicate;
	return (*this);
}

handle&
handle::operator=(handle&& that) noexcept {
	if (this == &that) {
		return (*this);
	}
	close();
	_raw = that.release();
	return (*this);
}

// Public methods

bool
handle::is_open() const noexcept {
	return (_raw != _invalid_handle);
}

handle::raw_type
handle::release() noexcept {
	raw_type const	to_release = _raw;

	_raw = _invalid_handle;
	return (to_release);
}

void
handle::close() {
	if (is_open() && ::close(release()) == -1) {
		throw (exception("close"));
	}
}