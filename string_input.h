/* Copyright (c) 2011 Kevin Wells */
/* D_O Chat may be freely redistributed.  See license for details. */

#ifndef string_input_h
#define string_input_h

#include <string>
#include <SDL.h>

class string_input{
    private:
    public:
    //Storage string.
    std::string str1;
    std::string str2;
    std::string str3;

    string_input();

    ~string_input();

    void handle_events();
};

#endif
