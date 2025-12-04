enable_testing()

include(FetchContent)
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
	GIT_TAG			v3.0.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(BSF_NET-20-tests
	tests.cpp
)
target_include_directories(
	BSF_NET-20-tests
	PUBLIC
	../include
)
target_link_libraries(
	BSF_NET-20-tests
	PRIVATE
	Catch2::Catch2WithMain
)
target_link_libraries(
	BSF_NET-20-tests
	PRIVATE
	BSF_NET-20
)
target_compile_features(
	BSF_NET-20
	PUBLIC
	cxx_std_20)

include(CTest)
include(Catch)
catch_discover_tests(BSF_NET-20-tests)
add_test(NAME BSF_NET-20-tests COMMAND BSF_NET-20-tests)