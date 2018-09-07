//@file
//@copyright defined in xmax/LICENSE
#include "int64_bind.hpp"
namespace xmax {

	namespace scriptv8 {

		void GetH(const FunctionCallbackInfo<v8::Value>& args)
		{
			Local<Object> self = args.Holder();

			Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
			void* ptr = wrap->Value();

			args.GetReturnValue().Set(Int32::New(args.GetIsolate(), static_cast<V8i64*>(ptr)->data[0]));
		}

		void GetL(const const FunctionCallbackInfo<v8::Value>& args)
		{
			Local<Object> self = args.Holder();

			Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
			void* ptr = wrap->Value();

			args.GetReturnValue().Set(Int32::New(args.GetIsolate(), static_cast<V8i64*>(ptr)->data[1]));
		}


		V8i64* NewV8i64Function(const FunctionCallbackInfo<v8::Value>&  args)
		{
			if (args.Length() == 2)
			{
				Local<Value> v1 = args[0];
				Local<Value> v2 = args[1];
				return new V8i64(v1->Int32Value(), v2->Int32Value());
			}
			else
			{
				return new V8i64();
			}	
		}

		void V8i64WeakExternalReferenceCallback(const WeakCallbackInfo<V8i64>& data)
		{
			if (V8i64* cpp_object = data.GetParameter())
			{
				delete cpp_object;
			}	
		}

		void V8i64FunctionInvocationCallback(const FunctionCallbackInfo<v8::Value>& args)
		{
			if (!args.IsConstructCall())
				return args.GetReturnValue().Set(Undefined(args.GetIsolate()));

			V8i64* cpp_object = NewV8i64Function(args);
			if (!cpp_object)
				return;
				
			Handle<Object> object = args.This();
			Local<External>  ee = External::New(args.GetIsolate(), cpp_object);
			object->SetInternalField(0, ee);
			Persistent<External> ret = Persistent<External>(args.GetIsolate(), ee);
			
			ret.SetWeak(cpp_object, V8i64WeakExternalReferenceCallback, WeakCallbackType::kParameter);

			args.GetReturnValue().Set(Undefined(args.GetIsolate()));
		}

		void SetupV8i64ObjectToJs(Isolate* isolate, Handle<ObjectTemplate> global)
		{
			
			Handle<FunctionTemplate> i64_templ = FunctionTemplate::New(isolate,&V8i64FunctionInvocationCallback);

			i64_templ->SetClassName(String::NewFromUtf8(
				isolate,
				"V8i64",
				NewStringType::kNormal).ToLocalChecked());
			i64_templ->InstanceTemplate()->SetInternalFieldCount(1);

			Handle<ObjectTemplate> i64_proto = i64_templ->PrototypeTemplate();

			i64_proto->Set(String::NewFromUtf8(
				isolate,
				"GetH",
				NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate,GetH));
			i64_proto->Set(String::NewFromUtf8(
				isolate,
				"GetL",
				NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate,GetL));

			global->Set(String::NewFromUtf8(
				isolate,
				"V8i64",
				NewStringType::kNormal).ToLocalChecked(), i64_templ);
		}
	}
}