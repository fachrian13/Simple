#include "Simple.h"

#include <tuple>

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::Exception;
using Simple::System::Time;
using Simple::Utility::Cipher;
using Simple::Utility::Result;

int main() {
	try {
		Console::SetSize(100, 30);
		Console::SetFont(L"Consolas", 20);
		Console::SetPositionToCenter();
		Console::SetTextColor(Color::Blue, Color::Black);
		Console::Print(Cipher::Vigenere("localhost", "util"));
		Console::GetKey();
		return EXIT_SUCCESS;
	}
	catch (Exception& e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error!\n",
			"File: ",			e.File(),		"\n",
			"Line: ",			e.Line(),		"\n",
			"Function: ",		e.Function(),	"\n",
			"Description: ",	e.Description()
		);
		Console::GetKey();
		return EXIT_FAILURE;
	}
	catch (std::exception& e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error!\n",
			"Description: ", e.what()
		);
		Console::GetKey();
		return EXIT_FAILURE;
	}
}