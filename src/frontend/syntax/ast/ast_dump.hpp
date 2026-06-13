// frontend/syntax/ast/ast_dump.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_AST_DUMP_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_AST_DUMP_HPP

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
		// Input
		const Module& module_;
		const IdentInterner& interner_;

		// State
		u64 indent_level_ = 0;
		std::string result_;

		explicit AstDumper(const Module& module, const IdentInterner& interner) noexcept
			: module_(module), interner_(interner) {}

		// IndentGuard

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

		// Helpers

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

		// Dispatchers

		void dump_module();
		void dump_decl(DeclNodeId id);
		void dump_stmt(StmtNodeId id);
		void dump_expr(ExprNodeId id, bool indented = true);
		void dump_labeled_expr(std::string_view label, ExprNodeId id);
		void dump_type(TypeNodeId id, bool indented = true);
		void dump_labeled_type(std::string_view label, TypeNodeId id);
		void dump_block(BlockNodeId id);

		// Declarations

		void dump_var_decl(const VarDeclNode& node);
		void dump_fn_decl(const FnDeclNode& node);
		void dump_record_decl(const RecordDeclNode& node);
		void dump_alias_decl(const AliasDeclNode& node);
		void dump_namespace_decl(const NamespaceDeclNode& node);

		// Statements

		void dump_decl_stmt(const DeclStmtNode& node);
		void dump_if_stmt(const IfStmtNode& node);
		void dump_while_stmt(const WhileStmtNode& node);
		void dump_return_stmt(const ReturnStmtNode& node);
		void dump_break_stmt(const BreakStmtNode& node);
		void dump_continue_stmt(const ContinueStmtNode& node);
		void dump_pass_stmt(const PassStmtNode& node);
		void dump_expr_stmt(const ExprStmtNode& node);
		void dump_assign_stmt(const AssignStmtNode& node);

		// Expressions

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

		// Types

		void dump_pointer_type(const PointerTypeNode& node);
		void dump_array_type(const ArrayTypeNode& node);
		void dump_paren_type(const ParenTypeNode& node);
		void dump_fn_type(const FnTypeNode& node);
		void dump_named_type(const NamedTypeNode& node);

		// Ident/Name

		void dump_ident(IdentId id);
		void dump_name(const Name& name);
	};

	inline std::string dump(const Module& module, const IdentInterner& interner) {
		return AstDumper::dump(module, interner);
	}

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_AST_DUMP_HPP
