#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen") or not os.path.isfile("../bitsig") or not os.path.isfile("../bitcry"):
	print "Error, application binaries do not exist"
	sys.exit()


if os.path.isfile("out_plain.txt"):
	os.remove("out_plain.txt")

if os.path.isfile("12xKq6VN8wfNoHwaraJiYG5cGNP2dospEu_share.txt"):
	os.remove("12xKq6VN8wfNoHwaraJiYG5cGNP2dospEu_share.txt")


if os.path.isfile("16uoLAEFbHw3a8rw8dg4wWbhMyaBWzEC9C_share.txt"):
	os.remove("16uoLAEFbHw3a8rw8dg4wWbhMyaBWzEC9C_share.txt")

if os.path.isfile("13CAC32E.armory"):
	os.remove("13CAC32E.armory")


#============ Start of tests ============

printHeader("Test 1")
retval1 = os.system("../bitsig verify testfiles/signedFile.txt")
if retval1 != 0:
	print "test 1 error"
	sys.exit()



printHeader("Test 2")
retval2 = os.system("../bitsig verify testfiles/errorsig.bin")
if retval2 == 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3")
retval3 = os.system("../bitcry decrypt testfiles/plain.txt.bitcry 5KjdzDrcyBfXDsRjwX5Uy5yZmSnKDPEWFujXP29LTiZpoJxWqeA")
if retval3 != 0:
	print "test 3 error"
	sys.exit()

#Verify that out_plain.txt is present

if not os.path.isfile("out_plain.txt"):
	print "Error, file does not exist"
	sys.exit()

myfile = open("out_plain.txt", 'r')

plainfile = myfile.read()
print plainfile
if plainfile != "plain\n":
	print "Error, incorrect file content"
	sys.exit()
	

printHeader("Test 4")
retval4 = os.system("../bitcry decrypt testfiles/plain.txt.bitcry")
if retval4 != 0:
	print "test 4 error"
	sys.exit()

#Verify that out_plain.txt is present

if not os.path.isfile("out_plain.txt"):
	print "Error, file does not exist"
	sys.exit()

myfile = open("out_plain.txt", 'r')

plainfile = myfile.read()
print plainfile
if plainfile != "plain\n":
	print "Error, incorrect file content"
	sys.exit()


printHeader("Test 5")
retval4 = os.system("../bitsig verifyMultibit testfiles/DAC344C4.multibit")
if retval4 != 0:
	print "test 5 error"
	sys.exit()

printHeader("Test 6")
retval5 = os.system("../bitsig verifyArmory testfiles/8E528116.armory")
if retval5 != 0:
	print "test 6 error"
	sys.exit()


printHeader("Test 7")
retval7 = os.system("../bitsig verifyAb64 testfiles/1338BB38.armoryb64")
if retval7 != 0:
	print "test 7 error"
	sys.exit()


printHeader("Test 8")
retval8 = os.system("../bitsig verifyAclear testfiles/46C746F4.armoryclear")
if retval8 != 0:
	print "test 8 error"
	sys.exit()



printHeader("Test 9 InputsIo sign OK")
retval9 = os.system("../bitsig verifyInputsIo testfiles/signok.inputsio")
if retval9 != 0:
	print "test 9 error"
	sys.exit()

printHeader("Test 10 InputsIo sign error, signature changed")
retval10 = os.system("../bitsig verifyInputsIo testfiles/signerror.inputsio")
if retval10 == 0:
	print "test 10 error"
	sys.exit()

printHeader("Test 11 InputsIo sign error, message changed")
retval11 = os.system("../bitsig verifyInputsIo testfiles/signerrormessagechanged.inputsio")
if retval11 == 0:
	print "test 11 error"
	sys.exit()

printHeader("Test 12 InputsIo sign error, address changed")
retval12 = os.system("../bitsig verifyInputsIo testfiles/signerroraddresschanged.inputsio")
if retval12 == 0:
	print "test 12 error"
	sys.exit()


printHeader("Test 13 bitgen infoPrivate")
retval13 = os.system("../bitgen infoPrivate 5JzsaTDecYGDhZv6u2ndW5uqhKY81r8cq3z4v2HJmQ31gh6jsEt")
if retval13 != 0:
	print "test 13 error"
	sys.exit()

#printHeader("Test 14 bitgen infoPrivate key error")
#retval14 = os.system("../bitgen infoPrivate 5JzsaTDecYGDhZv6u2PdW5uqhKY81r8cq3z4v2HJmQ31gh6jsEt")
#if retval14 == 0:
#	print "test 14 error"
#	sys.exit()


printHeader("Test 15 bitgen dice incorrect num dice")
retval15 = os.system("../bitgen dice 23424356152163562536253615236215356253612536125361523123123 nops")
if retval15 == 0:
	print "test 15 error"
	sys.exit()


printHeader("Test 16 bitgen dice ok")
retval16 = os.system("../bitgen dice 2342435615216356253625361523621535625361253612536152312312323423423423432423423423423423423423423423 nops")
if retval16 != 0:
	print "test 16 Error"
	sys.exit()

if not os.path.isfile("1H4zEXvcPK4R2jC4HDHQYM3qDbhemfSPYp_share.txt"):
	print "test 16 file error"
	sys.exit()


printHeader("Test 17 bitgen dice illegal char")
retval17 = os.system("../bitgen dice 2342435615216356253625361523621535625361253612536152312312323423423423432423GGG423423423423423423423")
if retval17 == 0:
	print "test 17 error"
	sys.exit()

printHeader("Test 18 bitgen hex OK")
retval18 = os.system("../bitgen hex 4654CCCABBCBDEF543543543543123123123BBCBBBBCBBBBBBCB123123123123")
if retval18 != 0:
	print "test 18 error"
	sys.exit()

if not os.path.isfile("1DxBqY5F4nAns6Ck1ASNobeQGc2PmY1BTd_share.txt"):
	print "test 16 file error"
	sys.exit()



printHeader("Test 19 bitgen hex error")
retval19 = os.system("../bitgen hex 4654CCCABBCBDEF543543543543123123123BBCBBBBCBBBBBBCB12312312")
if retval19 == 0:
	print "test 19 error"
	sys.exit()


printHeader("Test 20 bitgen hex digit out of range")
retval20 = os.system("../bitgen hex 4654CCCABBCBDEF543543543543123123123PPPBBBBCBBBBBBCB123123123123")
if retval20 == 0:
	print "test 20 error"
	sys.exit()


printHeader("Test 21 bitgen signArmory")
retval21 = os.system("../bitsig signArmory TheArMessage")
if retval21 != 0:
	print "test 21 error"
	sys.exit()

if not os.path.isfile("13CAC32E.armory"):
	print "test 21 error"
	sys.exit()

printHeader("Test 22 bitgen mnemonic")
retval22 = os.system("../bitgen mnemonic \"fame bind prevent question trap proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval22 != 0:
	print "test 22 error"
	sys.exit()
	
if not os.path.isfile("16SYJFzUBq4jVnVggVLYaCgetMdh2SeL5Z_share.txt"):
	print "test 22 file error"
	sys.exit()

printHeader("Test 23 bitgen mnemonic error")
retval23 = os.system("../bitgen mnemonic \"fame bind prevent trap question proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval23 == 0:
	print "test 23 error"
	sys.exit()

printHeader("Test 24 bitgen mnemonic word error")
retval24 = os.system("../bitgen mnemonic \"fame bind prevent notaword question proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval24 == 0:
	print "test 24 error"
	sys.exit()

printHeader("Test 25 bitgen brainwallet ok")
retval25 = os.system("../bitgen brainwallet tester@test.org \"This is an insecure brainwallet that should only be used for testing and nothing else\"")
if retval25 != 0:
	print "test 25 error"
	sys.exit()

if not os.path.isfile("13wwFKPRvPYMsE2mV7yYFtyVeobV7dxgxX_share.txt"):
	print "test 25 file error"
	sys.exit()


printHeader("Test 26 bitgen pubAddr")
retval26 = os.system("../bitgen pubAddr 1K6HFD9o9uMnyQSkoLH48Q5nnj3vCJCi8j")
if retval26 != 0:
	print "test 26 error"
	sys.exit()

#printHeader("Test 27 bitgen pubAddr error")
#retval27 = os.system("../bitgen pubAddr 1K6HFD9o9uMMyQSkoLH48Q5nnj3vCJCi8j")
#if retval27 == 0:
#	print "test 27 error"
#	sys.exit()

printHeader("Test 28 bitsig sign nonexisting file")
retval28 = os.system("../bitsig sign doesNotExist.txt")
if retval28 == 0:
	print "test 28 error"
	sys.exit()


printHeader("Test 29 bitsig signFile OK")
retval29 = os.system("../bitsig verifyFile testfiles/myLittleFile.txt.bitfsig")
if retval29 != 0:
	print "test 29 error"
	sys.exit()



printHeader("Test 30 bitcry encrypt")
retval30 = os.system("../bitcry encrypt testfiles/toBeEncrypted.txt 7UTZisACfzPr19WUEN24gN3Hng9kEGWgnMwv5AM3DGoLh9UdUoE")
if retval30 != 0:
	print "test 30 error"
	sys.exit()

printHeader("Test 31 bitcry decrypt")
retval31 = os.system("../bitcry decrypt testfiles/toBeEncrypted.txt.bitcry")
if retval31 != 0:
	print "test 31 error"
	sys.exit()


if not os.path.isfile("out_toBeEncrypted.txt"):
	print "test 31 file error"
	sys.exit()



printHeader("Test 32 bitcry import")
retval32 = os.system("../bitcry import")
if retval32 == 0:
	print "test 32 error"
	sys.exit()


printHeader("Test 33 bitcry importPrivate")
retval33 = os.system("../bitcry importPrivate")
if retval33 == 0:
	print "test 33 error"
	sys.exit()


printHeader("Test 34 bitcry list")
retval34 = os.system("../bitcry list")
if retval34 != 0:
	print "test 34 error"
	sys.exit()



printHeader("Test 35 bitcry help")
retval35 = os.system("../bitcry help")
if retval35 != 0:
	print "test 35 error"
	sys.exit()

printHeader("Test 36 bitcry incorrect command")
retval36 = os.system("../bitcry dskjhfskdfh")
if retval36 == 0:
	print "test 36 error"
	sys.exit()



printHeader("Test 37 bitcry missing encrypt argument")
retval37 = os.system("../bitcry encrypt")
if retval37 == 0:
	print "test 37 error"
	sys.exit()



printHeader("Test 38 bitcry missing decrypt argument")
retval38 = os.system("../bitcry decrypt")
if retval38 == 0:
	print "test 38 error"
	sys.exit()



printHeader("Test 39 bitcry encryptElectrum")
retval39 = os.system("../bitcry encryptElectrum ThisIsThePlaintext 7UTZisACfzPr19WUEN24gN3Hng9kEGWgnMwv5AM3DGoLh9UdUoE")
if retval39 != 0:
	print "test 39 error"
	sys.exit()


printHeader("Test 40 bitcry decryptElectrum")
out40 = os.popen("../bitcry decryptElectrum QklFMQMS87GCjxUlHIxDkABEKjKAq4Tl5+b2FyJBuvSqGHlh2/cmitu1Lh8oIppAK6zUhYFI93dK1ennRotPG4HxxBSa6cFViokjwXZKSZBFRZpLKU5r1pzLZCcm0oMmLeFdZY4= 5KjdzDrcyBfXDsRjwX5Uy5yZmSnKDPEWFujXP29LTiZpoJxWqeA")
result40 = out40.read()
print "Result: " + result40

if result40 != "Message: ThisIsThePlaintext\n":
	print "test 40 error: " + result40
	sys.exit()



printHeader("Test 41 bitcry encryptElectrum")
out41 = os.popen("../bitcry encryptElectrum ThisIsPlainInput 7UTZisACfzPr19WUEN24gN3Hng9kEGWgnMwv5AM3DGoLh9UdUoE")
result41 = out41.read()
partres41 = result41[201:215]
print "ResultOfEncryption: " + partres41
print "EndResultOfEncryption: "
shouldbe41 = "EncStr: QklFMQ"

if partres41 != shouldbe41:
	print "test 41 error: " + partres41
	print "should be    : " + shouldbe41
	
	sys.exit()


#print "Test 7"
#retval6 = os.system("../bitsig verifyRaw ThisIsAMessage HFC48Q8IpHYOH/LcNnKWl/oPx4iXu5V9c0KqfnsCwsmlGxafkXatjVoTc5hkDwm660cVLKimRzaRwFLuibyWnHw= 114MiwXTLxFHHDtvyBjbv9YxEV1QTwy8xf")
#if retval6 != 0:
#	print "test 7 error"
#	sys.exit()


os.system("rm *.txt *.ps *.armory")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


