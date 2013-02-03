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
#include "util.h"
#include <cstdlib>
#include <sys/stat.h>
#include <stdexcept>
//#include <algorithm>

using namespace std;

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
    string wordfq_filename(CANGJIE_DATA_DIR);
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

        wordfq_filename += "wordfreq.mb";

        cangjie_db_ = new Db(cangjie_env_, 0);
        wordfreq_ = new Db(cangjie_env_, 0);
        cangjie_db_->set_error_stream(&std::cerr);
        wordfreq_->set_error_stream(&std::cerr);

        uint32_t db_flags = DB_TRUNCATE;
        cangjie_db_->set_flags( DB_DUP );// | DB_DUPSORT);
        wordfreq_->set_flags( DB_DUP );

        // Open the database
        cangjie_db_->open(NULL, db_filename.c_str(), NULL, DB_BTREE, DB_RDONLY, 0);
        wordfreq_->open(NULL, wordfq_filename.c_str(), NULL, DB_BTREE, DB_RDONLY, 0);
    }
    // DbException is not a subclass of std::exception, so we
    // need to catch them both.
    catch(DbException &e)
    {
        std::cerr << "Cannot open tables or frequency database!\n";
        std::cerr << e.what() << std::endl;
    }
    catch(std::exception &e)
    {
        std::cerr << "Cannot open tables or frequency database!\n";
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
        wordfreq_->close(0);
        delete wordfreq_;
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


std::vector<ChChar> CangJie::getCharacters (std::string code) {
    vector<ChChar> result;

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
        int count = 0;
        while (ret != DB_NOTFOUND) {
            result.push_back(ChChar(string((char *)data.get_data(), data.get_size()), CHCHAR_BOTH, count++));
            ret = cursor->get(&key, &data, DB_NEXT_DUP);
        }

    } catch (DbException& e) {
        cerr << "DbException: " << e.what() << endl;
    } catch (std::exception& e) {
        cerr << e.what() << endl;
    }
    return result;
}

std::vector<ChChar> CangJie::getCharactersRange (std::string begin, std::string ending) {
    vector<ChChar> result;

    try {
        Dbc *cursor;
        cangjie_db_->cursor(NULL, &cursor, 0);

        Dbt key(const_cast<char *>(begin.c_str()), begin.size());
        Dbt data;
        string s_key;

        int ret = cursor->get(&key, &data, DB_SET_RANGE);
        s_key = string((char *)key.get_data(), key.get_size());
        int count = 0;
        while (ret != DB_NOTFOUND && startswith(s_key, begin)) {
            if ((s_key.length() >= (begin.length() + ending.length())) && endswith(s_key, ending)) {
                //if (std::find(result.begin(), result.end(),
                //ChChar(string((char *)data.get_data(), data.get_size()), CHCHAR_BOTH, count++)) == result.end()) {
                //Make sure no duplicate will be in the candidate list
                    result.push_back(ChChar(string((char *)data.get_data(), data.get_size()), CHCHAR_BOTH, count++));
                //}
            }
            ret = cursor->get(&key, &data, DB_NEXT);
            s_key = string((char *)key.get_data(), key.get_size());
        }
    } catch (DbException& e) {
        cerr << "DbException: " << e.what() << endl;
    } catch (std::exception& e) {
        cerr << e.what() << endl;
    }
    return result;//sortbyfreq(result);
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
        std::string msg = std::string("No full-width version of '") + key + "' char";
        throw std::invalid_argument(msg);
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
/*
std::vector<ChChar> CangJie::sortbyfreq (std::vector<ChChar> result)
{
    vector< int > freq;
    vector< ChChar > sorted;
    vector< pair<int, ChChar> > freqandword;

    try {
        Dbc *cursor;
        wordfreq_->cursor(NULL, &cursor, 0);
            for (vector<string>::iterator it = result.begin(); it != result.end(); ++it) { //get the candidates
                Dbt key(const_cast<char *>((*it).c_str()), (*it).size());
                Dbt data;
                int ret = cursor->get(&key, &data, DB_SET);
                if (!ret) {
                    freq.push_back (*((int*)data.get_data()));
                }//get the frequency from db
                else if (ret == DB_NOTFOUND) {
                    freq.push_back (atoi("0"));
                }//If we can't find the frequency of a word, put 0 in front of it.
            }
        }
    catch (DbException& e) {
        cerr << "DbException: " << e.what() << endl;
    } catch (std::exception& e) {
        cerr << e.what() << endl;
    }

    //Finish finding frequency of candidates.Now sorting
    if (freq.size()==result.size()) {
        //Make pair for sorting
        for (size_t i = 0; i < result.size(); ++i)
        freqandword.push_back( make_pair( freq[i], result[i] ));
        sort(freqandword.rbegin(), freqandword.rend());//sort...

        //Return the sorted characters
        for (int i=0; i<freqandword.size(); i++)
        sorted.push_back(freqandword[i].second);
    }
    return sorted;
}
*/
