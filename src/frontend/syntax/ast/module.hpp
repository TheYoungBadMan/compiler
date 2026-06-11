// frontend/syntax/ast/module.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_MODULE_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_MODULE_HPP

#include <utility>

#include "frontend/syntax/ast/block.hpp"
#include "frontend/syntax/ast/decl.hpp"
#include "frontend/syntax/ast/expr.hpp"
#include "frontend/syntax/ast/stmt.hpp"
#include "frontend/syntax/ast/type.hpp"

namespace compiler::frontend {

	class Module {
	public:

		[[nodiscard]] const DeclNodeIdList& global_decls() const noexcept { return global_decls_; }
		void add_global_decl(DeclNodeId decl) noexcept { global_decls_.push_back(decl); }

		[[nodiscard]] const DeclNodePool& decls() const noexcept { return decls_; }
		[[nodiscard]] const StmtNodePool& stmts() const noexcept { return stmts_; }
		[[nodiscard]] const ExprNodePool& exprs() const noexcept { return exprs_; }
		[[nodiscard]] const TypeNodePool& types() const noexcept { return types_; }
		[[nodiscard]] const BlockNodePool& blocks() const noexcept { return blocks_; }

		[[nodiscard]] const DeclNode& decl(DeclNodeId id) const { return decls_.get(id); }
		[[nodiscard]] const StmtNode& stmt(StmtNodeId id) const { return stmts_.get(id); }
		[[nodiscard]] const ExprNode& expr(ExprNodeId id) const { return exprs_.get(id); }
		[[nodiscard]] const TypeNode& type(TypeNodeId id) const { return types_.get(id); }
		[[nodiscard]] const BlockNode& block(BlockNodeId id) const { return blocks_.get(id); }

		DeclNodeId add_decl(DeclNode decl) { return decls_.add(std::move(decl)); }
		StmtNodeId add_stmt(StmtNode stmt) { return stmts_.add(stmt); }
		ExprNodeId add_expr(ExprNode expr) { return exprs_.add(std::move(expr)); }
		TypeNodeId add_type(TypeNode type) { return types_.add(std::move(type)); }
		BlockNodeId add_block(BlockNode block) { return blocks_.add(std::move(block)); }

	private:
		DeclNodeIdList global_decls_;

		DeclNodePool decls_;
		StmtNodePool stmts_;
		ExprNodePool exprs_;
		TypeNodePool types_;
		BlockNodePool blocks_;

		DeclNodeSpanPool decl_spans_;
		StmtNodeSpanPool stmt_spans_;
		ExprNodeSpanPool expr_spans_;
		TypeNodeSpanPool type_spans_;
		BlockNodeSpanPool block_spans_;
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_MODULE_HPP
