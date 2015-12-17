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
    if(argc == 1) {
        cout << "Please specify an input text file." << endl;
        cout << "Type /h as an input for more info." << endl;
        return -1;
    }
    if(argc > 2) {
        cout << "Too many input parameters, please specify only one." << endl;
        cout << "Type /h as an input for more info." << endl;
        return -1;
    }
    if(argc == 2 && argv[1] == "/h") {
        
        return 0;
    }
    // read text file.
    FREAD<hentry> txt_file(argv[1]);
    
    // get/create data components for huffman table object.
    Table<hentry> data = txt_file.get_data();
    Hdr states = txt_file.get_states();
    Hdr inputs = txt_file.get_inputs();
    string title = argv[1];
    title = title.substr(0,title.length()-4);
    
    // create huffman table object, call reduce function.
    hftable test(data,states,inputs,title);
    test.print_table();
    test.find_pairs();
    test.print_pair_chart();
    test.reduce_pair_chart();
    test.print_pair_chart();
    test.max_compatibles();
    test.print_max_comp();
    test.prime_compatibles();
    test.print_prime_comp();
    test.solve_prime_bcp();
    test.reduce_table();
    return 0;
}

