// support/debug.hpp

#ifndef COMPILER_SRC_SUPPORT_DEBUG_HPP
#define COMPILER_SRC_SUPPORT_DEBUG_HPP

#include <cstdio>
#include <cstdlib>
#include <source_location>

#ifndef NDEBUG

namespace compiler::detail {

	[[noreturn]] inline void fail(
		const char* kind, const char* expr, const char* file, int line,
		const char* func, const char* msg
	) noexcept {
		std::fprintf(stderr, "%s: %s at %s:%d in %s: %s\n",
			kind, expr, file, line, func, msg);
		std::fflush(stderr);
		std::abort();
	}

	[[noreturn]] inline void fail(
		const char* kind, const char* file, int line,
		const char* func, const char* msg
	) noexcept {
		std::fprintf(stderr, "%s at %s:%d in %s: %s\n",
			kind, file, line, func, msg);
		std::fflush(stderr);
		std::abort();
	}

} // namespace compiler::detail

	#define debug_assert(expr, msg) \
		do { \
			if (!(expr)) { \
				compiler::detail::fail("assertion failed", #expr, __FILE__, __LINE__, __func__, (msg)); \
			} \
		} while (0)

	#define debug_unreachable(msg) \
		compiler::detail::fail("unreachable code reached", __FILE__, __LINE__, __func__, (msg))

#else
	#define debug_assert(expr, msg) ((void)0)

	#include <utility>

	#define debug_unreachable(msg) std::unreachable()
#endif

#define pass(msg) ((void)0)
#define todo(msg) compiler::panic("TODO: " msg)

namespace compiler {

	[[noreturn]] inline void panic(
		const char* msg,
		std::source_location loc = std::source_location::current()
	) noexcept {
		std::fprintf(stderr, "panic: %s at %s:%d in %s\n",
			msg, loc.file_name(), loc.line(), loc.function_name());
		std::fflush(stderr);
		std::abort();
	}

} // namespace compiler

#endif // COMPILER_SRC_SUPPORT_DEBUG_HPP
