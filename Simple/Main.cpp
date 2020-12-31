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
using Simple::Utility::ConsoleTable;

int main() {
	try {
		Console::SetSize(100, 30);
		Console::SetFont(L"Consolas", 20);
		Console::SetPositionToCenter();
		
		ConsoleTable table{ "Header 1",	"Header 2",	"Header 3",	"Header 4",	"Header 5",	"Header 1",	"Header 2",	"Header 3",	"Header 4",	"Header 5",	"Header 1",	"Header 2",	"Header 3",	"Header 4",	"Header 5",	"Header 1",	"Header 2",	"Header 3",	"Header 4",	"Header 5" };
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table += {"Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5", "Row 1", "Row 2", "Row 3", "Row 4", "Row 5"};
		table.Print();

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