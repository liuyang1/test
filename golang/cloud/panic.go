package main
import "log"
func protect(g func()){
	/* defer 压后函数,具体可以见goplay中的说明 */
	defer func() {
		log.Println("done")
		/* recover 可以从错误中恢复出来,从而可以做一些清除工作 */
		/* 这一点很棒,这是一个很好的特性 */
		if x := recover(); x != nil {
			log.Printf("runtime panic: %v",x)
		}
	}()
	log.Println("start")
	g()
}

func main(){
	var s []byte
	protect(func() { s[0] = 0 })
	protect(func() { panic(42)})
	// s[0] = 42
}
