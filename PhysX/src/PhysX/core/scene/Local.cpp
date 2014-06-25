
#include <Galaxy-Log/log.hpp>

#include <neb/app/Base.hh>
#include <neb/core/scene/Local.hh>
#include <neb/core/actor/RigidBody/Base.hh>
#include <neb/timer/Actor/Release.hpp>
#include <neb/debug.hh>



#include <PhysX/util/convert.hpp>
#include <PhysX/core/scene/local.hpp>
#include <PhysX/core/actor/rigiddynamic/local.hpp>
#include <PhysX/core/actor/rigidstatic/local.hpp>

phx::core::scene::local::local(sp::shared_ptr<neb::core::scene::util::parent> parent):
	neb::core::scene::base(parent),
	neb::core::scene::local(parent),
	phx::core::scene::base(parent)
{
	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << __PRETTY_FUNCTION__;
}
void			phx::core::scene::local::init() {
	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << __PRETTY_FUNCTION__;

	neb::core::scene::local::init();
	phx::core::scene::base::init();
}
void			phx::core::scene::local::release() {
	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << __PRETTY_FUNCTION__;

	neb::core::scene::local::release();
	phx::core::scene::base::release();
}
void			phx::core::scene::local::step(gal::std::timestep const & ts) {
	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << __PRETTY_FUNCTION__ << " dt = " << ts.dt;

	neb::core::scene::local::step(ts);
	phx::core::scene::base::step(ts);

	auto app = neb::app::base::global();

	// timer
	//timer_set_.step(time);

	//physx::PxU32 nbPxactor = px_scene_->getNbActors(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC);

	// PxScene
	assert(px_scene_ != NULL);

	typedef neb::core::actor::util::parent A;


	//========================================================================
	// lock all actors

	A::map_.for_each<0>([&] (A::map_type::iterator<0> it) {
			auto actor = sp::dynamic_pointer_cast<neb::core::actor::base>(it->ptr_);
			assert(actor);
			actor->mutex_.lock();
			if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << "actor = " << actor.get();
			});

	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << "actors locked";
	/*A::map_.for_each<0>([&] (A::map_type::iterator<0> it) {
	  auto actor = sp::dynamic_pointer_cast<neb::core::actor::base>(it->ptr_);
	  assert(actor);
	  if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << "actor = " << actor.get();
	  });*/

	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << "simulate";

	px_scene_->simulate(ts.dt);
	px_scene_->fetchResults(true);

	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << "simulation complete";

	// retrieve array of actors that moved
	physx::PxU32 nb_active_transforms;
	const physx::PxActiveTransform* active_transforms = px_scene_->getActiveTransforms(nb_active_transforms);


	if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug)
		<< "active transforms: " << nb_active_transforms;

	//physx::PxTransform pose;
	physx::PxTransform pose;

	// update each render object with the new transform
	for(physx::PxU32 i = 0; i < nb_active_transforms; ++i) {
		//physx::PxActor* px_actor = active_transforms[i].actor;

		//printf( "actor type = %i\n", px_actor->getType() );

		physx::PxActor* pxactor = active_transforms[i].actor;
		assert(pxactor);

		void* ud = active_transforms[i].userData;
		assert(ud);

		if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug) << "ud = " << ud;

		physx::PxRigidBody* pxrigidbody = pxactor->isRigidBody();


		neb::core::actor::base* pactor = static_cast<neb::core::actor::base*>(ud);
		auto actor = pactor->isActorBase();
		assert(actor);

		if(actor) {
			pose = active_transforms[i].actor2World;
			actor->setPose(neb::core::pose(
						phx::util::convert(pose.p),
						phx::util::convert(pose.q)
						));

			if(DEBUG_NEB) BOOST_LOG_CHANNEL_SEV(lg, "phx core scene", debug)
				<< std::setw(8) << "p"
					<< std::setw(8) << pose.p.x
					<< std::setw(8) << pose.p.y
					<< std::setw(8) << pose.p.z;


			if(pxrigidbody != NULL) {
				auto rigidbody = actor->isActorRigidBody();
				if(!rigidbody) {
					std::cout << typeid(*actor).name() << std::endl;
					abort();
				}

				physx::PxVec3 v(pxrigidbody->getLinearVelocity());

				rigidbody->velocity_ = phx::util::convert(v);

				//v.print();
			}

			actor->flag_.set(neb::core::actor::util::flag::E::SHOULD_UPDATE);
		}
	}


	// unlock all actors
	A::map_.for_each<0>([&] (A::map_type::iterator<0> it) {
			auto actor = sp::dynamic_pointer_cast<neb::core::actor::base>(it->ptr_);
			assert(actor);
			actor->mutex_.unlock();
			});




	// vehicle
	//physx::PxVec3 g(0,-0.25,0);
	//vehicle_manager_.vehicle_suspension_raycasts(px_scene_);
	//vehicle_manager_.update((float)dt, g);

	send_actor_update();

}
sp::weak_ptr<neb::core::actor::rigidstatic::base>	phx::core::scene::local::createActorRigidStaticUninitialized() {

	auto actor(sp::make_shared<phx::core::actor::rigidstatic::local>(isPxSceneLocal()));

	neb::core::actor::util::parent::insert(actor);

	actor->simulation_.word0 = phx::filter::filter::type::STATIC;
	actor->simulation_.word1 = phx::filter::filter::RIGID_AGAINST;
	actor->simulation_.word2 = phx::filter::filter::type::STATIC;
	actor->simulation_.word3 = phx::filter::filter::type::PROJECTILE;

	return actor;
}


