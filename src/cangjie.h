#include <string.h>
#include <vector>
#include <db_cxx.h>

#define CANGJIE_3 


#define CANGJIE_SIMPLIFIED    0x00000001
#define CANGJIE_TRADITIONAL   0x00000010
#define CANGJIE_COMMON        0x00000011
#define CANGJIE_ALL_CJK       0x00000111

typedef enum {
    CangJie_Version_Type_3 = 1,
    CangJie_Version_Type_5,
} CangJie_Version_Type;

class CangJie {
    public:
        CangJie (CangJie_Version_Type version, uint32_t flags);
        ~CangJie() { close(); }
        std::vector<std::string> getCharacters (std::string code);

    private:
        DbEnv *cangjie_env_;
        Db* cangjie_db_;
        //Dbc *cursor;
        CangJie_Version_Type cangjie_version_;
        uint32_t cangjie_flags_;
        CangJie() : cangjie_env_(0) {}
        void close();
        char* discardCharacterGarbageByte (char* char_data, int size);
};

