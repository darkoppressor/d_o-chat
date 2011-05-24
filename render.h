/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef render_h
#define render_h

#include "image_data.h"
#include "enumerations.h"

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#define GLEW_STATIC
#include <glew.h>

#include <SDL_gfxPrimitives.h>

//Load an image that will be used to create an OpenGL texture.
SDL_Surface *load_image_opengl(std::string filename);

//Call this when loading image files to be used as textures throughout the program.
GLuint load_texture(std::string filename);

//Render a full texture to the screen.
void render_texture(double x,double y,image_data image_source,double opacity=1.0);

//Render a sprite from a spritesheet to the screen.
void render_sprite(double x,double y,image_data image_source,SDL_Rect* texture_clip=NULL,double opacity=1.0,double scale_x=1.0,double scale_y=1.0);

void render_rectangle(double x,double y,double w,double h,double opacity,short color_name);

//Render text to the screen using a bitmap font.
void render_font(double x,double y,image_data image_source,SDL_Rect* texture_clip=NULL,short color_name=COLOR_WHITE,double opacity=1.0,double scale=1.0);

//Holds red, green, blue color data in double format.
struct color_data{
    double red;
    double green;
    double blue;
};

//Converts a color name (defined in enumerations.h) to red, green, blue color data in double format.
color_data color_name_to_doubles(short color_number);

//Converts red, green, blue color data in 0-255 format to double format.
color_data color_shorts_to_doubles(short red,short green,short blue);

#endif
