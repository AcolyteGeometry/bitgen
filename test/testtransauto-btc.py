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




printHeader("Test 1 bitgen-btc transaction")
retval1 = os.system("../bitgen-btc transaction 6374263746324623746237463276423746273642736427364273642764234233 0 KxKuBVQ4pBdAyPauWDMrdG94EGyS8MXaYGxe61eZzQFg41YG4Duh 5000.0 out 17QfnvRhxHeP6sR79PaUju8DCBeBkNdah9 4999.0 extra fixednounce")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("btc_323E6AB74578CB66B662BB58EB24971DD431975527C5666559268CDE8F018B82.trans"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-btc transaction unknown input amount")
retval1 = os.system("../bitgen-btc transaction 6534534534534534534555545353453458345835834853845834853845834233 0 KxKuBVQ4pBdAyPauWDMrdG94EGyS8MXaYGxe61eZzQFg41YG4Duh 0 out 17QfnvRhxHeP6sR79PaUju8DCBeBkNdah9 4.999 extra fixednounce")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("btc_A7A189CEDF87FB6C2FC639E96489F50C4C6FE059A334379193D118B5FE1A7548.trans"):
	print "test 2 file error"
	sys.exit()


printHeader("Test 3 bitgen-btc transaction unknown input amount")
retval1 = os.system("../bitgen-btc transaction 653453453453453453455554535345345834583583485384583485384583423 0 KxKuBVQ4pBdAyPauWDMrdG94EGyS8MXaYGxe61eZzQFg41YG4Duh 0 out 17QfnvRhxHeP6sR79PaUju8DCBeBkNdah9 4.999 extra fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("btc_8A73C284053522460DB7397FFA30491D87A76BCFC989B56D801B08F5E06E6D07.trans"):
	print "test 3 file error"
	sys.exit()


printHeader("Test 4 bitgen-btc transaction")
retval1 = os.system("../bitgen-btc transaction 1231231231723712371273617236176237126371623716237162376127361722 0 KxKuBVQ4pBdAyPauWDMrdG94EGyS8MXaYGxe61eZzQFg41YG4Duh 1.00 out 7XL5X4YcEnzpdZ4Hczea3ppW1MiEcH277BiCpDzcX6Mc1JLk1yc 0.99 extra fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("btc_2FCD8BB22D18F8B8E73DDC387BA81654D3353147BAE09002695382E802CD3DF7.trans"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-btc transaction")
retval1 = os.system("../bitgen-btc transaction 9182918298129182928918982981928198219821982918291829182918291899 0 _KxKuBVQ4pBdAyPauWDMrdG94EGyS8MXaYGxe61eZzQFg41YG4Duh 2.01 out 17QfnvRhxHeP6sR79PaUju8DCBeBkNdah9 2.00 extra fixednounce")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("btc_A6812DAEA65ACF2443951E5ED49A864AD7C5814D11D5A4596B0E2F898B6C3E64.trans"):
	print "test 5 file error"
	sys.exit()



##printHeader("Test 6 bitgen-btc multisig")
##retval1 = os.system("../bitgen-btc multisig 1 2 7Xp8Yr2QDXxGRVZjuNked6vmKbzgwRRGmwDqsQ5ZMEmiUFABGV9 7WJfsaXyNxaHbPjMzxbv8TZL26egAzrfxqgSH2Uq2sHNGKQwDsw")
##if retval1 != 0:
##	print "test 6 error"
##	sys.exit()
##
##
##if not os.path.isfile("multi_redeem_btc_3K844tTWAbp7GoSkz7kRVDSTHFiLLLoeP4.script"):
##	print "test 6 file error"
##	sys.exit()
##
##
##printHeader("Test 7 bitgen-btc multisig")
##retval1 = os.system("../bitgen-btc transaction 7477474774747478484848488484848484848888348baaba77a777a777a77777 0 multi_redeem_btc_3K844tTWAbp7GoSkz7kRVDSTHFiLLLoeP4.script 10.01 out 1BaCqo219yLEY8fNajoCtU62Q4YcRDrA7W 10.00")
##if retval1 != 0:
##	print "test 7 error"
##	sys.exit()
##
##
##if not os.path.isfile("btc_nonsigned_8EA3B934F7D92269E1EB4E49D039D776CC16714EBE0FEE68E041DE6CB193A591.trans"):
##	print "test 7 file error"
##	sys.exit()
##
##
##printHeader("Test 8 bitgen-btc signmultisig")
##retval1 = os.system("../bitgen-btc signmultisig btc_nonsigned_8EA3B934F7D92269E1EB4E49D039D776CC16714EBE0FEE68E041DE6CB193A591.trans L1A7tqP6uo4Bikh1QzRv1pgShRxFkLafFn13rNonNJ6CjAQyB2AS fixednounce")
##if retval1 != 0:
##	print "test 8 error"
##	sys.exit()
##
##
##if not os.path.isfile("sig_btc_7Xp8Yr2QDXxGRVZjuNked6vmKbzgwRRGmwDqsQ5ZMEmiUFABGV9.sig"):
##	print "test 8 file error"
##	sys.exit()
##
##
##printHeader("Test 9 bitgen-btc combinemultisig")
##retval1 = os.system("../bitgen-btc combinemultisig btc_nonsigned_8EA3B934F7D92269E1EB4E49D039D776CC16714EBE0FEE68E041DE6CB193A591.trans sig_btc_7Xp8Yr2QDXxGRVZjuNked6vmKbzgwRRGmwDqsQ5ZMEmiUFABGV9.sig")
##if retval1 != 0:
##	print "test 9 error"
##	sys.exit()
##
##
##if not os.path.isfile("btc_0154F652C51C008D9E24F40529AAF52018D6652971FD708951247C16BF4C25DA.trans"):
##	print "test 9 file error"
##	sys.exit()
##
##
##
##printHeader("Test 10 bitgen-btc combinemultisig")
##retval1 = os.system("../bitgen signatureinfo 3044022056D49CBF002248CEA721C96001A615759553E48039061ED932757B813AA9D91D0220093FC8B60036A58D1A974664DA449CEA58A904D05A44D50064809F17556BB1D501")
##if retval1 != 0:
##	print "test 10 error"
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

