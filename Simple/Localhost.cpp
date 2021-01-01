#include "Simple.h"
#include "Siswa.h"

using namespace Simple;
using System::BinaryFile;
using System::Color;
using System::Console;
using System::ConsoleColor;
using System::Exception;
using System::Time;
using Utility::Cipher;
using Utility::ConsoleMenu;
using Utility::Message;
using Utility::Result;
using Utility::Tools;

const ConsoleColor CursorColor{ Color::Green, Color::Black };
const Regex StandardEx{ "[a-zA-Z0-9]+" };
const String Key = "qwertyuiop";

User $User;

void SignUp() {
	ConsoleMenu mSignUp{
		{
			"Username         :",
			"Password         :",
			"Confirm password :",
			"[Sign Up]",
			"[Back]"
		},
		CursorColor,
		{2, 6}
	};
	ConsoleMenu::Selection sSignUp;
	String username;
	String password;
	String password2;

	do { // Main loop
		Tools::Print(2, 2, "=============");
		Tools::Print(2, 3, "   SIGN UP");
		Tools::Print(2, 4, "=============");

		sSignUp = mSignUp.Print();

		switch (sSignUp.First) {
		case 0: // Username
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), username);
			username = Console::GetLine();
			break;
		case 1: // Password
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), password);
			password = Tools::GetPassword();
			break;
		case 2: // Confirm password
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), password2);
			password2 = Tools::GetPassword();
			break;
		}

		if (sSignUp.Second == "[Sign Up]") { // SignUp
			// Checking user input
			if (username.empty() || password.empty() || password2.empty())
				Tools::PrintMessage(2, 12, Message::Warning, "Silakan lengkapi semua data.");
			else if (!regex_match(username, StandardEx) || !regex_match(password, StandardEx))
				Tools::PrintMessage(2, 12, Message::Warning, "Username dan password hanya menggunakan huruf dan angka.");
			else if (username.length() < 7 || username.length() > 15 || password.length() < 7 || password.length() > 15)
				Tools::PrintMessage(2, 12, Message::Information, "Panjang username dan password 7-15 karakter.");
			else if ($User.CheckUsername(username.c_str()))
				Tools::PrintMessage(2, 12, Message::Information, "Username sudah digunakan.");
			else if (password != password2)
				Tools::PrintMessage(2, 12, Message::Information, "Password tidak sama.");

			// Final stage
			else {
				DataUser database;

				// Assigning data into the database
				database.Blocked = false;
				strcpy_s(database.DateTime, Time::Now().StandardFormat().c_str());
				strcpy_s(database.Username, username.c_str());
				strcpy_s(database.Password, Cipher::Vigenere(password, Key).c_str());

				// Writing database to the file
				$User.Write(database);

				Tools::PrintMessage(2, 12, Message::Information, "Pendaftaran akun berhasil");
				break;
			}
		}
	} while (sSignUp.Second != "[Back]"); // Back
}
Result<bool, String> SignIn() {
	ConsoleMenu mSignIn{
		{
			"Username :",
			"Password :",
			"[SignIn]",
			"[Back]"
		},
		CursorColor,
		{2, 6}
	};
	ConsoleMenu::Selection sSignIn;
	String username;
	String password;

	do { // Main loop
		Tools::Print(2, 2, "=============");
		Tools::Print(2, 3, "   SIGN IN");
		Tools::Print(2, 4, "=============");

		sSignIn = mSignIn.Print();

		switch (sSignIn.First) {
		case 0: // Username
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), username);
			username = Console::GetLine();
			break;
		case 1: // Password
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), password);
			password = Tools::GetPassword();
			break;
		}

		if (sSignIn.Second == "[SignIn]") { // SignIn
			// Checking user input
			if (username.empty() || password.empty())
				Tools::PrintMessage(2, 11, Message::Warning, "Silakan lengkapi semua data");

			// Final stage
			else {
				// Pergi sesuai kondisi
				switch ($User.SignIn(username.c_str(), Cipher::Vigenere(password, Key).c_str())) {
				case User::Condition::UsernameNotFound:
					Tools::PrintMessage(2, 11, Message::Information, "Akun tidak ditemukan");
					break;
				case User::Condition::UserIsBlocked:
					Tools::PrintMessage(2, 11, Message::Information, "Akun diblokir");
					break;
				case User::Condition::WrongPassword:
					Tools::PrintMessage(2, 11, Message::Information, "Password salah");
					break;
				case User::Condition::Success:
					return { true, username };
					break;
				}
			}
		}
	} while (sSignIn.Second != "[Back]"); // Back

	return { false, "" };
}
void Home(String username) {
	Console::Print(username);
	Console::GetKey();
}

int main() {
	try { // Configuring console layout
		Console::DisableMaximizeButton();
		Console::DisableResize();
		Console::SetSize(100, 30);
		Console::SetFont(L"Jetbrains Mono", 20);
		Console::SetPositionToCenter();
	}
	catch (Exception& e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error!\n",
			"File: ", e.File(), "\n",
			"Line: ", e.Line(), "\n",
			"Function: ", e.Function(), "\n",
			"Description: ", e.Description()
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

	try { // Main program
		ConsoleMenu mDashboard{
			{
				"1. Sign Up",
				"2. Sign In",
				"[Exit]"
			},
			CursorColor,
			{2, 6}
		};
		ConsoleMenu::Selection sDashboard;

		do { // Main loop
			Tools::Print(2, 2, "===================");
			Tools::Print(2, 3, "   SMK LOCALHOST");
			Tools::Print(2, 4, "===================");

			sDashboard = mDashboard.Print();

			switch (sDashboard.First) {
			case 0: // Sign Up
				Console::Clear();
				SignUp();
				Console::Clear();
				break;
			case 1: // Sign In
				Console::Clear();
				Result signIn = SignIn();
				Console::Clear();
				if (signIn.First)
					Home(signIn.Second);
				Console::Clear();
				break;
			}
		} while (sDashboard.Second != "[Exit]"); // Exit

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