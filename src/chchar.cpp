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

#include "chchar.h"

ChChar::ChChar(std::string chchar, uint32_t type, uint32_t order) :
    chchar_(chchar),
    type_(type),
    frequency_(0),
    order_(order)
{
}

void ChChar::close()
{
}

std::string ChChar::chchar()
{
    return chchar_;
}

std::string ChChar::code()
{
    return code_;
}

void ChChar::set_code(std::string code)
{
    code_ = string(code);
}

uint32_t ChChar::type()
{
    return type_;
}

uint32_t ChChar::frequency()
{
    return frequency_;
}

void ChChar::set_frequency(uint32_t frequency)
{
    frequency_ = frequency;
}

uint32_t ChChar::order()
{
    return order_;
}

bool operator== (ChChar &c1, ChChar &c2)
{
    return (c1.chchar() == c2.chchar());
}

bool operator!= (ChChar &c1, ChChar &c2)
{
    return !(c1 == c2);
}
