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

printHeader("Test 1 bitgen-nmc info")
retval1 = os.system("../bitgen-nmc info KycSshTJeeZ6JCUsfdk9cs3h1s6SHA8Mt4Mf95knVCHTRf4rBmmF")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

printHeader("Test 2 bitgen-nmc info err")
retval1 = os.system("../bitgen-nmc info KycSshTJejjjjjjjjjjjjjjjjjjjjj8Mt4Mf95knVCHTRf4rBmmF")
if retval1 == 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-nmc info")
retval1 = os.system("../bitgen-nmc info N9NYy6LyMRj7w1RJR5czvfxXMv22q134PV")
if retval1 != 0:
	print "test 3 error"
	sys.exit()

printHeader("Test 4 bitgen-nmc info err")
retval1 = os.system("../bitgen-nmc info N9NYy6LyMRj7wttttttttttttt22q134PV")
if retval1 == 0:
	print "test 4 error"
	sys.exit()



printHeader("Test 5 bitgen-nmc hash")
retval1 = os.system("../bitgen-nmc hash kdfdksjdhfjksdhfjhdsjfhjdhfjhkfjsdjjjdjjdjdjdjjdsjfdkfjhsdkfjhsdfkjhsdkfjhsdkjfhskjdhfsadjhasdkhaskdjhaksjdhkjhjhhhhfhfhjdjjjdjjdjdjjdsadjalskdjaslkjdalksjdlaksjdlkasjdlaksjdlkasjdlkasjdlaksjdlkasjdasdasda nops")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("NHedfyzG87SHHUNTF46BxdyrEjQ8P4XVYN_private.txt"):
	print "test 5 file error"
	sys.exit()



printHeader("Test 6 bitgen-nmc hdice")
retval1 = os.system("../bitgen-nmc dice 6126361236126312635353526351263512635162536125361253612536125312312312312312312312312312312321332131 nops")
if retval1 != 0:
	print "test 6 error"
	sys.exit()


if not os.path.isfile("NFRDt2ts9AgXPZFQyjSq3i3yeXpvMqC7ja_private.txt"):
	print "test 6 file error"
	sys.exit()



printHeader("Test 7 bitgen-nmc hex")
retval1 = os.system("../bitgen-nmc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423444 nops")
if retval1 != 0:
	print "test 7 error"
	sys.exit()


if not os.path.isfile("N7EBt7Fr6vUAR1RQY2cHit1hNUcd1bgcJz_private.txt"):
	print "test 7 file error"
	sys.exit()



printHeader("Test 8 bitgen-nmc hex compressed")
retval1 = os.system("../bitgen-nmc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423417 compressed nops")
if retval1 != 0:
	print "test 8 error"
	sys.exit()


if not os.path.isfile("NCXRJa5AjxPAmeZDntjuLJTEp6bYrj1Aj4_private.txt"):
	print "test 8 file error"
	sys.exit()


printHeader("Test 9 bitgen-nmc mnemonic")
retval1 = os.system("../bitgen-nmc mnemonic \"nice upset area coconut emerge swim tomorrow hospital luggage mushroom abuse odor color early bicycle slow cart lamp gas lawn lava cancel insane amused\" nops")
if retval1 != 0:
	print "test 9 error"
	sys.exit()


if not os.path.isfile("N17zSF4cZrMT4mPANnkC4Mzb3HSpA4VGuA_private.txt"):
	print "test 9 file error"
	sys.exit()


printHeader("Test 10 bitgen-nmc wif")
retval1 = os.system("../bitgen-nmc wif KxgzFekUUMYcUjppjUVsqW3B7oPPMvwHEMqLBhkBjEkwpTPi3nBo nops")
if retval1 != 0:
	print "test 10 error"
	sys.exit()


if not os.path.isfile("Mz84nbyf8na8NBXAxBr1sC72HUhKcf7AxJ_private.txt"):
	print "test 10 file error"
	sys.exit()


printHeader("Test 11 bitgen-nmc mnemonic")
retval1 = os.system("../bitgen-nmc brainwallet mysalt@test.com \"This is the rather long passphrase that is used tO tEst thiS thing so we know that iit is workingg CorrectlY\" nops")
if retval1 != 0:
	print "test 11 error"
	sys.exit()


if not os.path.isfile("N6X6CHDei8aBS4vuZK51Wkeshq8SumYvJo_private.txt"):
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
