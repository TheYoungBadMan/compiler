// frontend/syntax/ast/expr.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_EXPR_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_EXPR_HPP

#include <variant>
#include <vector>

#include "core/ident.hpp"
#include "core/span.hpp"

#include "frontend/syntax/ast/name.hpp"
#include "frontend/syntax/ast/node_id.hpp"
#include "frontend/syntax/literal/literal.hpp"
#include "frontend/syntax/op/op.hpp"

#include "support/pool.hpp"

namespace compiler::frontend {

	struct BinaryExprNode {
		ExprNodeId lhs;
		BinaryOp op;
		ExprNodeId rhs;
	};

	struct UnaryExprNode {
		UnaryOp op;
		ExprNodeId base;
	};

	struct CallExprNode {
		ExprNodeId callee;
		ExprNodeIdList args;
	};

	struct IndexExprNode {
		ExprNodeId base;
		ExprNodeId index;
	};

	struct AccessExprNode {
		ExprNodeId base;
		IdentId member;
	};

	struct CastExprNode {
		ExprNodeId base;
		TypeNodeId type;
	};

	struct ParenExprNode {
		ExprNodeId base;
	};

	struct ArrayExprNode {
		ExprNodeIdList elems;
	};

	struct FieldInit {
		IdentId name;
		ExprNodeId init;
	};

	using FieldInitList = std::vector<FieldInit>;

	struct RecordExprNode {
		Name name;
		FieldInitList fields;
	};

	struct IdentExprNode {
		Name name;
	};

	struct LiteralExprNode {
		Literal value;
	};

	using ExprNode = std::variant<
		BinaryExprNode, UnaryExprNode, CallExprNode, IndexExprNode,
		AccessExprNode, CastExprNode, ParenExprNode, ArrayExprNode,
		RecordExprNode, IdentExprNode, LiteralExprNode
	>;

	using ExprNodeList = std::vector<ExprNode>;

	using ExprNodePool = Pool<ExprNode, ExprNodeId>;
	using ExprNodeSpanPool = Pool<SourceSpan, ExprNodeId>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_EXPR_HPP
