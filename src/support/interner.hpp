// support/interner.hpp

#ifndef COMPILER_SRC_SUPPORT_INTERNER_HPP
#define COMPILER_SRC_SUPPORT_INTERNER_HPP

#include <functional>
#include <unordered_map>

#include "support/pool.hpp"

namespace compiler {

	template <typename T, typename IdType, typename Hash = std::hash<T>> class Interner {
	public:

		const T& operator[](IdType id) const noexcept {
			return pool_[id];
		}

		IdType intern(T value) {
			if (auto it = dedup_.find(value); it != dedup_.end()) {
				return it->second;
			}

			auto id = pool_.add(value);
			dedup_.try_emplace(std::move(value), id);
			return id;
		}

		IdType add_unique(T value) {
			auto id = pool_.add(std::move(value));
			return id;
		}

		void reserve(usize capacity) {
			pool_.reserve(capacity);
		}

	private:
		Pool<T, IdType> pool_;
		std::unordered_map<T, IdType, Hash> dedup_;
	};

} // namespace compiler

#endif // COMPILER_SRC_SUPPORT_INTERNER_HPP
