//@file
//@copyright defined in xmax/LICENSE
#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>
#include "V8BindObject.hpp"
#include "pro/types/generictypes.hpp"



namespace xmax {
	namespace scriptv8 {

		//Internal struct
		class V8i128 : public V8BindObject<V8i128>
		{
		public:
			V8i128(pro::uint128 value) :value_(value) {}

			V8i128() :value_(0) {}

			operator pro::uint128() { return value_; }


			//V8 bind
			static constexpr inline const char* TypeName() {
				return "V8i128";
			}

			static V8i128* NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global);
			static void ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void WeakExternalReferenceCallback(const v8::WeakCallbackInfo<V8i128>& data);

		private:
			pro::uint128 value_;
		};


		


	}
}