// frontend/semantics/type/type_id.hpp

#ifndef COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_ID_HPP
#define COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_ID_HPP

#include <vector>

#include "support/id.hpp"

namespace compiler::frontend {

	enum class TypeId : Id {};

	using TypeIdList = std::vector<TypeId>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_ID_HPP
