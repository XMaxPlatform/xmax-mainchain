#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>
#include "V8BindObject.hpp"
#include "pro/types/generictypes.hpp"

namespace xmax {
	namespace scriptv8 {
		class V8u256 : public V8BindObject<V8u256>
		{
		public:
			V8u256(pro::uint128 value) :value_(value) {}

			V8u256() :value_(0) {}

			operator pro::uint256() { return value_; }

			static constexpr inline const char* TypeName() {
				return "V8u256";
			}

			static V8u256* NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global);
			static void ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void WeakExternalReferenceCallback(const v8::WeakCallbackInfo<V8u256>& data);

		private:
			pro::uint256 value_;
		};


	}
}