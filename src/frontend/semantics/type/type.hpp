// frontend/semantics/type/type.hpp

#ifndef COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_HPP
#define COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_HPP

#include <optional>
#include <variant>
#include <vector>

#include "core/ident.hpp"

#include "support/primitives.hpp"

#include "frontend/semantics/type/type_id.hpp"

namespace compiler::frontend {

	enum class BuiltinTypeKind : u8 {
		Int,
		Uint,
		Float,
		Bool,
		Void,
	};

	struct BuiltinType {
		BuiltinTypeKind kind;

		bool operator==(const BuiltinType& other) const noexcept = default;
	};

	struct PointerType {
		TypeId pointee;

		bool operator==(const PointerType& other) const noexcept = default;
	};

	struct ArrayType {
		TypeId elem;
		usize size;

		bool operator==(const ArrayType& other) const noexcept = default;
	};

	struct RecordField {
		IdentId name;
		TypeId type;

		bool operator==(const RecordField& other) const noexcept = default;
	};

	using RecordFieldList = std::vector<RecordField>;

	struct RecordInfo {
		IdentId name;
		RecordFieldList fields;

		[[nodiscard]] std::optional<RecordField> lookup_field(IdentId ident) const noexcept {
			for (auto field : fields) {
				if (field.name == ident) {
					return field;
				}
			}

			return std::nullopt;
		}
	};

	enum class RecordId : Id {};

	struct RecordType {
		RecordId id;

		bool operator==(const RecordType& other) const noexcept = default;
	};

	struct FnType {
		TypeId return_type;
		TypeIdList param_types;
	};

	using Type = std::variant<BuiltinType, PointerType, ArrayType, RecordType, FnType>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_HPP
