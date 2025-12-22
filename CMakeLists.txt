cmake_minimum_required(VERSION 3.15...3.25.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRES ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
	"BSF Net"
	VERSION 1.0
	DESCRIPTION "A networking library for C++"
	LANGUAGES CXX)

add_library(bsf-net
	libbsf/net/address/address_ipv4.cpp
	libbsf/net/address/address_ipv6.cpp
	# libbsf/net/address/address_local.cpp
	libbsf/net/async/event.cpp
	libbsf/net/async/poller.cpp
	libbsf/net/socket/acceptor.cpp
	libbsf/net/socket/socket_base.cpp
	libbsf/net/utility/except.cpp
	libbsf/net/utility/handle.cpp
)
target_include_directories(bsf-net
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(bsf-net
	PUBLIC
		cxx_std_20
)
target_compile_options(bsf-net
	PUBLIC
		-Wall -Wextra -Werror -g
)

add_subdirectory(test)