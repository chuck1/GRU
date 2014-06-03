#ifndef NEBULA_GRAPHICS_DRAWABLE_BASE_HPP
#define NEBULA_GRAPHICS_DRAWABLE_BASE_HPP

namespace Neb {
        namespace Graphics {
                namespace Drawable {
                        /** @brief %Drawable
                        * 
                        * Contains content to draw in a context (scene, layout, etc.).
                        */
                        class Base: gal::std::shared {
                                public:
                                        void            draw() = 0;
                        };     
                }
        }
}

#endif