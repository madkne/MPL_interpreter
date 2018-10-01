#----------------------init
import os
from os import path
import glob
#from subprocess import call

os.system("cls");
os.system("color 1f");
#----------------------define vars
#enable warnings :  -Wall -Wextra
cflags="-I .\\include -std=c99 -g  -fmax-errors=2 -c ";
build_folder="win32-release";
compiler="gcc ";
is_error=0;
logfile="build_win32_mingw64_list.txt";
compfiles=[];
writefiles=[];
#----------------------open build_win32_mingw64_list.txt
if os.path.exists(logfile):
	f=open(logfile,"r");
	compfiles=f.readlines();
	f.close();
#for j in compfiles:
#	print(compfiles)
#----------------------
print("\t~~~~~MPL Build Tool (BY Python3) V 2.1~~~~~");
print("=== Start Building win32 release of MPL Compiler using Mingw64....");
#----------------------init dirs
#-----create docs file
if not os.path.exists(build_folder+"\\docs"):
    os.makedirs(build_folder+"\\docs");
#-----create modules file
if not os.path.exists(build_folder+"\\modules"):
    os.makedirs(build_folder+"\\modules");
#-----create libs file
if not os.path.exists(build_folder+"\\packs"):
    os.makedirs(build_folder+"\\packs");
#-----create samples file
if not os.path.exists(build_folder+"\\samples"):
    os.makedirs(build_folder+"\\samples");
#-----delete all obj/.*o
#if os.path.exists("obj"):
 #   objs=os.listdir("obj");
  #  for ob in objs:
   #     os.remove("obj\\"+ob);
#-----create obj file
if not os.path.exists("obj"):
    os.makedirs("obj");

#-----delete mahdi.exe
if os.path.exists(build_folder+"\\bin\\mpl.exe"):
    os.remove(build_folder+"\\bin\\mpl.exe ");

#----------------------compile mpl sources
print("=== Start compiling source files [mpl.exe]...");
sources=[
["src/main.c","src/main.c -o obj/main.o"],
["src/data_defined.c","src/data_defined.c -o obj/data_defined.o"],
["src/exceptions.c","src/exceptions.c -o obj/exceptions.o"],
["src/built_in.c","src/built_in.c -o obj/built_in.o"],
["src/tools/common_funcs.c","src/tools/common_funcs.c -o obj/common_funcs.o"],
["src/tools/strings.c","src/tools/strings.c -o obj/strings.o"],
["src/tools/utf8.c","src/tools/utf8.c -o obj/utf8.o"],
["src/core/vars_mgr.c","src/core/vars_mgr.c -o obj/vars_mgr.o"],
["src/core/parser.c","src/core/parser.c -o obj/parser.o"],
["src/core/importer.c","src/core/importer.c -o obj/importer.o"],
["src/core/starter.c","src/core/starter.c -o obj/starter.o"],
["src/core/run_mgr.c","src/core/run_mgr.c -o obj/run_mgr.o"],
["src/core/virtual_memory.c","src/core/virtual_memory.c -o obj/virtual_memory.o"]
];
for i in range(0,len(sources),1):
	ind=sources[i];
	mtime=os.path.getmtime(ind[0]);
	if len(compfiles)>i and float(compfiles[i]) == mtime:
		print("=> Before Compiled: "+ind[0]);
	else:
		#print(mtime,compfiles[i]);
		is_error=os.system(compiler+cflags+ind[1])==1;
		print("=> Compiled: "+ind[0]);
		
	writefiles.append(mtime);
#----------------------save modified date of source files
fi=open(logfile,"w+");
for kl in writefiles:
	#print(kl)
	fi.write(str(kl)+"\n");
fi.close();
	
#----------------------link object files
print("=== Start linking object files [mpl.exe]...");
if is_error==1:
    os.system("color C0");
    print("*** Failed Compiling! ***");
    #----------------------pause
    os.system("pause");
else:
    obj_files=glob.glob("obj/*.o");
    all_files=' '.join(obj_files);
    is_error=os.system("gcc -g "+all_files+" -o "+build_folder+"\\mpl.exe");
    
#----------------------finish
if is_error==1:
    os.system("color C0");
    print("*** Failed Linking! ***");
    #----------------------pause
    #os.system("pause");
else:
	os.system("color A0");
	print("=== Finish Building! All Done in "+build_folder+" folder");
	#----------------------run gdb
	#print("=== Running GDB ...");
	#os.system("gdb "+build_folder+"\\bin\\mpl.exe main.mpl");
	#----------------------run mpl.exe
	print("=== Running mpl.exe ...");
	print("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_");
	os.system(build_folder+"\\mpl.exe main.mpl");
	#os.system("dir");
	#os.system("pause");




