// frontend/syntax/ast/block.hpp

#ifndef COMPILER_SRC_FRONTEND_SYNTAX_AST_BLOCK_HPP
#define COMPILER_SRC_FRONTEND_SYNTAX_AST_BLOCK_HPP

#include <vector>

#include "core/span.hpp"

#include "frontend/syntax/ast/node_id.hpp"

#include "support/pool.hpp"

namespace compiler::frontend {

	struct BlockNode {
		StmtNodeIdList stmts;
	};

	using BlockNodeList = std::vector<BlockNode>;

	using BlockNodePool = Pool<BlockNodeId, BlockNode>;
	using BlockNodeSpanPool = Pool<BlockNodeId, SourceSpan>;

} // namespace compiler::frontend

#endif // COMPILER_SRC_FRONTEND_SYNTAX_AST_BLOCK_HPP
