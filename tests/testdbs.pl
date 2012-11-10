#!/usr/bin/perl

# Copyright (C) 2012  Wan Leung Wong <wanleung at linkomnia dot com>
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

use warnings;
use strict;
use utf8;
use Encode;
use BerkeleyDB;
use Cwd 'abs_path';
use File::Basename;

binmode(STDOUT, ":utf8");
binmode(STDERR, ":utf8");

my %tables = ("tables/cj3-6763.txt"  => "data/cj3-sc.mb",
              "tables/cj3-13053.txt" => "data/cj3-tc.mb",
              "tables/cj3-20902.txt" => "data/cj3-cc.mb",
              "tables/cj3-70000.txt" => "data/cj3-cjk.mb",
              "tables/cj5-8300.txt"  => "data/cj5-sc.mb",
              "tables/cj5-13053.txt" => "data/cj5-tc.mb",
              "tables/cj5-20902.txt" => "data/cj5-cc.mb",
              "tables/cj5-70000.txt" => "data/cj5-cjk.mb");


sub main {
    my $tablefilename = shift;
    my $dbfilename = shift;

    print(STDOUT "Checking $tablefilename...\n");

    my $db = new BerkeleyDB::Btree
                -Filename => $dbfilename,
                -Property  => DB_DUP, # | DB_DUPSORT
        or die "Cannot open file $dbfilename: $! $BerkeleyDB::Error\n" ;

    # Tests might be run from different places (e.g `make distcheck`)
    $tablefilename  = dirname(abs_path($0))."/../".$tablefilename;

    # Check that all the data from the text table file is in the DB
    my $in_data = 0;
    my $table_count = 0;
    open(FILE, "<:encoding(UTF-8)", "$tablefilename") or die($!);
    while (<FILE>) {
        my $line = $_;

        # Strip whitespace
        $line=~s/^ +//;
        $line=~s/ +$//;

        if ($in_data) {
            $table_count = $table_count + 1;

            (my $key, my $table_value) = split(/ +/, $line);

            my $db_value = encode_utf8($table_value);
            $db->db_get($key, $db_value, DB_GET_BOTH);
            if ($table_value ne decode_utf8($db_value)) {
                die("Could not find ($key, $table_value)\n");
            }
        } else {
            if ($line =~ /^\[DATA\]$/) {
                # Next line is the first data line
                $in_data = 1;
            }
        }
    }

    # Check that the DB has the same number of entries as the text table file
    my $cursor = $db->db_cursor();

    my $db_count = 0;
    my $key = 0;
    my $value = 0;
    while ($cursor->c_get($key, $value, DB_NEXT) == 0) {
        $db_count = $db_count+1;
    }

    if ($table_count != $db_count) {
        die("Table contained $table_count entries but DB contains $db_count\n");
    }
}


for my $key (keys %tables) {
    main($key, $tables{$key});
}
