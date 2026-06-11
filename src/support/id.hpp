// support/id.hpp

#ifndef COMPILER_SRC_SUPPORT_ID_HPP
#define COMPILER_SRC_SUPPORT_ID_HPP

#include <type_traits>

#include "support/primitives.hpp"

namespace compiler {

	using Id = u32;

	template <typename IdType>
	constexpr usize to_index(IdType id) noexcept {
		static_assert(std::is_enum_v<IdType>, "to_index requires an enum type");
		return static_cast<usize>(id);
	}

	template <typename IdType>
	constexpr IdType to_id(usize index) noexcept {
		static_assert(std::is_enum_v<IdType>, "to_id requires an enum type");
		return IdType{static_cast<std::underlying_type_t<IdType>>(index)};
	}

} // namespace compiler

#endif // COMPILER_SRC_SUPPORT_ID_HPP
