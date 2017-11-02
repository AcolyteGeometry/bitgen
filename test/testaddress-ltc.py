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

printHeader("Test 1 bitgen-ltc info")
retval1 = os.system("../bitgen-ltc info T4kEdhQRLaq7uKuSd7riv7uveLJqBZHCLJD2KbJDVe2YYfFBc5uh")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

printHeader("Test 2 bitgen-ltc info err")
retval1 = os.system("../bitgen-ltc info T4kEdhQRrrrrrrrrrrrrrrrrrrrrBZHCLJD2KbJDVe2YYfFBc5uh")
if retval1 == 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-ltc info")
retval1 = os.system("../bitgen-ltc info LbpdpruGj37iFgKy8jqB3fg2h76HW6guBF")
if retval1 != 0:
	print "test 3 error"
	sys.exit()

printHeader("Test 4 bitgen-ltc info err")
retval1 = os.system("../bitgen-ltc info LbpdpruGyyyyyyyy8jqB3fg2h76HW6guBF")
if retval1 == 0:
	print "test 4 error"
	sys.exit()



printHeader("Test 5 bitgen-ltc hash")
retval1 = os.system("../bitgen-ltc hash kdfdksjdhfjksdhfjhdsjfhjdhfjhkfjsdjjjdjjdjdjdjjdsjfdkfjhsdkfjhsdfkjhsdkfjhsdkjfhskjdhfsadjhasdkhaskdjhaksjdhkjhjhhhhfhfhjdjjjdjjdjdjjdsadjalskdjaslkjdalksjdlaksjdlkasjdlaksjdlkasjdlkasjdlaksjdlkasjdasdasda nops")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("LgJDjYo7HPan1jp79Nkv28thiiNMZ4wXFN_private.txt"):
	print "test 5 file error"
	sys.exit()



printHeader("Test 6 bitgen-ltc hdice")
retval1 = os.system("../bitgen-ltc dice 6126361236126312635353526351263512635162536125361253612536125312312312312312312312312312312321332131 nops")
if retval1 != 0:
	print "test 6 error"
	sys.exit()


if not os.path.isfile("Le4owbhiJSq27ph4t47Z7Cxq8Wo9RwnXp8_private.txt"):
	print "test 6 file error"
	sys.exit()



printHeader("Test 7 bitgen-ltc hex")
retval1 = os.system("../bitgen-ltc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423444 nops")
if retval1 != 0:
	print "test 7 error"
	sys.exit()


if not os.path.isfile("LVsmwg4hGCcf9Gs4SMH1nNvYrTar9trskY_private.txt"):
	print "test 7 file error"
	sys.exit()



printHeader("Test 8 bitgen-ltc hex compressed")
retval1 = os.system("../bitgen-ltc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423417 compressed nops")
if retval1 != 0:
	print "test 8 error"
	sys.exit()


if not os.path.isfile("LbB1N8t1uEXfVuzshDQdPoN6J5ZmzDHhPE_private.txt"):
	print "test 8 file error"
	sys.exit()


printHeader("Test 9 bitgen-ltc mnemonic")
retval1 = os.system("../bitgen-ltc mnemonic \"nice upset area coconut emerge swim tomorrow hospital luggage mushroom abuse odor color early bicycle slow cart lamp gas lawn lava cancel insane amused\" nops")
if retval1 != 0:
	print "test 9 error"
	sys.exit()


if not os.path.isfile("LPmaVosTj8Vwo2ppH7Qv7ruSXGR3KrkDW5_private.txt"):
	print "test 9 file error"
	sys.exit()


printHeader("Test 10 bitgen-ltc mnemonic")
retval1 = os.system("../bitgen-ltc wif T697bD97y9WQq4Juh7rLfU3Bqy3SJz5vULqUTdpsPti6fjN8rYr3 nops")
if retval1 != 0:
	print "test 10 error"
	sys.exit()


if not os.path.isfile("LPmaVosTj8Vwo2ppH7Qv7ruSXGR3KrkDW5_private.txt"):
	print "test 10 file error"
	sys.exit()


printHeader("Test 11 bitgen-ltc mnemonic")
retval1 = os.system("../bitgen-ltc brainwallet mysalt@test.com \"This is the rather long passphrase that is used tO tEst thiS thing so we know that iit is workingg CorrectlY\" nops")
if retval1 != 0:
	print "test 11 error"
	sys.exit()


if not os.path.isfile("LVAgFr2VsQigALNZTdjjaFZjBp6g9EnEp3_private.txt"):
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
