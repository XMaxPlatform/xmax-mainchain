#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>
using namespace v8;

namespace xmax {
	namespace scriptv8 {
		/**
		* implementation of int64
		*/
		class V8ui64
		{
		public:
			V8ui64(uint64_t)
			{

			}
			V8ui64()
			{

			}
			V8ui64(int l, int h)
			{
				data[0] = l;
				data[1] = h;
			}
			int data[2];
		};

		V8ui64* V8ui64NewFunction(const FunctionCallbackInfo<v8::Value>& args);

		void V8ui64WeakExternalReferenceCallback(Persistent<v8::Value>, void* parameter);

		void V8ui64FunctionInvocationCallback(const FunctionCallbackInfo<v8::Value>& args);

		void V8ui64SetupObjectToJs(Isolate* isolate, Handle<ObjectTemplate> global);

		void V8ui64GetH(const FunctionCallbackInfo<v8::Value>& args);

		void V8ui64GetL(const FunctionCallbackInfo<v8::Value>& args);


	}
}