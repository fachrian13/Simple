#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::Exception;
using Simple::Utility::ConsoleMenu;
using Simple::Utility::ConsoleTable;
using Simple::Utility::Tools;


class CommaSeparatedValue {
private:
	std::fstream _File;
	std::filesystem::path _Path;

public:
	CommaSeparatedValue(std::filesystem::path path) : _Path(path) {}

	//
	// Membaca isi dari file csv.
	// 
	// @return std::vector<std::vector<std::string>>: Data dari dalam file.
	//
	std::vector<std::vector<std::string>> Read() {
		std::string line;
		std::vector<std::vector<std::string>> stored;

		_File.open(_Path, std::ios::in);
		if (!_File)
			throw std::runtime_error{ "Gagal membuka file." };
		while (getline(_File, line)) {
			std::string data;
			std::stringstream ss(line);
			std::vector<std::string> temp;

			while (getline(ss, data, ','))
				temp.push_back(data);
			stored.push_back(temp);
		}
		_File.close();
		return stored;
	}
};

int main() {
	try {
		// Configuring windows
		Console::Size = { 120, 35 };
		Console::SetFont(L"Consolas", 20);
		Console::DisableMaximizeButton();
		Console::DisableMinimizeButton();
		Console::DisableResize();
		Console::SetPositionToCenter();

		ConsoleMenu mDashboard{
			{
				"1. Print \"Hello World\"",
				"2. About",
				"3. CSV Example Read",
				"[Exit]"
			},
			{ Color::Green, Color::Black },
			{ 2, 6 }
		};
		ConsoleMenu::Selected sDashboard;

		Console::Clear();
		do {
			Tools::Write(2, 2, "============");
			Tools::Write(2, 3, "   SIMPLE");
			Tools::Write(2, 4, "============");

			sDashboard = mDashboard.Print();

			switch (sDashboard.Index) {
			case 0:
				Console::Clear();
				Console::Write("Hello World");
				Console::ReadKey();
				Console::Clear();
				break;
			case 1:
				Console::Clear();
				Console::Write(
					"Simple.h/n\n"

					"Header C++ berisi kelas-kelas yang bisa membantu untuk menulis kode.\n"
					"Header ini dibuat dengan tujuan untuk pembelajaran, masih banyak bug yang terdapat pada header ini,\n"
					"Jadi pengguna diharapkan berhati-hati dalam menggunakannya. Tidak semua kode yang terdapat disini\n"
					"ditulis sendiri, namum ada beberapa yang diambil dari berbagai sumber."
				);
				Console::ReadKey();
				Console::Clear();
				break;
			case 2:
				Console::Clear();

				CommaSeparatedValue csv("C:/Users/Fanny Fachrian/Downloads/10000-Sales-Records.csv");
				auto data = csv.Read();
				ConsoleTable table(data[0]);
				data.erase(data.begin());

				for (auto index : data)
					table += index;

				Tools::Write(2, 2, "==================");
				Tools::Write(2, 3, "   SALES RECORD");
				Tools::Write(2, 4, "==================");
				Console::Cursor::Position = { 0, 6 };
				table.Print();
				Console::Read();
				Console::Clear();
				break;
			}
		} while (sDashboard.Value != "[Exit]");
	}
	catch (Exception& e) {
		Console::Clear();
		Console::Write(
			"Program mengalami error!\n",
			"File: ", e.File(), "\n",
			"Line: ", e.Line(), "\n",
			"Function: ", e.Function(), "\n",
			"Description: ", e.Description()
		);
		Console::ReadKey();
	}
	catch (std::exception& e) {
		Console::Clear();
		Console::Write(
			"Program mengalami error!\n",
			"Description: ", e.what()
		);
		Console::ReadKey();
	}
	return 0;
}