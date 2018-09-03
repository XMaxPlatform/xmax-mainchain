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




// macros for DB_FIELD 
#define _REFLECT_BODY_FIELD_(_t, _v, _def) _t _v = _def;
#define _REFLECT_SZ_FIELD_(_t, _v, _def) ar(CEREAL_NVP((_v)));

// macros for DB_SFIELD 
#define _REFLECT_BODY_SFIELD_(_t, _v) _t _v;
#define _REFLECT_SZ_SFIELD_(_t, _v) ar(CEREAL_NVP((_v)));

#define _REFLECT_MACRO_CAT_(r, data, elem) BOOST_PP_CAT(data, elem)


#define _REFLECT_DEF_(_args) BOOST_PP_SEQ_FOR_EACH(_REFLECT_MACRO_CAT_, _REFLECT_BODY, _args)

#define _REFLECT_SERIALIZATION_(_args)\
	friend class cereal::access; \
	template<class Archive> \
	void serialize(Archive & ar) { \
		BOOST_PP_SEQ_FOR_EACH(_REFLECT_MACRO_CAT_, _REFLECT_SZ, _args) {}\
	}	
// \

#define _REFLECT_BODY_(_args)  \
public:\
_REFLECT_DEF_(_args)\
_REFLECT_SERIALIZATION_(_args)\
private:


#define RF_BODY(_args) _REFLECT_BODY_(_args)

// define field of simple value. e.g. int.
#define RF_FIELD(_type, _name, _default_value) _FIELD_(_type, _name, _default_value) 
// define field of simple value width out default value. e.g. int.
#define RF_SFIELD(_type, _name) _SFIELD_(_type, _name)