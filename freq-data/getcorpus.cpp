/*
 * Copyright (C) 2013  Ben Au <benau2006 at yahoo dot com dot hk>
 *
 * This file is part of libcangjie.
 *
 * libcangjie is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcangjie is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libcangjie.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char* argv[]) {

    const char* input = argv[1];

    string line;
    ifstream myfile (input);
    if (myfile.is_open()) {
        while ( getline (myfile,line)) {
            int i =0;
            while (i<line.length()){
                if (line[i]==(char)0xE4 |line[i]==(char)0xE5 |  //Hex code E4 to E9 are the first char
                    line[i]==(char)0xE6 |line[i]==(char)0xE7 |  //of utf8 chinese character.
                    line[i]==(char)0xE8 |line[i]==(char)0xE9 ) {
                        cout <<line[i];
                        cout <<line[i+1];
                        cout <<line[i+2]<<endl;  //Print the UTF8 Chinese character.
                        i=i+3;
                        }
                    else i++;  //If it isn't chinese, skip.
                }
        }
        myfile.close();
    } else {
        cerr << "Unable to open file";
    }
    return 0;
}
