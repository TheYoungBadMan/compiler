// support/pool.hpp

#ifndef COMPILER_SRC_SUPPORT_POOL_HPP
#define COMPILER_SRC_SUPPORT_POOL_HPP

#include <utility>
#include <vector>

#include "support/id.hpp"
#include "support/primitives.hpp"

namespace compiler {

	template <typename IdType, typename T>
	class Pool {
	public:

		[[nodiscard]] usize size() const noexcept {
			return data_.size();
		}

		T& get(IdType id) noexcept {
			return data_[to_index(id)];
		}

		[[nodiscard]] const T& get(IdType id) const noexcept {
			return data_[to_index(id)];
		}

		IdType add(T value) {
			IdType id = next_id();
			data_.push_back(std::move(value));
			return id;
		}

	private:
		std::vector<T> data_;

		constexpr IdType next_id() noexcept {
			return to_id<IdType>(data_.size());
		}
	};

} // namespace compiler

#endif // COMPILER_SRC_SUPPORT_POOL_HPP
