//
// Created by amindelavar on 9/19/2018.
//
#include <MPL/system.h>


void init_built_in_funcs() {
	add_to_bifs(0, 0, 0, 0, 0);
	/**
	 * a=bool|str|num : value
	 * aa=bool[?,..]|str[?,..]|num[?,..] : var
	 * st=struct : value
	 * sta=struct[?,..] : var
	 */
	//----------------------------------mpl built_in
	add_to_bifs(1, MPL_BUILT_IN_TYPE, "len", "aa", "num[?]");
	add_to_bifs(2, MPL_BUILT_IN_TYPE, "print", "a", "bool");
	add_to_bifs(3, MPL_BUILT_IN_TYPE, "printA", "aa", "bool");
	add_to_bifs(4, MPL_BUILT_IN_TYPE, "printS", "st", "bool");
	add_to_bifs(5, MPL_BUILT_IN_TYPE, "printSA", "sta", "bool");
	add_to_bifs(6, MPL_BUILT_IN_TYPE, "push", "aa;aa", "bool");
	add_to_bifs(7, MPL_BUILT_IN_TYPE, "pop", "aa", "bool");
	add_to_bifs(8, MPL_BUILT_IN_TYPE, "typeof", "a", "str");
	add_to_bifs(9, MPL_BUILT_IN_TYPE, "delete", "aa", "bool");
	add_to_bifs(10, MPL_BUILT_IN_TYPE, "input", "bool", "str");
	add_to_bifs(11, MPL_BUILT_IN_TYPE, "error_handle", "num;str;str", "bool");
	add_to_bifs(12, MPL_BUILT_IN_TYPE, "config_all", "", "str[?,2]");
	add_to_bifs(13, MPL_BUILT_IN_TYPE, "define_all", "", "str[?,2]");
	add_to_bifs(14, MPL_BUILT_IN_TYPE, "embed_run", "str[?]", "num");
	add_to_bifs(15, MPL_BUILT_IN_TYPE, "input", "num", "str");
	add_to_bifs(16, MPL_BUILT_IN_TYPE, "argvs", "", "str[?]");
	add_to_bifs(16, MPL_BUILT_IN_TYPE, "define_isset", "str", "bool");
	add_to_bifs(16, MPL_BUILT_IN_TYPE, "session_all", "", "str[?,2]");
	add_to_bifs(16, MPL_BUILT_IN_TYPE, "session_isset", "str", "bool");
	//----------------------------------data types built_in
	//TODO:
	//----------------------------------os built_in
	//TODO:
	
	
}

//***********************************
void init_built_in_defines() {
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ERROR", str_from_int32(ERROR_ID));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "WARNING", str_from_int32(WARNING_ID));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "FATAL", str_from_int32(FATAL_ID));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "OSType", OS_TYPE);
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "OSArch", OS_ARCH);
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "PathSeparator", char_to_str(OS_SEPARATOR));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "EOF", str_from_int32(EOF));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "IntSize", str_from_int32(MAX_INT_LEN));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "FloatSize", str_from_int32(MAX_FLOAT_LEN));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "MplVersion", str_multi_append(VERSION_NAME, "-", VERSION, 0, 0, 0));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "AppPath", project_root);
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "LeftTrim", "1");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "RightTrim", "2");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "BothTrim", "0");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadChar", "1");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadLine", "2");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadAll", "0");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "CpuInfo", "1");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "MemoryInfo", "2");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "OSInfo", "3");
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "PID", str_from_long_int(get_pid()));
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "StartedTime", AppStartedTime);
	add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "HostName", get_host_name());
}

//***********************************
void init_default_configs() {
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "ErrorsMode", str_from_int32(ERROR_ID)); //fatals
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "WarningsMode", str_from_int32(WARNING_ID));//errors,fatals
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "SetLogfile", 0);
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "DebugMode", "0");//0..5
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "MaxHugeDivideSteps", str_from_int32(max_steps_estimate_huge));
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "TabSize", str_from_int32(tab_size_int));
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableSafeMode", "true");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppVersion", "1.0.0");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppName", main_source_name);
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppLicense", "free");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableSession", "true");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "SaveSessionOnExit", "true");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "AutoCreateDocs", "false");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableHelpArgument", "true");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "OptimizeMode", "false");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableAboutArgument", "true");
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "SetByteCode", 0);
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "RunOnlyOS", 0);
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "RunOnlyArch", 0);
	add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppCreator", "MPL-Interpreter");
}