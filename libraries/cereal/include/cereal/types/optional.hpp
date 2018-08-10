#pragma once

#include "cereal/cereal.hpp"
#include <optional>

namespace cereal
{
	template<class Archive, class T> inline
	void
	CEREAL_SAVE_FUNCTION_NAME(Archive & ar, std::optional<T> const & opt)
	{
		bool hasValue = opt.has_value();
		ar(hasValue);
		if (hasValue)
		{
			ar(*opt);
		}

	}

	template<class Archive, class T> inline
	void
	CEREAL_LOAD_FUNCTION_NAME(Archive & ar, std::optional<T> & opt)
	{
		bool hasValue;
		ar(hasValue);
		if (hasValue)
		{
			T t;
			ar(t);
			opt = t;
		}
	}
}