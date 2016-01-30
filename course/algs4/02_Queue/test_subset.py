# A 2, B 1, C 4, D 1
seq = ['A', 'A', 'B', 'C', 'C', 'C', 'C', 'D']
seq.extend(seq)
seq.extend(seq)
print seq
classpath = ".:/home/liuy/.local/jar/algs4.jar"
cmd = "echo %s | java -cp %s Subset 4" % (" ".join(seq), classpath)


def runOnce():
    import subprocess
    ret = subprocess.check_output(cmd, shell=True)
    return ret


def statSubset():
    st = {}
    for i in range(200):
        ret = runOnce()
        ret = ret.split()
        for c in ret:
            if c not in st.keys():
                st[c] = 1
            else:
                st[c] += 1
    print st


if __name__ == "__main__":
    statSubset()
