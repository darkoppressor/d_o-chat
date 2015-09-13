/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#include "font.h"
#include "render.h"
#include "world.h"

using namespace std;

BitmapFont::BitmapFont(){
    //Initialize the variables:
    w=0;
    h=0;
    spacing_x=0;
    spacing_y=0;
}

void BitmapFont::unload_font(){
    glDeleteTextures(1,&bitmap_font_texture.texture);
}

void BitmapFont::build_font(string font_location,double get_w,double get_h,short get_spacing_x,short get_spacing_y){
    bitmap_font_texture.texture=load_texture(font_location);

    //Set the texture dimensions:
    w=get_w;
    h=get_h;
    bitmap_font_texture.w=w;
    bitmap_font_texture.h=h;

    //Set the spacing amounts.
    spacing_x=get_spacing_x;
    spacing_y=get_spacing_y;

    //Set the cell dimensions:
    double cellW=w/256;
    double cellH=h;

    //The current character we are setting.
    short currentChar=0;

    //Go through the cell columns.
    for(short cols=0;cols<256;cols++){
        //Set the character offset:
        chars[currentChar].x=cellW*cols;
        chars[currentChar].y=0;
        //Set the dimensions of the character:
        chars[currentChar].w=cellW;
        chars[currentChar].h=cellH;
        //Go to the next character.
        currentChar++;
    }
}

void BitmapFont::show(double x,double y,string text,short font_color,double opacity,double scale){
    //Temporary offsets.
    double X=x,Y=y;

    //Bind the texture object.
    if(image.current_texture!=bitmap_font_texture.texture){
        glBindTexture(GL_TEXTURE_2D,bitmap_font_texture.texture);
        image.current_texture=bitmap_font_texture.texture;
    }

    //Go through the text.
    for(short show=0;text[show]!='\0';show++){
        //Get the ASCII value of the character.
        short ascii=(unsigned char)text[show];

        if(text[show]!='\xA'){
            //Show the character.
            render_font((int)X,(int)Y,bitmap_font_texture,&chars[ascii],font_color,opacity,scale);

            //Move over the character space amount.
            X+=spacing_x;
        }
        else{
            Y+=spacing_y;
            X=x;
        }
    }
}
