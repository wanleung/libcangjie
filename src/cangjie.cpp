#include "cangjie.h"

using namespace std;

string CANGJIE_SIMPLIFIED_DB("sc.mb");
string CANGJIE_TRADITIONAL_DB("tc.mb");
string CANGJIE_COMMON_DB("cc.mb");
string CANGJIE_ALL_CJK_DB("cjk.mb");

string CANGJIE_DATA_PATH("./data/");

CangJie::CangJie (CangJie_Version_Type version, uint32_t flags) : 
    cangjie_flags_(flags),
    cangjie_version_(version)
{
    string db_filename("");
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
        //cout << db_filename << endl;
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
        //std::cout << "Database " << ""
        //          << " is closed." << std::endl;
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
        result.push_back(string((char *)data.get_data()));
        //std::cout << " key: " << (char *)key.get_data() 
        //          << " data: " << (char *)data.get_data()<< std::endl;
        ret = cursor->get(&key, &data, DB_NEXT_DUP);
    }

  } catch (DbException& e) {
    cerr << "DbException: " << e.what() << endl;
  } catch (std::exception& e) {
    cerr << e.what() << endl;
  }
  return result;
}

