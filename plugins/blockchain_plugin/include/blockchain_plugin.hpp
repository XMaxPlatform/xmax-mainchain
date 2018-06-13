/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace Xmax
{
	using Xmaxapp::options_desc;

	class blockchain_plugin : public Xmaxapp::plugin_face
	{
		GENERATED_PLUGIN(blockchain_plugin, Xmaxapp::plugin_face, &init_options)
	public:

	protected:
		static void init_options(options_desc& cli, options_desc& cfg);
	};
}