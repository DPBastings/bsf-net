cmake_minimum_required(VERSION 3.15...3.25.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRES ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
	BSF
	VERSION 1.0
	DESCRIPTION "A networking library for C++"
	LANGUAGES CXX)

add_library(bsf_net
	libbnet/address/address_ipv4.cpp
	libbnet/address/address_ipv6.cpp
	libbnet/address/address_local.cpp
	libbnet/utility/except.cpp
)
target_include_directories(bsf_net
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(bsf_net
	PUBLIC
		cxx_std_20
)
target_compile_options(bsf_net
	PUBLIC
		-Wall -Wextra -Werror -g
)