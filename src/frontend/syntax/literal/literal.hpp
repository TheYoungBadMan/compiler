// frontend/syntax/literal/literal.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_LITERAL_LITERAL_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_LITERAL_LITERAL_HPP

#include <variant>

#include "support/primitives.hpp"

namespace compiler::frontend {

	enum class IntBase : u8 {
		Bin,
		Oct,
		Dec,
		Hex
	};

	enum class Signedness : u8 {
		Unspecified,
		Signed,
		Unsigned
	};

	struct IntLiteral {
		IntBase base;
		Signedness signedness;
	};

	enum class FloatKind : u8 {
		Finite,
		Inf,
		NaN
	};

	struct FloatLiteral {
		FloatKind kind;
	};

	struct BoolLiteral {
		bool value;
	};

	struct NullLiteral {};

	using Literal = std::variant<IntLiteral, FloatLiteral, BoolLiteral, NullLiteral>;

	inline Literal make_int(IntBase base, Signedness signedness) noexcept {
		return IntLiteral{
			.base = base,
			.signedness = signedness
		};
	}

	inline Literal make_float(FloatKind kind) noexcept {
		return FloatLiteral{
			.kind = kind
		};
	}

	inline Literal make_bool(bool value) noexcept {
		return BoolLiteral{
			.value = value
		};
	}

	inline Literal make_null() noexcept {
		return NullLiteral{};
	}

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_LITERAL_LITERAL_HPP
