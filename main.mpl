
//import "file:$/س.mpl"
//import "file:$/sss/gh.mpl"
import "mod:@/fs"
import "mod:@/sqlite"
//$con["DebugMode"]=true
//__config["AppName"]="MyProgram"
/*num jg=45;
j++
__config["AppName"]="dfhfg"*/
/*func hello(str s[?], num n,gg df,vars pa){
	print("struct is : ",df,"\n----\n")
	str fg=input(2)
	if(fg=="hello")print("............\n")
	return 45,fg
}*/
struct aaaaaa{
		num n,
		bool b
	}
struct gg{
		bool bb[?],
		num n
	}
	struct gg1{
		gg g,str u
	}

	
	/*func samp(num n){
		return n * 2
	}*/
	
	struct zz{
	num j,gg vb[2]
	}

/*func fib(num n){
	if (n != 0){
		num bb= n * fib(n-1); // recursive call
		print("##:%n%=>%bb%\n");
		return n * fib(n-1);
		//return bb;
	}
	else
		return 1;
}*/

//$con['DebugMode']=true
func main(){
	str s="hi\'fg"
	//str dr[?]={'er','سلام','امین'}
	//str ss='سلام به دنیا'+'!'
	//num n[4,3],str s,s={{3,-8,9.78},{(24^3)/4+56.5,8f,null},{0x45,0o34+4,0b01011},{-3454,45h,12.4i}},"سلام"+" !"
	num j,j1,j2[3]=10,29,{67,9.6787,-35}
	//num jj[?,?]={{0x45,0o34+4,0b01011},{-3454,45h,12.4i}}
	//gg gh[?]={struct({{"A","B"},{"AA","BB"}},6),struct({{"D","E"},{"DD","EE"}},-56*9)}
	//gg1 st1[?]={struct(struct({true,false},1000),"Hello"),struct(struct({true,false},20),"Amin")}
	num sql=sqlite::open('$/mydb.db')
	//sqlite::exec(sql,"create table product(id int PRIMARY KEY,name varchar(30),price int);")
	str op[?,?]=sqlite::exec(sql,"INSERT INTO product(id,name,price) VALUES(2,\'Reza\',66);")
	str ui[?,?] =sqlite::exec(sql,"select * from product")
	bool isclose=sqlite::close(sql)
	//print(sqlite::LibVersion(),sql,isclose,'\n')
	//str mm[1,1,1]={{{"SSSSSSS"}}}
	print(ui,'\n',mm[0,0,0])
	//zz st3[?]={struct(23,{struct({true,true},1000),struct({true,true},3500)})}
	//aaaaaa st2=struct(10,true);
	//num ll[3,2]={{45,45},{45,78},{5756,0}}
	//print(j2,j1)
	//gg1 fg=struct(struct({true,false},1000),"Hello");
	//str jj=fs::abspath("$/sss")
	//str jj="Hello"+"!"
	//num jk=samp(5)
	//gg cv=null;
	//cv=st3[0].vb[0]
	//str cv;
	//print("DD:\t67\n")
	//$ses['hi']={5f,7,8+2,0x45,78}
	//$ses['ss']=s+"سلام به همه\n"
	//$ses['gh']=ss
	//$def['df']=j2//{struct(2+3,{struct({true&&false,true},1000),struct({true,true},3500)})}//struct ({true~~false,true},10*70)//{{0x45,0o34+4,0b01011},{-3454,45h,12.4i}}//{56+4,89}//struct ({true~~false,true},10*70)//j2//st3[0]
	//cv=st1[1].u
	//cv=struct(100,true);
	//print(jj,"\n")
	//bool j3[2]
	//j3=st1[1].g.bb
	//print(cv,"\n")
	//bool io=st1[1].g.bb[0]~~st3[0].vb[0].bb[1] //true~~false=true
	//j2[j1]//st1[st2.n].g.n;//st3[0].vb[1].n;//(st1[1].g.n*33)+1000//st1[1].g.n;//st1[st2.n].g.n; //st1[1].g.bb[1];
	//error_handle(-2,"myerror","this is my error")
	//num js=rshift(56,4)
	//print($def['df'],"\ntyu"+"!")
	//str s=shell("cls")
	//print("Dirs:\n",s,"\n")
	//loop(str s1="67",str s2;j>0;j--) {
	//	print("====>%j%:%s1%,%s2%\n");
		
		//loop(num z=0;z<3;z++) {print("WWWW:%z%\n");}
	//}
	//print("j:%j% , j1:%j1%\n")
	//num ns=push(gh,struct({{"Ac","Bc"},{"Ac","Bc"}},6000),2)//__define["LeftDirect"] push(jj,"uiuiuiuiui",2)
	//print(gh,"\n")
	/*if(jj=="Hello!") {
		if(j<40)print("11\n")
		if(j>40) print("12\n")
		
	}
	elif(jj=="Hello!"&&true)print("2\n")
	else {
	print("3\n")
	print("4\n")
	}
	
	jj++*/
	//elif(j=="h")print("ffffff")
	/*exception e;
	manage(e){
		num n=5/0
		str sdsds="!!!!!!!"
	
	}*/
	//--jj
	//j1++
	//--jj
	//num q=1
	//num e1=(45*q++) //=>e1=45,q=2
	//q=1
	//num e2=(45*++q)
	//bool bb[2]={true,false}
	//gg ss1=struct({true,false},56.7)
	//gg ss2=struct(bb,67)
	//ss1:=ss2
	//jj="ZZZZZZZZZZ%j%ZZZZZ"
	//fg=gh
	//fg=struct(struct({{"A","B"},{"AA","BB"}},1000),"Hello")
	//j,jj+= 4,"---"
	//j:=j2
	//j:=j2[1]
	//jk[67-45]=12
	//num i1,str i2=hello({"ff","gg","ll"},&j,struct({true,false},56.7),"strkk",j)//{{"Q","A"},{"F",("rt")+"!"}}
	//bool bb[2]={(j>=56)&&false,false||true}
	//__define["OSInfo"]=jj+" !..."
	//str gh=__define["AppPath"]+__config["AppVersion"]
	

	
	//gg ss=struct(bb,67)
	//gg1 ss1[2]={struct(ss,"rt"),struct(struct(true,123424),"Amin")}
	//str s="Hello to %j% !!!"+" ..."
	/*print("Hello World!\n")
	j1++
	
	gg v[3]={struct({true,false},6),struct(true,-56*9),struct({true},0x45)}
	if(rt&&jk){
	
		loop(num i=0;i<len(e);i++){
			printS(e[i])
			if(true) zx="FFFF";
		}
		manage(u){
			num j=0;
		}
	}elif("jk"){
		t++
		n--
	}else 
		bn+=45.7
	
	bn-=5;*/
		
}
