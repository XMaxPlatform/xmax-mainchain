#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>
#include "V8BindObject.hpp"
//#include "libraries/protobuf/src/google/protobuf/stubs/int128.h"



namespace xmax {
	namespace scriptv8 {

		//Internal struct
		class V8u128 : public V8BindObject<V8u128>
		{
		public:
			//V8u128(uint128 value) :value_(value) {}

			//V8u128() :value_(0) {}

			//operator Basetypes::uint128() { return value_; }


			//V8 bind
			static constexpr inline const char* TypeName() {
				return "V8u128";
			}

			static V8u128* NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global);
			static void ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void WeakExternalReferenceCallback(const v8::WeakCallbackInfo<V8u128>& data);

		private:
			//:uint128 value_;
		};


		


	}
}