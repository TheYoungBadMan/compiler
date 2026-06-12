// frontend/parser/parse_stmt.cpp

#include <optional>

#include "frontend/parser/parser.hpp"

namespace compiler::frontend::parser {

	namespace {

		std::optional<AssignOp> lookup_assign_op(TokenKind kind) noexcept {
			switch (kind) {
				case TokenKind::Equal: return AssignOp::Assign;
				case TokenKind::PlusEqual: return AssignOp::AddAssign;
				case TokenKind::MinusEqual: return AssignOp::SubAssign;
				case TokenKind::StarEqual: return AssignOp::MulAssign;
				case TokenKind::SlashEqual: return AssignOp::DivAssign;
				case TokenKind::PercentEqual: return AssignOp::ModAssign;
				case TokenKind::AmpEqual: return AssignOp::BitAndAssign;
				case TokenKind::BarEqual: return AssignOp::BitOrAssign;
				case TokenKind::CaretEqual: return AssignOp::BitXorAssign;
				case TokenKind::LessLessEqual: return AssignOp::ShlAssign;
				case TokenKind::GreaterGreaterEqual: return AssignOp::ShrAssign;
				default:
					return std::nullopt;
			}
		}

	} // namespace

	StmtNodeId Parser::parse_stmt() {
		if (cursor_.check(TokenKind::Var)) {
			return parse_decl_stmt();
		}
		if (cursor_.check(TokenKind::Const)) {
			return parse_decl_stmt(true);
		}
		if (cursor_.check(TokenKind::If)) {
			return parse_if_stmt();
		}
		if (cursor_.check(TokenKind::While)) {
			return parse_while_stmt();
		}
		if (cursor_.check(TokenKind::Return)) {
			return parse_return_stmt();
		}
		if (cursor_.check(TokenKind::Break)) {
			return parse_break_stmt();
		}
		if (cursor_.check(TokenKind::Continue)) {
			return parse_continue_stmt();
		}
		if (cursor_.check(TokenKind::Pass)) {
			return parse_pass_stmt();
		}
		return parse_expr_or_assign_stmt();
	}

	StmtNodeId Parser::parse_decl_stmt(bool is_const) {
		auto start = cursor_.offset();
		auto decl = parse_var_decl(is_const);
		return make_stmt<DeclStmtNode>(start, decl);
	}

	StmtNodeId Parser::parse_if_stmt() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'if'
		cursor_.expect(TokenKind::LeftParen, "Expected '(' after 'if'");
		auto cond = parse_expr();
		cursor_.expect(TokenKind::RightParen, "Expected ')' after if condition");
		auto then_body = parse_block();

		if (cursor_.match(TokenKind::Else)) {
			auto else_body = parse_block();
			return make_stmt<IfStmtNode>(start, cond, then_body, else_body);
		}

		return make_stmt<IfStmtNode>(start, cond, then_body, std::nullopt);
	}

	StmtNodeId Parser::parse_while_stmt() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'while'
		cursor_.expect(TokenKind::LeftParen, "Expected '(' after 'while'");
		auto cond = parse_expr();
		cursor_.expect(TokenKind::RightParen, "Expected ')' after while condition");

		auto body = parse_block();
		return make_stmt<WhileStmtNode>(start, cond, body);
	}

	StmtNodeId Parser::parse_return_stmt() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'return'
		if (cursor_.match(TokenKind::Semicolon)) {
			return make_stmt<ReturnStmtNode>(start);
		}

		auto expr = parse_expr();
		cursor_.expect(TokenKind::Semicolon, "Expected ';' after return value");
		return make_stmt<ReturnStmtNode>(start, expr);
	}

	StmtNodeId Parser::parse_break_stmt() {
		auto start = cursor_.offset();
		cursor_.advance(); // consume 'break'
		cursor_.expect(TokenKind::Semicolon, "Expected ';' after 'break'");
		return make_stmt<BreakStmtNode>(start);
	}

	StmtNodeId Parser::parse_continue_stmt() {
		auto start = cursor_.offset();
		cursor_.advance(); // consume 'continue'
		cursor_.expect(TokenKind::Semicolon, "Expected ';' after 'continue'");
		return make_stmt<ContinueStmtNode>(start);
	}

	StmtNodeId Parser::parse_pass_stmt() {
		auto start = cursor_.offset();
		cursor_.advance(); // consume 'pass'
		cursor_.expect(TokenKind::Semicolon, "Expected ';' after 'pass'");
		return make_stmt<PassStmtNode>(start);
	}

	StmtNodeId Parser::parse_expr_or_assign_stmt() {
		auto start = cursor_.offset();

		auto lhs = parse_expr();

		if (auto op = lookup_assign_op(cursor_.peek_kind())) {
			cursor_.advance();
			auto rhs = parse_expr();
			cursor_.expect(TokenKind::Semicolon, "Expected ';' after assignment");
			return make_stmt<AssignStmtNode>(start, lhs, *op, rhs);
		}

		cursor_.expect(TokenKind::Semicolon, "Expected ';' after expression");
		return make_stmt<ExprStmtNode>(start, lhs);
	}

} // namespace compiler::frontend::parser
