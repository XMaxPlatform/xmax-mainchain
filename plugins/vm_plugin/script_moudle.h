#pragma once

#include <v8.h>
#include <string>

namespace Xmax {

	namespace ScriptV8 {

		class ScriptMoudle {
		public:
			ScriptMoudle();
			virtual ~ScriptMoudle();

			void Init();
			void LoadCode(const std::string& code);
			void Call(const std::string& fooName);
			void UnloadCode();
			void Discard();

		private:
			std::string m_CurrentCode;
		};
	}
}
