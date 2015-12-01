/* 
 * File:   main.cpp
 * Author: ocarinahuff
 *
 * Created on November 7, 2015, 10:40 AM
 */

#include <cstdlib>
#include "fread.h"
#include "table.h"
#include "typedefs.h"
#include "hftable.h"
#include <string>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    FREAD<hentry> txt_file(argv[1]);
    Table<hentry> data = txt_file.get_data();
    Hdr states = txt_file.get_states();
    Hdr inputs = txt_file.get_inputs();
    string title = argv[1];
    title = title.substr(0,title.length()-4);
    hftable test(data,states,inputs,title);
    test.print_table();
    test.find_pairs();
    test.print_pair_chart();
    test.reduce_pair_chart();
    test.print_pair_chart();
    test.max_compatibles();
    return 0;
}

