/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   table.h
 * Author: ocarinahuff
 *
 * Created on November 15, 2015, 12:24 AM
 */

#ifndef TABLE_H
#define TABLE_H

#include "typedefs.h"

template <class T>
class table {
public:
    table();
    table(Table<T> data, Hdr rows, Hdr cols, std::string title);
    virtual ~table();
    Row<T> getRow(row r);
    Col<T> getCol(col c);
    T getElement(row r, col c);
    Table<T> getData();
    void setData(Table<T> data);
    std::string getTitle();
    void setTitle(std::string title);
    Hdr getRowHdr();
    void setRowHdr(Hdr rows);
    Hdr getColHdr();
    void setColHdr(Hdr cols);
    void delRow(row r);
    void delCol(col c);
    void setRow(Row<T> R, row r);
    void setCol(Col<T> C, col c);
    void setElement(T e, row r, col c);
    void addRow(Row<T> R, row r, std::string name);
    void addCol(Col<T> C, col c, std::string name);
private:
    Table<T> data;
    Hdr rows;
    Hdr cols;
    std::string title;
};

#include "table_impl.h"

#endif /* TABLE_H */