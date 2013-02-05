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

#include <stdint.h>
#include <string>
using namespace std;

#define CHCHAR_SIMPLIFIED    0x00000001
#define CHCHAR_TRADITIONAL   0x00000010
#define CHCHAR_BOTH          0x00000011
#define CHCHAR_OTHER         0x00000000

class ChChar
{
    public:
        ChChar(string chchar, uint32_t type, uint32_t order);
        ~ChChar() { close(); }
        
        string chchar(); 
        uint32_t type();
        uint32_t frequency();
        void set_frequency(uint32_t frequency);
        uint32_t order();

        string code();
        void set_code(string code);

        friend bool operator== (ChChar &c1, ChChar &c2);
        friend bool operator!= (ChChar &c1, ChChar &c2);

    private:
        string chchar_;
        string code_;
        uint32_t type_;
        uint32_t frequency_;
        uint32_t order_;
 
        void close();
};
