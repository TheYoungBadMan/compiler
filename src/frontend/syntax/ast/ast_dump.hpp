// frontend/syntax/ast/ast_dump.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_AST_DUMP_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_AST_DUMP_HPP

#include "support/variant.hpp"

#include "frontend/syntax/ast/module.hpp"

namespace compiler::frontend {

	class AstDumper {
	public:
		static std::string dump(const Module& module, const IdentInterner& interner) {
			AstDumper dumper(module, interner);
			dumper.dump_module();
			return std::move(dumper.result_);
		}

	private:
		// State
		const Module& module_;
		const IdentInterner& interner_;
		u64 indent_level_ = 0;
		std::string result_;

		explicit AstDumper(const Module& module, const IdentInterner& interner) noexcept
			: module_(module), interner_(interner) {}

		// Helpers

		class IndentGuard {
		public:

			explicit IndentGuard(u64& indent_level) noexcept : indent_level_(indent_level) {
				indent_level_++;
			}

			~IndentGuard() noexcept {
				indent_level_--;
			}

			IndentGuard(const IndentGuard&) = delete;
			IndentGuard& operator=(const IndentGuard&) = delete;
			IndentGuard(IndentGuard&&) = delete;
			IndentGuard& operator=(IndentGuard&&) = delete;

		private:
			u64& indent_level_;
		};

		void indent() {
			result_.append(indent_level_ * 2, ' ');
		}

		void newline() {
			result_.push_back('\n');
		}

		void append(std::string_view text) {
			result_.append(text);
		}

		void append_indented(std::string_view text) {
			indent();
			result_.append(text);
		}

		void append_line(std::string_view text) {
			indent();
			result_.append(text);
			newline();
		}

		// Dumping
		void dump_module() {
			append_line("Module:");
			for (auto decl : module_.global_decls()) {
				dump_decl(decl);
			}
		}

		void dump_decl(DeclNodeId id) {
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

		void dump_var_decl(const VarDeclNode& node);
		void dump_fn_decl(const FnDeclNode& node);
		void dump_record_decl(const RecordDeclNode& node);
		void dump_alias_decl(const AliasDeclNode& node);
		void dump_namespace_decl(const NamespaceDeclNode& node);

		void dump_stmt(StmtNodeId id) {
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

		void dump_decl_stmt(const DeclStmtNode& node);
		void dump_if_stmt(const IfStmtNode& node);
		void dump_while_stmt(const WhileStmtNode& node);
		void dump_return_stmt(const ReturnStmtNode& node);
		void dump_break_stmt(const BreakStmtNode& node);
		void dump_continue_stmt(const ContinueStmtNode& node);
		void dump_pass_stmt(const PassStmtNode& node);
		void dump_expr_stmt(const ExprStmtNode& node);
		void dump_assign_stmt(const AssignStmtNode& node);

		void dump_expr(ExprNodeId id, bool indented = true) {
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

		void dump_binary_expr(const BinaryExprNode& node);
		void dump_unary_expr(const UnaryExprNode& node);
		void dump_call_expr(const CallExprNode& node);
		void dump_index_expr(const IndexExprNode& node);
		void dump_access_expr(const AccessExprNode& node);
		void dump_cast_expr(const CastExprNode& node);
		void dump_paren_expr(const ParenExprNode& node);
		void dump_array_expr(const ArrayExprNode& node);
		void dump_ident_expr(const IdentExprNode& node);
		void dump_record_expr(const RecordExprNode& node);
		void dump_literal_expr(const LiteralExprNode& node);

		void dump_int_literal(IntLiteral literal);
		void dump_float_literal(FloatLiteral literal);
		void dump_bool_literal(BoolLiteral literal);

		void dump_type(TypeNodeId id, bool indented = true) {
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

		void dump_pointer_type(const PointerTypeNode& node);
		void dump_array_type(const ArrayTypeNode& node);
		void dump_paren_type(const ParenTypeNode& node);
		void dump_fn_type(const FnTypeNode& node);
		void dump_named_type(const NamedTypeNode& node);

		void dump_block(BlockNodeId id) {
			const auto& block = module_.block(id);
			IndentGuard indent_guard(indent_level_);
			for (auto stmt : block.stmts) {
				dump_stmt(stmt);
			}
		}

		void dump_ident(IdentId id) {
			result_.append(std::format("'{}'", interner_.get(id)));
		}

		void dump_name(const Name& name) {
			for (auto qualifier : name.qualifiers) {
				dump_ident(qualifier);
				append("::");
			}
			dump_ident(name.name);
		}
	};

	inline std::string dump(const Module& module, const IdentInterner& interner) {
		return AstDumper::dump(module, interner);
	}

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_AST_DUMP_HPP
