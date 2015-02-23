#include <gal/log/log.hpp>

#include <neb/fnd/util/debug.hpp>
#include <neb/fnd/util/log.hpp>

#include <neb/gfx/core/light/base.hpp>
#include <neb/gfx/core/light/util/parent.hpp>

/*std::weak_ptr<neb::gfx::core::scene::base>		neb::gfx::core::light::util::parent::getScene() {
	auto shape = isShapeBase();
	assert(shape);

	auto scene1 = shape->getScene();

	return scene1;
}*/
void			neb::gfx::core::light::util::parent::setPose(neb::fnd::math::pose const & pose) {
	LOG(lg, neb::fnd::core::shape::sl, debug) << __PRETTY_FUNCTION__;
	
	auto lambda_light = [&] (map_type::pointer p) {
		
		auto light = std::dynamic_pointer_cast<neb::gfx::core::light::base>(p);
		assert(light);
		
		//if(i == neb::fnd::light::light_max) return L::map_type::BREAK;
		
		light->setPose(pose);
		
	};

	map_.for_each(lambda_light);

}
void			neb::gfx::core::light::util::parent::load_lights(
		neb::fnd::core::light::util::count & light_count,
		neb::fnd::math::pose const & pose)
{
	LOG(lg, neb::fnd::core::shape::sl, debug) << __PRETTY_FUNCTION__;

	assert(0);

	auto lambda_light = [&] (map_type::pointer p) {

		auto light = std::dynamic_pointer_cast<neb::gfx::core::light::base>(p);
		assert(light);

		//if(i == neb::fnd::light::light_max) return L::map_type::BREAK;

		//light->load(light_count, pose);

	};

	map_.for_each(lambda_light);

}

