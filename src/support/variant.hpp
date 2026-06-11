// support/variant.hpp

#ifndef COMPILER_SRC_SUPPORT_VARIANT_HPP
#define COMPILER_SRC_SUPPORT_VARIANT_HPP

#include <utility>
#include <variant>

#include "support/debug.hpp"

namespace compiler {

	template <typename... Fs>
	struct Overloaded : Fs... {
		using Fs::operator()...;
	};

	template <typename Variant, typename... Fs>
	decltype(auto) match(Variant&& variant, Fs&&... fs) {
		return std::visit(Overloaded{std::forward<Fs>(fs)...}, std::forward<Variant>(variant));
	}

	template <typename T, typename... Ts>
	const T& expect_alt(const std::variant<Ts...>& variant, [[maybe_unused]] const char* msg) noexcept {
		debug_assert(std::holds_alternative<T>(variant), msg);
		return std::get<T>(variant);
	}

} // namespace compiler

#endif // COMPILER_SRC_SUPPORT_VARIANT_HPP
