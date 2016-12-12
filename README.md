# scil

### Simple Compiler Implementation for Learning

#### _TOKENIZER_

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
    format: [token type] [line number] [token]
    
##### Usage
    ./output/bin/tokenizer.elf [file-list]
    Sample and output layout:
```
>> cat -n data/hello_world.c
     1
     2  /*dsada
     3  dsadada
     4  */
     5
     6
     7  int
     8  main(int argc, char **argv)
     9  {
    10      int i = 0;
    11      i++;
    12      printf("Hello, World.\n");
    13      return i;
    14  }
    15
    16  // da
    17  // da
    18
    19
    20  /*
    21   * dsadsa
    22   * dadadada d
    23   *
    24
    25
    26   */
    27
    28

>> ./output/obj/tokenizer.elf data/hello_world.c
>>>>>> tokenizer analysis 'data/hello_world.c' start:
KWRD 7 int
IDTR 8 main
PCTT 8 (
KWRD 8 int
IDTR 8 argc
PCTT 8 ,
KWRD 8 char
OPTR 8 **
IDTR 8 argv
PCTT 8 )
PCTT 9 {
KWRD 10 int
IDTR 10 i
OPTR 10 =
CNST 10 0
PCTT 10 ;
IDTR 11 i
OPTR 11 ++
PCTT 11 ;
IDTR 12 printf
PCTT 12 (
CNST 12 "Hello, World.\n"
PCTT 12 )
PCTT 12 ;
KWRD 13 return
IDTR 13 i
PCTT 13 ;
PCTT 14 }
<<<<<< analysis token end.

  >> SCIL Memory Usage Summary <<
    . Malloc        2594
    . Realloc         11
    . Free          2594
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

---------------------


   
