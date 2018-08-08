//send for vm_plugin 
#include "uint128_bind.hpp"

using namespace v8;
namespace xmax {

	namespace scriptv8 {


		V8u128* V8u128::NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args)
		{
			return nullptr;
		}

		void V8u128::RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global)
		{
			Handle<FunctionTemplate> func_template = FunctionTemplate::New(isolate, &ConstructV8Object);

			func_template->SetClassName(String::NewFromUtf8(
				isolate,
				TypeName(),
				NewStringType::kNormal).ToLocalChecked());
			func_template->InstanceTemplate()->SetInternalFieldCount(1);
			global->Set(String::NewFromUtf8(
				isolate,
				TypeName(),
				NewStringType::kNormal).ToLocalChecked(), func_template);


			Handle<ObjectTemplate> proto_template = func_template->PrototypeTemplate();
		}

		void V8u128::ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args)
		{
			if (!args.IsConstructCall())
				return args.GetReturnValue().Set(Undefined(args.GetIsolate()));

			V8u128* cpp_object = nullptr;
			if (args.Length() == 1)
			{
				Local<Object> self = args.Holder();
				Local<External> wrap = Local<External>::Cast(args[0]);
				cpp_object = (V8u128*)wrap->Value();
			}
			else
			{
				NewV8CppObj(args);
			}

			if (!cpp_object)
				return;

			Handle<Object> object = args.This();
			Wrap(args.GetIsolate(), cpp_object, object);
			args.GetReturnValue().Set(object);
		}

		void V8u128::WeakExternalReferenceCallback(const v8::WeakCallbackInfo<V8u128>& data)
		{

		}

	}
}