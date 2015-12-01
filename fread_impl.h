/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fread_impl.h
 * Author: ocarinahuff
 *
 * Created on November 15, 2015, 4:44 AM
 */

#ifndef FREAD_IMPL_H
#define FREAD_IMPL_H

template <class T>
FREAD<T>::FREAD(std::string fname) {
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
                    data[{i,j++}] = {temp[0],temp[1][0]};
                } while(iss >> word);
                i++;
            }
        }
        myfile.close();
    }
}

template <class T>
Table<T> FREAD<T>::get_data() {
    return data;
}

template <class T>
Hdr FREAD<T>::get_states() {
    return states;
}

template <class T>
Hdr FREAD<T>::get_inputs() {
    return inputs;
}

#endif /* FREAD_IMPL_H */

