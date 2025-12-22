#ifndef BSF_NET_HANDLE_HPP
# define BSF_NET_HANDLE_HPP

namespace bsf::net {

class handle {
public:
	using raw_t = int;

	static constexpr raw_t	null = -1;

	explicit handle() = default;
	~handle() noexcept;
	explicit handle(raw_t);
	explicit handle(handle&&) noexcept;
	handle&	operator=(handle&&) noexcept;
	handle(handle const&) = delete;
	handle&	operator=(handle const&) = delete;

	[[nodiscard]] bool	is_open() const noexcept;
	operator bool() const noexcept;

	[[nodiscard]] raw_t	release() noexcept;
	void				close();

	[[nodiscard]] raw_t&	raw() noexcept;
	[[nodiscard]] raw_t		raw() const noexcept;
private:
	raw_t	_raw = null;
}; // class handle

}; // namespace bsf::net

#endif // BSF_NET_FILE_HANDLE_HPP
