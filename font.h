/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef font_h
#define font_h

#include "image_data.h"
#include "enumerations.h"

#include <SDL.h>
#include <string>

#define GLEW_STATIC
#include <glew.h>

class BitmapFont{
    private:
    //The font surface.
    image_data bitmap_font_texture;

    //The individual characters.
    SDL_Rect chars[256];

    public:

    //The width and height of the entire texture.
    double w,h;

    //Character spacing.
    short spacing_x;

    //Line spacing.
    short spacing_y;

    //The default constructor.
    BitmapFont();

    //Unloads the font from memory.
    void unload_font();

    //Generates the font.
    void build_font(std::string font_location,double get_w,double get_h,short get_spacing_x,short get_spacing_y);

    //Shows the text.
    void show(double x,double y,std::string text,short font_color,double opacity=1.0,double scale=1.0);
};

#endif
