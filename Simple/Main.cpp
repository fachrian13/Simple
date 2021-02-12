#include "Simple.h"
#include <io.h>
#include <fcntl.h>

using namespace Simple;
using System::Color;
using System::Console;
using System::Exception;
using System::Time;
using Utility::Message;
using Utility::Cipher;
using Utility::Convert;
using Utility::Result;
using Utility::Tools;
using Utility::ConsoleMenu;
using Utility::ConsoleTable;

int main() {
	try {
		Console::SetSize(120, 30);
		Console::SetFont(L"Consolas", 18);
		Console::DisableMaximizeButton();
		Console::DisableResize();
		Console::SetPositionToCenter();

		ConsoleMenu menu{
			{
				"1 asjkdhfgkjdhsfg",
				"2 skdhfbgseryh",
				"3 isudbhfguhd",
				"4 sidufbgyhrt",
				"5 sidujfbgishtbrg",
				"6 sidujrbfgsdhjkrtbg",
				"7 sihuebghdtbtg",
				"8 syergsdhrg",
				"9 skerbngsdofgjsdf",
				"10 souyhdbfgtusdebrgtjs"
			},
			{Color::Green, Color::Black},
			{2, 2},
			true
		};
		menu.Print(5);

		//Console::GetKey();
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