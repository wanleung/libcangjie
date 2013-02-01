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

  try {

    env.set_error_stream(&cerr);
    env.open("./", DB_CREATE | DB_INIT_MPOOL, 0);

    pdb = new Db(&env, 0);
    pdb->set_flags( DB_DUP );// | DB_DUPSORT);
    pdb->open(NULL, kDatabaseName, NULL, DB_BTREE, DB_CREATE | DB_TRUNCATE, 0);

    ifstream myfile (filename);
    if (myfile.is_open()) {
        int count = 0;
        while ( myfile.good() ) {
            getline (myfile,line);
            string word = line.substr(0, 3);
            int freq = atoi((line.substr(4, std::string::npos)).c_str());
            Dbt key(const_cast<char*>(word.data()), word.size());
            Dbt value(&freq, sizeof(int));
            pdb->put(NULL, &key, &value, 0);
        }
        myfile.close();
    } else {
        cerr << "Unable to open file";
    }

    // You need to set ulen and flags=DB_DBT_USERMEM to prevent Dbt
    // from allocate its own memory but use the memory provided by you.
    int buffer[1024];
    Dbt data;
    data.set_data(buffer);
    data.set_ulen(1024);
    data.set_flags(DB_DBT_USERMEM);

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

