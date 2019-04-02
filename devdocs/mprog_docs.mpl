		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
		| Mini Portable Language - MPL  |
		|			mprog.exe           |
		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
		

- Its useful for creating and managing a mpl project.		
------------------------
========================
> mprog init myproject (create new mpl project)
..
myproject[dir]
	->main.mpl
	->config.mcf
--------------config.mcf
$project_name
myproject
$start_point
main.mpl
$default_argvs
$created_date
2018.24.11-11:38:20
$author
mprog
$test_count
0
--------------main.mpl
/**
	@des:
        start point of program
    @params:
    @output:
        return status of program end
    */
func main(){
	//=>Write your code
}
========================
> mprog test (run test your codes on cmd)
main:1> num i=45i
main:2> str s="'i' is : %i%"
main:3> print(s)
main:4> --rs (run script)
> 'i' is : 45
main:4> --fu:sum:num i,num j (create new func)
main:4> --gl (go to global for define global vars)
[global]:1>$con['AppName']='myprog'
[global]:1> --go:sum (go to sum function)
sum:1>return i+j
sum:1> --go (go to main func by default)
main:4> num su=sum(34,6.5676)
main:4> --go:main.1 (go to main func and line 1,overwrite it!)
main:1> num i=100+4.57h
main:2> --ex:Export.mpl (export these codes to 'Export.mpl')
main:2> --qu (quit)
--------------Export.mpl
$con['AppName']='myprog'
func main(){
	num i=100+4.57h
	str s="'i' is : %i%"
	print(s)
	num su=sum(34,6.5676)
}
func sum(num i,num j){
	return i+j
}
========================
> mprog start argv1 argv2 (run myproject by argv1,argv2 if mprog be in myproject dir)
========================
> mprog version
========================
> mprog config start_point
main.mpl
> mprog config start_point des.mpl
========================
> mprog locate
c:\windows\myproject
========================
> mprog root c:\windows\myproject (every time that mprog executes, its directory by default is 'c:\windows\myproject')
> mprog (root dir is 'c:\windows\myproject' in any where)
- when root of mprog be null, then mprog executes on current directory
- for reset root dir, use 'mprog root null'
========================
> mprog list (list of all files and dirs of current project by details)
========================
> mprog recent 2 last (print projects path limit 2 and by lastest)
- options: last,useful
1. c:\windows\myprj			2018.24.10-17:38:20
2. c:\windows\myproject		2018.24.11-11:38:20
========================
> mprog help
========================
> mprog remove (remove myproject project completely!)
> if you sure, then press ENTER...
Remove Successfuly!
========================
> mprog search myproject (search for a project name)
c:\windows\myproject



