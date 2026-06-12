// frontend/syntax/ast/type.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_TYPE_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_TYPE_HPP

#include <variant>
#include <vector>

#include "core/span.hpp"

#include "frontend/syntax/ast/name.hpp"
#include "frontend/syntax/ast/node_id.hpp"

#include "support/pool.hpp"

namespace compiler::frontend {

	struct PointerTypeNode {
		TypeNodeId pointee_type;
	};

	struct ArrayTypeNode {
		TypeNodeId elem_type;
		ExprNodeId size;
	};

	struct ParenTypeNode {
		TypeNodeId inner_type;
	};

	struct FnTypeNode {
		TypeNodeId ret_type;
		TypeNodeIdList param_types;
	};

	struct NamedTypeNode {
		Name name;
	};

	using TypeNode = std::variant<
		PointerTypeNode, ArrayTypeNode,
		ParenTypeNode, FnTypeNode, NamedTypeNode
	>;

	using TypeNodeList = std::vector<TypeNode>;

	using TypeNodePool = Pool<TypeNodeId, TypeNode>;
	using TypeNodeSpanPool = Pool<TypeNodeId, SourceSpan>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_TYPE_HPP
