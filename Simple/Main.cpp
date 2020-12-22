#include "Simple.h"
#include <filesystem>

using Simple::System::BinaryFile;

int main() {
	BinaryFile<char[10]> bin{ "Hello" };
	std::cin.get();
	return EXIT_SUCCESS;
}