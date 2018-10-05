Mini Portable Language - MPL (BETA-0.1)
============================================

_This is a small,fast and simple interpreter also best solution to common problems of programmers._
[![license](https://img.shields.io/github/license/Netflix/pollyjs.svg)](http://www.apache.org/licenses/LICENSE-2.0)

TODO
--------

* init exeptions **[OK]**
* define mpl arguments **[OK]**
* defnie mpl help **[OK]**
* init basic interpreter variables **[OK]**
* init data defined **[OK]**
* init virtual memory(vm) **[OK]**
* init built-in functions **[OK]**
* init built-in defines **[OK]**
* create memory structure **[OK]**
* reading source file as utf8 **[OK]**
* remove comments and convert utf8 to ascii **[OK]**
* extract tokens **[OK]**
* parsing tokens **[OK]**
* parsing imports **[OK]**
* parsing functions **[OK]**
* parsing structs **[OK]**
* parsing instructions **[OK]**
* parsing structures **[OK]**
* start runtime **[OK]**
* init global vars **[OK]**
* start APP_CONTROLLER **[OK]**
* start INSTRUCTION_EXECUTOR **[OK]**
* labeled instructions **[OK]**
* implement define vars **[OK]**
* set new var in memory **[OK]**
* delete var in memory **[OK]**
* manage exceptions **[OK]**
* create other memory functions **[OK]**
* determine type of data **[OK]**
* calculate math expressions **[OK]**
* calculate radix numbers **[OK]**
* calculate integer,float numbers **[OK]**
* calculate string expressions **[OK]**
* calculate utf8 string expressions **[OK]**
* calculate boolean expressions **[OK]**
* define struct expressions **[OK]**
* define complex struct expressions **[OK]**
* set built-in __define variables **[OK]**
* set built-in __config variables **[OK]**
* set new __define [..]
* define global vars
* use magic macros in define vars
* edit __config
* calculate huge numbers
* implement function call
* init func parameters
* call built-in functions
* implement return
* implement alloc_vars
* alloc struct expressions
* implement short alloc vars
* implement structures
* define manage
* start exception_handler
* define if
* define elif,else
* comparison struct expressions
* recursive functions
* set arrays in functions parameter
* alloc array vars
* set structs in functions parameter
* define loop
* start garbage collector
* start review_array_loop
* implement next
* implement break
* create offline documentation of mpl
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
* complete sample codes
* optimizing RAM,CPU usage

### <<publish BETA version of MPL (for windows)>>

* support package files
* publish BETA version for linux
* run scripts in command line
* add math module for advanced calculation
* add net module
* support for a debugger
* add many built-in functions
* support minimal gui for windows
* add regex module

### <<publish MPL1 version of MPL (for windows,linux)>>

* support for multi threading
* support minimal gui for gtk
* support mpl repository
* support mpl forum

### To be continued ...

MPL Language Features
--------

> support utf-8 strings **[OK]** <br>
> support radix numbers **[OK]** <br>
> has XOR operand **[OK]** <br>
> support data structures [..] <br>
> support magic macros [..] <br>
> support huge numbers <br>
> support override functions <br>
> has built-in functions <br>
> manage exceptions <br>
> has bit operands <br>
> support unlimited function parameters <br>
> support multi return values <br>
> support call by refrence vars <br>
> support short allocations for vars <br>
> support browse arrays in loop <br>
> support switch condition <br>

MPL Hello World Program
--------

    func main(){
        print("Hello World!\n)
    }

Get Starterd
--------

### [Windows] : Build Mpl.exe

> First clone the repository and then install python3 for ruuning build tool and mingw64 for compiling mpl. Then run 'build_win32_migw64.py' or double click on 'build_run_win32.bat'. Finally created a folder by name 'win32-release'. <br>
> Now  you can run  a file by name 'main.mpl' through command 'mpl.exe main.mpl' and you can see the result! <br>
> [Mpl is portable!] you can move 'mpl.exe' to any where. it's a static executable file and not need to any files.(It's not need any modules or packages or docs or anything... Their are just for easier programming by mpl ) <br>


MPL Sample Program
--------
     //=>import an external file
    import "file:$/data/sam.mpl"
    //=>edit __config values
    __config["AppName"]="MyProgram"
    //=>edit __session values. it is store in a database
    __session["AppName"]="MyProgram"
    //------------
    //=>define main function,start point of any program
    func main(){
        print("Hello World!\n");
        //=>print an utf8 string
        print("سلام بر دنیا!\n"); 
        str uu[2,2]={{"x","v"},{"h","i"}}
        //=> browse an array
        loop(str u,b=null;u,b:uu){
            //=>print a calculated string
            print("u is: %u%,and b is: %b%\n");
        }
        //=>define some vars by one value
        num sum1,sum2=0;
        //=>call a function by reference var
        add(&sum1,5,7.7,-2,0.7) //=>sum1=3
        sum2=add(sum2,5,7.7,-2,0.7) //=>sum2=3
        struct st1{bool b,str s,num n}
        struct st2{st1 v,bool b}
        //=>create a var by a customized data type
        st1 s1=struct(true,"ty",78.7)
        //=>using multiple struct into other
        st2 s2[2]={struct(struct(false,"ui",-9.99h),true),null}
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
    //=>define a function by unlimited data type
    func add(num ref,vars nums){
        loop(num i=0;i<len(nums);i++){
            //=>check type of var by 'typeof' function
            if(typeof(nums[i])=="i") ref+=nums[i]
        }
        return ref
    }

Programmers & Designers
--------

1. Mohammad Amin Delavar Khalafi [main programmer,designer],[Iran],[amindelavar@hotmail.com]

Events
--------

* 0.1.0-0.6.0 : Publish 'BETA' version of MPL (2018)  <b>[DONE]</b>
* 0.3.0-1.0.0 : complete 'www.mpl-lang.ir' website and MPL documentation (2018-2019)
* 0.9.0-1.0.0 : 5 daily rolling publish (2018-2019)
* 1.0.0-2.0.0 : Publish 'MPL1' version of MPL (2019-2020)
* 2.0.0-3.0.0 : Publish 'MPL2' version of MPL and complete gui programming on windows and linux (2019-2021)
* 3.0.0-4.0.0 : Publish 'MPL3' version of MPL and entering mpl to servers and web applications (2020-2022)

MPL is best choice FOR
--------

1. build tools of personal projects or big projects
2. start to learning the programming world
3. create small and common tools for programmers
4. personal programming for spend timeend time