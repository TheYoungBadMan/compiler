// frontend/frontend.hpp

#ifndef COMPILER_SRC_FRONTEND_FRONTEND_HPP
#define COMPILER_SRC_FRONTEND_FRONTEND_HPP

#include "core/source.hpp"
#include "core/ident.hpp"

#include "frontend/syntax/ast/module.hpp"

#include "frontend/lexer/lexer.hpp"
#include "frontend/parser/parser.hpp"

namespace compiler::frontend {

	using LexErrorList = lexer::ErrorList;

	struct FrontendResult {
		TokenList tokens;
		LexErrorList lexer_errors;

		Module module;
		IdentInterner interner;

		[[nodiscard]] bool has_errors() const noexcept {
			return !lexer_errors.empty();
		}
	};

	inline FrontendResult run(const Source& source) {
		auto lexer_result = lexer::run(source);
		auto parser_result = parser::run(lexer_result.tokens, source);

		return FrontendResult{.tokens = std::move(lexer_result.tokens),
			.lexer_errors = std::move(lexer_result.errors),
			.module = std::move(parser_result.module),
			.interner = std::move(parser_result.interner)};
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_FRONTEND_HPP
