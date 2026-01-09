#include <libbsf/net/utility/handle.hpp>
#ifndef BSF_NET_CATCH_CLOSE_FAILURE
# include <libbsf/net/utility/except.hpp>
#endif // BSF_NET_CATCH_CLOSE_FAILURE

extern "C" {
#include <unistd.h>
}

namespace bsf::net {

// Basic operations

handle::handle(raw_t raw):
	_raw{ raw } {}

handle::~handle() {
	close();
}

handle::handle(handle&& that) noexcept:
	_raw{ that.release() } {}

handle&
handle::operator=(handle&& that) noexcept {
	if (this == &that) return (*this);
	close();
	_raw = that.release();
	return (*this);
}

// Public methods

bool
handle::is_open() const noexcept {
	return (_raw != null);
}

handle::operator bool() const noexcept {
	return (is_open());
}

handle::raw_t
handle::raw() const noexcept {
	return (_raw);
}

handle::raw_t
handle::release() noexcept {
	raw_t const	to_release = _raw;

	_raw = null;
	return (to_release);
}

void
handle::close() {
	if (is_open()) {
# ifdef BNET_CATCH_CLOSE_FAILURE
		if (::close(release()) == -1) throw (exception("close"));
# else
		::close(release());
# endif // BNET_CATCH_CLOSE_FAILURE
	}
}

}; // namespace bsf::net