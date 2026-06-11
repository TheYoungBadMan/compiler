// frontend/syntax/ast/node_id.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_NODE_ID_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_NODE_ID_HPP

#include <vector>

#include "support/id.hpp"

namespace compiler::frontend {

	enum class DeclNodeId : Id {};
	enum class StmtNodeId : Id {};
	enum class ExprNodeId : Id {};
	enum class TypeNodeId : Id {};
	enum class BlockNodeId : Id {};

	using DeclNodeIdList = std::vector<DeclNodeId>;
	using StmtNodeIdList = std::vector<StmtNodeId>;
	using ExprNodeIdList = std::vector<ExprNodeId>;
	using TypeNodeIdList = std::vector<TypeNodeId>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_NODE_ID_HPP
