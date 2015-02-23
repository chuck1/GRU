#ifndef NEBULA_LIGHT_SPOT_HH
#define NEBULA_LIGHT_SPOT_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <galaxy/flag.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <neb/fnd/core/light/base.hpp>
#include <neb/fnd/util/config.hpp>
#include <neb/fnd/util/decl.hpp>
#include <neb/fnd/itf/shared.hpp>
#include <neb/fnd/math/serialization/glm.hpp>

#include <neb/gfx/core/light/base.hpp>
#include <neb/gfx/texture/Base.hpp>

namespace neb { namespace gfx { namespace core { namespace light {
		/** @brief
		 *
		 */
		class spot: public neb::gfx::core::light::base {
			private:
				template<class Archive> void		serializeTemplate(Archive & ar, unsigned int const & version) {


					ar & boost::serialization::make_nvp("spot_direction",spot_direction_);
					ar & boost::serialization::make_nvp("spot_cutoff",spot_cutoff_);
					ar & boost::serialization::make_nvp("spot_exponent",spot_exponent_);
					ar & boost::serialization::make_nvp("spot_light_cos_cutoff",spot_light_cos_cutoff_);
				}
			public:
				spot();
				virtual void			init(neb::fnd::core::light::util::parent * const & p);
				virtual type::e			getType();

				virtual void			callbackPose(neb::fnd::math::pose const & gpose);



				virtual void				load(neb::fnd::core::light::util::count & light_count, neb::fnd::math::pose const & pose);

				virtual void			serialize(boost::archive::polymorphic_iarchive & ar, unsigned int const & version) {
					neb::gfx::core::light::base::serialize(ar, version);
					serializeTemplate(ar, version);
				}
				virtual void			serialize(boost::archive::polymorphic_oarchive & ar, unsigned int const & version) {
					neb::gfx::core::light::base::serialize(ar, version);
					serializeTemplate(ar, version);
				}	

				virtual void			setShadowEnviron(std::shared_ptr<neb::gfx::environ::base> environ);
				

				virtual void	load(ba::polymorphic_iarchive & ar, unsigned int const &);
				virtual void	save(ba::polymorphic_oarchive & ar, unsigned int const &) const;
				BOOST_SERIALIZATION_SPLIT_MEMBER();

		};

}}}}

#endif
