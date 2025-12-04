#ifndef BSF_NET_type::type_HPP
# define BSF_NET_type::type_HPP

namespace bsf::net::socket::type {

enum type {
    stream = SOCK_STREAM,
    datagram = SOCK_DGRAM,
    seqpacket = SOCK_SEQPACKET,
    raw = SOCK_RAW,
    rdm = SOCK_RDM,
}; // enum type

template<>
struct traits_base {};

template<type T>
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

#endif // BSF_NET_type::type_HPP