//send for vm_plugin 
#include "uint128_bind.hpp"
namespace xmax {

	namespace scriptv8 {


		V8u128* V8u128::NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args)
		{
			return nullptr;
		}

		void V8u128::RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global)
		{

		}

		void V8u128::ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args)
		{

		}

		void V8u128::WeakExternalReferenceCallback(const v8::WeakCallbackInfo<V8u128>& data)
		{

		}

	}
}