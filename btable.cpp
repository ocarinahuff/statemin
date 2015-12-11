/* 
 * File:   btable.cpp
 * Author: ocarinahuff
 * 
 * Created on December 8, 2015, 4:16 PM
 */

#include "btable.h"
#include "table.h"
#include <iostream>

using namespace std;

btable::btable() {
    clear();
}

void btable::print_table(State s) {
    switch(s) {
    case INIT:
        cout << "Initial ";
        break;
    case INTMED:
        cout << "Intermediate ";
        break;
    case FINAL:
        cout << "Final ";
        break;
    }
    cout << "state of " << getTitle() << " table:" << endl;
    if(isempty()) {
        cout << "Table is empty!" << endl;
        return;
    }	

    for(const auto& c : getColHdr())
        cout << c.second << ((c.first < 10)?" ":"");
    cout << endl;

    for(const auto& r : getRowHdr()) {
        for(const auto& c : getColHdr())
            cout << getElement(r.first,c.first) << "  ";
        cout << r.second << endl;
    }
}

void btable::print_A(const btable& A, State s) {
    btable Ap = A;
    switch(s) {
    case INIT:
        cout << "Initial ";
        break;
    case INTMED:
        cout << "Intermediate ";
        break;
    case FINAL:
        cout << "Final ";
        break;
    }
    cout << "state of " << Ap.getTitle() << " table:" << endl;
    if(Ap.isempty()) {
        cout << "Table is empty!" << endl;
        return;
    }	

    for(const auto& c : Ap.getColHdr())
        cout << c.second << ((c.first < 10)?" ":"");
    cout << endl;

    for(const auto& r : Ap.getRowHdr()) {
        for(auto& c : Ap.getColHdr())
            cout << Ap.getElement(r.first,c.first) << "  ";
        cout << r.second << endl;
    }
}

void btable::print_x(const Sol& x, State s) {
    if(x.find(0) != x.end()) {
        cout << "No solution!" << endl;
        return;
    }
    switch(s) {
    case INIT:
        cout << "Initial ";
        break;
    case INTMED:
        cout << "Intermediate ";
        break;
    case FINAL:
        cout << "Final ";
        break;
    }
    cout << "solution is:" << endl;
    cout << "{";
    int count = 0;
    for(auto& c : x) {
        if(c.second) {
            ++count;
            if(count > 1)
                cout << ",";
            cout << getColHdr()[c.first];
        }
    }
    cout << "}" << endl;
}

void btable::del_rows_col(col c, char val, btable& A) {
    for(auto& r : A.getRowHdr())
        if(A.getElement(r.first,c) == val)
            A.delRow(r.first);
    A.delCol(c);
}

void btable::bcp() {
    //print_A(this,INIT);
    //Sol xp = bcp(this,x,b);
    //print_x(xp,FINAL);
}
    
Sol& btable::bcp(const btable& A, const Sol& x, const Sol& b) {
    btable Ap = A;
    static Sol xp = x;
    static Sol bp = b;
    reduce(Ap,xp);
    int L = lower_bound(Ap,xp);
    if(L >= cost(bp)) {
        return bp;
    }
    if(terminal_case1(Ap)) {
        return xp;
    }
    else if(terminal_case2(Ap)) {
        return bp;
    }
//    int c = choose_column(Ap);
//    xp[c] = true;
//    btbl A1 = select_column(Ap,c);
//    mapSOL x1 = bcp(A1,xp,bp);
//    if(cost(x1) < cost(bp)) {
//        bp=x1;
//        if(cost(bp) == L) {
//            return bp;
//        }
//    }
//    xp[c] = false;
//    btbl A0 = remove_column(Ap,c);
//    mapSOL x0 = bcp(A0,xp,bp);
//    if(cost(x0) < cost(bp))
//        bp=x0;
//    return bp;
}

void btable::reduce(btable& A, Sol& x) {
    btable Ap;
    do {
        Ap = A;
        remove_essential_rows(A,x);
        remove_dominating_rows(A);
        remove_dominated_columns(A,x);
    } while(!A.isempty() && A.getData() != Ap.getData());
}

bool btable::find_essential_row(const btable& A, col& Col, char& val) {
    btable Ap = A;
    int count = 0;
    char check_val;
    for(auto& r : Ap.getRowHdr()) {
        for(auto& c : Ap.getColHdr()) {
            check_val = Ap.getElement(r.first,c.first);
            if(check_val != '-') {
                ++count;
                Col = c.first;
                val = check_val;
            }
        }
        if(count == 1) return true;
    }
    return false;
}

void btable::remove_essential_rows(btable& A, Sol& x) {
    col C;
    char val;
    while(find_essential_row(A, C, val)) {
        x[C] = val == '1';
        del_rows_col(C, val, A);
    }
}

bool btable::check_row_dominance(row r1, row r2, const btable& A) {
    btable Ap = A;
    for(auto& c : Ap.getColHdr()) {
        col C = c.first;
        char val1 = Ap.getElement(r1,C);
        char val2 = Ap.getElement(r2,C);
        if(val1 == '-')
            continue;
        if(val1 != val2)
            return false;
    }
    return true;
}

void btable::remove_dominating_rows(btable& A) {
    for(auto& r1 : A.getRowHdr()) {
        for(auto& r2 : A.getRowHdr()) {
            row row1 = r1.first, row2 = r2.first;
            if(row1 == row2)
                continue;
            if(check_row_dominance(row1,row2,A))
                A.delRow(row2);
        }
    }
}

bool btable::check_column_dominance(col c1, col c2, const btable& A) {
    btable Ap = A;
    for(const auto& r : Ap.getRowHdr()) {
        row R = r.first;
        char val1 = Ap.getElement(R,c1);
        char val2 = Ap.getElement(R,c2);
        if(val1 == '1')
            continue;
        if(val1 == '-' && val2 == '1')
            return false;
        if(val1 == '0' && val2 != '0')
            return false;
    }
    return true;
}

void btable::remove_dominated_columns(btable& A, Sol& x) {
    for(auto& c1 : A.getColHdr()) {
        for(auto& c2 : A.getColHdr()) {
            col col1 = c1.first, col2 = c2.first;
            if(col1 == col2)
                continue;
            if(check_column_dominance(col1, col2, A)) {
                x[col2] = false;
                del_rows_col(col2, '0', A);
            }
        }
    }
}

int btable::lower_bound(const btable& A, const Sol& x) {
    btable Ac = A;
    btable MIS;
    //MIS.data.clear();
    MIS.setColHdr(Ac.getColHdr());
    for(auto& r : Ac.getRowHdr()) {
        row rw = r.first;
        for(auto& c : Ac.getColHdr()) {
            col cw = c.first;
            if(Ac.getElement(rw,cw) == '0') {
                Ac.delRow(rw);
                break;
            }
        }
    }
    do {
        shortest_row(Ac,MIS);
        delete_intersect_rows(Ac,MIS);
    } while(!Ac.isempty());
    return MIS.getRowHdr().size() + cost(x);
}

void btable::shortest_row(btable& A, btable& MIS) {
    int shortest_row = 0, min_count = 32000, count;
    for(auto& r : A.getRowHdr()) {
        count = row_count(r.first,A);
        if(count < min_count) {
            shortest_row = r.first;
            min_count = count;
        }
    }
    MIS.addRow(A.getRow(shortest_row), shortest_row, A.getRowHdr()[shortest_row]);
    A.delRow(shortest_row);
}

void btable::delete_intersect_rows(btable& A, const btable& MIS) {
    if(A.isempty())
        return;
    for(auto& rm : MIS.getRowHdr()) {
        for(auto& ra : A.getRowHdr()) {
            row r1 = rm.first, r2 = ra.first;
            if(check_row_intersect(r1, r2, MIS, A))
                A.delRow(r2);
        }
    }
}

bool btable::check_row_intersect(row r1, row r2, 
                                 const btable& MIS, const btable& A) {
    for(auto& c : A.getColHdr())
        if(MIS.getElement(r1,c.first) == '1' && 
           A.getElement(r2,c.first) == '1')
            return true;
    return false;
}

bool btable::terminal_case1(const btable& A) {
    return A.isempty();
}

bool btable::terminal_case2(const btable& A) {
    for(const auto& r : A.getRowHdr())
        if(row_all_dashes(r.first, A))
            return false;
    return false;
}

bool btable::row_all_dashes(row r, const btable& A) {
    for(const auto& c : A.getColHdr())
        if(A.getElement(r,c.first) != '-')
            return false;
    return true;
}

int btable::cost(const Sol& sol) {
    if(sol.find(0) != sol.end()) return 2^32 - 1;
    int count = 0;
    for(auto& s : sol)
        if(s.second)
            ++count;
    return count;
}

int btable::row_count(row r, const btable& A) {
    int count = 0;
    for(const auto& c : A.getColHdr())
        if(A.getElement(r,c.first) != '-')
            ++count;
    return count;
}