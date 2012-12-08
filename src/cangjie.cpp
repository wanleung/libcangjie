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

using namespace std;

string CANGJIE_SIMPLIFIED_DB("sc.mb");
string CANGJIE_TRADITIONAL_DB("tc.mb");
string CANGJIE_COMMON_DB("cc.mb");
string CANGJIE_ALL_CJK_DB("cjk.mb");

string CANGJIE_DATA_PATH("./data/");

CangJie::CangJie (CangJie_Version_Type version, uint32_t flags) :
    cangjie_version_(version),
    cangjie_flags_(flags),
    isEnglishMode_(false),
    isFullWidthMode_(false)
{
    string db_filename(CANGJIE_DATA_DIR);
    try {

        cangjie_env_ = new DbEnv(0);
        cangjie_env_->open(CANGJIE_DATA_PATH.c_str(), DB_CREATE | DB_INIT_MPOOL, 0);

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

bool CangJie::isCangJieInputKey(char c) {
    if (c >= 'a' && c <='z') {
        return true;
    }
    return false;
}

void CangJie::setFullWidthModeEnable(bool enable) {
    isFullWidthMode_ = enable;
}

bool CangJie::isFullWidthMode() {
    return isFullWidthMode_;
}

void CangJie::setEnglishModeEnable(bool enable) {
    isEnglishMode_ = enable;
}

bool CangJie::isEnglishMode() {
    return isEnglishMode_;
}

