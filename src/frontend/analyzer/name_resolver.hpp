// frontend/analyzer/name_resolver.hpp

#ifndef COMPILER_SRC_FRONTEND_ANALYZER_NAME_RESOLVER_HPP
#define COMPILER_SRC_FRONTEND_ANALYZER_NAME_RESOLVER_HPP

#include "frontend/syntax/ast/module.hpp"

#include "frontend/semantics/scope/scope_pool.hpp"

namespace compiler::frontend::analyzer {

    class NameResolver {
    public:
        std::optional<Symbol> resolve(IdentId ident) const noexcept {
            return scopes_.lookup(ident, current_scope_);
        }
        std::optional<Symbol> resolve(Name name) const noexcept;

    private:
        const Module& module_;
        const ScopePool& scopes_;
        ScopeId& current_scope_;
    };

} // namespace compiler::frontend::analyzer

#endif // COMPILER_SRC_FRONTEND_ANALYZER_NAME_RESOLVER_HPP