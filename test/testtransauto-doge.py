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




printHeader("Test 1 bitgen-doge transaction")
retval1 = os.system("../bitgen-doge transaction 6374263746324623746237463276423746273642736427364273642764234233 0 QPa5wsVDhgc3mdMgorxji8kM7t1qh6xfG8eipZA97Rx8n9iHErgD 5000.0 out DU1ht6UXe5p6ygwd1z9r9NtVrDBPtNHPzB 4999.0 extra fixednounce")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("doge_C9EC09483058F42B1FE8CC5E492B3A6EC708CC0BD07AA8EA567A787A6F05D8A8.trans"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-doge transaction unknown input amount")
retval1 = os.system("../bitgen-doge transaction 6534534534534534534555545353453458345835834853845834853845834233 0 QPu3vZQeYBRMi8hvRA2nQTpTAV2ZjzBDKbuN4Jejw1AFDkDHRy5R 0 out DGLN6k8R4UoM2Yp5uMEsdz6vaHCVjdknJG 4.999 extra fixednounce")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("doge_E53206226D87D636FE149073F37F10E1317C040F3CF9E433ECFDF65330615485.trans"):
	print "test 2 file error"
	sys.exit()


printHeader("Test 3 bitgen-doge transaction unknown input amount")
retval1 = os.system("../bitgen-doge transaction 653453453453453453455554535345345834583583485384583485384583423 0 QPu3vZQeYBRMi8hvRA2nQTpTAV2ZjzBDKbuN4Jejw1AFDkDHRy5R 0 out DGLN6k8R4UoM2Yp5uMEsdz6vaHCVjdknJG 4.999 extra fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("doge_469FE4BAF9506663F26D6742F4AD2A60FC1BE06864AAD0BDFBB2FA5D927EEFAE.trans"):
	print "test 3 file error"
	sys.exit()


printHeader("Test 4 bitgen-doge transaction")
retval1 = os.system("../bitgen-doge transaction 1231231231723712371273617236176237126371623716237162376127361722 0 QPcA8UC35uCq79QCAdkiwTfy2bEN4UMctNW1yMEZhVkKjHwZpozC 1.00 out 7ZPumB4i62ULtorbi164G4qDU6Xr1FCkmsqPVjmkcv3QhRTBrAS 0.99 extra fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("doge_16611F5A3166412D94CAA50F07A2CE8A61D48517E961C4CF17F54594A37C132F.trans"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-doge transaction")
retval1 = os.system("../bitgen-doge transaction 9182918298129182928918982981928198219821982918291829182918291899 0 _QW2PyBwM1D2QoyUDv7TFSELHMgDbbEZQcKp9tgTr4u6eFcv6UJjH 2.01 out DTwptZQ5tGSUhA67YC49BnHJbkVcdMx3Ld 2.00 extra fixednounce")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("doge_91F337682DDDA419EF568AF04F92F4B4E072D7079DDC4B0AC65AC143D73D6D00.trans"):
	print "test 5 file error"
	sys.exit()



##printHeader("Test 6 bitgen-doge multisig")
##retval1 = os.system("../bitgen-doge multisig 1 2 7Yu7mTUygRznw3pwyJmknZM6fbtYcVzmaN1wKz7C9uxViMgHryK 7WJfsaXyNxaHbPjMzxbv8TZL26egAzrfxqgSH2Uq2sHNGKQwDsw")
##if retval1 != 0:
##	print "test 6 error"
##	sys.exit()
##
##
##if not os.path.isfile("multi_redeem_doge_9zABLaoHghCdhFunJDopS37nc5vdQSfjDd.script"):
##	print "test 6 file error"
##	sys.exit()
##
##
##printHeader("Test 7 bitgen-doge multisig")
##retval1 = os.system("../bitgen-doge transaction 7477474774747478484848488484848484848888348baaba77a777a777a77777 0 multi_redeem_doge_9zABLaoHghCdhFunJDopS37nc5vdQSfjDd.script 10.01 out DEzXscY4122sfUSF8zeJb4x9jxMXhnpxWS 10.00")
##if retval1 != 0:
##	print "test 7 error"
##	sys.exit()
##
##
##if not os.path.isfile("doge_nonsigned_29FC13FF757C30273AF3117D935C5415B9E9D785A04CAD9283A5B82E080F6B81.trans"):
##	print "test 7 file error"
##	sys.exit()
##
##
##printHeader("Test 8 bitgen-doge signmultisig")
##retval1 = os.system("../bitgen-doge signmultisig doge_nonsigned_29FC13FF757C30273AF3117D935C5415B9E9D785A04CAD9283A5B82E080F6B81.trans QUdXyjjwFC9VZD8m3BuHHK3b56gshqhuHRsHLsynscFx127shLxS fixednounce")
##if retval1 != 0:
##	print "test 8 error"
##	sys.exit()
##
##
##if not os.path.isfile("sig_doge_7Yu7mTUygRznw3pwyJmknZM6fbtYcVzmaN1wKz7C9uxViMgHryK.sig"):
##	print "test 8 file error"
##	sys.exit()
##
##
##printHeader("Test 9 bitgen-doge combinemultisig")
##retval1 = os.system("../bitgen-doge combinemultisig doge_nonsigned_29FC13FF757C30273AF3117D935C5415B9E9D785A04CAD9283A5B82E080F6B81.trans sig_doge_7Yu7mTUygRznw3pwyJmknZM6fbtYcVzmaN1wKz7C9uxViMgHryK.sig")
##if retval1 != 0:
##	print "test 9 error"
##	sys.exit()
##
##
##if not os.path.isfile("doge_1E8A78FEF6423180DC8429B184D7750BBAC25EC43EE20FFBBCF3531584D612F3.trans"):
##	print "test 9 file error"
##	sys.exit()
##
##



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
