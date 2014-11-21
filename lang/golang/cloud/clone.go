/* */
package main
import (
	"fmt"
)

var ()
const ()
func f() {}
func main() {
	c := '\x60'
	fmt.Printf("%s%s%c%s%c%c",s,"const s = ",c,s,c,'\n')
}
const s = `/* */
package main
import (
	"fmt"
)

var ()
const ()
func f() {}
func main() {
	c := '\x60'
	fmt.Printf("%s%s%c%s%c%c",s,"const s = ",c,s,c,'\n')
}
`

// 这是一个很有意思的程序.
// 该程序,可以自己输出自己.也就是运行结果与代码是完全一致的.
// 这里的技巧性,就是在于手工的构造一个输出的内容
// auth: liuyang1
// mtime: 2013-03-24 15:30:02
