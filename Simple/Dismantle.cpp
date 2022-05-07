#include "Simple.h"

using Simple::System::Exception;
using Simple::System::Console;
using Simple::System::BinaryFile;
using Simple::System::Coordinate;
using Simple::System::Color;
using Simple::System::ConsoleColor;
using Simple::Utility::Tools;
using Simple::Utility::Table;
using Simple::Utility::ConsoleMenu::Input;
using Simple::Utility::ConsoleMenu::Selection;
using Simple::Utility::ConsoleMenu::Question;

constexpr auto BlackOnYellow = ConsoleColor{ Color::Yellow, Color::Black };

struct DataUser {
	unsigned long long NIK;
	char Password[16];
	bool Suspend;

	DataUser() : NIK(0), Password(""), Suspend(false) {}
	DataUser(std::string nik, std::string password) : Suspend(false) {
		this->NIK = std::stoull(nik);
		strcpy_s(this->Password, password.c_str());
	}
};

struct DataDismantling {
	unsigned long long NoInternet;
	char NamaPelanggan[64];
	char STO[8];
	bool Status;
	char SNONT[64];
	char SNSTB[64];
	char Keterangan[64];

	DataDismantling() : NoInternet(0), NamaPelanggan(""), STO(""), Status(false), SNONT(""), SNSTB(""), Keterangan("") {}
	DataDismantling(std::string noInternet, std::string namaPelanggan, std::string sto, bool status, std::string snOnt, std::string snStb, std::string keterangan) {
		this->NoInternet = std::stoull(noInternet);
		strcpy_s(this->NamaPelanggan, namaPelanggan.c_str());
		strcpy_s(this->STO, sto.c_str());
		this->Status = status;
		strcpy_s(this->SNONT, snOnt.c_str());
		strcpy_s(this->SNSTB, snStb.c_str());
		strcpy_s(this->Keterangan, keterangan.c_str());
	}
};

BinaryFile<DataUser> User("user.bin");
BinaryFile<DataDismantling> Dismantling("dismantling.bin");

void SignUp();
void SignIn();
void Dashboard(DataUser);
void TambahData();
void LihatData();
void CariData();
void UpdateData();
void HapusData();

int main() {
	try {
		Console::CursorBlinking = false;
		Console::CursorVisible = false;
		Console::DisableMaximizeButton();
		Console::DisableResizeWindow();
		Console::SetWindowSize(170, 40);
		Console::EnableVirtual();

		if (User.IsEmpty()) {
			Console::Clear();
			SignUp();
		}

		if (!User.IsEmpty()) {
			Console::Clear();
			SignIn();
		}
	}
	catch (Exception& e) {
		Console::Clear();
		Console::Write(e);
		Console::ReadKey();

		return 1;
	}
	catch (std::exception& e) {
		Console::Clear();
		Console::Write(e.what());
		Console::ReadKey();

		return 1;
	}

	return 0;
}

void SignUp() {
	std::string nik;
	std::string password;
	std::string password1;
	Input iSignUp(
		{
			Input::Text("NIK                 :", &nik),
			Input::Text("Password            :", &password, "", true),
			Input::Text("Konfirmasi password :", &password1, "", true),
			Input::Text("[Register]"),
			Input::Text("[Kembali]")
		},
		{ 3, 7 }
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "========================",
			Coordinate{ 3, 4 }, "   REGISTER USER BARU",
			Coordinate{ 3, 5 }, "========================"
		);
		iSignUp.Run();

		switch (iSignUp.Selected.Index) {
		case 0:
			Console::CursorVisible = true;
			nik = Tools::ReadNumber(15);
			Console::CursorVisible = false;
			break;
		case 1:
			Console::CursorVisible = true;
			password = Tools::ReadPassword(15);
			Console::CursorVisible = false;
			break;
		case 2:
			Console::CursorVisible = true;
			password1 = Tools::ReadPassword(15);
			Console::CursorVisible = false;
			break;
		}

		if (Tools::Compare(iSignUp.Selected.Value, "[Register]")) {
			if (nik.empty() || password.empty() || password1.empty())
				Tools::WriteMessage({ 3, 13 }, "WARNING", "Silakan lengkapi seluruh data.", BlackOnYellow);
			else if (password != password1)
				Tools::WriteMessage({ 3, 13 }, "INFORMATION", "Password tidak sama.");
			else {
				DataUser userData = DataUser(nik, password);

				User.Write(userData);
				Tools::WriteMessage({ 3, 13 }, "INFORMATION", "Pendaftaran user berhasil.");
				iSignUp.Stop();
			}
		}
		else if (Tools::Compare(iSignUp.Selected.Value, "[Kembali]"))
			iSignUp.Stop();
	} while (iSignUp.Running);
}
void SignIn() {
	std::string nik;
	std::string password;
	Input iSignIn(
		{
			Input::Text("NIK      :", &nik),
			Input::Text("Password :", &password, "", true),
			Input::Text("[Masuk]"),
			Input::Text("[Kembali]")
		},
		{ 3, 7 }
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "=====================",
			Coordinate{ 3, 4 }, "   MASUK AKUN ANDA",
			Coordinate{ 3, 5 }, "====================="
		);
		iSignIn.Run();

		switch (iSignIn.Selected.Index) {
		case 0:
			Console::CursorVisible = true;
			nik = Tools::ReadNumber(15);
			Console::CursorVisible = false;
			break;
		case 1:
			Console::CursorVisible = true;
			password = Tools::ReadPassword(15);
			Console::CursorVisible = false;
			break;
		}

		if (Tools::Compare(iSignIn.Selected.Value, "[Masuk]")) {
			if (nik.empty() || password.empty())
				Tools::WriteMessage({ 3, 12 }, "WARNING", "Silakan lengkapi seluruh data.", BlackOnYellow);
			else {
				DataUser dataUser;
				unsigned long long ullNik = std::stoull(nik);
				const auto fileData = User.Read();

				for (const auto& data : fileData)
					if (ullNik == data.NIK)
						dataUser = data;

				if (dataUser.NIK == 0)
					Tools::WriteMessage({ 3, 12 }, "INFORMATION", "User dengan nik " + nik + " tidak terdaftar.");
				else if (dataUser.Password != password)
					Tools::WriteMessage({ 3, 12 }, "INFORMATION", "Password salah.");
				else {
					Console::Clear();
					Dashboard(dataUser);
					Console::Clear();
					
					nik.clear();
					password.clear();
				}
			}
		}
		else if (Tools::Compare(iSignIn.Selected.Value, "[Kembali]"))
			iSignIn.Stop();
	} while (iSignIn.Running);
}
void Dashboard(DataUser dataUser) {
	Selection sDashboard(
		{
			"1. Tambah data dismantling",
			"2. Lihat seluruh data",
			"3. Cari data dismantling",
			"4. Update data",
			"5. Hapus data dismantling",
			"[Keluar]"
		},
		{ 3, 8 },
		false
	);
	Question qExit(
		"Apakah anda yakin?",
		{
			"[Ya]",
			"[Tidak]"
		},
		{ 3, 15 },
		false
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "=========================",
			Coordinate{ 3, 4 }, "   REKAPAN DISMANTLING",
			Coordinate{ 3, 5 }, "=========================",
			Coordinate{ 3, 6 }, "Login sebagai: ", dataUser.NIK
		);
		sDashboard.Run();

		switch (sDashboard.Selected.Index) {
		case 0:
			Console::Clear();
			TambahData();
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
		case 3:
			Console::Clear();
			UpdateData();
			Console::Clear();
			break;
		case 4:
			Console::Clear();
			HapusData();
			Console::Clear();
			break;
		}

		if (Tools::Compare(sDashboard.Selected.Value, "[Keluar]"))
		{
			qExit.Run();

			switch (qExit.Selected.Index) {
			case 0:
				sDashboard.Stop();
				break;
			default:
				qExit.Clear();
			}
		}
	} while (sDashboard.Running);
}
void TambahData() {
	std::string noInternet;
	std::string namaPelanggan;
	std::string sto;
	std::string status;
	std::string snOnt;
	std::string snStb;
	std::string keterangan;
	Input iTambahData(
		{
			Input::Text("Nomor internet :", &noInternet),
			Input::Text("Nama pelanggan :", &namaPelanggan),
			Input::Text("STO            :", &sto, "Silakan pilih"),
			Input::Text("Status         :", &status, "Silakan pilih"),
			Input::Text("SN ONT         :", &snOnt),
			Input::Text("SN STB         :", &snStb),
			Input::Text("Keterangan     :", &keterangan),
			Input::Text("[Tambah]"),
			Input::Text("[Kembali]")
		},
		{ 3, 7 }
	);
	Selection sSto(
		{
			"BDK",
			"CCD",
			"CJA",
			"DGO",
			"GGK",
			"HGM",
			"LBG",
			"SMD",
			"TAS",
			"TLE",
			"TRG",
			"UBR"
		},
		{ 20, 9 }
	);
	Selection sStatus(
		{
			"Kendala",
			"Berhasil diambil"
		},
		{ 20, 10 },
		true
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "=============================",
			Coordinate{ 3, 4 }, "   TAMBAH DATA DISMANTLING",
			Coordinate{ 3, 5 }, "============================="
		);
		iTambahData.Run();

		switch (iTambahData.Selected.Index) {
		case 0:
			Console::CursorVisible = true;
			noInternet = Tools::ReadNumber(15);
			Console::CursorVisible = false;
			break;
		case 1:
			Console::CursorVisible = true;
			namaPelanggan = Tools::ReadLine(63);
			Console::CursorVisible = false;
			break;
		case 2:
			sSto.Run();
			sSto.Clear();
			sto = sSto.Selected.Value;
			break;
		case 3:
			sStatus.Run();
			sStatus.Clear();
			status = sStatus.Selected.Value;
			break;
		case 4:
			Console::CursorVisible = true;
			snOnt = Console::ReadLine();
			Console::CursorVisible = false;
			break;
		case 5:
			Console::CursorVisible = true;
			snStb = Console::ReadLine();
			Console::CursorVisible = false;
			break;
		case 6:
			Console::CursorVisible = true;
			keterangan = Tools::ReadLine(63);
			Console::CursorVisible = false;
			break;
		}

		if (Tools::Compare(iTambahData.Selected.Value, "[Tambah]")) {
			if (noInternet.empty() || namaPelanggan.empty() || sto.empty() || status.empty())
				Tools::WriteMessage({ 3, 17 }, "WARNING", "Silakan isi No Internet, Nama, STO, dan Status.", BlackOnYellow);
			else if ((sStatus.Selected.Index == 1) && (snOnt.empty() && snStb.empty()))
				Tools::WriteMessage({ 3, 17 }, "WARNING", "Status ONT \'Berhasil diambil\', silakan isi SN perangkat.");
			else {
				DataDismantling ddsm(noInternet, namaPelanggan, sto, sStatus.Selected.Index, snOnt, snStb, keterangan);

				Dismantling.Write(ddsm);
				Tools::WriteMessage({ 3, 17 }, "INFORMATION", "Data berhasil ditambahkan.");

				noInternet.clear();
				namaPelanggan.clear();
				sto.clear();
				status.clear();
				snOnt.clear();
				snStb.clear();
				keterangan.clear();
			}
		}
		else if (Tools::Compare(iTambahData.Selected.Value, "[Kembali]"))
			iTambahData.Stop();
	} while (iTambahData.Running);
}
void LihatData() {
	size_t no = 0;
	const auto dataDismantling = Dismantling.Read();
	Table tAll = Table({ "No", "No Internet", "Nama Pelanggan", "STO", "Status", "SN ONT", "SN STB", "Keterangan" });

	for (const auto& i : dataDismantling)
		tAll.Add({ std::to_string(++no), std::to_string(i.NoInternet), i.NamaPelanggan, i.STO, i.Status ? "Berhasil diambil" : "Kendala", i.SNONT, i.SNSTB, i.Keterangan });

	Console::Write(
		Coordinate{ 3, 3 }, "======================",
		Coordinate{ 3, 4 }, "   DATA DISMANTLING",
		Coordinate{ 3, 5 }, "======================",
		Coordinate{ 0, 7 }, tAll
	);

	Console::ReadKey();
}
void CariData() {
	std::string cari;
	Input iCariData(
		{
			Input::Text("Masukkan data :", &cari, "No Internet atau Nama pelanggan"),
			Input::Text("[Cari]"),
			Input::Text("[Kembali]")
		},
		{ 3, 7 }
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "===========================",
			Coordinate{ 3, 4 }, "   CARI DATA DISMANTLING",
			Coordinate{ 3, 5 }, "==========================="
		);
		iCariData.Run();

		switch (iCariData.Selected.Index) {
		case 0:
			Console::CursorVisible = true;
			cari = Tools::ReadLine(63);
			Console::CursorVisible = false;
			break;
		}

		if (Tools::Compare(iCariData.Selected.Value, "[Cari]")) {
			if (cari.empty())
				Tools::WriteMessage({ 3, 11 }, "WARNING", "Silakan isi data.", BlackOnYellow);
			else {
				Console::Write(Coordinate{ 0, 11 }, Simple::Utility::ConsoleMenu::EraseDisplayAfterCursor);

				if (Tools::IsNumber(cari)) {
					unsigned long long ullCari = std::stoull(cari);
					DataDismantling dataDismantling;
					const auto fileData = Dismantling.Read();

					for (const auto& i : fileData) {
						if (i.NoInternet == ullCari) {
							dataDismantling = i;
							break;
						}
					}

					if (dataDismantling.NoInternet == 0)
						Tools::WriteMessage({ 3, 11 }, "INFORMATION", "Data dengan no internet " + cari + " tidak ditemukan.");
					else {
						Table tData = Table({ "No Internet", "Nama Pelanggan", "STO", "Status", "SN ONT", "SN STB", "Keterangan" });

						tData.Add({ std::to_string(dataDismantling.NoInternet), dataDismantling.NamaPelanggan, dataDismantling.STO, dataDismantling.Status ? "Berhasil diambil" : "Kendala", dataDismantling.SNONT, dataDismantling.SNSTB , dataDismantling.Keterangan });

						Console::Write(Coordinate{ 0,11 }, tData);
					}
				}
				else {
					std::vector<DataDismantling> dataDismantling;
					const auto fileData = Dismantling.Read();

					for (const auto& i : fileData) {
						std::string current = i.NamaPelanggan;

						if (current.find(cari) != std::string::npos)
							dataDismantling.push_back(i);
					}

					if (dataDismantling.empty())
						Tools::WriteMessage({ 3, 11 }, "INFORMATION", "Data tidak ditemukan.");
					else {
						Table tData = Table({ "No Internet", "Nama Pelanggan", "STO", "Status", "SN ONT", "SN STB", "Keterangan" });

						for(const auto& i:dataDismantling)
							tData.Add({ std::to_string(i.NoInternet), i.NamaPelanggan, i.STO, i.Status ? "Berhasil diambil" : "Kendala", i.SNONT, i.SNSTB , i.Keterangan });

						Console::Write(Coordinate{ 0,11 }, tData);
					}
				}
			}
		}
		else if (Tools::Compare(iCariData.Selected.Value, "[Kembali]"))
			iCariData.Stop();
	} while (iCariData.Running);
}
void UpdateData() {
	std::string cari;
	Input iCariData(
		{
			Input::Text("Masukkan data :", &cari, "No Internet pelanggan"),
			Input::Text("[Cari]"),
			Input::Text("[Kembali]")
		},
		{ 3, 7 }
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "=============================",
			Coordinate{ 3, 4 }, "   UPDATE DATA DISMANTLING",
			Coordinate{ 3, 5 }, "============================="
		);
		iCariData.Run();

		switch (iCariData.Selected.Index) {
		case 0:
			Console::CursorVisible = true;
			cari = Tools::ReadNumber(15);
			Console::CursorVisible = false;
			break;
		}

		if (Tools::Compare(iCariData.Selected.Value, "[Cari]")) {
			if (cari.empty())
				Tools::WriteMessage({ 3, 11 }, "WARNING", "Silakan isi data.", BlackOnYellow);
			else {
				size_t index = 0;
				unsigned long long ullCari = std::stoull(cari);
				DataDismantling dataDismantling;
				const auto fileData = Dismantling.Read();

				for (const auto& i : fileData) {
					if (i.NoInternet == ullCari) {
						dataDismantling = i, index++;
						break;
					}
				}

				if (dataDismantling.NoInternet == 0)
					Tools::WriteMessage({ 3, 11 }, "INFORMATION", "Data dengan no internet " + cari + " tidak ditemukan.");
				else {
					std::string noInternet = std::to_string(dataDismantling.NoInternet);
					std::string namaPelanggan = dataDismantling.NamaPelanggan;
					std::string sto = dataDismantling.STO;
					std::string status = dataDismantling.Status ? "Berhasil diambil" : "Kendala";
					std::string snOnt = dataDismantling.SNONT;
					std::string snStb = dataDismantling.SNSTB;
					std::string keterangan = dataDismantling.Keterangan;
					Input iUpdateData(
						{
							Input::Text("Nomor internet :", &noInternet),
							Input::Text("Nama pelanggan :", &namaPelanggan),
							Input::Text("STO            :", &sto, "Silakan pilih"),
							Input::Text("Status         :", &status, "Silakan pilih"),
							Input::Text("SN ONT         :", &snOnt),
							Input::Text("SN STB         :", &snStb),
							Input::Text("Keterangan     :", &keterangan),
							Input::Text("[Update]"),
							Input::Text("[Kembali]")
						},
						{ 3, 15 }
					);
					Selection sSto(
						{
							"BDK",
							"CCD",
							"CJA",
							"DGO",
							"GGK",
							"HGM",
							"LBG",
							"SMD",
							"TAS",
							"TLE",
							"TRG",
							"UBR"
						},
						{ 20, 17 }
					);
					Selection sStatus(
						{
							"Kendala",
							"Berhasil diambil"
						},
						{ 20, 18 },
						true
					);

					do {
						Console::Write(
							Coordinate{ 3, 11 }, "========================",
							Coordinate{ 3, 12 }, "   MASUKKAN DATA BARU",
							Coordinate{ 3, 13 }, "========================"
						);
						iUpdateData.Run();

						switch (iUpdateData.Selected.Index) {
						case 0:
							Console::CursorVisible = true;
							noInternet = Tools::ReadNumber(15);
							Console::CursorVisible = false;
							break;
						case 1:
							Console::CursorVisible = true;
							namaPelanggan = Tools::ReadLine(63);
							Console::CursorVisible = false;
							break;
						case 2:
							sSto.Run();
							sSto.Clear();
							sto = sSto.Selected.Value;
							break;
						case 3:
							sStatus.Run();
							sStatus.Clear();
							status = sStatus.Selected.Value;
							break;
						case 4:
							Console::CursorVisible = true;
							snOnt = Console::ReadLine();
							Console::CursorVisible = false;
							break;
						case 5:
							Console::CursorVisible = true;
							snStb = Console::ReadLine();
							Console::CursorVisible = false;
							break;
						case 6:
							Console::CursorVisible = true;
							keterangan = Tools::ReadLine(63);
							Console::CursorVisible = false;
							break;
						}

						if (Tools::Compare(iUpdateData.Selected.Value, "[Update]")) {
							if (noInternet.empty() || namaPelanggan.empty() || sto.empty() || status.empty())
								Tools::WriteMessage({ 3, 25 }, "WARNING", "Silakan isi No Internet, Nama, STO, dan Status.", BlackOnYellow);
							else if ((sStatus.Selected.Index == 1) && (snOnt.empty() && snStb.empty()))
								Tools::WriteMessage({ 3, 25 }, "WARNING", "Status ONT \'Berhasil diambil\', silakan isi SN perangkat.");
							else {
								dataDismantling = DataDismantling(noInternet, namaPelanggan, sto, sStatus.Selected.Index, snOnt, snStb, keterangan);

								Dismantling.Update(index, dataDismantling);
								Tools::WriteMessage({ 3, 25 }, "INFORMATION", "Data berhasil diupdate.");

								iUpdateData.Stop();
							}
						}
						else if (Tools::Compare(iUpdateData.Selected.Value, "[Kembali]"))
							iUpdateData.Stop();
					} while (iUpdateData.Running);

					Console::Write(Coordinate{ 0, 11 }, Simple::Utility::ConsoleMenu::EraseDisplayAfterCursor);
				}
			}
		}
		else if (Tools::Compare(iCariData.Selected.Value, "[Kembali]"))
			iCariData.Stop();
	} while (iCariData.Running);
}
void HapusData() {
	std::string cari;
	Input iCariData(
		{
			Input::Text("Masukkan data :", &cari, "No Internet pelanggan"),
			Input::Text("[Cari]"),
			Input::Text("[Kembali]")
		},
		{ 3, 7 }
	);

	do {
		Console::Write(
			Coordinate{ 3, 3 }, "============================",
			Coordinate{ 3, 4 }, "   HAPUS DATA DISMANTLING",
			Coordinate{ 3, 5 }, "============================"
		);
		iCariData.Run();

		switch (iCariData.Selected.Index) {
		case 0:
			Console::CursorVisible = true;
			cari = Tools::ReadNumber(15);
			Console::CursorVisible = false;
			break;
		}

		if (Tools::Compare(iCariData.Selected.Value, "[Cari]")) {
			if (cari.empty())
				Tools::WriteMessage({ 3, 11 }, "WARNING", "Silakan isi data.", BlackOnYellow);
			else {
				size_t index = 0;
				unsigned long long ullCari = std::stoull(cari);
				DataDismantling dataDismantling;
				const auto fileData = Dismantling.Read();
				Question qHapus("Apakah anda yakin menghapus data ini?", { "[Tidak]", "[Ya]" }, { 3, 17 }, false);

				for (const auto& i : fileData) {
					if (i.NoInternet == ullCari) {
						dataDismantling = i, index++;
						break;
					}
				}

				if (dataDismantling.NoInternet == 0)
					Tools::WriteMessage({ 3, 11 }, "INFORMATION", "Data dengan no internet " + cari + " tidak ditemukan.");
				else {
					Table tData = Table({ "No Internet", "Nama Pelanggan", "STO", "Status", "SN ONT", "SN STB", "Keterangan" });

					tData.Add({ std::to_string(dataDismantling.NoInternet), dataDismantling.NamaPelanggan, dataDismantling.STO, dataDismantling.Status ? "Berhasil diambil" : "Kendala", dataDismantling.SNONT, dataDismantling.SNSTB , dataDismantling.Keterangan });

					Console::Write(Coordinate{ 0,11 }, tData);

					qHapus.Run();
					switch (qHapus.Selected.Index) {
					case 1:
						Dismantling.Delete(index);
						Tools::WriteMessage({ 3, 21 }, "INFORMATION", "Data berhasil dihapus.");
						break;
					}
				}

				Console::Write(Coordinate{ 0, 11 }, Simple::Utility::ConsoleMenu::EraseDisplayAfterCursor);
			}
		}
		else if (Tools::Compare(iCariData.Selected.Value, "[Kembali]"))
			iCariData.Stop();
	} while (iCariData.Running);
}