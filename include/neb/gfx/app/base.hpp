#ifndef NEB_GFX_APP_BASE_HPP
#define NEB_GFX_APP_BASE_HPP

#include <neb/gfx/app/__gfx.hpp>
#include <neb/gfx/app/__gfx_glsl.hpp>
#include <neb/gfx/app/glfw.hpp>

namespace neb { namespace gfx { namespace app {
	/** @brief base */
	class Base:
		virtual public neb::fnd::app::Base,
		virtual public neb::gfx::app::draw,
		virtual public neb::gfx::app::glsl,
		virtual public neb::gfx::app::glfw
	{
		public:
			static weak_ptr<neb::gfx::app::Base>		initialize();
		protected:
			virtual void					__init();
		public:
			virtual void					render();
			virtual void					release();
			virtual void					step(gal::etc::timestep const & ts);
			void						loop();
	};
}}}

#endif
