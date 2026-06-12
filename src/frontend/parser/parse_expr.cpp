// frontend/parser/parse_expr.cpp

#include <optional>
#include <utility>

#include "frontend/parser/parser.hpp"
#include "frontend/syntax/ast/expr.hpp"

namespace compiler::frontend::parser {

	namespace {

		std::optional<std::pair<BinaryOp, u8>> lookup_binary_op(TokenKind kind) noexcept {
			switch (kind) {
				case TokenKind::BarBar: return std::make_pair(BinaryOp::LogOr, 1);
				case TokenKind::AmpAmp: return std::make_pair(BinaryOp::LogAnd, 2);
				case TokenKind::EqualEqual: return std::make_pair(BinaryOp::Eq, 3);
				case TokenKind::BangEqual: return std::make_pair(BinaryOp::Neq, 3);
				case TokenKind::Less: return std::make_pair(BinaryOp::Lt, 4);
				case TokenKind::Greater: return std::make_pair(BinaryOp::Gt, 4);
				case TokenKind::LessEqual: return std::make_pair(BinaryOp::Leq, 4);
				case TokenKind::GreaterEqual: return std::make_pair(BinaryOp::Geq, 4);
				case TokenKind::Bar: return std::make_pair(BinaryOp::BitOr, 5);
				case TokenKind::Caret: return std::make_pair(BinaryOp::BitXor, 6);
				case TokenKind::Amp: return std::make_pair(BinaryOp::BitAnd, 7);
				case TokenKind::LessLess: return std::make_pair(BinaryOp::Shl, 8);
				case TokenKind::GreaterGreater: return std::make_pair(BinaryOp::Shr, 8);
				case TokenKind::Plus: return std::make_pair(BinaryOp::Add, 9);
				case TokenKind::Minus: return std::make_pair(BinaryOp::Sub, 9);
				case TokenKind::Star: return std::make_pair(BinaryOp::Mul, 10);
				case TokenKind::Slash: return std::make_pair(BinaryOp::Div, 10);
				case TokenKind::Percent: return std::make_pair(BinaryOp::Mod, 10);
				default: return std::nullopt;
			}
		}

		std::optional<UnaryOp> lookup_unary_op(TokenKind kind) noexcept {
			switch (kind) {
				case TokenKind::Plus: return UnaryOp::Pos;
				case TokenKind::Minus: return UnaryOp::Neg;
				case TokenKind::Bang: return UnaryOp::LogNot;
				case TokenKind::Tilde: return UnaryOp::BitNot;
				case TokenKind::Star: return UnaryOp::Deref;
				case TokenKind::Amp: return UnaryOp::AddrOf;
				default: return std::nullopt;
			}
		}

	} // namespace

	ExprNodeId Parser::parse_expr() {
		return parse_binary_expr();
	}

	ExprNodeId Parser::parse_binary_expr(u8 min_precedence) {
		auto start = cursor_.offset();
		auto lhs = parse_prefix_expr();
		while (true) {
			auto op_info = lookup_binary_op(cursor_.peek_kind());
			if (!op_info || op_info->second < min_precedence) {
				break;
			}

			auto [op, precedence] = *op_info;
			cursor_.advance(); // consume operator
			auto rhs = parse_binary_expr(precedence + 1);
			lhs = make_expr<BinaryExprNode>(start, lhs, op, rhs);
		}
		return lhs;
	}

	ExprNodeId Parser::parse_prefix_expr() {
		if (auto op = lookup_unary_op(cursor_.peek_kind())) {
			auto start = cursor_.offset();
			cursor_.advance(); // consume operator
			auto base = parse_prefix_expr();
			return make_expr<UnaryExprNode>(start, *op, base);
		}

		return parse_postfix_expr();
	}

	ExprNodeId Parser::parse_postfix_expr() {
		auto start = cursor_.offset();

		auto expr = parse_primary_expr();
		while (true) {
			if (cursor_.match(TokenKind::LeftParen)) {
				ExprNodeIdList args;
				if (!cursor_.match(TokenKind::RightParen)) {
					do {
						args.push_back(parse_expr());
					} while (cursor_.match(TokenKind::Comma) && !cursor_.check(TokenKind::RightParen));
					cursor_.expect(TokenKind::RightParen, "Expected ')' after arguments");
				}
				expr = make_expr<CallExprNode>(start, expr, args);
			} else if (cursor_.match(TokenKind::LeftBracket)) {
				auto index = parse_expr();
				cursor_.expect(TokenKind::RightBracket, "Expected ']' after index");
				expr = make_expr<IndexExprNode>(start, expr, index);
			} else if (cursor_.match(TokenKind::Dot)) {
				auto member = parse_ident();
				expr = make_expr<AccessExprNode>(start, expr, member);
			} else if (cursor_.match(TokenKind::As)) {
				auto type = parse_type();
				expr = make_expr<CastExprNode>(start, expr, type);
			} else {
				break;
			}
		}
		return expr;
	}

	ExprNodeId Parser::parse_primary_expr() {
		if (cursor_.check(TokenKind::LeftParen)) {
			return parse_paren_expr();
		}
		if (cursor_.check(TokenKind::LeftBracket)) {
			return parse_array_literal();
		}
		if (cursor_.check(TokenKind::Ident)) {
			return parse_ident_expr();
		}
		if (cursor_.check(TokenKind::Int)) {
			return parse_int_literal();
		}
		if (cursor_.check(TokenKind::Float)) {
			auto start = cursor_.offset();
			cursor_.advance(); // consume float literal
			return make_expr<LiteralExprNode>(
				start,
				make_float(FloatKind::Finite)
			);
		}
		if (cursor_.check(TokenKind::True)) {
			auto start = cursor_.offset();
			cursor_.advance(); // consume 'true'
			return make_expr<LiteralExprNode>(
				start,
				make_bool(true)
			);
		}
		if (cursor_.check(TokenKind::False)) {
			auto start = cursor_.offset();
			cursor_.advance(); // consume 'false'
			return make_expr<LiteralExprNode>(
				start,
				make_bool(false)
			);
		}
		if (cursor_.check(TokenKind::Inf)) {
			auto start = cursor_.offset();
			cursor_.advance(); // consume 'inf'
			return make_expr<LiteralExprNode>(
				start,
				make_float(FloatKind::Inf)
			);
		}
		if (cursor_.check(TokenKind::Nan)) {
			auto start = cursor_.offset();
			cursor_.advance(); // consume 'nan'
			return make_expr<LiteralExprNode>(
				start,
				make_float(FloatKind::NaN)
			);
		}
		if (cursor_.check(TokenKind::Null)) {
			auto start = cursor_.offset();
			cursor_.advance(); // consume 'null'
			return make_expr<LiteralExprNode>(
				start,
				make_null()
			);
		}

		throw std::runtime_error("Expected expression");
	}

	ExprNodeId Parser::parse_paren_expr() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume '('
		auto expr = parse_expr();
		cursor_.expect(TokenKind::RightParen, "Expected ')' after '(' in parenthesized expression");
		return make_expr<ParenExprNode>(start, expr);
	}

	ExprNodeId Parser::parse_array_literal() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume '['
		std::vector<ExprNodeId> elems;
		if (!cursor_.match(TokenKind::RightBracket)) {
			do {
				elems.push_back(parse_expr());
			} while (cursor_.match(TokenKind::Comma) && !cursor_.check(TokenKind::RightBracket));
			cursor_.expect(TokenKind::RightBracket, "Expected ']' after array literal");
		}
		return make_expr<ArrayExprNode>(start, elems);
	}

	ExprNodeId Parser::parse_ident_expr() {
		auto start = cursor_.offset();

		auto name = parse_name();
		if (!cursor_.match(TokenKind::LeftBrace)) {
			return make_expr<IdentExprNode>(start, name);
		}

		FieldInitList fields;
		if (!cursor_.match(TokenKind::RightBrace)) {
			do {
				auto field_name = parse_ident();
				cursor_.expect(TokenKind::Equal, "Expected '=' after field name");
				auto init = parse_expr();
				fields.emplace_back(field_name, init);
			} while (cursor_.match(TokenKind::Comma) && !cursor_.check(TokenKind::RightBrace));
			cursor_.expect(TokenKind::RightBrace, "Expected '}' after record fields");
		}

		return make_expr<RecordExprNode>(start, name, fields);
	}

	ExprNodeId Parser::parse_int_literal() {
		auto start = cursor_.offset();

		auto lexeme = cursor_.peek_lexeme();
		auto base = IntBase::Dec;
		if (lexeme.starts_with("0b") || lexeme.starts_with("0B")) {
			base = IntBase::Bin;
		} else if (lexeme.starts_with("0o") || lexeme.starts_with("0O")) {
			base = IntBase::Oct;
		} else if (lexeme.starts_with("0x") || lexeme.starts_with("0X")) {
			base = IntBase::Hex;
		}

		auto signedness = Signedness::Unspecified;
		if (lexeme.ends_with("u")) {
			signedness = Signedness::Unsigned;
		} else if (lexeme.ends_with("i")) {
			signedness = Signedness::Signed;
		}

		cursor_.advance(); // consume int literal
		return make_expr<LiteralExprNode>(
			start,
			make_int(base, signedness)
		);
	}

} // namespace compiler::frontend::parser
