#ifndef NETPP_HANDLE_HPP
# define NETPP_HANDLE_HPP

# include "network.hpp"

# include <memory>
# include <stdexcept>

namespace network {
	
class handle {
public:
	using raw_type = int;

	static constexpr raw_type	_invalid_handle = -1;

	handle(raw_type = _invalid_handle);
	~handle();
	handle(handle const&);
	handle(handle&&) noexcept;
	handle&	operator=(handle const&) ;
	handle&	operator=(handle&&) noexcept;

	bool	is_open() const noexcept;

	raw_type	release() noexcept;
	void		close();

private:
	template<socket_domain D, socket_type T>
	friend class socket;
	friend class poller;

	raw_type	_raw;
}; // class handle

using shared_handle = std::shared_ptr<handle>;

}; // namespace Network

#endif // NETPP_FILE_HANDLE_HPP
