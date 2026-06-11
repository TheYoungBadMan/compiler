// frontend/lexer/chars.hpp

#ifndef COMPILER_SRC_FRONTEND_LEXER_CHARS_HPP
#define COMPILER_SRC_FRONTEND_LEXER_CHARS_HPP

namespace compiler::frontend::lexer {

	constexpr bool is_whitespace(unsigned char c) noexcept {
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	}

	constexpr bool is_alpha(unsigned char c) noexcept {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	constexpr bool is_digit(unsigned char c) noexcept {
		return c >= '0' && c <= '9';
	}

	constexpr bool is_bin_digit(unsigned char c) noexcept {
		return c == '0' || c == '1';
	}

	constexpr bool is_oct_digit(unsigned char c) noexcept {
		return c >= '0' && c <= '7';
	}

	constexpr bool is_hex_digit(unsigned char c) noexcept {
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
	}

	constexpr bool is_alnum(unsigned char c) noexcept {
		return is_alpha(c) || is_digit(c);
	}

	constexpr bool is_ident_start(unsigned char c) noexcept {
		return is_alpha(c) || c == '_';
	}

	constexpr bool is_ident_cont(unsigned char c) noexcept {
		return is_alnum(c) || c == '_';
	}

	constexpr bool is_integer_suffix(unsigned char c) noexcept {
		return c == 'u' || c == 'i';
	}

	constexpr bool is_exponent_start(unsigned char c) noexcept {
		return c == 'e' || c == 'E';
	}

	constexpr bool is_sign(unsigned char c) noexcept {
		return c == '+' || c == '-';
	}

} // namespace compiler::frontend::lexer

#endif // COMPILER_SRC_FRONTEND_LEXER_CHARS_HPP
