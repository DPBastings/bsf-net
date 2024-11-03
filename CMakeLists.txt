cmake_minimum_required(VERSION 3.15...3.25.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRES ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
	NetPP
	VERSION 1.0
	DESCRIPTION "A networking library for C++"
	LANGUAGES CXX)

add_library(
	netpp-20
	source/address_ipv4.cpp
	source/address_ipv6.cpp
	source/exception.cpp
	source/handle.cpp
	source/misc.cpp
	source/poller.cpp)
target_include_directories(
	netpp-20
	PUBLIC
	include)
target_compile_features(
	netpp-20
	PUBLIC
	cxx_std_20)

add_subdirectory(test)