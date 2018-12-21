-----------------------------
external modules:
sqlite,math,mgt,net,hack,fs,os

------------------module:math
. PI
. E

- num sqrt(num) : calculate root 2 of a num

- num factorial(num)

- num sin(num)

- num cos(num)

- num tan(num)

- str eval(str) //eval("5*7+4")

- num round(num) //3=round(2.8)

- num abs(num)

- num floor(num) //2=floor(2.8)

- num log10(num)

- num acos(num)

- num asin(num)

- num atan(num)

- num sinh(num)

- num cosh(num)

- num tanh(num)

- num degrees(num)

- num radians(num)

------------------module:os

- str homedir()

- num uptime()

- str[?] osinfo()

- str[?] cpuinfo()

- str[?] meminfo()

- bool shutdown(num)

- bool pstart(str,str[?])

------------------module:fs
- bool mkdir(str)

- num fopen(str,str)

- bool fclose(num)

- num fwrite(num,str)

- str fread_all(num)

- str fread_line(num)

- str fread_until(num,str)

- str fread_until(num,num)

- str[?] finfo(str)

- str abspath(str)

- bool copy(str,str)

- bool move(str,str)

- bool exist(str)

- bool remove(str)

- str[?,2] scandir(str,num)

- bool isdir(str)

- bool isfile(str)

- str parentpath(str)