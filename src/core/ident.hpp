// core/ident.hpp

#ifndef COMPILER_SRC_CORE_IDENT_HPP
#define COMPILER_SRC_CORE_IDENT_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "support/id.hpp"
#include "support/interner.hpp"

namespace compiler {

	enum class IdentId : Id {};
	using IdentIdList = std::vector<IdentId>;

	using Ident = std::string;
	using IdentView = std::string_view;

	class IdentInterner {
	public:

		[[nodiscard]] const Ident& get(IdentId id) const noexcept {
			return pool_[id];
		}

		IdentId intern(IdentView ident) {
			return pool_.intern(Ident(ident));
		}

	private:
		Interner<Ident, IdentId> pool_;
	};

} // namespace compiler

#endif // COMPILER_SRC_CORE_IDENT_HPP
