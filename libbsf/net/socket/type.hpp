#ifndef BSF_NET_SOCKET_TYPE_HPP
# define BSF_NET_SOCKET_TYPE_HPP

namespace bsf::net::socket::type {

enum type {
    stream = SOCK_STREAM,
    datagram = SOCK_DGRAM,
    seqpacket = SOCK_SEQPACKET,
    raw = SOCK_RAW,
    rdm = SOCK_RDM,
}; // enum type

template<bool ConnectionBased>
struct traits_base {
    /// Whether this communications mode is connection-based. Notably, only 
    /// connection-based sockets can listen for incoming connection requests.
    static constexpr bool   is_connection_based = ConnectionBased;
};

template<type T>
struct traits;

template<>
struct traits<stream>:
    traits_base<true> {};
template<>
struct traits<datagram>:
    traits_base<false> {};
template<>
struct traits<seqpacket>:
    traits_base<true> {};
template<>
struct traits<raw>:
    traits_base<false> {};
template<>
struct traits<rdm>:
    traits_base<false> {};

}; // namespace bsf::net::socket::type

#endif // BSF_NET_SOCKET_TYPE_HPP