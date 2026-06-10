module;

#include <sol/sol.hpp>

export module engine:sol.exports;

namespace sol
{
	using
		::sol::state,
		::sol::table,
		::sol::function,
		::sol::protected_function,
		::sol::lib,
		::sol::stack::push,
		::sol::stack::get,
		::sol::stack::pop
		;
}
