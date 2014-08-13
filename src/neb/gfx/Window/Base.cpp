#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <map>

#include <gal/log/log.hpp>

#include <neb/core/debug.hh>
#include <neb/core/scene/util/decl.hpp>
#include <neb/core/actor/base.hpp>
#include <neb/core/light/base.hpp>

#include <neb/gfx/free.hpp>

#include <neb/gfx/app/__gfx.hpp>
#include <neb/gfx/app/__gfx_glsl.hpp>

#include <neb/gfx/Context/Base.hh>
#include <neb/gfx/window/Base.hh>
#include <neb/gfx/window/util/Parent.hh>
#include <neb/gfx/util/log.hpp>

neb::gfx::window::base::base():
	x_(0),
	y_(0),
	w_(600),
	h_(600)
{
}
neb::gfx::window::base::base(sp::shared_ptr<neb::gfx::window::util::parent> parent):
	parent_(parent),
	x_(0),
	y_(0),
	w_(600),
	h_(600)
{
}
neb::gfx::window::base::~base() {
}
void neb::gfx::window::base::init() {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	neb::std::shared::init();

	auto app = neb::app::__gfx::global().lock();
	assert(app);

	assert(app->flag_.any(neb::core::app::util::flag::INIT_GLFW));

	self_ = sp::dynamic_pointer_cast<neb::gfx::window::base>(shared_from_this());

	// create window
	window_ = glfwCreateWindow(
			w_,
			h_,
			title_.c_str(),
			NULL,
			NULL);

	if(window_ == NULL) {
		glfwTerminate();
		if(DEBUG_NEB) LOG(lg, neb::gfx::sl, critical) << "glfwCreateWindow failed";
		exit(EXIT_FAILURE);
	}



	glfwMakeContextCurrent(window_);

	glfwSetWindowPosCallback(
			window_,
			neb::app::__gfx::static_window_pos_fun);
	glfwSetWindowSizeCallback(
			window_,
			neb::app::__gfx::static_window_size_fun);
	glfwSetWindowCloseCallback(
			window_,
			neb::app::__gfx::static_window_close_fun);
	glfwSetWindowRefreshCallback(
			window_,
			neb::app::__gfx::static_window_refresh_fun);
	glfwSetKeyCallback(
			window_,
			neb::app::__gfx::static_key_fun);
	glfwSetMouseButtonCallback(
			window_,
			neb::app::__gfx::static_mouse_button_fun);

	// add window to app's window map
	app->windows_glfw_[window_] = self_;


	//if(all(neb::app::base::option::SHADERS)) create_programs();


	app->init_glew();

	auto app2 = neb::app::__gfx_glsl::global().lock();


	app2->create_programs();







	//printf("%s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Check for necessary extensions
	if(!GL_ARB_depth_texture || !GL_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
		exit(0);//return false;
	}

	//Shading states
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);



	checkerror("unknown");
}
void		neb::gfx::window::base::release() {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;


	glfwDestroyWindow(window_);
}
void		neb::gfx::window::base::render() {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	glfwMakeContextCurrent(window_);

	/** @todo rendering multiple contexts in a window */


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	typedef neb::gfx::context::util::parent C;

	C::map_.for_each<0>([] (C::map_type::iterator<0> it) {
			auto context = sp::dynamic_pointer_cast<neb::gfx::context::base>(it->ptr_);
			assert(context);
			context->render();
			});

	glFinish();
	glfwSwapBuffers(window_);
}
void neb::gfx::window::base::callback_window_refresh_fun(GLFWwindow*) {
}
void			neb::gfx::window::base::step(gal::std::timestep const & ts) {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	neb::gfx::context::util::parent::step(ts);


	if(glfwWindowShouldClose(window_)) {
		parent_->erase(i_);
		return;
	}



	/** @todo wtf is this doing here?? */
	render();
}
void neb::gfx::window::base::callback_window_size_fun(GLFWwindow* window, int w, int h) {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	w_ = w;
	h_ = h;

	resize();

	callback_window_refresh_fun(window);
}
void neb::gfx::window::base::callback_window_pos_fun(GLFWwindow* window, int x, int y) {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	x_ = x;
	y_ = y;

	callback_window_refresh_fun(window);
}
void neb::gfx::window::base::callback_window_close_fun(GLFWwindow* window){
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

}
void neb::gfx::window::base::callback_mouse_button_fun(GLFWwindow* window, int button, int action, int mods) {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	sig_.mouse_button_fun_(self_, button, action, mods);
}
void neb::gfx::window::base::callback_key_fun(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	sig_.key_fun_(self_, key, scancode, action, mods);

}
void neb::gfx::window::base::resize() {
	if(DEBUG_NEB) LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;


	glViewport(0, 0, w_, h_);

	typedef neb::gfx::context::util::parent C;

	C::map_.for_each<0>([&] (C::map_type::iterator<0> it) {
			auto context = sp::dynamic_pointer_cast<neb::gfx::context::base>(it->ptr_);
			assert(context);
			context->resize(w_, h_);
			});

}




