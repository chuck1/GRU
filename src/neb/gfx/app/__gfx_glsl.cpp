#include <neb/gfx/util/config.hpp>
#include <neb/gfx/app/__gfx_glsl.hpp>
#include <neb/gfx/glsl/program.hpp>

weak_ptr<neb::app::__gfx_glsl>		neb::app::__gfx_glsl::global() {
	auto app(dynamic_pointer_cast<neb::app::__gfx_glsl>(g_app_));
	assert(app);
	return app;
}
void		neb::app::__gfx_glsl::__init() {
}
void		neb::app::__gfx_glsl::create_programs() {

	printf("%s\n", __PRETTY_FUNCTION__);

	std::shared_ptr<neb::glsl::program> p;

	// text
	{
		p.reset(new neb::glsl::program(neb::program_name::TEXT));
		p->init();

		p->add_shader(NEB_GFX_SHADER_DIR"v130/text/vs.glsl", GL_VERTEX_SHADER);
		p->add_shader(NEB_GFX_SHADER_DIR"v130/text/fs.glsl", GL_FRAGMENT_SHADER);

		p->compile();

		p->add_attrib(neb::attrib_name::e::COOR, "coord", 0);

		p->scanUniforms();
		p->locate();

		programs_[neb::program_name::TEXT] = p;
	}
	// quad

	// light
	{
		p.reset(new neb::glsl::program(neb::program_name::LIGHT));
		p->init();

		p->add_shader(NEB_GFX_SHADER_DIR"v130/light/vs.glsl", GL_VERTEX_SHADER);
		p->add_shader(NEB_GFX_SHADER_DIR"v130/light/fs.glsl", GL_FRAGMENT_SHADER);

		p->compile();

		p->add_attrib(neb::attrib_name::e::POSITION, "position", 1);
		p->add_attrib(neb::attrib_name::e::NORMAL, "normal", 2);

		p->scanUniforms();
		p->locate();

		programs_[neb::program_name::LIGHT] = p;


	}

	//light and image
	{
		p.reset(new neb::glsl::program(neb::program_name::IMAGE));
		p->init();

		p->add_shader(NEB_GFX_SHADER_DIR"v130/image/vs.glsl", GL_VERTEX_SHADER);
		p->add_shader(NEB_GFX_SHADER_DIR"v130/image/fs.glsl", GL_FRAGMENT_SHADER);

		p->compile();

		p->add_attrib(neb::attrib_name::e::POSITION, "position", 1);
		p->add_attrib(neb::attrib_name::e::NORMAL, "normal", 2);
		p->add_attrib(neb::attrib_name::e::TEXCOOR, "texcoor", 3);

		p->scanUniforms();
		p->locate();

		programs_[neb::program_name::IMAGE] = p;
	}

}
shared_ptr<neb::glsl::program>		neb::app::__gfx_glsl::use_program(neb::program_name::e name){
	auto p = get_program(name);

	p->use();

	current_ = p;

	return p;
}
shared_ptr<neb::glsl::program>		neb::app::__gfx_glsl::current_program(){
	assert(current_);

	return current_;
}
shared_ptr<neb::glsl::program>		neb::app::__gfx_glsl::get_program(neb::program_name::e name){
	auto it = programs_.find(name);

	if(it == programs_.end())
	{
		printf("program '%i' not found\n", name);
		exit(0);
	}

	auto p = (*it).second;

	assert(p);

	return p;
}


