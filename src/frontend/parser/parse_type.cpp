// frontend/parser/parse_type.cpp

#include "frontend/parser/parser.hpp"

namespace compiler::frontend::parser {

	TypeNodeId Parser::parse_type() {
		return parse_postfix_type();
	}

	TypeNodeId Parser::parse_postfix_type() {
		auto start = cursor_.offset();

		auto type = parse_primary_type();
		for (;;) {
			if (cursor_.match(TokenKind::Star)) {
				type = make_type<PointerTypeNode>(start, type);
			} else if (cursor_.match(TokenKind::LeftBracket)) {
				auto size = parse_expr();
				cursor_.expect(TokenKind::RightBracket, "Expected ']' after '[' in array type");
				type = make_type<ArrayTypeNode>(start, type, size);
			} else {
				break;
			}
		}

		return type;
	}

	TypeNodeId Parser::parse_primary_type() {
		if (cursor_.check(TokenKind::LeftParen)) {
			return parse_paren_type();
		}
		if (cursor_.check(TokenKind::Fn)) {
			return parse_fn_type();
		}
		if (cursor_.check(TokenKind::Ident)) {
			return parse_named_type();
		}

		throw std::runtime_error("Expected type");
	}

	TypeNodeId Parser::parse_paren_type() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume '('
		auto type = parse_type();
		cursor_.expect(TokenKind::RightParen, "Expected ')' after '(' in parenthesized type");
		return make_type<ParenTypeNode>(start, type);
	}

	TypeNodeId Parser::parse_fn_type() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'fn'
		cursor_.expect(TokenKind::LeftParen, "Expected '(' to start function type");
		TypeNodeIdList param_types;
		if (!cursor_.match(TokenKind::RightParen)) {
			do {
				param_types.push_back(parse_type());
			} while (cursor_.match(TokenKind::Comma) && !cursor_.check(TokenKind::RightParen));
			cursor_.expect(TokenKind::RightParen, "Expected ')' after parameter types in function type");
		}

		cursor_.expect(TokenKind::Arrow, "Expected '->' after parameter types in function type");
		auto ret_type = parse_type();
		return make_type<FnTypeNode>(start, ret_type, std::move(param_types));
	}

	TypeNodeId Parser::parse_named_type() {
		auto start = cursor_.offset();

		auto name = parse_name();
		return make_type<NamedTypeNode>(start, std::move(name));
	}

} // namespace compiler::frontend::parser
