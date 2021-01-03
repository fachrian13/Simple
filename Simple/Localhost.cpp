#include "Simple.h"
#include "User.h"

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

bool FirstPage() {
	ConsoleMenu mFirst{
		{
			"Username         :",
			"Password         :",
			"Confirm password :",
			"[Sign Up]",
			"[Exit]"
		},
		CursorColor,
		{2, 7}
	};
	ConsoleMenu::Selection sFirst;
	String username;
	String password;
	String password2;

	do { // Main loop
		Tools::Print(2, 2, "===================");
		Tools::Print(2, 3, "   SMK LOCALHOST");
		Tools::Print(2, 4, "===================");
		Tools::Print(2, 5, "Selamat datang! silakan buat akun dulu :)");

		sFirst = mFirst.Print();

		switch (sFirst.First) {
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

		if (sFirst.Second == "[Sign Up]") { // SignUp
			// Checking user input
			if (username.empty() || password.empty() || password2.empty())
				Tools::PrintMessage(2, 13, Message::Warning, "Silakan lengkapi semua data.");
			else if (!regex_match(username, StandardEx) || !regex_match(password, StandardEx))
				Tools::PrintMessage(2, 13, Message::Warning, "Username dan password hanya menggunakan huruf dan angka.");
			else if (username.length() < 7 || username.length() > 15 || password.length() < 7 || password.length() > 15)
				Tools::PrintMessage(2, 13, Message::Information, "Panjang username dan password 7-15 karakter.");
			else if ($User.CheckUsername(username.c_str()))
				Tools::PrintMessage(2, 13, Message::Information, "Username sudah digunakan.");
			else if (password != password2)
				Tools::PrintMessage(2, 13, Message::Information, "Password tidak sama.");

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

				Tools::PrintMessage(2, 13, Message::Information, "Pendaftaran akun berhasil.");

				return true;
			}
		}
	} while (sFirst.Second != "[Exit]"); // Back

	return false;
}

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

				Tools::PrintMessage(2, 12, Message::Information, "Pendaftaran akun berhasil.");
				break;
			}
		}
	} while (sSignUp.Second != "[Back]"); // Back
}

void Activate(Result<SizeType, DataUser> account) {
	ConsoleMenu mActivate{
		{
			"Key :",
			"[Activate]",
			"[Back]"
		},
		CursorColor,
		{2, 7}
	};
	ConsoleMenu::Selection sActivate;
	String key = Tools::GenerateRandomKey(20);
	String keyInput;

	do { // Main loop
		Tools::Print(2, 2, "======================");
		Tools::Print(2, 3, "   ACTIVATE ACCOUNT");
		Tools::Print(2, 4, "======================");
		Tools::Print(2, 5, "Masukkan kode ini: ", key);

		sActivate = mActivate.Print();

		switch (sActivate.First) {
		case 0: // Key
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), keyInput);
			keyInput = Console::GetLine();
			break;
		}

		if (sActivate.Second == "[Activate]") { // Activate
			// Checking user input
			if (keyInput.empty())
				Tools::PrintMessage(2, 11, Message::Information, "Silakan masukkan kode.");
			else if (keyInput != key)
				Tools::PrintMessage(2, 11, Message::Information, "Kode aktivasi tidak sama.");

			// Final stage
			else {
				DataUser newData = account.Second;
				newData.Blocked = false;

				$User.Update(account.First, newData);
				Tools::PrintMessage(2, 11, Message::Information, "Akun berhasil diaktivasi.");
				break;
			}
		}
	} while (sActivate.Second != "[Back]");
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
	String userTemp = "";
	short failed = 0;

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
			if (userTemp != username)
				userTemp = username, failed = 0;

			// Checking user input
			if (username.empty() || password.empty())
				Tools::PrintMessage(2, 11, Message::Warning, "Silakan lengkapi semua data.");

			// Final stage
			else {
				Vector<DataUser> stored = $User.Read();
				Result<bool, Result<SizeType, DataUser>> result{ false, {0, {}} };

				for (SizeType i = 0; i < stored.size(); i++)
					if (strcmp(username.c_str(), stored[i].Username) == 0)
						result = { true, {i, stored[i]} };

				if (!result.First)
					Tools::PrintMessage(2, 11, Message::Information, "Username tidak ditemukan.");
				else if (result.Second.Second.Blocked)
					Console::Clear(), Activate(result.Second), Console::Clear();
				else if (strcmp(Cipher::Vigenere(password, Key).c_str(), result.Second.Second.Password) != 0)
					Tools::PrintMessage(2, 11, Message::Information, "Password salah."), failed++;
				else
					return { true, username };

				if (failed == 3) {
					DataUser newData = result.Second.Second;
					newData.Blocked = true;

					Tools::PrintMessage(2, 11, Message::Information, "Anda sudah 3 kali salah password, akun sementara diblokir.");
					$User.Update(result.Second.First, newData);
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
		Console::SetFont(L"Jetbrains Mono", 18);
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

		if ($User.Empty()) {
			if (FirstPage()) {
				Console::Clear();
				goto ActuallyIDontWantToUseGotoButImOutOfIdea;
			}
		}
		else {
		ActuallyIDontWantToUseGotoButImOutOfIdea:

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
		}

		return EXIT_SUCCESS;
	}
	catch (Exception& e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error :(\n",
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
			"Program mengalami error :(\n",
			"Description: ", e.what()
		);
		Console::GetKey();

		return EXIT_FAILURE;
	}
}