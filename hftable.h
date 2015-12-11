/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hftable.h
 * Author: ocarinahuff
 *
 * Created on November 8, 2015, 3:18 AM
 */

#ifndef HFTABLE_H
#define HFTABLE_H

#include <string>
#include <set>
#include <map>
#include "typedefs.h"
#include "table.h"

class hftable : public table<hentry> {
public:
    hftable(Table<hentry> data, Hdr rows, Hdr cols, std::string title) : table(data, rows, cols, title) {}
    void print_table();
    void print_pair_chart();
    void print_max_comp();
    void print_prime_comp();
    
    void find_pairs();
    void reduce_pair_chart();
    
    void max_compatibles();
    
    void prime_compatibles();
    
    void solve_prime_bcp();
    
private:
    std::set<std::set<std::string> > M;
    std::map<int,std::set<std::string> > P;
    std::map<cp,cpset> C;
    
    //generate pairs helpers
    bool check_out_comp(const Row<hentry>& row1, const Row<hentry>& row2);
    bool check_unc_comp(const Row<hentry>& row1, const Row<hentry>& row2, const cp& current_state);
    void get_cond_pairs(const Row<hentry>& row1, const Row<hentry>& row2, const cp& current_state);
    bool contains_incompatible(const cpset& first, const cpset& second);
    
    //max comp helpers    
    void check_intersectibles(std::string i, const mc& Si);
    bool subset(const mc& s1, const mc& s2);
    
    //prime comp helpers
    cpset& class_set(const cp& p);
    cpset& max_subsets(const cp& p);
    bool subset(const cpset& s1, const cpset& s2);
    
    //bcp helpers
    bool contains(std::string s, const cp& p);
};

#endif /* HFTABLE_H */

