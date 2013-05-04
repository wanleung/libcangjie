/*
 * Copyright (C) 2012-2013 Mathieu Bridon <bochecha@fedoraproject.org>
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

#include "util.h"

/* These are some simple helper functions, used by various parts of libcangjie.
 *
 * They are not (and should not) be exposed as part of the public API.
 */

// This could eventually be replaced by boost::starts_with
bool startswith(std::string s, std::string begin) {
    return (s.find(begin) == 0);
}

// This could eventually be replaced by boost::ends_with
bool endswith(std::string s, std::string ending) {
    if (s.length() >= ending.length()) {
        return (s.compare(s.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}
