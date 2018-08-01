#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>


namespace Xmaxplatform {
	namespace Chain {
		
		template <typename ObjType>
		class V8BindObject {
		public:
			//V8 bind
			
			static ObjType* Unwrap(v8::Handle<v8::Object> jsObj) {
				v8::Handle<External> ptr = Handle<External>::Cast(jsObj->GetInternalField(0));
				void* testvalue = ptr->Value();
				return static_cast<ObjType*>(testvalue);
			}

			static void Wrap(v8::Isolate* isolate, ObjType* cpp_object, v8::Handle<v8::Object>& object) {
				using namespace v8;
				
				Local<External> ee = External::New(isolate, cpp_object);
				object->SetInternalField(0, ee);
				Persistent<External> ret = Persistent<External>(isolate, ee);

				ret.SetWeak(cpp_object, ObjType::WeakExternalReferenceCallback, WeakCallbackType::kParameter);
			}

			static constexpr const char* TypeName() {
				static_assert(false, "Need to implemented in the derived class.");
			}

			static ObjType* NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args) { static_assert(false, "Need to implemented in the derived class."); }
			static void RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global) { static_assert(false, "Need to implemented in the derived class."); }
			static void ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args) { static_assert(false, "Need to implemented in the derived class."); }
			static void WeakExternalReferenceCallback(const v8::WeakCallbackInfo<ObjType>& data) { static_assert(false, "Need to implemented in the derived class."); }
		};
	} // namespace Chain
}