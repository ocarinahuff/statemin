/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fread.cpp
 * Author: ocarinahuff
 *
 * Created on November 15, 2015, 4:44 AM
 */

#include "fread.h"
#include <fstream>
#include <sstream>
#include <vector>

FREAD::FREAD(std::string fname) {
    std::string line, word;
    std::ifstream myfile(fname.c_str());
    if(myfile.is_open()) {
        int i = 1;
        while(std::getline(myfile,line)) {
            std::istringstream iss (line);
            iss >> word;
            if(!word.compare(".inputs")) {
                int j = 1;
                iss >> word;
                do {
                    inputs[j++] = word;
                } while(iss >> word);
            } else if(!word.compare(".states")) {
                for(int j = 1; iss >> word;)
                    states[j++] = word;
            }
            else if(!word.compare(".e")) {
                break;
            } else {
                int j = 1;
                do {
                    std::stringstream ss(word);
                    std::string token;
                    std::vector<std::string> temp;
                    while(std::getline(ss, token, ','))
                        temp.push_back(token);
                    for(auto& state : states) {
                        if(temp[0] == "-") {
                            data[{i,j++}] = {NextState::NOSTATE,temp[1][0]};
                            break;
                        }
                        if(state.second == temp[0]) {
                            data[{i,j++}] = {state.first,temp[1][0]};
                            break;
                        }
                    }
                } while(iss >> word);
                i++;
            }
        }
        myfile.close();
    }
}