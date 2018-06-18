//@file
//@copyright defined in xmax/LICENSE
#pragma once
#include <functional>
#include <map>
#include <v8.h>



namespace xmax {

	namespace scriptv8{

		using namespace v8;

		typedef std::function<void(const HandleScope&, const Local<ObjectTemplate>&, const Local<Context>&, const Context::Scope&)> DoWorkInJsCtx;

		typedef std::map<std::string, Local<FunctionTemplate>> JsFooBindMap;

		void EnterJsContext(Isolate* pIsolate, DoWorkInJsCtx dowork);

		void BindJsFoos(Isolate* pIsolate, const Local<ObjectTemplate>& fooGlobal, const JsFooBindMap& foosToBind);

		void CompileJsCode(Isolate* pIsolate, const Local<Context>& context,const char* jsCode);

		Handle<v8::Value> CallJsFoo(Isolate* pIsolate, const Local<Context>& context, const char* fooname, unsigned int argc, Handle<v8::Value>* params);

		Handle<v8::Value> I64Cpp2JS(Isolate* isolate, const Local<Context>& context, int64_t v);

		namespace FooBind
		{
			void exportFoo(const FunctionCallbackInfo<v8::Value>& args);

			inline JsFooBindMap GetBindFoos(Isolate* pIsolate)
			{
#define bindfoo(fooname)\
ret.insert(std::pair<std::string, Local<FunctionTemplate>>(#fooname, v8::FunctionTemplate::New(pIsolate, fooname)))

				JsFooBindMap ret;
				bindfoo(exportFoo);
				return std::move(ret);
			}
		}
	}
}