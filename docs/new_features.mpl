
********************************************MPL-HELLO new Features
×××××××××××××××××××××××
+++++++++++switch keyword [GOOD]
×××××××××××××××××××××××
num n=readme()
switch(n){
	if(3): //if(n==3)
		//do somethings...
	elif(5||6): //elif(n==5||n==6)
		//do somethings...
	else:
		//do somethings...
}
//------------------
switch(n){
	if(4):
		num m=read()
		switch(m){
			if(6.7): return 1;
			else: return 0;
		}
	elif(6||5): return 89
	else: return 0
	
}
×××××××××××××××××××××××
+++++++++++callback keyword [GOOD]
×××××××××××××××××××××××
num n=callback{
	readline()=="hello" =>first(readline), 	//if(readline()=="hello") n=first(readline)
	delta>=4 			=>78.8,
	null				=>0 				//else n=0
}
×××××××××××××××××××××××
+++++++++++comparison struct expressions
×××××××××××××××××××××××
st1 s1={0,"56",78}
st1 s2={7,"56",90}
if(s1==s2) //do somethings...
×××××××××××××××××××××××
+++++++++++development magic macros($def,$con,$ses) [GOOD]
×××××××××××××××××××××××
$def["er"]={{5,8},{3,7}}
$ses["rt"]=struct(0,56,true)
×××××××××××××××××××××××
+++++++++++development import
×××××××××××××××××××××××
//import "file:..."
//import "mod:...."
//import "pack:..."
import "wmod:dl.mpl-lang.ir/modules/fs" 
import "wpack:dl.mpl-lang.ir/packages/xxx" 
×××××××××××××××××××××××
+++++++++++magic keyword and data type [GOOD]
×××××××××××××××××××××××
magic bn;
$bn["k1"]="Hello"
$bn["k2"]=45;
$bn["k3"]=true;
$bn["k4"]={6,8,5}
num n=$bn["k2"]
×××××××××××××××××××××××
+++++++++++call function by named parameters [GOOD]
×××××××××××××××××××××××
func fn(num x,y,z){return x+y+z;}
num n=fn(x:45,z:12,y:7.89)
×××××××××××××××××××××××
+++++++++++alias keyword
×××××××××××××××××××××××
//------------------for functions
func vb(num j,str s) alias vbnet
{
	//do somethings...
}
//------------------for structs
struct struct_vb{
	num n,str j
} alias vbst;
func main(){
	num n=vbnet(5,"hi")
	vbst st=struct(56,"Hi");
}
×××××××××××××××××××××××
+++++++++++#predef keyword [GOOD]
×××××××××××××××××××××××
#predef def1(e1,e2){
	loop(num i=0;i<#e1;i++){#e2}
}
#predef ret(r){
	return (#r);
}
func main(){
	#def1(32,print("%i%:good:)\n")); //(runtime)=>loop(num i=0;i<32;i++){print("%i%:good:)}
	#ret(45) 						//(runtime)=>return (45);
}
×××××××××××××××××××××××
+++++++++++development vars keyword usage [GOOD]
×××××××××××××××××××××××
func fg(num n,vars v) //v can only num,str,bool
func fg1(num n,vars v?) //v can struct or array or num,str,bool
{
	num b=len(v?)
	str s[2]=var_type(v?[b]) //s={"num","3"}
	num m[?]=v?[b] //m={5,8,9}
}
func main(){
	fg1(4,5,struct(8,true),{5,8,9})
}
×××××××××××××××××××××××
+++++++++++%%...%% operators
×××××××××××××××××××××××
- %%...%% operators instead of error ocuurs
//--------------for import
import "pack:@/sample" 
%%import "wpack:sample.com/sample"%%
//--------------for unknown vars type
func fg(num n,vars s){
	s*=34 %%s+="Hi"%% //if s not a num and a str type, so ...
}
×××××××××××××××××××××××
+++++++++++development struct
×××××××××××××××××××××××
- using functions in struct make struct as a object
struct st1{
	str s,
	num n,
	func sum(num x){
		return n+x
	},
	func plus(){n++}
}
func main(){
	st1 ss;
	ss.sum(3) //make an error :( 
	ss=struct("hi",34)
	ss.plus() //ss.n=35
	
}
×××××××××××××××××××××××
+++++++++++development mmod program
×××××××××××××××××××××××

×××××××××××××××××××××××
+++++++++++development mdoc program
×××××××××××××××××××××××

×××××××××××××××××××××××
+++++++++++development mclang program
×××××××××××××××××××××××

×××××××××××××××××××××××
+++++++++++development mprog program [GOOD]
×××××××××××××××××××××××