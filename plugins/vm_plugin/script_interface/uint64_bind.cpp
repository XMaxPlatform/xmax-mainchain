#include "uint64_bind.hpp"
namespace xmax {

	namespace scriptv8 {

		void V8ui64GetH(const FunctionCallbackInfo<v8::Value>& args)
		{
			Local<Object> self = args.Holder();

			Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
			void* ptr = wrap->Value();

			args.GetReturnValue().Set(Int32::New(args.GetIsolate(), static_cast<V8ui64*>(ptr)->data[0]));
		}

		void V8ui64GetL(const const FunctionCallbackInfo<v8::Value>& args)
		{
			Local<Object> self = args.Holder();

			Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
			void* ptr = wrap->Value();

			args.GetReturnValue().Set(Int32::New(args.GetIsolate(), static_cast<V8ui64*>(ptr)->data[1]));
		}


		V8ui64* V8ui64NewFunction(const FunctionCallbackInfo<v8::Value>&  args)
		{
			if (args.Length() == 2)
			{
				Local<Value> v1 = args[0];
				Local<Value> v2 = args[1];
				return new V8ui64(v1->Int32Value(), v2->Int32Value());
			}
			else
			{
				return new V8ui64();
			}
		}

		void V8ui64WeakExternalReferenceCallback(const WeakCallbackInfo<V8ui64>& data)
		{
			if (V8ui64* cpp_object = data.GetParameter())
			{
				delete cpp_object;
			}
		}

		void V8ui64FunctionInvocationCallback(const FunctionCallbackInfo<v8::Value>& args)
		{
			if (!args.IsConstructCall())
				return args.GetReturnValue().Set(Undefined(args.GetIsolate()));

			V8ui64* cpp_object = V8ui64NewFunction(args);
			if (!cpp_object)
				return;

			Handle<Object> object = args.This();
			Local<External>  ee = External::New(args.GetIsolate(), cpp_object);
			object->SetInternalField(0, ee);
			Persistent<External> ret = Persistent<External>(args.GetIsolate(), ee);

			ret.SetWeak(cpp_object, V8ui64WeakExternalReferenceCallback, WeakCallbackType::kParameter);

			args.GetReturnValue().Set(Undefined(args.GetIsolate()));
		}

		void V8ui64SetupObjectToJs(Isolate* isolate, Handle<ObjectTemplate> global)
		{

			Handle<FunctionTemplate> ui64_templ = FunctionTemplate::New(isolate, &V8ui64FunctionInvocationCallback);

			ui64_templ->SetClassName(String::NewFromUtf8(
				isolate,
				"V8ui64",
				NewStringType::kNormal).ToLocalChecked());
			ui64_templ->InstanceTemplate()->SetInternalFieldCount(1);

			Handle<ObjectTemplate> ui64_proto = ui64_templ->PrototypeTemplate();

			ui64_proto->Set(String::NewFromUtf8(
				isolate,
				"GetH",
				NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, V8ui64GetH));
			ui64_proto->Set(String::NewFromUtf8(
				isolate,
				"GetL",
				NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, V8ui64GetL));

			global->Set(String::NewFromUtf8(
				isolate,
				"V8ui64",
				NewStringType::kNormal).ToLocalChecked(), ui64_templ);
		}
	}
}