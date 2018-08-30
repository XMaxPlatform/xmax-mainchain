#pragma once

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/list.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <iostream>


#define SERIAL_EACH_MEMBER(r, data, elem) ar(CEREAL_NVP(elem));
#define REFLECT_MEMBER_SERIALIZATION(MEMBERS) \
	friend class cereal::access; \
	template<class Archive> \
	void serialize(Archive & ar) { \
		BOOST_PP_SEQ_FOR_EACH(SERIAL_EACH_MEMBER, _, MEMBERS) \
	}							 

template<typename T>
void Serialize(const T& t)
{
	std::stringbuf sb;
	std::ostream os(&sb);
	cereal::BinaryOutputArchive archive(os);
	archive(CEREAL_NVP(t));
}

template<typename T>
void Deserialize(const T& t)
{
	std::stringbuf sb;
	std::istream os(&sb);
	cereal::BinaryInputArchive archive(os);
	archive(CEREAL_NVP(t));
}