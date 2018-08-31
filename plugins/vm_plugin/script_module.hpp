/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include "pro/utils/singleton.hpp "
#include <v8.h>
#include <string>
#include <list>
#include "script_util.hpp"
//script module
namespace xmax {

	namespace scriptv8 {

		using namespace v8;

		class ScriptMoudle{
		public:
			ScriptMoudle();
			~ScriptMoudle();

			void EnterContext();

			void LoadScript(const char* code);

			void RunFoo(const char* foo);

			void ExitContext();

			void StoreInstrunction(int ins);

			void CleanInstrunction();

		private:
		
			void AstBlockCallbackInsert();

			int instruction_count_;
			std::list<int> last_intruction_;
			
			std::string				 current_code_;
			std::string				 main_foo_;


			PersistentCpyableContext current_context_;

		};
	
	}
}
