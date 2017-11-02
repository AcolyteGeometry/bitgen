#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen"):
	print "Error, application binary does not exist"
	sys.exit()


if os.path.isfile("12xKq6VN8wfNoHwaraJiYG5cGNP2dospEu_share.txt"):
	os.remove("12xKq6VN8wfNoHwaraJiYG5cGNP2dospEu_share.txt")


if os.path.isfile("16uoLAEFbHw3a8rw8dg4wWbhMyaBWzEC9C_share.txt"):
	os.remove("16uoLAEFbHw3a8rw8dg4wWbhMyaBWzEC9C_share.txt")


#============ Start of tests ============




printHeader("Test 1 bitgen infoPrivate")
retval1 = os.system("../bitgen infoPrivate 5JzsaTDecYGDhZv6u2ndW5uqhKY81r8cq3z4v2HJmQ31gh6jsEt")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

#printHeader("Test 2 bitgen infoPrivate key error")
#retval2 = os.system("../bitgen infoPrivate 5JzsaTDecYGDhZv6u2PdW5uqhKY81r8cq3z4v2HJmQ31gh6jsEt")
#if retval2 == 0:
#	print "test 2 error"
#	sys.exit()


printHeader("Test 3 bitgen dice incorrect num dice")
retval3 = os.system("../bitgen dice 23424356152163562536253615236215356253612536125361523123123 nops")
if retval3 == 0:
	print "test 3 error"
	sys.exit()


printHeader("Test 4 bitgen dice ok")
retval4 = os.system("../bitgen dice 2342435615216356253625361523621535625361253612536152312312323423423423432423423423423423423423423423 nops")
if retval4 != 0:
	print "test 4 error"
	sys.exit()

if not os.path.isfile("1H4zEXvcPK4R2jC4HDHQYM3qDbhemfSPYp_share.txt"):
	print "test 4 file error"
	sys.exit()


printHeader("Test 5 bitgen dice illegal char")
retval5 = os.system("../bitgen dice 2342435615216356253625361523621535625361253612536152312312323423423423432423GGG423423423423423423423")
if retval5 == 0:
	print "test 5 error"
	sys.exit()

printHeader("Test 6 bitgen hex OK")
retval6 = os.system("../bitgen hex 4654CCCABBCBDEF543543543543123123123BBCBBBBCBBBBBBCB123123123123")
if retval6 != 0:
	print "test 6 error"
	sys.exit()

if not os.path.isfile("1DxBqY5F4nAns6Ck1ASNobeQGc2PmY1BTd_share.txt"):
	print "test 6 file error"
	sys.exit()



printHeader("Test 7 bitgen hex error")
retval7 = os.system("../bitgen hex 4654CCCABBCBDEF543543543543123123123BBCBBBBCBBBBBBCB12312312")
if retval7 == 0:
	print "test 7 error"
	sys.exit()


printHeader("Test 8 bitgen hex digit out of range")
retval8 = os.system("../bitgen hex 4654CCCABBCBDEF543543543543123123123PPPBBBBCBBBBBBCB123123123123")
if retval8 == 0:
	print "test 8 error"
	sys.exit()


printHeader("Test 9 bitgen mnemonic")
retval9 = os.system("../bitgen mnemonic \"fame bind prevent question trap proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval9 != 0:
	print "test 9 error"
	sys.exit()
	
if not os.path.isfile("16SYJFzUBq4jVnVggVLYaCgetMdh2SeL5Z_share.txt"):
	print "test 9 file error"
	sys.exit()

printHeader("Test 10 bitgen mnemonic error")
retval10 = os.system("../bitgen mnemonic \"fame bind prevent trap question proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval10 == 0:
	print "test 10 error"
	sys.exit()

printHeader("Test 11 bitgen mnemonic word error")
retval11 = os.system("../bitgen mnemonic \"fame bind prevent notaword question proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval11 == 0:
	print "test 11 error"
	sys.exit()

printHeader("Test 12 bitgen brainwallet ok")
retval12 = os.system("../bitgen brainwallet tester@test.org \"This is an insecure brainwallet that should only be used for testing and nothing else\"")
if retval12 != 0:
	print "test 12 error"
	sys.exit()

if not os.path.isfile("13wwFKPRvPYMsE2mV7yYFtyVeobV7dxgxX_share.txt"):
	print "test 12 file error"
	sys.exit()


printHeader("Test 13 bitgen pubAddr")
retval13 = os.system("../bitgen pubAddr 1K6HFD9o9uMnyQSkoLH48Q5nnj3vCJCi8j")
if retval13 != 0:
	print "test 13 error"
	sys.exit()

#printHeader("Test 14 bitgen pubAddr error")
#retval14 = os.system("../bitgen pubAddr 1K6HFD9o9uMMyQSkoLH48Q5nnj3vCJCi8j")
#if retval14 == 0:
#	print "test 14 error"
#	sys.exit()



printHeader("Test 15 bitgen miniToWif")
retval15 = os.system("../bitgen miniToWif")
if retval15 == 0:
	print "test 15 error"
	sys.exit()



printHeader("Test 16 bitgen miniToWif")
retval16 = os.system("../bitgen miniToWif Sp2oPDr42nAHK1RKBbtbymWcX8uUxH")
if retval16 != 0:
	print "test 16 error"
	sys.exit()

if not os.path.isfile("14WK8vmmR1ayhiMKxF8Et77AqobMYPc29h_share.txt"):
	print "test 16 file error"
	sys.exit()



printHeader("Test 17 bitgen info")
retval17 = os.system("../bitgen info 5J4MYXyqq699uv1Vdvb17sx8iHDUGcTTTTTYhx5citCTp5Zn7gy")
if retval17 == 0:
	print "test 17 error"
	sys.exit()

 

#printHeader("Test 18 bitgen info")
#retval18 = os.system("../bitgen infoPrivate werwrwer")
#if retval18 == 0:
#	print "test 18 error"
#	sys.exit()




printHeader("Test 19 bitgen info")
retval19 = os.system("../bitgen infoPrivate 5KX2CQtSBAAmHkfhD8T5cwrELFAF11dao3UiX7VDYncdAC3K5FX")
if retval19 != 0:
	print "test 19 error"
	sys.exit()

printHeader("Test 20 bitgen info")
retval20 = os.system("../bitgen info 5KX2CQtSBAAmHkfhD8T5cwrELFAF11dao3UiX7VDYncdAC3K5FX")
if retval20 != 0:
	print "test 20 error"
	sys.exit()


printHeader("Test 21 bitgen genRoot")
retval21 = os.system("../bitgen genRoot random")
if retval21 != 0:
	print "test 21 error"
	sys.exit()



printHeader("Test 22 bitgen genPub")
retval22 = os.system("../bitgen genPub testfiles/root_6F52FB10_pub.pubroot")
if retval22 != 0:
	print "test 22 error"
	sys.exit()


printHeader("Test 23 bitgen genPriv")
retval23 = os.system("../bitgen genPriv testfiles/root_6F52FB10_priv.privroot testfiles/17hdJkQsAVquNpkLhkQxMMjx4HHqF1524G.privindex")
if retval23 != 0:
	print "test 23 error"
	sys.exit()



printHeader("Test 24 bitgen genPriv")
retval24 = os.system("../bitgen genPub testfiles/root_BDB6C45A_pub.pubroot")
if retval24 != 0:
	print "test 24 error"
	sys.exit()



printHeader("Test 25 bitgen genPub no arg")
retval25 = os.system("../bitgen genPub")
if retval25 == 0:
	print "test 25 error"
	sys.exit()




printHeader("Test 26 bitgen genPriv no arg")
retval26 = os.system("../bitgen genPriv")
if retval26 == 0:
	print "test 26 error"
	sys.exit()



printHeader("Test 27 bitgen genPriv wrong arg")
retval27 = os.system("../bitgen genPriv DoesNotExist.privroot DoesNotExist.privindex")
if retval27 == 0:
	print "test 27 error"
	sys.exit()


printHeader("Test 28 bitgen genPriv")
retval28 = os.system("../bitgen genPriv testfiles/root_59AF4E40_priv.privroot testfiles/1N8vgFdM5Pt1zZDDRsywBZtf1ooif7HW2E.privindex")
if retval28 != 0:
	print "test 28 error"
	sys.exit()
	
if not os.path.isfile("1MczuwfXbiw3X4RovaPKWufExRxaj4Y7fz_share.txt"):
	print "test 28 file error"
	sys.exit()


printHeader("Test 29 bitgen genPriv")
retval29 = os.system("../bitgen genPriv testfiles/root_7F51B56B_priv.privroot testfiles/1Pnvq2JqP59TBjyxbcLhfAwxpKDzSL3M6J.privindex")
if retval29 != 0:
	print "test 29 error"
	sys.exit()
	
if not os.path.isfile("16EKDByyj2tb3GcLuBMKNouCupZfwa1MFP_share.txt"):
	print "test 29 file error"
	sys.exit()



printHeader("Test 30 info")
retval30 = os.system("../bitgen info 7UJVgMbSVu44Lcfgxi6w6PXieReFofhiEoDbskVbuH76QoXaJ32")
if retval30 != 0:
	print "test 30 error"
	sys.exit()


printHeader("Test 31 info")
retval31 = os.system("../bitgen info 5JUCibikWRh7R1NwkvcUiS4kH46M1LvKmSP7MVqViQemBQz7Ajf")
if retval31 != 0:
	print "test 31 error"
	sys.exit()


printHeader("Test 32 bitgen help")
retval32 = os.system("../bitgen help")
if retval32 != 0:
	print "test 32 error"
	sys.exit()

printHeader("Test 33 bitgen help")
retval33 = os.system("../bitgen -v")
if retval33 != 0:
	print "test 33 error"
	sys.exit()


print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.txt *.ps *.privroot *.pubroot *.privindex")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


