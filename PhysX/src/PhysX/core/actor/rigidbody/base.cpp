/*
#include <Nebula/Network/server.hh>

#include <Nebula/Graphics/Context/Base.hh>
#include <Nebula/Graphics/Camera/View/ridealong.hh>

#include <Nebula/debug.hh>
#include <Nebula/Types.hh>
#include <Nebula/App/Base.hh>
//#include <Nebula/Physics.hh>
//#include <Nebula/simulation_callback.hh>

#include <Nebula/Actor/Util/Types.hh>
//#include <Nebula/Actor/Control/Util/Types.hh>
#include <Nebula/Actor/RigidBody/Base.hh>
#include <Nebula/Actor/RigidDynamic/Local.hh>

#include <Nebula/network/Types.hh>
#include <Nebula/network/message.hh>
#include <Nebula/Message/Actor/Control.hh>
#include <Nebula/Message/Types.hh>
*/
#include <Galaxy-Log/log.hpp>

#include <Nebula/Graphics/Window/Base.hh>

#include <PhysX/util/convert.hpp>
#include <PhysX/core/scene/base.hpp>
#include <PhysX/core/actor/util/parent.hpp>
#include <PhysX/core/actor/control/rigidbody/base.hpp>
#include <PhysX/core/actor/rigiddynamic/local.hpp>


phx::core::actor::rigidbody::base::base(sp::shared_ptr<phx::core::actor::util::parent> parent):
	neb::core::actor::base(parent),
	neb::core::actor::actor::base(parent),
	neb::core::actor::rigidactor::base(parent),
	neb::core::actor::rigidbody::base(parent),
	phx::core::actor::base(parent),
	phx::core::actor::actor::base(parent),
	phx::core::actor::rigidactor::base(parent),
	force_(0.0,0.0,0.0),
	torque_(0.0,0.0,0.0)
{}
void			phx::core::actor::rigidbody::base::init() {

	BOOST_LOG_CHANNEL_SEV(lg, "phx core actor", debug) << __PRETTY_FUNCTION__;;
	
	neb::core::actor::rigidbody::base::init();
	phx::core::actor::rigidactor::base::init();
}
void			phx::core::actor::rigidbody::base::release() {

	BOOST_LOG_CHANNEL_SEV(lg, "phx core actor", debug) << __PRETTY_FUNCTION__;;

	neb::core::actor::rigidbody::base::release();
	phx::core::actor::rigidactor::base::release();
}
void			phx::core::actor::rigidbody::base::setPose(neb::core::pose const & pose) {

	BOOST_LOG_CHANNEL_SEV(lg, "phx core actor", debug) << __PRETTY_FUNCTION__;;

	neb::core::actor::rigidbody::base::setPose(pose);
	phx::core::actor::rigidactor::base::setPose(pose);
}
void			phx::core::actor::rigidbody::base::add_force(real time) {

	BOOST_LOG_CHANNEL_SEV(lg, "phx core actor", debug) << __PRETTY_FUNCTION__;;

	// non-user-controled
	//physx::PxVec3 f(force_[0],force_[1],force_[2]);
	//physx::PxVec3 t(torque_[0],torque_[1],torque_[2]);
	
	vec4 f(force_,1);
	vec4 t(torque_,1);

	// user-controlled
	if(control_) {
		f += control_->f();
		t += control_->t();
	}
	
	//physx::PxTransform pose = pose_;
	
	f = pose_.rot_ * f;//pose_.q.rotate(f);
	t = pose_.rot_ * t;//.q.rotate(t);

	assert(px_actor_);
	physx::PxRigidBody* pxrigidbody = px_actor_->isRigidBody();
	assert(pxrigidbody);


	BOOST_LOG_CHANNEL_SEV(lg, "phx core actor", debug)
		<< std::setw(8) << "f"
		<< std::setw(8) << f.x
		<< std::setw(8) << f.y
		<< std::setw(8) << f.z
		<< " mass " << pxrigidbody->getMass();



	
	
	pxrigidbody->addForce(phx::util::convert(vec3(f)));
	pxrigidbody->addTorque(phx::util::convert(vec3(t)));
}
sp::shared_ptr<phx::core::actor::rigiddynamic::local>		phx::core::actor::rigidbody::base::get_projectile() {
	BOOST_LOG_CHANNEL_SEV(lg, "phx core actor", debug) << __PRETTY_FUNCTION__;;
	
	auto parent(parent_.lock());

	sp::shared_ptr<phx::core::scene::base> scene = parent->getScene();
	
	auto actor(sp::make_shared<phx::core::actor::rigiddynamic::local>(scene));
	
	
	vec3 pos_relative(0,0,-2);
	vec3 vel_relative(0,0,-1);
	
	
	pos_relative = pose_.rot_ * pos_relative;
	
	
	
	actor->pose_ = pose_;

	// pose
	
	actor->pose_.pos_ += vec4(pos_relative,0);
	
	// velocity
	
	vec3 vel(velocity_);
	
	vel_relative = pose_.rot_ * vel_relative;
	
	vel += vel_relative;
	
	actor->velocity_ = vel;
	
	return actor;
}
void		phx::core::actor::rigidbody::base::create_control(sp::shared_ptr<neb::gfx::window::base> window) {

	//auto me = isRigidBodyBase();

	auto control(sp::make_shared<phx::core::actor::control::rigidbody::manual>());
	
	control_ = control;

	control->actor_ = isPxActorRigidBodyBase();
	
	control->conn_.key_fun_ = window->sig_.key_fun_.connect(
			20,
			neb::Signals::KeyFun::slot_type(
				&phx::core::actor::control::rigidbody::base::key_fun,
				control.get(),
				_1,
				_2,
				_3,
				_4,
				_5
				).track_foreign(control)
			);


}
void		phx::core::actor::rigidbody::base::step(neb::core::TimeStep const & ts) {

	BOOST_LOG_CHANNEL_SEV(lg, "phx core actor", debug) << __PRETTY_FUNCTION__;;
	
	add_force(ts.dt);
}


