#include "Simple.h"

using Simple::System::Console;

struct DateTime {
	int Second;
	int Minute;
	int Hour;
	int DayOfMonth;
	int Month;
	int Year;
	int DayOfWeek;
	int DayOfYear;
	int Daylight;

	static DateTime Now()
	{
		time_t now = time(0);
		tm time;
		DateTime res;

		localtime_s(&time, &now);

		res.Second = time.tm_sec;
		res.Minute = time.tm_min;
		res.Hour = time.tm_hour;
		res.DayOfWeek = time.tm_wday;
		res.DayOfMonth = time.tm_mday;
		res.DayOfYear = time.tm_yday;
		res.Month = time.tm_mon + 1;
		res.Year = time.tm_year + 1900;
		res.Daylight = time.tm_isdst;
		
		return res;
	}

	std::string ToString() const {
		return std::to_string(this->DayOfMonth) + "/" + std::to_string(this->Month) + "/" + std::to_string(this->Year) + " " + ((this->Hour < 10) ? "0" + std::to_string(this->Hour) + ":" : std::to_string(this->Hour) + ":") + ((this->Minute < 10) ? "0" + std::to_string(this->Minute) + ":" : std::to_string(this->Minute) + ":") + ((this->Second < 10) ? "0" + std::to_string(this->Second) : std::to_string(this->Second));
	}

	friend std::ostream& operator <<(std::ostream& out, const DateTime& dateTime) {
		return out << dateTime.ToString();
	}
};

int main() {
	Console::Write(DateTime::Now().ToString());
}