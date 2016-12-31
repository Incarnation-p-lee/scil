#!/bin/bash

base=$1
debug_mode=$2
verbose=$3

out_dir=$base/output
src_dir=$base/src
inc_dir=$src_dir/inc
obj_dir=$out_dir/obj

libds_dir=$base/libds
libds_inc_dir=$libds_dir/output/inc
libds_bin_dir=$libds_dir/output/bin
libds_archive=$libds_bin_dir/libds.a
libds_interface=$libds_inc_dir/data_structure_interface.h

###########################################
## update some header files and makefile ##
###########################################
echo "    Generate .. declarnation"
perl script/generate_declaration.pl $src_dir $debug_mode

echo "    Copy     .. data_structure_interface.h"
cp $libds_interface $inc_dir

external_module_list="$src_dir/common
                      $src_dir/log
                      $src_dir/tokenizer
                      $src_dir/regular
                      $src_dir/tokenizer/token
                      $src_dir/finite_automata/nfa
                      $src_dir/test
                      $src_dir/parser/grammar
                      $src_dir/parser"

for module in $external_module_list
do
    module_name=$(basename $module)
    external_file=$src_dir/inc/${module_name}_external.h
    echo "    Generate .. $external_file"
    perl script/generate_external_declaration.pl $external_file $module
done

#######################
## Generate Makefile ##
#######################
echo "    Makefile .. compile"
perl script/produce_compile_makefile.pl $src_dir

echo "    Copy     .. Makefile"
cp -v $src_dir/Makefile.in $obj_dir > $verbose

echo "    Copy     .. libds.a"
cp -v $libds_archive $obj_dir > $verbose


