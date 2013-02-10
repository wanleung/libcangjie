#!/bin/sh
#
# Copyright (C) 2013  Ben Au <benau2006 at yahoo dot com dot hk>
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
#

if [ $# -eq 0 ]
then
echo "No input file was supplied."
echo
echo "Usage: countfreq.sh [txt file] [-nocount]"
echo
echo "  -nocount    Just print out all chinese characters per line, useful for"
echo "              later combination with other corpus or counting total words."
echo "              ie: Total words = Total line of resulting file."
echo
echo "The input txt file must be in UTF8."
echo
echo "To save output, use countfreq.sh [txt file] > frequency.txt"
echo
echo "To save the corpus, use countfreq.sh [txt file] -nocount"
echo "The corpus file in the current directory is the word database."
echo
echo "Notice: Remember to delete the corpus file if you want to reset the database."
echo "ie: Don't combine with previous result."
exit
elif [ ! -f $1 ]
then
echo "$1:file not found"
echo
echo "Usage: countfreq.sh [txt file] [-nocount]"
echo
echo "  -nocount    Just print out all chinese characters per line, useful for"
echo "              later combination with other corpus or counting total words."
echo "              ie: Total words = Total line of resulting file."
echo
echo "The input txt file must be in UTF8."
echo
echo "To save output, use countfreq.sh [txt file] > frequency.txt"
echo
echo "To save the corpus, use countfreq.sh [txt file] -nocount"
echo "The corpus file in the current directory is the word database."
echo
echo "Notice: Remember to delete the corpus file if you want to reset the database."
echo "ie: Don't combine with previous result."
exit
else
#Compile getcorpus program
if [ ! -f ./getcorpus ]
then
    g++ -o getcorpus getcorpus.cpp
fi

#Read Input, Remove all non-chinese character and add newline after each chinese character
./getcorpus $1 >>corpus

#Sorting....
case $2 in
"-nocount")
;;
*)
cat ./corpus |sort | uniq -c |sort -g -r | awk ' { t = $1; $1 = $2; $2 = t; print; } ' | perl -pe 'chomp if eof'
;;
esac

fi
