#pragma once
#include "Simple.h"

using namespace Simple;
using System::BinaryFile;

struct DataSiswa {
	char DateTime[32];
	char Nama[64];
	char Alamat[64];
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
};