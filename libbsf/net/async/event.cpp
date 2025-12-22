#include <libbsf/net/async/event.hpp>

namespace bsf::net::async {

event::event(type t, handle::raw_t h) noexcept:
	_mask{ t },
	_ref{ h } {}

bool
event::concerns(handle const& h) const noexcept {
	return (_ref == h.raw());
}

bool
event::is(type t) const noexcept {
	return (_mask & static_cast<uint32_t>(t));
}

event::operator bool() const noexcept {
	return (!(is(type::none) || _ref == handle::null));
}

}; // namespace bsf::net::async