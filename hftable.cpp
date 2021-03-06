/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hftable.cpp
 * Author: ocarinahuff
 * 
 * Created on November 8, 2015, 3:18 AM
 */

#include "hftable.h"
#include "btable.h"
#include "typedefs.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace {
    // helps with formatting the hftable output.
    int max(Hdr test) {
        int max = 0;
        for(auto& x : test)
            if(x.second.length() > max)
                max = x.second.length();
        return max;
    }
    int max(Table<hentry> test) {
        int max = 0;
        for(auto& x : test) {
            int slength = to_string(x.second.next_state).length() + 3;
            if(slength > max)
                max = slength;
        }
        return max;
    }
}

void hftable::print_table() {
    cout << "Huffman flow table: " << getTitle() << endl;
    // preprocessing, find max string length of states, inputs, entries.
    int slen, ilen, dlen;
    slen = max(getRowHdr());
    ilen = max(getColHdr());
    dlen = max(getData());
    string s(slen,' ');
    cout << s;
    string s1(dlen-ilen,' ');
    for(auto& x : getColHdr())
        cout << s1 << x.second;
    cout << endl;
    // print current state, followed by row of possible next states.
    for(auto& x : getRowHdr()) {
        string s2(slen-x.second.length(),' ');
        cout << x.second << s2;
        for(auto& y : getColHdr()) {
            hentry e = getElement(x.first,y.first);
            string next_state = (e.next_state == 0 ? "-" : getRowHdr()[e.next_state]) + ',' + e.output;
            string s3(dlen-next_state.length(),' ');
            cout << s3 << next_state;
        }
        cout << endl;
    }
}

void hftable::print_reduced_key(map<int,cp>& P, cp& bcp_results) {
    cout << "Reduced state key:" << endl;
    for(auto& key : bcp_results) {
        cout << key << " -> ";
        for(auto& p : P[key]) {
            cout << getRowHdr()[p];
        }
        cout << endl;
    }
}

bool hftable::check_out_comp(const MSet<hentry>& row1, const MSet<hentry>& row2) {
    MSet<hentry>::const_iterator it1 = row1.begin(), it2 = row2.begin();
    for(;it1 != row1.end() && it2 != row2.end();it1++,it2++) {
        char out1 = it1->second.output;
        char out2 = it2->second.output;
        if(out1 == '-' || out2 == '-')
            continue;
        if(out1 != out2)
            return false;
    }
    return true;
}

bool hftable::check_unc_comp(const MSet<hentry>& row1, const MSet<hentry>& row2, const cp& current_state) {
    MSet<hentry>::const_iterator it1 = row1.begin(), it2 = row2.begin();
    for(;it1 != row1.end() && it2 != row2.end();it1++,it2++) {
        int nstate1 = it1->second.next_state;
        int nstate2 = it2->second.next_state;
        cp next_state = {nstate1,nstate2};
        if(nstate1 == NextState::NOSTATE || nstate2 == NextState::NOSTATE)
            continue;
        if(nstate1 != nstate2)
            if(current_state == next_state)
                continue;
            else
                return false;
    }
    return true;
}

void hftable::find_pairs(map<cp,cpset>& C) {
    Hdr rows = getRowHdr();
    Hdr::iterator it1, it2;
    cpset state_set;
    cp state_pair, current_state, next_state;
    MSet<hentry> row1, row2;
    MSet<hentry>::iterator itr1, itr2;
    for(it1 = rows.begin(); it1 != --(rows.end()); ++it1) {
        it2 = it1;
        for(++it2; it2 != rows.end(); ++it2) {
            row1 = getRow(it1->first);
            row2 = getRow(it2->first);
            current_state = {it1->first,it2->first};
            state_set.clear();
            state_pair.clear();
            next_state.clear();
            if(check_out_comp(row1,row2))
                if(check_unc_comp(row1,row2,current_state)) {
                    state_pair.emplace(PairState::UNCONDITIONAL);
                    state_set.emplace(state_pair);
                } else {
                    for(itr1 = row1.begin(), itr2 = row2.begin();
                        itr1 != row1.end() && itr2 != row2.end();
                        ++itr1,++itr2) {
                        int nstate1 = itr1->second.next_state;
                        int nstate2 = itr2->second.next_state;
                        if(nstate1 == NextState::NOSTATE || 
                           nstate2 == NextState::NOSTATE || 
                           nstate1 == nstate2)
                            continue;
                        else {
                            next_state = {nstate1,nstate2};
                        }
                        if(current_state != next_state)
                            state_set.emplace(next_state);
                    }
                }
            else {
                state_pair.emplace(PairState::INCOMPATIBLE);
                state_set.emplace(state_pair);
            }
            C[current_state] = state_set;
        }
    }
}

void hftable::reduce_pair_chart(map<cp,cpset>& C) {
    bool recheck;
    int state;
    cpset state_inc, mark_inc;
    cp inc;
    inc.emplace(PairState::INCOMPATIBLE);
    state_inc.emplace(inc);
    do {
        recheck = false;
        mark_inc.clear();
        for(auto& c : C) {
            state = *(c.second.begin()->begin());
            if(state == PairState::INCOMPATIBLE ||
               state == PairState::UNCONDITIONAL)
                continue;
            for(auto& cps : c.second) {
                state = *(C[cps].begin()->begin());
                if(state == PairState::INCOMPATIBLE) {
                    mark_inc.insert(c.first);
                    recheck = true;
                }
            }
        }
        if(recheck)
            for(auto& m : mark_inc)
                C[m] = state_inc;
    } while(recheck);
}

bool hftable::subset(const cp& s1, const cp& s2) {
    cp done = s2;
    for(const auto& s : s1) {
        for(const auto& t : s2) {
            if(s == t) {
                done.erase(s);
            }
        }
    }
    return done.empty();
}

bool hftable::subset(const cpset& s1, const cpset& s2) {
    cpset done = s2;
    for(const auto& s : s1) {
        for(const auto& t : s2) {
            if(subset(s,t)) {
                done.erase(s);
            }
        }
    }
    return done.empty();
}

void hftable::check_intersectibles(int i, const cp& Si, cpset& M) {
    cp tmp, rem;
    bool found = false;
    for(auto& cl : M) {
        tmp.clear();
        tmp.insert(i);
        rem = Si;
        for(auto& s : Si) {
            for(auto& c : cl) {
                if(s == c) {
                    tmp.insert(s);
                    rem.erase(s);
                    break;
                }
            }
        }
        // if more than one intersectible found, add to M.
        // size is number of intersectibles, plus i.
        if(tmp.size() > 2) {
            M.insert(tmp);
            found = true;
        }
    }
    // add remaining non-intersectibles.
    if(!found)
        for(auto& s : Si)
            M.insert({i,s});
    if(!rem.empty())
        for(auto& r : rem)
            M.insert({i,r});
    // remove subsets.
    cpset del_set;
    for(auto& c1 : M) {
        for(auto& c2 : M) {
            if(c1 == c2)
                continue;
            if(subset(c1,c2)) {
                del_set.emplace(c2);
            }
        }
    }
    for(auto& del : del_set) {
        M.erase(del);
    }
}

void hftable::max_compatibles(const map<cp,cpset>& C, cpset& M) {
    Hdr states = getRowHdr();
    Hdr::reverse_iterator it1;
    Hdr::iterator it2;
    set<int> Si;
    int i;
    for(it1 = states.rbegin(), it1++; it1 != states.rend(); it1++) {
        i = it1->first;
        Si.clear();
        for(it2 = it1.base(); it2 != states.end(); it2++) {
            // form a set of states for c-list checking.
            cp pair = {it1->first,it2->first};
            int state = *(C.at(pair).begin()->begin());
            if(state != PairState::INCOMPATIBLE) {
                Si.insert(it2->first);
            }
        }
        if(Si.empty())
            continue;
        if(!Si.empty() && M.empty()) {
            cp tmp;
            tmp.insert(i);
            for(auto& s : Si) {
                tmp.insert(s);
            }
            M.insert(tmp);
        } else {
            check_intersectibles(i,Si,M);
        }
    }
}

void hftable::prime_compatibles(const cpset& M, const map<cp,cpset>& C, map<int,cp>& P) {
    cpset done;
    bool prime = false;
    int index = 1;
    done.clear();
    int max_size = 0;
    for(auto& m : M)
        if(m.size() > max_size)
            max_size = m.size();
    for(int k = max_size; k >= 1; --k) {
        for(auto& m : M)
            if(m.size() == k)
                P.emplace(index++,m);
        for(auto& p : P) {
            if(p.second.size() == k) {
                cpset cs;
                cs.clear();
                //check if class set is empty.
                if(class_set(p.second, C).empty())
                    continue;
                for(auto& s : max_subsets(p.second)) {
                    if(done.find(s) != done.end()) {
                        continue;
                    }
                    cs = class_set(s, C);
                    prime = true;
                    map<int,cp>::iterator it1;
                    for(it1 = P.begin(); it1 != P.end(); ++it1) {
                        if(it1->second.size() >= k) {
                            if(subset(it1->second,s)) {
                                cpset cq = class_set(it1->second, C);
                                if(subset(cs,cq)) {
                                    prime = false;
                                    break;
                                }
                            }
                        }
                    }
                    if(prime) {
                        P.emplace(index++,s);
                        done.emplace(s);
                    }
                }
            }
        }
    }
}

cpset& hftable::max_subsets(const cp& p) {
    static cpset ms;
    ms.clear();
    cp::const_iterator it1, it2;
    if(p.size() == 2) {
        cp s1 = {*(p.begin())}, s2 = {*(p.rbegin())};
        ms.emplace(s1);
        ms.emplace(s2);
        return ms;
    }
    for(it1 = p.cbegin(); it1 != p.crbegin().base(); ++it1) {
        for(it2 = it1,++it2; it2 != p.cend(); ++it2) {
            cp pair;
            pair.emplace(*it1);
            pair.emplace(*it2);
            ms.insert(pair);
        }
    }
    return ms;
}

cpset& hftable::class_set(const cp& p, const map<cp,cpset>& C) {
    // create pairs from p.
    static cpset cs;
    cs.clear();
    if(p.size() == 1)
        return cs;
    cp::const_iterator it1, it2;
    for(it1 = p.cbegin(); it1 != p.crbegin().base(); ++it1) {
        for(it2 = it1,++it2; it2 != p.cend(); ++it2) {
            cp pair;
            pair.emplace(*it1);
            pair.emplace(*it2);
            for(auto& c : C.at(pair)) {
                int stemp = *(c.begin());
                if(stemp == PairState::UNCONDITIONAL || stemp == PairState::INCOMPATIBLE) {
                    continue;
                }
                if(!subset(p,c)) {
                    cs.emplace(c);
                }
            }
        }
    }
    return cs;
}

void hftable::solve_prime_bcp(const map<int,cp>& P, const map<cp,cpset>& C, cp& bcp_results) {
    btable btbl;
    Hdr cols;
    MSet<char> R;
    btbl.setTitle("Prime Compatible covering");
    row r = 1;
    for(col c = 1; c <= P.size(); ++c) {
        cols[c] = "c" + std::to_string(c);
    }
    btbl.setColHdr(cols);
    btbl.init_solutions();
    // scan each prime, for each possible state.
    for(auto& s : this->getRowHdr()) {
        for(auto& p : P) {
            if(contains(s.first, p.second)) {
                R[p.first] = '1';
            } else {
                R[p.first] = '-';
            }
        }
        btbl.addRow(R, r, "r" + to_string(r));
        ++r;
    }
    // scan each prime's class set for additional coverings.
    bool found;
    for(auto& p1 : P) {
        R[p1.first] = '0';
        for(auto& cs : class_set(p1.second,C)) {
            found = false;
            for(auto& p2 : P) {
                if(subset(p2.second,cs)) {
                    found = true;
                    R[p2.first] = '1';
                } else {
                    if(p1.first != p2.first)
                        R[p2.first] = '-';
                }
            }
            if(found) {
                btbl.addRow(R, r, "r" + std::to_string(r));
                ++r;
            }
        }
    }
    btbl.bcp(bcp_results);
}

bool hftable::contains(int state, const cp& state_pair) {
    for(auto& _state : state_pair) {
        if(state == _state) {
            return true;
        }
    }
    return false;
}

void hftable::reduce_table(map<int,cp>& P, cp& bcp_results) {
    Table<hentry> reduced_data;
    Hdr new_rows;
    for(auto& rw : bcp_results) {
        new_rows.emplace(rw, to_string(rw));
        for(auto& cl : getColHdr()) {
            char output = '-';
            cp next_state;
            next_state.emplace(NextState::NOSTATE);
            for(auto& old_row : P[rw]) {
                char output_temp = getElement(old_row,cl.first).output;
                if(output_temp != '-')
                    output = output_temp;
                int next_state_temp = getElement(old_row,cl.first).next_state;
                if(next_state_temp != NextState::NOSTATE) {
                    next_state.erase(NextState::NOSTATE);
                    next_state.emplace(next_state_temp);
                }
            }
            cell Cell = {rw,cl.first};
            if(next_state.find(NextState::NOSTATE) != next_state.end()) {
                hentry Hentry = {NextState::NOSTATE,output};
                reduced_data.emplace(Cell,Hentry);
            }
            else {
                for(auto& rw1 : bcp_results) {
                    if(subset(P[rw1],next_state)) {
                        hentry Hentry = {rw1,output};
                        reduced_data.emplace(Cell,Hentry);
                        break;
                    }
                }
            }
        }
    }
    hftable reduced_table(reduced_data,new_rows,getColHdr(),"Reduced " + getTitle());
    reduced_table.print_table();
    print_reduced_key(P,bcp_results);
}

void hftable::reduce() {
    map<cp,cpset> C;
    cpset M;
    map<int,cp> P;
    cp bcp_results;
    find_pairs(C);
    reduce_pair_chart(C);
    max_compatibles(C,M);
    prime_compatibles(M,C,P);
    solve_prime_bcp(P,C,bcp_results);
    reduce_table(P,bcp_results);
}