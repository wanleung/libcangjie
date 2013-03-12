#!/usr/bin/perl

# Copyright (C) 2012  Wan Leung Wong <me at wanleung dot com>
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

my @tables = ("cj3-sc", "cj3-tc", "cj3-cc", "cj3-cjk",
              "cj5-sc", "cj5-tc", "cj5-cc", "cj5-cjk");
my @additionals = ("jp");

sub main {
    my $table = shift;
    
    my $tablefilename = "tables/$table.txt";
    my $dbfilename = "data/$table.mb";

    print(STDOUT "Checking $tablefilename...\n");

    my $db = new BerkeleyDB::Btree
                -Filename => $dbfilename,
                -Property  => DB_DUP, # | DB_DUPSORT
        or die "Cannot open file $dbfilename: $! $BerkeleyDB::Error\n" ;

    # Tests might be run from different places (e.g `make distcheck`)
    $tablefilename  = dirname(abs_path($0))."/../".$tablefilename;

    # Check that all the data from the text table file is in the DB
    #my $in_data = 0;
    #my $table_count = 0;
    #open(FILE, "<:encoding(UTF-8)", "$tablefilename") or die($!);
    #while (<FILE>) {
    #    my $line = $_;

    #    # Strip whitespace
    #    $line=~s/^ +//;
    #    $line=~s/ +$//;
    #    $line=~s/\n+$//;

    #    # Ignore empty and commented lines
    #    if (($line =~ /^#/) || ($line eq "")) {
    #        next;
    #    }

    #    if ($in_data) {
    #        $table_count = $table_count + 1;

    #        (my $key, my $table_value) = split(/ +/, $line);

    #        my $db_value = encode_utf8($table_value);
    #        $db->db_get($key, $db_value, DB_GET_BOTH);
    #        if ($table_value ne decode_utf8($db_value)) {
    #            die("Could not find ($key, $table_value)\n");
    #        }
    #    } else {
    #        if ($line =~ /^\[DATA\]$/) {
    #            # Next line is the first data line
    #            $in_data = 1;
    #        }
    #    }
    #}
    my $table_count = readtable($db, $tablefilename);
    my $add_table_count = 0; 
    for my $addition (@additionals) {
        my $additional_filename = "tables/$addition.txt";
        $additional_filename  = dirname(abs_path($0))."/../".$additional_filename;
        my $m_count = readtable($db, $additional_filename);
        $add_table_count += $m_count;
    }


    # Check that the DB has the same number of entries as the text table file
    my $cursor = $db->db_cursor();

    my $db_count = 0;
    my $key = 0;
    my $value = 0;
    while ($cursor->c_get($key, $value, DB_NEXT) == 0) {
        $db_count++ ;
    }

    my $total = $table_count + $add_table_count;

    if ($total != $db_count) {
        die("Table contained $total entries but DB contains $db_count\n");
    }
}

sub readtable {
    my ($db, $filename) = @_;
    my $in_data = 0;
    my $table_count = 0;
    open(FILE, "<:encoding(UTF-8)", "$filename") or die($!);
    while (<FILE>) {
        my $line = $_;

        # Strip whitespace
        $line=~s/^ +//;
        $line=~s/ +$//;
        $line=~s/\n+$//;

        # Ignore empty and commented lines
        if (($line =~ /^#/) || ($line eq "")) {
            next;
        }

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
    close FILE;
    return $table_count;

}

#### Main Run Below

for my $key (@tables) {
    main($key);
}
