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




printHeader("Test 1 bitgen-btc outscript")
retval1 = os.system("../bitgen outscript addr 1DyFgTc1ofqhPBecybwyLbH82FJTMFQ8zY")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("fund_address_btc_1DyFgTc1ofqhPBecybwyLbH82FJTMFQ8zY.script"):
	print "test 1 file error"
	sys.exit()

retval1 = os.system("diff fund_address_btc_1DyFgTc1ofqhPBecybwyLbH82FJTMFQ8zY.script testfiles/fund_address_btc_1DyFgTc1ofqhPBecybwyLbH82FJTMFQ8zY.script")
if retval1 != 0:
	print "test 1 file compare error: " + str(retval1)
	sys.exit()


printHeader("Test 2 bitgen-btc outscript")
retval1 = os.system("../bitgen outscript pubkey 7ZpLGvTGWjRebpBxgkKiAuciqgACVj61JFPUSyVTzMDvVVP2Em2")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("fund_pubkey_btc_1J8ismdJtebpjghs2uYJkvhxWrEafT5tP9.script"):
	print "test 2 file error"
	sys.exit()



printHeader("Test 3 bitgen-btc outscript")
retval1 = os.system("../bitgen outscript opreturn \"Testing testing\" OP_RETURN")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("fund_opreturn_btc_D4753028F41D8FE3C31B03D2F4C49AACA33489D5A1E979C9863A9BC6F58D8760.script"):
	print "test 3 file error"
	sys.exit()



printHeader("Test 4 bitgen-btc outscript")
retval1 = os.system("../bitgen outscript opreturn 2342342342343ABCDEF2342342342342342342342342342342342342342342342 DOCPROOF")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("fund_opreturn_btc_9327187F160029CDF7325956B6E7F69A429053315DB9C407329671DFC9DC4D87.script"):
	print "test 4 file error"
	sys.exit()



printHeader("Test 5 bitgen-btc outscript")
retval1 = os.system("../bitgen outscript anyone")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("fund_anyone_btc.script"):
	print "test 5 file error"
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

