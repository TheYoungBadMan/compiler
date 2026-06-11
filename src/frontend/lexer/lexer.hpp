// frontend/lexer/lexer.hpp

#ifndef COMPILER_SRC_FRONTEND_LEXER_LEXER_HPP
#define COMPILER_SRC_FRONTEND_LEXER_LEXER_HPP

#include <utility>

#include "core/source.hpp"

#include "frontend/lexer/chars.hpp"
#include "frontend/lexer/error.hpp"
#include "frontend/lexer/scanner.hpp"
#include "frontend/lexer/spelling.hpp"
#include "frontend/syntax/token/token.hpp"

#include "support/primitives.hpp"

namespace compiler::frontend::lexer {

	struct LexerResult {
		TokenList tokens;
		ErrorList errors;

		[[nodiscard]] bool has_errors() const noexcept {
			return !errors.empty();
		}
	};

	class Lexer {
	public:
		static LexerResult run(const Source& source) {
			Lexer lexer(source);
			lexer.tokenize();

			lexer.tokens_.shrink_to_fit();
			lexer.errors_.shrink_to_fit();

			return LexerResult{
				.tokens = std::move(lexer.tokens_),
				.errors = std::move(lexer.errors_),
			};
		}

	private:

		// State
		Scanner scanner_;
		TokenList tokens_;
		ErrorList errors_;

		explicit Lexer(const Source& source) noexcept
			: scanner_(source) {}

		void emit(TokenKind kind) {
			tokens_.push_back(Token{
				.kind = kind,
				.span = scanner_.span(),
			});
		}

		void error(ErrorKind kind) {
			errors_.push_back(Error{
				.kind = kind,
				.span = scanner_.span(),
			});
		}

		void tokenize() {
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

		void skip_trivia() {
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

		void skip_block_comment() {
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

		void lex_token() {
			scanner_.sync();

			if (scanner_.match_if(is_ident_start)) {
				lex_ident();
			} else if (scanner_.check_if(is_digit)) {
				lex_number();
			} else {
				lex_symbol();
			}
		}

		void lex_ident() {
			scanner_.advance_while(is_ident_cont);

			emit(lookup_keyword(scanner_.slice()));
		}

		void lex_number() {
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

		void lex_radix_number(auto&& predicate, ErrorKind error_kind) {
			if (!scanner_.match_if(predicate)) {
				emit(TokenKind::Int);
				error(error_kind);
				return;
			}

			scanner_.advance_while(predicate);

			scanner_.match_if(is_integer_suffix);

			emit(TokenKind::Int);
		}

		void lex_dec_number() {
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

		void lex_exponent() {
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

		void lex_symbol() {
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
	};

	inline LexerResult run(const Source& source) {
		return Lexer::run(source);
	}

} // namespace compiler::frontend::lexer

#endif // COMPILER_SRC_FRONTEND_LEXER_LEXER_HPP
