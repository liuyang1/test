/*
 * 这是一个云端编译的例子.
 */
package main
import (
	"io"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strconv"
)
var uniq = make(chan int)
func init(){
	/* go route
	 * 这是一个类似与线程的编程方法
	 * go +函数,就是执行该函数作为去程go routine?
	 *
	 * 这种很方便的属性,和erlang是有一定的相似性的
	 */
	go func() {
		for i := 0; ; i++ {
			/* 将i放到程道uniq中 */
			uniq <- i
		}
	}()
}

func main(){
	log.Println("---- start ----")
	if err := os.Chdir(os.TempDir()); err != nil {
		log.Fatal(err)
	}
	http.HandleFunc("/",FrontPage)							// 直接访问,则到首页
	http.HandleFunc("/compile",Compile)						// 如果到compile地址,则调用compile方法
	log.Fatal(http.ListenAndServe("127.0.0.1:1234",nil))	// 监听地址
}

func FrontPage(w http.ResponseWriter,_ *http.Request){		// 写首页的内容
	w.Write([]byte(frontPage))
}

func err(w http.ResponseWriter,e error) bool {
	if e!= nil {
		w.Write([]byte(e.Error()))
		return true
	}
	return false
}

func Compile(w http.ResponseWriter, req *http.Request) {
	/* 从uniq程道获取数字 */
	x := "play_" + strconv.Itoa(<-uniq) + ".go"		// 创建临时文件
	f,e := os.Create(x)
	if err(w,e) {
		return
	}
	defer os.Remove(x)
	defer f.Close()
	/* defer
	 * 当defer所处的函数(此处为Compile)结束的时侯,
	 * 程序在此直接依次调用向defer注册的函数
	 *
	 * 也就是退出之前执行 os.Remove(x) f.Close()
	 */
	log.Println("compile: ")
	log.Println(req.Body)
	_,e = io.Copy(f, req.Body)						// 复制内容到文件?
	if err(w,e) {
		return
	}
	f.Close()

	cmd := exec.Command("go","run",x)				// 编译该文件
	o,e := cmd.CombinedOutput()
	if err(w,e){
		return
	}
	w.Write(o)										// 通过网络输出
	log.Println("output:")
	log.Println(o)
}

const frontPage = `<!doctype html>
<html><head>
<script>
var req;
<!-- 编译函数,调用远端的服务器的compile方法,并且将返回结果显示到output窗体
function compile(){
	var prog=document.getElementById("edit").value;
	var req = new XMLHttpRequest();
	req.onreadystatechange=function(){
		if(!req || req.readyState != 4)
			return
		document.getElementById("output").innerHTML=req.responseText;
	}
	req.open("POST","/compile",true)
	req.setRequestHeader("Content-Type","text/plain;charset=utf-8");
	req.send(prog);
}
</script>
</head>
<body>
<textarea rows="25" cols="80" id="edit" spellcheck="false">
package main
import "fmt"
func main() {
	fmt.Println("hello,world")
}
</textarea>
<button onclick="compile();">run</button>
<textarea rows="5" cols="80" id="output"></textarea>
</body>
</html>
`
