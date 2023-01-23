#pragma once

//------------------------------------------------------------------------------
// Some simple logging helpers.
//
// Uses the excellent https://fmt.dev/ library to allow for simple,
// python like formatting.
//
//
// This code isn't intented for your review. Of course, if you feel like it, dive
// right in.
//------------------------------------------------------------------------------

#include <fmt/format.h>
// #include <vivid/vivid.h>

namespace Log
{
	// namespace ansi = vivid::ansi;
	template <typename S1, typename S3, typename... Args>
	void _log(const S1 &prefix, const S3 &format_str, Args &&...args)
	{
		fmt::print(
			"{}[{}]{}: {}\n",
			// c,
			prefix,
			// ansi::reset,
			fmt::format(format_str, std::forward<Args>(args)...));
	}

	template <typename S, typename... Args>
	void debug(const S &format_str, Args &&...args)
	{
		_log("DEBUG", format_str, args...);
	}

	template <typename S, typename... Args>
	void info(const S &format_str, Args &&...args)
	{
		_log("INFO", format_str, args...);
	}

	template <typename S, typename... Args>
	void warning(const S &format_str, Args &&...args)
	{
		_log("WARN", format_str, args...);
	}
	template <typename S, typename... Args>
	void warn(const S &format_str, Args &&...args)
	{
		_log("WARN", format_str, args...);
	}

	template <typename S, typename... Args>
	void error(const S &format_str, Args &&...args)
	{
		_log("ERROR", format_str, args...);
	}

}
