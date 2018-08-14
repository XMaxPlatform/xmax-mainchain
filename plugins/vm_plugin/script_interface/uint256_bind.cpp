#include "uint256_bind.hpp"
using namespace v8;
namespace xmax {

	namespace scriptv8 {

		V8u256* V8u256::NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args)
		{
			return nullptr;
		}

		void V8u256::RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global)
		{

		}

		void V8u256::ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args)
		{
			if (!args.IsConstructCall())
				return args.GetReturnValue().Set(Undefined(args.GetIsolate()));

		
		}

		void V8u256::WeakExternalReferenceCallback(const v8::WeakCallbackInfo<V8u256>& data)
		{
			if (V8u256* cpp_object = data.GetParameter())
			{
				delete cpp_object;
			}
		}

	}
}