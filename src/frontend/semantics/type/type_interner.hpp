// frontend/semantics/type/type_interner.hpp

#ifndef COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_INTERNER_HPP
#define COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_INTERNER_HPP

#include "support/interner.hpp"
#include "support/variant.hpp"

#include "frontend/semantics/type/type.hpp"
#include "frontend/semantics/type/type_id.hpp"

namespace compiler::frontend {

	struct TypeHash {}; // TODO: implement

	class TypeInterner {
	public:

		const Type& operator[](TypeId id) const noexcept {
			return interner_[id];
		}

		TypeId intern(const Type& type) {
			debug_assert(!std::holds_alternative<RecordType>(type), "cannot intern record type");
			return interner_.intern(type);
		}

		TypeId declare_record() {
			auto record_id = record_infos_.add({});
			return interner_.add_unique(RecordType{record_id});
		}

		void define_record(TypeId id, RecordInfo info) {
			auto record = expect_alt<RecordType>(interner_[id], "expected record type");
			record_infos_[record.id] = std::move(info);
		}

	private:
		Interner<Type, TypeId, TypeHash> interner_;
		Pool<RecordInfo, RecordId> record_infos_;
	};

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SEMANTICS_TYPE_TYPE_INTERNER_HPP
