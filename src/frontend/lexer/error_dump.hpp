// frontend/lexer/error_dump.hpp

#ifndef COMPILER_SRC_FRONTEND_LEXER_ERROR_DUMP_HPP
#define COMPILER_SRC_FRONTEND_LEXER_ERROR_DUMP_HPP

#include <format>
#include <string>
#include <string_view>

#include "core/source.hpp"

#include "frontend/lexer/error.hpp"

#include "support/debug.hpp"

namespace compiler::frontend::lexer {

	[[nodiscard]] inline std::string_view dump(ErrorKind kind) {
		switch (kind) {
			case ErrorKind::UnexpectedCharacter: return "unexpected character";
			case ErrorKind::UnterminatedBlockComment: return "unterminated block comment";
			case ErrorKind::InvalidBinaryLiteral: return "invalid binary literal";
			case ErrorKind::InvalidOctalLiteral: return "invalid octal literal";
			case ErrorKind::InvalidHexadecimalLiteral: return "invalid hexadecimal literal";
			case ErrorKind::InvalidExponent: return "invalid exponent";
		}

		debug_unreachable("invalid lex error kind");
	}

	[[nodiscard]] inline std::string dump(const Error& error, const Source& source) {
		return std::format("{} at {} [{}]: {}",
			dump(error.kind),
			source.loc(error.span.start),
			error.span,
			source.slice(error.span));
	}

	[[nodiscard]] inline std::string dump(const ErrorList& errors, const Source& source) {
		std::string result;
		for (const auto& error : errors) {
			result.append(std::format("{}\n", dump(error, source)));
		}
		return result;
	}

} // namespace compiler::frontend::lexer

#endif // COMPILER_SRC_FRONTEND_LEXER_ERROR_DUMP_HPP
