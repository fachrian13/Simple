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
	enum class Condition {
		Success				= 0,
		UsernameNotFound	= 1,
		WrongPassword		= 2,
		UserIsBlocked		= 3
	};

	User() :
		BinaryFile("User.bin") {}
	bool CheckUsername(Cstring username) {
		Vector<DataUser> stored = Read();

		for (DataUser index : stored)
			if (strcmp(username, index.Username) == 0)
				return true;

		return false;
	}
	Condition SignIn(Cstring username, Cstring password) {
		Vector<DataUser> stored = Read();

		for (DataUser index : stored)
			if (strcmp(username, index.Username) == 0) {
				if (index.Blocked)
					return Condition::UserIsBlocked;
				else
					if (strcmp(password, index.Password) == 0)
						return Condition::Success;
					else
						return Condition::WrongPassword;
		}
		return Condition::UsernameNotFound;
	}
};