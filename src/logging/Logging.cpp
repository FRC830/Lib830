#include <set>
#include <string>
#include <WPILib.h>

#include "Logging.h"

namespace Lib830 {
namespace Logging {

static std::set<void*> condition_tokens;
void Logging::warn_cond_change(void *token, bool cond, std::string msg, bool silent) {
	std::string out;
	if (cond && !condition_tokens.count(token)) {
		out += "[warn] ";
		condition_tokens.insert(token);
	}
	else if (!cond && condition_tokens.count(token)) {
		out += "[end warn] ";
		condition_tokens.erase(token);
	}
	else
		return;
	out += msg;
	if (!silent)
		DriverStation::ReportWarning(out);
}

}}
