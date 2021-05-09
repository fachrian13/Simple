#include "Simple.h"

using namespace Simple;
using System::Color;
using System::Console;
using System::Exception;
using System::Time;
using Utility::Cipher;
using Utility::Convert;
using Utility::Result;
using Utility::Tools;
using Utility::ConsoleMenu;
using Utility::ConsoleTable;

int main() {
	try {
		Console::Size = { 120, 30 };
		Console::SetFont(L"Menlo", 18);
		Console::DisableMaximizeButton();
		Console::DisableResize();
		Console::SetPositionToCenter();

		Tools::WriteLine(Color::Red, Color::Blue, "Hello World", "Hello Lur");

		Console::ReadKey();
		return EXIT_SUCCESS;
	}
	catch (Exception e) {
		Console::Clear();
		Console::Write(
			"Program mengalami error:(\n\n",
			"File: ",			e.File(),		"\n",
			"Line: ",			e.Line(),		"\n",
			"Function: ",		e.Function(),	"\n",
			"Description: ",	e.Description()
		);
		Console::ReadKey();
		return EXIT_FAILURE;
	}
	catch (std::exception e) {
		Console::Clear();
		Console::Write(
			"Program mengalami error:(\n\n",
			"Description: ", e.what()
		);
		Console::ReadKey();
		return EXIT_FAILURE;
	}
}