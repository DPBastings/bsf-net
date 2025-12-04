#ifndef BSF_NET_HANDLE_HPP
# define BSF_NET_HANDLE_HPP

namespace bsf::net {

template<typename Derived>
class handle {
public:
	using impl_t = int;

	static constexpr impl_t	null = -1;

	explicit handle() = default;
	~handle() noexcept;
	explicit handle(impl_t);
	explicit handle(handle&&) noexcept;
	explicit handle&	operator=(handle&&) noexcept;
	handle(handle const&) = delete;
	handle&	operator=(handle const&) = delete;

	operator bool() const noexcept;

	bool	is_open() const noexcept;

	impl_t	release() noexcept;
	void	close();
protected:
	impl_t&	raw() noexcept;
	impl_t	raw() const noexcept;
private:
	impl_t	_raw = null;
}; // class handle

}; // namespace bsf::net

# include "./handle.ipp"

#endif // BSF_NET_FILE_HANDLE_HPP
