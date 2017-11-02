#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-nmc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============



printHeader("Test 1 bitgen-nmc multisig")
retval1 = os.system("../bitgen-nmc multisig 1 2 7YRiUmnDtj4wsFuZ6nmtkhrVdxD9BogwtkddeNNX9zBMUBW9a78 7ZYcTr3WDEqQB4rB76jWSkUvx56xfem14ikU69T1BQdDECRwQFS")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("multi_redeem_nmc_6TBX3PyJnPM1Kd8W95tVLiMsfJK9haGBNP.script"):
	print "test 1 file error"
	sys.exit()




printHeader("Test 2 bitgen-nmc transaction")
retval1 = os.system("../bitgen-nmc transaction 1342342342342342342342342342342342343234243423333242342342342349 0 multi_redeem_nmc_6TBX3PyJnPM1Kd8W95tVLiMsfJK9haGBNP.script 11.000 out NAQrTbLfubaUgDgCAAeDUPiYu2CExLSCE4 10.999")
if retval1 != 0:
	print "test 2 error"
	sys.exit()

if not os.path.isfile("nmc_nonsigned_B17C08A36109B966BA985F2963A24F5DF60266475233DCF060D08025CF400395.trans"):
	print "test 2 file error"
	sys.exit()




printHeader("Test 3 bitgen-nmc signmultisig")
retval1 = os.system("../bitgen-nmc signmultisig nmc_nonsigned_B17C08A36109B966BA985F2963A24F5DF60266475233DCF060D08025CF400395.trans Kzkm6ggMSLbkZkBVLJ58HrJeMhPe8VXktPpcUYSGxqksYjkS1kod fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("sig_nmc_7ZYcTr3WDEqQB4rB76jWSkUvx56xfem14ikU69T1BQdDECRwQFS.sig"):
	print "test 3 file error"
	sys.exit()




printHeader("Test 4 bitgen-nmc signmultisig")
retval1 = os.system("../bitgen-nmc signmultisig nmc_nonsigned_B17C08A36109B966BA985F2963A24F5DF60266475233DCF060D08025CF400395.trans Kzkm6ggMSLbkZkBVLJ58HrJeMhPe8VXktPpcUYSGxqksYjkS1kod fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("sig_nmc_7ZYcTr3WDEqQB4rB76jWSkUvx56xfem14ikU69T1BQdDECRwQFS.sig"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-nmc combinemultisig")
retval1 = os.system("../bitgen-nmc combinemultisig nmc_nonsigned_B17C08A36109B966BA985F2963A24F5DF60266475233DCF060D08025CF400395.trans sig_nmc_7ZYcTr3WDEqQB4rB76jWSkUvx56xfem14ikU69T1BQdDECRwQFS.sig")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("nmc_4ADC87027FB336E62C3C0EDD330BA625B5E6AF1188251A1BC70564BB542E433D.trans"):
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


