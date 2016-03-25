/*
 * Logging.h
 *
 *  Created on: Mar 12, 2016
 *      Author: Ratpack
 */

#pragma once
#include <sstream>

namespace Logging {
	inline std::string to_string(const std::ostream &out) {
		std::stringstream ss;
		ss << out.rdbuf();
		return ss.str();
	}
	void warn_cond_change(void *token, bool cond, std::string msg, bool silent = false);
	inline void warn_cond_on(void *token, bool cond, std::string msg) {
		warn_cond_change(token, cond, msg, !cond);
	}
	inline void warn_cond_off(void *token, bool cond, std::string msg) {
		warn_cond_change(token, cond, msg, cond);
	}
}

// wraps "contents" in a new scope and defines a useless unique variable, "ulogtoken"
// This means that "&ulogtoken" is unique and can be used to identify invocations of various macros
#define ULOG_TOKEN_GUARD(contents) do { static struct{} ulogtoken; contents; } while(0);

// wraps "contents" in a new scope and defines "ulogmsg" as whatever a stringstream would contain if fed "msg"
// e.g. ULOG_GEN_MSG("a" << 1 << "b") would set ulogmsg to "a1b"
// this is much easier to work with than macros that take variable numbers of arguments
#define ULOG_GEN_MSG(msg, contents) do { std::stringstream ulogss; ulogss << msg; std::string ulogmsg = ulogss.str(); contents; } while(0);

// helpers to define macros that call out to Logging::* functions
#define ULOG_CALL(func, cond, msg) ULOG_GEN_MSG(msg, ULOG_TOKEN_GUARD(Logging::func((void*)&ulogtoken, (cond), ulogmsg)))

/*** Warning macros ***/
#define WARN_COND_CHANGE(cond, msg) ULOG_CALL(warn_cond_change, cond, msg)
#define WARN_COND_ON(cond, msg)     ULOG_CALL(warn_cond_on, cond, msg)
#define WARN_COND_OFF(cond, msg)    ULOG_CALL(warn_cond_off, cond, msg)
