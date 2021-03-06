/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   table_impl.h
 * Author: ocarinahuff
 *
 * Created on November 15, 2015, 4:42 AM
 */

#ifndef TABLE_IMPL_H
#define TABLE_IMPL_H

#include "table.h"
#include <iostream>

template <class T>
table<T>::table() {
    clear();
}

template <class T>
void table<T>::clear() {
    data.clear();
    rows.clear();
    cols.clear();
    title.clear();
}

template <class T>
table<T>::table(Table<T> data, Hdr rows, Hdr cols, std::string title)
: data(data), rows(rows), cols(cols), title(title) {
}

template <class T>
table<T>::~table() {    
}

template <class T>
void table<T>::setData(Table<T> data) {
    this->data = data;
}

template <class T>
void table<T>::setRowHdr(Hdr rows) {
    this->rows = rows;
}

template <class T>
void table<T>::setColHdr(Hdr cols) {
    this->cols = cols;
}

template <class T>
void table<T>::setTitle(std::string title) {
    this->title = title;
}

template <class T>
MSet<T> table<T>::getRow(row r) const {
   MSet<T> ROW;
   for(auto& c : cols) {
       try {
           ROW.emplace(c.first,data.at({r,c.first}));
       }
       catch(std::out_of_range& e) {
           std::cerr << e.what() << " row " << r << std::endl;
       }
   }
   
   return ROW;
}

template <class T>
MSet<T> table<T>::getCol(col c) const {
   MSet<T> COL;
   for(auto& r : rows) {
       try {
           COL.emplace(r.first,data.at({r.first,c}));
       }
       catch(std::out_of_range& e) {
           std::cerr << e.what() << " col " << c << std::endl;
       }
   }
   
   return COL;
}

template <class T>
T table<T>::getElement(row r, col c) const {
    
    try {
        return data.at({r,c});
    }
    catch(std::out_of_range& e) {
        std::cerr << e.what() << " cell " << r << ',' << c << std::endl;
    }
}

template <class T>
Table<T> table<T>::getData() {
   return data;
}

template <class T>
std::string table<T>::getTitle() {
   return title;
}

template <class T>
Hdr table<T>::getRowHdr() const {
   return rows;
}

template <class T>
Hdr table<T>::getColHdr() const {
   return cols;
}

template <class T>
void table<T>::delRow(row r) {
   for(auto& c : cols)
       data.template erase({r,c.first});
   rows.erase(r);
}

template <class T>
void table<T>::delCol(col c) {
   for(auto& r : rows)
       data.template erase({r.first,c});
   cols.erase(c);
}

template <class T>
void table<T>::setRow(MSet<T> R, row r) {
   if(rows.find(r) == rows.end())
       return;
   for(auto& c : cols)
       data[{r,c.first}] = R[c.first];
}

template <class T>
void table<T>::setCol(MSet<T> C, col c) {
   if(cols.find(c) == cols.end())
       return;
   for(auto& r : rows)
       data[{r.first, c}] = C[r.first];

}

template <class T>
void table<T>::setElement(T e, row r, col c) {
   if(rows.find(r) == rows.end() || cols.find(c) == cols.end())
       return;
   data[{r,c}] = e;
}

template <class T>
void table<T>::addRow(MSet<T> R, row r, std::string name) {
   if(rows.find(r) != rows.end())
       return;
   for(auto& c : cols)
       data[{r,c.first}] = R[c.first];
   rows[r] = name;
}

template <class T>
void table<T>::addCol(MSet<T> C, col c, std::string name) {
   if(cols.find(c) != cols.end())
       return;
   for(auto& r : rows)
       data[{r.first, c}] = C[r.first];
   cols[c] = name;
}

template <class T>
bool table<T>::isempty() const {
    return data.empty();
}

#endif /* TABLE_IMPL_H */

