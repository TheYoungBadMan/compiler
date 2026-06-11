// frontend/syntax/token/token_kind.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_KIND_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_KIND_HPP

#include "support/primitives.hpp"

namespace compiler::frontend {

	enum class TokenKind : u8 {
		Ident,

		// Keywords
		Var,
		Const,
		Fn,
		Record,
		Alias,
		Namespace,
		If,
		Else,
		While,
		Return,
		Break,
		Continue,
		Pass,
		Inf,
		Nan,
		True,
		False,
		Null,
		As,

		// Literals
		Int,
		Float,

		// Three-char symbols
		LessLessEqual,
		GreaterGreaterEqual, // <<=, >>=

		// Two-char symbols
		EqualEqual,
		BangEqual,
		LessEqual,
		GreaterEqual,
		AmpAmp,
		BarBar, // &&, ||
		LessLess,
		GreaterGreater, // <<, >>
		PlusEqual,
		MinusEqual,
		StarEqual,
		SlashEqual,
		PercentEqual, // +=, -=, *=, /=, %=,
		AmpEqual,
		BarEqual,
		CaretEqual, // &=, |=, ^=,
		Arrow,
		ColonColon, // ->, ::

		// One-char symbols
		Plus,
		Minus,
		Star,
		Slash,
		Percent, // + - * / %
		Equal,
		Bang,
		Less,
		Greater,
		Amp,
		Bar,
		Caret,
		Tilde, // & | ^ ~
		LeftParen,
		RightParen, // ( )
		LeftBracket,
		RightBracket, // [ ]
		LeftBrace,
		RightBrace, // { }
		Semicolon,
		Colon,
		Comma,
		Dot, // ; : , .

		// Special
		Eof,
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_KIND_HPP
