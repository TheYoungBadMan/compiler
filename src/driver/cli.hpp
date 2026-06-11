// driver/cli.hpp

#ifndef COMPILER_SRC_DRIVER_CLI_HPP
#define COMPILER_SRC_DRIVER_CLI_HPP

#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace compiler::driver::cli {

	struct CliOptions {
		bool show_help = false;
		bool show_version = false;
		bool show_tokens = false;
		bool show_ast = false;
		bool show_ir = false;
		bool show_asm = false;

		std::string input_file;
	};

	inline std::optional<CliOptions> parse_options(std::span<char* const> args) {
		CliOptions opts;
		bool positional_only = false;

		for (const std::string_view arg : args) {
			if (!positional_only && arg == "--") {
				positional_only = true;
			} else if (!positional_only && (arg == "-h" || arg == "--help")) {
				opts.show_help = true;
			} else if (!positional_only && (arg == "-v" || arg == "--version")) {
				opts.show_version = true;
			} else if (!positional_only && arg == "--show-tokens") {
				opts.show_tokens = true;
			} else if (!positional_only && arg == "--show-ast") {
				opts.show_ast = true;
			} else if (!positional_only && arg == "--show-ir") {
				opts.show_ir = true;
			} else if (!positional_only && arg == "--show-asm") {
				opts.show_asm = true;
			} else if (positional_only || !arg.starts_with('-')) {
				if (!opts.input_file.empty()) {
					return std::nullopt; // multiple input files unsupported
				}

				opts.input_file = arg;
			} else {
				return std::nullopt; // unrecognized option
			}
		}

		return opts;
	}

	constexpr std::string_view help_message() noexcept {
		return "Usage: compiler [options] <input_file>\n"
			   "Options:\n"
			   "  -h, --help         Show this help message\n"
			   "  -v, --version      Show version information\n"
			   "      --             Treat remaining arguments as positional\n"
			   "      --show-tokens  Show tokens\n"
			   "      --show-ast     Show AST\n"
			   "      --show-ir      Show IR\n"
			   "      --show-asm     Show assembly\n";
	}

	constexpr std::string_view version_message() noexcept {
		return "Compiler version 1.0";
	}

} // namespace compiler::driver::cli

#endif // COMPILER_SRC_DRIVER_CLI_HPP
