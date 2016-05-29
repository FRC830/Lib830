#pragma once

#include <string>

// undefine glibc macros
#ifdef major
	#undef major
#endif
#ifdef minor
	#undef minor
#endif

namespace Lib830 {

	struct VersionInfo {
		int major;
		int minor;
		int patch;
		VersionInfo(int major, int minor = 0, int patch = 0):
			major(major),
			minor(minor),
			patch(patch)
		{}
		bool operator==(const VersionInfo& other);
		bool operator<(const VersionInfo& other);
		inline bool operator<=(const VersionInfo& other) {
			return operator==(other) || operator<(other);
		}
		inline bool operator!=(const VersionInfo &other) {
			return !operator==(other);
		}
		inline bool operator>(const VersionInfo &other) {
			return !operator<=(other);
		}
		inline bool operator>=(const VersionInfo &other) {
			return !operator<(other);
		}
	};

	std::string GetWPILibVersionString();
	VersionInfo GetWPILibVersion(std::string raw);
	inline VersionInfo GetWPILibVersion() {
		return GetWPILibVersion(GetWPILibVersionString());
	}
}
