// frontend/syntax/ast/ast_dump.cpp

#include "frontend/syntax/ast/ast_dump.hpp"

#include "support/variant.hpp"

#include "frontend/syntax/literal/literal_dump.hpp"
#include "frontend/syntax/op/op_dump.hpp"

namespace compiler::frontend {

	// Dispatchers

	void AstDumper::dump_module() {
		append("Module:\n");
		IndentGuard guard(indent_level_);
		for (auto decl : module_.global_decls()) {
			dump_decl(decl);
		}
	}

	void AstDumper::dump_decl(DeclNodeId id) {
		indent();
		const auto& decl = module_.decl(id);
		match(
			decl,
			[&](const VarDeclNode& node) { dump_var_decl(node); },
			[&](const FnDeclNode& node) { dump_fn_decl(node); },
			[&](const RecordDeclNode& node) { dump_record_decl(node); },
			[&](const AliasDeclNode& node) { dump_alias_decl(node); },
			[&](const NamespaceDeclNode& node) { dump_namespace_decl(node); }
		);
	}

	void AstDumper::dump_stmt(StmtNodeId id) {
		indent();
		const auto& stmt = module_.stmt(id);
		match(
			stmt,
			[&](const DeclStmtNode& node) { dump_decl_stmt(node); },
			[&](const IfStmtNode& node) { dump_if_stmt(node); },
			[&](const WhileStmtNode& node) { dump_while_stmt(node); },
			[&](const ReturnStmtNode& node) { dump_return_stmt(node); },
			[&](const BreakStmtNode& node) { dump_break_stmt(node); },
			[&](const ContinueStmtNode& node) { dump_continue_stmt(node); },
			[&](const PassStmtNode& node) { dump_pass_stmt(node); },
			[&](const ExprStmtNode& node) { dump_expr_stmt(node); },
			[&](const AssignStmtNode& node) { dump_assign_stmt(node); }
		);
	}

	void AstDumper::dump_expr(ExprNodeId id, bool indented) {
		if (indented) {
			indent();
		}
		const auto& expr = module_.expr(id);
		match(
			expr,
			[&](const BinaryExprNode& node) { dump_binary_expr(node); },
			[&](const UnaryExprNode& node) { dump_unary_expr(node); },
			[&](const CallExprNode& node) { dump_call_expr(node); },
			[&](const IndexExprNode& node) { dump_index_expr(node); },
			[&](const AccessExprNode& node) { dump_access_expr(node); },
			[&](const CastExprNode& node) { dump_cast_expr(node); },
			[&](const ParenExprNode& node) { dump_paren_expr(node); },
			[&](const ArrayExprNode& node) { dump_array_expr(node); },
			[&](const IdentExprNode& node) { dump_ident_expr(node); },
			[&](const RecordExprNode& node) { dump_record_expr(node); },
			[&](const LiteralExprNode& node) { dump_literal_expr(node); }
		);
	}

	void AstDumper::dump_labeled_expr(std::string_view label, ExprNodeId id) {
		append_indented(label);
		dump_expr(id, false);
	}

	void AstDumper::dump_type(TypeNodeId id, bool indented) {
		if (indented) {
			indent();
		}
		const auto& type = module_.type(id);
		match(
			type,
			[&](const PointerTypeNode& node) { dump_pointer_type(node); },
			[&](const ArrayTypeNode& node) { dump_array_type(node); },
			[&](const ParenTypeNode& node) { dump_paren_type(node); },
			[&](const FnTypeNode& node) { dump_fn_type(node); },
			[&](const NamedTypeNode& node) { dump_named_type(node); }
		);
	}

	void AstDumper::dump_labeled_type(std::string_view label, TypeNodeId id) {
		append_indented(label);
		dump_type(id, false);
	}

	void AstDumper::dump_block(BlockNodeId id) {
		const auto& block = module_.block(id);
		if (block.stmts.empty()) {
			append(" <empty>\n");
			return;
		}
		newline();
		IndentGuard guard(indent_level_);
		for (auto stmt : block.stmts) {
			dump_stmt(stmt);
		}
	}

	// Declarations

	void AstDumper::dump_var_decl(const VarDeclNode& node) {
		append("VarDecl ");
		append(node.is_const ? "[const] " : "[var] ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		dump_labeled_type("type: ", node.type);
		if (node.init) {
			dump_labeled_expr("init: ", *node.init);
		}
	}

	void AstDumper::dump_fn_decl(const FnDeclNode& node) {
		append("FnDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		for (const auto& param : node.params) {
			indent();
			append("param ");
			dump_ident(param.name);
			append(": ");
			dump_type(param.type, false);
		}
		dump_labeled_type("ret: ", node.ret_type);
		append_indented("body:");
		dump_block(node.body);
	}

	void AstDumper::dump_record_decl(const RecordDeclNode& node) {
		append("RecordDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		for (const auto& field : node.fields) {
			indent();
			append("field ");
			dump_ident(field.name);
			append(": ");
			dump_type(field.type, false);
		}
	}

	void AstDumper::dump_alias_decl(const AliasDeclNode& node) {
		append("AliasDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		dump_type(node.type);
	}

	void AstDumper::dump_namespace_decl(const NamespaceDeclNode& node) {
		append("NamespaceDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		for (auto decl : node.decls) {
			dump_decl(decl);
		}
	}

	// Statements

	void AstDumper::dump_decl_stmt(const DeclStmtNode& node) {
		append("DeclStmt\n");
		IndentGuard guard(indent_level_);
		dump_decl(node.decl);
	}

	void AstDumper::dump_if_stmt(const IfStmtNode& node) {
		append("IfStmt\n");
		IndentGuard guard(indent_level_);
		dump_labeled_expr("cond: ", node.cond);
		append_indented("then:");
		dump_block(node.then_body);
		if (node.else_body) {
			append_indented("else:");
			dump_block(*node.else_body);
		}
	}

	void AstDumper::dump_while_stmt(const WhileStmtNode& node) {
		append("WhileStmt\n");
		IndentGuard guard(indent_level_);
		dump_labeled_expr("cond: ", node.cond);
		append_indented("body:");
		dump_block(node.body);
	}

	void AstDumper::dump_return_stmt(const ReturnStmtNode& node) {
		append("ReturnStmt\n");
		if (node.value) {
			IndentGuard guard(indent_level_);
			dump_labeled_expr("value: ", *node.value);
		}
	}

	void AstDumper::dump_break_stmt([[maybe_unused]] const BreakStmtNode& node) {
		append("BreakStmt\n");
	}

	void AstDumper::dump_continue_stmt([[maybe_unused]] const ContinueStmtNode& node) {
		append("ContinueStmt\n");
	}

	void AstDumper::dump_pass_stmt([[maybe_unused]] const PassStmtNode& node) {
		append("PassStmt\n");
	}

	void AstDumper::dump_expr_stmt(const ExprStmtNode& node) {
		append("ExprStmt\n");
		IndentGuard guard(indent_level_);
		dump_expr(node.expr);
	}

	void AstDumper::dump_assign_stmt(const AssignStmtNode& node) {
		append(std::format("AssignStmt [{}]\n", to_string(node.op)));
		IndentGuard guard(indent_level_);
		dump_expr(node.lhs);
		dump_expr(node.rhs);
	}

	// Expressions

	void AstDumper::dump_binary_expr(const BinaryExprNode& node) {
		append(std::format("BinaryExpr [{}]\n", to_string(node.op)));
		IndentGuard guard(indent_level_);
		dump_expr(node.lhs);
		dump_expr(node.rhs);
	}

	void AstDumper::dump_unary_expr(const UnaryExprNode& node) {
		append(std::format("UnaryExpr [{}]\n", to_string(node.op)));
		IndentGuard guard(indent_level_);
		dump_expr(node.base);
	}

	void AstDumper::dump_call_expr(const CallExprNode& node) {
		append("CallExpr\n");
		IndentGuard guard(indent_level_);
		dump_expr(node.callee);
		for (auto arg : node.args) {
			dump_expr(arg);
		}
	}

	void AstDumper::dump_index_expr(const IndexExprNode& node) {
		append("IndexExpr\n");
		IndentGuard guard(indent_level_);
		dump_expr(node.base);
		dump_expr(node.index);
	}

	void AstDumper::dump_access_expr(const AccessExprNode& node) {
		append("AccessExpr ");
		dump_ident(node.member);
		newline();
		IndentGuard guard(indent_level_);
		dump_expr(node.base);
	}

	void AstDumper::dump_cast_expr(const CastExprNode& node) {
		append("CastExpr\n");
		IndentGuard guard(indent_level_);
		dump_expr(node.base);
		dump_type(node.type);
	}

	void AstDumper::dump_paren_expr(const ParenExprNode& node) {
		append("ParenExpr\n");
		IndentGuard guard(indent_level_);
		dump_expr(node.base);
	}

	void AstDumper::dump_array_expr(const ArrayExprNode& node) {
		append("ArrayExpr\n");
		IndentGuard guard(indent_level_);
		for (auto elem : node.elems) {
			dump_expr(elem);
		}
	}

	void AstDumper::dump_ident_expr(const IdentExprNode& node) {
		append("IdentExpr ");
		dump_name(node.name);
		newline();
	}

	void AstDumper::dump_record_expr(const RecordExprNode& node) {
		append("RecordExpr ");
		dump_name(node.name);
		newline();
		IndentGuard guard(indent_level_);
		for (const auto& field : node.fields) {
			indent();
			dump_ident(field.name);
			append(": ");
			dump_expr(field.init, false);
		}
	}

	void AstDumper::dump_literal_expr(const LiteralExprNode& node) {
		match(
			node.value,
			[&](IntLiteral lit) {
				append("IntLiteral ");
				append(to_string(lit.base));
				append(" ");
				append(to_string(lit.signedness));
				newline();
			},
			[&](FloatLiteral lit) {
				append("FloatLiteral ");
				append(to_string(lit.kind));
				newline();
			},
			[&](BoolLiteral lit) {
				append(lit.value ? "BoolLiteral true\n" : "BoolLiteral false\n");
			},
			[&](NullLiteral) { append("NullLiteral\n"); }
		);
	}

	// Types

	void AstDumper::dump_pointer_type(const PointerTypeNode& node) {
		append("PointerType\n");
		IndentGuard guard(indent_level_);
		dump_type(node.pointee_type);
	}

	void AstDumper::dump_array_type(const ArrayTypeNode& node) {
		append("ArrayType\n");
		IndentGuard guard(indent_level_);
		dump_labeled_expr("size: ", node.size);
		dump_type(node.elem_type);
	}

	void AstDumper::dump_paren_type(const ParenTypeNode& node) {
		append("ParenType\n");
		IndentGuard guard(indent_level_);
		dump_type(node.inner_type);
	}

	void AstDumper::dump_fn_type(const FnTypeNode& node) {
		append("FnType\n");
		IndentGuard guard(indent_level_);
		for (const auto& param : node.param_types) {
			dump_labeled_type("param: ", param);
		}
		dump_labeled_type("ret: ", node.ret_type);
	}

	void AstDumper::dump_named_type(const NamedTypeNode& node) {
		append("NamedType ");
		dump_name(node.name);
		newline();
	}

	// Utilities

	void AstDumper::dump_ident(IdentId id) {
		append("'");
		append(interner_.get(id));
		append("'");
	}

	void AstDumper::dump_name(const Name& name) {
		for (auto qualifier : name.qualifiers) {
			dump_ident(qualifier);
			append("::");
		}
		dump_ident(name.name);
	}

} // namespace compiler::frontend
