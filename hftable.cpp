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
#include "typedefs.h"
#include <iostream>

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
    // used for debugging.
    void print_pair(cp pair) {
        cout << *pair.begin();
        if(pair.size() > 1)
            cout << ',' << *pair.rbegin();
        cout << endl;
    }
}

void hftable::print_table() {
    cout << "Huffman flow table: " << getTitle() << endl;
    // preprocessing, find max string length of states, inputs, entries.
    int slen, ilen, elen;
    slen = max(getRowHdr());
    ilen = max(getColHdr());
    elen = slen + 2;
    string s(slen+2,' ');
    cout << s;
    string s1(elen-ilen+2,' ');
    for(auto& x : getColHdr())
        cout << x.second << s1;
    cout << endl;
    // print current state, followed by row of possible next states.
    for(auto& x : getRowHdr()) {
        cout << x.second << "  ";
        for(auto& y : getColHdr()) {
            hentry e = getElement(x.first,y.first);
            cout << e.next_state << ',' << e.output << "  ";
        }
        cout << endl;
    }
}

void hftable::print_pair_chart() {
    cout << "Pairs chart" << endl;
    for(auto& c : C) {
        cout << *(c.first.begin()) << ',' << *(c.first.rbegin()) << ": ";
        for(auto& cps : c.second) {
            cout << *(cps.begin());
            if(cps.size() == 1)
                break;
            else
                cout << ',' << *(cps.rbegin()) << ' ';
        }
        cout << endl;
    }
}

void hftable::print_max_comp() {
    cout << "Max compatibles:" << endl;
    for(auto& m : M) {
        for(auto& s : m) {
            cout << s;
        }
        cout << endl;
    }
}

void hftable::print_prime_comp() {
    
}

bool hftable::check_out_comp(const Row<hentry>& row1, const Row<hentry>& row2) {
    Row<hentry>::const_iterator it1 = row1.begin(), it2 = row2.begin();
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

bool hftable::check_unc_comp(const Row<hentry>& row1, const Row<hentry>& row2, const cp& current_state) {
    Row<hentry>::const_iterator it1 = row1.begin(), it2 = row2.begin();
    for(;it1 != row1.end() && it2 != row2.end();it1++,it2++) {
        string nstate1 = it1->second.next_state;
        string nstate2 = it2->second.next_state;
        cp next_state = {nstate1,nstate2};
        if(!nstate1.compare("-") || !nstate2.compare("-"))
            continue;
        if(!!nstate1.compare(nstate2))
            if(current_state == next_state)
                continue;
            else
                return false;
    }
    return true;
}

bool hftable::contains_incompatible(const cpset& first, const cpset& second) {
    for(const auto& f : first)
        for(const auto& s : second)
            if(f == s)
                return true;
    return false;
}

void hftable::find_pairs() {
    Hdr rows = getRowHdr();
    Hdr::iterator it1, it2;
    cpset state_set;
    cp state_pair, current_state, next_state;
    Row<hentry> row1, row2;
    Row<hentry>::iterator itr1, itr2;
    for(it1 = rows.begin(); it1 != rows.end(); ++it1) {
        for(++(it2 = it1); it2 != rows.end(); ++it2) {
            row1 = getRow(it1->first);
            row2 = getRow(it2->first);
            current_state = {it1->second,it2->second};
            state_set.clear();
            state_pair.clear();
            next_state.clear();
            if(check_out_comp(row1,row2))
                if(check_unc_comp(row1,row2,current_state)) {
                    state_pair.emplace("unconditional");
                    state_set.emplace(state_pair);
                } else {
                    for(itr1 = row1.begin(), itr2 = row2.begin();
                        itr1 != row1.end() && itr2 != row2.end();
                        ++itr1,++itr2) {
                        string nstate1 = itr1->second.next_state;
                        string nstate2 = itr2->second.next_state;
                        if(!nstate1.compare("-") || !nstate2.compare("-") || !nstate1.compare(nstate2))
                            continue;
                        else
                            next_state = {nstate1,nstate2};
                        if(current_state != next_state)
                            state_set.emplace(next_state);
                    }
                }
            else {
                state_pair.emplace("incompatible");
                state_set.emplace(state_pair);
            }
            C[current_state] = state_set;
        }
    }
}

void hftable::reduce_pair_chart() {
    bool recheck;
    string state;
    cpset state_inc, mark_inc;
    cp inc;
    inc.emplace("incompatible");
    state_inc.emplace(inc);
    do {
        recheck = false;
        mark_inc.clear();
        for(auto& c : C) {
            state = *(c.second.begin()->begin());
            if(!state.compare("incompatible") || !state.compare("unconditional"))
                continue;
            for(auto& cps : c.second) {
                state = *(C[cps].begin()->begin());
                if(!state.compare("incompatible")) {
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

bool hftable::subset(const mc& s1, const mc& s2) {
    mc done = s2;
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

void hftable::check_intersectibles(string i, const mc& Si) {
    mc tmp, rem;
    bool found = false;
    for(auto& cl : M) {
        tmp.clear();
        tmp.insert(i);
        for(auto& s : Si) {
            for(auto& c : cl) {
                if(!s.compare(c)) {
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
    // remove subsets.
    for(auto& c1 : M) {
        for(auto& c2 : M) {
            if(c1 == c2)
                continue;
            if(subset(c1,c2)) {
                M.erase(c2);
                break;
            }
        }
    }
    // add remaining non-intersectibles.
    if(!found)
        for(auto& s : Si)
            M.insert({i,s});
    else if(!rem.empty())
        for(auto& r : rem)
            M.insert({i,r});
}

void hftable::max_compatibles() {
    Hdr states = getRowHdr();
    Hdr::reverse_iterator it1;
    Hdr::iterator it2;
    set<string> Si;
    string i;
    for(it1 = states.rbegin(), it1++; it1 != states.rend(); it1++) {
        i = it1->second;
        Si.clear();
        for(it2 = it1.base(); it2 != states.end(); it2++) {
            // form a set of states for c-list checking.
            cp tmp = {it1->second,it2->second};
            string state = *(C[tmp].begin()->begin());
            if(state.compare("incompatible")) {
                Si.insert(it2->second);
            }
        }
        if(M.empty()) {
            set<string> tmp;
            tmp.insert(i);
            for(auto& s : Si) {
                tmp.insert(s);
            }
            M.insert(tmp);
        } else {
            check_intersectibles(i,Si);
        }
    }
}

void hftable::prime_compatibles() {
    cpset done;
    bool prime = false;
    done.clear();
    int max_size = 0;
    for(auto& m : M)
        if(m.size() > max_size)
            max_size = m.size();
    for(int k = max_size; k >= 1; --k) {
        for(auto& m : M)
            if(m.size() == k)
                P.insert(m);
        for(auto& p : P) {
            if(p.size() == k) {
                cpset cs;
                cs.clear();
                //check if class set is empty.
                if(class_set(p).empty())
                    continue;
                for(auto& s : max_subsets(p)) {
                    if(done.find(s) != done.end()) {
                        continue;
                    }
                    cs = class_set(p);
                    prime = true;
                    cpset::iterator it1;
                    for(it1 = P.begin(); it1 != P.end(); ++it1) {
                        if(it1->size() >= k) {
                            if(subset(*it1,s)) {
                                cpset cq = class_set(*it1);
                                if(subset(cs,cq)) {
                                    prime = false;
                                    break;
                                }
                            }
                        }
                    }
                    if(prime) {
                        P.emplace(s);
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

cpset& hftable::class_set(const cp& p) {
    // create pairs from p.
    static cpset cs;
    cs.clear();
    cp::const_iterator it1, it2;
    for(it1 = p.cbegin(); it1 != p.crbegin().base(); ++it1) {
        for(it2 = it1,++it2; it2 != p.cend(); ++it2) {
            cp pair;
            pair.emplace(*it1);
            pair.emplace(*it2);
            for(auto& c : C[pair]) {
                string stemp = *(c.begin());
                if(!stemp.compare("unconditional") || !stemp.compare("incompatible")) {
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
/*
 * prime_compatibles(C,M){
 *  done = 0;  Initialize already computed set.
 *  for(k = |largest(M)|;k>=1;k--)   Loop largest to smallest.
 *      foreach(q in M; |q| = k) enqueue(P,q);  Queue all of size k.
 *      foreach(p in P;|p| = k) {
 *          if(class_set(C,p) = 0) then continue;  If empty, skip.
 *          foreach(s in max_subsets(p)) { check all maximal subsets.
 *              if(s in done) then continue;  if computed, skip.
 *              Ts = class_set(C,s);   Find subset's class set.
 *              prime = true;           Init prime as true.
 *              foreach(q in P;|q} > k) {   Check all larger primes.
 *                  if(s subset q) then {
 *                      Tq = class_set(C,q);
 *                      if(Tq subset or equal Ts) then {
 *                          prime = false;
 *                          break;
 *                      }
 *                  }
 *              }
 *              if(prime = 1) then enqueue(P,s);
 *              done = done union {s};
 *          }
 *      }
 *  }
 * return(P);
 * }
 */