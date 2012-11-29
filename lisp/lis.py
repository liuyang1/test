#! /usr/bin/env python
# *-* encoding:  utf-8

# ------------------------------------------------------------
# liuyang,mtime: 2012-11-29 15:01:39 
# 这是一个python版本的简易lisp解释其的实现.
# 参考代码:http://www.googies.info/articles/lispy.html
# 利用python的特性,可以很方便的写出这个解释器,代码量相当之小
# 但是麻雀虽小,五脏俱全.
# 这个解释器完整的实现了,从输入,解析,环境,计算的过程.对理解编译过程,与语言的设计都有很大帮助.
#-------------------------------------------------------------


# ------------------------ define ENVRONMENT -----------------
class Env(dict):
    " An envirionment: a dict of {'var':val} pairs,with an outer Env"
    def __init__(self,params=(),args=(),outer=None):
        self.update(zip(params,args))
        self.outer=outer
    def find(self,var):
        "Find the innermost Env where var appears"
        # 根据词法定界查找正确的环境
        return self if var in self else self.outer.find(var)

def add_globals(env):
    "add some Scheme standard procedures to an environment"
    import math,operator as op
    env.update(vars(math))  # add sin,sqrt,...
    env.update({
        '+'       : op.add,
        '-'       : op.sub,
        '*'       : op.mul,
        '/'       : op.div,
        'not'     : op.not_,
        '>'       : op.gt,
        '<'       : op.lt,
        '>='      : op.ge,
        '<='      : op.le,
        '='       : op.eq,
        'equal?'  : op.eq,
        'eq?'     : op.is_,
        'length'  : len,
        'cons'    : lambda x,y : [x]+y,
        'car'     : lambda x   : x[0],
        'cdr'     : lambda x   : x[1           : ],
        'append'  : op.add,
        'list'    : lambda *x  : list(x),
        'list?'   : lambda x   : isa(x,list),
        'null?'   : lambda x   : x==[],
        'symbol?' : lambda x   : isa(x,Symbol)
        })
    return env

g_env=add_globals(Env())

# ----------------- defube EVAL -----------------------------
def eval(x,env=g_env):
    "Evaluate an expression in an envirionment"
    if isa(x,Symbol):       # variable reference
        return env.find(x)[x]
    elif not isa(x,list):   # constant literal
        return x
    elif x[0]=='quote':     # (quote exp)
        (_,exp)=x
        return exp
    elif x[0]=='if':        # (if test coneq alt)
        (_,test,conseq,alt)=x
        return eval((conseq if eval(test,env) else alt),env)
    elif x[0]=='set!':      # (set! var exp)
        (_var,exp)=x
        env.find(var)[var]=eval(exp,env)
    elif x[0]=='define':    # (define var exp)
        (_,var,exp)=x
        env[var]=eval(exp,env)
    elif x[0]=='lambda':    # (lambda (var*) exp)
        (_,vars,exp)=x
        return lambda *args: eval(exp,Env(vars,args,env))
    elif x[0]=='begin':     # (begin exp*)
        for exp in x[1:]:
            val = eval(exp,env)
        return val
    else:                   # (proc exp*)
        exps = [eval(exp,env) for exp in x]
        proc = exps.pop(0)
        return proc(*exps)
isa = isinstance

Symbol = str

# ---------------------- Paring Module ----------------------

def atom(token):
    "numbers become numbers,every other token is a symbol"
    try:
        return int(token)
    except ValueError:
        try:
            return float(token)
        except ValueError:
            return Symbol(token)

def read_from(tokens):
    "read an expression from a sequence of tokens"
    if len(tokens)==0:
        raise SyntaxError("unexpected EOF while reading")
    token=tokens.pop(0)
    if '('==token:
        L=[]
# if first token is ),mean read_form(parsing func should over)
        while tokens[0]!=')':
            L.append(read_from(tokens))
        ret = tokens.pop(0) # pop off )
        return L
    elif ')'==token:
        raise SyntaxError('unexpected ")"')
    else:
        return atom(token)

def tokenize(s):
    "Convert a string into a list of tokens"
    return s.replace('(',' ( ').replace(')',' ) ').split()

def read(s):
    "Read a Scheme expression from a string"
    return read_from(tokenize(s))

parse=read

# ---------------------- string func -----------------------

def to_string(exp):
    "convert a Python object back in to a lisp-readable string"
    return '('+' '.join(map(to_string,exp))+')' if isa(exp,list) else str(exp)

def repl(prompt='lis.py>> '):
    "a prompt read-eval-print loop"
    while True:
        val = eval(parse(raw_input(prompt)))
        if val is not None:
            print to_string(val)

# -------------------- main ----------------------------------
def test():
    eval(parse('( + 1 2 )'))
if __name__=="__main__":
    repl()

