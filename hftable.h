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
    void reduce();
private:
    void reduce_table(std::map<int,cp>& P, cp& bcp_results);
    void print_reduced_key(std::map<int,cp>& P, cp& bcp_results);
    
    //generate pairs
    void find_pairs(std::map<cp,cpset>& C);
    void reduce_pair_chart(std::map<cp,cpset>& C);
    // return true if two rows are output compatible. False otherwise.
    bool check_out_comp(const MSet<hentry>& row1, const MSet<hentry>& row2);
    // return true if two rows are unconditionally compatible. False otherwise.
    bool check_unc_comp(const MSet<hentry>& row1, const MSet<hentry>& row2, const cp& current_state);
    // determine the conditional compatibles for these two rows, add to pairs chart C.
    void get_cond_pairs(const MSet<hentry>& row1, const MSet<hentry>& row2, const cp& current_state);
    
    //max comp
    void max_compatibles(const std::map<cp,cpset>& C, cpset& M);
    void check_intersectibles(int i, const cp& Si, cpset& M);
    bool subset(const cp& s1, const cp& s2);
    
    //prime comp
    void prime_compatibles(const cpset& M, const std::map<cp,cpset>& C, std::map<int,cp>& P);
    cpset& class_set(const cp& p, const std::map<cp,cpset>& C);
    cpset& max_subsets(const cp& p);
    bool subset(const cpset& s1, const cpset& s2);
    
    //bcp
    void solve_prime_bcp(const std::map<int,cp>& P, const std::map<cp,cpset>& C, cp& bcp_results);
    bool contains(int s, const cp& p);
};

#endif /* HFTABLE_H */

