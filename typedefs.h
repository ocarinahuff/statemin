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

// simple substitution for readability.
typedef int row;
typedef int col;

/**
 * Tthis stores the next state and output for each entry
 * in the huffman flow table.
 */
typedef struct HENTRY {
    int next_state;
    char output;
    bool operator<(const HENTRY& he) const {
        if(next_state == he.next_state)
            return output < he.output;
        else
            return next_state < he.next_state;
    }
    bool operator==(const HENTRY& he) const {
        return next_state == he.next_state && output == he.output;
    }
    bool operator!=(const HENTRY& he) const {
        return next_state != he.next_state || output != he.output;
    }
} hentry;

/**
 * This is the row/column cell construct used to map a table to a linear
 * std::map data structure.  By default rows and columns start from 1.
 */
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

/**
 * This stores the names of the rows and columns, mapped to the 
 * internal ids used for rows and columns.  It is used by the 
 * printer functions to output to std::cout.
 */ 
using Hdr = std::map<int,std::string>;

// this is a template that maps each cell to a data of type class T.
template <class T>
using Table = std::map<cell,T>;


/**
 * This is a template that maps a set of int to a set of data elements
 * of type class T.
 * It is used to contain a linear subset of Table<T>, such as a Row or
 * Column.
 */
template <class T>
using MSet = std::map<int,T>;

/**
 * This is used by the bcp table to contain the computed solution.
 * The column number represents the input variables, and the mapped
 * boolean value is true if it is part of the solution, and false 
 * otherwise.
 */
using Sol = std::map<col,bool>;

/**
 * cp is a set of one or more states, it is officially called compatible pair,
 * but in this program it is being used to represent more than just a pair of
 * states.
 * cpset is a set of cp.
 * 
 * std::set automatically sorts its elements, this is an unintended side effect.
 * It also does not allow duplicates, this property was intentionally chosen
 * for this program.
 */
using cp = std::set<int>;
using cpset = std::set<cp>;

/**
 * Used by printer functions during debug.  It denotes
 * the initial, intermediate and final states of various
 * tables used by this program.
 */
enum State {INIT, INTMED, FINAL};

/**
 * Used by hftable to denote incompatible and unconditional pairs
 * during the generation and manipulation of the pairs chart.
 */
enum PairState {INCOMPATIBLE = -1, UNCONDITIONAL = 0};

/**
 * Used by the table to denote an entry in which the
 * next state is undefined(denoted by '-' in text file.
 */
enum NextState {NOSTATE = 0};

#endif /* TYPEDEFS_H */

