#!/usr/bin/python

import os
import sys
import os.path



if os.path.isfile("out_plain.txt"):
	os.remove("out_plain.txt")

if os.path.isfile("12xKq6VN8wfNoHwaraJiYG5cGNP2dospEu_share.txt"):
	os.remove("12xKq6VN8wfNoHwaraJiYG5cGNP2dospEu_share.txt")


if os.path.isfile("16uoLAEFbHw3a8rw8dg4wWbhMyaBWzEC9C_share.txt"):
	os.remove("16uoLAEFbHw3a8rw8dg4wWbhMyaBWzEC9C_share.txt")

if os.path.isfile("13CAC32E.armory"):
	os.remove("13CAC32E.armory")



os.system("rm *.txt *.ps *.armory")
os.system("rm *.txt *.ps *.privroot *.pubroot *.privindex")
os.system("rm *.trans *.script *.sig")

print "Cleanup ready"


