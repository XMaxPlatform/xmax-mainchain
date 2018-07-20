//@file
//@copyright defined in xmax/LICENSE
#include "script_module.hpp"
#include "script_global_module.hpp"
#include <libplatform/libplatform.h>

namespace xmax {

	namespace scriptv8 {
		
		ScriptMoudle::ScriptMoudle()
			:current_code_()
			,main_foo_()
			,instruction_count_(0)
			,last_intruction_()
		{

		}

		ScriptMoudle::~ScriptMoudle()
		{
			{
			//	Discard();
			}
		}
		v8::Object* CallBackCheck(int args_length, v8::Object** args_object, v8::Isolate* isolate) {
		
			void* arg1 = *(reinterpret_cast<v8::Object**>(reinterpret_cast<intptr_t>(args_object) - 1 * sizeof(int)));


			int value = (int)arg1;
			//ScriptMoudle::GetInstance().StoreInstrunction(value);
			return args_object[0];
		}

// 		void ScriptMoudle::Init()
// 		{
// 			V8_AddIntrinsicFoo("CallBackCheck", (void*)CallBackCheck, 2, 1);
// 
// 			V8::InitializeICUDefaultLocation("");
// 			V8::InitializeExternalStartupData("");
// 			Platform* platform = platform::CreateDefaultPlatform();
// 			V8::InitializePlatform(platform);
// 			V8::Initialize();
// 
// 		}

		v8::Handle<v8::Value> ScriptMoudle::DoworkInContext(const v8::HandleScope& scope, const v8::Local<ObjectTemplate>& global, const v8::Local<Context>& context, const v8::Context::Scope& ctxScope)
		{
			V8_ParseWithPlugin();
			CompileJsCode(ScriptGlobalMoudle::GetInstance().GetIsolate(), context, current_code_.c_str() );
			CleanInstrunction();
			v8::Handle<v8::Value> result =  CallJsFoo(ScriptGlobalMoudle::GetInstance().GetIsolate(), context, main_foo_.c_str() , 0, NULL);
			V8_ParseWithOutPlugin();
			int test = result->Int32Value();
			return result;
		}

		void ScriptMoudle::AstBlockCallbackInsert()
		{

		}

// 		v8::Handle<v8::Value> ScriptMoudle::Call(const std::string& code, const std::string& fooName)
// 		{
// 			current_code_ = code;
// 			main_foo_ = fooName;
// 			namespace  ph = std::placeholders;
// 			//return EnterJsContext(isolate_, std::bind(&ScriptMoudle::DoworkInContext, this, ph::_1, ph::_2, ph::_3, ph::_4));
// 			return Undefined(ScriptGlobalMoudle::GetInstance().GetIsolate());
// 		}

		void ScriptMoudle::LoadScript(const char* code)
		{
			current_context_ = CreateJsContext(ScriptGlobalMoudle::GetInstance().GetIsolate(), *ScriptGlobalMoudle::GetInstance().GetGlobalObjTemplate());
			EnterJsContext(ScriptGlobalMoudle::GetInstance().GetIsolate(), current_context_);
			CompileJsCode(ScriptGlobalMoudle::GetInstance().GetIsolate(), current_context_.Get(ScriptGlobalMoudle::GetInstance().GetIsolate()), code);
		}

		void ScriptMoudle::RunFoo(const char* foo)
		{
			
			Local<Context> context = current_context_.Get(ScriptGlobalMoudle::GetInstance().GetIsolate());
			Handle<v8::Value> params[2];
		
			CallJsFoo(ScriptGlobalMoudle::GetInstance().GetIsolate(), context, foo, 0, NULL);
		}

// 		void ScriptMoudle::Discard()
// 		{
// 						
// 			v8::V8::Dispose();
// 			v8::V8::ShutdownPlatform();
// 		}

		void ScriptMoudle::StoreInstrunction(int ins)
		{
			instruction_count_++;
			last_intruction_.push_back(ins);
		}

		void ScriptMoudle::CleanInstrunction()
		{
			instruction_count_ = 0;
			last_intruction_.clear();
		}



	}
}


