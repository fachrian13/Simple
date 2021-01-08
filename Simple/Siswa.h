#pragma once
#include "Simple.h"

using namespace Simple;
using System::BinaryFile;

struct DataSiswa {
	char DateTime[32];
	long NIS;
	char Nama[64];
	char Alamat[128];
	char TempatLahir[32];
	char TanggalLahir[32];
	char Agama[16];
	char JenisKelamin[16];
	char Jurusan[64];
};

class Siswa : public BinaryFile<DataSiswa> {
public:
	Siswa()
		:BinaryFile("Siswa.bin") {}

	long GetNis() {
		Vector<DataSiswa> stored = Read();

		if (stored.empty())
			return 0;
		else
			return stored.back().NIS;
	}
};