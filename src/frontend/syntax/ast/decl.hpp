// frontend/syntax/ast/decl.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_DECL_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_DECL_HPP

#include <optional>
#include <variant>
#include <vector>

#include "core/ident.hpp"
#include "core/span.hpp"

#include "frontend/syntax/ast/node_id.hpp"

#include "support/pool.hpp"

namespace compiler::frontend {

	struct VarDeclNode {
		bool is_const;
		IdentId name;
		TypeNodeId type;
		std::optional<ExprNodeId> init;
	};

	struct ParamDecl {
		bool is_const;
		IdentId name;
		TypeNodeId type;
	};

	using ParamDeclList = std::vector<ParamDecl>;

	struct FnDeclNode {
		IdentId name;
		ParamDeclList params;
		TypeNodeId ret_type;
		BlockNodeId body;
	};

	struct FieldDecl {
		IdentId name;
		TypeNodeId type;
	};

	using FieldDeclList = std::vector<FieldDecl>;

	struct RecordDeclNode {
		IdentId name;
		FieldDeclList fields;
	};

	struct AliasDeclNode {
		IdentId name;
		TypeNodeId type;
	};

	struct NamespaceDeclNode {
		IdentId name;
		DeclNodeIdList decls;
	};

	using DeclNode = std::variant<
		VarDeclNode, FnDeclNode, RecordDeclNode, AliasDeclNode, NamespaceDeclNode
	>;

	using DeclNodeList = std::vector<DeclNode>;

	using DeclNodePool = Pool<DeclNodeId, DeclNode>;
	using DeclNodeSpanPool = Pool<DeclNodeId, SourceSpan>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_DECL_HPP
