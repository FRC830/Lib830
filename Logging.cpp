/*
 * Logging.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: Ratpack
 */
#include <set>
#include <string>
#include <WPILib.h>
#include "Logging.h"
void Logging::warn_change_condition(void *token, bool cond, const char *msg) {
	static std::set<void*> conditions;
	std::string out;
	if (cond && !conditions.count(token)) {
		out += "warning on: ";
		conditions.insert(token);
	}
	else if (!cond && conditions.count(token)) {
		out += "warning off: ";
		conditions.erase(token);
	}
	else
		return;
	out += msg;
	DriverStation::ReportWarning(msg);
}


