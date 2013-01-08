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

#include "cangjie.h"
#include "cangjieconfig.h"
#include <cstdlib>
#include <sys/stat.h>

using namespace std;

/* These are some simple helper functions, used by the CangJie class below.
 *
 * They are not (and should not) be exposed as part as the public API.
 */
bool startswith(std::string s, std::string begin) {
    return (s.find(begin) == 0);
}

bool endswith(std::string s, std::string ending) {
    if (s.length() >= ending.length()) {
        return (s.compare(s.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}
// End of the helper functions


string CANGJIE_SIMPLIFIED_DB("sc.mb");
string CANGJIE_TRADITIONAL_DB("tc.mb");
string CANGJIE_COMMON_DB("cc.mb");
string CANGJIE_ALL_CJK_DB("cjk.mb");


CangJie::CangJie (CangJie_Version_Type version, uint32_t flags) :
    cangjie_version_(version),
    cangjie_flags_(flags)
{
    string base_runtime_dir, cangjie_runtime_dir;
    try {
        base_runtime_dir = (std::string)getenv("XDG_RUNTIME_DIR");
    } catch(std::exception &e) {
        base_runtime_dir = "/tmp";
    }
    cangjie_runtime_dir = base_runtime_dir + "/libcangjie";
    mkdir(cangjie_runtime_dir.c_str(), S_IRWXU);

    string db_filename(CANGJIE_DATA_DIR);
    try {

        cangjie_env_ = new DbEnv(0);
        cangjie_env_->open(cangjie_runtime_dir.c_str(), DB_CREATE | DB_INIT_MPOOL, 0);

        if (cangjie_version_ == CangJie_Version_Type_3 ) {
            db_filename += "cj3-";
        } else {
            db_filename += "cj5-";
        }

        switch (cangjie_flags_) {
            case CANGJIE_SIMPLIFIED : {
                db_filename += CANGJIE_SIMPLIFIED_DB;
                break;
            }
            case CANGJIE_TRADITIONAL : {
                db_filename += CANGJIE_TRADITIONAL_DB;
                break;
            }
            case CANGJIE_COMMON : {
                db_filename += CANGJIE_COMMON_DB;
                break;
            }
            case CANGJIE_ALL_CJK : {
                db_filename += CANGJIE_ALL_CJK_DB;
                break;
            }
            default: {
                db_filename += CANGJIE_ALL_CJK_DB;
                break;
            }

        }

        cangjie_db_ = new Db(cangjie_env_, 0);
        cangjie_db_->set_error_stream(&std::cerr);

        uint32_t db_flags = DB_TRUNCATE;
        cangjie_db_->set_flags( DB_DUP );// | DB_DUPSORT);

        // Open the database
        cangjie_db_->open(NULL, db_filename.c_str(), NULL, DB_BTREE, DB_RDONLY, 0);
    }
    // DbException is not a subclass of std::exception, so we
    // need to catch them both.
    catch(DbException &e)
    {
        std::cerr << "Error opening database: " << db_filename << "\n";
        std::cerr << e.what() << std::endl;
    }
    catch(std::exception &e)
    {
        std::cerr << "Error opening database: " << db_filename << "\n";
        std::cerr << e.what() << std::endl;
    }
}


void CangJie::close()
{
    // Close the db
    try
    {
        cangjie_db_->close(0);
        delete cangjie_db_;
        cangjie_env_->close(0);
        delete cangjie_env_;
    }
    catch(DbException &e)
    {
        std::cerr << "Error closing database: " << "" << "\n";
        std::cerr << e.what() << std::endl;
    }
    catch(std::exception &e)
    {
        std::cerr << "Error closing database: " << "" << "\n";
        std::cerr << e.what() << std::endl;
    }
}


std::vector<std::string> CangJie::getCharacters (std::string code) {
    vector<string> result;

    // If the input code has a wildcard, call the dedicated function
    int pos = code.find("*");
    if (pos > 0) {
        string begin = code.substr(0, pos);
        string end = code.substr(pos+1);

        // Make sure there actually is something to match
        if ((begin.size() > 0) && (end.size() > 0)) {
            return this->getCharactersRange(begin, end);
        } else {
            return result;
        }
    }

    try {
        Dbc *cursor;
        cangjie_db_->cursor(NULL, &cursor, 0);
        Dbt key(const_cast<char *>(code.c_str()), code.size());
        Dbt data;

        int ret = cursor->get(&key, &data, DB_SET);
        while (ret != DB_NOTFOUND) {
            result.push_back(string((char *)data.get_data(), data.get_size()));
            ret = cursor->get(&key, &data, DB_NEXT_DUP);
        }

    } catch (DbException& e) {
        cerr << "DbException: " << e.what() << endl;
    } catch (std::exception& e) {
        cerr << e.what() << endl;
    }
    return result;
}

std::vector<std::string> CangJie::getCharactersRange (std::string begin, std::string ending) {
    vector<string> result;

    try {
        Dbc *cursor;
        cangjie_db_->cursor(NULL, &cursor, 0);

        Dbt key(const_cast<char *>(begin.c_str()), begin.size());
        Dbt data;
        string s_key;

        int ret = cursor->get(&key, &data, DB_SET_RANGE);
        s_key = string((char *)key.get_data(), key.get_size());
        while (ret != DB_NOTFOUND && startswith(s_key, begin)) {
            if (endswith(s_key, ending)) {
                result.push_back(string((char *)data.get_data(), data.get_size()));
            }
            ret = cursor->get(&key, &data, DB_NEXT);
            s_key = string((char *)key.get_data(), key.get_size());
        }
    } catch (DbException& e) {
        cerr << "DbException: " << e.what() << endl;
    } catch (std::exception& e) {
        cerr << e.what() << endl;
    }
    return result;
}

bool CangJie::isCangJieInputKey(char c) {
    if (c >= 'a' && c <='z') {
        return true;
    }
    return false;
}

std::string CangJie::getFullWidthChar(char key) {
    extern const char * fullengchar[];
    if (key < ' ' || key > 127) {
        return NULL;
    }
    return string(fullengchar[key-' ']);
}

std::string CangJie::translateInputKeyToCangJie(char key) {
    extern const char * inputcodemap[];
    if (key == '*') {
        extern const char * fullengchar[];
        return string(fullengchar['*'-' ']);
    }
    if (key < 'a' || key > 'z') {
        return NULL;
    }
    return string(inputcodemap[key - 'a']);
}
