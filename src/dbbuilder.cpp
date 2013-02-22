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

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <db_cxx.h>
#include <fstream>

#include "util.h"

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
    pdb->set_flags(DB_DUP);
    // Create (or clear its content if it exists) the database
    pdb->open(NULL, kDatabaseName, NULL, DB_BTREE, DB_CREATE | DB_TRUNCATE, 0);

    ifstream myfile (filename);
    if (myfile.is_open()) {
        while ( myfile.good() ) {
            getline (myfile,line);

            // Ignore empty and commented lines
            if ((line.length() == 0) || (startswith(line, std::string("#")))) {
                continue;
            }

            // Ignore the preamble
            if (!datamode) {
                if ("[DATA]" == line ) {
                    datamode = true;
                }
                continue;
            }

            // Get the key code
            int index = 0;
            while (32 != (char)line[index]) {
                index++;
            }
            string cjkey = line.substr(0, index);
            while (32 == line[index]) {
                index++;
            }
            Dbt key(const_cast<char*>(cjkey.data()), cjkey.size());

            // Get the character
            string cjdata = line.substr(index, (line.size()-index));
            Dbt value(const_cast<char*>(cjdata.data()), cjdata.size());
            pdb->put(NULL, &key, &value, 0);
        }
        myfile.close();
    } else {
        cerr << "Unable to open file";
    }

    // Clean up
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
