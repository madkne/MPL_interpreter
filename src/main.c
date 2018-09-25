#include <MPL/system.h>

//************************************************prototypes
Boolean start_interpreter(String source);

Boolean mpl_help(String command);

void init_interpreter();

uint8 interpreter_mode = 0;

//************************************************
int main(int argc, char **argv) {
	//-------------------------time of start program
	clock_t t;
	t = clock();
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
				Boolean ret = start_interpreter(argv[1]);
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
		t = clock() - t;
		double time_taken = ((double) t) / CLOCKS_PER_SEC; // in seconds
		if (time_taken == 0) {
			time_taken += 0.000001;
			//printf("BAD\n");
		} else
			printf("Process finished during %.6f seconds with exit code 0\n", time_taken);
	}
	return 0;
}

//************************************************
Boolean start_interpreter(String source) {
	//-----------------------init compiler
	init_interpreter();
	//-----------------------parsing source codes
	interpreter_level = "parse";
	Boolean ret0 = import_all_files(source);
	//printf("VVVVVV:%i\n",ret0);
	if (!ret0) {
		return false;
	}
	print_struct(PRINT_FUNC_ST);
	print_struct(PRINT_STRU_ST);
	print_struct(PRINT_INSTRU_ST);
	print_struct(PRINT_STRUCT_ST);
	//-----------------------meaning&running instructions
	interpreter_level = "runtime";
	Boolean ret3 = start_runtime();
	if (!ret3) {
		return false;
	}
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
