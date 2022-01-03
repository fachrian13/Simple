#include "Simple.h"

using Simple::System::BinaryFile;
using Simple::System::Coordinate;
using Simple::System::Color;
using Simple::System::ConsoleColor;
using Simple::System::Console;
using Simple::Utility::Tools;
using Simple::Utility::ConsoleMenu;

struct StructSiswa final
{
	char NamaLengkap[64];
	char Alamat[64];
	int Usia;
	char TempatLahir[32];
	int TanggalLahir;
	char BulanLahir[16];
	int TahunLahir;
	char JenisKelamin[16];

	StructSiswa() {}

	StructSiswa(const char* namaLengkap, const char* alamat, int usia, const char* tempatLahir, int tanggalLahir, const char* bulanLahir, int tahunLahir, const char* jenisKelamin)
	{
		strcpy_s(this->NamaLengkap, namaLengkap);
		strcpy_s(this->Alamat, alamat);
		this->Usia = usia;
		strcpy_s(this->TempatLahir, tempatLahir);
		this->TanggalLahir = tanggalLahir;
		strcpy_s(this->BulanLahir, bulanLahir);
		this->TahunLahir = tahunLahir;
		strcpy_s(this->JenisKelamin, jenisKelamin);
	}
};

BinaryFile<StructSiswa> FileSiswa("Siswa.bin");

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
		{ Color::White, Color::Black }
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
		{ Color::White, Color::Black },
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
		{ Color::White, Color::Black },
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
		{ Color::White, Color::Black },
	};
	ConsoleMenu mJenisKelamin
	{
		{
			"Pria",
			"Wanita"
		},
		{ 19, 13 },
		{ Color::White, Color::Black },
		true
	};
	std::string namaLengkap;
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

		Console::Write(Coordinate{ 19, 6 }, namaLengkap);
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
			Tools::Clear(namaLengkap);
			namaLengkap = Console::ReadLine();
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

		if (strcmp(mRegistrasi.Selected.Value, "[Registrasi]") == 0)
		{
			if (namaLengkap.empty() || alamat.empty() || tempatLahir.empty() || tanggalLahir.empty() || bulanLahir.empty() || tahunLahir.empty() || jenisKelamin.empty())
			{
				Console::Write(Coordinate{ 3, 17 }, ConsoleColor{ Color::Yellow, Color::Black }, "[WARNING]", ConsoleColor{ Color::Default, Color::Default }, " Silakan lengkapi data.");
				Console::ReadKey();
				Console::Write("\033[1K");
			}
			else if (!Tools::IsNumber(usia))
			{
				Console::Write(Coordinate{ 3, 17 }, ConsoleColor{ Color::Yellow, Color::Black }, "[WARNING]", ConsoleColor{ Color::Default, Color::Default }, " Usia siswa hanya menggunakan angka.");
				Console::ReadKey();
				Console::Write("\033[1K");
			}
			else
			{
				StructSiswa data(namaLengkap.c_str(), alamat.c_str(), std::stoi(usia), tempatLahir.c_str(), std::stoi(tanggalLahir), bulanLahir.c_str(), std::stoi(tahunLahir), jenisKelamin.c_str());

				FileSiswa.Write(data);

				Console::Write(Coordinate{ 3, 17 }, ConsoleColor{ Color::Green, Color::Black }, "[INFORMATION]", ConsoleColor{ Color::Default, Color::Default }, " Data berhasil disimpan.");
				Console::ReadKey();
				Console::Write("\033[1K");
				break;
			}
		}
	} while (strcmp(mRegistrasi.Selected.Value, "[Back]") != 0);
}

void LihatData()
{
	std::vector dataSiswa = FileSiswa.Read();
	int no = 1;

	Console::Write(Coordinate{ 3, 2 }, "=====================");
	Console::Write(Coordinate{ 3, 3 }, "   LIST DATA SISWA");
	Console::Write(Coordinate{ 3, 4 }, "=====================");
	Console::Write("\n\n");

	for (const auto& index : dataSiswa)
	{
		Console::Write
		(
			"  No            : ", no, "\n",
			"  Nama lengkap  : ", index.NamaLengkap, "\n",
			"  Alamat        : ", index.Alamat, "\n",
			"  Usia          : ", index.Usia, "\n",
			"  Tempat lahir  : ", index.TempatLahir, "\n",
			"  Tanggal lahir : ", index.TanggalLahir, "\n",
			"  Bulan lahir   : ", index.BulanLahir, "\n",
			"  Tahun lahir   : ", index.TahunLahir, "\n",
			"  Jenis kelamin : ", index.JenisKelamin, "\n\n"
		);
		no++;
	}
	Console::ReadKey();
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
		{ Color::White, Color::Black }
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
		case 1:
			Console::Clear();
			LihatData();
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
		{ Color::White, Color::Black }
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