import os
from subprocess import Popen,PIPE
md = "mdBasic.exe"
# p = Popen(md,stdout = PIPE,bufsize =10000)
# for line in iter(p.stdout.readline,''):
# 	line = line.split(",")
# 	print type(line)
# 	print line
os.system(md)