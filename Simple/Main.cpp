#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::ConsoleColor;
using Simple::System::Exception;
using Simple::Utility::Convert;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu;

int main()
{
	try
	{
		ConsoleMenu mDashboard(
			{
				"Teknik Elektronika",
				"Teknik Ketenagalistrikan",
				"Teknik Perkapalan",
				"Teknologi Tekstil",
				"Teknologi Pesawat Udara",
				"Teknik Grafika",
				"Teknik Otomotif",
				"Teknik Mesin",
				"Teknik Konstruksi Properti",
				"Teknik Kimia",
				"Teknik Industri",
				"Instrumental Industri",
				"Seni Industri Kreatif",
				"Agribisnis Agroteknologi",
				"Kesehatan dan Pekerjaan Sosial",
				"Pariwisata",
				"Bisnis Manajemen",
				"Teknologi dan Rekayasa",
				"Kemaritiman",
				"Teknologi Informasi",
				"Energi Pertambangan"
			}
		, { 6, 2 });

		mDashboard.Run(10);

		std::cout << "\n\n\n" << "Index: " << mDashboard.Selected.Index << "\n" << "Value: ";
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