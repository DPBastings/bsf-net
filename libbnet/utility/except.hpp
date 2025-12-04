#ifndef BSF_NET_HPP
# define BSF_NET_HPP

# include <stdexcept>

namespace bsf::net {

class exception: public std::exception {
public:
	explicit exception();
	explicit exception(char const*) noexcept;

	char const*	what() const noexcept;
	int			error() const noexcept;
private:
	char const*	_msg = "";
	int			_errno;
}; // class exception

}; // namespace bsf::net

#endif // BSF_NET_HPP
