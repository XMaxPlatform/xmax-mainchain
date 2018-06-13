#pragma once

#include <v8.h>
#include <string>

namespace Xmax {

	namespace ScriptV8 {

		using namespace v8;

		class ScriptMoudle {

		public:
			static ScriptMoudle& Instance()
			{
				static ScriptMoudle _instance;
				return _instance;
			}
			
			virtual ~ScriptMoudle();

			void Init();
			
			void Call(const std::string& code, const std::string& fooName);

			void Discard();

		private:
			ScriptMoudle();
			void DoworkInContext(const HandleScope& scope, const Local<ObjectTemplate>& global, const Local<Context>& context, const Context::Scope& ctxScope);

			std::string				 m_CurrentCode;
			std::string				 m_MainFooName;

			Isolate*                 m_pIsolate;

		};
	}
}
