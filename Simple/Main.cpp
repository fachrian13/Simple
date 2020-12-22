#include "Simple.h"
#include <filesystem>

using Simple::System::Console;

int main() {
	std::cout << Console::GetBufferSize().Y;
	std::cin.get();
	return EXIT_SUCCESS;
}