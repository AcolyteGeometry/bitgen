#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-ppc"):
	print "Error, application binary does not exist"
	sys.exit()




#============ Start of tests ============

printHeader("Test 1 bitgen-ppc info")
retval1 = os.system("../bitgen-ppc info U8kkZrsN3f9cCujnsuJ5jLTh245B5tns1XRUESiaAqxrZULhG9zw")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

printHeader("Test 2 bitgen-ppc info err")
retval1 = os.system("../bitgen-ppc info U8kkZrsN3kkkkkkkkkkkkkkkkkkkkkkkkkRUESiaAqxrZULhG9zw")
if retval1 == 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-ppc info")
retval1 = os.system("../bitgen-ppc info PTgCW43WNHR5RGHdHy76FWqxuAi34chVz7")
if retval1 != 0:
	print "test 3 error"
	sys.exit()

printHeader("Test 4 bitgen-ppc info err")
retval1 = os.system("../bitgen-ppc info PTgCW43Wwwwwwwwwwwwwwwwwwww34chVz7")
if retval1 == 0:
	print "test 4 error"
	sys.exit()



printHeader("Test 5 bitgen-ppc hash")
retval1 = os.system("../bitgen-ppc hash kdfdksjdhfjksdhfjhdsjfhjdhfjhkfjsdjjjdjjdjdjdjjdsjfdkfjhsdkfjhsdfkjhsdkfjhsdkjfhskjdhfsadjhasdkhaskdjhaksjdhkjhjhhhhfhfhjdjjjdjjdjdjjdsadjalskdjaslkjdalksjdlaksjdlkasjdlaksjdlkasjdlkasjdlaksjdlkasjdasdasda nops")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("PVfSdJt8FepujmniKK69R1oD8FAxa3k9Hq_private.txt"):
	print "test 5 file error"
	sys.exit()



printHeader("Test 6 bitgen-ppc hdice")
retval1 = os.system("../bitgen-ppc dice 6126361236126312635353526351263512635162536125361253612536125312312312312312312312312312312321332131 nops")
if retval1 != 0:
	print "test 6 error"
	sys.exit()


if not os.path.isfile("PTS2qMnjGi59qrfg3zSnW5sLY3bkQexegi_private.txt"):
	print "test 6 file error"
	sys.exit()



printHeader("Test 7 bitgen-ppc hex")
retval1 = os.system("../bitgen-ppc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423444 nops")
if retval1 != 0:
	print "test 7 error"
	sys.exit()


if not os.path.isfile("PKEzqS9iETrnsJqfcHcFBFq4FzPT9atTDb_private.txt"):
	print "test 7 file error"
	sys.exit()



printHeader("Test 8 bitgen-ppc hex compressed")
retval1 = os.system("../bitgen-ppc hex 116161678ABCDEFEFA1231231236127327633764736473643434342343423417 compressed nops")
if retval1 != 0:
	print "test 8 error"
	sys.exit()


if not os.path.isfile("PQYEFty2sVmoDwyUs9jrngGbhcNNzXK2dP_private.txt"):
	print "test 8 file error"
	sys.exit()


printHeader("Test 9 bitgen-ppc mnemonic")
retval1 = os.system("../bitgen-ppc mnemonic \"nice upset area coconut emerge swim tomorrow hospital luggage mushroom abuse odor color early bicycle slow cart lamp gas lawn lava cancel insane amused\" nops")
if retval1 != 0:
	print "test 9 error"
	sys.exit()


if not os.path.isfile("PD8oPZxUhPk5X4oRT3k9WjowvoDeCY6ixh_private.txt"):
	print "test 9 file error"
	sys.exit()


printHeader("Test 10 bitgen-ppc wif")
retval1 = os.system("../bitgen-ppc wif UDUoW47FCbqVaz4vRCaYL5zGPC37v8VBJTKDxbsPp4pNbZ7f8Nua nops")
if retval1 != 0:
	print "test 10 error"
	sys.exit()


if not os.path.isfile("PLbWb33DVKrkFoUHWSBwdpRPNora8kbamR_private.txt"):
	print "test 10 file error"
	sys.exit()


printHeader("Test 11 bitgen-ppc mnemonic")
retval1 = os.system("../bitgen-ppc brainwallet mysalt@test.com \"This is the rather long passphrase that is used tO tEst thiS thing so we know that iit is workingg CorrectlY\" nops")
if retval1 != 0:
	print "test 11 error"
	sys.exit()


if not os.path.isfile("PJXu9c7WqfxotNMAda4xy8UEbLuH7Yks9T_private.txt"):
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
