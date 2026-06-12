// frontend/parser/parse_decl.cpp

#include "frontend/parser/parser.hpp"

namespace compiler::frontend::parser {

	DeclNodeId Parser::parse_decl() {
		if (cursor_.check(TokenKind::Var)) {
			return parse_var_decl();
		}
		if (cursor_.check(TokenKind::Const)) {
			return parse_var_decl(true);
		}
		if (cursor_.check(TokenKind::Fn)) {
			return parse_fn_decl();
		}
		if (cursor_.check(TokenKind::Record)) {
			return parse_record_decl();
		}
		if (cursor_.check(TokenKind::Alias)) {
			return parse_alias_decl();
		}
		if (cursor_.check(TokenKind::Namespace)) {
			return parse_namespace_decl();
		}

		throw std::runtime_error("Expected declaration");
	}

	DeclNodeId Parser::parse_var_decl(bool is_const) {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'var' or 'const'
		auto name = parse_ident();
		cursor_.expect(TokenKind::Colon, "Expected ':' after variable name");
		auto type = parse_type();

		if (cursor_.match(TokenKind::Equal)) {
			auto init = parse_expr();
			cursor_.expect(TokenKind::Semicolon, "Expected ';' after variable declaration");
			return make_decl<VarDeclNode>(start, is_const, name, type, init);
		}

		cursor_.expect(TokenKind::Semicolon, "Expected ';' after variable declaration");
		return make_decl<VarDeclNode>(start, is_const, name, type, std::nullopt);
	}

	DeclNodeId Parser::parse_fn_decl() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'fn'
		auto name = parse_ident();

		cursor_.expect(TokenKind::LeftParen, "Expected '(' after function name");
		ParamDeclList params;
		if (!cursor_.match(TokenKind::RightParen)) {
			do {
				auto is_const = cursor_.match(TokenKind::Const);
				auto param_name = parse_ident();
				cursor_.expect(TokenKind::Colon, "Expected ':' after parameter name");
				auto type = parse_type();
				params.emplace_back(is_const, param_name, type);
			} while (cursor_.match(TokenKind::Comma) && !cursor_.check(TokenKind::RightParen));
			cursor_.expect(TokenKind::RightParen, "Expected ')' after parameters");
		}

		cursor_.expect(TokenKind::Arrow, "Expected '->' after parameter list");
		auto ret_type = parse_type();
		auto body = parse_block();
		return make_decl<FnDeclNode>(start, name, std::move(params), ret_type, body);
	}

	DeclNodeId Parser::parse_record_decl() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'record'
		auto name = parse_ident();
		cursor_.expect(TokenKind::LeftBrace, "Expected '{' after record name");
		FieldDeclList fields;
		if (!cursor_.match(TokenKind::RightBrace)) {
			do {
				auto field_name = parse_ident();
				cursor_.expect(TokenKind::Colon, "Expected ':' after field name");
				auto type = parse_type();
				fields.emplace_back(field_name, type);
			} while (cursor_.match(TokenKind::Comma) && !cursor_.check(TokenKind::RightBrace));
			cursor_.expect(TokenKind::RightBrace, "Expected '}' after record fields");
		}

		return make_decl<RecordDeclNode>(start, name, fields);
	}

	DeclNodeId Parser::parse_alias_decl() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'alias'
		auto name = parse_ident();
		cursor_.expect(TokenKind::Equal, "Expected '=' after alias name");
		auto type = parse_type();
		cursor_.expect(TokenKind::Semicolon, "Expected ';' after alias declaration");
		return make_decl<AliasDeclNode>(start, name, type);
	}

	DeclNodeId Parser::parse_namespace_decl() {
		auto start = cursor_.offset();

		cursor_.advance(); // consume 'namespace'
		auto name = parse_ident();
		cursor_.expect(TokenKind::LeftBrace, "Expected '{' after namespace name");
		DeclNodeIdList decls;
		while (!cursor_.check(TokenKind::RightBrace) && !cursor_.is_eof()) {
			decls.push_back(parse_decl());
		}
		cursor_.expect(TokenKind::RightBrace, "Expected '}' after namespace body");
		return make_decl<NamespaceDeclNode>(start, name, std::move(decls));
	}

} // namespace compiler::frontend::parser
