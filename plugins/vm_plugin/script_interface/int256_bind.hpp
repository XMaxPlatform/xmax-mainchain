#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>
#include "V8BindObject.hpp"
#include "pro/types/generictypes.hpp"

namespace xmax {
	namespace scriptv8 {
		class V8i256 : public V8BindObject<V8i256>
		{
		public:
			V8i256() {}
			~V8i256() {}
		private:
		};


	}
}