#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitsig"):
	print "Error, application binary do not exist"
	sys.exit()


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
retval3 = os.system("../bitsig verifyMultibit testfiles/DAC344C4.multibit")
if retval3 != 0:
	print "test 3 error"
	sys.exit()

printHeader("Test 4")
retval4 = os.system("../bitsig verifyArmory testfiles/8E528116.armory")
if retval4 != 0:
	print "test 4 error"
	sys.exit()


printHeader("Test 5")
retval5 = os.system("../bitsig verifyAb64 testfiles/1338BB38.armoryb64")
if retval5 != 0:
	print "test 5 error"
	sys.exit()


printHeader("Test 6")
retval6 = os.system("../bitsig verifyAclear testfiles/46C746F4.armoryclear")
if retval6 != 0:
	print "test 6 error"
	sys.exit()



printHeader("Test 7 InputsIo sign OK")
retval7 = os.system("../bitsig verifyInputsIo testfiles/signok.inputsio")
if retval7 != 0:
	print "test 7 error"
	sys.exit()

printHeader("Test 8 InputsIo sign error, signature changed")
retval8 = os.system("../bitsig verifyInputsIo testfiles/signerror.inputsio")
if retval8 == 0:
	print "test 8 error"
	sys.exit()

printHeader("Test 9 InputsIo sign error, message changed")
retval9 = os.system("../bitsig verifyInputsIo testfiles/signerrormessagechanged.inputsio")
if retval9 == 0:
	print "test 9 error"
	sys.exit()

printHeader("Test 10 InputsIo sign error, address changed")
retval10 = os.system("../bitsig verifyInputsIo testfiles/signerroraddresschanged.inputsio")
if retval10 == 0:
	print "test 10 error"
	sys.exit()


printHeader("Test 11 bitsig signArmory")
retval11 = os.system("../bitsig signArmory TheArMessage")
if retval11 != 0:
	print "test 11 error"
	sys.exit()

if not os.path.isfile("13CAC32E.armory"):
	print "test 11 error"
	sys.exit()



printHeader("Test 12 bitsig sign nonexisting file")
retval12 = os.system("../bitsig sign doesNotExist.txt")
if retval12 == 0:
	print "test 12 error"
	sys.exit()


printHeader("Test 13 bitsig signFile OK")
retval13 = os.system("../bitsig verifyFile testfiles/myLittleFile.txt.bitfsig")
if retval13 != 0:
	print "test 13 error"
	sys.exit()




printHeader("Test 14 bitsig delete err")
retval14 = os.system("../bitsig delete")
if retval14 == 0:
	print "test 14 error"
	sys.exit()


printHeader("Test 15 bitsig delete err 2")
retval15 = os.system("../bitsig delete sadasdasdasd")
if retval15 == 0:
	print "test 15 error"
	sys.exit()



printHeader("Test 16 bitsig delete err 3")
retval16 = os.system("../bitsig delete 1ED9WMSQuJgpiJkPBniy269APSXPMUYPEE 563CD92")
if retval16 == 0:
	print "test 16 error"
	sys.exit()



printHeader("Test 17 bitsig list")
retval17 = os.system("../bitsig list")
if retval17 != 0:
	print "test 17 error"
	sys.exit()


printHeader("Test 18 bitsig listOwn")
retval18 = os.system("../bitsig listOwn")
if retval18 != 0:
	print "test 18 error"
	sys.exit()



printHeader("Test 19 bitsig listOwn")
retval19 = os.system("../bitsig listPublic")
if retval19 != 0:
	print "test 19 error"
	sys.exit()


printHeader("Test 20 bitsig setDef")
retval20 = os.system("../bitsig setDef")
if retval20 == 0:
	print "test 20 error"
	sys.exit()


printHeader("Test 21 bitsig no arg")
retval21 = os.system("../bitsig")
if retval21 == 0:
	print "test 21 error"
	sys.exit()

printHeader("Test 22 bitsig verifyMultibit")
retval22 = os.system("../bitsig verifyMultibit testfiles/0324B408.multibit")
if retval22 != 0:
	print "test 22 error"
	sys.exit()



printHeader("Test 23 bitsig verifyMultibit")
retval23 = os.system("../bitsig signMultibit TheMultibitMess")
if retval23 != 0:
	print "test 23 error"
	sys.exit()

if not os.path.isfile("72D12F80.multibit"):
	print "test 13 error"
	sys.exit()



printHeader("Test 24 bitsig signArmory")
retval24 = os.system("../bitsig signArmory")
if retval24 == 0:
	print "test 24 error"
	sys.exit()


printHeader("Test 25 bitsig signArmory ok")
retval25 = os.system("../bitsig signArmory MyArmMess")
if retval25 != 0:
	print "test 25 error"
	sys.exit()

if not os.path.isfile("E9B12B46.armory"):
	print "test 25 error"
	sys.exit()


 
printHeader("Test 26 bitsig verifyArmory ok")
retval26 = os.system("../bitsig verifyArmory testfiles/545D8F28.armory")
if retval26 != 0:
	print "test 26 error"
	sys.exit()

 
 
printHeader("Test 27 bitsig verifyArmory err")
retval27 = os.system("../bitsig verifyArmory")
if retval27 == 0:
	print "test 27 error"
	sys.exit()



printHeader("Test 28 bitsig signAb64 err")
retval28 = os.system("../bitsig signAb64")
if retval28 == 0:
	print "test 28 error"
	sys.exit()



printHeader("Test 29 bitsig signAb64 ok")
retval29 = os.system("../bitsig signAb64 MyArmoryBase64Mess")
if retval29 != 0:
	print "test 29 error"
	sys.exit()

if not os.path.isfile("3DF8F9A7.armoryb64"):
	print "test 29 error"
	sys.exit()



printHeader("Test 30 bitsig verifyAb64 err")
retval30 = os.system("../bitsig verifyAb64")
if retval30 == 0:
	print "test 30 error"
	sys.exit()


 
printHeader("Test 31 bitsig verifyAb64 err")
retval31 = os.system("../bitsig verifyAb64 testfiles/3DF8F9A7.armoryb64")
if retval31 != 0:
	print "test 31 error"
	sys.exit()



printHeader("Test 32 bitsig verifyAb64 err")
retval32 = os.system("../bitsig verifyAb64 testfiles/modified.armoryb64")
if retval32 == 0:
	print "test 32 error"
	sys.exit()


printHeader("Test 33 bitsig signAclear err")
retval33 = os.system("../bitsig signAclear")
if retval33 == 0:
	print "test 33 error"
	sys.exit()


printHeader("Test 34 bitsig signAclear ok")
retval34 = os.system("../bitsig signAclear TheArmoryClearMessage")
if retval34 != 0:
	print "test 34 error"
	sys.exit()
	
if not os.path.isfile("3D6697E1.armoryclear"):
	print "test 34 file error"
	sys.exit()
	
	
	

printHeader("Test 35 bitsig verifyAclear err")
retval35 = os.system("../bitsig verifyAclear")
if retval35 == 0:
	print "test 35 error"
	sys.exit()



printHeader("Test 36 bitsig verifyAclear ok")
retval36 = os.system("../bitsig verifyAclear testfiles/20AC7253.armoryclear")
if retval36 != 0:
	print "test 36 error"
	sys.exit()


printHeader("Test 37 bitsig signInputsIo err")
retval37 = os.system("../bitsig signInputsIo")
if retval37 == 0:
	print "test 37 error"
	sys.exit()



printHeader("Test 38 bitsig signInputsIo ok")
retval38 = os.system("../bitsig signInputsIo MyInpIoMessage")
if retval38 != 0:
	print "test 38 error"
	sys.exit()

if not os.path.isfile("0806C67F.inputsio"):
	print "test 38 error"
	sys.exit()


 
printHeader("Test 39 bitsig verifyInputsIo ok")
retval39 = os.system("../bitsig verifyInputsIo testfiles/F5EA6D6A.inputsio")
if retval39 != 0:
	print "test 39 error"
	sys.exit()


printHeader("Test 40 bitsig verifyInputsIo err")
retval40 = os.system("../bitsig verifyInputsIo testfiles/changed.inputsio")
if retval40 == 0:
	print "test 40 error"
	sys.exit()


printHeader("Test 41 bitsig verifyMessage ok")
retval41 = os.system("../bitsig verifyMessage testfiles/F5EA6D6A.inputsio")
if retval41 != 0:
	print "test 41 error"
	sys.exit()


printHeader("Test 42 bitsig verifyMessage ok")
retval42 = os.system("../bitsig verifyMessage testfiles/20AC7253.armoryclear")
if retval42 != 0:
	print "test 42 error"
	sys.exit()



printHeader("Test 43 bitsig verifyMessage ok")
retval43 = os.system("../bitsig verifyMessage testfiles/3DF8F9A7.armoryb64")
if retval43 != 0:
	print "test 43 error"
	sys.exit()



printHeader("Test 44 bitsig verifyMessage ok")
retval44 = os.system("../bitsig verifyMessage testfiles/46C746F4.armoryclear")
if retval44 != 0:
	print "test 44 error"
	sys.exit()



printHeader("Test 45 bitsig verifyMessage ok")
retval45 = os.system("../bitsig verifyMessage testfiles/545D8F28.armory")
if retval45 != 0:
	print "test 45 error"
	sys.exit()


printHeader("Test 46 bitsig verifyMessage err")
retval46 = os.system("../bitsig verifyMessage")
if retval46 == 0:
	print "test 46 error"
	sys.exit()


printHeader("Test 47 bitsig verifyMessage ok")
retval47 = os.system("../bitsig verifyMessage testfiles/8E528116.armory")
if retval47 != 0:
	print "test 47 error"
	sys.exit()



printHeader("Test 48 bitsig verifyMessage ok")
retval48 = os.system("../bitsig verifyMessage testfiles/DAC344C4.multibit")
if retval48 != 0:
	print "test 48 error"
	sys.exit()



printHeader("Test 49 bitsig verifyMessage ok")
retval49 = os.system("../bitsig verifyMessage testfiles/1338BB38.armoryb64")
if retval49 != 0:
	print "test 49 error"
	sys.exit()



printHeader("Test 50 bitsig verifyMessage ok")
retval50 = os.system("../bitsig verifyMessage testfiles/modified.armoryb64")
if retval50 == 0:
	print "test 50 error"
	sys.exit()



#print "Test 7"
#retval6 = os.system("../bitsig verifyRaw ThisIsAMessage HFC48Q8IpHYOH/LcNnKWl/oPx4iXu5V9c0KqfnsCwsmlGxafkXatjVoTc5hkDwm660cVLKimRzaRwFLuibyWnHw= 114MiwXTLxFHHDtvyBjbv9YxEV1QTwy8xf")
#if retval6 != 0:
#	print "test 7 error"
#	sys.exit()


os.system("rm *.armory *.multibit *.bitsig *.bitfsig *.armoryb64 *.armoryclear *.inputsio")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


