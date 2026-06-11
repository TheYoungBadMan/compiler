// frontend/syntax/op/op_dump.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_OP_OP_DUMP_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_OP_OP_DUMP_HPP

#include <string_view>

#include "frontend/syntax/op/op.hpp"

#include "support/debug.hpp"

namespace compiler::frontend {

	[[nodiscard]] inline std::string_view dump(BinaryOp op) noexcept {
		switch (op) {
			case BinaryOp::Add: return "+";
			case BinaryOp::Sub: return "-";
			case BinaryOp::Mul: return "*";
			case BinaryOp::Div: return "/";
			case BinaryOp::Mod: return "%";
			case BinaryOp::Eq: return "==";
			case BinaryOp::Neq: return "!=";
			case BinaryOp::Lt: return "<";
			case BinaryOp::Gt: return ">";
			case BinaryOp::Leq: return "<=";
			case BinaryOp::Geq: return ">=";
			case BinaryOp::LogAnd: return "&&";
			case BinaryOp::LogOr: return "||";
			case BinaryOp::BitAnd: return "&";
			case BinaryOp::BitOr: return "|";
			case BinaryOp::BitXor: return "^";
			case BinaryOp::Shl: return "<<";
			case BinaryOp::Shr: return ">>";
		}

		debug_unreachable("invalid binary operator");
	}

	[[nodiscard]] inline std::string_view dump(UnaryOp op) noexcept {
		switch (op) {
			case UnaryOp::Pos: return "+";
			case UnaryOp::Neg: return "-";
			case UnaryOp::LogNot: return "!";
			case UnaryOp::BitNot: return "~";
			case UnaryOp::Deref: return "*";
			case UnaryOp::AddrOf: return "&";
		}

		debug_unreachable("invalid unary operator");
	}

	[[nodiscard]] inline std::string_view dump(AssignOp op) noexcept {
		switch (op) {
			case AssignOp::Assign: return "=";
			case AssignOp::AddAssign: return "+=";
			case AssignOp::SubAssign: return "-=";
			case AssignOp::MulAssign: return "*=";
			case AssignOp::DivAssign: return "/=";
			case AssignOp::ModAssign: return "%=";
			case AssignOp::BitAndAssign: return "&=";
			case AssignOp::BitOrAssign: return "|=";
			case AssignOp::BitXorAssign: return "^=";
			case AssignOp::ShlAssign: return "<<=";
			case AssignOp::ShrAssign: return ">>=";
		}

		debug_unreachable("invalid assignment operator");
	}

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_OP_OP_DUMP_HPP
