/* 
 * File:   btable.h
 * Author: ocarinahuff
 *
 * Created on December 8, 2015, 4:16 PM
 */

#ifndef BTABLE_H
#define	BTABLE_H

class btable : public table<char> {
public:
    btable();
    btable(Table<char> data, Hdr rows, Hdr cols, std::string title) : table(data, rows, cols, title) {}
    btable(const btable& orig);
    virtual ~btable();
    void bcp();
    void print_table();
private:

};

#endif	/* BTABLE_H */

