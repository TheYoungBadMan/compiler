// frontend/syntax/token/token_dump.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_DUMP_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_DUMP_HPP

#include <algorithm>
#include <format>
#include <string>
#include <string_view>

#include "core/source.hpp"

#include "frontend/syntax/token/token.hpp"
#include "frontend/syntax/token/token_kind.hpp"

#include "support/debug.hpp"

#define TOKEN_KIND(name) case TokenKind::name: return #name;

namespace compiler::frontend {

	[[nodiscard]] inline std::string_view dump(TokenKind kind) {
		switch (kind) {
			TOKEN_KIND(Ident)
			TOKEN_KIND(Var)
			TOKEN_KIND(Const)
			TOKEN_KIND(Fn)
			TOKEN_KIND(Record)
			TOKEN_KIND(Alias)
			TOKEN_KIND(Namespace)
			TOKEN_KIND(If)
			TOKEN_KIND(Else)
			TOKEN_KIND(While)
			TOKEN_KIND(Return)
			TOKEN_KIND(Break)
			TOKEN_KIND(Continue)
			TOKEN_KIND(Pass)
			TOKEN_KIND(Inf)
			TOKEN_KIND(Nan)
			TOKEN_KIND(True)
			TOKEN_KIND(False)
			TOKEN_KIND(Null)
			TOKEN_KIND(As)
			TOKEN_KIND(Int)
			TOKEN_KIND(Float)
			TOKEN_KIND(LessLessEqual)
			TOKEN_KIND(GreaterGreaterEqual)
			TOKEN_KIND(EqualEqual)
			TOKEN_KIND(BangEqual)
			TOKEN_KIND(LessEqual)
			TOKEN_KIND(GreaterEqual)
			TOKEN_KIND(AmpAmp)
			TOKEN_KIND(BarBar)
			TOKEN_KIND(LessLess)
			TOKEN_KIND(GreaterGreater)
			TOKEN_KIND(PlusEqual)
			TOKEN_KIND(MinusEqual)
			TOKEN_KIND(StarEqual)
			TOKEN_KIND(SlashEqual)
			TOKEN_KIND(PercentEqual)
			TOKEN_KIND(AmpEqual)
			TOKEN_KIND(BarEqual)
			TOKEN_KIND(CaretEqual)
			TOKEN_KIND(Arrow)
			TOKEN_KIND(ColonColon)
			TOKEN_KIND(Plus)
			TOKEN_KIND(Minus)
			TOKEN_KIND(Star)
			TOKEN_KIND(Slash)
			TOKEN_KIND(Percent)
			TOKEN_KIND(Equal)
			TOKEN_KIND(Bang)
			TOKEN_KIND(Less)
			TOKEN_KIND(Greater)
			TOKEN_KIND(Amp)
			TOKEN_KIND(Bar)
			TOKEN_KIND(Caret)
			TOKEN_KIND(Tilde)
			TOKEN_KIND(LeftParen)
			TOKEN_KIND(RightParen)
			TOKEN_KIND(LeftBracket)
			TOKEN_KIND(RightBracket)
			TOKEN_KIND(LeftBrace)
			TOKEN_KIND(RightBrace)
			TOKEN_KIND(Semicolon)
			TOKEN_KIND(Colon)
			TOKEN_KIND(Comma)
			TOKEN_KIND(Dot)
			TOKEN_KIND(Eof)
		}

		debug_unreachable("invalid token kind");
	}

	[[nodiscard]] inline std::string dump(const Token& token, const Source& source) {
		return std::format(
			"{} `{}` at {} [{}]",
			dump(token.kind),
			source.slice(token.span),
			source.loc(token.span.start),
			token.span
		);
	}

	[[nodiscard]] inline std::string dump(const TokenList& tokens, const Source& source) {
		std::string result = "Tokens\n";
		if (tokens.empty()) {
			return result;
		}

		usize kind_w = 0;
		usize lexeme_w = 0;
		usize loc_w = 0;

		for (const auto& token : tokens) {
			kind_w = std::max(kind_w, dump(token.kind).size());
			lexeme_w = std::max(lexeme_w, source.slice(token.span).size() + 2);
			loc_w = std::max(loc_w, std::format("{}", source.loc(token.span.start)).size());
		}

		for (const auto& token : tokens) {
			auto kind = std::string(dump(token.kind));
			auto lexeme = std::format("`{}`", source.slice(token.span));
			auto loc = std::format("{}", source.loc(token.span.start));

			kind.resize(kind_w, ' ');
			lexeme.resize(lexeme_w, ' ');
			loc.resize(loc_w, ' ');

			result += std::format("  {}  {}  at {}  [{}]\n",
				kind, lexeme, loc, token.span);
		}
		return result;
	}

} // namespace compiler::frontend

#undef TOKEN_KIND

#endif // COMPILER_SRC_FRONTEND_SYNTAX_TOKEN_TOKEN_DUMP_HPP
