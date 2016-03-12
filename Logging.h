/*
 * Logging.h
 *
 *  Created on: Mar 12, 2016
 *      Author: Ratpack
 */

#pragma once

namespace Logging {
	void warn_change_condition(void *token, bool cond, const char *msg);
}

#define WARN_CHANGE(cond, msg) { static bool token; Logging::warn_change_condition((void*)&token, (cond), msg);}
