#pragma once

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define SERIAL_EACH_MEMBER(r, data, elem) ar(CEREAL_NVP(elem));
#define REFLECT_MEMBER_SERIALIZATION(MEMBERS) \
	friend class cereal::access; \
	template<class Archive> \
	void serialize(Archive & ar) { \
		BOOST_PP_SEQ_FOR_EACH(SERIAL_EACH_MEMBER, _, MEMBERS) \
	}							 
