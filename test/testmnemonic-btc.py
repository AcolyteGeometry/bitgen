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




printHeader("Test 11 bitgen-btc genMnemonic")
retval1 = os.system("../bitgen genMnemonic 24 hash asdadasdasdasdhhhhhhhhhasdkhasdkjahsdjkhasudyuasiduasiduisdufsodiufsdoifusodifusodifusodiufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoiudfsoidufsodiufsodiufsdoiufidsufysidufysiufysiudfyisudyfsiudfysiudfysidyfuisuduududuudisdfsoooososdifsdf8sd888d888dfssdoiufus")
if retval1 != 0:
	print "test 11 error"
	sys.exit()

if not os.path.isfile("mnemonic_24_84FC7BC89FE99E43B3BD1BF7AAEE9D715A9A9654143045EA4A78034E0AB435F8_private.txt"):
	print "test 11 file error"
	sys.exit()





printHeader("Test 13 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen genMnemonic 24 hash kjasdhksjdhfksjdhfskjdhfskdjhfskdjfhjsdjdsjfjjjfksfjskdjfksdjfksdjfksdjfkjjdjdjjdjfjsdfjsdfkjsdkfjsdkfjskdjfskjdfkdsjfksjfkjsdfkjsdkfjkjkjsdkfjksdjfksdjfksdjfksdjfjsdkfsdfsdfdsfsdjsdfhdjfhsjdhfsjhfsjdhfjsdhfjsdhfjsdhfjsdhfjshdfjshdfjshfjhsjdhfjsdhfjsdhfjsdhfsdf")
if retval1 != 0:
	print "test 13 error"
	sys.exit()

if not os.path.isfile("mnemonic_24_175E81AC077462FB0D910E99967DDD086D7BFFA1C8559A4EF655EF9C5E98CF8B_private.txt"):
	print "test 13 file error"
	sys.exit()



printHeader("Test 14 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen genMnemonic 12 hash ABCsdhksjdhfksjdhfskjdhfskdjhfskdjfhjsdjdsjfjjjfksfjskdjfksdjfksdjfksdjfkjjdjdjjdjfjsdfjsdfkjsdkfjsdkfjskdjfskjdfkdsjfksjfkjsdfkjsdkfjkjkjsdkfjksdjfksdjfksdjfksdjfjsdkfsdfsdfdsfsdjsdfhdjfhsjdhfsjhfsjdhfjsdhfjsdhfjsdhfjsdhfjshdfjshdfjshfjhsjdhfjsdhf")
if retval1 != 0:
	print "test 14 error"
	sys.exit()

if not os.path.isfile("mnemonic_12_8574C428C3832E8536988F747BC852042A93F774B65FA373653F36D908228978_private.txt"):
	print "test 14 file error"
	sys.exit()


printHeader("Test 15 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen genMnemonic 24 hash hhfdhkjsdhfkjshdfjhjdhfjkdshfuyudsifusoidfuiouwuuwuwuwuwueiouuvuvuvuxoicuvxoicuvxoicuvoiuxvcoivuxcoivuxoicuvdosiufsoidufsdllsdfsdfkkskdfjsdfhsjkdfsdfsiiuUUUusidfusdoiufsoidufoisudfoisfdoisuoiudsfoiuiouioiuoiuqiouqoiuqoiuqoqoiqoiuquioqoiuiuoqoiuqiouqouiqiouqoiuieueiruiedfsdfsdfkkfkkfkfk")
if retval1 != 0:
	print "test 15 error"
	sys.exit()

if not os.path.isfile("mnemonic_24_E1EFD8450C2426302FAE37E5F973D5942A0256543B7AB10D587C349069838F64_private.txt"):
	print "test 15 file error"
	sys.exit()



printHeader("Test 16 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen genMnemonic 12 hex 234823764876ABBBCFFC242342342342")
if retval1 != 0:
	print "test 16 error"
	sys.exit()

if not os.path.isfile("mnemonic_12_5C89A5250012017CDE20B76D08A979F7B02964E857D876E3F82F275600B61010_private.ps"):
	print "test 16 file error"
	sys.exit()




printHeader("Test 17 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen genMnemonic 12 hash hfhfhJJJSDJFUIUSIDUisudfoisudfydsyuwjmmjjwjjsdfkjhkjhsdfHHSJDFyUIUyiuyIuyiusdfsdfsdfsdkfjhskdfhsjfhhhhdhdhhdhdhhdhhwhwhwhhwhwjwjwhwjwjduifsdfsjkdfhsdfuhsdkjfhakajdhjashdjashdfrrreeeegfgfr name \"My test name\"")
if retval1 != 0:
	print "test 17 error"
	sys.exit()

if not os.path.isfile("mnemonic_12_07CFE2A5532B9EEDA4780DEEFA8116500242DC36EEDC3287BA2A796C3C894440_private.txt"):
	print "test 17 file error"
	sys.exit()



printHeader("Test 18 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen genMnemonic 12 dice 61253615236215365126351263561253612361251616166612 name \"Dice test\"")
if retval1 != 0:
	print "test 18 error"
	sys.exit()

if not os.path.isfile("mnemonic_12_0A33C7DB51CC7080A0CBFAA2DD9B192C896EE1EF267E55D2B4156F725E9F5C94_private.txt"):
	print "test 18 file error"
	sys.exit()




printHeader("Test 19 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen genMnemonic 18 hash asdaksjdhaksjdhaksjhdkjashdkjadhfkjdshdskjfhsdkjhfskjdhfkjsdhfkjsdhfkjsdhfkjshdfkjsdhfkjshdfkjshdfkjhdsfkjsdhfkjhsdfkjhsdfkjhasjkhdajkhdkjashdkjahsdkjhasdkjhasdkjhjkfhjjjdjjdjdhfhfhfhasdhfsuydfsiudyfsiudyfsiudyfsiudyfasuidyasksajkjsjsjdhasjkdhasjkdhjjjjJJjshdfjsdfkjhdsfsdfsdf")
if retval1 != 0:
	print "test 19 error"
	sys.exit()

if not os.path.isfile("mnemonic_18_34AD622A68D4A559697502D0C6B54723CE768888DE22F2F86EEEA18C92130107_private.txt"):
	print "test 19 file error"
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

