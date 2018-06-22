//@file
//@copyright defined in xmax/LICENSE
#include "script_module.hpp"
#include "script_util.hpp"
#include <libplatform/libplatform.h>

namespace xmax {

	namespace scriptv8 {
		
		ScriptMoudle::ScriptMoudle()
			:isolate_(nullptr)
			,current_code_()
			,main_foo_()
		{

		}

		ScriptMoudle::~ScriptMoudle()
		{
			if (isolate_!=nullptr)
			{
				Discard();
			}
		}
		v8::Object* test(v8::internal::Arguments& args,v8::Isolate* iso)
		{
			return nullptr;
		}
		void ScriptMoudle::Init()
		{
			V8_AddIntrinsicFoo("CallBackTest", (void*)test, 0, 1);

			V8::InitializeICUDefaultLocation("");
			V8::InitializeExternalStartupData("");
			Platform* platform = platform::CreateDefaultPlatform();
			V8::InitializePlatform(platform);
			V8::Initialize();

			Isolate::CreateParams create_params;
			create_params.array_buffer_allocator =v8::ArrayBuffer::Allocator::NewDefaultAllocator();
			isolate_ = Isolate::New(create_params);
		}

		void ScriptMoudle::DoworkInContext(const v8::HandleScope& scope, const v8::Local<ObjectTemplate>& global, const v8::Local<Context>& context, const v8::Context::Scope& ctxScope)
		{
			CompileJsCode(isolate_, context, current_code_.c_str() );
			CallJsFoo(isolate_, context, main_foo_.c_str() , 0, NULL);
		}

		void ScriptMoudle::AstBlockCallbackInsert()
		{

		}

		void ScriptMoudle::Call(const std::string& code, const std::string& fooName)
		{
			current_code_ = code;
			main_foo_ = fooName;
			namespace  ph = std::placeholders;
			EnterJsContext(isolate_, std::bind(&ScriptMoudle::DoworkInContext, this, ph::_1, ph::_2, ph::_3, ph::_4));
		}

		void ScriptMoudle::Discard()
		{
			isolate_->Dispose();
			isolate_ = nullptr;
			v8::V8::Dispose();
			v8::V8::ShutdownPlatform();
		}

	}
}


