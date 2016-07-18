#!/bin/sh
## addition cc and ld config ##
cc_config="-fPIC"
ld_config="-Wl,--stats"
ld_library=
debug_mode="Yes"

if [ $# == 0 ]
then
    cat << EOF
Build Script Usage:
    sh src/script/build.sh DEBUG/RELEASE
                           X86_64/X86_32
                           LIBC
                           PROFILE       -optional
                           COVERAGE      -optional
EOF
    exit 1
fi

base=$(pwd)
src_dir=$base/src
obj_dir=$base/output
bin_dir=$obj_dir/out
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
            cc_config="$cc_config -g3 -DDEBUG" ;;
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
    esac
done

#############################
## create output directory ##
#############################
if [ -d $obj_dir ]
then
    rm -rf $obj_dir
    echo "Remove last build directory $obj_dir"
fi
mkdir -p $bin_dir

###########################################
## update some header files and makefile ##
###########################################
perl script/generate_declaration.pl $src_dir $debug_mode
perl script/produce_compile_makefile.pl $src_dir

####################################
## compiling object file function ##
####################################
function obj_compile() {
    cd $1 > /dev/null
    make "cc_config=$cc_config" > /dev/null
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
module_list=$(find src/ -type d | grep -v inc)
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
perl script/produce_link_makefile.pl
echo "    Link     .. scil.elf"
cd $obj_dir && make "ld_config=$ld_config" "ld_library=$ld_library" > /dev/null

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

