#!/usr/bin/python3

# Copyright (C) 2013 Mathieu Bridon <bochecha@fedoraproject.org>
#
# This file is part of libcangjie.
#
# libcangjie is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libcangjie is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libcangjie.  If not, see <http://www.gnu.org/licenses/>.


from operator import itemgetter
import struct


MAX_CODE = 0xfefe


def pad4(bs):
    length = len(bs)

    if length == 4:
        return bs

    padding = b"\x00"*(4 - (length%4))

    return padding+bs


def get_freq(s):
    bs = pad4(s.encode("big5"))
    i = struct.unpack(">I", bs)[0]

    return i


result = set()

with open("tables/cj5-tc.txt", "r") as table:
    in_data = False

    while True:
        try:
            line = next(table)

        except UnicodeDecodeError:
            # We seem to have some characters which Python can't read :-/
            continue

        except StopIteration:
            # End of the file
            break

        line = line.strip("\n")

        if not line:
            continue

        if line.startswith("#"):
            continue

        if in_data:
            char = line.split(" ")[-1]

            try:
                result.add((char, get_freq(char)))

            except UnicodeEncodeError:
                # Ignore character not in Big5
                pass

        if not in_data and line == "[DATA]":
            in_data = True


sorted_chars = sorted(result, key=itemgetter(1))

with open("classic-frequency.txt", "w") as out_:
    out_.write("""# This file lists the characters found in tables/cj5-tc.txt, ordered by their
# code in the Big5 encoding.
#
# This corresponds to the ordering of the candidates returned by the Cangjie
# and Quick input methods on a well-known Operating System.
#
# Note that the frequency computed below is not directly the code of the
# character in Big5. Instead it is the result of the following operation:
#
#     frequency = max_code - code(character)
#
# In the above, max_code is 65278 (0xfefe), which is the highest possible code
# in Big5.
#
# The below list is placed in the Public Domain

""")

    for char, code in sorted_chars:
        out_.write("%s %s\n" % (char, MAX_CODE-code))
