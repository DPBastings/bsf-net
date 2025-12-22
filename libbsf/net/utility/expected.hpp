#ifndef BSF_EXPECTED_HPP
# define BSF_EXPECTED_HPP

# include <stdexcept> // std::exception
# include <utility> // std::in_place_t

namespace bsf {

template<typename E>
class unexpected {
public:
	template<typename T = E>
	unexpected(T&&) noexcept;
	template<typename... Args>
	unexpected(Args&&...);

	E const&	error() const& noexcept;
	E&			error() & noexcept;
	E const&&	error() const&& noexcept;
	E&&			error() && noexcept;
private:
	E	_error{};
}; // class unexpected<E>



template<typename E>
class bad_expected_access;

template<>
class bad_expected_access<void>: public std::exception {
public:
	char const*	what() noexcept {
		return ("bad expected access");
	}
}; // bad_expected_access<void>

template<typename E>
class bad_expected_access: public bad_expected_access<void> {
public:
	explicit bad_expected_access(E&& error):
		_error{ std::move(error) } {}
	E const&	error() const noexcept {
		return (_error);
	}
	E&	error() noexcept {
		return (_error);
	}
private:
	E	_error;
}; // bad_expected_access<E>



template<typename T, typename E>
class expected {
public:
	using value_type = T;
	using error_type = E;
	using unexpected_type = unexpected<E>;

	expected();
	~expected();
	template<typename U, typename F>
	expected(expected<U, F> const&);
	template<typename U, typename F>
	expected&	operator=(expected<U, F> const&);
	template<typename U, typename F>
	expected(expected<U, F>&&) noexcept;
	template<typename U, typename F>
	expected&	operator=(expected<U, F>&&) noexcept;
	template<typename U = std::remove_cv_t<T>>
	explicit(!std::is_convertible_v<U, T>) expected(U&&);
	template<typename F>
	explicit expected(unexpected<F> const&);
	template<typename F>
	expected&	operator=(unexpected<F> const&);
	expected(unexpected<E>&&) noexcept;
	expected&	operator=(unexpected<E>&&) noexcept;

	T const*	operator->() const noexcept;
	T*			operator->() noexcept;
	T const&	operator*() const noexcept;
	T&			operator*() noexcept;

	bool	has_value() const noexcept;
	operator bool() const noexcept;

	T const&	value() const;
	T&			value();
	E const&	error() const;
	E&			error();
private:
	template<typename U, typename F>
	void	copy(expected<U, F> const&);
	template<typename U, typename F>
	void	move(expected<U, F>&&) noexcept;
	void	move(unexpected<E>&&) noexcept;
	void	destroy() noexcept;

	union {
		value_type	_value;
		error_type	_error;
	}; // anonymous union
	bool	_has_value;
}; // class expected<T, E>

}; // namespace bsf

# include "./expected.ipp"

#endif // BSF_EXPECTED_HPP