#include "Simple.h"

using Simple::System::Exception;
using Simple::System::Console;
using Simple::System::BinaryFile;
using Simple::System::Coordinate;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu::Input;

struct DataUser {
	unsigned long long NIK;
	char Password[16];
	bool Suspend;

	DataUser() : NIK(0), Password(""), Suspend(false) {}
	DataUser(std::string nik, std::string password, bool suspend = false) {
		this->NIK = std::stoull(nik);
		strcpy_s(this->Password, password.c_str());
		this->Suspend = suspend;
	}
};

BinaryFile<DataUser> User("user.bin");

void SignUp();

int main() {
	try {
		Console::CursorBlinking = false;
		Console::CursorVisible = false;
		Console::DisableMaximizeButton();
		Console::DisableResizeWindow();
		Console::SetWindowSize(170, 40);
		Console::EnableVirtual();

		if (User.IsEmpty())
			SignUp();
	}
	catch (Exception& e) {
		Console::Clear();
		Console::Write(e);
		Console::ReadKey();

		return 1;
	}
	catch (std::exception& e) {
		Console::Clear();
		Console::Write(e.what());
		Console::ReadKey();

		return 1;
	}

	return 0;
}

void SignUp() {
	std::string nik;
	std::string password;
	std::string password1;
	Input iSignUp(
		{
			Input::Text("NIK                 :", &nik),
			Input::Text("Password            :", &password, "", true),
			Input::Text("Konfirmasi password :", &password1, "", true),
			Input::Text("[Register]"),
			Input::Text("[Kembali]")
		},
		{ 3, 7 }
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "========================",
			Coordinate{ 3, 4 }, "   REGISTER USER BARU",
			Coordinate{ 3, 5 }, "========================"
		);
		iSignUp.Run();

		switch (iSignUp.Selected.Index) {
		case 0:
			Console::CursorVisible = true;
			nik = Tools::ReadNumber(15);
			Console::CursorVisible = false;
			break;
		case 1:
			Console::CursorVisible = true;
			password = Tools::ReadPassword(15);
			Console::CursorVisible = false;
			break;
		case 2:
			Console::CursorVisible = true;
			password1 = Tools::ReadPassword(15);
			Console::CursorVisible = false;
			break;
		}

		if (Tools::Compare(iSignUp.Selected.Value, "[Register]")) {
			if (nik.empty() || password.empty() || password1.empty())
				Tools::WriteMessage({ 3, 13 }, "WARNING", "Silakan lengkapi seluruh data.");
			else {

			}
		}
		else if (Tools::Compare(iSignUp.Selected.Value, "[Kembali]"))
			iSignUp.Stop();
	} while (iSignUp.Running);
}