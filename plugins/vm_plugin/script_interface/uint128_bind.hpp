//uint ui28 decl
#pragma  once
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
		class V8ui128
		{
		public:
			V8ui128(uint64_t)
			{

			}
			V8ui128()
			{

			}
			V8ui128(int l1, int l2, int l3, int l4)
			{
				data[0] = l1;
				data[1] = l2;
				data[2] = l3;
				data[3] = l4;
			}
			int data[4];
		};

		V8ui128* NewV8ui128Function(const FunctionCallbackInfo<v8::Value>& args);

		void V8ui128WeakExternalReferenceCallback(Persistent<v8::Value>, void* parameter);

		void V8ui128FunctionInvocationCallback(const FunctionCallbackInfo<v8::Value>& args);

		void SetupV8ui128ObjectToJs(Isolate* isolate, Handle<ObjectTemplate> global);

		void GetBit(int i, const FunctionCallbackInfo<v8::Value>& args);

	}
}