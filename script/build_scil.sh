#!/bin/bash

## addition cc and ld config ##
cc_config="-fPIC"
ld_config="-Wl,--stats"
ld_library=
debug_mode="Yes"
verbose="/dev/null"
tkz="tkz"
parser="parser"

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
bin_dir=$out_dir/bin


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
            cc_config="$cc_config -g3 -ggdb -DDEBUG"
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
        "PROFILE")
            cc_config="$cc_config -pg"
            ld_config="$ld_config -pg" ;;
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


###########################
## build scil dependence ##
###########################
sh $base/script/build_scil_dependence.sh $base $debug_mode $verbose


####################################
## compiling object file function ##
####################################
function obj_compile() {
    # cd $1 > /dev/null
    make -f $1/Makefile "cc_config=$cc_config" > $verbose

    if [ "$?" != 0 ]
    then
        exit 3
    fi

    ## else
    ##     mv *.o $obj_dir
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
mv -v *.o $obj_dir > $verbose


###############################
## generate linking Makefile ##
###############################
echo "    Generate .. Link Makefile "
perl script/produce_link_makefile.pl $obj_dir

cd $obj_dir
echo "    Link     .. scil.elf"
make -f Makefile "ld_config=$ld_config" "ld_library=$ld_library" > $verbose

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

