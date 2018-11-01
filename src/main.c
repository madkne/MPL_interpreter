#include <MPL/system.h>

//************************************************prototypes
Boolean start_interpreter();

Boolean mpl_help(String command);

void init_interpreter();

uint8 interpreter_mode = 0;

//************************************************
int main(int argc, char **argv) {
	//-------------------------time of start program
	AppStartedClock = clock();
	AppStartedTime = get_current_datetime(2);
	//-------------------------init exceptions list
	init_exceptions_list_data();
	//--------------------------get Argvs,analyze it
	if (argc > 1) {
		if (str_equal(argv[1], "-v")) {
			printf("MPL Interpreter %s|%s - %s(%s)\n", VERSION_NAME, VERSION, APP_NAME, OS_ARCH);
			interpreter_mode = 1;
		} else if (str_equal(argv[1], "-h")) {
			if (argc < 3) mpl_help("help");
			else if (argc > 2) mpl_help(argv[2]);
			interpreter_mode = 2;
		}/*else if(str_equal(argv[1],"-s")){
            start_run_script();
            interpreter_mode=3;
        }*/else {
			if (str_length(argv[1]) > 0 && argv[1][0] == '-') {
				
				print_error(0, "unknown_opt", "stdin", argv[1], 0, "main");
				mpl_help("usage");
			} else {
				//store arguments of program
				if (argc > 2)
					for (int ii = 2; ii < argc; ++ii) {
						str_list_append(&program_argvs, argv[ii], argvs_len++);
					}
				//printf("argv:%s",argv[4]);
				str_init(&stdin_source_path, argv[1]);
				stdin_source_path = get_absolute_path(stdin_source_path);
				Boolean ret = start_interpreter();
				interpreter_mode = 4;
				if (!ret) {
					print_error(0, "bad_exit", "stdin", 0, 0, "main");
					return 1;
				}
			}
		}
	}
	//printf("Hello World:%s,%li\n",argv[1],str_length(""));
	//-------------------------time of end program
	if (is_programmer_debug >= 1 && interpreter_mode == 4) {
		AppStartedClock = clock() - AppStartedClock;
		double time_taken = ((double) AppStartedClock) / CLOCKS_PER_SEC; // in seconds
		if (time_taken == 0) {
			time_taken += 0.000001;
			//printf("BAD\n");
		} else
			printf("Process finished during %.6f seconds with exit code 0\n", time_taken);
	}
	return 0;
}

//************************************************
Boolean start_interpreter() {
	//-----------------------init compiler
	init_interpreter();
	//-----------------------parsing source codes
	interpreter_level = "parse";
	Boolean ret0 = import_all_files();
	//printf("VVVVVV:%i\n",ret0);
	if (!ret0) {
		return false;
	}
	//printf("\nHello.....\n\n");
	//print_struct(PRINT_FUNC_ST);
	//print_struct(PRINT_MAIN_SOURCE_ST);
	//print_struct(PRINT_STRU_ST);
	//print_struct(PRINT_INSTRU_ST);
	//print_struct(PRINT_STRUCT_ST);
	//String h=0;
	//str_init(&h,"AMindelavar");
	//str_to_lower_case(&h);
	//printf("WWWWW:%s,%f\n",divide_huge_numbers("56","200.4"),(float)56 / (float)200.4);
	//printf("WWWWW:%s,%f\n",divide_huge_numbers("56","0.565"),(float)56 / (float)0.565);
	//printf("QQQQQQQQ:%s\n", calculate_two_numbers("5.78", "10",'^','h'));
	//printf("AAAAA:%s\n",str_from_double(0.0400,3) );
	//-----------------------meaning&running instructions
	interpreter_level = "runtime";
	Boolean ret3 = start_runtime();
	if (!ret3) {
		return false;
	}
	
//	String sname = 0;
//	long_int sid = determine_struct_type("struct(struct({true,false},768+56),\"Hello\")", 0, &sname);
//	printf("##SSSSSSSSS:%s,%i\n", sname, sid);
	//print_struct(PRINT_STRUCT_DES_ST);
	//print_struct(PRINT_MAGIC_MACROS_ST);
	//copy_memory_var(find_index_var_memory(2),"rrrr",0);
	show_memory(0);
	//printf("@ZZXXX:%i\n",str_has_suffix("j4364567457","457"));
	
	//-----------------------free memory
	interpreter_level = "free";
}

//************************************************
void init_interpreter() {
	//********************
	init_database();
	//********************
	init_memory();
	//********************
	init_built_in_funcs();
	//********************
	init_built_in_defines();
	//********************
	init_default_configs();
	//********************
	//get_basic_system_info()
}

//************************************************
Boolean mpl_help(String command) {
	//help || usage || groups || [group] || [group]:[subject] || [group]:[subject]:[segment] || [group]:subjects || [group]:[subject]:segments
	/**
	 * Groups:
	 * 1- keywords
	 * 2- built-in
	 * 3- exceptions
	 * 4- get_started => (what's new,lessons,samples,features)
	 * 5- about => (license,credits,README.md)
	 */
	if (str_equal(command, "help")) {
		printf("\t In The Name Of ALLAH\n\t----------------------\n");
		printf("%s[%s-%s] build:[%s %s-%s] arch:[%s] under:[%s]\n", APP_NAME, VERSION_NAME, VERSION, OS_BUILD,
				BUILD_DATE, BUILD_TIME, OS_ARCH, LICENCE);
		printf("usage: mpl [option] --OR-- mpl [path] [arg]...\n");
		printf("Options:\n");
		printf("-v	:Display interpreter version information\n");
		printf("-h	:Display information about anything. \nfor see offline documentation, type \'mpl -h groups\'\n");
		printf("-------------------------------------\n");
		printf("%s\n", SLOGAN);
		printf("See '%s' for more details.\n", OFFICIAL_WEBSITE);
	} else if (str_equal(command, "usage")) {
		printf("usage: mpl [option] --OR-- mpl [path] [arg]...\n");
		printf("Try 'mpl -h' for more information.\n");
	}
	return true;
}
//************************************************
