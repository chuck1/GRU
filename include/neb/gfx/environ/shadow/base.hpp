#ifndef NEBULA_GFX_ENVIRON_SHADOW_BASE_HPP
#define NEBULA_GFX_ENVIRON_SHADOW_BASE_HPP

#include <neb/fnd/core/light/base.hpp>
#include <neb/fnd/environ/shadow/Base.hpp>

#include <neb/gfx/util/decl.hpp>
#include <neb/gfx/glsl/util/decl.hpp>
#include <neb/gfx/environ/three.hpp>

namespace neb { namespace gfx { namespace environ { namespace shadow {
	/** @brief base
	 *
	 * Abstract class that contains functions and data needed to render a specific kind of drawable.
	 */
	template<typename LIGHT>
	class base:
		//virtual public neb::fnd::environ::shadow::Base,
		virtual public neb::gfx::environ::three
	{
		public:
			typedef std::shared_ptr<neb::gfx::glsl::program::Base>		program_shared;
			typedef std::weak_ptr<neb::gfx::environ::SceneDefault>		environ_scene_weak;
		public:
			virtual ~base() {}
			virtual void		init(parent_t * const & p) = 0;
			virtual void		step(gal::etc::timestep const & ts) = 0;
			//virtual void		resize(int w, int h) = 0;
			virtual void		render(std::shared_ptr<neb::gfx::context::base> context) = 0;
			virtual bool		shouldRender() = 0;
			virtual bool		shouldRender(unsigned int) { return true; }
		/*
			virtual void		setLight(std::shared_ptr<neb::fnd::core::light::base> light)
			{
				auto l = std::dynamic_pointer_cast<LIGHT>(light);
				assert(l);
				light_ = l;
			}
			virtual void		setSceneEnviron(std::shared_ptr<neb::fnd::environ::Base> environ)
			{
				auto e = std::dynamic_pointer_cast<neb::gfx::environ::SceneDefault>(environ);
				assert(e);
				environ_scene_ = e;
			}
			*/
		public:	

	};
}}}}

#endif
