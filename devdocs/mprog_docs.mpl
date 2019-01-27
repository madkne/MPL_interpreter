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
$$project_name
myproject
$$start_point
main.mpl
$$default_argvs
$$created_date
2018.24.11-11:38:20
$$author
mprog
$$test_count
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
main:4> ***r (run script)
> 'i' is : 45
main:4> ***g (go to global for create like a func)
[global]:1> func sum(num i,num j){
[global]:2> return i+j}
[global]:3> ***f:main (go to main func)
main:4> num su=sum(34,6.5676)
main:4> ***f:main.1 (go to main func and line 1,overwrite it!)
main:1> num i=100+4.57h
main:2> ***e:Export.mpl (export these codes to 'Export.mpl')
main:2> ***q (quit)
--------------Export.mpl
func main(){
	num i=100+4.57h
	str s="'i' is : %i%"
	print(s)
	num su=sum(34,6.5676)
}
func sum(num i,num j){
	return i+j}
========================
> mprog start argv1 argv2 (run myproject by argv1,argv2 if mprog be in myproject dir)
========================
> mprog version
========================
> mprog locate
c:\windows\myproject
========================
> mprog root c:\windows\myproject (every time that mprog executes, its directory is 'c:\windows\myproject')
- if you want to reset root dir you can use 'mprog root'
========================
> mprog list (list of all files and dirs of current project)
========================
> mprog recent 2 last (print projects path limit 2 and by lastest)
- options: last,useful
1. c:\windows\myprj			2018.24.10-17:38:20
2. c:\windows\myproject		2018.24.11-11:38:20
========================
> mprog help
========================
> mprog remove (remove myproject project completely!)
options :-f
> if you sure, then press ENTER...
Remove Successfuly!
========================
> mprog search myproject (search for a project name)
c:\windows\myproject



