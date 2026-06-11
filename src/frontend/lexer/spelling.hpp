// frontend/lexer/spelling.hpp

#ifndef COMPILER_SRC_FRONTEND_LEXER_SPELLING_HPP
#define COMPILER_SRC_FRONTEND_LEXER_SPELLING_HPP

#include <array>
#include <string_view>

#include "frontend/syntax/token/token_kind.hpp"

namespace compiler::frontend::lexer {

	struct Spelling {
		std::string_view name;
		TokenKind kind;
	};

	constexpr std::array keywords = std::to_array<Spelling>({
		{.name = "var", .kind = TokenKind::Var},
		{.name = "const", .kind = TokenKind::Const},
		{.name = "fn", .kind = TokenKind::Fn},
		{.name = "record", .kind = TokenKind::Record},
		{.name = "alias", .kind = TokenKind::Alias},
		{.name = "namespace", .kind = TokenKind::Namespace},
		{.name = "if", .kind = TokenKind::If},
		{.name = "else", .kind = TokenKind::Else},
		{.name = "while", .kind = TokenKind::While},
		{.name = "return", .kind = TokenKind::Return},
		{.name = "break", .kind = TokenKind::Break},
		{.name = "continue", .kind = TokenKind::Continue},
		{.name = "pass", .kind = TokenKind::Pass},
		{.name = "inf", .kind = TokenKind::Inf},
		{.name = "nan", .kind = TokenKind::Nan},
		{.name = "true", .kind = TokenKind::True},
		{.name = "false", .kind = TokenKind::False},
		{.name = "null", .kind = TokenKind::Null},
		{.name = "as", .kind = TokenKind::As},
	});

	constexpr TokenKind lookup_keyword(std::string_view ident) noexcept {
		for (auto [name, kind] : keywords) {
			if (name == ident) {
				return kind;
			}
		}

		return TokenKind::Ident;
	}

	constexpr std::array three_char_symbols = std::to_array<Spelling>({
		{.name = "<<=", .kind = TokenKind::LessLessEqual},
		{.name = ">>=", .kind = TokenKind::GreaterGreaterEqual},
	});

	constexpr std::array two_char_symbols = std::to_array<Spelling>({
		{.name = "==", .kind = TokenKind::EqualEqual},
		{.name = "!=", .kind = TokenKind::BangEqual},
		{.name = "<=", .kind = TokenKind::LessEqual},
		{.name = ">=", .kind = TokenKind::GreaterEqual},
		{.name = "&&", .kind = TokenKind::AmpAmp},
		{.name = "||", .kind = TokenKind::BarBar},
		{.name = "<<", .kind = TokenKind::LessLess},
		{.name = ">>", .kind = TokenKind::GreaterGreater},
		{.name = "+=", .kind = TokenKind::PlusEqual},
		{.name = "-=", .kind = TokenKind::MinusEqual},
		{.name = "*=", .kind = TokenKind::StarEqual},
		{.name = "/=", .kind = TokenKind::SlashEqual},
		{.name = "%=", .kind = TokenKind::PercentEqual},
		{.name = "&=", .kind = TokenKind::AmpEqual},
		{.name = "|=", .kind = TokenKind::BarEqual},
		{.name = "^=", .kind = TokenKind::CaretEqual},
		{.name = "->", .kind = TokenKind::Arrow},
		{.name = "::", .kind = TokenKind::ColonColon},
	});

	constexpr std::array one_char_symbols = std::to_array<Spelling>({
		{.name = "+", .kind = TokenKind::Plus},
		{.name = "-", .kind = TokenKind::Minus},
		{.name = "*", .kind = TokenKind::Star},
		{.name = "/", .kind = TokenKind::Slash},
		{.name = "%", .kind = TokenKind::Percent},
		{.name = "=", .kind = TokenKind::Equal},
		{.name = "!", .kind = TokenKind::Bang},
		{.name = "<", .kind = TokenKind::Less},
		{.name = ">", .kind = TokenKind::Greater},
		{.name = "&", .kind = TokenKind::Amp},
		{.name = "|", .kind = TokenKind::Bar},
		{.name = "^", .kind = TokenKind::Caret},
		{.name = "~", .kind = TokenKind::Tilde},
		{.name = "(", .kind = TokenKind::LeftParen},
		{.name = ")", .kind = TokenKind::RightParen},
		{.name = "[", .kind = TokenKind::LeftBracket},
		{.name = "]", .kind = TokenKind::RightBracket},
		{.name = "{", .kind = TokenKind::LeftBrace},
		{.name = "}", .kind = TokenKind::RightBrace},
		{.name = ";", .kind = TokenKind::Semicolon},
		{.name = ":", .kind = TokenKind::Colon},
		{.name = ",", .kind = TokenKind::Comma},
		{.name = ".", .kind = TokenKind::Dot},
	});

} // namespace compiler::frontend::lexer

#endif // COMPILER_SRC_FRONTEND_LEXER_SPELLING_HPP
