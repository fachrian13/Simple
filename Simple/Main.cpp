#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::ConsoleColor;
using Simple::System::Exception;
using Simple::Utility::Convert;

int main()
{
	try
	{
		Console::SetColor({ Color::Green, Color::Red });
		ConsoleColor color = Console::GetColor();
		Console::SetCursorPosition({ 10, 5 }); Console::WriteLine("Hello World");
		Console::Write(Convert::ToString(color.Background), " ", Convert::ToString(color.Foreground));
		Console::Read();
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