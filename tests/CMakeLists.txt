include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")

# ---------------------------------------------------------------------------
# Test sources — add files here as you write tests
# ---------------------------------------------------------------------------

set(COMPILER_TEST_SOURCES
	# lexer/test_lexer.cpp
	# parser/test_parser.cpp
)

if(NOT COMPILER_TEST_SOURCES)
	message(STATUS "compiler: no test sources — skipping compiler_tests target")
	return()
endif()

# ---------------------------------------------------------------------------
# Test target
# ---------------------------------------------------------------------------

add_executable(compiler_tests ${COMPILER_TEST_SOURCES})

target_link_libraries(compiler_tests PRIVATE
	compiler_headers
	Catch2::Catch2WithMain
)

compiler_apply_settings(compiler_tests)

include(CTest)
include(Catch)
catch_discover_tests(compiler_tests)
