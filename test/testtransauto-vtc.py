#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-vtc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============




printHeader("Test 1 bitgen-vtc transaction")
retval1 = os.system("../bitgen-vtc transaction 6374263746324623746237463276423746273642736427364273642764234233 0 WZ6urxhWtuQCJtgs5nFnkHVdahVfnx9Cx72yScD4gUEYkSkkvErJ 5.000 out VpLqqrw2ArJYPpTmVsFfkRtr9KMexBStqq 4.999 extra fixednounce")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("vtc_04018EF573696564B59384DB9A183537521A2AB27DB20BCF7A8D816B7557D30E.trans"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-vtc transaction unknown input amount")
retval1 = os.system("../bitgen-vtc transaction 6534534534534534534555545353453458345835834853845834853845834233 0 WZ6urxhWtuQCJtgs5nFnkHVdahVfnx9Cx72yScD4gUEYkSkkvErJ 0 out VpLqqrw2ArJYPpTmVsFfkRtr9KMexBStqq 4.999 extra fixednounce")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("vtc_933A453498E9D1424649133FE55852837B004F28C7A817E50861869652248715.trans"):
	print "test 2 file error"
	sys.exit()


printHeader("Test 3 bitgen-vtc transaction unknown input amount")
retval1 = os.system("../bitgen-vtc transaction 653453453453453453455554535345345834583583485384583485384583423 0 WZ6urxhWtuQCJtgs5nFnkHVdahVfnx9Cx72yScD4gUEYkSkkvErJ 0 out VpLqqrw2ArJYPpTmVsFfkRtr9KMexBStqq 4.999 extra fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("vtc_F8C8851C3E84F0833C5C72012E55A35085752BD5E982F209F05BCE7265B409CF.trans"):
	print "test 3 file error"
	sys.exit()


printHeader("Test 4 bitgen-vtc transaction")
retval1 = os.system("../bitgen-vtc transaction 1231231231723712371273617236176237126371623716237162376127361722 0 WYzLadG7PioiaknjrKvwqPxpfLSSCnrUi2JZs51Tx1MMaPiCZPP2 1.00 out 7ZnTjgGZZWK9CeYcWtg6Jcbc9n1Y5BkWuG2aFaWFs6HwrFHpqjo 0.99 extra fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("vtc_B815FFA047B64478018CB5CC402856085FF39338ADA1A3620F6A1294ED3FE328.trans"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-vtc transaction")
retval1 = os.system("../bitgen-vtc transaction 9182918298129182928918982981928198219821982918291829182918291899 0 _WYzLadG7PioiaknjrKvwqPxpfLSSCnrUi2JZs51Tx1MMaPiCZPP2 2.01 out VpLqqrw2ArJYPpTmVsFfkRtr9KMexBStqq 2.00 extra fixednounce")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("vtc_FB2B7835D4FD2376F5DF4F09ABC0870B7C0EF8CE6CAAB87DB7FAB66C58DD4AA6.trans"):
	print "test 5 file error"
	sys.exit()



##printHeader("Test 6 bitgen-vtc multisig")
##retval1 = os.system("../bitgen-vtc multisig 1 2 7ZmEjVrrjKSXZHR2mGFy8jsDSr18wvWYgfkHd8NEQBcbirnsDH5 7WJfsaXyNxaHbPjMzxbv8TZL26egAzrfxqgSH2Uq2sHNGKQwDsw")
##if retval1 != 0:
##	print "test 6 error"
##	sys.exit()
##
##
##if not os.path.isfile("multi_redeem_vtc_33ctHSxkLTVpUQoQGaMVGxb5KUHxjKv4Yu.script"):
##	print "test 6 file error"
##	sys.exit()
##
##
##printHeader("Test 7 bitgen-vtc multisig")
##retval1 = os.system("../bitgen-vtc transaction 7477474774747478484848488484848484848888348baaba77a777a777a77777 0 multi_redeem_vtc_33ctHSxkLTVpUQoQGaMVGxb5KUHxjKv4Yu.script 10.01 out Vv1aaSHaUnVPm7pM8p75gA9qUSsTenS2xh 10.00")
##if retval1 != 0:
##	print "test 7 error"
##	sys.exit()
##
##
##if not os.path.isfile("vtc_nonsigned_930B0AC1E2A764FFF30F31567AAF59B79BE7E7D4AC9F8C17CDF86F54A7CDBCBF.trans"):
##	print "test 7 file error"
##	sys.exit()
##
##
##printHeader("Test 8 bitgen-vtc signmultisig")
##retval1 = os.system("../bitgen-vtc signmultisig vtc_nonsigned_930B0AC1E2A764FFF30F31567AAF59B79BE7E7D4AC9F8C17CDF86F54A7CDBCBF.trans WZK8t3dudoT2fSLP1rCZsjtxmvC1XqoVge9uiz4twZKaSmcLqdN8 fixednounce")
##if retval1 != 0:
##	print "test 8 error"
##	sys.exit()
##
##
##if not os.path.isfile("sig_vtc_7ZmEjVrrjKSXZHR2mGFy8jsDSr18wvWYgfkHd8NEQBcbirnsDH5.sig"):
##	print "test 8 file error"
##	sys.exit()
##
##
##printHeader("Test 9 bitgen-vtc combinemultisig")
##retval1 = os.system("../bitgen-vtc combinemultisig vtc_nonsigned_930B0AC1E2A764FFF30F31567AAF59B79BE7E7D4AC9F8C17CDF86F54A7CDBCBF.trans sig_vtc_7ZmEjVrrjKSXZHR2mGFy8jsDSr18wvWYgfkHd8NEQBcbirnsDH5.sig")
##if retval1 != 0:
##	print "test 9 error"
##	sys.exit()
##
##
##if not os.path.isfile("vtc_8F778190E0F5100179399355E703AA239517213095F3705FA2FB70A872994CEF.trans"):
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
