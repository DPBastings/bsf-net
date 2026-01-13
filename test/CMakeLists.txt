enable_testing()

include(FetchContent)
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
	GIT_TAG			v3.0.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(bsf-net_tests
	socket/stream.test.cpp
	address.test.cpp
	protocol.test.cpp
)
target_compile_features(bsf-net_tests
	PUBLIC
		cxx_std_20)
target_link_libraries(bsf-net_tests
	PRIVATE
		Catch2::Catch2WithMain
		bsf-net
)

include(CTest)
include(Catch)
catch_discover_tests(bsf-net_tests)
add_test(NAME bsf-net_tests COMMAND bsf-net_tests)