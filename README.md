Mini Portable Language - MPL (BETA-0.1)
============================================

This is a small,fast and simple interpreter also best solution to common problems of programmers.
[![license](https://img.shields.io/github/license/Netflix/pollyjs.svg)](http://www.apache.org/licenses/LICENSE-2.0)

TODO
--------

* init exeptions [OK]
* define mpl arguments [OK]
* defnie mpl help [OK]
* init basic interpreter variables [OK]
* init data defined [OK]
* init virtual memory(vm) [OK]
* init built-in functions [OK]
* init built-in defines [OK]
* create memory structure [OK]
* reading source file as utf8 [OK]
* remove comments and convert utf8 to ascii [OK]
* extract tokens [OK]
* parsing tokens [OK]
* parsing imports [OK]
* parsing functions [OK]
* parsing structs [OK]
* parsing instructions [OK]
* parsing structures [OK]
* start runtime [OK]
* init global vars [OK]
* start APP_CONTROLLER [OK]
* start INSTRUCTION_EXECUTOR [OK]
* labeled instructions [OK]
* implement define vars [..]
* set new var in memory [OK]
* delete var in memory 
* manage exceptions [OK]
* create other memory functions [..]
* determine type of data [OK]
* calculate math expressions [OK]
* calculate radix numbers [OK]
* calculate integer,float numbers [OK]
* calculate string expressions [OK]
* calculate utf8 string expressions [OK]
* calculate boolean expressions
* set new __define
* use magic macros in define vars
* edit __config
* calculate huge numbers
* implement function call
* init func parameters
* call built-in functions
* implement return
* implement alloc_vars
* implement short alloc vars
* implement structures
* define manage
* start exception_handler
* define if
* define elif,else
* recursive functions
* set arrays in functions parameter
* alloc array vars
* set structs in functions parameter
* start garbage collector
* define loop
* start review_array_loop
* implement next
* implement break
* complete import error_handling
* complete mpl built-in functions
* complete built-in constants
* complete data built-in functions
* building MPL website
* add switch keyword instead of if,elif,else
* parsing and converting switch instruction
* support for embedded files
* complete parse error_handling
* complete os built-in functions
* suppot for modules files
* using sqlite3 as a module
* complete mean_run error_handling
* complete support utf8
* complete offline docs
* optimizing RAM,CPU usage

## :<<publish BETA version of MPL (for windows)>>: 

* support package files
* publish BETA version for linux
* run scripts in command line
* add math module for advanced
* add net module
* add many built-in functions
* support minimal gui for windows
* add regex module

## :<<publish MPL1 version of MPL (for windows,linux)>>: 

* support for multi threading
* support minimal gui for gtk

## To be continued ...

MPL Language Features
--------

* support radix numbers [..]
* support utf-8 strings [OK]
* support override functions
* has built-in functions
* support huge numbers
* manage exceptions
* support data structures
* has bit operands
* support unlimited function parameters
* support multi return values
* support call by refrence vars
* has XOR operand
* support short allocations for vars
* support browse arrays in loop
* support switch condition

MPL Hello World Program
--------

    func main(){
        print("Hello World!\n)
    }

MPL Sample Program
--------

    import "file:$/data/sam.mpl"  //=>import an external file
    __config["AppName"]="MyProgram" //=>edit __config values
    __session["AppName"]="MyProgram" //=>edit __session values. it is store in a database
    //------------
    func main(){ //=>define main function,start point of any program
        print("Hello World!\n");
        print("سلام بر دنیا!\n"); //=>print an utf8 string
        str uu[2,2]={{"x","v"},{"h","i"}} //=>define a 2 in 2 array of string
        loop(str u,b=null;u,b:uu){ //=> browse an array
            print("u is: %u%,and b is: %b%\n"); //=>print a calculated string
        }
        num sum1,sum2=0; //=>define some vars by one value
        add(&sum1,5,7.7,-2,0.7) //=>sum1=3 , call a function by reference var
        sum2=add(sum2,5,7.7,-2,0.7) //=>sum2=3 , call a function in normal
        struct st1{bool b,str s,num n} //=>define a struct
        struct st2{st1 v,bool b}
        st1 s1=struct(true,"ty",78.7) //=>create a var by a customized data type
        st2 s2[2]={struct(struct(false,"ui",-9.99h),true),null} //=>using multiple struct into other
    }
    //------------
    //=>create a documentation for a function
    /**
    @params:
        ref(num) : can set sum of value by reference var
        nums(vars) : get unlimited numbers
    @des:
        add up just integer numbers and return it
    @ver:
        1.0.0
    */
    func add(num ref,vars nums){ //=>define a function by unlimited data type
        loop(num i=0;i<len(nums);i++){
            if(typeof(nums[i])=="i") ref+=nums[i] //=>check type of var by 'typeof' function
        }
        return ref
    }

Events
--------

* 0.1.0-0.4.0 : Publish 'BETA' version of MPL (2018)
* 0.4.0-1.0.0 : complete 'www.mpl-lang.ir' website and MPL documentation (2018-2019)
* 0.7.0-1.0.0 : 5 daily rolling publish (2018-2019)
* 1.0.0-2.0.0 : Publish 'MPL1' version of MPL (2019-2020)
* 2.0.0-3.0.0 : Publish 'MPL2' version of MPL and complete gui programming on windows and linux (2019-2021)
* 3.0.0-4.0.0 : Publish 'MPL3' version of MPL and entering mpl to servers and web applications (2020-2022)

MPL is best choice FOR
--------

* build tools of personal projects or big projects
* enter to programming world
* create small and common tools for programmers
* personal programming for spend time