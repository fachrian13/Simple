#include "Simple.h"

using Simple::System::Coordinate;
using Simple::System::Color;
using Simple::System::Console;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu;

void Registrasi()
{
	ConsoleMenu mRegistrasi
	{
		{
			"Nama lengkap  :",
			"Alamat        :",
			"Usia          :",
			"Tempat lahir  :",
			"Tanggal lahir :",
			"Bulan lahir   :",
			"Tahun lahir   :",
			"Jenis kelamin :",
			"[Registrasi]",
			"[Back]"
		},
		{ 3, 6 },
		{ Color::Green, Color::Black }
	};
	ConsoleMenu mTanggal
	{
		{
			"1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
			"11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
			"21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
			"31"
		},
		{ 19, 10 },
		{ Color::Green, Color::Black },
		true
	};
	ConsoleMenu mBulan
	{
		{
			"Januari" , "Februari" , "Maret" , "April",
			"Mei" , "Juni" , "Juli" , "Agustus",
			"September" , "Oktober" , "November", "Desember"
		},
		{ 19, 11 },
		{ Color::Green, Color::Black },
		true
	};
	ConsoleMenu mTahun
	{
		{
			"1971", "1972", "1973", "1974", "1975", "1976", "1977", "1978", "1979", "1980",
			"1981", "1982", "1983", "1984", "1985", "1986", "1987", "1988", "1989", "1990",
			"1991", "1992", "1993", "1994", "1995", "1996", "1997", "1998", "1999", "2000",
			"2001", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009", "2010"
		},
		{ 19, 12 },
		{ Color::Green, Color::Black },
	};
	ConsoleMenu mJenisKelamin
	{
		{
			"Pria",
			"Wanita"
		},
		{ 19, 13 },
		{ Color::Green, Color::Black },
		true
	};
	std::string nama;
	std::string alamat;
	std::string usia;
	std::string tempatLahir;
	std::string tanggalLahir;
	std::string bulanLahir;
	std::string tahunLahir;
	std::string jenisKelamin;

	do
	{
		Console::Write(Coordinate{ 3, 2 }, "======================");
		Console::Write(Coordinate{ 3, 3 }, "   REGISTRASI SISWA");
		Console::Write(Coordinate{ 3, 4 }, "======================");

		Console::Write(Coordinate{ 19, 6 }, nama);
		Console::Write(Coordinate{ 19, 7 }, alamat);
		Console::Write(Coordinate{ 19, 8 }, usia);
		Console::Write(Coordinate{ 19, 9 }, tempatLahir);
		Console::Write(Coordinate{ 19, 10 }, tanggalLahir);
		Console::Write(Coordinate{ 19, 11 }, bulanLahir);
		Console::Write(Coordinate{ 19, 12 }, tahunLahir);
		Console::Write(Coordinate{ 19, 13 }, jenisKelamin);

		mRegistrasi.Run();

		switch (mRegistrasi.Selected.Index)
		{
		case 0:
			Console::Write(" ");
			Tools::Clear(nama);
			nama = Console::ReadLine();
			break;
		case 1:
			Console::Write(" ");
			Tools::Clear(alamat);
			alamat = Console::ReadLine();
			break;
		case 2:
			Console::Write(" ");
			Tools::Clear(usia);
			usia = Console::ReadLine();
			break;
		case 3:
			Console::Write(" ");
			Tools::Clear(tempatLahir);
			tempatLahir = Console::ReadLine();
			break;
		case 4:
			mTanggal.Run(7);
			mTanggal.Clear();
			tanggalLahir = mTanggal.Selected.Value;
			break;
		case 5:
			mBulan.Run(7);
			mBulan.Clear();
			bulanLahir = mBulan.Selected.Value;
			break;
		case 6:
			mTahun.Run(7);
			mTahun.Clear();
			tahunLahir = mTahun.Selected.Value;
			break;
		case 7:
			mJenisKelamin.Run();
			mJenisKelamin.Clear();
			jenisKelamin = mJenisKelamin.Selected.Value;
			break;
		}
	} while (strcmp(mRegistrasi.Selected.Value, "[Back]") != 0);
}

void MainMenu()
{
	ConsoleMenu mMainMenu
	{
		{
			"1. Registrasi siswa",
			"2. Lihat seluruh data",
			"3. Cari data siswa",
			"4. Update data",
			"5. Hapus data siswa",
			"[Back]"
		},
		{ 3, 6 },
		{ Color::Green, Color::Black }
	};

	do
	{
		Console::Write(Coordinate{ 3, 2 }, "=============================");
		Console::Write(Coordinate{ 3, 3 }, "   SMK LOCALHOST DASHBOARD");
		Console::Write(Coordinate{ 3, 4 }, "=============================");

		mMainMenu.Run();

		switch (mMainMenu.Selected.Index)
		{
		case 0:
			Console::Clear();
			Registrasi();
			Console::Clear();
			break;
		}
	} while (strcmp(mMainMenu.Selected.Value, "[Back]") != 0);
}

int main()
{
	ConsoleMenu mDashboard
	{
		{
			"Username :",
			"Password :",
			"[Sign In]",
			"[Exit]"
		},
		{ 3, 6 },
		{ Color::Green, Color::Black }
	};
	std::string username;
	std::string password;

	Console::CursorBlinking = false;
	do
	{
		Console::Write(Coordinate{ 3, 2 }, "===============");
		Console::Write(Coordinate{ 3, 3 }, "   LOCALHOST");
		Console::Write(Coordinate{ 3, 4 }, "===============");

		mDashboard.Run();

		switch (mDashboard.Selected.Index)
		{
		case 0:
			Console::Write(" ");
			Tools::Clear(username);
			username = Console::ReadLine();
			break;
		case 1:
			Console::Write(" ");
			Tools::Clear(password);
			password = Tools::ReadPassword();
			break;
		}

		if (strcmp(mDashboard.Selected.Value, "[Sign In]") == 0)
		{
			if (username == "localhost" && password == "localhost")
			{
				Console::Clear();
				MainMenu();
				Console::Clear();
			}
		}
	} while (strcmp(mDashboard.Selected.Value, "[Exit]") != 0);

	return 0;
}