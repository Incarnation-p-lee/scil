#!/usr/bin/perl
use strict;
use 5.010;

my $work_dir = shift @ARGV;
my $include_dir = "$ENV{'PWD'}/src/inc";
my $makefile_in = "$ENV{'PWD'}/src/Makefile.in";

if ($work_dir eq "") {
    say "Please specify the work directory for makefile producing.";
    exit 1;
} else {
    $work_dir = $1 if $work_dir =~ /(\w+)\/?$/;
    &create_compile_makefile("$work_dir") if -e "$work_dir/main.c";
    &visit_workspace($work_dir);
}

sub visit_workspace {
    my $base = shift @_;

    if ( -e -d $base) {
        opendir WORKSPACE, $base or
            die "Failed to open work directory $base, $?\n";

        my @work_list = readdir(WORKSPACE);

        foreach my $entry (@work_list) {
            chomp;
            next if $entry =~ /^\.$/;
            next if $entry =~ /^\.\.$/;
            next if $entry =~ /^inc$/;

            my $sub_dir = "$base/$entry";
            if (-e -d $sub_dir) {
                &create_compile_makefile($sub_dir);
                &visit_workspace($sub_dir)
            }
        }

        closedir WORKSPACE;
    }
}

sub create_compile_makefile {
    my $dir;
    my $file;
    my $module;

    $dir = shift @_;
    $dir =~ /\w+/ or
        die "Script DO NOT know how to make one makefile without a module name.";

    $module = $1 if $dir =~ /\/?(\w+)$/;

    ## handle main.c Makefile ##
    $module = "main" if -e "$dir/main.c";
    $file = "$dir/$module.c";

    say "    Makefile .. $module";

    open MAKEFILE, '>', "$dir/Makefile" or
        die "Failed to create makefile, $?\n";

    printf MAKEFILE "# Automaticaly generated by $0.\n\n";
    printf MAKEFILE "include $makefile_in\n";
    printf MAKEFILE "CFLAG += -I$include_dir\n\n";
    printf MAKEFILE "TARGET = $module.o\n";
    printf MAKEFILE 'all:$(TARGET)' ."\n\n";
    printf MAKEFILE ".s.o:\n";
    printf MAKEFILE "\t" . 'nasm $(AFLAG) -o $@ $<' . "\n";
    printf MAKEFILE "$module.o:$file\n";
    printf MAKEFILE "\t" . 'cc $(CFLAG) -o $@ $<' . "\n";

    close MAKEFILE;
}

