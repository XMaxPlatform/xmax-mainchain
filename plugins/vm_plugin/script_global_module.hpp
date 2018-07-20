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
		public:
			void SetupV8Env();
			void DiscardV8Env();

			Isolate * GetIsolate();
			v8::Local<v8::ObjectTemplate>* GetGlobalObjTemplate();
		private:
			Isolate * isolate_;

			v8::Local<v8::ObjectTemplate>* global_obj_template_;

		};

		inline v8::Isolate * ScriptGlobalMoudle::GetIsolate()
		{
			return isolate_;
		}

		inline v8::Local<v8::ObjectTemplate>* ScriptGlobalMoudle::GetGlobalObjTemplate()
		{
			return global_obj_template_;
		}


	}
}