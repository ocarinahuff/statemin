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

/**
 * This class implements the table template for huffman tables.
 */
class hftable : public table<hentry> {
public:
    hftable(Table<hentry> data, Hdr rows, Hdr cols, std::string title) : table(data, rows, cols, title) {}
    void print_table();
    void print_pair_chart();
    void print_max_comp();
    void print_prime_comp();
    void print_reduced_key();
    void print_cp(const cp& c);
    
    void find_pairs();
    void reduce_pair_chart();
    
    void max_compatibles();
    
    void prime_compatibles();
    
    void solve_prime_bcp();
    
    void reduce_table();
    
private:
    // this holds the maximal compatibles.
    cpset M;
    // this holds the prime compatibles.
    std::map<int,cp> P;
    // this holds the pairs chart.
    std::map<cp,cpset> C;
    // this stores the results from bcp calculation.
    cp bcp_results;
    
    //generate pairs helpers
    // return true if two rows are output compatible. False otherwise.
    bool check_out_comp(const Row<hentry>& row1, const Row<hentry>& row2);
    // return true if two rows are unconditionally compatible. False otherwise.
    bool check_unc_comp(const Row<hentry>& row1, const Row<hentry>& row2, const cp& current_state);
    // determine the conditional compatibles for these two rows, add to pairs chart C.
    void get_cond_pairs(const Row<hentry>& row1, const Row<hentry>& row2, const cp& current_state);
    
    //max comp helpers    
    void check_intersectibles(int i, const cp& Si);
    bool subset(const cp& s1, const cp& s2);
    
    //prime comp helpers
    cpset& class_set(const cp& p);
    cpset& max_subsets(const cp& p);
    bool subset(const cpset& s1, const cpset& s2);
    
    //bcp helpers
    bool contains(int s, const cp& p);
};

#endif /* HFTABLE_H */

