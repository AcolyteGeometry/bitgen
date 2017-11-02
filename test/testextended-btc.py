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




printHeader("Test 1 bitgen-btc extended")
retval1 = os.system("../bitgen extended hash sdkjfhskhfskdjhfksjdhfksjdhfkjsdhfkjdshfkjsdhfkjshdfkjshdfkjhsdfkjhsdfkjhsdfjkhsdfkjhsdkjfhsdkjfhsdkjfhksjdhfksjdhfdfiuufdusdfsdfdsuyfsdufyysudfysdufysdiufysdiufysidufysidufysiudfy nops")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("extended_HuBkkEZybP5K6oNTrixd8hZ8zdk8p8G2sBjB66PCjzVWsqWxLBgUWo7r6u1BJ_private.txt"):
	print "test 1 file error"
	sys.exit()



printHeader("Test 2 bitgen-btc extended")
retval1 = os.system("../bitgen extended hash sdkjfhskhfskdjhfksjdhfksjdhfkjsdhfkjdshfkjsdhfkjshdfkjshdfkjhsdfkjhsdfkjhsdfjkhsdfkjhsdkjfhsdkjfhsdkjfhksjdhfksjdhfdfiuufdusdfsdfdsuyfsdufyysudfyyyueueuweurYIUYIUyuiyiuyioysdfsdf name ThisIsMyName nops")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("extended_6fYvv9NfMVYVDa4uNBPZWZ1s9H48nREuKDfDtXcJ5iyjwj3oBFCUNZaz8oVxw_private.txt"):
	print "test 2 file error"
	sys.exit()



printHeader("Test 3 bitgen-btc extended")
retval1 = os.system("../bitgen extended hex 182736182736ABCBEFABCDEF12323123213238746238746238746823746823764287364823746872364782342342342342342342342342342348899999999934")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("extended_jZmXJhXEsgJwkYUnekd9P68spZBQoxs9pdquBUASc9uy7SuGPncXAYHaRBwYX_private.txt"):
	print "test 3 file error"
	sys.exit()



printHeader("Test 4 bitgen-btc extended")
retval1 = os.system("../bitgen extended dice 21361263123651236125635126352165361253612536523645236456253615236153265123652163516235612536456235246523423423423412312312331231231236216321535235126351235126352165316253621536125361523651236512631231")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("extended_jZmXJhXEsgJwkYUnekd9P68spZBQoxs9pdquBUASc9uy7SuGPncXAYHaRBwYX_private.txt"):
	print "test 4 file error"
	sys.exit()





printHeader("Test 5 bitgen-btc extended")
retval1 = os.system("../bitgen extended brain MySalt KJkjshdfkjsdhfksjdfhksjdhfkjhdfsuidsfusdifusdifuisdufisdufisudfiusdifusdifudsifusdfiusdufudfusdfydsiufysiudyfisudyfisudyfiusydfiusdyfiusdyfiusdyfiusdyfiusydfiusydfiusydfsdfsdfsdfsdfsdjjjdsfjsjdfsdfeeee")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("extended_CPJqfL6FiBKjiGH5YZjbSsY242oFeCafaQcGwsq4uAdt8i4m2b87CH8wjWrNn_private.txt"):
	print "test 5 file error"
	sys.exit()



printHeader("Test 6 bitgen-btc extended")
retval1 = os.system("../bitgen extended infopub xpub661MyMwAqRbcEZwkLsG7L3XT3Z8tD36ZuabsViazoMptpg5gEdhMFoEEXHwt4kbpNkUSwQQMi1x2hXZp6vt9EctJ5VpLDtv1f41iE9Dxi54")
if retval1 != 0:
	print "test 6 error"
	sys.exit()



printHeader("Test 7 bitgen-btc extended")
retval1 = os.system("../bitgen extended infopriv xprv9s21ZrQH143K2L3CD8qn7m5H5DvP5v1Ef7AaN3aQCR1Ccig7273gTCaTpvwN4UYDU7Y48R2w9Sr4Zs6hfF64rsVxQuwh2LtTbQHYRoU6F3c")
if retval1 != 0:
	print "test 7 error"
	sys.exit()

printHeader("Test 8 bitgen-btc extended")
retval1 = os.system(".././bitgen extended infopriv xprv9s21ZrQH143K2nNEoHDUSJZtWJ2w3NtQGpcGGTWDwotyMUxdmqw8KKeKiJnCMLSn9q4SbmnneTEwJMjkXyAQWN5zg2uCMF48TCzcsqKMZb5")
if retval1 != 0:
	print "test 8 error"
	sys.exit()

printHeader("Test 9 bitgen-btc extended derivation")
retval1 = os.system(".././bitgen derivation xprv9s21ZrQH143K2nNEoHDUSJZtWJ2w3NtQGpcGGTWDwotyMUxdmqw8KKeKiJnCMLSn9q4SbmnneTEwJMjkXyAQWN5zg2uCMF48TCzcsqKMZb5 m/1")
if retval1 != 0:
	print "test 9 error"
	sys.exit()
	

printHeader("Test 10 bitgen-btc extended addressderivation")
retval1 = os.system("../bitgen addressderivation xprv9s21ZrQH143K2nNEoHDUSJZtWJ2w3NtQGpcGGTWDwotyMUxdmqw8KKeKiJnCMLSn9q4SbmnneTEwJMjkXyAQWN5zg2uCMF48TCzcsqKMZb5 m/0 10")
if retval1 != 0:
	print "test 10 error"
	sys.exit()

if not os.path.isfile("derddr_btc_0_10_4quqW9RxEHHnKPFNs7xoZZUskYZ9x1Du9STGrHsNLoG9sLy4jhPJyLadrPFPakhUaZKqV9w_private.txt"):
	print "test 10 file error"
	sys.exit()



printHeader("Test 11 bitgen-btc genMnemonic")
retval1 = os.system("../bitgen genMnemonic 24 hash asdadasdasdasdhhhhhhhhhasdkhasdkjahsdjkhasudyuasiduasiduisdufsodiufsdoifusodifusodifusodiufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoidufsoiudfsoidufsodiufsodiufsdoiufidsufysidufysiufysiudfyisudyfsiudfysiudfysidyfuisuduududuudisdfsoooososdifsdf8sd888d888dfssdoiufus")
if retval1 != 0:
	print "test 11 error"
	sys.exit()

if not os.path.isfile("mnemonic_24_84FC7BC89FE99E43B3BD1BF7AAEE9D715A9A9654143045EA4A78034E0AB435F8_private.txt"):
	print "test 11 file error"
	sys.exit()



printHeader("Test 12 bitgen-btc extened mnemonicc")
retval1 = os.system("../bitgen extended mnemonic \"build become hood juice already breeze gate chat horn raise admit orphan forest country party pass speak mountain prefer two please grit hurt blush\"")
if retval1 != 0:
	print "test 12 error"
	sys.exit()

if not os.path.isfile("extended_mmh2fs81wYnR86Eykt2KtSzKkFoxgfV6X74zipwwdqxQvTqgawpot7fgUQqq4_private.txt"):
	print "test 12 file error"
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

