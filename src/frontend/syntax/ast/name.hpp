// frontend/syntax/ast/name.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_NAME_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_NAME_HPP

#include "core/ident.hpp"

namespace compiler::frontend {

	struct Name {
		IdentIdList qualifiers;
		IdentId name;
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_NAME_HPP
