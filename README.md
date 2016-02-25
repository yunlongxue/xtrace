#平台:  Linux64

#软件:  xdemo/xtrace

#docker:  https://hub.docker.com/r/xueyunlong/xtrace/

#手册：
```sh
  xtrace -o src_file: 打开对c/c++源代码src_file调试
  xtrace -c src_file: 关闭对c/c++源代码src_file调试
```
```sh 
  1. xtrace是源码级调试器：
        * 支持c/c++源代码
        * 支持ESQL/C等嵌入式C源码
        * 支持仅对部分代码进行调试
        * 支持多进程/多线程
        * 支持so模式
        * 可以在任意用户下使用。
  2. 与gdb的区别：
        * 2.1： xtrace是源码级调试器，gdb是二进制码级别调试器。
              当一个c源码xdump.c文件中出现除0错误，程序异常终止； xtrace可以直接定位引起该异常的源代码位置，包括源代码文件名，函数名，在源哪一行，如果是在循环中，能够确定是第几次循环引起该异常。gdb则需要bt指令后，再具体分析。
        * 2.2： xtrace是嵌入式源代码调试器，gdb是交互式调试器
              用xtrace可以调试ESQL/C， pro*c源码，gdb在调试时，如果使用步进调试，遇到EXEC SQL...语句， 会卡住。
              用xtrace可以在不改变程序时序情况下，调试多进程/多线程程序。 使用gdb，在attach到进程过程中，由于操作的延时，则可能改变进程的时序，导致影响结果的准确性。
```                

#示例：
  * 示例源码在xdemo目录中，该目录同时包含了一个已编译好的xtrace
```sh    
  cd xdemo
  xtrace -o xdump.c #open xtrace
  cc xdump.c -o xdump;
  ./xdump
  xtrace -c xdump.c #close xtrace
```    
  * xdump输出：
```sh
XTRACE|27142|Process (ppid=27065) start at: 2015-3-3:21-32-5
XTRACE|27142|xdump.c-15-main|1|0s|0s-523us|
XTRACE|27142|xdump.c-16-main|1|0s|0s-160us|
XTRACE|27142|xdump.c-10-fun|1|0s|0s-135us|
XTRACE|27142|xdump.c-11-fun|1|2s|2s-230us|
XTRACE|27142|xdump.c-18-main|1|2s|0s-253us|
XTRACE|27142|xdump.c-19-main|1|2s|0s-129us|
XTRACE|27142|xdump.c-20-main|1|2s|0s-145us|
XTRACE|27142|xdump.c-19-main|2|2s|0s-126us|
Floating point exception (core dumped)
```
  * 根据输出信息：
    1. 程序开始时间：2015-3-3:21-32-5
          程序开始运行到完成，使用时间：2s
    2. 程序的执行路径如下:
          main ->  fun  ->  main
    3. 程序的性能瓶颈在函数fun， 文件第11行，本处执行用了时间：2s
          性能瓶颈代码：sleep( 2 );
    4. 程序在第19行第二次循环后产生异常。
          所以dump.c在第20行，for循环执行第二次循环时候，程序出现异常。
