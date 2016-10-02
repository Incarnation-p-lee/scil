#!/bin/bash
## addition cc and ld config ##
cc_config="-fPIC"
ld_config="-Wl,--stats"
ld_library=
debug_mode="Yes"
verbose="/dev/null"

if [ $# == 0 ]
then
    cat << EOF
Build Script Usage:
    sh src/script/build.sh DEBUG/RELEASE
                           X86_64/X86_32
                           LIBC
                           VERBOSE       -optional
                           PROFILE       -optional
                           COVERAGE      -optional
EOF
    exit 1
fi

base=$(pwd)
src_dir=$base/src
out_dir=$base/output
obj_dir=$out_dir/obj
bin_dir=$out_dir/out
lib_dir=$base/lib

#######################
## parameters handle ##
#######################
for pm in "$@"
do
    case $pm in
        "X86_64")
            cc_config="$cc_config -m64 -DX86_64"
            ld_config="$ld_config -m64" ;;
        "X86_32")
            cc_config="$cc_config -m32 -DX86_32 -fno-stack-protector"
            ld_config="$ld_config -m32" ;;
        "DEBUG")
            debug_mode="Yes"
            cc_config="$cc_config -g3 -DDEBUG"
            ld_config="$ld_config -g3" ;;
        "RELEASE")
            debug_mode="No"
            cc_config="$cc_config -o3 -ofast"
            ld_config="$ld_config -Wl,-O3" ;;
        "LIBC")
            cc_config="$cc_config -DLIBC"
            ld_library="-lc -lm" ;;
        "KERNEL")
            cc_config="$cc_config -DKERNEL -nostdlib -nostdinc -fno-builtin" ;;
        "COVERAGE")
            cc_config="$cc_config --coverage"
            ld_config="$ld_config --coverage" ;;
        "VERBOSE")
            verbose="/dev/stdout" ;;
    esac
done

#############################
## create output directory ##
#############################
if [ -d $out_dir ]
then
    rm -rf $out_dir
    echo "Remove last build directory $out_dir"
fi
mkdir -p $obj_dir
mkdir -p $bin_dir

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

#######################
## Generate Makefile ##
#######################
echo "    Makefile .. compile"
perl script/produce_compile_makefile.pl $src_dir

####################################
## compiling object file function ##
####################################
function obj_compile() {
    cd $1 > /dev/null
    make "cc_config=$cc_config" > $verbose
    mv *.o $obj_dir

    if [ "$?" != 0 ]
    then
        exit 3
    fi

    ## else
    ##     mv *.o $obj_dir
    cd - > /dev/null 
}

########################################
## compiling all other subdir .o file ##
########################################
module_list=$(find src/* -type d | grep -v inc)
for var in ${module_list[@]}
do
    echo "    Compile  .. $(basename $var)"
    obj_compile $var
done

########################################################
## generate linking Makefile and link to final target ##
########################################################
echo "    Copy     .. Makefile"
cp $src_dir/Makefile.in $obj_dir
echo "    Copy     .. libds.a"
cp $lib_dir/* $obj_dir
perl script/produce_link_makefile.pl $obj_dir
echo "    Link     .. scil.elf"
cd $obj_dir && make "ld_config=$ld_config" "ld_library=$ld_library" > $verbose

if [ "$?" != 0 ]
then
    exit 4
else
    cd - > /dev/null
fi

#################################
## Update tags and cleanup log ##
#################################
ctags -R src

########################################
## End of build script execution flow ##
########################################

