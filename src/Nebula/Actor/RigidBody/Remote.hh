#ifndef NEBULA_CONTENT_ACTOR_RIGID_BODY_REMOTE_HH
#define NEBULA_CONTENT_ACTOR_RIGID_BODY_REMOTE_HH

#include <Nebula/Actor/RigidActor/Remote.hh>
#include <Nebula/Actor/RigidBody/Base.hh>

namespace Neb {
	namespace Actor {
		namespace RigidBody {
			class Remote:
				virtual public Neb::Actor::RigidBody::Base,
				virtual public Neb::Actor::RigidActor::Remote
			{
				public:
					virtual void		step(double const & time, double const & dt);
				
			};
		}
	}
}

#endif

