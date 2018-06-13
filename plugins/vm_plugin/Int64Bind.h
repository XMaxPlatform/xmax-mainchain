#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>
using namespace v8;

namespace Xmaxplatform {
	namespace Chain {
		class V8i64
		{
		public:
			V8i64(uint64_t)
			{

			}
			V8i64()
			{

			}
			V8i64(int l, int h)
			{
				data[0] = l;
				data[1] = h;
			}
			int data[2];
		};

		V8i64* NewV8i64Function(const FunctionCallbackInfo<v8::Value>& args);

		void V8i64WeakExternalReferenceCallback(Persistent<v8::Value>, void* parameter);

		void V8i64FunctionInvocationCallback(const FunctionCallbackInfo<v8::Value>& args);

		void SetupV8i64ObjectToJs(Isolate* isolate,Handle<ObjectTemplate> global);

		void GetH(const FunctionCallbackInfo<v8::Value>& args);

		void GetL(const FunctionCallbackInfo<v8::Value>& args);


	}
}