// driver/driver.hpp

#ifndef COMPILER_SRC_DRIVER_DRIVER_HPP
#define COMPILER_SRC_DRIVER_DRIVER_HPP

#include <print>
#include <span>
#include <utility>

#include "core/source.hpp"

#include "frontend/frontend.hpp"
#include "frontend/lexer/error_dump.hpp"
#include "frontend/syntax/token/token_dump.hpp"

#include "driver/cli.hpp"
#include "driver/reader.hpp"

#include "frontend/syntax/token/token_dump.hpp"
#include "frontend/frontend.hpp"

namespace compiler::driver {

	inline int run(std::span<char* const> args) {
		auto opts = cli::parse_options(args.subspan(1));
		if (!opts) {
			std::println(stderr, "error: invalid command line arguments");
			std::println(stderr, "{}", cli::help_message());
			return 1;
		}

		if (opts->show_help) {
			std::println(stdout, "{}", cli::help_message());
			return 0;
		}

		if (opts->show_version) {
			std::println(stdout, "{}", cli::version_message());
			return 0;
		}

		if (opts->input_file.empty()) {
			std::println(stderr, "Error: No input file specified");
			return 1;
		}

		auto reader_result = read_file(opts->input_file);
		if (!reader_result) {
			// error handling based on reader_result.error()
			return 1;
		}

		auto source = Source(std::move(reader_result.value()));

		auto result = frontend::run(source);

		if (result.has_errors()) {
			std::print(stderr, "{}", frontend::lexer::dump(result.lexer_errors, source));
			return 1;
		}

		if (opts->show_tokens) {
			std::print("{}", frontend::dump(result.tokens, source));
		}

		// if (opts->show_ast) {
		// 	// print AST
		// }

		// if (opts->show_ir) {
		// 	// print IR
		// }

		return 0;
	}

} // namespace compiler::driver

#endif // COMPILER_SRC_DRIVER_DRIVER_HPP
