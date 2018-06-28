/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include "pro/utils/singleton.hpp "
#include <v8.h>
#include <string>

namespace xmax {

	namespace scriptv8 {

		using namespace v8;

		class ScriptMoudle:public pro::Singleton<ScriptMoudle> {
			DECLARE_USE_SINGLETON(ScriptMoudle)
		public:
			void Init();
			
			void Call(const std::string& code, const std::string& fooName);

			void Discard();

			void InstrunctionIncrease();

		private:
			void DoworkInContext(const HandleScope& scope, const Local<ObjectTemplate>& global, const Local<Context>& context, const Context::Scope& ctxScope);

			void AstBlockCallbackInsert();

			int instruction_count_;

			std::string				 current_code_;
			std::string				 main_foo_;

			Isolate*                 isolate_;

		};
	}
}
