/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   typedefs.h
 * Author: ocarinahuff
 *
 * Created on November 8, 2015, 3:24 AM
 */

#include <map>
#include <string>
#include <set>

#ifndef TYPEDEFS_H
#define	TYPEDEFS_H

typedef int row;
typedef int col;

typedef struct {
    std::string next_state;
    char output;
} hentry;

typedef struct CELL {
    row r;
    col c;
    bool operator<(const CELL& cl) const {
        if(r == cl.r)
            return c < cl.c;
        else
            return r < cl.r;
    }
    bool operator==(const CELL& c1) const {
        return r == c1.r && c == c1.c;
    }
    bool operator!=(const CELL& c1) const {
        return r != c1.r || c != c1.c;
    }
} cell;

// this stores the names of the rows and columns, mapped to the 
// internal ids used for rows and columns.
typedef std::map<int,std::string> Hdr;

// this is a template that maps each cell to a data of type class T.
template <class T>
using Table = std::map<cell,T>;
template <class T>
using Row = std::map<col,T>;
template <class T>
using Col = std::map<row,T>;

typedef std::map<int,bool> Sol;

typedef std::set<std::string> cp;
typedef std::set<cp> cpset;

typedef std::set<std::string> mc;
typedef std::set<mc> mcset;

enum State {INIT, INTMED, FINAL};

#endif /* TYPEDEFS_H */

