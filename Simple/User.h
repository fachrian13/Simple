#pragma once
#include "Simple.h"

using namespace Simple;
using System::BinaryFile;

struct DataUser {
	bool Blocked;
	char DateTime[32];
	char Username[16];
	char Password[16];
};

class User : public BinaryFile<DataUser> {
public:
	User() :
		BinaryFile("User.bin") {}
	bool CheckUsername(Cstring username) {
		Vector<DataUser> stored = Read();

		for (DataUser index : stored)
			if (strcmp(username, index.Username) == 0)
				return true;

		return false;
	}
};