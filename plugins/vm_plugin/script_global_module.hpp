/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include "pro/utils/singleton.hpp "
#include <v8.h>
#include <string>
#include <list>
#include "script_util.hpp"

namespace xmax {

	namespace scriptv8 {

		using namespace v8;

		class ScriptGlobalMoudle :public pro::Singleton<ScriptGlobalMoudle> {
			DECLARE_USE_SINGLETON(ScriptGlobalMoudle)
		}
	}
}