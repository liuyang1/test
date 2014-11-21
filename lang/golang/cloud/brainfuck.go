package main
import "fmt"
var (
	a	[1024]byte
	prog = "++++++++++[>++++++++++<-]>++++.+."
	p,pc int
)
func loop(inc int){
	for i := inc; i != 0; pc +=inc {
		switch prog[pc+inc] {
		case '[':
			i++
		case ']':
			i--
		}
	}
}
func main() {
	for {
		switch prog[pc] {
		case '>':
			p++
		case '<':
			p--
		case '+':
			a[p]++
		case '-':
			a[p]--
		case '.':
			fmt.Printf("%c",a[p])
		case '[':
			if a[p] == 0 {
				loop(1)
			}
		case ']':
			if a[p] != 0 {
				loop(-1)
			}
		default:
			fmt.Println("Illegal instruction")
		}
		pc++
		if pc == len(prog) {
			fmt.Println()
			return
		}
	}
}

/*
本程序实现了一个简单的编译器.目标语言为brainfuck语言
该语言为图灵完备的,也就是计算能力与图灵机等价.
该语言本身语法规范,异常简单.
+	当前值加1
-	当前值减1
>	以下一个单元作为当前值
<	以上一个单元作为当前值
[	若当前值为0,则jmp到对应的]之后
]	若当前值不为0,则jmp到对应的[之后
.	输出当前值,以字符形式

可以看到代码中,以a为存储空间,以prog为程序空间.
p为存储空间指针,pc为prog程序空间指针
loop函数实现了跳转功能

这里的loop函数实现很精妙

auth: liuyang1
mtime: 2013-03-24 15:29:39 
*/

/*
因此上面的代码就可以如下翻译
	"++++++++++[>++++++++++<-]>++++.+."

	brainfuck:	+++++++++++		
	c sytle:	a[0]=10

	brainfuck:	[>++++++++++<-]
	c sytle:	while(a[0]>0]
				a[1]+=10
				a[0]--

	brainfuck: >++++.+.
	c sytle:	a[1]+=4
				printf("%c",a[1])
				a[1]++
				printf("%c",a[1])

*/
