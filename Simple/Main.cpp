#include "Simple.h"

using Simple::System::BinaryFile;
using Simple::System::Console;
using Simple::Utility::ConsoleMenu;
using Simple::Utility::Tools;

struct DataUser
{
	bool Disabled;
	char Username[16];
	char Password[16];
};
BinaryFile<DataUser> UserFile{ "User.bin" };

void Register();

int main()
{
	if (UserFile.Empty())
	{
		Register();
	}
}

void Register()
{
	ConsoleMenu mRegister
	{
		{
			"Username         :",
			"Password         :",
			"Confirm password :",
			"[Register]",
			"[Back]"
		},
		{2, 6}
	};
	mRegister.ShiftDown = 'j';
	mRegister.ShiftUp = 'k';
	
	std::string username;
	std::string password;
	std::string password1;

	Tools::Write({ 2, 2 }, "=====================");
	Tools::Write({ 2, 3 }, "   REGISTRASI USER");
	Tools::Write({ 2, 4 }, "=====================");
	do
	{
		mRegister.Run();

		switch (mRegister.Selected.Index)
		{
		case 0: // Username
			Console::Write(" ");
			Tools::Clear({ 21, 6 }, username);
			username = Tools::ReadLine(15);
			break;
		case 1: // Password
			Console::Write(" ");
			Tools::Clear({ 21, 7 }, password);
			password = Tools::ReadPassword(15);
			break;
		case 2: // Confirm password
			Console::Write(" ");
			Tools::Clear({ 21, 8 }, password1);
			password1 = Tools::ReadPassword(15);
			break;
		case 3: // [Register]
			if (username.empty() || password.empty() || password1.empty())
			{
				Tools::Write({ 2, 15 }, "Silakan lengkapi semua data.");
			}
			break;
		}
	} while (mRegister.Selected.Index != 4); // [Back]
}