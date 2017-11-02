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

printHeader("Test 1 bitgen-btc multisig")
retval1 = os.system("../bitgen-btc multisig 1 2 7XncH1N1NhzvWLmZKbMgY5G6KPMtNSRd5ksbguRGin2gLcFVm47 7Ymgkmw35CFk4mjWvCEmmYhXhQ7gn11cF7eAHTMoccPZyYKz6hj")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("multi_redeem_btc_3QEdVuYEQd8Nb2B6yuWprXBNfpmBPYfTQQ.script"):
	print "test 1 file error"
	sys.exit()



printHeader("Test 2 bitgen-btc transaction")
retval1 = os.system("../bitgen-btc transaction 1376575765765765675666657657657657657657657657657652342342342349 0 multi_redeem_btc_3QEdVuYEQd8Nb2B6yuWprXBNfpmBPYfTQQ.script 11.000 out 16RFJbU1cEVbGSDAN2cNuYjab1UkAgUBbE 7.999")
if retval1 != 0:
	print "test 2 error"
	sys.exit()

if not os.path.isfile("btc_nonsigned_B148F58C7E89D9ED38A46A495C3FC2965F4C2D12127D99F1FBC944EBEFB24566.trans"):
	print "test 2 file error"
	sys.exit()




printHeader("Test 3 bitgen-btc signmultisig")
retval1 = os.system("../bitgen-btc signmultisig btc_nonsigned_B148F58C7E89D9ED38A46A495C3FC2965F4C2D12127D99F1FBC944EBEFB24566.trans L1vi8Zt6LsBsDRzdhmyzg7wkG23p2AtW8wegNU1tSa12KBM3Luef fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("sig_btc_7XncH1N1NhzvWLmZKbMgY5G6KPMtNSRd5ksbguRGin2gLcFVm47.sig"):
	print "test 3 file error"
	sys.exit()



printHeader("Test 5 bitgen-btc combinemultisig")
retval1 = os.system("../bitgen-btc combinemultisig btc_nonsigned_B148F58C7E89D9ED38A46A495C3FC2965F4C2D12127D99F1FBC944EBEFB24566.trans sig_btc_7XncH1N1NhzvWLmZKbMgY5G6KPMtNSRd5ksbguRGin2gLcFVm47.sig")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("btc_9B638855233878C7B5C5C54BCF9B4F50EBCEF5D43A1EFE79B32D91533A8388EC.trans"):
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
