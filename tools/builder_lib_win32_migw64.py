#----------------------init
import os
from os import path
import glob
import sys
import shutil
#from subprocess import call

#-----------------------------
#python3 builder_lib_win32_migw64.py
#-----------------------------
#-------------------------------functions------------------------------------
def copy_dir(src,dst,folder,just_contents):
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
					new_dst=dst+"/"+item+"/"+entry.name
					if item==folder: 
						new_dst=dst+"/"+entry.name
					#print("Cfile:",src+"/"+item+"/"+entry.name,new_dst);
					if os.path.exists(new_dst):
						os.remove(new_dst);
					ret=shutil.copyfile(src+"/"+item+"/"+entry.name,new_dst);
						#print("\nCOpy",ret)
						#if len(ret)<2 : print("Not copy :(");
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
#----------------------
print("\t~~~~~MPL Builder Library Tool (BY Python3) V 1.5~~~~~");
print("=== Start Building win32 release of Builder.dll using Mingw64....");
#----------------------init dirs
#-----delete all obj/.*o
if os.path.exists(obj_folder):
    objs=os.listdir(obj_folder);
    for ob in objs:
        os.remove(obj_folder+"\\"+ob);
#-----create obj file
if not os.path.exists(obj_folder):
    os.makedirs(obj_folder);
#----------------------compile mpl sources
print("=== Start compiling source files [Builder.dll]...");
sources=[
[scr_folder+"/builder.c",scr_folder+"/builder.c -o "+obj_folder+"/builder.o"],
[scr_folder+"/mpl_help.c",scr_folder+"/mpl_help.c -o "+obj_folder+"/mpl_help.o"],
[scr_folder+"/module.c",scr_folder+"/module.c -o "+obj_folder+"/module.o"],
[scr_folder+"/data_defined.c",scr_folder+"/data_defined.c -o "+obj_folder+"/data_defined.o"],
[scr_folder+"/exceptions.c",scr_folder+"/exceptions.c -o "+obj_folder+"/exceptions.o"],
[scr_folder+"/debugger.c",scr_folder+"/debugger.c -o "+obj_folder+"/debugger.o"],
[scr_folder+"/built_in.c",scr_folder+"/built_in.c -o "+obj_folder+"/built_in.o"],
[scr_folder+"/tools/common_funcs.c",scr_folder+"/tools/common_funcs.c -o "+obj_folder+"/common_funcs.o"],
[scr_folder+"/tools/console.c",scr_folder+"/tools/console.c -o "+obj_folder+"/console.o"],
[scr_folder+"/tools/strings.c",scr_folder+"/tools/strings.c -o "+obj_folder+"/strings.o"],
[scr_folder+"/tools/encoder.c",scr_folder+"/tools/encoder.c -o "+obj_folder+"/encoder.o"],
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
[scr_folder+"/built_in/os_builtin.c",scr_folder+"/built_in/os_builtin.c -o "+obj_folder+"/os_builtin.o"],
[scr_folder+"/built_in/data_builtin.c",scr_folder+"/built_in/data_builtin.c -o "+obj_folder+"/data_builtin.o"],
	[scr_folder+"/built_in/fs_builtin.c",scr_folder+"/built_in/fs_builtin.c -o "+obj_folder+"/fs_builtin.o"],
[scr_folder+"/modules/sqlite_interface.c",scr_folder+"/modules/sqlite_interface.c -o "+obj_folder+"/sqlite_interface.o"]
];
for i in range(0,len(sources),1):
	ind=sources[i];
	is_error=os.system(compiler+cflags+ind[1])==1;
	print("=> Compiled: "+ind[0]);

#----------------------link object files
print("=== Start linking object files [Builder.dll]...");
if is_error==1:
    os.system("color C0");
    print("*** Failed Compiling! ***");
    #----------------------pause
    os.system("pause");
else:
    obj_files=glob.glob(obj_folder+"/*.o");
    all_files=' '.join(obj_files);
    #print("gcc .\win32rc.res "+all_files+" -o "+build_folder+"/mpl.exe")
    is_error=os.system("ar rcs "+build_folder+"/builder.dll "+all_files);
    
#----------------------finish
if is_error==1:
    os.system("color C0");
    print("*** Failed Linking! ***");
    #----------------------pause
    #os.system("pause");
else:
	os.system("color A0");
	print("=== Finish Building! All Done in "+build_folder+" folder");
	#-----delete 'build_win32_mingw64_list.txt' file
	if os.path.exists('build_win32_mingw64_list.txt'):
		os.remove('build_win32_mingw64_list.txt')
	
	
	
	






