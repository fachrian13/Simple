#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::Coordinate;
using Simple::Utility::Tools;
using Simple::Utility::Menu::Input;
using Simple::Utility::Menu::Dropdown;

void Register();

int main()
{
	Console::EnableVirtualTerminal();
	Console::SetWindowSize(120, 30);
	Console::DisableResizeWindow();

	Dropdown dashboard
	{
		{
			"1. Registrasi Siswa",
			"2. Tampilkan seluruh data",
			"3. Cari data Siswa",
			"4. Perbarui data",
			"5. Hapus data siswa",
			"[Kembali]"
		},
		{ 3, 6 }
	};

	do
	{
		Console::Write(Coordinate{ 3, 2 }, "===============");
		Console::Write(Coordinate{ 3, 3 }, "   LOCALHOST");
		Console::Write(Coordinate{ 3, 4 }, "===============");

		dashboard.Run();

		switch (dashboard.Selected.Index)
		{
		case 0:
			Console::Clear();
			Register();
			Console::Clear();
			break;
		case 5:
			dashboard.Stop();
			break;
		}
	} while (dashboard.Running);
	return 0;
}

void Register()
{
	std::string namaLengkap;
	std::string alamat;
	std::string tanggalLahir;
	std::string bulanLahir;
	std::string tahunLahir;
	std::string jenisKelamin;
	Input iRegister
	{
		{
			{ "Nama Lengkap  :", &namaLengkap },
			{ "Alamat        :", &alamat },
			{ "Tanggal Lahir :", &tanggalLahir },
			{ "Bulan Lahir   :", &bulanLahir },
			{ "Tanun Lahir   :", &tahunLahir },
			{ "Jenis Kelamin :", &jenisKelamin },
			{ "[Registrasi]", nullptr },
			{ "[Kembali]", nullptr }
		},
		{ 3, 6 }
	};

	do
	{
		Console::Write(Coordinate{ 3, 2 }, "======================");
		Console::Write(Coordinate{ 3, 3 }, "   REGISTRASI SISWA");
		Console::Write(Coordinate{ 3, 4 }, "======================");

		iRegister.Run();

		switch (iRegister.Selected.Index)
		{
		case 0:
			Console::Write(" ");
			Tools::Clear(namaLengkap);
			namaLengkap = Console::ReadLine();
			break;
		case 1:
			Console::Write(" ");
			Tools::Clear(alamat);
			alamat = Console::ReadLine();
			break;
		case 6:
		{
			if (namaLengkap.empty())
				Tools::WriteMessage({ 3, 15 }, { Color::Yellow, Color::Black }, "WARNING", "Silakan lengkapi data.");
		}
		break;
		case 7:
			iRegister.Stop();
			break;
		}
	} while (iRegister.Running);
}