// frontend/syntax/ast/ast_dump.cpp

#include "frontend/syntax/ast/ast_dump.hpp"

#include "frontend/syntax/literal/literal.hpp"
#include "frontend/syntax/op/op_dump.hpp"

namespace compiler::frontend {

	// Declarations

	void AstDumper::dump_var_decl(const VarDeclNode& node) {
		append_indented("VarDecl ");
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
		append_indented("FnDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		for (const auto& param : node.params) {
			append_indented("param ");
			dump_ident(param.name);
			append(": ");
			dump_type(param.type, false);
		}
		dump_labeled_type("ret: ", node.ret_type);
		dump_labeled_block("body:", node.body);
	}

	void AstDumper::dump_record_decl(const RecordDeclNode& node) {
		append_indented("RecordDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		for (const auto& field : node.fields) {
			append_indented("field ");
			dump_ident(field.name);
			append(": ");
			dump_type(field.type, false);
		}
	}

	void AstDumper::dump_alias_decl(const AliasDeclNode& node) {
		append_indented("AliasDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		dump_type(node.type);
	}

	void AstDumper::dump_namespace_decl(const NamespaceDeclNode& node) {
		append_indented("NamespaceDecl ");
		dump_ident(node.name);
		newline();

		IndentGuard guard(indent_level_);
		for (auto decl : node.decls) {
			dump_decl(decl);
		}
	}

	// Statements

	void AstDumper::dump_decl_stmt(const DeclStmtNode& node) {
		append_indented("DeclStmt\n");
		IndentGuard guard(indent_level_);
		dump_decl(node.decl);
	}

	void AstDumper::dump_if_stmt(const IfStmtNode& node) {
		append_indented("IfStmt\n");
		IndentGuard guard(indent_level_);
		dump_labeled_expr("cond: ", node.cond);
		dump_labeled_block("then:", node.then_body);
		if (node.else_body) {
			dump_labeled_block("else:", *node.else_body);
		}
	}

	void AstDumper::dump_while_stmt(const WhileStmtNode& node) {
		append_indented("WhileStmt\n");
		IndentGuard guard(indent_level_);
		dump_labeled_expr("cond: ", node.cond);
		dump_labeled_block("body:", node.body);
	}

	void AstDumper::dump_return_stmt(const ReturnStmtNode& node) {
		append_indented("ReturnStmt\n");
		if (node.value) {
			IndentGuard guard(indent_level_);
			dump_labeled_expr("value: ", *node.value);
		}
	}

	void AstDumper::dump_break_stmt([[maybe_unused]] const BreakStmtNode& node) {
		append_indented("BreakStmt\n");
	}

	void AstDumper::dump_continue_stmt([[maybe_unused]] const ContinueStmtNode& node) {
		append_indented("ContinueStmt\n");
	}

	void AstDumper::dump_pass_stmt([[maybe_unused]] const PassStmtNode& node) {
		append_indented("PassStmt\n");
	}

	void AstDumper::dump_expr_stmt(const ExprStmtNode& node) {
		append_indented("ExprStmt\n");
		IndentGuard guard(indent_level_);
		dump_expr(node.expr);
	}

	void AstDumper::dump_assign_stmt(const AssignStmtNode& node) {
		append_indented(std::format("AssignStmt [{}]\n", to_string(node.op)));
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
			append_indented("\"");
			dump_ident(field.name);
			append("\": ");
			dump_expr(field.init, false);
		}
	}

	void AstDumper::dump_literal_expr(const LiteralExprNode& node) {
		match(
			node.value,
			[&](IntLiteral literal) { dump_int_literal(literal); },
			[&](FloatLiteral literal) { dump_float_literal(literal); },
			[&](BoolLiteral literal) { dump_bool_literal(literal); },
			[&](NullLiteral) { append("NullLiteral\n"); }
		);
	}

	void AstDumper::dump_int_literal(IntLiteral literal) {
		append("IntLiteral ");
		switch (literal.base) {
			case IntBase::Bin: append("bin"); break;
			case IntBase::Oct: append("oct"); break;
			case IntBase::Dec: append("dec"); break;
			case IntBase::Hex: append("hex"); break;
		}
		switch (literal.signedness) {
			case Signedness::Signed: append(" signed"); break;
			case Signedness::Unsigned: append(" unsigned"); break;
			default: break;
		}
		newline();
	}

	void AstDumper::dump_float_literal(FloatLiteral literal) {
		append("FloatLiteral ");
		switch (literal.kind) {
			case FloatKind::Finite: append("finite"); break;
			case FloatKind::Inf: append("inf"); break;
			case FloatKind::NaN: append("nan"); break;
		}
		newline();
	}

	void AstDumper::dump_bool_literal(BoolLiteral literal) {
		append("BoolLiteral ");
		append(literal.value ? "true" : "false");
		newline();
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

} // namespace compiler::frontend
