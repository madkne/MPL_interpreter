-----------------------------
external modules:
sqlite3,math,fs[..],os,strs,mhl,mgl

------------------module:mgl (mpl graphics library)
- str version()

------------------module:strs
- str version()

- str str_crop(str,num,num)

- str str_indexof(str,str,num)

- str str_uppercase(str)

- str str_lowercase(str)

- str str_trim(str,num)

- str str_replace(str,str,str)

- str str_split(str[..],str) 

- str str_contains(str,str)

- str str_join(str,str)

- str str_reverse(str)

------------------module:math
- num PI()
- num E()
- str version()

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

- num base_convert(num,num,bool)

------------------module:os
- str version()



- str homedir()

- num uptime()

- str[?] osinfo()

- str[?] cpuinfo()

- str[?] meminfo()

- bool shutdown(num)

- bool pstart(str,str[?])

------------------module:fs
- str version()



- str abspath(str)

- bool mkdir(str)

- num fopen(str,str)

- bool fclose(num)

- num fwrite(num,str)

- str fread_all(num)

- str fread_line(num)

- str fread_until(num,str)

- str fread_until(num,num)

- str[?] finfo(str)

- bool copy(str,str)

- bool move(str,str)

- bool exist(str)

- bool remove(str)

- str[?,2] scandir(str,num)

- bool isdir(str)

- bool isfile(str)

- str parentpath(str)


------------------module:mhl (mpl hacking library)
- str Version()

- dns_mapping()

- dns_tracing()

- whois_info()

- gathering_data(num type)