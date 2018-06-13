#include "script_module.h"
#include "script_util.h"
#include <libplatform/libplatform.h>

namespace Xmax {

	namespace ScriptV8 {
		
		ScriptMoudle::ScriptMoudle():m_pIsolate(nullptr)
		{

		}

		ScriptMoudle::~ScriptMoudle()
		{

		}

		void ScriptMoudle::Init()
		{
			V8::InitializeICUDefaultLocation("");
			V8::InitializeExternalStartupData("");
			Platform* platform = platform::CreateDefaultPlatform();
			V8::InitializePlatform(platform);
			V8::Initialize();

			Isolate::CreateParams create_params;
			create_params.array_buffer_allocator =v8::ArrayBuffer::Allocator::NewDefaultAllocator();
			m_pIsolate = Isolate::New(create_params);
		}


		void ScriptMoudle::DoworkInContext(const v8::HandleScope& scope, const v8::Local<ObjectTemplate>& global, const v8::Local<Context>& context, const v8::Context::Scope& ctxScope)
		{
			CompileJsCode(m_pIsolate, context, m_CurrentCode.c_str() );
			CallJsFoo(m_pIsolate, context, m_MainFooName.c_str() , 0, NULL);
		}


		void ScriptMoudle::Call(const std::string& code,const std::string& fooName)
		{
			m_CurrentCode = code;
			m_MainFooName = fooName;
			namespace  ph = std::placeholders;
			EnterJsContext(m_pIsolate, std::bind(&ScriptMoudle::DoworkInContext, this, ph::_1, ph::_2, ph::_3, ph::_4));
		}

		void ScriptMoudle::Discard()
		{
			m_pIsolate->Dispose();
			v8::V8::Dispose();
			v8::V8::ShutdownPlatform();
		}

	}
}


