#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-doge"):
	print "Error, application binary does not exist"
	sys.exit()




#============ Start of tests ============

printHeader("Test 1 bitgen-doge info")
retval1 = os.system("../bitgen-doge info QP9fkrTu6bG15M5WZRFHZPhpoSRrr6iJd8YrZLrXYY8oQq16AGxM")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

printHeader("Test 2 bitgen-doge info err")
retval1 = os.system("../bitgen-doge info QP9fkrTu6tttttttttttttttttttttttd8YrZLrXYY8oQq16AGxM")
if retval1 == 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-doge info")
retval1 = os.system("../bitgen-doge info DEPSRH8zo5tQ2WicX1q3XjLaMomnLVKDy6")
if retval1 != 0:
	print "test 3 error"
	sys.exit()

printHeader("Test 4 bitgen-doge info err")
retval1 = os.system("../bitgen-doge info DEPSRH8zbbbbbbbbbbbbbbbbbbbbLVKDy6")
if retval1 == 0:
	print "test 4 error"
	sys.exit()



printHeader("Test 5 bitgen-doge hash")
retval1 = os.system("../bitgen-doge hash kdfdksjdhfjksdhfjhdsjfhjdhfjhkfjsdjjjdjjdjdjdjjdsjfdkfjhsdkfjhsdfkjhsdkfjhsdkjfhskjdhfsadjhasdkhaskdjhaksjdhkjhjhhhhfhfhjdjjjdjjdjdjjdsadjalskdjaslkjdalksjdlaksjdlkasjdlaksjdlkasjdlkasjdlaksjdlkasjdasdasda nops")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("DSDN1bRvW9F1HwJYhpmBHszYPdjNinYCBr_private.txt"):
	print "test 5 file error"
	sys.exit()



printHeader("Test 6 bitgen-doge hdice")
retval1 = os.system("../bitgen-doge dice 6126361236126312635353526351263512635162536125361253612536125312312312312312312312312312312321332131 nops")
if retval1 != 0:
	print "test 6 error"
	sys.exit()


if not os.path.isfile("DPyxDeLXXCVFQ2BWSW7pNx4foSAAjHr4wB_private.txt"):
	print "test 6 file error"
	sys.exit()



printHeader("Test 7 bitgen-doge hex")
retval1 = os.system("../bitgen-doge hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423444 nops")
if retval1 != 0:
	print "test 7 error"
	sys.exit()


if not os.path.isfile("DFnvDihWUxGtRUMVzoHH482PXNwsSmnxQs_private.txt"):
	print "test 7 file error"
	sys.exit()



printHeader("Test 8 bitgen-doge hex compressed")
retval1 = os.system("../bitgen-doge hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423417 compressed nops")
if retval1 != 0:
	print "test 8 error"
	sys.exit()


if not os.path.isfile("DM69eBWq7zBtn7VKFfQtfYTvxzvoBswDKg_private.txt"):
	print "test 8 file error"
	sys.exit()


printHeader("Test 9 bitgen-doge mnemonic")
retval1 = os.system("../bitgen-doge mnemonic \"nice upset area coconut emerge swim tomorrow hospital luggage mushroom abuse odor color early bicycle slow cart lamp gas lawn lava cancel insane amused\" nops")
if retval1 != 0:
	print "test 9 error"
	sys.exit()


if not os.path.isfile("D9gimrWGwtAB5EKFqZRBPc1HCBn4SQT6mi_private.txt"):
	print "test 9 file error"
	sys.exit()


printHeader("Test 10 bitgen-doge wif")
retval1 = os.system("../bitgen-doge wif QR3yxnXHRBzUDSgCxga5P3teZPHzJmP9EDrL4bGamh32DhXWX5WF nops")
if retval1 != 0:
	print "test 10 error"
	sys.exit()


if not os.path.isfile("DCceWQ52LuKQxf6ycKAtBPVykZpsxDDGmY_private.txt"):
	print "test 10 file error"
	sys.exit()


printHeader("Test 11 bitgen-doge mnemonic")
retval1 = os.system("../bitgen-doge brainwallet mysalt@test.com \"This is the rather long passphrase that is used tO tEst thiS thing so we know that iit is workingg CorrectlY\" nops")
if retval1 != 0:
	print "test 11 error"
	sys.exit()


if not os.path.isfile("DF5pXtfK6ANuSXs125jzqzfZrjThFRgp4L_private.txt"):
	print "test 11 file error"
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
