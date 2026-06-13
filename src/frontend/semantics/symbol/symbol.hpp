// frontend/semantics/symbol/symbol.hpp

#ifndef COMPILER_SRC_FRONTEND_SEMANTICS_SYMBOL_SYMBOL_HPP
#define COMPILER_SRC_FRONTEND_SEMANTICS_SYMBOL_SYMBOL_HPP

#include <variant>

#include "frontend/semantics/type/type_id.hpp"
#include "frontend/semantics/scope/scope_id.hpp"

namespace compiler::frontend {

	struct VarSymbol {
		TypeId type;
		bool is_const;
	};

	struct FnSymbol {
		TypeId type;
	};

	struct TypeSymbol {
		TypeId type;
	};

	struct NamespaceSymbol {
		ScopeId scope;
	};

	using ValueSymbol = std::variant<VarSymbol, FnSymbol>;

	using Symbol = std::variant<VarSymbol, FnSymbol, TypeSymbol, NamespaceSymbol>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SEMANTICS_SYMBOL_SYMBOL_HPP
