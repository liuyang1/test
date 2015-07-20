import subprocess

print 'paraent write:'
proc = subprocess.Popen(['cat', '-'],
                        stdin=subprocess.PIPE,
                        )
# send input to proc's stdin
print 'paraent comm:'
proc.communicate('\tstdin: to stdin\n')
