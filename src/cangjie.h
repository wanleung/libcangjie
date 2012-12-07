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
        bool isCangJieInputKey (char c);
        //char CangJie::get_full_width_version(char)

    private:
        DbEnv *cangjie_env_;
        Db* cangjie_db_;
        //Dbc *cursor;
        CangJie_Version_Type cangjie_version_;
        uint32_t cangjie_flags_;
        CangJie() : cangjie_env_(0) {}
        void close();
};

