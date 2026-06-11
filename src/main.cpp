#include <cstddef>

#include "driver/driver.hpp"

int main(int argc, char* argv[]) {
	return compiler::driver::run({argv, static_cast<std::size_t>(argc)});
}
