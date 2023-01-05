#ifndef CTRPLUGINFRAMEWORK_ALICECODES_HPP
#define CTRPLUGINFRAMEWORK_ALICECODES_HPP

#include <CTRPluginFramework.hpp>
#include "types.h"

namespace CTRPluginFramework
{
	class AliceCodes
	{
	public:
		static Result SetTopScreenBackground(const std::string &filename, bool resize = false);
		static Result SetBottomScreenBackground(const std::string &filename, bool resize = false);
	};
}

#endif