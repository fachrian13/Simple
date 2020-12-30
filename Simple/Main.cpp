#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::Exception;
using Simple::System::Time;
using Simple::Utility::Message;
using Simple::Utility::Cipher;
using Simple::Utility::Result;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu;

int main() {
	try {
		Console::SetSize(100, 30);
		Console::SetFont(L"Consolas", 20);
		Console::SetPositionToCenter();
		
		ConsoleMenu menu{
			{
				"Menu 1",
				"Menu 2",
				"Menu 3",
				"Menu 4",
				"Menu 5",
				"Menu 6",
				"Menu 7",
				"Menu 8",
				"Menu 9",
				"Menu 0"
			},
			{Color::Green, Color::Black},
			{2, 2}
		};
		ConsoleMenu::Selection result = menu.Print(5);

		menu.Clear();
		Tools::Print(50, 2, result.First);
		Tools::Print(50, 3, result.Second);

		Console::GetKey();
		return EXIT_SUCCESS;
	}
	catch (Exception e) {
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
	catch (std::exception e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error!\n",
			"Description: ", e.what()
		);
		Console::GetKey();
		return EXIT_FAILURE;
	}
}