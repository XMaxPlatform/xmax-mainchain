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


			Isolate::CreateParams create_params;
			create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
			isolate_ = Isolate::New(create_params);
		}

		void ScriptGlobalMoudle::DiscardV8Env()
		{
			isolate_->Dispose();
			v8::V8::Dispose();
			v8::V8::ShutdownPlatform();
		}

		void ScriptGlobalMoudle::V8SetupGlobalObjTemplate(v8::Local<v8::ObjectTemplate>* pGlobalTemp)
		{
			global_obj_template_ = pGlobalTemp;
			//if (m_pBind != nullptr)
			{
				BindJsFoos(isolate_, *global_obj_template_, FooBind::GetBindFoos(isolate_));
			//	m_pBind->Setup(m_pIsolate, *m_pGlobalObjectTemplate);
			}
		}

		void ScriptGlobalMoudle::SetCurrentModule(ScriptMoudle* pModule)
		{
			script_module_ = pModule;
		}

	}
}
