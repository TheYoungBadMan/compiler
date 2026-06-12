// frontend/lexer/lexer.cpp

#include "frontend/lexer/lexer.hpp"

#include "support/primitives.hpp"

#include "frontend/lexer/chars.hpp"
#include "frontend/lexer/spelling.hpp"

namespace compiler::frontend::lexer {

	void Lexer::tokenize() {
		while (!scanner_.is_eof()) {
			skip_trivia();
			if (scanner_.is_eof()) {
				break;
			}

			lex_token();
		}

		scanner_.sync();
		emit(TokenKind::Eof);
	}

	void Lexer::skip_trivia() {
		while (true) {
			scanner_.advance_while(is_whitespace);
			if (scanner_.match("//")) {
				scanner_.advance_until('\n');
			} else if (scanner_.match("/*")) {
				scanner_.sync();
				skip_block_comment();
			} else {
				break;
			}
		}
	}

	void Lexer::skip_block_comment() {
		for (u64 depth = 1; depth > 0;) {
			if (scanner_.is_eof()) {
				error(ErrorKind::UnterminatedBlockComment);
				return;
			}

			if (scanner_.match("/*")) {
				++depth;
			} else if (scanner_.match("*/")) {
				--depth;
			} else {
				scanner_.advance();
			}
		}
	}

	void Lexer::lex_token() {
		scanner_.sync();

		if (scanner_.match_if(is_ident_start)) {
			lex_ident();
		} else if (scanner_.check_if(is_digit)) {
			lex_number();
		} else {
			lex_symbol();
		}
	}

	void Lexer::lex_ident() {
		scanner_.advance_while(is_ident_cont);

		emit(lookup_keyword(scanner_.slice()));
	}

	void Lexer::lex_number() {
		if (scanner_.match('0')) {
			if (scanner_.match('b')) {
				lex_radix_number(is_bin_digit, ErrorKind::InvalidBinaryLiteral);
				return;
			}
			if (scanner_.match('o')) {
				lex_radix_number(is_oct_digit, ErrorKind::InvalidOctalLiteral);
				return;
			}
			if (scanner_.match('x')) {
				lex_radix_number(is_hex_digit, ErrorKind::InvalidHexadecimalLiteral);
				return;
			}
		}
		lex_dec_number();
	}

	void Lexer::lex_radix_number(auto&& predicate, ErrorKind error_kind) {
		if (!scanner_.match_if(predicate)) {
			emit(TokenKind::Int);
			error(error_kind);
			return;
		}

		scanner_.advance_while(predicate);

		scanner_.match_if(is_integer_suffix);

		emit(TokenKind::Int);
	}

	void Lexer::lex_dec_number() {
		scanner_.advance_while(is_digit);

		if (scanner_.match_if(is_exponent_start)) {
			lex_exponent();
			return;
		}

		if (scanner_.match('.')) {
			scanner_.advance_while(is_digit);
			if (scanner_.match_if(is_exponent_start)) {
				lex_exponent();
			} else {
				emit(TokenKind::Float);
			}
			return;
		}

		scanner_.match_if(is_integer_suffix);

		emit(TokenKind::Int);
	}

	void Lexer::lex_exponent() {
		if (scanner_.check_if(is_sign)) {
			scanner_.advance();
		}

		if (!scanner_.match_if(is_digit)) {
			emit(TokenKind::Float);
			error(ErrorKind::InvalidExponent);
			return;
		}

		scanner_.advance_while(is_digit);
		emit(TokenKind::Float);
	}

	void Lexer::lex_symbol() {
		for (auto [name, kind] : three_char_symbols) {
			if (scanner_.match(name)) {
				emit(kind);
				return;
			}
		}

		for (auto [name, kind] : two_char_symbols) {
			if (scanner_.match(name)) {
				emit(kind);
				return;
			}
		}

		for (auto [name, kind] : one_char_symbols) {
			if (scanner_.match(name)) {
				emit(kind);
				return;
			}
		}

		scanner_.advance();
		error(ErrorKind::UnexpectedCharacter);
	}

} // namespace compiler::frontend::lexer
