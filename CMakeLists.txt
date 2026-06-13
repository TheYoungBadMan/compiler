cmake_minimum_required(VERSION 3.31)

project(compiler
	VERSION 1.0
	LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

option(COMPILER_ENABLE_WARNINGS    "Enable recommended compiler warnings" ON)
option(COMPILER_WARNINGS_AS_ERRORS "Treat compiler warnings as errors"    OFF)
option(COMPILER_ENABLE_ASAN        "Enable AddressSanitizer"              OFF)
option(COMPILER_ENABLE_UBSAN       "Enable UndefinedBehaviorSanitizer"    OFF)
option(COMPILER_ENABLE_CLANG_TIDY  "Run clang-tidy during compilation"    OFF)
option(COMPILER_ENABLE_TESTING     "Build tests"                          OFF)

# ---------------------------------------------------------------------------
# Interface library — shared between compiler and tests
# ---------------------------------------------------------------------------

add_library(compiler_headers INTERFACE)

target_include_directories(compiler_headers INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(compiler_headers INTERFACE
	cxx_std_23
)

# ---------------------------------------------------------------------------
# Helper: apply warnings and sanitizers to any target
# ---------------------------------------------------------------------------

function(compiler_apply_settings target)
	if(COMPILER_ENABLE_WARNINGS)
		if(MSVC)
			target_compile_options(${target} PRIVATE /W4 /permissive-)
		else()
			target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
		endif()
	endif()

	if(COMPILER_WARNINGS_AS_ERRORS)
		if(MSVC)
			target_compile_options(${target} PRIVATE /WX)
		else()
			target_compile_options(${target} PRIVATE -Werror)
		endif()
	endif()

	if(COMPILER_ENABLE_ASAN OR COMPILER_ENABLE_UBSAN)
		if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
			message(FATAL_ERROR "Sanitizers require Clang or GCC")
		endif()

		set(sanitizers "")
		if(COMPILER_ENABLE_ASAN)
			list(APPEND sanitizers address)
		endif()
		if(COMPILER_ENABLE_UBSAN)
			list(APPEND sanitizers undefined)
		endif()

		list(JOIN sanitizers "," sanitizer_list)

		target_compile_options(${target} PRIVATE
			-fsanitize=${sanitizer_list}
			-fno-omit-frame-pointer
		)
		target_link_options(${target} PRIVATE
			-fsanitize=${sanitizer_list}
		)
	endif()
endfunction()

# ---------------------------------------------------------------------------
# Main executable
# ---------------------------------------------------------------------------

add_executable(compiler
	src/main.cpp
	src/frontend/syntax/ast/ast_dump.cpp
	src/frontend/lexer/lexer.cpp
	src/frontend/parser/parse_decl.cpp
	src/frontend/parser/parse_expr.cpp
	src/frontend/parser/parse_stmt.cpp
	src/frontend/parser/parse_type.cpp
)
target_link_libraries(compiler PRIVATE compiler_headers)
compiler_apply_settings(compiler)

target_precompile_headers(compiler PRIVATE
    <optional>
    <variant>
    <vector>
    <string>
    <string_view>
    <utility>
    <stdexcept>
)

if(COMPILER_ENABLE_CLANG_TIDY)
	find_program(COMPILER_CLANG_TIDY NAMES clang-tidy REQUIRED)
	set_target_properties(compiler PROPERTIES
		CXX_CLANG_TIDY "${COMPILER_CLANG_TIDY}"
	)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

if(COMPILER_ENABLE_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()
