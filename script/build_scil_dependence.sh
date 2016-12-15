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

external_file=$src_dir/inc/external.h
external_module_list="$src_dir/common $src_dir/log"
echo "    Generate .. $external_file"
perl script/generate_external_declaration.pl $external_file $external_module_list

tokenizer_external_file=$src_dir/inc/tokenizer_external.h
tokenizer_external_module_list="$src_dir/tokenizer"
echo "    Generate .. $tokenizer_external_file"
perl script/generate_external_declaration.pl $tokenizer_external_file $tokenizer_external_module_list

regular_external_file=$src_dir/inc/regular_external.h
regular_external_module_list="$src_dir/regular"
echo "    Generate .. $regular_external_file"
perl script/generate_external_declaration.pl $regular_external_file $regular_external_module_list

token_external_file=$src_dir/inc/token_external.h
token_external_module_list="$src_dir/tokenizer/token"
echo "    Generate .. $token_external_file"
perl script/generate_external_declaration.pl $token_external_file $token_external_module_list

nfa_external_file=$src_dir/inc/nfa_external.h
nfa_external_module_list="$src_dir/finite_automata/nfa"
echo "    Generate .. $nfa_external_file"
perl script/generate_external_declaration.pl $nfa_external_file $nfa_external_module_list

test_external_file=$src_dir/inc/test_external.h
test_external_module_list="$src_dir/test"
echo "    Generate .. $test_external_file"
perl script/generate_external_declaration.pl $test_external_file $test_external_module_list

echo "    Copy     .. data_structure_interface.h"
cp $libds_interface $inc_dir


#######################
## Generate Makefile ##
#######################
echo "    Makefile .. compile"
perl script/produce_compile_makefile.pl $src_dir


###############################
## generate linking Makefile ##
###############################
echo "    Copy     .. Makefile"
cp -v $src_dir/Makefile.in $obj_dir > $verbose

echo "    Copy     .. libds.a"
cp -v $libds_archive $obj_dir > $verbose

module="tkz"
echo "    Generate .. Makefile.$module"
perl script/produce_link_makefile.pl $obj_dir $module

