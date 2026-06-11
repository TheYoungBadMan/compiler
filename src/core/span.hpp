// core/span.hpp

#ifndef COMPILER_SRC_CORE_SPAN_HPP
#define COMPILER_SRC_CORE_SPAN_HPP

#include <format>

#include "core/offset.hpp"

#include "support/debug.hpp"

namespace compiler {

	struct SourceSpan {
		Offset start;
		Offset end;

		[[nodiscard]] Offset length() const noexcept {
			debug_assert(end >= start, "invalid span bounds");
			return end - start;
		}
	};

} // namespace compiler

template <>
struct std::formatter<compiler::SourceSpan> {
	static constexpr auto parse(format_parse_context& ctx) noexcept {
		return ctx.begin();
	}

	static constexpr auto format(const compiler::SourceSpan& span, format_context& ctx) {
		return std::format_to(
			ctx.out(),
			"{}..{}",
			span.start,
			span.end
		);
	}
};

#endif // COMPILER_SRC_CORE_SPAN_HPP
