#include <fstream>
#include <sstream>

#include "Version.h"

namespace Lib830 {

	bool VersionInfo::operator==(const VersionInfo& other) {
		return major == other.major &&
			minor == other.minor &&
			patch == other.patch;
	}

	bool VersionInfo::operator<(const VersionInfo &other) {
		if (major != other.major)
			return major < other.major;

		else if (minor != other.minor)
			return minor < other.minor;

		else if (patch != other.patch)
			return patch < other.patch;

		else
			return false;
	}

	std::string GetWPILibVersionString() {
		char line[256];
		std::ifstream file("/tmp/frc_versions/FRC_Lib_Version.ini");
		file.getline(line, sizeof(line)/sizeof(char));
		return line;
	}

	VersionInfo GetWPILibVersion(std::string raw) {
		int year = 0, release = 0;
		std::stringstream ss;
		ss << raw;
		ss >> year;
		while (!isdigit(ss.peek()) && ss)
			ss.get();
		ss >> release;
		return VersionInfo(year, release);
	}

}
