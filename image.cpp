/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#include "image.h"
#include "world.h"
#include "render.h"

using namespace std;

//Load global images.
void Image::load_images_global(){
    //Set texture dimensions.

    background_static.w=1024;
    background_static.h=768;

    text_back.w=800;
    text_back.h=600;

    //Load the global images.
    background_static.texture=load_texture("data/images/background_static.png");
    text_back.texture=load_texture("data/images/text_back.png");
}

//Unload global images.
void Image::unload_images_global(){
    //Free up the global images.
    glDeleteTextures(1,&background_static.texture);
    glDeleteTextures(1,&text_back.texture);
}
