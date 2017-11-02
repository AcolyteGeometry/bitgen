#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-ltc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============



printHeader("Test 1 bitgen-ltc outscript addr")
retval1 = os.system("../bitgen-ltc outscript addr LiB41nq5hDLmv2rhDcsQWRryFUZvXUoqCm")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("fund_address_ltc_LiB41nq5hDLmv2rhDcsQWRryFUZvXUoqCm.script"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-ltc outscript pubkey")
retval1 = os.system("../bitgen-ltc outscript pubkey 7WdRV6RF24Q2tWaPD7L6XQ2AmRw8CLJLaLjxgYmbmWedeUXBLrh")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("fund_pubkey_ltc_LZwJpoxEbRfGYYVhqP25V5JUDcPQ31NP7v.script"):
	print "test 2 file error"
	sys.exit()



printHeader("Test 3 bitgen-ltc outscript opreturn")
retval1 = os.system("../bitgen-ltc outscript opreturn \"The test\" OP_RETURN")
if retval1 != 0:
	print "test 3 error"
	sys.exit()

if not os.path.isfile("fund_opreturn_ltc_43A2AA214B450950E43B38003A1AB13C54F570D60473680E3408427D8468DAEF.script"):
	print "test 3 file error"
	sys.exit()



printHeader("Test 4 bitgen-ltc outscript anyone")
retval1 = os.system("../bitgen-ltc outscript anyone")
if retval1 != 0:
	print "test 4 error"
	sys.exit()

if not os.path.isfile("fund_anyone_ltc.script"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-ltc outscript addr")
retval1 = os.system("../bitgen-ltc outscript addr LQz1GwB8g87XBxp4FUWcgeZZpAY3MZyYKU")
if retval1 != 0:
	print "test 5 error"
	sys.exit()

if not os.path.isfile("fund_address_ltc_LQz1GwB8g87XBxp4FUWcgeZZpAY3MZyYKU.script"):
	print "test 5 file error"
	sys.exit()


printHeader("Test 6 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 1231231231231231231231231231231236666666555555555544444444444441 0 TARBM4FSBrKux1S41BDyc5AFjFfdGQD9ottejMHUr7t41fSZJxK3 1.000 out fund_address_ltc_LQz1GwB8g87XBxp4FUWcgeZZpAY3MZyYKU.script 0.999 extra fixednounce")
if retval1 != 0:
	print "test 6 error"
	sys.exit()
if not os.path.isfile("ltc_2042B09577689DB05008D63B746D859C23A6C079FD2023DF549EE165BC6F9C2D.trans"):
	print "test 6 file error"
	sys.exit()




printHeader("Test 7 bitgen-ltc outscript pubkey")
retval1 = os.system("../bitgen-ltc outscript pubkey 7Xir5m9zgXMAPbmA3V6m8EwiN3K9puXZnJb1Qk1DKwQsJ6ghE1V")
if retval1 != 0:
	print "test 7 error"
	sys.exit()

if not os.path.isfile("fund_pubkey_ltc_Lf4bsR8Yu4K5QVCkwwcuYozQE25dsocPoN.script"):
	print "test 7 file error"
	sys.exit()

printHeader("Test 8 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 1231231231231231231231231231231236666666555555555544444444444441 0 TARBM4FSBrKux1S41BDyc5AFjFfdGQD9ottejMHUr7t41fSZJxK3 1.000 out fund_pubkey_ltc_Lf4bsR8Yu4K5QVCkwwcuYozQE25dsocPoN.script 0.999 extra fixednounce")
if retval1 != 0:
	print "test 8 error"
	sys.exit()

if not os.path.isfile("ltc_E5255C064047C8F1A39474BB24CA9823265EA69EE8F77C976D1AC0FC10A3B955.trans"):
	print "test 8 file error"
	sys.exit()




printHeader("Test 9 bitgen-ltc outscript opreturn")
retval1 = os.system("../bitgen-ltc outscript opreturn 23423423423423423423423423421312312312312312323122131231231231232 DOCPROOF")
if retval1 != 0:
	print "test 9 error"
	sys.exit()

if not os.path.isfile("fund_opreturn_ltc_F591B5B23A6D86942DE7723F700454F5C05765CA8D8F90EFD7BC4E1DA7931AED.script"):
	print "test 9 file error"
	sys.exit()

printHeader("Test 10 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 1231231231231231231231231231231236666666555555555544444444444441 0 TARBM4FSBrKux1S41BDyc5AFjFfdGQD9ottejMHUr7t41fSZJxK3 1.000 out fund_opreturn_ltc_F591B5B23A6D86942DE7723F700454F5C05765CA8D8F90EFD7BC4E1DA7931AED.script 0.999 extra fixednounce")
if retval1 != 0:
	print "test 10 error"
	sys.exit()

if not os.path.isfile("ltc_7A18D3C8A0BB45F52C4D8B8E63FC4FF4BA2A9FA1FDCBB8D2BF57C18855B522F2.trans"):
	print "test 10 file error"
	sys.exit()


printHeader("Test 11 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 1231231231231231231231231231231236666666555555555544444444444441 0 TARBM4FSBrKux1S41BDyc5AFjFfdGQD9ottejMHUr7t41fSZJxK3 1.000 out fund_anyone_ltc.script 0.999 extra fixednounce")
if retval1 != 0:
	print "test 11 error"
	sys.exit()

if not os.path.isfile("ltc_51262D6F0AF48A17A0E304F5C65C3CB5948DF15D2F1A88AA018CAAE2E622F3C5.trans"):
	print "test 11 file error"
	sys.exit()



print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.trans *.script *.txt")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


sys.exit(123)
