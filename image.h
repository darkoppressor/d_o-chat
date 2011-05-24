/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef image_h
#define image_h

#include <vector>

#include "image_data.h"

#include <SDL.h>
#include <string>

#define GLEW_STATIC
#include <glew.h>

class Image{
    private:
    public:
    //
    void load_images_global();
    void unload_images_global();

    //The currently bound texture.
    GLuint current_texture;

    //Global images.
    image_data background_static;
    image_data text_back;
};

#endif
