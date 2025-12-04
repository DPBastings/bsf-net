#ifndef BSF_NET_UTILITY_HANDLE_IPP
# define BSF_NET_UTILITY_HANDLE_IPP

# ifndef BNET_CATCH_CLOSE_FAILURE
#  include <libbnet/utility/except.hpp>
# endif // BNET_CATCH_CLOSE_FAILURE
extern "C" {
# include <unistd.h>
}

namespace bsf::net {

// Basic operations

handle::handle(raw_type raw):
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

handle::operator bool() const noexcept {
	return (is_open());
}

bool
handle::is_open() const noexcept {
	return (_raw != null);
}

handle::raw_type
handle::release() noexcept {
	raw_type const	to_release = _raw;

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

#endif // BSF_NET_UTILITY_HANDLE_IPP