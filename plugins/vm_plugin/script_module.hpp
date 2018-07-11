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

		class ScriptMoudle{
		public:
			ScriptMoudle();
			~ScriptMoudle();
			void Init();
			
			v8::Handle<v8::Value> Call(const std::string& code, const std::string& fooName);

			void Discard();

			void StoreInstrunction(int ins);

			void CleanInstrunction();

			Isolate* GetIsolate();
		private:
			v8::Handle<v8::Value> DoworkInContext(const HandleScope& scope, const Local<ObjectTemplate>& global, const Local<Context>& context, const Context::Scope& ctxScope);

			void AstBlockCallbackInsert();

			int instruction_count_;
			std::list<int> last_intruction_;
			
			std::string				 current_code_;
			std::string				 main_foo_;

			Isolate*                 isolate_;

			PersistentCpyableContext current_context_;

		};
		inline v8::Isolate* ScriptMoudle::GetIsolate()
		{
			return isolate_;
		}
	}
}
