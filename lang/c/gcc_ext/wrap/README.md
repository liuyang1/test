# 阴影中的魔法：链接器覆盖符号

日程工程当中，经常有魔改的需求。这需要熟悉编译工具链中的细节，从而巧妙的根据需求，小改动、可扩展的完成要求。当然，我们同时希望这个技巧，在满足需求的情况下，尽量的简单，从而规避使用过于复杂的特性，例如[双截棍](https://www.runoob.com/w3cnote/if-programming-languages-were-weapons.html)，不小心打伤自己。因此，务必 **理解清楚需求**，再考虑使用黑魔法的技巧。

一种常见的需求，是覆盖对象文件中的特定的函数。

这种需求，可以用于覆盖系统调用、库函数、或者其他分布广泛的函数。比如：

- 替换为自己的实现，比如自定义的内存分配，自定义的printf等
- 增加trace的log
- 其他修饰

小范围的性质的改动，直接替换为调用自己的函数更为简单直接。或者使用宏，这样可修改性更好。

如果这种覆盖，是广泛的或者非侵入式（尽可能少的修改源码），那么可以考虑使用本文介绍的技巧：通过gcc的链接器`ld`，覆盖对象文件obj中的符号🔣。

gcc的linker有一个选项：

    --wrap=SYMBOL

这个选项会，将原链接到`SYMBOL`的函数调用，修改为`__wrap_SYMBOL`。同时，将`__real_SYMBOL`的函数调用，定向到原本的`SYMBOL`的符号。

有没有绕？其实，看一眼下图，其实还是很清晰的。

    --> SYMBOL --> [TEXT]
    --> __wrap_SYMBOL ...???... --> __real_SYMBOL -> [TEXT]

注意：这里`__wrap_SYMBOL`到`__real_SYMBOL`之间的关系，是完全由我们自己写出的`__wrap_SYMBOL`函数决定的，可以替换为我们自己的实现，而不调用到`__real_SYMBOL`符号。也可以加上修饰，然后继续调用到实际的`__real_SYMBOL`符号。

## 覆盖`malloc`

直接看代码，更为清楚：

    void *__real_malloc(size_t size); #声明real函数，确保没有警告
    void *__wrap_malloc(size_t size) {
        # 增加调用log
        __real_printf("calling into my malloc with sz=%d\n", size);
        # 调用到实际的系统的malloc函数
        void *p = (void *)__real_malloc(size);
        return p;
    }

很简单吧？这样，在其他的C文件里面，一样继续调用`malloc`函数，但是链接器会将实际上调用到我们自己的`__wrap_malloc`。

相应的，需要增加编译选项。这里的`-Wl`前缀，是告诉gcc的编译器，把后续的参数信息，传递给linker。

    gcc -Wl,--wrap=malloc

----

## 覆盖`printf`

但是，当我尝试覆盖`printf`的时候，发现并没有按照预期进行。看上去似乎是无效的。通过`objdump -d`反汇编发现，原来gcc对于简单的`printf`调用，会直接替换为`puts`调用，从而在最终可执行文件里面，并不会调用到真正的`printf`。

具体而言，简单的`printf`调用，（经过我的粗略测试，很可能不完全）包括

- 只打印字符串本身
- 格式化字符串，只包含一个"%s"，然后只打印一个字符串

可以通过gcc的`--fno-builtin`选项关闭掉这个优化。

----

更为详细的例子，参见同目录下的代码。

就这么简单。最后提醒，黑魔法，如无必要，请勿使用；如若使用，三思而行。
当然，黑魔法，有黑魔法的趣味。😈

----

TLDR 文常不看的核心摘要回顾

## Shadow symbol with linker

- use linker's `--wrap=symbol` option to introduce `__wrap_FUNC` and `__real_FUNC`
- gcc pass linker's option with `-Wl,OPTION,OPTION`
- `printf` with only one string, compiler will optimize it to `puts` function.
  - use `-ffreestanding` or `-fno-builtin` to forbidden this.
