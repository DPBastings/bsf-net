#ifndef BSF_NET_SOCKET_PROPERTIES_HPP
# define BSF_NET_SOCKET_PROPERTIES_HPP

namespace bsf::net::socket::type {

struct stream {};
struct datagram {};
struct seqpacket {};
struct raw {};
struct rdm {};


template<>
struct traits_base {};

template<typename T>
struct traits;

template<>
struct traits<stream> {};
template<>
struct traits<datagram> {};
template<>
struct traits<seqpacket> {};
template<>
struct traits<raw> {};
template<>
struct traits<rdm> {};

}; // namespace bsf::net::socket::type

#endif // BSF_BNET_SOCKET_PROPERTIES_HPP