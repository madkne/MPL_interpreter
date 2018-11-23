
import "fg.mpl"
__config["errors_mode"]="errors";
__define["version"]="1.0.0";
//----------------------
func main(){
	print(__define["version"])
	f1(["er","FG"])
	return len("ER")
}

func f1(str y[?]){
	print(y[0])
	printA(y)
	__config["errors_mode"]="fatals";
}