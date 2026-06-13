// frontend/semantics/scope/scope_pool.hpp

#ifndef COMPILER_SRC_FRONTEND_SEMANTICS_SCOPE_SCOPE_POOL_HPP
#define COMPILER_SRC_FRONTEND_SEMANTICS_SCOPE_SCOPE_POOL_HPP

#include "support/pool.hpp"
#include "frontend/semantics/scope/scope_id.hpp"
#include "frontend/semantics/scope/scope.hpp"

namespace compiler::frontend {

	struct ScopePool {
	public:
		ScopePool() {
			global_ = add(Scope(std::nullopt));
		}

		const Scope& operator[](ScopeId id) const noexcept {
			return scopes_[id];
		}

		[[nodiscard]] ScopeId global() const noexcept {
			return global_;
		}

		ScopeId add(Scope scope) {
			return scopes_.add(std::move(scope));
		}

		[[nodiscard]] std::optional<Symbol> lookup(IdentId ident, ScopeId from) const noexcept {
			for (std::optional<ScopeId> current = from; current; current = parent(*current)) {
				if (auto symbol = lookup_local(ident, *current)) {
					return symbol;
				}
			}

			return std::nullopt;
		}

		bool declare(IdentId ident, Symbol symbol, ScopeId scope) {
			return scopes_[scope].declare(ident, symbol);
		}

	private:
		Pool<Scope, ScopeId> scopes_;
		ScopeId global_;

		[[nodiscard]] std::optional<ScopeId> parent(ScopeId id) const noexcept {
			return scopes_[id].parent();
		}

		[[nodiscard]] std::optional<Symbol> lookup_local(IdentId ident, ScopeId id) const noexcept {
			return scopes_[id].lookup(ident);
		}
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SEMANTICS_SCOPE_SCOPE_POOL_HPP
