#----------------------init
import os
from os import path
import glob
import sys
import shutil
#from subprocess import call

#-----------------------------
#python3 build_win32_migw64.py
#-----------------------------
#-------------------------------functions------------------------------------
def copy_dir(src,dst,folder):
	dirs=[folder];
	i=0;
	line_counter=0
	while True:
		if(i==len(dirs)):break;
		#print("Dir:"+dirs[i],i,len(dirs),src,dst)
		item=dirs[i]
		#del dirs[i]
		with os.scandir(src+"/"+item) as it:
			for entry in it:
				if entry.is_file():
					#print("Cfile:",dst+"/"+item+"/"+entry.name);
					if os.path.exists(dst+"/"+item+"/"+entry.name):
						os.remove(dst+"/"+item+"/"+entry.name);
						shutil.copyfile(src+"/"+item+"/"+entry.name,dst+"/"+item+"/"+entry.name)  
				elif entry.is_dir():
					#print("Cdir:",dst+"/"+item+"/"+entry.name);
					if not os.path.exists(dst+"/"+item+"/"+entry.name):
						os.makedirs(dst+"/"+item+"/"+entry.name);
					dirs.append(item+"/"+entry.name)
		i+=1


#-------------------------------functions------------------------------------

os.system("cls");
os.system("color 1f");
#----------------------define vars
#enable warnings :  -Wall -Wextra
cflags="-I ../include -std=c99  -fmax-errors=2 -c ";
build_folder="../win32-release";
obj_folder="../obj";
scr_folder="../src";
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
print("\t~~~~~MPL Build Tool (BY Python3) V 2.7~~~~~");
print("=== Start Building win32 release of MPL Compiler using Mingw64....");
#----------------------init dirs
#-----create docs file
if not os.path.exists(build_folder+"\\docs"):
    os.makedirs(build_folder+"\\docs");
copy_dir("../", build_folder,"docs");
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
#if os.path.exists(obj_folder):
 #   objs=os.listdir(obj_folder);
  #  for ob in objs:
   #     os.remove("obj\\"+ob);
#-----create obj file
if not os.path.exists(obj_folder):
    os.makedirs(obj_folder);

#-----delete mahdi.exe
if os.path.exists(build_folder+"\\bin\\mpl.exe"):
    os.remove(build_folder+"\\bin\\mpl.exe ");

#----------------------compile mpl sources
print("=== Start compiling source files [mpl.exe]...");
sources=[
[scr_folder+"/main.c",scr_folder+"/main.c -o "+obj_folder+"/main.o"],
[scr_folder+"/mpl_help.c",scr_folder+"/mpl_help.c -o "+obj_folder+"/mpl_help.o"],
[scr_folder+"/data_defined.c",scr_folder+"/data_defined.c -o "+obj_folder+"/data_defined.o"],
[scr_folder+"/exceptions.c",scr_folder+"/exceptions.c -o "+obj_folder+"/exceptions.o"],
[scr_folder+"/debugger.c",scr_folder+"/debugger.c -o "+obj_folder+"/debugger.o"],
[scr_folder+"/built_in.c",scr_folder+"/built_in.c -o "+obj_folder+"/built_in.o"],
[scr_folder+"/tools/common_funcs.c",scr_folder+"/tools/common_funcs.c -o "+obj_folder+"/common_funcs.o"],
[scr_folder+"/tools/strings.c",scr_folder+"/tools/strings.c -o "+obj_folder+"/strings.o"],
[scr_folder+"/tools/utf8.c",scr_folder+"/tools/utf8.c -o "+obj_folder+"/utf8.o"],
[scr_folder+"/tools/syscalls.c",scr_folder+"/tools/syscalls.c -o "+obj_folder+"/syscalls.o"],
[scr_folder+"/core/vars_mgr.c",scr_folder+"/core/vars_mgr.c -o "+obj_folder+"/vars_mgr.o"],
[scr_folder+"/core/parser.c",scr_folder+"/core/parser.c -o "+obj_folder+"/parser.o"],
[scr_folder+"/core/importer.c",scr_folder+"/core/importer.c -o "+obj_folder+"/importer.o"],
[scr_folder+"/core/starter.c",scr_folder+"/core/starter.c -o "+obj_folder+"/starter.o"],
[scr_folder+"/core/run_mgr.c",scr_folder+"/core/run_mgr.c -o "+obj_folder+"/run_mgr.o"],
[scr_folder+"/core/virtual_memory.c",scr_folder+"/core/virtual_memory.c -o "+obj_folder+"/virtual_memory.o"],
[scr_folder+"/core/magic_macro.c",scr_folder+"/core/magic_macro.c -o "+obj_folder+"/magic_macro.o"],
[scr_folder+"/built_in/mpl_builtin.c",scr_folder+"/built_in/mpl_builtin.c -o "+obj_folder+"/mpl_builtin.o"],
[scr_folder+"/built_in/os_builtin.c",scr_folder+"/built_in/os_builtin.c -o "+obj_folder+"/os_builtin.o"]
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
    obj_files=glob.glob(obj_folder+"/*.o");
    all_files=' '.join(obj_files);
    is_error=os.system("gcc "+all_files+" -o "+build_folder+"/mpl.exe");
    
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
	os.system("..\\win32-release\\mpl.exe ..\\main.mpl");
	#os.system("..\\win32-release\\mpl.exe -h keywords -l");
	#os.system("dir");
	#os.system("pause");
	
	
	
	






