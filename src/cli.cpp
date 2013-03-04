/*
 * Copyright (C) 2012  Wan Leung Wong <me at wanleung dot com>
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
#include <string>
#include <vector>

#include "cangjie.h"

using namespace std;


int main(int argc, const char* argv[]) {
    if (argc !=2) {
        cout << "usage: "<< argv[0] << " <code>" << endl;
        return 1;
    }

    CangJie cj(CangJie_Version_Type_5, CANGJIE_ALL_CJK);

    vector<ChChar> ss;
    string str(argv[1]);
    ss = cj.getCharacters(str);   

   for(int i=0; i < ss.size(); i++) {
      cout << ss[i].chchar() << " " << ss[i].code() << " " << ss[i].order() << " " << ss[i].frequency() << " " << ss[i].classic_frequency() << " " << ss[i].type() << " " << endl;
   }
   cout << endl;
   cout << "TOTAL:" << ss.size() << endl;

    return 0;
}
