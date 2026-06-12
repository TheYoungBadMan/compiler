// core/ident.hpp

#ifndef COMPILER_SRC_CORE_IDENT_HPP
#define COMPILER_SRC_CORE_IDENT_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "support/id.hpp"
#include "support/pool.hpp"
#include "support/primitives.hpp"

namespace compiler {

	enum class IdentId : Id {};
	using IdentIdList = std::vector<IdentId>;

	using Ident = std::string;
	using IdentList = std::vector<Ident>;

	using IdentMap = std::unordered_map<Ident, IdentId>;

	class IdentInterner {
	public:

		[[nodiscard]] usize size() const noexcept {
			return pool_.size();
		}

		[[nodiscard]] const Ident& get(IdentId id) const noexcept {
			return pool_.get(id);
		}

		IdentId intern(const Ident& ident) {
			if (auto it = map_.find(ident); it != map_.end()) {
				return it->second;
			}

			IdentId id = pool_.add(ident);
			map_.emplace(ident, id);
			return id;
		}

	private:
		Pool<IdentId, Ident> pool_;
		IdentMap map_;
	};

} // namespace compiler

#endif // COMPILER_SRC_CORE_IDENT_HPP
