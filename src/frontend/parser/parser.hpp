// frontend/parser/parser.hpp

#ifndef COMPILER_SRC_FRONTEND_PARSER_PARSER_HPP
#define COMPILER_SRC_FRONTEND_PARSER_PARSER_HPP

#include <string>
#include <utility>
#include <vector>

#include "core/ident.hpp"
#include "core/span.hpp"

#include "frontend/syntax/token/token.hpp"

#include "frontend/syntax/ast/module.hpp"

#include "frontend/parser/cursor.hpp"

namespace compiler::frontend::parser {

	struct ParserResult {
		Module module;
		IdentInterner interner;
	};

	class Parser {
	public:
		static ParserResult run(const std::vector<Token>& tokens, const Source& source) {
			Parser parser(tokens, source);
			parser.parse_module();
			return ParserResult{.module = std::move(parser.module_),
				.interner = std::move(parser.interner_)};
		}

	private:
		// State
		Cursor cursor_;
		Module module_;
		IdentInterner interner_;

		explicit Parser(const std::vector<Token>& tokens, const Source& source) noexcept
			: cursor_(tokens, source) {}

		void parse_module() {
			while (!cursor_.is_eof()) {
				auto decl = parse_decl();
				module_.add_global_decl(decl);
			}
		}

		IdentId parse_ident(const std::string& message = "Expected identifier") {
			auto lexeme = cursor_.consume(TokenKind::Ident, message);
			return interner_.intern(Ident(lexeme));
		}

		Name parse_name() {
			IdentIdList qualifiers;

			IdentId name = parse_ident();
			while (cursor_.match(TokenKind::ColonColon)) {
				qualifiers.push_back(name);
				name = parse_ident("Expected identifier after '::'");
			}

			return Name{.qualifiers = std::move(qualifiers), .name = name};
		}

		DeclNodeId parse_decl();
		DeclNodeId parse_var_decl(bool is_const = false);
		DeclNodeId parse_fn_decl();
		DeclNodeId parse_record_decl();
		DeclNodeId parse_alias_decl();
		DeclNodeId parse_namespace_decl();

		StmtNodeId parse_stmt();
		StmtNodeId parse_decl_stmt(bool is_const = false);
		StmtNodeId parse_if_stmt();
		StmtNodeId parse_while_stmt();
		StmtNodeId parse_return_stmt();
		StmtNodeId parse_break_stmt();
		StmtNodeId parse_continue_stmt();
		StmtNodeId parse_pass_stmt();
		StmtNodeId parse_expr_or_assign_stmt();

		ExprNodeId parse_expr();
		ExprNodeId parse_binary_expr(u8 min_precedence = 1);
		ExprNodeId parse_prefix_expr();
		ExprNodeId parse_postfix_expr();
		ExprNodeId parse_primary_expr();
		ExprNodeId parse_paren_expr();
		ExprNodeId parse_array_literal();
		ExprNodeId parse_ident_expr();
		ExprNodeId parse_int_literal();

		TypeNodeId parse_type();
		TypeNodeId parse_postfix_type();
		TypeNodeId parse_primary_type();
		TypeNodeId parse_fn_type();
		TypeNodeId parse_paren_type();
		TypeNodeId parse_named_type();

		BlockNodeId parse_block() {
			auto start = cursor_.offset();
			cursor_.expect(TokenKind::LeftBrace, "Expected '{' to start block");
			StmtNodeIdList stmts;
			while (!cursor_.check(TokenKind::RightBrace) && !cursor_.is_eof()) {
				stmts.push_back(parse_stmt());
			}
			cursor_.expect(TokenKind::RightBrace, "Expected '}' to end block");
			return make_block(start, std::move(stmts));
		}

		template <typename DeclType, typename... Args>
		DeclNodeId make_decl(Offset start, Args&&... args) {
			return module_.add_decl(DeclType{std::forward<Args>(args)...},
				SourceSpan{.start = start, .end = cursor_.offset()});
		}

		template <typename StmtType, typename... Args>
		StmtNodeId make_stmt(Offset start, Args&&... args) {
			return module_.add_stmt(StmtType{std::forward<Args>(args)...},
				SourceSpan{.start = start, .end = cursor_.offset()});
		}

		template <typename ExprType, typename... Args>
		ExprNodeId make_expr(Offset start, Args&&... args) {
			return module_.add_expr(ExprType{std::forward<Args>(args)...},
				SourceSpan{.start = start, .end = cursor_.offset()});
		}

		template <typename TypeType, typename... Args>
		TypeNodeId make_type(Offset start, Args&&... args) {
			return module_.add_type(TypeType{std::forward<Args>(args)...},
				SourceSpan{.start = start, .end = cursor_.offset()});
		}

		BlockNodeId make_block(Offset start, StmtNodeIdList stmts) {
			return module_.add_block(BlockNode{std::move(stmts)},
				SourceSpan{.start = start, .end = cursor_.offset()});
		}
	};

	inline ParserResult run(const std::vector<Token>& tokens, const Source& source) {
		return Parser::run(tokens, source);
	}

} // namespace compiler::frontend::parser

#endif // COMPILER_SRC_FRONTEND_PARSER_PARSER_HPP
