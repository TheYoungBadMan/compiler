// frontend/parser/cursor.hpp

#ifndef COMPILER_SRC_FRONTEND_PARSER_CURSOR_HPP
#define COMPILER_SRC_FRONTEND_PARSER_CURSOR_HPP

#include <stdexcept>
#include <string_view>
#include <vector>

#include "core/source.hpp"

#include "support/primitives.hpp"
#include "support/debug.hpp"

#include "frontend/syntax/token/token.hpp"

namespace compiler::frontend::parser {

	class Cursor {
	public:
		explicit Cursor(const std::vector<Token>& tokens, const Source& source) noexcept
			: tokens_(tokens), source_(source) {}

		[[nodiscard]] Offset offset() const noexcept {
			debug_assert(pos_ < tokens_.size(), "position past end of tokens");
			return tokens_[pos_].span.start;
		}

		[[nodiscard]] TokenKind peek_kind() const noexcept {
			debug_assert(pos_ < tokens_.size(), "peeking past end of tokens");
			return tokens_[pos_].kind;
		}

		[[nodiscard]] std::string_view peek_lexeme() const noexcept {
			debug_assert(pos_ < tokens_.size(), "peeking past end of tokens");
			return source_.slice(tokens_[pos_].span);
		}

		[[nodiscard]] bool check(TokenKind kind) const noexcept {
			return peek_kind() == kind;
		}

		bool match(TokenKind kind) noexcept {
			if (!check(kind)) {
				return false;
			}

			advance();
			return true;
		}

		[[nodiscard]] bool is_eof() const noexcept {
			return peek_kind() == TokenKind::Eof;
		}

		std::string_view take() noexcept {
			auto lexeme = peek_lexeme();
			advance();
			return lexeme;
		}

		std::string_view consume(TokenKind kind, const std::string& message) {
			if (!check(kind)) {
				throw std::runtime_error(message);
			}

			auto lexeme = peek_lexeme();
			advance();
			return lexeme;
		}

		void expect(TokenKind kind, const std::string& message) {
			if (!check(kind)) {
				throw std::runtime_error(message);
			}

			advance();
		}

		void advance() noexcept {
			debug_assert(pos_ < tokens_.size(), "advancing past end of tokens");
			++pos_;
		}

	private:
		// Input
		const std::vector<Token>& tokens_;

		// State
		usize pos_ = 0;
		const Source& source_;
	};

} // namespace compiler::frontend::parser

#endif // COMPILER_SRC_FRONTEND_PARSER_CURSOR_HPP
