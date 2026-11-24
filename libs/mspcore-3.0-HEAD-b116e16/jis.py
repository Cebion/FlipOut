#!/usr/bin/python

import sys
import os

in_fn = sys.argv[1]
out_fn = os.path.splitext(in_fn)[0]+".table"

data = []
for line in file(in_fn):
	line = line.strip()
	if line[0]=='#':
		continue
	parts = line.split(None, 3)
	code = eval(parts[1])-0x2020
	code2 = (code&0xFF)+((code>>8)&0xFF)*94-95
	data.append((code, eval(parts[2]), code2))

out = file(out_fn, "w")
out.write("namespace {\n\n")

data.sort(lambda x,y: cmp(x[0],y[0]))
out.write("const unsigned short jisx0208_to_ucs_table[94*94] =\n{\n\t")
i = 0
for code in xrange(94*94):
	if code>0:
		out.write(", ")
		if (code%94%12)==0 or (code%94)==0:
			out.write("\n\t")
	if i<len(data) and data[i][2]==code:
		out.write("0x%04X"%data[i][1])
		i += 1
	else:
		out.write("0     ")
out.write("\n};\n\n")

data.sort(lambda x,y: cmp(x[1],y[1]))
out.write("struct UcsToJis\n{\n\tunsigned short ucs;\n\tunsigned short jis;\n};\nconst UcsToJis ucs_to_jisx0208_table[] =\n{\n\t")
for i in xrange(len(data)):
	if i>0:
		out.write(", ")
		if (i%6)==0:
			out.write("\n\t")
	out.write("{ 0x%04X, 0x%04X }"%(data[i][1], data[i][0]))
out.write("\n};\n\n")

out.write("unsigned ucs_to_jisx0208_table_size=%d;\n\n"%len(data))

out.write("}\n")
