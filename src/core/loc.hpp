// core/loc.hpp

#ifndef COMPILER_SRC_CORE_LOC_HPP
#define COMPILER_SRC_CORE_LOC_HPP

#include <format>

#include "support/primitives.hpp"

namespace compiler {

	struct SourceLoc {
		u32 line;
		u32 column;

		bool operator==(const SourceLoc& other) const noexcept = default;
	};

} // namespace compiler

template <>
struct std::formatter<compiler::SourceLoc> {
	static constexpr auto parse(format_parse_context& ctx) noexcept {
		return ctx.begin();
	}

	static constexpr auto format(const compiler::SourceLoc& loc, format_context& ctx) {
		return std::format_to(
			ctx.out(),
			"{}:{}",
			static_cast<compiler::u64>(loc.line) + 1,
			static_cast<compiler::u64>(loc.column) + 1
		);
	}
};

#endif // COMPILER_SRC_CORE_LOC_HPP
