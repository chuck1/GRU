#ifndef GLUTPP_CAMERA_PROJECTION_BASE_HPP
#define GLUTPP_CAMERA_PROJECTION_BASE_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include <gal/stl/child.hpp>
#include <gal/etc/timestep.hpp>

//#include <PxPhysicsAPI.h>

// neb/phx/camera/proj/base.hpp

#include <neb/gfx/camera/proj/util/cast.hpp>
#include <neb/gfx/util/decl.hpp>
#include <neb/gfx/glsl/util/decl.hpp>


namespace neb { namespace gfx { namespace camera { namespace proj {
	/** @brief @Base */
	class base:
		virtual public gal::stl::child<neb::gfx::environ::base>,
		virtual public neb::gfx::camera::proj::util::cast
	{
		public:
			typedef typename gal::stl::child<neb::gfx::environ::base>::parent_t parent_t;
			/** @brief Constructor */
			base();
			/***/
			virtual void					init(parent_t * const &) = 0;
			/***/
			virtual glm::mat4&				proj() = 0;
			virtual void					calculate() = 0;
			virtual void					calculate_geometry() = 0;
			
			void						load(neb::gfx::glsl::program::base const * const p);
			/** @brief step
			 * @todo explain when in timeline this occurs and in which thread and why
			 */
			void						step(gal::etc::timestep const & ts);
		protected:
			// persistence
			glm::mat4					_M_matrix;
	};
}}}}


#endif


