#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::ConsoleColor;
using Simple::System::Exception;
using Simple::Utility::Convert;
using Simple::Utility::Tools;

int main()
{
	try
	{
		std::string password;

		std::cout << "Password: "; password = Tools::ReadPassword(10);

		std::cout << "\n\n" << password;
	}
	catch (Exception& e)
	{
		system("cls");
		std::cout
			<< "Program mengalami error!"
			<< "\nFile: " << e.File
			<< "\nLine: " << e.Line
			<< "\nFunction: " << e.Function
			<< "\nDescription: " << e.Description;
		std::cin.get();
	}
}