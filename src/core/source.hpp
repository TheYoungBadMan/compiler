// core/source.hpp

#ifndef COMPILER_SRC_CORE_SOURCE_HPP
#define COMPILER_SRC_CORE_SOURCE_HPP

#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/loc.hpp"
#include "core/offset.hpp"
#include "core/span.hpp"

#include "support/debug.hpp"
#include "support/primitives.hpp"

namespace compiler {

	class Source {
	public:

		explicit Source(std::string content)
			: content_(std::move(content)), line_starts_(make_line_starts(this->content_)) {
			debug_assert(
				content_.size() <= std::numeric_limits<Offset>::max(),
				"source exceeds the 4 GiB offset limit"
			);
		}

		[[nodiscard]] Offset size() const noexcept {
			return static_cast<Offset>(content_.size());
		}

		unsigned char operator[](Offset index) const noexcept {
			debug_assert(index < content_.size(), "index out of bounds");
			return static_cast<unsigned char>(content_[index]);
		}

		[[nodiscard]] std::string_view slice(Offset start, Offset end) const noexcept {
			debug_assert(start <= end, "invalid slice bounds");
			debug_assert(end <= content_.size(), "slice end out of bounds");
			return {content_.data() + start, end - start};
		}

		[[nodiscard]] std::string_view slice(SourceSpan span) const noexcept {
			return slice(span.start, span.end);
		}

		[[nodiscard]] SourceLoc loc(Offset offset) const noexcept {
			debug_assert(offset <= content_.size(), "offset out of bounds");

			auto it = std::ranges::upper_bound(line_starts_, offset);
			debug_assert(it != line_starts_.begin(), "invalid line starts");
			--it;

			auto line = static_cast<u32>(std::distance(line_starts_.begin(), it));
			auto column = static_cast<u32>(offset - *it);

			return SourceLoc{
				.line = line,
				.column = column
			};
		}

	private:
		std::string content_;

		std::vector<u32> line_starts_;

		static std::vector<u32> make_line_starts(std::string_view content) {
			std::vector<u32> line_starts;
			line_starts.push_back(0);

			for (usize i = 0; i < content.size(); ++i) {
				if (content[i] == '\n') {
					line_starts.push_back(static_cast<u32>(i + 1));
				}
			}

			return line_starts;
		}
	};

} // namespace compiler

#endif // COMPILER_SRC_CORE_SOURCE_HPP
