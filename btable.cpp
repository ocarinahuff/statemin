/* 
 * File:   btable.cpp
 * Author: ocarinahuff
 * 
 * Created on December 8, 2015, 4:16 PM
 */

#include "btable.h"
#include "table.h"
#include <iostream>
#include <set>

using namespace std;

btable::btable() {
    clear();
}

btable::btable(Table<char> data, Hdr rows, Hdr cols, std::string title)
                                : table(data, rows, cols, title) {
    b.emplace(0,false);
    for(auto& c : cols)
        x.emplace(c.first,false);
}

void btable::init_solutions() {
    b.emplace(0,false);
    for(auto& c : this->getColHdr())
        x.emplace(c.first,false);
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
    print_table(INIT);
    //Sol xp = bcp(*this,x,b);
    print_x(bcp(*this,x,b),FINAL);
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
    int c = choose_column(Ap);
    xp[c] = true;
    btable A1 = select_column(Ap,c);
    print_A(A1,INTMED);
    Sol x1 = bcp(A1,xp,bp);
    if(cost(x1) < cost(bp)) {
        bp=x1;
        if(cost(bp) == L) {
            return bp;
        }
    }
    xp[c] = false;
    btable A0 = remove_column(Ap,c);
    Sol x0 = bcp(A0,xp,bp);
    if(cost(x0) < cost(bp))
        bp=x0;
    return bp;
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
    if(A.isempty())
        return false;
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
        if(count == 1)
            return true;
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
    if(r1 == r2 || A.isempty())
        return false;
    //btable Ap = A;
    for(auto& c : A.getColHdr()) {
        col C = c.first;
        char val1 = A.getElement(r1,C);
        char val2 = A.getElement(r2,C);
        if(val1 == '-')
            continue;
        if(val1 != val2)
            return false;
    }
    return true;
}

void btable::remove_dominating_rows(btable& A) {
    set<int> del;
    for(auto& r1 : A.getRowHdr()) {
        for(auto& r2 : A.getRowHdr()) {
            row row1 = r1.first, row2 = r2.first;
            if(row1 == row2)
                continue;
            if(check_row_dominance(row1,row2,A))
                del.emplace(row2);
                //A.delRow(row2);
        }
    }
    for(auto& r : del)
        A.delRow(r);
}

bool btable::check_column_dominance(col c1, col c2, const btable& A) {
    if(c1 == c2 || A.isempty())
        return false;
    //btable Ap = A;
    for(const auto& r : A.getRowHdr()) {
        row R = r.first;
        char val1 = A.getElement(R,c1);
        char val2 = A.getElement(R,c2);
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
    set<int> del;
    for(auto& c1 : A.getColHdr()) {
        for(auto& c2 : A.getColHdr()) {
            col col1 = c1.first, col2 = c2.first;
            if(col1 == col2)
                continue;
            if(check_column_dominance(col1, col2, A)) {
                x[col2] = false;
                del.emplace(col2);
            }
        }
    }
    for(auto& c : del)
        del_rows_col(c, '0', A);
}

int btable::lower_bound(const btable& A, const Sol& x) {
    btable Ac = A;
    btable MIS;
    MIS.clear();
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
    if(A.isempty())
        return;
    int shortest_row = 0, min_count = 32000, count;
    for(auto& r : A.getRowHdr()) {
        count = row_count(A.getRow(r.first));
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
    set<int> del;
    for(auto& rm : MIS.getRowHdr()) {
        for(auto& ra : A.getRowHdr()) {
            row r1 = rm.first, r2 = ra.first;
            if(check_row_intersect(MIS.getRow(r1), A.getRow(r2)))
                //A.delRow(r2);
                del.emplace(r2);
        }
    }
    for(auto& r : del)
        A.delRow(r);
}

bool btable::check_row_intersect(const Row<char>& R1, const Row<char>& R2) {
    Row<char>::const_iterator itr1, itr2;
    for(itr1 = R1.cbegin(), itr2 = R2.cbegin();
        itr1 != R1.cend() && itr2 != R2.cend();
        ++itr1,++itr2) {
        if(itr1->second == '1' && itr2->second == '1') {
            return true;
        }
    }
    return false;
}

bool btable::terminal_case1(const btable& A) {
    return A.isempty();
}

bool btable::terminal_case2(const btable& A) {
    for(const auto& r : A.getRowHdr())
        if(row_all_dashes(A.getRow(r.first)))
            return false;
    return false;
}

bool btable::row_all_dashes(const Row<char>& R) {
    for(auto& r : R)
        if(r.second != '-')
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

int btable::row_count(const Row<char>& R) {
    int count = 0;
    for(auto& r : R)
        if(r.second == '1')
            ++count;
    return count;
}

int btable::choose_column(const btable &A) {
    double max = 0.0;
    int col = 0;
    map<int,double> col_weights;
    map<int,double> row_weights;
    for(auto& r : A.getRowHdr()) {
        row_weights[r.first] = 1.0 / (double)row_count(A.getRow(r.first));
    }
    for(auto& c : A.getColHdr()) {
        double sum = 0.0;
        for(auto& cval : A.getCol(c.first)) {
            if(cval.second == '1') {
                sum += row_weights[cval.first];
            }
        }
        col_weights[c.first] = sum;
        if(sum > max) {
            max = sum;
            col = c.first;
        }
    }
    return col;
}

btable& btable::select_column(const btable &A, int col) {
    static btable A1 = A;
    del_rows_col(col, '1', A1);
    return A1;
}

btable& btable::remove_column(const btable &A, int col) {
    static btable A0 = A;
    del_rows_col(col, '0', A0);
    return A0;
}