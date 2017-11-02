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




printHeader("Test 1 bitgen-btc electrum")
retval1 = os.system("../bitgen-btc electrum hash ydydyusuusdasiduyiuyiusaydiuaysdiuyasdiuyuyuasdyuasydysayyyfyfyduasfskjdhfkjdhfjkshhhfhdjsfhsjdhfjsdhfdjshfjhyuasdfyuasydfaisdfiuisdufisdufisudfhhhfhshdfkjshdfkjhjkhjhsadfkjhsdkjfhsdkjfhskjdfhkjsdhfkjsdhfkjdshfkjhhghfguuuUUUUSDFusdfiusdfyiuyiuyTTT")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("extended_csKYqcEjPgimvFp2BECyPjzD1CUq4nTYoXEYMRD74aCvroi1Uf69dr8JcFmj6_private.txt"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hash QUQUIISOSKDuyiusaydiuaysdiuyasdiuyuyuasdyuasydysayyyfyfyduasfskjdhfkjdhfjkshhhfhdjsfhsjdhfjsdhfdjshfjhyuasdfyuasydfaisdfiuisdufisdufisudfhhhfhshdfkjshdfkjhjkhjhsadfkjhsdkjfhsdkjfhskjdfhkjsdhfkjsdhfkjdshfkjhhghfguuuUUUUSDFusdfiusdfyiuyisdasduyTTT 15 name \"Fifteen word mnemonic\"")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("extended_SieVKFhJPrVZjR73hB8tPSnKm8y3GhQ6VSmZBJcYdRW3diJ5KSthoD26oPEqg_share.txt"):
	print "test 2 file error"
	sys.exit()


printHeader("Test 3 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum mnemonic \"abstract embody sell friend useless clutch field print cupboard movie hat myself follow arm quiz screen stick burger chuckle found mimic core refuse card\"")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("extended_1h8igJ9nPbXFn4DZtW2kGMq6QDwm97KcUfUBKLAKQ66CeHyi5zeGDJNrjTKhc_private.ps"):
	print "test 3 file error"
	sys.exit()




printHeader("Test 4 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum mnemonic \"capable lazy unusual word pumpkin pipe parade hidden system scheme soda snack first\" name \"My test seed\"")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("extended_RWpS9eGhbUDA4MFHxQNnMA5jH2hTNgSY2PCBQPxi1wZxe9dAiELHeCx8DUGAs_private.txt"):
	print "test 4 file error"
	sys.exit()






printHeader("Test 5 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hex 978AF717B88C88E88F8817399109998AABAABFF5673892387429839919919111")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("extended_kz62zkpg99sDQcrcarmY3rFXeNLZjYBWXfZAAnfk4eWj6mwnsc6BsTUE81H2X_private.txt"):
	print "test 5 file error"
	sys.exit()







printHeader("Test 6 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hex 978AF717B88C88E886667623476BBBCBB01 13")
if retval1 != 0:
	print "test 6 error"
	sys.exit()

if not os.path.isfile("extended_sejMMKtjBtYMUQEC9HWZpUoRwgLCEzgwDSMdzfjEHFZ3QXbk2dia2X8JdzUdv_private.txt"):
	print "test 6 file error"
	sys.exit()




printHeader("Test 7 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum dice 1313166123662662666231131231235355123123125155611232231252552524352341523415243123123123123333222561")
if retval1 != 0:
	print "test 7 error"
	sys.exit()

if not os.path.isfile("extended_SpygddvxAwZ3BZBwWdNdASxdpbHvLFqZNEnq2a1MZkD6Xn3VK9G7ZGBq82g2w_private.txt"):
	print "test 7 file error"
	sys.exit()




printHeader("Test 8 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum dice 445116612366266266623113123123535512312312515561123221 13 name \"My dice test\"")
if retval1 != 0:
	print "test 8 error"
	sys.exit()

if not os.path.isfile("extended_UgncwW6XXYJykHZmpLowBVGfFk5vXjoShqkuJnu618BsNb627ibzWCYa4m15b_private.txt"):
	print "test 8 file error"
	sys.exit()




printHeader("Test 9 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum brain myemail@salt.com \"Brown brown feet in wet wet mud will jump jump jump up up UP to catch the little bird abovE the rainbow when I go to sleep ooooh zzzz zzzz zzzz\"")
if retval1 != 0:
	print "test 9 error"
	sys.exit()

if not os.path.isfile("extended_1yDqrxEt6hC9YwPXhGQhuyhcuuwUwhVCZWti1WeXgbWu6x1wdehL6z5qb6LhU_private.txt"):
	print "test 9 file error"
	sys.exit()




printHeader("Test 10 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum brain myemail@salt.com \"Black black feet in wet wet mud will jump jump jump up up UP to catch the little bird abovE the rainbow when I go to sleep ooooh zzzz zzzz zzzz\" 15 name \"My brainwallet test\"")
if retval1 != 0:
	print "test 10 error"
	sys.exit()

if not os.path.isfile("extended_KyETEwtXicBfBC41EYqKt1ySzNmfmxzT1TnUifiP5NvwGgbcZnmKHgPoXi3t2_private.txt"):
	print "test 10 file error"
	sys.exit()






printHeader("Test 11 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum random")
if retval1 != 0:
	print "test 11 error"
	sys.exit()



printHeader("Test 12 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hash kdsjfskjdhfkjsdhfjkdjjdjj")
if retval1 == 0:
	print "test 12 error"
	sys.exit()




printHeader("Test 13 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum random 10")
if retval1 == 0:
	print "test 13 error"
	sys.exit()



printHeader("Test 14 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hex 234234324123123FCA123123123123990 12 name \"Hex Test\" nops")
if retval1 != 0:
	print "test 14 error"
	sys.exit()

if not os.path.isfile("extended_NPgzxWwJApNayQYPgYrj74eaJQrmb8eE6CfhSf2PGbEjNDkpdgHa8Wm9NeT86_private.txt"):
	print "test 14 file error"
	sys.exit()




printHeader("Test 15 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hex 111234324123123FCA123123123123222 12 name \"Hex Test 2\" notxt")
if retval1 != 0:
	print "test 15 error"
	sys.exit()

if not os.path.isfile("extended_n2uzGprsmn3iXHuK7ZbUCmFz1An4DVha3SKLLs8dow5M1WLTbuiVTG5sTFMYt_share.ps"):
	print "test 15 file error"
	sys.exit()




printHeader("Test 16 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hex 0000000000000000000000000000000000000000000000000 18")
if retval1 != 0:
	print "test 16 error"
	sys.exit()

if not os.path.isfile("extended_LQynphqqp4wdhNydoQWmX5rSX6qDVXBSY1ZcCYAU46XbQkrK29aSAXS7aFT52_private.txt"):
	print "test 16 file error"
	sys.exit()




printHeader("Test 17 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum hex FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF 13")
if retval1 != 0:
	print "test 17 error"
	sys.exit()

if not os.path.isfile("extended_XWZAdbbwCLfcGP2JTtWgQ6mitDoqaAw6VwYxcEozo14Qekpdb6csdvxaRkZxk_private.ps"):
	print "test 17 file error"
	sys.exit()




printHeader("Test 18 bitgen-btc electrum")
retval1 = os.system("../bitgen electrum dice 11111111111111111111111111111111111111111111111111 12")
if retval1 != 0:
	print "test 18 error"
	sys.exit()

if not os.path.isfile("extended_XPHU2CFrYJ47EfnhLNJFK8EBDe9uCeYDnHpVDLLVCiQWgqHAGN5KRSStYQZqg_private.txt"):
	print "test 18 file error"
	sys.exit()




print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.trans *.script *.txt *.sig *.ps")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

sys.exit(123)

