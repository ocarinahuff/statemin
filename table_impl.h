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
Row<T> table<T>::getRow(row r) {
   Row<T> ROW;
   for(auto& c : cols)
       ROW[c.first] = data[{r,c.first}];
   return ROW;
}

template <class T>
Col<T> table<T>::getCol(col c) {
   Col<T> COL;
   for(auto& r : rows)
       COL[r.first] = data[{r.first,c}];
   return COL;
}

template <class T>
T table<T>::getElement(row r, col c) {
   return data[{r,c}];
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
Hdr table<T>::getRowHdr() {
   return rows;
}

template <class T>
Hdr table<T>::getColHdr() {
   return cols;
}

template <class T>
void table<T>::delRow(row r) {
   for(auto& c : cols)
       data.template erase({r,c});
   rows.erase(r);
}

template <class T>
void table<T>::delCol(col c) {
   for(auto& r : rows)
       data.template erase({r.first,c});
   cols.erase(c);
}

template <class T>
void table<T>::setRow(Row<T> R, row r) {
   if(rows.find(r) == rows.end())
       return;
   for(auto& c : cols)
       data[{r,c.first}] = R[c.first];
}

template <class T>
void table<T>::setCol(Col<T> C, col c) {
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
void table<T>::addRow(Row<T> R, row r, std::string name) {
   if(rows.find(r) != rows.end())
       return;
   for(auto& c : cols)
       data[{r,c.first}] = R[c.first];
   rows[r] = name;
}

template <class T>
void table<T>::addCol(Col<T> C, col c, std::string name) {
   if(cols.find(c) != cols.end())
       return;
   for(auto& r : rows)
       data[{r.first, c}] = C[r.first];
   cols[c] = name;
}


#endif /* TABLE_IMPL_H */

