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


printHeader("Test 1 bitgen-ltc multisig")
retval1 = os.system("../bitgen-ltc multisig 1 2 7XtwtHkGudTphT3s27GB7cgUfftPzMxtiB3HvcVdCUAU4vYmmC3 7YqeMkBofHvZDoNv4KsvRNZ5G9uA2XgeGX1EbFrHWSuB5YxJ5hW")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("multi_redeem_ltc_3LEvJTZ4LkN9PYJciERzyq5HhmoASQHL64.script"):
	print "test 1 file error"
	sys.exit()




printHeader("Test 2 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 1342342342342342342342342342342342343234243423333242342342342349 0 multi_redeem_ltc_3LEvJTZ4LkN9PYJciERzyq5HhmoASQHL64.script 10.000 out LYx3YjFPUvDNfWjsf9dCzo8TinAZ9Hc9eF 9.999")
if retval1 != 0:
	print "test 2 error"
	sys.exit()

if not os.path.isfile("ltc_nonsigned_BF7888D665EF46A4232613443DB216FDBA1D15B3329380998CE7A2FE35047C32.trans"):
	print "test 2 file error"
	sys.exit()




printHeader("Test 3 bitgen-ltc signmultisig")
retval1 = os.system("../bitgen-ltc signmultisig ltc_nonsigned_BF7888D665EF46A4232613443DB216FDBA1D15B3329380998CE7A2FE35047C32.trans T7sm4hPKbpGm5KhfsZkbTeaBLGikhnP7TH4TdgHqXqvv35x5UrLz fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("sig_ltc_7XtwtHkGudTphT3s27GB7cgUfftPzMxtiB3HvcVdCUAU4vYmmC3.sig"):
	print "test 3 file error"
	sys.exit()




printHeader("Test 4 bitgen-ltc signmultisig")
retval1 = os.system("../bitgen-ltc signmultisig ltc_nonsigned_BF7888D665EF46A4232613443DB216FDBA1D15B3329380998CE7A2FE35047C32.trans T7dEssEuG6BmEFGz9SZgvjY4LP4hbrcWZRacqh5d1o2Yk74YXSBE fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("sig_ltc_7YqeMkBofHvZDoNv4KsvRNZ5G9uA2XgeGX1EbFrHWSuB5YxJ5hW.sig"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-ltc combinemultisig")
retval1 = os.system("../bitgen-ltc combinemultisig ltc_nonsigned_BF7888D665EF46A4232613443DB216FDBA1D15B3329380998CE7A2FE35047C32.trans sig_ltc_7XtwtHkGudTphT3s27GB7cgUfftPzMxtiB3HvcVdCUAU4vYmmC3.sig sig_ltc_7YqeMkBofHvZDoNv4KsvRNZ5G9uA2XgeGX1EbFrHWSuB5YxJ5hW.sig")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("ltc_E01B4A5EC3303CD9A520A4AF191CFF138CE7C9C078705DB534D303E63E5C7204.trans"):
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


