# scil

### Simple Compiler Implementation for Learning

##### Build Binary
```
Build Script Usage:
    sh src/script/build.sh DEBUG/RELEASE
                           X86_64/X86_32
                           LIBC
                           VERBOSE       -optional
                           PROFILE       -optional
                           COVERAGE      -optional
```

##### Output
    output/bin/tokenizer.elf
    
##### Usage
    ./output/bin/tokenizer.elf [file-list]
    Sample and output layout:
```
>> cat data/hello_world.c
int
main(int argc, char **argv)
{
    int i = 0;
    i++;
    printf("Hello, World.\n");
    return i;
}

>> ./output/bin/tokenizer.elf data/hello_world.c
>>>>>> tokenizer analysis 'data/hello_world.c' start:
KWRD int
IDTR main
PCTT (
KWRD int
IDTR argc
PCTT ,
KWRD char
OPTR **
IDTR argv
PCTT )
PCTT {
KWRD int
IDTR i
OPTR =
CNST 0
PCTT ;
IDTR i
OPTR ++
PCTT ;
IDTR printf
PCTT (
CNST "Hello, World.\n"
PCTT )
PCTT ;
KWRD return
IDTR i
PCTT ;
PCTT }
<<<<<< analysis token end.

  >> SCIL Memory Usage Summary <<
    . Malloc        1956
    . Realloc          9
    . Free          1956
  >> Memory End <<

  >> LIBDS Memory Usage Summary <<
    . Malloc           0
    . Realloc          0
    . Free             0
  >> Memory End <<
```
  
    If no args are given, will run default test.
```
>> ./output/bin/tokenizer.elf
  >> SCIL Memory Usage Summary <<
    . Malloc        2463
    . Realloc         13
    . Free          2463
  >> Memory End <<

  >> LIBDS Memory Usage Summary <<
    . Malloc           0
    . Realloc          0
    . Free             0
  >> Memory End <<
```

##### Debug log
   If specific DEBUG macro, will create one file named tokenizer.elf.log. It will print something the tokenizer has done.

   
