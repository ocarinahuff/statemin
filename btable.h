/* 
 * File:   btable.h
 * Author: ocarinahuff
 *
 * Created on December 8, 2015, 4:16 PM
 */

#ifndef BTABLE_H
#define	BTABLE_H

#include "typedefs.h"
#include "table.h"

class btable : public table<char> {
public:
    btable();
    btable(Table<char> data, Hdr rows, Hdr cols, std::string title) : table(data, rows, cols, title) {}
    void bcp();
    void print_table(State s);
private:
    Sol x, b;
    
    // printer functions
    void print_A(const btable &A, State s);
    void print_x(const Sol &x, State s);
    
    // deletes rows/cols from A where col has value val
    void del_rows_col(col c, char val, btable& A);

    // primary bcp function
    Sol& bcp(const btable& A, const Sol& x, const Sol& b);

    // sub-algorithm: reduce
    void reduce(btable& A, Sol& x);
    // helper functions for reduce
    bool find_essential_row(const btable& A, col& Col, char& val);
    void remove_essential_rows(btable& A, Sol& x);
    bool check_row_dominance(row r1, row r2, const btable& A);
    void remove_dominating_rows(btable& A);
    bool check_column_dominance(col c1, col c2, const btable& A);
    void remove_dominated_columns(btable& A, Sol& x);

//    // general helper functions.
    int cost(const Sol &sol);
    int row_count(row r, const btable& A);

    // sub-algorithm: lower_bound
    int lower_bound(const btable &A, const Sol &x);
    // helper functions for lower_bound
    void shortest_row(btable &A, btable &MIS);
    void delete_intersect_rows(btable &A, const btable &MIS);
    bool check_row_intersect(row r1, row r2, const btable &MIS, const btable& A);

    // functions for terminal case
    bool terminal_case1(const btable& A);
    bool terminal_case2(const btable& A);
    bool row_all_dashes(row r, const btable& A);
//
//    // functions for branching
//    int choose_column(const mapBTBL &A);
//    mapBTBL& select_column(const mapBTBL &A, int c);
//    mapBTBL& remove_column(const mapBTBL &A, int c);
};

#endif	/* BTABLE_H */

