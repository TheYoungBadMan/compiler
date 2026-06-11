// frontend/syntax/op/op.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_OP_OP_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_OP_OP_HPP

#include "support/primitives.hpp"

namespace compiler::frontend {

	enum class BinaryOp : u8 {
		Add,
		Sub,
		Mul,
		Div,
		Mod,
		Eq,
		Neq,
		Lt,
		Gt,
		Leq,
		Geq,
		LogAnd,
		LogOr,
		BitAnd,
		BitOr,
		BitXor,
		Shl,
		Shr,
	};

	enum class UnaryOp : u8 {
		Pos,
		Neg,
		LogNot,
		BitNot,
		Deref,
		AddrOf,
	};

	enum class AssignOp : u8 {
		Assign,
		AddAssign,
		SubAssign,
		MulAssign,
		DivAssign,
		ModAssign,
		BitAndAssign,
		BitOrAssign,
		BitXorAssign,
		ShlAssign,
		ShrAssign,
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_OP_OP_HPP
