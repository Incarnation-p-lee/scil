#!/usr/bin/perl
use strict;
use 5.010;

my $output = "output";
my $obj_list = "";

die "Failed to find output directory, $output." unless -d -e $output;

## Collecting all objective files in output ##
opendir OUTPUT, $output or
    die "Failed to open output directory $output, $?\n";

while(readdir(OUTPUT)) {
    $obj_list = "$_ $obj_list" if /(\w+)\.o/;
}

chop $obj_list;
closedir OUTPUT;

## Generate link Makefile in output ##
open MAKEFILE, '>', "$output/Makefile" or
    die "Failed to create makefile, $?\n";

printf MAKEFILE "LDFLAGS+=-Tlink.ld -m elf_i386\n";
printf MAKEFILE "TARGET=kernel\n";
printf MAKEFILE 'all:$(TARGET)' ."\n\n";
printf MAKEFILE '$(TARGET):' . "$obj_list\n";
printf MAKEFILE "\t" . 'ld $(LDFLAGS) -o $@ $?' . "\n";

close MAKEFILE;

