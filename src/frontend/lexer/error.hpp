// frontend/lexer/error.hpp

#ifndef COMPILER_SRC_FRONTEND_LEXER_ERROR_HPP
#define COMPILER_SRC_FRONTEND_LEXER_ERROR_HPP

#include <vector>

#include "core/span.hpp"

#include "support/primitives.hpp"

namespace compiler::frontend::lexer {

	enum class ErrorKind : u8 {
		UnexpectedCharacter,
		UnterminatedBlockComment,
		InvalidBinaryLiteral,
		InvalidOctalLiteral,
		InvalidHexadecimalLiteral,
		InvalidExponent,
	};

	struct Error {
		ErrorKind kind;
		SourceSpan span;
	};

	using ErrorList = std::vector<Error>;

} // namespace compiler::frontend::lexer

#endif // COMPILER_SRC_FRONTEND_LEXER_ERROR_HPP
