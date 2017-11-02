#!/usr/bin/python

import os
import sys
import os.path

def hasSubstring(myStr, substr):
	if myStr.find(substr) == -1:
		return False
	else:
		return True
		

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-btc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============





printHeader("Test 1 bitgen-btc extended, test vectors from BIP 32")
retval1 = os.system("../bitgen extended seed 000102030405060708090a0b0c0d0e0f")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("extended_wybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8_private.txt"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-btc extended, test vectors from BIP 32")
retval1 = os.system("../bitgen extended infopriv xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-btc extended, test vectors from BIP 32")
output = os.popen("../bitgen extended infopriv xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi").read()

print "Got THIS: " + output

#Chaincode
if not hasSubstring(output, "873DFF81C02F525623FD1FE5167EAC3A55A049DE3D314BB42EE227FFED37D508"):
	print "test 3 error"
	sys.exit()

#Fingerprint
if not hasSubstring(output, "3442193E"):
	print "test 3 error"
	sys.exit()

#Pubaddress
if not hasSubstring(output, "15mKKb2eos1hWa6tisdPwwDC1a5J1y9nma"):
	print "test 3 error"
	sys.exit()

#Privaddress
if not hasSubstring(output, "L52XzL2cMkHxqxBXRyEpnPQZGUs3uKiL3R11XbAdHigRzDozKZeW"):
	print "test 3 error"
	sys.exit()


#Priv key hex
if not hasSubstring(output, "E8F32E723DECF4051AEFAC8E2C93C9C5B214313817CDB01A1494B917C8436B35"):
	print "test 3 error"
	sys.exit()



printHeader("Test 4 bitgen-btc extended, test vectors from BIP 32")
derOutput = os.popen("../bitgen derivation xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi \"m/0H\"").read()


if not hasSubstring(derOutput, "xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7"):
	print "test 4.1 error"
	sys.exit()



printHeader("Test 5 bitgen-btc extended, test vectors from BIP 32")
myderOutput = os.popen("../bitgen extended infopriv xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7").read()



if not hasSubstring(myderOutput, "19Q2WoS5hSS6T8GjhK8KZLMgmWaq4neXrh"):
	print "test 5.1 error"
	sys.exit()


if not hasSubstring(myderOutput, "L5BmPijJjrKbiUfG4zbiFKNqkvuJ8usooJmzuD7Z8dkRoTThYnAT"):
	print "test 5.2 error"
	sys.exit()



if not hasSubstring(myderOutput, "19Q2WoS5hSS6T8GjhK8KZLMgmWaq4neXrh"):
	print "test 5.3 error"
	sys.exit()


if not hasSubstring(myderOutput, "L5BmPijJjrKbiUfG4zbiFKNqkvuJ8usooJmzuD7Z8dkRoTThYnAT"):
	print "test 5.5 error"
	sys.exit()



printHeader("Test 6 bitgen-btc extended, test vector 2 from BIP 32")
derOutput2 = os.popen("../bitgen extended seed fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542").read()

print "Test 6 result: " + derOutput2

if not hasSubstring(derOutput2, "xpub661MyMwAqRbcFW31YEwpkMuc5THy2PSt5bDMsktWQcFF8syAmRUapSCGu8ED9W6oDMSgv6Zz8idoc4a6mr8BDzTJY47LJhkJ8UB7WEGuduB"):
	print "test 6.1 error"
	sys.exit()

if not hasSubstring(derOutput2, "xprv9s21ZrQH143K31xYSDQpPDxsXRTUcvj2iNHm5NUtrGiGG5e2DtALGdso3pGz6ssrdK4PFmM8NSpSBHNqPqm55Qn3LqFtT2emdEXVYsCzC2U"):
	print "test 6.2 error"
	sys.exit()





print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.trans *.script *.txt *.sig *.ps")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

sys.exit(123)

