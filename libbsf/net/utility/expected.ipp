#ifndef BSF_EXPECTED_IPP
# define BSF_EXPECTED_IPP

# include <type_traits> // std::decay_t

namespace bsf {

/// @brief Default constructor: default-initialize the unexpected value.
template<typename T, typename E>
expected<T, E>::expected():
	_error{},
	_has_value{ false } {}

/// @brief Destructor.
template<typename T, typename E>
expected<T, E>::~expected() {
	destroy();
}

/// @brief Construct an
template<typename T, typename E>
template<typename U, typename F>
expected<T, E>::expected(expected<U, F> const& that):
	_has_value{ that._has_value } {
	copy(that);
}

/// @brief Assign an
template<typename T, typename E>
template<typename U, typename F>
expected<T, E>&
expected<T, E>::operator=(expected<U, F> const& that) {
	if (this == &that) return (*this);
	destroy();
	_has_value = that._has_value;
	copy(that);
	return (*this);
}

/// @brief Construct an
template<typename T, typename E>
template<typename U, typename F>
expected<T, E>::expected(expected<U, F>&& that) noexcept:
	_has_value{ that._has_value } {
	move(std::move(that));
}

/// @brief Assign an
template<typename T, typename E>
template<typename U, typename F>
expected<T, E>&
expected<T, E>::operator=(expected<U, F>&& that) noexcept {
	if (this == &that) return (*this);
	destroy();
	_has_value = that._has_value;
	move(std::move(that));
	return (*this);
}

template<typename T, typename E>
template<typename U, typename F>
void
expected<T, E>::copy(expected<U, F> const& that) {
	if (that._has_value) {
		(void)new(&_value) T{ that._value };
	} else {
		(void)new(&_error) E{ that._error };
	}
}


template<typename T, typename E>
template<typename U, typename F>
void
expected<T, E>::move(expected<U, F>&& that) noexcept {
	if (that._has_value) {
		(void)new(&_value) T{ std::move(that._value) };
	} else {
		(void)new(&_error) E{ std::move(that._error) };
	}
}

/// @brief Construct an
template<typename T, typename E>
template<typename F>
expected<T, E>::expected(unexpected<F> const& that):
	_error{ that },
	_has_value{ false } {}

/// @brief Assign an
template<typename T, typename E>
template<typename F>
expected<T, E>&
expected<T, E>::operator=(unexpected<F> const& that) {
	destroy();
	_has_value = false;
	_error = that;
	return (*this);
}

/// @brief Construct an
template<typename T, typename E>
template<typename F>
expected<T, E>::expected(unexpected<F>&& that) noexcept:
	_has_value{ false } {
	move(that);
}

template<typename T, typename E>
template<typename F>
expected<T, E>&
expected<T, E>::operator=(unexpected<F>&& that) noexcept {
	destroy();
	_has_value = false;
	_error = std::move(that);
	return (*this);
}


/// @brief Get a pointer to the expected value.
template<typename T, typename E>
T const*
expected<T, E>::operator->() const noexcept {
	return (&_value);
}

/// @brief Get a pointer to the expected value.
template<typename T, typename E>
T*
expected<T, E>::operator->() noexcept {
	return (&_value);
}

/// @brief Get a reference to the expected value.
template<typename T, typename E>
T const&
expected<T, E>::operator*() const noexcept {
	return (_value);
}

/// @brief Get a reference to the expected value.
template<typename T, typename E>
T&
expected<T, E>::operator*() noexcept {
	return (_value);
}

/**
 * @brief Check whether the instance holds an expected value.
 */
template<typename T, typename E>
bool
expected<T, E>::has_value() const noexcept {
	return (_has_value);
}

/// @brief Check whether the instance holds an expected value.
template<typename T, typename E>
expected<T, E>::operator bool() const noexcept {
	return (has_value());
}

template<typename T, typename E>
T const&
expected<T, E>::value() const {
	if (!has_value()) {
		throw (bad_expected_access<std::decay_t<E>>(_error));
	}
	return (_value);
}

template<typename T, typename E>
T&
expected<T, E>::value() {
	if (!has_value()) {
		throw (bad_expected_access<std::decay_t<E>>(std::move(_error)));
	}
	return (_value);
}

template<typename T, typename E>
E const&
expected<T, E>::error() const {
	return (_error);
}

template<typename T, typename E>
E&
expected<T, E>::error() {
	return (_error);
}



template<typename T, typename E>
void
expected<T, E>::destroy() noexcept {
	if (has_value()) {
		_value.~T();
	} else {
		_error.~E();
	}
}

}; // namespace bsf

#endif // BSF_EXPECTED_IPP