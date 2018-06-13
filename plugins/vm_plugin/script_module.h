#pragma once

#include <v8.h>
#include <string>

namespace Xmax {

	namespace ScriptV8 {

		using namespace v8;
		class ScriptMoudle {
		public:
			ScriptMoudle();
			virtual ~ScriptMoudle();

			void Init();
			void LoadCode(const std::string& code);
			void Call(const std::string& fooName);
			void UnloadCode();
			void Discard();

		private:

			void ScriptMoudle::vm_init(const HandleScope& scope, const Local<ObjectTemplate>& global, const Local<Context>& context, const Context::Scope& ctxScope);

			std::string m_CurrentCode;

			Local<Script>			 m_Script;
			Isolate*                 m_pIsolate;
			Local<Context>           m_Context;

			Isolate::Scope*          m_pIsolateScope;
			HandleScope*             m_pHandleScope;
			Context::Scope*			 m_pContextScope;
		};
	}
}
