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

#include <stdio.h>
#include <locale.h>
#include <iostream>

#include "cangjie.h"
#include <string.h>
#include <vector>
//#include <db_cxx.h>

//#include "chchar.h"

using namespace std;

int main(int argc, const char* argv[]) {
    if (argc !=2) {
        cout << "usage: "<< argv[0] << " <code>" << endl;
        return 1;
    }

    //string input = string(argv[1]);

    //cout << input << endl;

    CangJie cj(CangJie_Version_Type_5, CANGJIE_ALL_CJK);

    //cj.open_db();

    vector<ChChar> ss;

    string str(argv[1]);
    ss = cj.getCharacters(str);   
    //cj.close_db();

   for(int i=0; i < ss.size(); i++) {
      cout << ss[i].chchar() << " " << ss[i].code() << " " << ss[i].order() << " " << ss[i].frequency() << " " << ss[i].type() << " " << endl;
   }
   cout << endl;
   cout << "TOTAL:" << ss.size() << endl;

/*
   string aaaa("");
   while (1) {
       char c;
       cin >> c;
       if (cj.isCangJieInputKey(c)) {
           string b = cj.translateInputKeyToCangJie(c);
           cout << "yes " << b  << endl;
           aaaa += b;
       } else {
           cout << "no" << endl;
           break;
       }
   }
   cout << aaaa << endl;
*/
/*
    std::string envHome("./cj5/");
    u_int32_t env_flags = DB_CREATE ;
    DbEnv myEnv(0);
    myEnv.open(envHome.c_str(), env_flags, 0);

    Db* pdb;

    pdb = new Db(NULL, 0);
    // If you want to support duplicated records and make duplicated
    // records sorted by data, you need to call:
    //   pdb->set_flags(DB_DUPSORT);
    // Note that only Btree-typed database supports sorted duplicated
    // records

    // If the database does not exist, create it.  If it exists, clear
    // its content after openning.
    pdb->set_flags( DB_DUP );// | DB_DUPSORT);
    pdb->open(NULL, "cjk.mb", NULL, DB_BTREE, DB_RDONLY, 0);

    string *code;
    code = new string(argv[1]);
  try {
    Dbt key(const_cast<char*>(code->c_str()), code->size());
    char buffer[1024];
    Dbt data;
    data.set_data(buffer);
    data.set_ulen(1024);
    data.set_flags(DB_DBT_USERMEM);
    if (pdb->get(NULL, &key, &data, 0) == DB_NOTFOUND) {
        cout << "nothing" <<endl; 
    } else {
        std::string res =  std::string(buffer, strlen(buffer));
        cout << res <<endl;
        //return res;
    }
  } catch (DbException& e) {
    cerr << "DbException: " << e.what() << endl;
  } catch (std::exception& e) {
    cerr << e.what() << endl;
  }
 
    pdb->close(0);
//*/
    return 0;
}
