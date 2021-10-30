#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::ConsoleColor;
using Simple::System::Exception;
using Simple::System::Application;
using Simple::Utility::Convert;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu;

void Login()
{
	ConsoleMenu mLogin
	(
		{
			"Username :",
			"Password :",
			"[Login]",
			"[Exit]"
		},
		{ 2, 6 }
	);
	std::string username;
	std::string password;

	Tools::Write({ 2, 2 }, "================");
	Tools::Write({ 2, 3 }, "   LOGIN AKUN");
	Tools::Write({ 2, 4 }, "================");

	do
	{
		mLogin.Run();

		switch (mLogin.Selected.Index)
		{
		case 0:
			Tools::Clear({ 16, 2 }, username);
			username = Console::ReadLine();
			break;
		case 1:
			Tools::Clear({ 16, 3 }, password);
			password = Tools::ReadPassword();
			break;
		}

	} while (strcmp(mLogin.Selected.Value, "[Exit]") != 0);
}

class Dashboard final : public Application
{
private:
	void Main()
	{
		Login();
	}
} dashboard;

int main()
{
	dashboard.Run();

	return 0;
}