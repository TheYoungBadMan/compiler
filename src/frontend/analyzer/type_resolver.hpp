// frontend/analyzer/type_resolver.hpp

#ifndef COMPILER_SRC_FRONTEND_ANALYZER_TYPE_RESOLVER_HPP
#define COMPILER_SRC_FRONTEND_ANALYZER_TYPE_RESOLVER_HPP

#include "core/ident.hpp"

#include "frontend/syntax/ast/module.hpp"
#include "frontend/syntax/ast/node_id.hpp"

#include "frontend/semantics/type/type_interner.hpp"

namespace compiler::frontend::analyzer {

    class TypeResolver {
    public:
        std::optional<TypeId> resolve(TypeNodeId id) const noexcept;
    private:
        const Module& module_;
        const IdentInterner& idents_;
        TypeInterner& types_;
    };

} // namespace compiler::frontend::analyzer

#endif // COMPILER_SRC_FRONTEND_ANALYZER_TYPE_RESOLVER_HPP