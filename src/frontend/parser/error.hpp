// frontend/parser/error.hpp

#ifndef COMPILER_FRONTEND_PARSER_ERROR_HPP
#define COMPILER_FRONTEND_PARSER_ERROR_HPP

#include <vector>

#include "support/primitives.hpp"

#include "core/span.hpp"

namespace compiler::frontend::parser {

	enum class ErrorKind : u8 {
	};

	struct Error {
		ErrorKind kind;
		SourceSpan span;
	};

	using ErrorList = std::vector<Error>;

} // namespace compiler::frontend::parser

#endif // COMPILER_FRONTEND_PARSER_ERROR_HPP
