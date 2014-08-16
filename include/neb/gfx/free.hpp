#ifndef GRU_FREE_HPP
#define GRU_FREE_HPP

#include <memory>

#include <neb/core/color/Color.hh>

#include <neb/gfx/util/decl.hpp>

using namespace std;

namespace neb {
	void	init_log();
	void	init();
}

namespace neb {
	void	draw_quad(shared_ptr<neb::glsl::program> p,
			float, float, float, float, neb::Color::color<float>);
	void	draw_text(
			shared_ptr<neb::glsl::program> p,
			float, float, float, float, neb::Color::color<float>, ::std::string);
}

void	checkerror(char const *);
bool	isGLError();

template<class A> A*	new_copy(std::shared_ptr<A> a) {
	if(a) {
		return new A(*a);
	}
	return NULL;
}

#endif
