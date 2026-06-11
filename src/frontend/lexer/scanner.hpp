// frontend/lexer/scanner.hpp

#ifndef COMPILER_SRC_FRONTEND_LEXER_SCANNER_HPP
#define COMPILER_SRC_FRONTEND_LEXER_SCANNER_HPP

#include <string_view>

#include "core/offset.hpp"
#include "core/source.hpp"
#include "core/span.hpp"

#include "support/debug.hpp"

namespace compiler::frontend::lexer {

	class Scanner {
	public:
		explicit Scanner(const Source& source) noexcept
			: source_(source) {}

		[[nodiscard]] SourceSpan span() const noexcept {
			return SourceSpan{
				.start = start_,
				.end = pos_,
			};
		}

		[[nodiscard]] std::string_view slice() const noexcept {
			return source_.slice(start_, pos_);
		}

		[[nodiscard]] unsigned char peek() const noexcept {
			return peek(0);
		}

		[[nodiscard]] bool is_eof() const noexcept {
			return is_eof(0);
		}

		[[nodiscard]] bool check(unsigned char c) const noexcept {
			return peek() == c;
		}

		bool check_if(auto&& predicate) const noexcept {
			return !is_eof() && predicate(peek());
		}

		bool match(unsigned char c) noexcept {
			if (!check(c)) {
				return false;
			}

			advance();
			return true;
		}

		bool match(std::string_view str) noexcept {
			if (!check(str)) {
				return false;
			}

			advance(static_cast<Offset>(str.size()));
			return true;
		}

		bool match_if(auto&& predicate) noexcept {
			if (!check_if(predicate)) {
				return false;
			}

			advance();
			return true;
		}

		void advance(Offset n = 1) noexcept {
			debug_assert(n <= source_.size() - pos_, "advancing past the end of the source");
			pos_ += n;
		}

		void advance_while(auto&& predicate) noexcept {
			while (check_if(predicate)) {
				advance();
			}
		}

		void advance_until(unsigned char c) noexcept {
			while (!is_eof() && !check(c)) {
				advance();
			}
		}

		void sync() noexcept {
			start_ = pos_;
		}

	private:
		// Input
		const Source& source_;

		// State
		Offset start_ = 0;
		Offset pos_ = 0;

		[[nodiscard]] bool check(std::string_view str) const noexcept {
			debug_assert(!str.empty(), "checking for an empty string");

			if (is_eof(static_cast<Offset>(str.size() - 1))) {
				return false;
			}

			return source_.slice(pos_, pos_ + static_cast<Offset>(str.size())) == str;
		}

		[[nodiscard]] bool is_eof(Offset offset) const noexcept {
			return offset >= source_.size() - pos_;
		}

		[[nodiscard]] unsigned char peek(Offset offset) const noexcept {
			return is_eof(offset) ? '\0' : source_[pos_ + offset];
		}
	};

} // namespace compiler::frontend::lexer

#endif // COMPILER_SRC_FRONTEND_LEXER_SCANNER_HPP
