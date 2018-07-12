#include "script_global_module.hpp"
#include <libplatform/libplatform.h>
namespace xmax {

	namespace scriptv8 {

		ScriptGlobalMoudle::ScriptGlobalMoudle()
		{

		}

		ScriptGlobalMoudle::~ScriptGlobalMoudle()
		{

		}

		void ScriptGlobalMoudle::SetupV8Env()
		{
			//V8_AddIntrinsicFoo("CallBackCheck", (void*)CallBackCheck, 2, 1);

			V8::InitializeICUDefaultLocation("");
			V8::InitializeExternalStartupData("");
			Platform* platform = platform::CreateDefaultPlatform();
			V8::InitializePlatform(platform);
			V8::Initialize();
		}

		void ScriptGlobalMoudle::DiscardV8Env()
		{

		}

	}
}
