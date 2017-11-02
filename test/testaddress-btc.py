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

printHeader("Test 1 bitgen-btc info")
retval1 = os.system("../bitgen-btc info Kyaogh9vZXqRkVr8yyyqQuTTataBGzv4GMY7g5nK9kTsEzu9nQHK")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

printHeader("Test 2 bitgen-btc info err")
retval1 = os.system("../bitgen-btc info Kyaogh66666666666666666666666666666665nK9kTsEzu9nQHK")
if retval1 == 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-btc info")
retval1 = os.system("../bitgen-btc info 13KJmY9eL3w32LtHLYU48BD23iuXPRtauW")
if retval1 != 0:
	print "test 3 error"
	sys.exit()

printHeader("Test 4 bitgen-btc info err")
retval1 = os.system("../bitgen-btc info 13KJ777777777777777777D23iuXPRtauW")
if retval1 == 0:
	print "test 4 error"
	sys.exit()



printHeader("Test 5 bitgen-btc hash")
retval1 = os.system("../bitgen-btc hash kdfdksjdhfjksdhfjhdsjfhjdhfjhkfjsdjjjdjjdjdjdjjdsjfdkfjhsdkfjhsdfkjhsdkfjhsdkjfhskjdhfsadjhasdkhaskdjhaksjdhkjhjhhhhfhfhjdjjjdjjdjdjjdsadjalskdjaslkjdalksjdlaksjdlkasjdlaksjdlkasjdlkasjdlaksjdlkasjdasdasda nops")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("1N5GULVHCjLikw7wyEmck7pwWW15U3waYv_private.txt"):
	print "test 5 file error"
	sys.exit()



printHeader("Test 6 bitgen-btc hdice")
retval1 = os.system("../bitgen-btc dice 6126361236126312635353526351263512635162536125361253612536125312312312312312312312312312312321332131 nops")
if retval1 != 0:
	print "test 6 error"
	sys.exit()


if not os.path.isfile("1KqrgPPtDnaxs1zuhv8FqBu4vJRsPiqEcd_private.txt"):
	print "test 6 file error"
	sys.exit()



printHeader("Test 7 bitgen-btc hex")
retval1 = os.system("../bitgen-btc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423444 nops")
if retval1 != 0:
	print "test 7 error"
	sys.exit()


if not os.path.isfile("1BepgTksBYNbtUAuGDHiWMrneFDa3PwBzW_private.txt"):
	print "test 7 file error"
	sys.exit()



printHeader("Test 8 bitgen-btc hex compressed")
retval1 = os.system("../bitgen-btc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423417 compressed nops")
if retval1 != 0:
	print "test 8 error"
	sys.exit()


if not os.path.isfile("1Gx46vaBpaHcF7JiX5RL7nJL5sCVtfxTRr_private.txt"):
	print "test 8 file error"
	sys.exit()


printHeader("Test 9 bitgen-btc mnemonic")
retval1 = os.system("../bitgen-btc mnemonic \"nice upset area coconut emerge swim tomorrow hospital luggage mushroom abuse odor color early bicycle slow cart lamp gas lawn lava cancel insane amused\" nops")
if retval1 != 0:
	print "test 9 error"
	sys.exit()


if not os.path.isfile("15YdEbZdeUFtYE8f6yRcqqqgK43mCgEb93_private.txt"):
	print "test 9 file error"
	sys.exit()


printHeader("Test 10 bitgen-btc mnemonic")
retval1 = os.system("../bitgen-btc wif L4NM9PQ82dLVhfkAQ4ofxj4VqwcyfP7fwy2b49GdG7fKEdKmLv8D nops")
if retval1 != 0:
	print "test 10 error"
	sys.exit()


if not os.path.isfile("1AwohJQY59DpxhMGqm3xfMfStS6nE5KL21_private.txt"):
	print "test 10 file error"
	sys.exit()


printHeader("Test 11 bitgen-btc mnemonic")
retval1 = os.system("../bitgen-btc brainwallet mysalt@test.com \"This is the rather long passphrase that is used tO tEst thiS thing so we know that iit is workingg CorrectlY\" nops")
if retval1 != 0:
	print "test 11 error"
	sys.exit()


if not os.path.isfile("1AwizdifnkUcuXgQHVkSJEVxybjPw7KHqj_private.txt"):
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
