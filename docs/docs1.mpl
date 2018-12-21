		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
		| Mini Portable Language - MPL  |
		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

-----------------------------
Influence by:
C,PHP,Mahdi
-----------------------------
Keywords:
__config,__define,__session
true,false,null
if,elif,else,loop,manage
num,str,bool,struct,vars
next,break
import,func,return
-----------------------------
Operators:
+,-,*,/,%,^
==,<=,>=,!=,<,>
+=,++,--,-=,*=,/=,%=,^=,:=,&=,|=,~=,!=
!,&&,||,~~
-----------------------------
Extensions:
- mpl : mpl source files
- mbc : mpl bytecode files
- mdb : mpl database files
- mdo : mpl documentation files
- mcf : mpl configuration files
-----------------------------
Applications:
- mpl		: MPL Interpreter [OK]
- mdebug	: MPL Debugger [OK]
- mprog		: MPL Program Manager [..]
- madoc		: MPL Advanced Documentation
-----------------------------
-----:=
work for values,arrays,structs
change and replace pointer of two variables
and for values change their no its pointers
num k1={4,8,9} //var_memory.pointer_id=4
num k2={41,81,91} //var_memory.pointer_id=7
k1:=k2  //k1.pointer_id=7;k2.pointer_id=4;
-----------------------------
built-in structs :
exception
-----------------------------
built-in functions:
-------MPL functions:
--Completed:
print(val|var[..])[OK],typeof(var)[OK],input(num)[OK],push(var[..],var[..],num)[OK]/*just for var[?]*/
--Not Completed:
len(var[..]),error_handle(num,str,str),config_all(),define_all(),define_isset(str),embed_run(str[?]),argvs(),session_all(),session_isset(str),crop(var[..],num,num),search(var[..],var),mpl_execute(str),trace_var(var[?,..]),trace_func(str,num),echo(var)/*just used in embed files*/,var_type(var[..]),pop(var[..],num)/*just for var[?]*/,del(var[..])
-------DATA functions:
--Completed:
--Not Completed:
str_split(str[..],str) ,str_replace(str,str,str),to_num(var,bool),to_bool(var),to_str(var),at(var,num),str_at(str,num,str),str_crop(str,num,num),str_indexof(str,str,num),str_uppercase(str),str_lowercase(str),str_trim(str,num),is_num(var),is_bool(var),is_str(var),str_contains(str,str),str_join(str,str),bool_switch(bool),str_reverse(str),to_chars(var),base_convert(num,num,bool),bit_xor(num,num),bit_and(num,num),bit_or(num,num),bit_not(num),bit_cmp(num)
-------OS functions:
--Completed:
exit(num)
--Not Completed:
shell(str),time(),rand(num,num)
-----------------------------
built-in constants:(by __define)
- not edited and just use
--Completed:
ERROR:num,FATAL:num,WARNING:num,OSType:str,OSArch:str,PathSeparator:str,EOF:num,EOL:str,IntSize:num,FloatSize:num,MplVersion:str,AppPath:str,LeftDirect:num,RightDirect:num,BothDirect:num,ReadChar:num,ReadLine:num,ReadAll:num,PID:num,StartedTime:num,HostName:str
--Not Completed:
-----------------------------
built-in config:(by __config)
- can use and just edited in global and outside of functions
ErrorsState,WarningsState,ExportLogFile,MaxHugeDivideSteps,MaxHugeDecimalNumbers,TabSize,AppVersion,AppName,AppLicense,AppCreator,SafeMode,SessionMode,HelpArgumentMode,OptimizeMode,OverwriteBuiltinMode,DebugMode,ExportByteCode,RunOnlyOS,RunOnlyArch,NameSpace/*used for package files*/,SessionDatabasePath
-----------------------------
++,--:
str:("Hello")
	++:"Hello " (add a space end of string)
	--:"Hell" (remove last char of string)
num:(45.5)
	++:46.5 (plus num by 1)
	--:44.5 (subtraction num by 1)
samples:
num q=1
num e1=(45*q++) //=>e1=45,q=2
num e2=(45*++q) //=>e2=90,q=2
-----------------------------
null for data types:
num  k=null => k=null
str  k=null => k=""
bool k=null => k=false
st1  k=null => k=struct{null,"",false}
vars k=null => k=null
-----------------------------
control characters:
\n : new line
\t : insert tab
\" : print "
\\ : print \
\b : backspace
\a : play alert sound
-----------------------------
sample using keywords:
-----import:
import "file:$/data/data1.mpl" //import from project root($)
import "mod:$/sqlite" //without any extension .so or .dll
//import "embed:c:/java/main.java"
import "pack:$/libs/lib1.mpl"
-----magic macros
- just can get a value of str,num or bool and nothing else(no array,no struct)
- defines alone not by other variables like num j,__define["jj"]=45,6.7
-----__config:
__config["errors_mode"]="fatals"
-----__define:
__define["l1"]=45
num k1=__define["l1"]
-----__session:
- __session can edit anywhere of program and it data is not destroy when exit app. it's data saved in a database file in project_root and then you can change or use it. (it's like a registery for apps)
__session["gh"]=56.76
-----loop:
loop([section1];[section2];[section3])
[section1] : just run in first time (for define vars)
[section2] : run every time and decide that loop be continue (for check vars) [is required]
[section3] : run every time except first time (for change vars)
sample:
- loop(num i=0,str u=null;i<10,u!="Hello";i++)
- loop(str u,str b=null,num i=0;u,b:uu;i+=5,u+=" ") //str uu[2,2]={{"x","v"},{"h","i"}} browse uu array
- loop(;i<5) or loop(;i<5;)
- loop(num h=get1(),num k=0;h<get2(k);k++,h++)
- loop(str u,b=null,num i=0;u!="Hi") //=>make an error Correct:loop(str u=null,str b=null,num i=0;u!="Hi") OR loop(str u,str b,num i=null;u!="Hi")
-----review array in loop
num v[2,3,4]
loop(num i1,i2,i3;i1,i2,i3:v) //i1[4]=v[0,0],i2[4]=v[0,1],i3[4]=v[0,2]
num b=[5]
loop(num i;i:b) //i=b[0]
-----func headers:
- define func headers
func f(str s,s1,num n,vars others)
func f(str s,s1,num n) //=>it raised an error because it exist.(vars others) is not for defferent between funcs. because it can has not any value.
func f(str s,s1,vars others)
func f(str s[?],num n[2,?])
func f(str s[?],st1 s)
func f(st1 s)
- call func
f("g",6.8)
num j=f("g",6.8)[1]  //=>j get 2nd returned value
f({"F","K"},{{4,8.8,9},{4,null,null}})
num j1,j2,str j3=f("g",6.8) //=>j1,j2,j3 get 3 returned values
f("56",f("E","s",6)[0])[1]
num h=56;
f("S",&h)  //=>any change on h var be saved.function by refrence parameters
- main func has no parameters. and any override of this function is not call by mpl(system)
- mpl(system) just call 'main()' on startup. if in main func returned a value like 'return 1' , it run by 'exit(1)'.

-----struct:
- define a struct header like define a function header

struct st1{
	bool b[?],
	str s,
	num n
}
struct st2{st1 v,bool b}
struct st3{num n[2],bool b}
st1 h[2];
h[0]=struct(true,"ty",78.7);
push(h)
h[2]=h[0]
st2 j=struct(struct(false,"ui",-9.99h),true)
st3 f=struct({3.6,-8},true)
num jj=f.n[0];bool bb=f.b
num kl=h[1].n+f.n[0]
- you can call entries of a struct by '.'
num a=j.v.n[1]
-----str,num,bool:
str y="GH"
str h,num n[3,5],bool b[3];
str s,num n[2,2],bool b="SSS",{{4,8.8},{-9h,6.5f}},{true,false}
num j1,j2=56
num n={{2,7},{8.9,-9}} //=>n[2,2]
n={5,8}//=>raise an error
-----arrays
num j[4,7]
- you must define an array by its dimensions like: num k[3]={3,5,8}
- you can define an array by '?' dimension but you must assign it by a value like : num k[?]={5,7,8,8} //k[4]

-----vars:
- vars just include struct and num,bool,str values and not arrays
- vars just implement in function headers
- vars is not different between two override functions
- vars items can not a refrence variable
fg(5.7,3,"df",j,true,"HI")
func fg(num n,vars v){
	num l=len(v); //=>l=5
	loop(num i=-1;i<l;i++){
	}
}
-----manage:
- manage can get not any parameters like : manage() or manage(null)
- you can create complex manage structures.
exception e; //built-in struct => struct exception{num id,str msg,str group,num type,str src,num line};
manage(e){
//err1
//warning1
//err2
}
if(len(e)>0){
	loop(num i=0;i<len(e);i++){
		printS(e[i])
	}
}
-----switch:
- switch is a parser instruction and not a real structure. it convert to if,elif,else in parser mode
switch(n){
	if(3):
		print("hhhh\n")
		n++
	if(34||78):
		print("bbbbb\n")
		n--
	else:
		print("EEEEEE\n")
}

------------------------------using external modules
-----sqlite mod:
import "mod:sqlite"
str s[?,?]=sqlite::query("SELECT * FROM USERS");
str s1="hj"
str s2[?,?]=sqlite::query("SELECT %s1% FROM USERS");

------------------------------using in embedded files
-----main.c
#include <stdio.h>
int main(){
	printf("{%mpl  printA(argvs());  %}"); //=>printf("{[argv1],[argv2],..}");
	{%mpl
	loop(str s;s:argvs())
		echo(s+"\n");
	%}
	return 0;
}
-----main.mpl
//import "embed:$/main.c"
//import "embed:$/sam1.c"
func main(){
	bool su=embed_run("./main.c","./main.out") //=>run 'main.c' and then put results in 'main.out' file and return true if success running.
	if(su) print("Run Success!\n")
}
-----main.out 
#include <stdio.h>
int main(){
	printf("{[argv1],[argv2],..}");
	[argv1]
	[argv2]
	,...
	return 0;
}
------------------------------using in package files
-----main.mpl
import "pack:$/lib1.mpl"
func main(){
	num j=mylib->sum(6,7,8.8)
}
-----lib1.mpl
-------------lib1.mpl can not have a main function
__config["NameSpace"]="mylib"
func sum(vars nums){
return 0;
}