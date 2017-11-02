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
retval1 = os.system("../bitgen-doge info 6Kjy185frYFaiJmRG3f9zAzu7kwQiCG25NvCgD19vRZrRw16fHx")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

printHeader("Test 2 bitgen-doge infoPrivate key error")
retval2 = os.system("../bitgen-doge info 6vWbfAxaw7nYaDUH6GmAsUo3Y1F47rbteBdewpxUKsuZT1KfQQQ")
if retval2 == 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-doge dice incorrect num dice")
retval3 = os.system("../bitgen-doge dice 23424356152163562536253615236215356253612536125361523123123 nops")
if retval3 == 0:
	print "test 3 error"
	sys.exit()


printHeader("Test 4 bitgen-doge dice ok")
retval4 = os.system("../bitgen-doge dice 2342435615216356253625361523621535625361253612536152312312323423423423432423423423423423423423423423 nops")
if retval4 != 0:
	print "test 4 error"
	sys.exit()

if not os.path.isfile("DMD5mnsFgixhZjNf1oGy67DS6jRxAKgpjb_share.txt"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-doge dice illegal char")
retval5 = os.system("../bitgen-doge dice 2342435615216356253625361523621535625361253612536152312312323423423423432423GGG423423423423423423423")
if retval5 == 0:
	print "test 5 error"
	sys.exit()

printHeader("Test 6 bitgen-doge hex OK")
retval6 = os.system("../bitgen-doge hex 4654CCCABBCBDEF543543543543123123123BBCBBBBCBBBBBBCB123123123123")
if retval6 != 0:
	print "test 6 error"
	sys.exit()

if not os.path.isfile("DJ6HNo1tNC55Q6PLjkRwMMp19jkhB73uV7_share.txt"):
	print "test 6 file error"
	sys.exit()



printHeader("Test 7 bitgen-doge hex error")
retval7 = os.system("../bitgen-doge hex 4654CCCABBCBDEF543543543543123123123BBCBBBBCBBBBBBCB12312312")
if retval7 == 0:
	print "test 7 error"
	sys.exit()


printHeader("Test 8 bitgen-doge hex digit out of range")
retval8 = os.system("../bitgen-doge hex 4654CCCABBCBDEF543543543543123123123PPPBBBBCBBBBBBCB123123123123")
if retval8 == 0:
	print "test 8 error"
	sys.exit()


printHeader("Test 9 bitgen-doge mnemonic")
retval9 = os.system("../bitgen-doge mnemonic \"fame bind prevent question trap proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval9 != 0:
	print "test 9 error"
	sys.exit()
	
if not os.path.isfile("DAadqWw7VEy22ngHR5L77xrFmVMzSRJHSd_share.txt"):
	print "test 9 file error"
	sys.exit()



printHeader("Test 10 bitgen-doge mnemonic error")
retval10 = os.system("../bitgen-doge mnemonic \"fame bind prevent trap question proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval10 == 0:
	print "test 10 error"
	sys.exit()

printHeader("Test 11 bitgen-doge mnemonic word error")
retval11 = os.system("../bitgen-doge mnemonic \"fame bind prevent notaword question proof fringe loan foam couple safe oval father foot this execute estate shoe alcohol away proof gesture blue window\"")
if retval11 == 0:
	print "test 11 error"
	sys.exit()

printHeader("Test 12 bitgen-doge brainwallet ok")
retval12 = os.system("../bitgen-doge brainwallet tester@test.org \"This is an insecure brainwallet that should only be used for testing and nothing else\"")
if retval12 != 0:
	print "test 12 error"
	sys.exit()

if not os.path.isfile("D862naL5DoSeQEDNDhy6of96XwKnSmxkyf_share.txt"):
	print "test 12 file error"
	sys.exit()




printHeader("Test 13 bitgen-doge info")
retval13 = os.system("../bitgen-doge info DEpZSWmHgp5kiLQ5LDPdEFdXaHRBUdX3kc")
if retval13 != 0:
	print "test 13 error"
	sys.exit()


printHeader("Test 14 bitgen-doge info error")
retval14 = os.system("../bitgen-doge info 1K6HFD9o9uMMyQSkoLH48Q5nnj3vCJCi8j")
if retval14 == 0:
	print "test 14 error"
	sys.exit()





printHeader("Test 15 bitgen-doge info")
retval15 = os.system("../bitgen-doge info 5J4MYXyqq699uv1Vdvb17sx8iHDUGcTTTTTYhx5citCTp5Zn7gy")
if retval15 == 0:
	print "test 15 error"
	sys.exit()

 

printHeader("Test 16 bitgen-doge info")
retval16 = os.system("../bitgen-doge info werwrwer")
if retval16 == 0:
	print "test 16 error"
	sys.exit()




printHeader("Test 17 bitgen-doge info")
retval17 = os.system("../bitgen-doge infoPrivate 5KX2CQtSBAAmHkfhD8T5cwrELFAF11dao3UiX7VDYncdAC3K5FX")
if retval17 != 0:
	print "test 17 error"
	sys.exit()


printHeader("Test 18 bitgen-doge info")
retval18 = os.system("../bitgen-doge info 6JACymFHkTdcEUCZJ4t7ovyLCZBNkTR5S4qcRqHtYuXecLdehQj")
if retval18 != 0:
	print "test 18 error"
	sys.exit()


printHeader("Test 19 bitgen-doge genRoot")
retval19 = os.system("../bitgen-doge genRoot random")
if retval19 != 0:
	print "test 19 error"
	sys.exit()



printHeader("Test 20 bitgen-doge genPub")
retval20 = os.system("../bitgen-doge genPub testfiles/root_6F52FB10_pub.pubroot")
if retval20 != 0:
	print "test 20 error"
	sys.exit()


printHeader("Test 21 bitgen-doge genPriv")
retval21 = os.system("../bitgen-doge genPriv testfiles/root_6F52FB10_priv.privroot testfiles/17hdJkQsAVquNpkLhkQxMMjx4HHqF1524G.privindex")
if retval21 != 0:
	print "test 21 error"
	sys.exit()



printHeader("Test 22 bitgen-doge genPriv")
retval22 = os.system("../bitgen-doge genPub testfiles/root_BDB6C45A_pub.pubroot")
if retval22 != 0:
	print "test 22 error"
	sys.exit()





printHeader("Test 30 info")
retval30 = os.system("../bitgen-doge info 7UJVgMbSVu44Lcfgxi6w6PXieReFofhiEoDbskVbuH76QoXaJ32")
if retval30 != 0:
	print "test 30 error"
	sys.exit()


printHeader("Test 31 info")
retval31 = os.system("../bitgen-doge info 6KLdonzPVsmPSn51eWY91i3ZwEmiuU7ybffeC4sR9D6isXybpps")
if retval31 != 0:
	print "test 31 error"
	sys.exit()


printHeader("Test 32 bitgen-doge help")
retval32 = os.system("../bitgen-doge help")
if retval32 != 0:
	print "test 32 error"
	sys.exit()

printHeader("Test 33 bitgen-doge help")
retval33 = os.system("../bitgen-doge -v")
if retval33 != 0:
	print "test 33 error"
	sys.exit()


printHeader("Test 34 bitgen-doge info error")
retval34 = os.system("../bitgen-doge info 18ZicQ72Yhcp55U9DyAuGdJXyThcyPfv46")
if retval34 == 0:
	print "test 34 error"
	sys.exit()


printHeader("Test 35 bitgen-doge random compressed")
retval35 = os.system("../bitgen-doge random compressed")
if retval35 != 0:
	print "test 35 error"
	sys.exit()



printHeader("Test 36 bitgen-doge dice compressed")
retval36 = os.system("../bitgen-doge dice 1231231231232123123123123432342342342342342342342342341223423423423423423423423423423431231232341333 compressed")
if retval36 != 0:
	print "test 36 error"
	sys.exit()

if not os.path.isfile("D5EgtqfrPHu2sCNaTomJCXVAZM4rH2P5oG_share.txt"):
	print "test 36 file error"
	sys.exit()



printHeader("Test 37 bitgen-doge dice compressed")
retval37 = os.system("../bitgen-doge dice 6262626262525235234234234234234234234234424663464363646236462346236462346263462346236462342323423423 name \"Testing this\"")
if retval37 != 0:
	print "test 37 error"
	sys.exit()

if not os.path.isfile("DKzMjhDX4ZKL1RNX1Xkt5pVgAY1UcVjviM_share.txt"):
	print "test 37 file error"
	sys.exit()



printHeader("Test 38 bitgen-doge dice compressed")
retval38 = os.system("../bitgen-doge hash lsdkfjlsdkjflksdjfklsdjflksdjfjdsjjsfdsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdfsfsdfsdfdsfsdfkjsdhfjksdhfkjsdfsdfkjsdflkjsdlkfjsdlkfjsdlkjfsldkjflksdjflkdsjflkdsjflkdsjflksdjflskdjflksdjlkjjdjjdjdjfdjfksjdflkjdsflkjsdlkfjsdlkfjsdlkjflksdjflkdsjkldkkdkdkfsjdkfjsldfjlsdkjf")
if retval38 != 0:
	print "test 38 error"
	sys.exit()

if not os.path.isfile("DAVUFCH2cQuvPaQnH97BbPpw7MUGUQcaBm_share.txt"):
	print "test 38 file error"
	sys.exit()



printHeader("Test 39")
retval39 = os.system("../bitgen-doge info 1AM3de4fV52yrjJd5ByegK7cRpmKDezZj1")
if retval39 == 0:
	print "test 39 error"
	sys.exit()


printHeader("Test 40")
retval40 = os.system("../bitgen-doge info LaUedzmKKKoBTQiKSRRTnJZVF3S6vWVoLJ")
if retval40 == 0:
	print "test 40 error"
	sys.exit()



printHeader("Test 41")
retval41 = os.system("../bitgen-doge info 6L3ijYQkJTWV2wmz2NinXsTxBeb4FABnE646LA4kayMKJN6WBmc")
if retval41 != 0:
	print "test 41 error"
	sys.exit()


printHeader("Test 42")
retval42 = os.system("../bitgen-doge hex 123123123123b123b123b12b31b23b123b12b3b123b12b31b23b123b123b1b23")
if retval42 != 0:
	print "test 42 error"
	sys.exit()

if not os.path.isfile("DAyJuaqk2AmQbYtNvDBpshRspKApVaViaU_share.txt"):
	print "test 42 file error"
	sys.exit()


#printHeader("Test 25 bitgen-doge genPub no arg")
#retval25 = os.system("../bitgen-doge genPub")
#if retval25 == 0:
#	print "test 25 error"
#	sys.exit()



#printHeader("Test 26 bitgen-doge genPriv no arg")
#retval26 = os.system("../bitgen-doge genPriv")
#if retval26 == 0:
#	print "test 26 error"
#	sys.exit()



#printHeader("Test 27 bitgen-doge genPriv wrong arg")
#retval27 = os.system("../bitgen-doge genPriv DoesNotExist.privroot DoesNotExist.privindex")
#if retval27 == 0:
#	print "test 27 error"
#	sys.exit()


#printHeader("Test 28 bitgen-doge genPriv")
#retval28 = os.system("../bitgen-doge genPriv testfiles/root_59AF4E40_priv.privroot testfiles/1N8vgFdM5Pt1zZDDRsywBZtf1ooif7HW2E.privindex")
#if retval28 != 0:
#	print "test 28 error"
#	sys.exit()
	
#if not os.path.isfile("1N8vgFdM5Pt1zZDDRsywBZtf1ooif7HW2E_share.txt"):
#	print "test 28 file error"
#	sys.exit()


#printHeader("Test 29 bitgen-doge genPriv")
#retval29 = os.system("../bitgen-doge genPriv testfiles/root_7F51B56B_priv.privroot testfiles/1Pnvq2JqP59TBjyxbcLhfAwxpKDzSL3M6J.privindex")
#if retval29 != 0:
#	print "test 29 error"
#	sys.exit()
#	
#if not os.path.isfile("1Pnvq2JqP59TBjyxbcLhfAwxpKDzSL3M6J_share.txt"):
#	print "test 29 file error"
#	sys.exit()



print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.txt *.ps")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


