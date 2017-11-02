#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-btc"):
	print "Error, application binary does not exist"
	sys.exit()




#============ Start of tests ============

printHeader("Test 1 bitgen-btc timelock")
retval1 = os.system("../bitgen-btc timelock 123 7WUtvUqbkvwapLgHqZWQoWdfYNom5tVaCYvLBk7H2u5CxcK3Kzr")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("redeem_timelock_btc_3BpbwkyXJDniyoiQiGwYLjKciTPXwLz3oh.script"):
	print "test 1 file error"
	sys.exit()

printHeader("Test 2 bitgen-btc timelock")
retval1 = os.system("../bitgen-btc timelock 123456789 7XLkkBwAMyvLKYePvt2EcnwfyEVnSuj3wGqQpRDdomfHJj5tTTP")
if retval1 != 0:
	print "test 2 error"
	sys.exit()

if not os.path.isfile("redeem_timelock_btc_3FEKSUEKtiuHE23XxYbZJLsJg6mXMCcADZ.script"):
	print "test 2 file error"
	sys.exit()

print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.trans *.script *.txt *.sig")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

sys.exit(123)

