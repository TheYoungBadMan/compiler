// support/pool.hpp

#ifndef COMPILER_SRC_SUPPORT_POOL_HPP
#define COMPILER_SRC_SUPPORT_POOL_HPP

#include <utility>
#include <vector>

#include "support/id.hpp"
#include "support/primitives.hpp"

namespace compiler {

	template <typename T, typename IdType> class Pool {
	public:

		[[nodiscard]] usize size() const noexcept {
			return data_.size();
		}

		T& operator[](IdType id) noexcept {
			debug_assert(to_index(id) < data_.size(), "invalid id");
			return data_[to_index(id)];
		}

		[[nodiscard]] const T& operator[](IdType id) const noexcept {
			debug_assert(to_index(id) < data_.size(), "invalid id");
			return data_[to_index(id)];
		}

		IdType add(T value) {
			IdType id = next_id();
			data_.push_back(std::move(value));
			return id;
		}

		void reserve(usize capacity) {
			data_.reserve(capacity);
		}

	private:
		std::vector<T> data_;

		IdType next_id() noexcept {
			return to_id<IdType>(data_.size());
		}
	};

} // namespace compiler

#endif // COMPILER_SRC_SUPPORT_POOL_HPP
