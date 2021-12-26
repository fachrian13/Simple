#include "Simple.h"

using Simple::System::Coordinate;
using Simple::System::Color;
using Simple::System::Console;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu;

int main()
{
	ConsoleMenu mDashboard
	{
		{
			"Username :",
			"Password :",
			"[Sign In]",
			"[Exit]"
		},
		{ 3, 6 },
		{ Color::BrightGreen, Color::Black }
	};
	std::string username;
	std::string password;

	Console::CursorBlinking = false;
	do
	{
		Console::Write(Coordinate{ 3, 2 }, "===============");
		Console::Write(Coordinate{ 3, 3 }, "   LOCALHOST");
		Console::Write(Coordinate{ 3, 4 }, "===============");

		mDashboard.Run();

		switch (mDashboard.Selected.Index)
		{
		case 0:
			Console::Write(" ");
			Tools::Clear(username);
			username = Console::ReadLine();
			break;
		}
	} while (strcmp(mDashboard.Selected.Value, "[Exit]") != 0);

	return 0;
}