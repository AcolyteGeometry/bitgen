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

if not os.path.isfile("../bitgen-btc"):
	print "Error, application binary does not exist"
	sys.exit()

if not os.path.isfile("../bitgen-ltc"):
	print "Error, application binary does not exist"
	sys.exit()

#============ Start of tests ============


retval = os.system("./testaddress-btc.py")
if retval != 31488:
	print "combined test 1 error"
	sys.exit()

retval = os.system("./testaddress-ltc.py")
if retval != 31488:
	print "combined test 2 error"
	sys.exit()

retval = os.system("./testaddress-doge.py")
if retval != 31488:
	print "combined test 3 error"
	sys.exit()

retval = os.system("./testaddress-vtc.py")
if retval != 31488:
	print "combined test 4 error"
	sys.exit()

retval = os.system("./testaddress-nmc.py")
if retval != 31488:
	print "combined test 5 error"
	sys.exit()

retval = os.system("./testaddress-ppc.py")
if retval != 31488:
	print "combined test 6 error"
	sys.exit()


retval = os.system("./testtransauto-vtc.py")
if retval != 31488:
	print "combined test 7 error"
	sys.exit()

retval = os.system("./testtransauto-btc.py")
if retval != 31488:
	print "combined test 8 error"
	sys.exit()

retval = os.system("./testtransauto-ltc.py")
if retval != 31488:
	print "combined test 9 error"
	sys.exit()

retval = os.system("./testtransauto-doge.py")
if retval != 31488:
	print "combined test 10 error"
	sys.exit()

retval = os.system("./testtransauto-ftc.py")
if retval != 31488:
	print "combined test 11 error"
	sys.exit()

retval = os.system("./testtransauto-nmc.py")
if retval != 31488:
	print "combined test 12 error"
	sys.exit()


##retval = os.system("./testtimelock-btc.py")
##if retval != 31488:
##	print "combined test 13 error"
##	sys.exit()
##
##retval = os.system("./testtimelock-ltc.py")
##if retval != 31488:
##	print "combined test 14 error"
##	sys.exit()
##
##
##retval = os.system("./testmultisig-btc.py")
##if retval != 31488:
##	print "combined test 15 error"
##	sys.exit()
##
##
##retval = os.system("./testmultisig-doge.py")
##if retval != 31488:
##	print "combined test 16 error"
##	sys.exit()
##
##
##retval = os.system("./testmultisig-ltc.py")
##if retval != 31488:
##	print "combined test 17 error"
##	sys.exit()
##
##
##retval = os.system("./testmultisig-vtc.py")
##if retval != 31488:
##	print "combined test 18 error"
##	sys.exit()
##
##retval = os.system("./testmultisig-ftc.py")
##if retval != 31488:
##	print "combined test 19 error"
##	sys.exit()
##
##retval = os.system("./testmultisig-nmc.py")
##if retval != 31488:
##	print "combined test 20 error"
##	sys.exit()
##
##
##
##retval = os.system("./testoutscript-btc.py")
##if retval != 31488:
##	print "combined test 21 error"
##	sys.exit()
##
##retval = os.system("./testoutscript-ltc.py")
##if retval != 31488:
##	print "combined test 22 error"
##	sys.exit()
##


#retval = os.system("./testextended-btc.py")
#if retval != 31488:
#	print "extended test 23 error"
#	sys.exit()

retval = os.system("./testmnemonic-btc.py")
if retval != 31488:
	print "extended test 24 error"
	sys.exit()


print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.trans *.script *.txt *.sig")

print ""
print ""
print ""
print "##############################################################################"
print "####################### All combined tests passed OK #########################"
print "##############################################################################"


