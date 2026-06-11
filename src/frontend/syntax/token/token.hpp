// frontend/syntax/token/token.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_HPP

#include <vector>

#include "core/span.hpp"

#include "frontend/syntax/token/token_kind.hpp"

namespace compiler::frontend {

	struct Token {
		TokenKind kind;
		SourceSpan span;
	};

	using TokenList = std::vector<Token>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_HPP
