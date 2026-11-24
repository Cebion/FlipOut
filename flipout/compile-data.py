#!/usr/bin/python
# $Id$

import os

ext_order={".png":" 1", ".ogg":" 2", ".mesh":" 3", ".shader":" 4", ".font":" 5", ".data":"", ".level":"l"}

def fn_cmp(fn1, fn2):
	base1,ext1=os.path.splitext(fn1)
	base2,ext2=os.path.splitext(fn2)

	return cmp(ext_order[ext1]+base1, ext_order[ext2]+base2)

def escape(str):
	result=""
	for c in str:
		o=ord(c)
		if c=='\n':
			result+="\\n"
		elif c=='\t':
			result+="\\t"
		elif c=='"':
			result+='\\"'
		elif c=='\\':
			result+="\\\\"
		elif o<32 or o>126:
			result+="\%03o"%(o)
		else:
			result+=c
	return result

import sys
import commands

for fn in sorted([f for f in os.listdir("data") if os.path.splitext(f)[1] in ext_order], cmp=fn_cmp):
	base,ext=os.path.splitext(fn)
	full=os.path.join("data", fn)
	if ext==".mesh":
		print 'mesh "%s" { %s };'%(fn, file(full).read())
	elif ext==".shader":
		print 'shader "%s" { %s };'%(fn, file(full).read())
	elif ext==".font":
		print 'ui_resources { font "%s" { %s }; };'%(fn, file(full).read())
	elif ext==".png":
		st='texture "%s" { min_filter LINEAR; image_data "%s"; };'%(fn, escape(file(full).read()))
		if base=="gui" or os.path.exists(os.path.join("data", base+".font")):
			print "ui_resources { %s };"%st
		else:
			print st
	elif ext==".ogg":
		print 'sound "%s" { sound_data "%s"; };'%(fn, escape(file(full).read()))
	elif ext==".level":
		print 'level "%s" { %s };'%(fn, commands.getoutput("mspdatatool \"%s\""%full))
	elif ext==".data":
		print file(full).read()
