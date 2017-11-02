#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-nmc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============




printHeader("Test 1 bitgen-nmc transaction")
retval1 = os.system("../bitgen-nmc transaction 6374263746324623746237463276423746273642736427364273642764234233 0 L1k2Gr6tkgVieJdJuvriCVC22pL15aDm38vVTjkqhhG1eDrLLLJS 5.000 out NJU8aWDJoxaamDhPxCFsRZrHKqkRmscTtW 4.999 extra fixednounce")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("nmc_A0980AA70AE11C8FF4ACDD954D4202AEBF928A58AFF92D3ECBC019DF34D7FE9C.trans"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-nmc transaction unknown input amount")
retval1 = os.system("../bitgen-nmc transaction 6534534534534534534555545353453458345835834853845834853845834233 0 L1k2Gr6tkgVieJdJuvriCVC22pL15aDm38vVTjkqhhG1eDrLLLJS 0 out NJU8aWDJoxaamDhPxCFsRZrHKqkRmscTtW 4.999 extra fixednounce")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("nmc_87C4EF39DFA5AC9C10DB84F2970D062CE786B6C669DB0DBE5CE85A9103C721D8.trans"):
	print "test 2 file error"
	sys.exit()


printHeader("Test 3 bitgen-nmc transaction unknown input amount")
retval1 = os.system("../bitgen-nmc transaction 653453453453453453455554535345345834583583485384583485384583423 0 L1k2Gr6tkgVieJdJuvriCVC22pL15aDm38vVTjkqhhG1eDrLLLJS 0 out NJU8aWDJoxaamDhPxCFsRZrHKqkRmscTtW 4.999 extra fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("nmc_E095BAFF661ED78369CF411CEA613AF1A0F4F0211C3F6E65A347CC7EEF8060EB.trans"):
	print "test 3 file error"
	sys.exit()


printHeader("Test 4 bitgen-nmc transaction")
retval1 = os.system("../bitgen-nmc transaction 1231231231723712371273617236176237126371623716237162376127361722 0 KyHmkeb61bNd5mxZLBfBX8cgHMYuteWvS5s76uNGT4RJUQwx2wA3 1.00 out 7YWD3QoRk1AABRUkqUqmpw7khWkdEjcUU4tNhBr8RppyyabJZTb 0.99 extra fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("nmc_23A44A4DE3579C3368E0160515900E4E3F8F027D5F3B76422AA7E18525ECA8FE.trans"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-nmc transaction")
retval1 = os.system("../bitgen-nmc transaction 9182918298129182928918982981928198219821982918291829182918291899 0 _KyHmkeb61bNd5mxZLBfBX8cgHMYuteWvS5s76uNGT4RJUQwx2wA3 2.01 out NJU8aWDJoxaamDhPxCFsRZrHKqkRmscTtW 2.00 extra fixednounce")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("nmc_C65B1371E298000873D591F56AA26686F80A7D93899BAD7F0685E0F1D0B82C6B.trans"):
	print "test 5 file error"
	sys.exit()



##
##printHeader("Test 6 bitgen-nmc multisig")
##retval1 = os.system("../bitgen-nmc multisig 1 2 7ZmAjGWYJxx4ZD9CHAo6BkfuEGLXJM1bHV3EHQrPvLRfAZoyUD8 7XDcRT3hw1V4bHBVxSu5vfHKPX7CGVMh9Ri7MWaLmhF42ncGfis")
##if retval1 != 0:
##	print "test 6 error"
##	sys.exit()
##
##
##if not os.path.isfile("multi_redeem_nmc_6NekoPd9nP3Hj4pzeNAkFHhEpTwfjojAcz.script"):
##	print "test 6 file error"
##	sys.exit()
##
##
##printHeader("Test 7 bitgen-nmc multisig")
##retval1 = os.system("../bitgen-nmc transaction 7477474774747478484848488484848484848888348baaba77a777a777a77777 0 multi_redeem_nmc_6NekoPd9nP3Hj4pzeNAkFHhEpTwfjojAcz.script 10.01 out MxVUy2NBgiQuc2iSHQ6CmmTArRSMNCtjwx 10.00")
##if retval1 != 0:
##	print "test 7 error"
##	sys.exit()
##
##
##if not os.path.isfile("nmc_nonsigned_1EA86949978A183ABFB519271EC78466504571EFEB81AB2D87A90A9A3B42AAEB.trans"):
##	print "test 7 file error"
##	sys.exit()
##
##
##printHeader("Test 8 bitgen-nmc signmultisig")
##retval1 = os.system("../bitgen-nmc signmultisig nmc_nonsigned_1EA86949978A183ABFB519271EC78466504571EFEB81AB2D87A90A9A3B42AAEB.trans KzjX1Thmm1cx25xrhiLNa9RWCxfVDtzfNw4QMqKAtKiw16qMzVb7 fixednounce")
##if retval1 != 0:
##	print "test 8 error"
##	sys.exit()
##
##
##if not os.path.isfile("sig_nmc_7ZmAjGWYJxx4ZD9CHAo6BkfuEGLXJM1bHV3EHQrPvLRfAZoyUD8.sig"):
##	print "test 8 file error"
##	sys.exit()
##
##
##printHeader("Test 9 bitgen-nmc combinemultisig")
##retval1 = os.system("../bitgen-nmc combinemultisig nmc_nonsigned_1EA86949978A183ABFB519271EC78466504571EFEB81AB2D87A90A9A3B42AAEB.trans sig_nmc_7ZmAjGWYJxx4ZD9CHAo6BkfuEGLXJM1bHV3EHQrPvLRfAZoyUD8.sig")
##if retval1 != 0:
##	print "test 9 error"
##	sys.exit()
##
##
##if not os.path.isfile("nmc_3C68D9D0D039802700612441C1DC6715715356057616F215DCD33B654EFA4E86.trans"):
##	print "test 9 file error"
##	sys.exit()
##
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
