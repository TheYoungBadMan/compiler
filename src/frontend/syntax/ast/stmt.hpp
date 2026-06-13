// frontend/syntax/ast/stmt.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_STMT_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_STMT_HPP

#include <optional>
#include <variant>
#include <vector>

#include "core/span.hpp"

#include "frontend/syntax/ast/node_id.hpp"
#include "frontend/syntax/op/op.hpp"

#include "support/pool.hpp"

namespace compiler::frontend {

	struct DeclStmtNode {
		DeclNodeId decl;
	};

	struct ExprStmtNode {
		ExprNodeId expr;
	};

	struct AssignStmtNode {
		ExprNodeId lhs;
		AssignOp op;
		ExprNodeId rhs;
	};

	struct IfStmtNode {
		ExprNodeId cond;
		BlockNodeId then_body;
		std::optional<BlockNodeId> else_body;
	};

	struct WhileStmtNode {
		ExprNodeId cond;
		BlockNodeId body;
	};

	struct ReturnStmtNode {
		std::optional<ExprNodeId> value;
	};

	struct BreakStmtNode {};
	struct ContinueStmtNode {};
	struct PassStmtNode {};

	using StmtNode = std::variant<
		DeclStmtNode, ExprStmtNode, AssignStmtNode,
		IfStmtNode, WhileStmtNode, ReturnStmtNode,
		BreakStmtNode, ContinueStmtNode, PassStmtNode
	>;

	using StmtNodeList = std::vector<StmtNode>;

	using StmtNodePool = Pool<StmtNode, StmtNodeId>;
	using StmtNodeSpanPool = Pool<SourceSpan, StmtNodeId>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_STMT_HPP
