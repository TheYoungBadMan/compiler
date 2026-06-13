// frontend/syntax/literal/literal_dump.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_LITERAL_LITERAL_DUMP_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_LITERAL_LITERAL_DUMP_HPP

#include <string_view>

#include <support/debug.hpp>

#include "frontend/syntax/literal/literal.hpp"

namespace compiler::frontend {

	constexpr std::string_view to_string(IntBase base) noexcept {
		switch (base) {
			case IntBase::Bin: return "bin";
			case IntBase::Oct: return "oct";
			case IntBase::Dec: return "dec";
			case IntBase::Hex: return "hex";
		}

		debug_unreachable("invalid int literal base");
	}

	constexpr std::string_view to_string(Signedness signedness) noexcept {
		switch (signedness) {
			case Signedness::Unspecified: return "default";
			case Signedness::Signed: return "signed";
			case Signedness::Unsigned: return "unsigned";
		}

		debug_unreachable("invalid int literal signedness");
	}

	constexpr std::string_view to_string(FloatKind kind) noexcept {
		switch (kind) {
			case FloatKind::Finite: return "finite";
			case FloatKind::Inf: return "inf";
			case FloatKind::NaN: return "nan";
		}

		debug_unreachable("invalid float literal kind");
	}

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_LITERAL_LITERAL_DUMP_HPP
