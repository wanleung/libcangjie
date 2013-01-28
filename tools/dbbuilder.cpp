/*
 * Copyright (C) 2012  Wan Leung Wong <wanleung at linkomnia dot com>
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

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <db_cxx.h>
#include <fstream>

using namespace std;


int main(int argc, const char* argv[]) {

  if (argc !=3) {
      cout << "usage: "<< argv[0] << " <filename> <db filename>" << endl;
      return 1;
  }

  const char* kDatabaseName = argv[2];
  const char* filename = argv[1];

  DbEnv env(0);
  Db* pdb;

  string line;

  bool datamode = false;

  try {

    env.set_error_stream(&cerr);
    env.open("./", DB_CREATE | DB_INIT_MPOOL, 0);

    pdb = new Db(&env, 0);
    // If you want to support duplicated records and make duplicated
    // records sorted by data, you need to call:
    //   pdb->set_flags(DB_DUPSORT);
    // Note that only Btree-typed database supports sorted duplicated
    // records

    // If the database does not exist, create it.  If it exists, clear
    // its content after openning.
    pdb->set_flags( DB_DUP );// | DB_DUPSORT);
    pdb->open(NULL, kDatabaseName, NULL, DB_BTREE, DB_CREATE | DB_TRUNCATE, 0);


    ifstream myfile (filename);
    if (myfile.is_open()) {
        while ( myfile.good() ) {
            getline (myfile,line);
            if (!datamode) {
                if ("[DATA]" == line ) {
                    datamode = true;
                }
                continue;
            }

            int index = 0;
            while (32 != (char)line[index]) {
                index++;
            }
            string cjkey = line.substr(0, index);
            while (32 == line[index]) {
                index++;
            }
            string cjdata = line.substr(index, (line.size()-index));
            cout << cjkey << "---" << cjdata << endl;
            Dbt key(const_cast<char*>(cjkey.data()), cjkey.size());
            Dbt value(const_cast<char*>(cjdata.data()), cjdata.size());
            pdb->put(NULL, &key, &value, 0);
        }
        myfile.close();
    } else {
        cout << "Unable to open file";
    }

    // You need to set ulen and flags=DB_DBT_USERMEM to prevent Dbt
    // from allocate its own memory but use the memory provided by you.
    string search("aa");
    Dbt key(const_cast<char*>(search.c_str()), search.size());
    char buffer[1024];
    Dbt data;
    data.set_data(buffer);
    data.set_ulen(1024);
    data.set_flags(DB_DBT_USERMEM);
    if (pdb->get(NULL, &key, &data, 0) == DB_NOTFOUND) {
      cerr << "Not found" << endl;
    } else {
      cout << "Found: " << "PPPPPP" <<buffer << "PPPPPP" << endl;
    }

    if (pdb != NULL) {
      pdb->close(0);
      delete pdb;
    }
    env.close(0);

  } catch (DbException& e) {
    cerr << "DbException: " << e.what() << endl;
    return -1;
  } catch (std::exception& e) {
    cerr << e.what() << endl;
    return -1;
  }

  return 0;
}

