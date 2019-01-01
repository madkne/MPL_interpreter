#----------------------init
import os
from os import path
import glob
import sys
import shutil
#from subprocess import call

#-----------------------------
#python3 modules_build_win32_migw64.py
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


def compile_module(name,src_dir,list):
		#----------------------compile mod sources
		print("=== Start compiling "+name+" module files ["+name+".dll]...");
		for i in range(0,len(list),1):
			ind=list[i];
			# mtime=src_dir+os.path.getmtime(ind[0]);
			# if len(compfiles)>i and float(compfiles[i]) == mtime:
			# 	print("=> Before Compiled: "+ind[0]);
			# else:
				#print(mtime,compfiles[i]);
			is_error=os.system(compiler+cflags+src_dir+ind[1])==1;
			print("=> Compiled: "+ind[0]);
				
			# writefiles.append(mtime);
		#----------------------save modified date of source files
		# fi=open(logfile,"w+");
		# for kl in writefiles:
		# 	#print(kl)
		# 	fi.write(str(kl)+"\n");
		# fi.close();
		#----------------------link object files
		print("=== Start linking object files ["+name+".dll]...");
		if is_error==1:
			os.system("color C0");
			print("*** Failed Compiling! ***");
			#---------pause
			os.system("pause");
		else:
			obj_files=glob.glob(obj_folder+"/*.o");
			all_files=' '.join(obj_files);
			is_error=os.system("gcc "+lflags+build_folder+"/"+name+".dll "+all_files);
			print("gcc "+lflags+build_folder+"/"+name+".dll "+all_files);
			#----------------------finish
			if is_error==1:
				os.system("color C0");
				print("*** Failed Linking! ***");
				#---------pause
				os.system("pause");
			else:
				#---------delete all *.o
				objs=os.listdir(obj_folder);
				for ob in objs:
					os.remove(obj_folder+"/"+ob);
#-------------------------------functions------------------------------------

os.system("cls");
os.system("color 1f");
#----------------------define vars
#enable warnings :  -Wall -Wextra
cflags="-I ../include -std=c99  -fmax-errors=2 -c ";
build_folder="../win32-release/modules";
obj_folder="../mod_obj";
scr_folder="../src/modules";
lflags=" -shared -o ";
compiler="gcc ";
is_error=0;
logfile="modules_build_win32_mingw64_list.txt";
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
print("\t~~~~~MPL Modules Build Tool (BY Python3) V 1.6~~~~~");
print("=== Start Building win32 release of Modules using Mingw64....");
#----------------------init dirs
#-----create obj file
if not os.path.exists(obj_folder):
    os.makedirs(obj_folder);

#----------------------compile fs mod sources
compile_module("fs",scr_folder+"/",[
	["fs.c","fs.c -o "+obj_folder+"/fs.o"],
	["../tools/syscalls.c","../tools/syscalls.c -o "+obj_folder+"/syscalls.o"],
	["../tools/utf8.c","../tools/utf8.c -o "+obj_folder+"/utf8.o"],
	["../tools/strings.c","../tools/strings.c -o "+obj_folder+"/strings.o"],
	#["../data_defined.c","../data_defined.c -o "+obj_folder+"/data_defined.o"],
	["../tools/common_funcs.c","../tools/common_funcs.c -o "+obj_folder+"/common_funcs.o"]
])





