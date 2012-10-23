#!/usr/bin/perl

use warnings;
use strict;
use utf8;
use BerkeleyDB;

sub main {
    my ($filename, $key) = @_;
    #my $filename = ".db"; 

    my $db = new BerkeleyDB::Btree
                -Filename => $filename,
                -Property  => DB_DUP, # | DB_DUPSORT
        or die "Cannot open file $filename: $! $BerkeleyDB::Error\n" ;



    my ($k, $v) = ($key, "") ;
    #my $rc = $db->db_get($k, $v );
    my $cursor = $db->db_cursor() ;
    my $rc = $cursor->c_get($k, $v, DB_SET_RANGE);
    
    #print $cursor->c_get($k, $v, DB_FIRST);
    #if (! $rc) {
        print "$rc, $k, $v\n";
    #}

    while ($cursor->c_get($k, $v, DB_NEXT_DUP) == 0)
      { print "$k -> $v\n" }

    #undef $cursor ;
    undef $db ;

}




my $num_args = $#ARGV + 1;

if ($num_args != 2) {
  print "\nUsage: test.pl <dbfilename> <code>\n";
  exit;
}


main($ARGV[0], $ARGV[1]);
