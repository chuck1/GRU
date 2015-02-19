#ifndef __GLUTPP_SHADER_H__
#define __GLUTPP_SHADER_H__

#include <GLFW/glfw3.h>

#include <neb/core/itf/verbosity.hpp>

namespace neb { namespace gfx { namespace glsl {
	class shader:
		public neb::fnd::itf::verbosity<neb::gfx::glsl::shader>
	{
		public:
			void	load(const char * filename, GLenum shader_type);

			GLuint	o_;
	};
}}}

#endif
