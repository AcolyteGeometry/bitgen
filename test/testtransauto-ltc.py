#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-ltc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============




printHeader("Test 1 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 6374263746324623746237463276423746273642736427364273642764234233 0 T3JUdwMyRui7837c1MMfncLVpCDsRMywXM9F3DREPmtduTyfeNgx 5.000 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999 extra fixednounce")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("ltc_B7C499048EB17EB2B6D02E0877CD7CC22699DACBFFF746576665F8687F33561E.trans"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-ltc transaction unknown input amount")
retval1 = os.system("../bitgen-ltc transaction 6534534534534534534555545353453458345835834853845834853845834233 0 T3JUdwMyRui7837c1MMfncLVpCDsRMywXM9F3DREPmtduTyfeNgx 0 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999 extra fixednounce")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("ltc_FC5192686C83D9AFAB37D7C8459C4771DAD74FFC55084A0958697DF568A9FF06.trans"):
	print "test 2 file error"
	sys.exit()


printHeader("Test 3 bitgen-ltc transaction unknown input amount")
retval1 = os.system("../bitgen-ltc transaction 653453453453453453455554535345345834583583485384583485384583423 0 T3JUdwMyRui7837c1MMfncLVpCDsRMywXM9F3DREPmtduTyfeNgx 0 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999 extra fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("ltc_59C01FACA77654BDD570A303FAB35E2A0DFF8F36634AA4979776F58A49435B6C.trans"):
	print "test 3 file error"
	sys.exit()


printHeader("Test 4 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 1231231231723712371273617236176237126371623716237162376127361722 0 T7KBvpjZHZvVqpnncHUGB6ihG74garMdsKzbQ2zupUp4aeKWtWEF 1.00 out 7WtqybJbRy9JwE1ypqvBYxFTTUVomsZpyi9mmkHA1PRBddq9T5s 0.99 extra fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("ltc_64CF81AF568C6AD286B14C826D7EFE810253614FC0D126C8D6397D983CB755EE.trans"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 9182918298129182928918982981928198219821982918291829182918291899 0 _TAEbaikp6stXDmVwiyjrFzXYF4r835CGmv4eN7Kr1NLt8b3fZQmd 2.01 out LfkT7zLbcBw8JL6gYARqfjGxg3gkE27oQ2 2.00 extra fixednounce")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("ltc_016A401F34C83162B711193A0B9A157E92C14F61A2E51C206588F6F7B5FD2BD8.trans"):
	print "test 5 file error"
	sys.exit()



##printHeader("Test 6 bitgen-ltc multisig")
##retval1 = os.system("../bitgen-ltc multisig 1 2 7Zas9FTAeseDDEUUxNoqK1t6ukvmyYAtLhY1Q62yDZx63qT59jn 7WJfsaXyNxaHbPjMzxbv8TZL26egAzrfxqgSH2Uq2sHNGKQwDsw")
##if retval1 != 0:
##	print "test 6 error"
##	sys.exit()
##
##
##if not os.path.isfile("multi_redeem_ltc_3QAUnHocxmSWoSoNCP41voUHrMKnNXTLWM.script"):
##	print "test 6 file error"
##	sys.exit()
##
##
##printHeader("Test 7 bitgen-ltc multisig")
##retval1 = os.system("../bitgen-ltc transaction 7477474774747478484848488484848484848888348baaba77a777a777a77777 0 multi_redeem_ltc_3QAUnHocxmSWoSoNCP41voUHrMKnNXTLWM.script 10.01 out LNngwPqmBGZzoHwCNtdWeMn4LsEtoti1Tq 10.00")
##if retval1 != 0:
##	print "test 7 error"
##	sys.exit()
##
##
##if not os.path.isfile("ltc_nonsigned_0D053C779CE333FC72D1A2CA9875A04AE12FB2C199C42CB28D199794C2CAF944.trans"):
##	print "test 7 file error"
##	sys.exit()
##
##
##printHeader("Test 8 bitgen-ltc signmultisig")
##retval1 = os.system("../bitgen-ltc signmultisig ltc_nonsigned_0D053C779CE333FC72D1A2CA9875A04AE12FB2C199C42CB28D199794C2CAF944.trans T3JXw7JKGxKYUj9VkSm5ientU6tRQye8EPSasXKNAg9sxuF2yjqg fixednounce")
##if retval1 != 0:
##	print "test 8 error"
##	sys.exit()
##
##
##if not os.path.isfile("sig_ltc_7WJfsaXyNxaHbPjMzxbv8TZL26egAzrfxqgSH2Uq2sHNGKQwDsw.sig"):
##	print "test 8 file error"
##	sys.exit()
##
##
##printHeader("Test 9 bitgen-ltc combinemultisig")
##retval1 = os.system("../bitgen-ltc combinemultisig ltc_nonsigned_0D053C779CE333FC72D1A2CA9875A04AE12FB2C199C42CB28D199794C2CAF944.trans sig_ltc_7WJfsaXyNxaHbPjMzxbv8TZL26egAzrfxqgSH2Uq2sHNGKQwDsw.sig")
##if retval1 != 0:
##	print "test 9 error"
##	sys.exit()
##
##
##if not os.path.isfile("ltc_1377AE8D04571576623D519037D681A0AE7CF88BC3CA1B66AC16F797A7FBFC60.trans"):
##	print "test 9 file error"
##	sys.exit()
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

