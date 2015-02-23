#ifndef NEBULA_LIGHT_DIRECTIONAL_HH
#define NEBULA_LIGHT_DIRECTIONAL_HH

#include <neb/fnd/environ/util/decl.hpp>
#include <neb/gfx/core/light/base.hpp>

namespace neb { namespace gfx { namespace core { namespace light {
	/** @brief
	 */
	class directional: virtual public neb::gfx::core::light::base {
		public:
			directional();
			virtual void		init(neb::fnd::core::light::util::parent * const & p);
			virtual type::e		getType();
			virtual void		callbackPose(neb::fnd::math::pose const &);
			virtual void		setShadowEnviron(std::shared_ptr<neb::fnd::environ::Base> environ);
			virtual void	load(ba::polymorphic_iarchive & ar, unsigned int const &);
			virtual void	save(ba::polymorphic_oarchive & ar, unsigned int const &) const;
			BOOST_SERIALIZATION_SPLIT_MEMBER();
	};
}}}}

#endif
