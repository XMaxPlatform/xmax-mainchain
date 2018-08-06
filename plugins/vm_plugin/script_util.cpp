//@file
//@copyright defined in xmax/LICENSE
#include "script_util.hpp"
#include <iostream>

namespace xmax {
	namespace scriptv8 {
		v8::Handle<v8::Value> EnterJsContext(Isolate* isolate, DoWorkInJsCtx dowork)
		{
			HandleScope current_handle_scope(isolate);

			v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

			BindJsFoos(isolate, global, FooBind::GetBindFoos(isolate));

			Local<Context> context = Context::New(isolate, NULL, global);
			Context::Scope context_scope(context);

			return dowork(current_handle_scope, global, context, context_scope);
		}


		void BindJsFoos(Isolate* isolate, const Local<ObjectTemplate>& foo_global, const std::map<std::string, Local<FunctionTemplate>>& foos_to_bind)
		{
			for (const std::pair<std::string, Local<FunctionTemplate> >& foo_bind : foos_to_bind)
			{
				foo_global->Set(
					v8::String::NewFromUtf8(isolate, foo_bind.first.c_str(), v8::NewStringType::kNormal).ToLocalChecked(),
					foo_bind.second);
			}
		}


		PersistentCpyableContext CreateJsContext(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate>& global)
		{
			Local<Context> context = Context::New(isolate, NULL, global);
			PersistentCpyableContext ret(isolate, context);
			return ret;
		}

		void EnterJsContext(v8::Isolate* pIsolate, v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>>& context)
		{
			Local<Context> localContext = context.Get(pIsolate);
			localContext->Enter();
		}

		void ExitJsContext(v8::Isolate* isolate, v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>>& context)
		{
			Local<Context> local_context = context.Get(isolate);
			local_context->Exit();
		}

		void CompileJsCode(Isolate* isolate, const Local<Context>& context,const char* js_code)
		{
			//V8_ParseWithPlugin();

			Local<String> source =
				String::NewFromUtf8(isolate, js_code,
					NewStringType::kNormal).ToLocalChecked();
			MaybeLocal<Script> temp = Script::Compile(context, source);

			if (temp.IsEmpty())
			{
				std::cerr << "js compile failed" << std::endl;
			}
			Local<Script> script = temp.ToLocalChecked();
			if (script.IsEmpty()) {
				std::cerr << "js compile failed" << std::endl;
			}
			TryCatch trycatch(isolate);
			Local<Value> v = script->Run();
			if (v.IsEmpty()) {
				Local<Value> exception = trycatch.Exception();
				String::Utf8Value exception_str(exception);
				printf("Exception: %s\n", *exception_str);
				// ...
			}
		}

		v8::Handle<v8::Value> CallJsFoo(Isolate* pIsolate, const Local<Context>& context, const char* fooname, unsigned int argc, Handle<v8::Value>* params)
		{
			Handle<String> js_func_name = String::NewFromUtf8(pIsolate, fooname, NewStringType::kNormal).ToLocalChecked();
			Handle<v8::Value>  js_func_val = context->Global()->Get(js_func_name);
			if (!js_func_val->IsFunction())
			{
				std::cerr << "Can't find js funcion" << std::endl;
				return  Undefined(pIsolate);
			}
			else
			{
				Handle<Function> js_func = Handle<Function>::Cast(js_func_val);
				Handle<v8::Value> hResult = js_func->Call(context->Global(), argc, params);
				return hResult;
			}
		}

		Handle<v8::Value> I64Cpp2JS(Isolate* isolate, const Local<Context>& context, int64_t v)
		{
			Handle<String> js_data = String::NewFromUtf8(isolate, "V8i64", NewStringType::kNormal).ToLocalChecked();
			Handle<v8::Value> js_data_value = context->Global()->Get(js_data);

			bool bIsObject = js_data_value->IsObject();
			if (bIsObject)
			{
				Handle<Object> js_data_object = Handle<Object>::Cast(js_data_value);

				int* pvarr = (int*)&v;

				Handle<v8::Value>  argcodev[2];
				argcodev[0] = Int32::New(isolate, pvarr[1]);
				argcodev[1] = Int32::New(isolate, pvarr[0]);

				Handle<v8::Value> codeObj = js_data_object->CallAsConstructor(2, argcodev);
				return codeObj;
			}
			return Undefined(isolate);
		}

		namespace FooBind
		{
			void FooBind::exportFoo(const FunctionCallbackInfo<v8::Value>& args)
			{
				bool first = true;
				for (int i = 0; i < args.Length(); i++) {
					v8::HandleScope handle_scope(args.GetIsolate());

					Handle<v8::Value> js_data_value = args[i];

					bool bIsObject = js_data_value->IsObject();
					if (bIsObject)
					{

					}
				}
			}
		}
	}
}
