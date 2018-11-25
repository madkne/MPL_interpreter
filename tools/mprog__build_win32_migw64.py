#----------------------init
import os
from os import path
import glob
import sys
import shutil
#from subprocess import call

os.system("cls");
os.system("color 1f");
#----------------------define vars
#enable warnings :  -Wall -Wextra
cflags="-I ../include -std=c99  -fmax-errors=2 -c ";
build_folder="../win32-release";
obj_folder="../mprog/obj";
scr_folder="../mprog";
compiler="gcc ";
is_error=0;
logfile="mprog__build_win32_mingw64_list.txt";
compfiles=[];
writefiles=[];
#----------------------open mprog__build_win32_mingw64_list.txt
if os.path.exists(logfile):
	f=open(logfile,"r");
	compfiles=f.readlines();
	f.close();
#for j in compfiles:
#	print(compfiles)
#----------------------
print("\t~~~~~MPROG-MPL Build Tool (BY Python3) V 1.0~~~~~");
print("=== Start Building win32 release of mprog Program using Mingw64....");
#-----delete all obj/.*o
#if os.path.exists(obj_folder):
 #   objs=os.listdir(obj_folder);
  #  for ob in objs:
   #     os.remove("obj\\"+ob);
#-----create obj file
if not os.path.exists(obj_folder):
    os.makedirs(obj_folder);

#-----delete mahdi.exe
if os.path.exists(build_folder+"\\bin\\mprog.exe"):
    os.remove(build_folder+"\\bin\\mprog.exe ");

#----------------------compile mprog sources
print("=== Start compiling source files [mprog.exe]...");
sources=[
[scr_folder+"/main.c",scr_folder+"/main.c -o "+obj_folder+"/main.o"]
]

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
print("=== Start linking object files [mprog.exe]...");
if is_error==1:
    os.system("color C0");
    print("*** Failed Compiling! ***");
    #----------------------pause
    os.system("pause");
else:
    obj_files=glob.glob(obj_folder+"/*.o");
    all_files=' '.join(obj_files);
    is_error=os.system("gcc "+all_files+" -o "+build_folder+"/mprog.exe");
    
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
	#os.system("gdb "+build_folder+"\\bin\\mprog.exe");
	#----------------------run mpl.exe
	print("=== Running mprog.exe ...");
	print("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_");
	os.system("..\\win32-release\\mprog.exe");
	#os.system("dir");
	#os.system("pause");
	
	
	
	






