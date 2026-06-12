// frontend/lexer/lexer.hpp

#ifndef COMPILER_SRC_FRONTEND_LEXER_LEXER_HPP
#define COMPILER_SRC_FRONTEND_LEXER_LEXER_HPP

#include <utility>

#include "core/source.hpp"

#include "frontend/lexer/error.hpp"
#include "frontend/lexer/scanner.hpp"
#include "frontend/syntax/token/token.hpp"

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

		void tokenize();

		void skip_trivia();
		void skip_block_comment();

		void lex_token();
		void lex_ident();
		void lex_number();
		void lex_radix_number(auto&& predicate, ErrorKind error_kind);
		void lex_dec_number();
		void lex_exponent();
		void lex_symbol();
	};

	inline LexerResult run(const Source& source) {
		return Lexer::run(source);
	}

} // namespace compiler::frontend::lexer

#endif // COMPILER_SRC_FRONTEND_LEXER_LEXER_HPP
