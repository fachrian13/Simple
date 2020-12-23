#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;

int main() {
	Console::SetTextColor(Color::Red, Color::Yellow);
	Console::Print("Hello World");
	Console::GetKey();
	return EXIT_SUCCESS;
}