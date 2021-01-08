#include "Simple.h"
#include "User.h"
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
using Utility::ConsoleTable;
using Utility::Message;
using Utility::Result;
using Utility::Tools;

const ConsoleColor CursorColor{ Color::Green, Color::Black };
const Regex StandardEx{ "[a-zA-Z0-9]+" };
const Regex NisEx{ "\\b(2021)([^ ]*)" };
const String Key = "qwertyuiop";

User $User;
Siswa $Siswa;

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
				strcpy_s(database.DateTime, Time::Now().StandardFormat.c_str());
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
				strcpy_s(database.DateTime, Time::Now().StandardFormat.c_str());
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

void Tambah() {
	ConsoleMenu mRegistrasi{
		{
			"Nama siswa    :",
			"Alamat        :",
			"Tempat lahir  :",
			"Tanggal lahir :",
			"Agama         :",
			"Jenis kelamin :",
			"Jurusan       :",
			"[Daftar]",
			"[Kembali]"
		},
		CursorColor,
		{2, 7}
	};
	ConsoleMenu mTanggal{
		{
			"1"	, "2"	, "3"	, "4"	, "5"	, "6"	, "7"	, "8"	, "9"	, "10",
			"11", "12"	, "13"	, "14"	, "15"	, "16"	, "17"	, "18"	, "19"	, "20",
			"21", "22"	, "23"	, "24"	, "25"	, "26"	, "27"	, "28"	, "29"	, "30",
			"31"
		},
		CursorColor,
		{18, 10},
		true
	};
	ConsoleMenu mBulan{
		{
			"Januari"	, "Februari"	, "Maret"		, "April",
			"Mei"		, "Juni"		, "Juli"		, "Agustus",
			"September"	, "Oktober"		, "November"	, "Desember"
		},
		CursorColor,
		{21, 10},
		true
	};
	ConsoleMenu mTahun{
		{
			"1971",	"1972",	"1973",	"1974",	"1975",	"1976",	"1977",	"1978",	"1979",	"1980",
			"1981",	"1982",	"1983",	"1984",	"1985",	"1986",	"1987",	"1988",	"1989",	"1990",
			"1991",	"1992",	"1993",	"1994",	"1995",	"1996",	"1997",	"1998",	"1999",	"2000",
			"2001",	"2002",	"2003",	"2004",	"2005",	"2006",	"2007",	"2008",	"2009",	"2010"
		},
		CursorColor,
		{31, 10}
	};
	ConsoleMenu mAgama{
		{
			"Islam",
			"Katolik",
			"Protestan",
			"Hindu",
			"Budha",
			"Khonghucu"
		},
		CursorColor,
		{18, 11},
		true
	};
	ConsoleMenu mJk{
		{
			"Pria",
			"Wanita"
		},
		CursorColor,
		{18, 12},
		true
	};
	ConsoleMenu mJurusan{
		{
			"Seni Industri Kreatif",
			"Agribisnis Agroteknologi",
			"Kesehatan dan Pekerjaan Sosial",
			"Pariwisata",
			"Bisnis Manajemen",
			"Teknologi dan Rekayasa",
			"Kemaritiman",
			"Teknologi Informasi",
			"Energi Pertambangan"
		},
		CursorColor,
		{18, 13},
		true
	};
	ConsoleMenu mJList[]
	{
		// Seni Industri Kreatif
		{
			{
				"Seni Tari",
				"Seni Pedalangan",
				"Seni Teater",
				"Seni Karawitan",
				"Seni Broadcastring dan Film"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Agribisnis Agroteknologi
		{
			{
				"Agribisnis Pengolahan Hasil Pertanian",
				"Teknik Pertanian",
				"Kesehatan Hewan",
				"Kehutanan",
				"Agribisnis Tanaman"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Kesehatan dan Pekerjaan Sosial
		{
			{
				"Pekerjaan Sosial",
				"Kesehatan Gigi",
				"Keperawatan",
				"Farmasi"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Pariwisata
		{
			{
				"Tata Kecantikan",
				"Tata Busana",
				"Kuliner",
				"Perhotelan dan Jasa Pariwisata"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Bisnis Manajemen
		{
			{
				"Akuntansi dan Keuangan",
				"Bisnis dan Pemasaran",
				"Manajemen Perkantoran"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Teknologi dan Rekayasa
		{
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
				"Instrumental Industri"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Kemaritiman
		{
			{
				"Perikanan",
				"Pengolahan Hasil Perikanan",
				"Pelayaran Kapal Niaga"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Teknologi Informasi
		{
			{
				"Teknik Telekomunikasi",
				"Teknik Komputer Informatika"
			},
			CursorColor,
			{49, 13},
			true
		},

		// Energi Pertambangan
		{
			{
				"Geologi Pertambangan",
				"Teknik Perminyakan"
			},
			CursorColor,
			{49, 13},
			true
		}
	};
	ConsoleMenu::Selection sRegistrasi;
	String nama;
	String alamat;
	String tempat;
	String tanggal;
	String agama;
	String jk;
	String jurusan;

	do { // Main loop
		Tools::Print(2, 2, "======================");
		Tools::Print(2, 3, "   REGISTRASI SISWA");
		Tools::Print(2, 4, "======================");
		Tools::Print(2, 5, "Silakan lengkapi data dibawah ini");

		Tools::Print(18, 7, nama);
		Tools::Print(18, 8, alamat);
		Tools::Print(18, 9, tempat);
		Tools::Print(18, 10, tanggal);
		Tools::Print(18, 11, agama);
		Tools::Print(18, 12, jk);
		Tools::Print(18, 13, jurusan);

		sRegistrasi = mRegistrasi.Print();

		switch (sRegistrasi.First) {
		case 0: // Nama siswa
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), nama);
			nama = Console::GetLine();
			break;
		case 1: // Alamat
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), alamat);
			alamat = Console::GetLine();
			break;
		case 2: // Tempat lahir
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), tempat);
			tempat = Console::GetLine();
			break;
		case 3: // Tanggal lahir
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), tanggal);
			tanggal += mTanggal.Print(5).Second + " ";
			tanggal += mBulan.Print(5).Second	+ " ";
			tanggal += mTahun.Print(5).Second;

			mTanggal.Clear();
			mBulan.Clear();
			mTahun.Clear();
			break;
		case 4: // Agama
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), agama);
			agama = mAgama.Print(5).Second;

			mAgama.Clear();
			break;
		case 5: // Jenis kelamin
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), jk);
			jk = mJk.Print().Second;

			mJk.Clear();
			break;
		case 6: // Jurusan
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), jurusan);

			ConsoleMenu::Selection result = mJurusan.Print(5);

			jurusan += result.Second + ": ";
			jurusan += mJList[result.First].Print(5).Second;

			mJurusan.Clear();
			mJList[result.First].Clear();
			break;
		}

		if (sRegistrasi.Second == "[Daftar]") { // Registrasi
			// Cheking user input
			if (nama.empty() || alamat.empty() || tempat.empty() || tanggal.empty() || agama.empty() || jk.empty() || jurusan.empty())
				Tools::PrintMessage(2, 17, Message::Warning, "Silakan lengkapi semua data.");
			else if (nama.length() >= 64)
				Tools::PrintMessage(2, 17, Message::Information, "Silakan persingkat nama siswa.");
			else if (alamat.length() >= 64)
				Tools::PrintMessage(2, 17, Message::Information, "Silakan persingkat alamat siswa.");
			else if (tempat.length() >= 64)
				Tools::PrintMessage(2, 17, Message::Information, "Tempat lahir terlalu panjang.");

			// Final stage
			else {
				DataSiswa database;
				long lNis = $Siswa.GetNis();
				String sNis = std::to_string(lNis);

				database.NIS = lNis == 0 ? 20211 : std::stoi(sNis.replace(4, -1, std::to_string(std::stoi(sNis.substr(4)) + 1)));
				strcpy_s(database.DateTime, System::Time::Now().StandardFormat.c_str());
				strcpy_s(database.Nama, nama.c_str());
				strcpy_s(database.Alamat, alamat.c_str());
				strcpy_s(database.TempatLahir, tempat.c_str());
				strcpy_s(database.TanggalLahir, tanggal.c_str());
				strcpy_s(database.Agama, agama.c_str());
				strcpy_s(database.JenisKelamin, jk.c_str());
				strcpy_s(database.Jurusan, jurusan.c_str());

				$Siswa.Write(database);

				Tools::PrintMessage(2, 17, Message::Information, "Registrasi siswa berhasil.");
				break;
			}
		}
	} while (sRegistrasi.Second != "[Kembali]");
}
void Lihat() {
	ConsoleTable tSiswa{
		"No",
		"Tanggal Registrasi",
		"Nomor Induk Siswa",
		"Nama siswa",
		"Alamat",
		"Tempat lahir",
		"Tanggal lahir",
		"Agama",
		"Jenis kelamin",
		"Jurusan",
	};
	Vector<DataSiswa> stored = $Siswa.Read();

	for (SizeType i = 0; i < stored.size(); i++) tSiswa += {
		std::to_string(i + 1),
		stored[i].DateTime,
		std::to_string(stored[i].NIS),
		stored[i].Nama,
		stored[i].Alamat,
		stored[i].TempatLahir,
		stored[i].TanggalLahir,
		stored[i].Agama,
		stored[i].JenisKelamin,
		stored[i].Jurusan
	};

	Tools::Print(2, 2, "=====================================");
	Tools::Print(2, 3, "   DATA SISWA YANG TELAH TERDAFTAR");
	Tools::Print(2, 4, "=====================================");

	Console::SetCursorPosition(0, 6);
	tSiswa.Print();
}
void Cari() {
	ConsoleMenu mCari{
		{
			"Nomor Induk/Nama Siswa :",
			"[Cari]",
			"[Kembali]"
		},
		CursorColor,
		{2, 6}
	};
	ConsoleMenu::Selection sCari;
	String cari;

	do { // Main loop
		Tools::Print(2, 2, "=====================");
		Tools::Print(2, 3, "   CARI DATA SISWA");
		Tools::Print(2, 4, "=====================");

		sCari = mCari.Print();

		switch (sCari.First) {
		case 0: // Nomor Induk/Nama Siswa
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), cari);
			cari = Console::GetLine();
			break;
		}

		if (sCari.Second == "[Cari]") { // Cari
			// Checking user input
			if (cari.empty())
				Tools::PrintMessage(2, 10, Message::Warning, "Silakan masukkan Nama atau NIK Siswa.");

			// Final stage
			else {
				Vector<DataSiswa> stored;
				ConsoleTable tSiswa{
					"No",
					"Tanggal Registrasi",
					"Nomor Induk Siswa",
					"Nama siswa",
					"Alamat",
					"Tempat lahir",
					"Tanggal lahir",
					"Agama",
					"Jenis kelamin",
					"Jurusan",
				};

				if (Tools::IsNumber(cari)) {
					if (!std::regex_search(cari, NisEx))
						Tools::PrintMessage(2, 10, Message::Information, "Nomor Induk Siswa salah.");
					else {
						long lCari = std::stol(cari);
						Vector<DataSiswa> temp = $Siswa.Read();

						for (DataSiswa index : temp) {
							if (index.NIS == lCari) {
								stored.push_back(index);
								break;
							}
						}

						if (!stored.empty()) tSiswa += {
							"1",
							stored[0].DateTime,
							std::to_string(stored[0].NIS),
							stored[0].Nama,
							stored[0].Alamat,
							stored[0].TempatLahir,
							stored[0].TanggalLahir,
							stored[0].Agama,
							stored[0].JenisKelamin,
							stored[0].Jurusan
						};

						Console::SetCursorPosition(0, 10); tSiswa.Print();
					}
				}
				else {
					String lowCari = cari;
					String lowSiswa;
					Vector<DataSiswa> temp = $Siswa.Read();

					std::transform(lowCari.begin(), lowCari.end(), lowCari.begin(), ::tolower);

					for (DataSiswa index : temp) {
						lowSiswa = index.Nama;
						std::transform(lowSiswa.begin(), lowSiswa.end(), lowSiswa.begin(), ::tolower);

						if (lowSiswa.find(lowCari) != -1)
							stored.push_back(index);
					}

					for (SizeType i = 0, no = 1; i < stored.size(); i++, no++) tSiswa += {
						std::to_string(no),
						stored[i].DateTime,
						std::to_string(stored[i].NIS),
						stored[i].Nama,
						stored[i].Alamat,
						stored[i].TempatLahir,
						stored[i].TanggalLahir,
						stored[i].Agama,
						stored[i].JenisKelamin,
						stored[i].Jurusan
					};

					Console::SetCursorPosition(0, 10); tSiswa.Print();
				}
			}
		}
	} while (sCari.Second != "[Kembali]");
}
void Home(String username) {
	ConsoleMenu mHome{
		{
			"1. Registrasi siswa",
			"2. Lihat semua data",
			"3. Cari data siswa",
			"4. Update data siswa",
			"5. Hapus data siswa",
			"[Sign Out]"
		},
		CursorColor,
		{2, 7}
	};
	ConsoleMenu::Selection sHome;

	do { // Main loop
		Tools::Print(2, 2, "===================");
		Tools::Print(2, 3, "   SMK LOCALHOST");
		Tools::Print(2, 4, "===================");
		Tools::Print(2, 5, "Anda masuk sebagai: ", username);

		sHome = mHome.Print();

		switch (sHome.First) {
		case 0: // Tambah data siswa
			Console::Clear();
			Tambah();
			Console::Clear();
			break;
		case 1: // Lihat semua data
			Console::Clear();
			Lihat();
			Console::Clear();
			break;
		case 2:
			Console::Clear();
			Cari();
			Console::Clear();
			break;
		}
	} while (sHome.Second != "[Sign Out]"); // Sign Out
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
			"Program mengalami error:(\n\n",
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
			"Program mengalami error:(\n\n",
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
			"Program mengalami error :(\n\n",
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
			"Program mengalami error :(\n\n",
			"Description: ", e.what()
		);
		Console::GetKey();

		return EXIT_FAILURE;
	}
}