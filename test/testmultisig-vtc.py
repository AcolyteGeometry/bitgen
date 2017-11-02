#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-vtc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============



printHeader("Test 1 bitgen-vtc multisig")
retval1 = os.system("../bitgen-vtc multisig 1 2 7WRjSaVKr9ftSPixTf1mHyzFfQgrKuu7CJjh6oyRToJGwU4iYM1 7Z23Mk1xjFGjMMwrzq9njv5GPB4g1AzQGrAwytgkdzuZyXF1NeK")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("multi_redeem_vtc_35CgmWeBsE28AiM2nbWZMP6THpn39epMVS.script"):
	print "test 1 file error"
	sys.exit()




printHeader("Test 2 bitgen-vtc transaction")
retval1 = os.system("../bitgen-vtc transaction 1342342342342342342342342342342342343234243423333242342342342349 0 multi_redeem_vtc_35CgmWeBsE28AiM2nbWZMP6THpn39epMVS.script 11.000 out Vu6x5fYc1JEv6QL93P1qYDWi2LcG2oEK69 10.999")
if retval1 != 0:
	print "test 2 error"
	sys.exit()

if not os.path.isfile("vtc_nonsigned_8A2B2DF881945F75999A508B263004D8530CF44C5F9B7B3B4A7DE2B500790E29.trans"):
	print "test 2 file error"
	sys.exit()




printHeader("Test 3 bitgen-vtc signmultisig")
retval1 = os.system("../bitgen-vtc signmultisig vtc_nonsigned_8A2B2DF881945F75999A508B263004D8530CF44C5F9B7B3B4A7DE2B500790E29.trans WWqKLb37o4WWA7AbFUGkDcubNXAZGGApUTc2KHun3uRH7zJVvia4 fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("sig_vtc_7WRjSaVKr9ftSPixTf1mHyzFfQgrKuu7CJjh6oyRToJGwU4iYM1.sig"):
	print "test 3 file error"
	sys.exit()




printHeader("Test 4 bitgen-vtc signmultisig")
retval1 = os.system("../bitgen-vtc signmultisig vtc_nonsigned_8A2B2DF881945F75999A508B263004D8530CF44C5F9B7B3B4A7DE2B500790E29.trans WWqKLb37o4WWA7AbFUGkDcubNXAZGGApUTc2KHun3uRH7zJVvia4 fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("sig_vtc_7WRjSaVKr9ftSPixTf1mHyzFfQgrKuu7CJjh6oyRToJGwU4iYM1.sig"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-vtc combinemultisig")
retval1 = os.system("../bitgen-vtc combinemultisig vtc_nonsigned_8A2B2DF881945F75999A508B263004D8530CF44C5F9B7B3B4A7DE2B500790E29.trans sig_vtc_7WRjSaVKr9ftSPixTf1mHyzFfQgrKuu7CJjh6oyRToJGwU4iYM1.sig")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("vtc_87B25C7AD5DBE734465CEF70F1249253A97030C41B756D2CA001EF8C4B98F02A.trans"):
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


