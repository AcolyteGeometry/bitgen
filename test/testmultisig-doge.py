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

printHeader("Test 1 bitgen-doge multisig")
retval1 = os.system("../bitgen-doge multisig 1 2 7We9wPk2Vwinx3yL6iWLoB77V2QKrUBEzRCxmaMUkTBaSJtAEFf 7ZvzRJaqnMvvNCwhmAVmvTFE9M5iyLcEBMrgszQmh1gDKjJruwD 7Zu7qkeGZ3rVMFHpTnWGBLNnWjC72uH6XAF2RRJkiDmXoyvVHSh")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("multi_redeem_doge_A4Xpx72zx9RnsTFm7jm8ECa1yFTFnuKcYs.script"):
	print "test 1 file error"
	sys.exit()




printHeader("Test 2 bitgen-doge transaction")
retval1 = os.system("../bitgen-doge transaction 1342342342342342342342342342342342343234243423333242342342342349 0 multi_redeem_doge_A4Xpx72zx9RnsTFm7jm8ECa1yFTFnuKcYs.script 10.000 out DPanFaaKz9dd5PKUGfotuFQndaQbQC8FvN 9.999")
if retval1 != 0:
	print "test 2 error"
	sys.exit()

if not os.path.isfile("doge_nonsigned_A278D0AC962C96670BE4E9A566FED79803BF7A5452A750F6221252B8BC654E6E.trans"):
	print "test 2 file error"
	sys.exit()




printHeader("Test 3 bitgen-doge signmultisig")
retval1 = os.system("../bitgen-doge signmultisig doge_nonsigned_A278D0AC962C96670BE4E9A566FED79803BF7A5452A750F6221252B8BC654E6E.trans QWbXhq9uDzDiyuKqw49LQHatYHC2qq5tadqkVHsj5zB9cmzSXoBQ fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("sig_doge_7ZvzRJaqnMvvNCwhmAVmvTFE9M5iyLcEBMrgszQmh1gDKjJruwD.sig"):
	print "test 3 file error"
	sys.exit()



printHeader("Test 5 bitgen-doge combinemultisig")
retval1 = os.system("../bitgen-doge combinemultisig doge_nonsigned_A278D0AC962C96670BE4E9A566FED79803BF7A5452A750F6221252B8BC654E6E.trans sig_doge_7ZvzRJaqnMvvNCwhmAVmvTFE9M5iyLcEBMrgszQmh1gDKjJruwD.sig")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("doge_C65F60FDF424CA69CC60348DFE2945C3E57EB13A75AA2D3965B044EA7265036F.trans"):
	print "test 5 file error"
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
