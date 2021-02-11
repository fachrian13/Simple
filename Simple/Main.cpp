#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::Exception;
using Simple::System::Time;
using Simple::Utility::Message;
using Simple::Utility::Cipher;
using Simple::Utility::Convert;
using Simple::Utility::Result;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu;
using Simple::Utility::ConsoleTable;

int main() {
	try {
		Console::SetSize(120, 30);
		Console::SetFont(L"Consolas", 18);
		Console::DisableMaximizeButton();
		Console::DisableResize();
		Console::SetPositionToCenter();
		
		Console::Print(Time::Now().StandardFormat(), " ", Convert::ToString(4.12f));

		Console::GetKey();
		return EXIT_SUCCESS;
	}
	catch (Exception e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error:(\n\n",
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
			"Program mengalami error:(\n\n",
			"Description: ", e.what()
		);
		Console::GetKey();
		return EXIT_FAILURE;
	}
}