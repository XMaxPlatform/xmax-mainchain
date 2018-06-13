/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace Xmax
{
	using Xmaxapp::options_desc;
	class blockbuilder_plugin : public Xmaxapp::plugin_face
	{
		GENERATED_PLUGIN(blockbuilder_plugin, Xmaxapp::plugin_face, nullptr)
	public:

		virtual void startup() override;

	protected:
		static void init_options(options_desc& cli, options_desc& cfg);

	};
}