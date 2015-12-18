/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fread.h
 * Author: ocarinahuff
 *
 * Created on November 8, 2015, 3:18 AM
 */

#ifndef FREAD_H
#define FREAD_H

#include <string>
#include "typedefs.h"

class FREAD {
/**
 * Reads a text file formatted as follows:
 *
 * .rows R
 * .cols C
 * .inputs 00 01 11 10 ...
 * .states a b c ...
 * a,1 b,- -,0 c,0 -,- ...
 * ...
 * .e
 * EOF
 *
 * Note that '...' is a placeholder representing any number of
 * elements or lines.  Also, R is an integer representing the
 * number of rows, and C is an integer representing the number
 * of columns.
 * .inputs are...
 * .states are...
 **/
 public:
    FREAD(std::string fname);
    Table<hentry> get_data() {return data;}
    Hdr get_states() {return states;}
    Hdr get_inputs() {return inputs;}
 private:
    Hdr states;
    Hdr inputs;
    Table<hentry> data;
};

#endif /* FREAD_H */