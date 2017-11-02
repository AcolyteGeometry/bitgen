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
retval1 = os.system("../bitgen-ltc transaction 6374263746324623746237463276423746273642736427364273642764234233 0 T3JUdwMyRui7837c1MMfncLVpCDsRMywXM9F3DREPmtduTyfeNgx 5.000 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

printHeader("Test 2 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 8276428346238746238746773737733399399393939999399393993939399399 5 T8ccSypoDxMUexMiBLXvnVFtmQCEwKoZZ4rjxVuRhxG19WziVSVA 55.000 out LT3gRsVUohDtotVSGUvKds8QRMqqHbyBvZ 54.999")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


printHeader("Test 3 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 6374263746324623746237463276423746273642736427364273642764234233 0 T3JUdwMyRui7837c1MMfncLVpCDsRMywXM9F3DREPmtduTyfeNgx 5.000 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999 extra fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()

if not os.path.isfile("ltc_B7C499048EB17EB2B6D02E0877CD7CC22699DACBFFF746576665F8687F33561E.trans"):
	print "test 3 file error"
	sys.exit()



printHeader("Test 4 bitgen-ltc transaction pay-to-pubkey")
retval1 = os.system("../bitgen-ltc transaction 1114263746324623746237463276423746273642736427364273642764234233 0 _T3JUdwMyRui7837c1MMfncLVpCDsRMywXM9F3DREPmtduTyfeNgx 5.000 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999 extra fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()

if not os.path.isfile("ltc_DD0A3A2E49624B380885DB4342739434F5F9F1EFBBD1130BA39E1B45B4E7E16C.trans"):
	print "test 4 file error"
	sys.exit()



printHeader("Test 5 bitgen-ltc transaction anyone-can-spend")
retval1 = os.system("../bitgen-ltc transaction 7856541746324623746237463276423746273642736427364273642764234233 0 anyone 5.000 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999 extra fixednounce")
if retval1 != 0:
	print "test 5 error"
	sys.exit()
	


if not os.path.isfile("ltc_nonsigned_F3A262949AFC9FB9F266785183AC0BE8103E764A8C59C7877493F4D6BEC31EF8.trans"):
	print "test 5 file error"
	sys.exit()


printHeader("Test 6 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc decodetrans ltc_DD0A3A2E49624B380885DB4342739434F5F9F1EFBBD1130BA39E1B45B4E7E16C.trans savescript")
if retval1 != 0:
	print "test 6 error"
	sys.exit()


if not os.path.isfile("ltc_script_input_DD0A3A2E49624B380885DB4342739434F5F9F1EFBBD1130BA39E1B45B4E7E16C_0.script"):
	print "test 6 file 1 error"
	sys.exit()

if not os.path.isfile("ltc_script_output_DD0A3A2E49624B380885DB4342739434F5F9F1EFBBD1130BA39E1B45B4E7E16C_0.script"):
	print "test 6 file 2 error"
	sys.exit()




printHeader("Test 7 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 1919141746324623746237463276423746273642736427364273642764234233 0 ltc_script_input_DD0A3A2E49624B380885DB4342739434F5F9F1EFBBD1130BA39E1B45B4E7E16C_0.script 5.000 out LeHocymWW3RVPEY7Ft7iuxb3UDWmC7coMs 4.999 extra fixednounce")
if retval1 != 0:
	print "test 7 error"
	sys.exit()

if not os.path.isfile("ltc_nonsigned_76C32D3CDFAA307068B4EB7B71D871A73E882834E7AB26F8DD1A14224A38C7FB.trans"):
	print "test 7 file error"
	sys.exit()



printHeader("Test 8 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 7856541746324623746237463276423746273642736427364273642764234233 0 T695Q7RMBtqqTMsfDBCUmBy9ckYofhkLgjrjgd7Qd46MqKeWXNe8 3.500 out OP_RETURN 0.000 LKu1i6Y9r2sYVmQM79R7naXv3XnZKJPubx 3.499 extra fixednounce")
if retval1 != 0:
	print "test 8 error"
	sys.exit()

if not os.path.isfile("ltc_40FABD38910CA9A62931D923F68989B2AD57ADADC5E4687650815CA099DF0327.trans"):
	print "test 8 file error"
	sys.exit()



printHeader("Test 9 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 7856541746324623746237463276423746273642736427364273642764234233 0 T695Q7RMBtqqTMsfDBCUmBy9ckYofhkLgjrjgd7Qd46MqKeWXNe8 3.500 out op_return \"202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F50\" LKu1i6Y9r2sYVmQM79R7naXv3XnZKJPubx 3.499")
if retval1 == 0:
	print "test 9 error"
	sys.exit()



printHeader("Test 10 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 7856541746324623746237463276423746273642736427364273642764211111 0 T695Q7RMBtqqTMsfDBCUmBy9ckYofhkLgjrjgd7Qd46MqKeWXNe8 3.500 out op_return \"202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F4041424344454647\" LNEg4XEiK2wNrpdPcbkZDw1EFh5TnLEUzq 3.499 extra fixednounce")
if retval1 != 0:
	print "test 10 error"
	sys.exit()

if not os.path.isfile("ltc_93737583768F99CF608B55F42DC3F49971A5EFC5F634394A855DB56F178D2F5D.trans"):
	print "test 10 file error"
	sys.exit()




printHeader("Test 11 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 7856541746324623746237463276423746273642736427364273642764211111 0 T695Q7RMBtqqTMsfDBCUmBy9ckYofhkLgjrjgd7Qd46MqKeWXNe8 3.500 out DOCPROOF 202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F LdQyLYWatGqxSJBK5d1sey5noXEjMSPrMM 3.499 extra fixednounce")
if retval1 != 0:
	print "test 11 error"
	sys.exit()

if not os.path.isfile("ltc_687EF521B1915B16145A4854715D00FA637943A9E09C7C94014E97961DF035DA.trans"):
	print "test 11 file error"
	sys.exit()


printHeader("Test 12 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 7856541746324623746237463276423746273642736427364273642764211111 0 T695Q7RMBtqqTMsfDBCUmBy9ckYofhkLgjrjgd7Qd46MqKeWXNe8 7.500 out EW \"This is a message to eternity\" LdQyLYWatGqxSJBK5d1sey5noXEjMSPrMM 7.499 extra fixednounce")
if retval1 != 0:
	print "test 12 error"
	sys.exit()

if not os.path.isfile("ltc_58C1937FB208549E941C49BC0C9DB302041B4FC6D9E0130D0CF7206CD07A590C.trans"):
	print "test 12 file error"
	sys.exit()




printHeader("Test 13 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 7856541746324623746237463276423745556442736427364273642764211111 0 T695Q7RMBtqqTMsfDBCUmBy9ckYofhkLgjrjgd7Qd46MqKeWXNe8 8.500 out EWA \"My message to eternity\" LggpUtx938X2YCCGdPE76q77S9ySJ7qqKw 8.499 extra fixednounce")
if retval1 != 0:
	print "test 13 error"
	sys.exit()

if not os.path.isfile("ltc_99A3F264310660CC1ECDBB62F430B898A6ED0E04E523A357192743993C6A156C.trans"):
	print "test 13 file error"
	sys.exit()




printHeader("Test 14 bitgen-ltc transaction")
retval1 = os.system("../bitgen-ltc transaction 2828381746324623746237463276423745556442736427364273642764211111 1234 T8nT5xdDkN53mgC4aLtSQgV1uLuauX7h8T7crZ6DNHNZmXeXu8wL 2221118.500 out EWA \"My little message\" anyone 2221118.499 extra fixednounce")
if retval1 != 0:
	print "test 14 error"
	sys.exit()

if not os.path.isfile("ltc_3F111958E059182DC908A843EEC249EFA474C7E2E01A32B1CC3EAC7706EBF84D.trans"):
	print "test 14 file error"
	sys.exit()


printHeader("Test 15 bitgen-ltc multisig")
retval1 = os.system("../bitgen-ltc multisig 1 2 7XthgB35pP5U2GCWvrYT7uqdXT3RQBAi5LTZexGJs8SETz9TVZs 7Z51yw5bL65Ri3iA92Zg678UMvAqUAxMoUXBFxHnRmBGxhSHG3q")
if retval1 != 0:
	print "test 15 error"
	sys.exit()

if not os.path.isfile("multi_redeem_ltc_3Jm3cnzG9ekPk5yP4VsGjFHvcap6jjAHj2.script"):
	print "test 15 file error"
	sys.exit()






printHeader("Test 16 bitgen-ltc multisig")
retval1 = os.system("../bitgen-ltc multisig 4 4 7XthgB35pP5U2GCWvrYT7uqdXT3RQBAi5LTZexGJs8SETz9TVZs 7Z51yw5bL65Ri3iA92Zg678UMvAqUAxMoUXBFxHnRmBGxhSHG3q 7X2qWU15VHVFgHkmJEZrAnbJPKDWH2B7dd93BScifxfr8Sui8QK 7XtFZ9mZTEzrDPTw9CNsdR468BCAD2wk9SWtaYVWoX5EsMUoYFG")
if retval1 != 0:
	print "test 16 error"
	sys.exit()

if not os.path.isfile("multi_redeem_ltc_33bBw6thX8YQoBHY4tC6MCyz5nsQbYHCXm.script"):
	print "test 16 file error"
	sys.exit()



printHeader("Test 17 bitgen-ltc multisig")
retval1 = os.system("../bitgen-ltc multisig 4 16 7XthgB35pP5U2GCWvrYT7uqdXT3RQBAi5LTZexGJs8SETz9TVZs 7Z51yw5bL65Ri3iA92Zg678UMvAqUAxMoUXBFxHnRmBGxhSHG3q 7X2qWU15VHVFgHkmJEZrAnbJPKDWH2B7dd93BScifxfr8Sui8QK 7XtFZ9mZTEzrDPTw9CNsdR468BCAD2wk9SWtaYVWoX5EsMUoYFG 7WEqCDtJBExu7G7AvkhSH1QWyWHQqzZwkeXcBHCVFzNAu6NgoVH 7Z5xoRLStX225gZfdi9iEmZd7URwYjvF859yiVKbZUEJ4Wi6txu 7YtL1xBF6wnbLNZsfbPkJAqewZhJYGxGuWdod4fQjCwKg6i4KSy 7WgYzadG34QgVgVrQriE1es7QfH4vSgBss3gem1fZqEMXtoejb9 7WeQpmkjB1FbdAersy4igV1XYpF79cBnjVBrasgKzr4t1gFq7MF 7XU8HQBuVUg5GG5HcfJc57GvETN8BYBaBnoSj9BXe4LN4VNLekr 7XAeRnRM5TpASV4rb3RpLQthjz4wvMUR1nchKpQe6UGkzDMvQZK 7ZTJooqZZS4TDdraUwdWAQ4W6gs3W9K5C2TXWiUvyQEm87iBEud 7WznNAbXtnprsnvucBauDECEzrRyBWtLzu7GnQqNBoBYDRbgDeT 7Wm96x7BMw4XGX2vJC6UEUTLESsUtgTcgDi5Y8cFxrRiNn8M9jw 7WUw8tAryFxDYsAcpPpt2te7VT5gQuiJ9NR8p85e5eQgnJ3naXr 7XLoX4KkqydJQzHz2YwHBd9M9UgfoiLK3sadmZXXdbHv4xyhaeZ")
if retval1 != 0:
	print "test 17 error"
	sys.exit()

if not os.path.isfile("multi_redeem_ltc_3QBynfbKFP7Nf7evoQUSAfNUBhkwdw2HW7.script"):
	print "test 17 file error"
	sys.exit()



printHeader("Test 18 bitgen-ltc multisig")
retval1 = os.system("../bitgen-ltc multisig 1 1 7XthgB35pP5U2GCWvrYT7uqdXT3RQBAi5LTZexGJs8SETz9TVZs")
if retval1 != 0:
	print "test 18 error"
	sys.exit()

if not os.path.isfile("multi_redeem_ltc_3LLFP6P8csN2kWBHXfGjMs4jQc5X7d4vNG.script"):
	print "test 18 file error"
	sys.exit()



print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.trans *.script *.txt")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


