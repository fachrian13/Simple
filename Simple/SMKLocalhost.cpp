#include "Simple.h"

using Simple::System::Color;
using Simple::System::Console;
using Simple::System::Exception;
using Simple::System::BinaryFile;
using Simple::System::Coordinate;
using Simple::Utility::ConsoleMenu;
using Simple::Utility::Tools;

struct DatabaseSiswa final
{
	unsigned long Id;
	char NamaLengkap[30];
	char Alamat[70];
	char KotaLahir[30];
	unsigned int TanggalLahir;
	char BulanLahir[10];
	unsigned int TahunLahir;
	char JenisKelamin[10];

	DatabaseSiswa() {}

	DatabaseSiswa(const char* namaLengkap, const char* alamat, const char* kotaLahir, int tanggalLahir, const char* bulanLahir, int tahunLahir, const char* jenisKelamin)
	{
		strcpy_s(this->NamaLengkap, namaLengkap);
		strcpy_s(this->Alamat, alamat);
		strcpy_s(this->KotaLahir, kotaLahir);
		this->TanggalLahir = tanggalLahir;
		strcpy_s(this->BulanLahir, bulanLahir);
		this->TahunLahir = tahunLahir;
		strcpy_s(this->JenisKelamin, jenisKelamin);
	}
};

struct FileSiswa final : BinaryFile<DatabaseSiswa>
{
	FileSiswa() : BinaryFile("Siswa.bin") {}

	unsigned long GetLastId()
	{
		DatabaseSiswa temp;

		this->File.open(this->FileName, std::ios::in | std::ios::binary);
		if (!this->File)
			THROW("Gagal membuka file.");

		this->File.seekp(-1 * sizeof DatabaseSiswa, std::ios::end);
		this->File.read((char*)&temp, sizeof DatabaseSiswa);
		this->File.close();
		return temp.Id;
	}
} FileSiswa;

void Registrasi();
void LihatData();
void CariData();

int main()
{
	try
	{
		ConsoleMenu mDashboard
		{
			{
				"1. Registrasi siswa",
				"2. Lihat seluruh data",
				"3. Cari data siswa",
				"4. Update data",
				"5. Hapus data siswa",
				"[Keluar]"
			},
			{ 3, 6 },
			{ Color::White, Color::Black }
		};

		do
		{
			Console::Write(Coordinate{ 3, 2 }, "===================");
			Console::Write(Coordinate{ 3, 3 }, "   SMK LOCALHOST");
			Console::Write(Coordinate{ 3, 4 }, "===================");

			mDashboard.Run();

			switch (mDashboard.Selected.Index)
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
			case 2:
				Console::Clear();
				CariData();
				Console::Clear();
				break;
			}

			if (mDashboard.Clicked("[Keluar]"))
				mDashboard.Stop();
		} while (mDashboard.Running);
	}
	catch (Exception& e)
	{
		Console::Clear();
		Console::Write
		(
			"Terjadi kesalahan pada program!"
			"\nFile : ", e.File,
			"\nLine : ", e.Line,
			"\nFunction : ", e.Function,
			"\nDescription : ", e.Description
		);
		Console::ReadKey();
		return 1;
	}
	catch (std::exception& e)
	{
		Console::Clear();
		Console::Write
		(
			"Terjadi kesalahan pada program!",
			"\nDescription : ", e.what()
		);
		Console::ReadKey();
		return 1;
	}
	return 0;
}

void Registrasi()
{
	ConsoleMenu mRegistrasi
	{
		{
			"Nama lengkap  :",
			"Alamat        :",
			"Tempat lahir  :",
			"Tanggal lahir :",
			"Bulan lahir   :",
			"Tahun lahir   :",
			"Jenis kelamin :",
			"[Registrasi]",
			"[Kembali]"
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
		{ mRegistrasi.IndexPosition.X[3] + 4, mRegistrasi.IndexPosition.Y[3] },
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
		{ mRegistrasi.IndexPosition.X[4] + 4, mRegistrasi.IndexPosition.Y[4] },
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
		{ mRegistrasi.IndexPosition.X[5] + 4, mRegistrasi.IndexPosition.Y[5] },
		{ Color::White, Color::Black },
	};
	ConsoleMenu mJenisKelamin
	{
		{
			"Pria",
			"Wanita"
		},
		{ mRegistrasi.IndexPosition.X[6] + 4, mRegistrasi.IndexPosition.Y[6] },
		{ Color::White, Color::Black },
		true
	};
	std::string namaLengkap;
	std::string alamat;
	std::string kotaLahir;
	std::string tanggalLahir;
	std::string bulanLahir;
	std::string tahunLahir;
	std::string jenisKelamin;

	do
	{
		Console::Write(Coordinate{ 3, 2 }, "===========================");
		Console::Write(Coordinate{ 3, 3 }, "   REGISTRASI SISWA BARU");
		Console::Write(Coordinate{ 3, 4 }, "===========================");

		Console::Write(Coordinate{ mRegistrasi.IndexPosition.X[0] + 4, mRegistrasi.IndexPosition.Y[0] }, namaLengkap);
		Console::Write(Coordinate{ mRegistrasi.IndexPosition.X[1] + 4, mRegistrasi.IndexPosition.Y[1] }, alamat);
		Console::Write(Coordinate{ mRegistrasi.IndexPosition.X[2] + 4, mRegistrasi.IndexPosition.Y[2] }, kotaLahir);
		Console::Write(Coordinate{ mRegistrasi.IndexPosition.X[3] + 4, mRegistrasi.IndexPosition.Y[3] }, tanggalLahir);
		Console::Write(Coordinate{ mRegistrasi.IndexPosition.X[4] + 4, mRegistrasi.IndexPosition.Y[4] }, bulanLahir);
		Console::Write(Coordinate{ mRegistrasi.IndexPosition.X[5] + 4, mRegistrasi.IndexPosition.Y[5] }, tahunLahir);
		Console::Write(Coordinate{ mRegistrasi.IndexPosition.X[6] + 4, mRegistrasi.IndexPosition.Y[6] }, jenisKelamin);

		mRegistrasi.Run();

		switch (mRegistrasi.Selected.Index)
		{
		case 0:
			Console::Write(" ");
			Tools::Clear(namaLengkap);
			namaLengkap = Tools::ReadLine(sizeof DatabaseSiswa::NamaLengkap);
			break;
		case 1:
			Console::Write(" ");
			Tools::Clear(alamat);
			alamat = Tools::ReadLine(sizeof DatabaseSiswa::Alamat);
			break;
		case 2:
			Console::Write(" ");
			Tools::Clear(kotaLahir);
			kotaLahir = Tools::ReadLine(sizeof DatabaseSiswa::KotaLahir);
			break;
		case 3:
			mTanggal.Run(7);
			mTanggal.Clear();
			tanggalLahir = mTanggal.Selected.Value;
			break;
		case 4:
			mBulan.Run(7);
			mBulan.Clear();
			bulanLahir = mBulan.Selected.Value;
			break;
		case 5:
			mTahun.Run(7);
			mTahun.Clear();
			tahunLahir = mTahun.Selected.Value;
			break;
		case 6:
			mJenisKelamin.Run();
			mJenisKelamin.Clear();
			jenisKelamin = mJenisKelamin.Selected.Value;
			break;
		case 7:
		{
			if (namaLengkap.empty() || alamat.empty() || kotaLahir.empty() || tanggalLahir.empty() || bulanLahir.empty() || tahunLahir.empty() || jenisKelamin.empty())
				Tools::WriteMessage({ 3, mRegistrasi.IndexPosition.Y[8] + 2 }, { Color::Yellow, Color::Black }, "PERINGATAN", "Silakan isi seluruh data.");
			if (!Tools::RegexMatch(namaLengkap, "[a-zA-Z]+"))
				Tools::WriteMessage({ 3, mRegistrasi.IndexPosition.Y[8] + 2 }, { Color::Yellow, Color::Black }, "PERINGATAN", "Nama hanya menggunakan huruf.");
			else
			{
				DatabaseSiswa dataSiswa(namaLengkap.c_str(), alamat.c_str(), kotaLahir.c_str(), std::stoi(tanggalLahir), bulanLahir.c_str(), std::stoi(tahunLahir), jenisKelamin.c_str());

				if (FileSiswa.Empty())
					dataSiswa.Id = 20221;
				else
				{
					std::string sId = std::to_string(FileSiswa.GetLastId());

					dataSiswa.Id = std::stoi(sId.replace(4, std::string::npos, std::to_string(std::stoi(sId.substr(4)) + 1)));
				}

				FileSiswa.Write(dataSiswa);
				Tools::WriteMessage({ 3, mRegistrasi.IndexPosition.Y[8] + 2 }, { Color::Yellow, Color::Black }, "INFORMASI", "Berhasil mendaftarkan siswa.");
				mRegistrasi.Stop();
			}
		}
			break;
		case 8:
			mRegistrasi.Stop();
			break;
		}
	} while (mRegistrasi.Running);
}

void LihatData()
{
	const auto fileData = FileSiswa.Read();
	int no = 1;

	Console::Write(Coordinate{ 3, 2 }, "================");
	Console::Write(Coordinate{ 3, 3 }, "   DATA SISWA");
	Console::Write(Coordinate{ 3, 4 }, "================");

	Console::Write(Coordinate{ 0, 6 });
	for (const auto& index : fileData)
	{
		Console::Write
		(
			"  No            : ", no, "\n",
			"  Id Siswa      : ", index.Id, "\n",
			"  Nama lengkap  : ", index.NamaLengkap, "\n",
			"  Alamat        : ", index.Alamat, "\n",
			"  Kota lahir    : ", index.KotaLahir, "\n",
			"  Tanggal lahir : ", index.TanggalLahir, "\n",
			"  Bulan lahir   : ", index.BulanLahir, "\n",
			"  Tahun lahir   : ", index.TahunLahir, "\n",
			"  Jenis kelamin : ", index.JenisKelamin, "\n\n"
		);
		no++;
	}
	Console::ReadKey();
}

void CariData()
{
	ConsoleMenu mCari
	{
		{
			"Id/Nama Siswa :",
			"[Cari]",
			"[Kembali]"
		},
		{ 3, 7 },
		{ Color::White, Color::Black }
	};
	std::string cari;

	do
	{
		Console::Write(Coordinate{ 3, 2 }, "=====================");
		Console::Write(Coordinate{ 3, 3 }, "   CARI DATA SISWA");
		Console::Write(Coordinate{ 3, 4 }, "=====================");
		Console::Write(Coordinate{ 3, 5 }, "Masukkan Id atau Nama Siswa.");

		mCari.Run();

		switch (mCari.Selected.Index)
		{
		case 0:
			Console::Write(" ");
			Tools::Clear(cari);
			cari = Tools::ReadLine(sizeof DatabaseSiswa::NamaLengkap);
			break;
		case 1:
		{
			if (cari.empty())
				Tools::WriteMessage({ 3, mCari.IndexPosition.Y[2] + 2 }, { Color::Yellow, Color::Black }, "PERINGATAN", "Silakan lengkapi data.");
			else if (Tools::RegexMatch(cari, "[0-9]+"))
			{
				if (Tools::RegexSearch(cari, "2022[0-9]+"))
				{
					const auto fileData = FileSiswa.Read();
					const auto ulCari = std::stoul(cari);
					const auto exist = std::find_if(fileData.begin(), fileData.end(), [&ulCari](DatabaseSiswa i) { return i.Id == ulCari; });

					if (exist != fileData.end())
					{
						Console::Write
						(
							Coordinate{ 0, mCari.IndexPosition.Y[2] + 2 },
							"  Id Siswa      : ", exist->Id, "\n",
							"  Nama lengkap  : ", exist->NamaLengkap, "\n",
							"  Alamat        : ", exist->Alamat, "\n",
							"  Kota lahir    : ", exist->KotaLahir, "\n",
							"  Tanggal lahir : ", exist->TanggalLahir, "\n",
							"  Bulan lahir   : ", exist->BulanLahir, "\n",
							"  Tahun lahir   : ", exist->TahunLahir, "\n",
							"  Jenis kelamin : ", exist->JenisKelamin
						);
						Console::ReadKey();
						for (int i = mCari.IndexPosition.Y[2] + 2; i <= 19; i++)
							Tools::EraseCharacter({ 3, i }, 2, 'K');
					}
					else
						Tools::WriteMessage({ 3, mCari.IndexPosition.Y[2] + 2 }, { Color::Green, Color::Black }, "INFORMASI", "Data Siswa tidak ditemukan.");
				}
				else
					Tools::WriteMessage({ 3, mCari.IndexPosition.Y[2] + 2 }, { Color::Yellow, Color::Black }, "PERINGATAN", "Id Siswa salah.");
			}
			else if (Tools::RegexMatch(cari, "[a-zA-Z]+"))
			{
				const auto fileData = FileSiswa.Read();
				std::vector<DatabaseSiswa> found;
				int no = 1;

				for (const auto& index : fileData)
					if (Tools::RegexSearch(index.NamaLengkap, cari + "\\s?[a-zA-Z]*"))
						found.push_back(index);

				Console::Write(Coordinate{ 0, mCari.IndexPosition.Y[2] + 2 });
				for (const auto& index : found)
				{
					Console::Write
					(
						"  No            : ", no, "\n",
						"  Id Siswa      : ", index.Id, "\n",
						"  Nama lengkap  : ", index.NamaLengkap, "\n",
						"  Alamat        : ", index.Alamat, "\n",
						"  Kota lahir    : ", index.KotaLahir, "\n",
						"  Tanggal lahir : ", index.TanggalLahir, "\n",
						"  Bulan lahir   : ", index.BulanLahir, "\n",
						"  Tahun lahir   : ", index.TahunLahir, "\n",
						"  Jenis kelamin : ", index.JenisKelamin, "\n\n"
					);
					no++;
				};
				Console::ReadKey();
			}
			else
				Tools::WriteMessage({ 3, mCari.IndexPosition.Y[2] + 2 }, { Color::Yellow, Color::Black }, "PERINGATAN", "Tidak dapat melakukan pencarian.");
		}
			break;
		case 2:
			mCari.Stop();
			break;
		}
	} while (mCari.Running);
}