#ifndef __NEBULA_ACTOR_DESC_H__
#define __NEBULA_ACTOR_DESC_H__

#include <math/transform.h>
#include <math/raw/raw.h>

#include <gal/network/serial.h>

#include <glutpp/actor/id.h>
#include <glutpp/shape/desc.h>

namespace glutpp {
	namespace actor {

		typedef gal::network::vector_ext<glutpp::actor::desc> vec_actor_desc;
		typedef gal::network::vector_ext<glutpp::shape::desc> vec_shape_desc;
		typedef std::shared_ptr<vec_actor_desc> vec_actor_desc_s;
		typedef std::shared_ptr<vec_shape_desc> vec_shape_desc_s;


		struct desc: gal::network::serial_ext<vec_shape_desc, vec_actor_desc, raw, id>
		{
			desc();
			void			load(glutpp::actor::actor_s);
			void			load(tinyxml2::XMLElement*);
			void			add_shape(glutpp::shape::desc);
			
			desc&			operator=(desc const &);
			
			
			id_s			get_id() const;
			raw_s			get_raw() const;
			vec_actor_desc_s	get_actors() const;
			vec_shape_desc_s	get_shapes() const;
			
			glutpp::actor::mode_create::e	mode_create_;
			
		};
	}
}

#endif








