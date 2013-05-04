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


class Big5Error(Exception):
    pass


def pad4(bs):
    length = len(bs)

    if length == 4:
        return bs

    padding = b"\x00"*(4 - (length%4))

    return padding+bs


def get_freq(s):
    try:
        bs = pad4(s.encode("big5"))

    except UnicodeEncodeError:
        raise Big5Error("The character '%s' does not exist in Big5" % s)

    i = struct.unpack(">I", bs)[0]

    # This is where it gets ugly.
    #
    # See the comments in the generated file (hint it's at the bottom of this
    # script) for the details of what happens below.

    if 0x8140 <=i and i <= 0xa0fe:
        # Reserved for user-defined characters
        raise Big5Error("The code 0x%x for '%s' is reserved for user-defined "
                         "characters" % (i, s))

    elif 0xa140 <= i and i <= 0xa3bf:
        # Graphical characters
        # Their frequency will be between 2025 and 1386
        freq = (MAX_CODE - 0xc67e) - 1 - (0xf9d5 - 0xc940) -1 - (i - 0xa140)

    elif 0xa3c0 <= i and i<= 0xa3fe:
        # Reserved, not for user-defined characters
        raise Big5Error("The code 0x%x for '%s' is reserved, but not for "
                         "user-defined characters" % (i, s))

    elif 0xa440 <= i and i<= 0xc67e:
        # Frequently used characters
        # Their frequency will be between 23230 and 14464
        freq = MAX_CODE - i

    elif 0xc6a1 <= i and i<= 0xc8fe:
        # Reserved for user-defined characters
        raise Big5Error("The code 0x%x for '%s' is reserved for user-defined "
                         "characters" % (i, s))

    elif 0xc940 <= i and i<= 0xf9d5:
        # Less frequently used characters
        # Their frequency will be between 14463 and 2026
        freq = (MAX_CODE - 0xc67e) - 1 - (i - 0xc940)

    elif 0xf9d6 <= i and i<= 0xfefe:
        # Reserved for user-defined characters
        raise Big5Error("The code 0x%x for '%s' is reserved for user-defined "
                         "characters" % (i, s))

    else:
        raise Big5Error("The code 0x%x for '%s' is undefined in the Big5 spec"
                         % (i, s))

    return freq if freq > 0 else 0


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

            except Big5Error as e:
                # The character is either in a reserved/undefined range of the
                # Big5 spec, or it doesn't exist in Big5 at all. Ignore it
                print(e)
                continue

        if not in_data and line == "[DATA]":
            in_data = True


sorted_chars = sorted(result, key=itemgetter(1), reverse=True)

with open("classic-frequency.txt", "w") as out_:
    out_.write("""# This file lists the characters found in tables/cj5-tc.txt, ordered by their
# code in the Big5 encoding.
#
# This corresponds to the ordering of the candidates returned by the Cangjie
# and Quick input methods on a well-known Operating System.
#
# Note that the frequency computed below is not directly the code of the
# character in Big5. Instead, it is based on the following heuristic:
#
#     1. We are only interested in 3 zones of Big5:
#         - Frequently used characters (from 0xa440 to 0xc67e)
#         - Less frequently used characters (from 0xc940 to 0xf9d5)
#         - Graphical characters (from 0xa140 to 0xa3bf)
#     2. These 3 zones need to be prioritized in the order above
#     3. Inside a zone, characters are ordered by their Big5 code (a smaller
#        code leads to a bigger frequency)
#
# The Wikipedia page gives details about the compartmentalization of Big5:
#     http://en.wikipedia.org/wiki/Big5#A_more_detailed_look_at_the_organization
#
# The below list is placed in the Public Domain

""")

    for char, freq in sorted_chars:
        out_.write("%s %s\n" % (char, freq))
