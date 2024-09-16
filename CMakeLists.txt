cmake_minimum_required(VERSION 3.15...3.25.1)

project(
	NetPP
	VERSION 1.0
	DESCRIPTION "A networking library for C++"
	LANGUAGES CXX)

add_library(netpp-17
	source/Address_IPv4.cpp
	source/except.cpp
	source/Handle.cpp
	source/Poller_ctor.cpp
	source/Poller_Event.cpp
	source/Poller_method.cpp)
target_include_directories(netpp-17 PUBLIC
	include)
target_compile_features(netpp-17 PUBLIC
	cxx_std_17)