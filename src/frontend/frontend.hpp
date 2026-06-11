// frontend/frontend.hpp

#ifndef COMPILER_SRC_FRONTEND_FRONTEND_HPP
#define COMPILER_SRC_FRONTEND_FRONTEND_HPP

#include "core/source.hpp"

#include "frontend/lexer/lexer.hpp"

namespace compiler::frontend {

	using LexErrorList = lexer::ErrorList;

	struct FrontendResult {
		TokenList tokens;
		LexErrorList lexer_errors;

		[[nodiscard]] bool has_errors() const noexcept {
			return !lexer_errors.empty();
		}
	};

	inline FrontendResult run(const Source& source) {
		auto lexer_result = lexer::run(source);
		return {std::move(lexer_result.tokens), std::move(lexer_result.errors)};
	}

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_FRONTEND_HPP
