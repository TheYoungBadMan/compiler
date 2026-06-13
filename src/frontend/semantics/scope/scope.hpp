// frontend/semantics/scope/scope.hpp

#ifndef COMPILER_SRC_FRONTEND_SEMANTICS_SCOPE_SCOPE_HPP
#define COMPILER_SRC_FRONTEND_SEMANTICS_SCOPE_SCOPE_HPP

#include <optional>
#include <unordered_map>

#include "core/ident.hpp"

#include "frontend/semantics/scope/scope_id.hpp"
#include "frontend/semantics/symbol/symbol.hpp"

namespace compiler::frontend {

	using SymbolTable = std::unordered_map<IdentId, Symbol>;

	class Scope {
	public:
		explicit Scope(std::optional<ScopeId> parent = std::nullopt) noexcept : parent_(parent) {}

		std::optional<ScopeId> parent() const noexcept {
			return parent_;
		}

		std::optional<Symbol> lookup(IdentId ident) const noexcept {
			if (auto it = symbols_.find(ident); it != symbols_.end()) {
				return it->second;
			}

			return std::nullopt;
		}

		bool declare(IdentId ident, Symbol symbol) noexcept {
			return symbols_.try_emplace(ident, symbol).second;
		}

	private:
		std::optional<ScopeId> parent_ = std::nullopt;
		SymbolTable symbols_;
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SEMANTICS_SCOPE_SCOPE_HPP
