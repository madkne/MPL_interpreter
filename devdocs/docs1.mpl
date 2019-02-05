		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
		| Mini Portable Language - MPL  |
		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

-----------------------------
Influence by:
C,PHP,Mahdi
-----------------------------
Keywords:
$con,$def,$ses
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
- mpb : mpl package bytecode files
- mdb : mpl database files
- mdo : mpl documentation files
- mcf : mpl configuration files
-----------------------------
Applications:
--built-in
- mpl		: MPL Interpreter [OK]			[c]
- mdebug	: MPL Debugger [OK]				[c]
- mbuild	: MPL Program Builder [OK]		[c]
--others
- mprog		: MPL Program Manager [..]		[mpl]
- mdoc		: MPL Documentation				[mpl]
- mmod		: MPL Module Executer			[mpl]
-----------------------------
-----mmod example:
> mmod strs str_split "Hi,hey,hello" ","
: {"Hi","hey","hello"}
> mmod math cos 180
: -1
> mmod fs abspath ./hey.txt
: E:\sam\hey.txt
-----:=
- work for values,arrays,structs
- change and replace pointer of two variables
- and for values change their no its pointers
num k1={4,8,9} //var_memory.pointer_id=4
num k2={41,81,91} //var_memory.pointer_id=7
k1:=k2  //k1.pointer_id=7;k2.pointer_id=4;
k1[0]:=k2[2]
-----------------------------
built-in structs :
exception
-----------------------------
built-in functions:
-------MPL functions:
--Completed:
typeof(var),push(var[..],var[..],num)/*just for var[?]*/,ErrHandle(num,str,str),ConAll(),DefAll(),DefIsset(str),SesAll(),SesIsset(str),len(var[..]),type(var[..])
--Not Completed:
pop(var[..],num)/*just for var[?]*/,del(var[..]),crop(var[..],num,num),DefUnset(str),SesUnset(str),EmbedRun(str[?]),exec(str),echo(var)/*just used in embed files*/
--TMP
TraceVar(var[?,..]),TraceFunc(str,num)
-------DATA functions:
--Completed:
and(num,num),xor(num,num),or(num,num),not(num),rshift(num,num),lshift(num,num),at(str,num),into(str,num,str),tnum(var),tbool(var),tstr(var),inum(str),ibool(str),tarray(var)
--Not Completed:
-------OS functions:
--Completed:
exit(num),print(val|var[..]),input(num),shell(str),time(),rand(num,num),argvs(),printf(str,var[..])
--Not Completed:
--TMP
modfree(str)
-----------------------------
built-in constants:(by $def)
- not edited and just use
--Completed:
ERROR:num,WARNING:num,OSType:str,OSArch:str,PathSeparator:str,EOF:num,EOL:str,IntSize:num,FloatSize:num,MplVersion:str,AppPath:str,LeftDirect:num,RightDirect:num,BothDirect:num,ReadChar:num,ReadLine:num,ReadAll:num,PID:num,StartedTime:num,HostName:str,MplPath:str
--Not Completed:
-----------------------------
built-in config:(by $con)
- can use and just edited in global and outside of functions
ErrorsState,WarningsState,ExportLogFile,MaxHugeDivideSteps,MaxHugeDecimalNumbers,AppVersion,AppName,AppLicense,AppCreator,SessionMode,OverwriteBuiltinMode,DebugMode,SessionDatabasePath,ExportBuildFile,AppIcon,BuildMode
--- just used for package files:
PackageMode,AccessVariablesMode,NameSpace,ExportByteCode
-- in feature:
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
- symbols for import files : $=>project root path , $$=>mpl root dir , @=>mpl modules dir , @@=>mpl packs dir 
import "file:$/data/data1.mpl" //import from project root($)
import "mod:$/sqlite" //without any extension .so or .dll
//import "embed:c:/java/main.java"
import "pack:$/libs/lib1.mpl"
import "mod:@/fs" //go to modules dir from mpl exe dir(@) and import fs module
import "file:$$/samples/sam1.mpl" //go to mpl dir ($$) and import sam1.mpl from samples dir
-----magic macros
- just can get a value of str,num or bool and array or struct but dont analyze it.
- defines alone not by other variables like num j,$def["jj"]=45,6.7
- can not access to sub values like $def['fg'][1,3] or $ses['df'].good . that's error.
-----$con:
- $con just edit in global. but it use anywhere.
$con["ErrorsState"]="fatals"
$con["ErrorsState"]="fatals"
-----$def:
- $def just define for a key(label) just once.
$def['er']=struct(45,true&&false)
$def['l1']=45
num k1=$def["l1"]
-----$ses:
- $ses can edit anywhere of program and its data is not destroy when exit app. it's data saved in a database file in project_root and then you can change or use it. (it's like a registery for apps)
$ses["gh"]=56.76
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
-----break,next
- break instruction can give a number that determine how many break from multiple loops
- next instruction can not gives any numbers!
loop(..){
	loop(..){
		a=b;
		break(2);
	}
	next;
}
-----review array in loop
- in review array just important in v[2,3,4] : 2 times for loop , 3 variables for allocing
num v[2,3,4]
loop(num i1[4],i2[4],i3[4];i1,i2,i3:v) //i1[4]=v[0,0],i2[4]=v[0,1],i3[4]=v[0,2]
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
- vars just include num,bool,str values and not arrays
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
call a function of a module like fs:
fs::fread_line(56)
call a constant variable of a module like math:
math::PI
-----sqlite.dll mod:
import "mod:@/sqlite"
num sql=sqlite::open('$/mydb.db')
sqlite::exec(sql,"create table product(id int PRIMARY KEY,name varchar(30),price int);")
sqlite::exec(sql,"INSERT INTO product(id,name,price) VALUES(2,\'Reza\',66);")
str ui[?,?] =sqlite::exec(sql,"select * from product")
bool isclose=sqlite::close(sql)

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
	bool su=embedrun("./main.c","./main.out") //=>run 'main.c' and then put results in 'main.out' file and return true if success running.
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
import "pack:$/lib1" // or import "pack:$/lib1.mpb"
func main(){
	num j=mylib->sum(6,7,8.8)
	num l=mylib->jk
}
-----lib1.mpb
-------------main.mpl can not have a main function
$con["NameSpace"]="mylib"
$con["ExportByteCode"]="$/libs/mylib.mpb"
$con["PackageMode"]=true //it is required
num jk=0
func sum(vars nums){
return 0;
}