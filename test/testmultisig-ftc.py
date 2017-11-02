#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitgen-ftc"):
	print "Error, application binary does not exist"
	sys.exit()



#============ Start of tests ============


#PUB : 6w4SpzvfP3xEg2zVukzJjVLJQi2dKpDUod
#PKIF: 7ZSQvrW9mDs2j9znrThzCUS2RQNwYzHPpDjD9iEzaSq7Vzk8h2F
#PRIV: N9ppEmwxwn1mKygDjwDuX5Rq7iAXAcEC96hYhXoCKGCFCwaZYMKc

#PUB : 6iuxvgCPTCpogy8qAP5GnMX94w9ViYoA8u
#PKIF: 7XzdHaadESMBkGDmsF6NdDoNr4d3oVXd9z4N7MkM3naakJLZxn2
#PRIV: N6FoXJwV5zL1j7xxQ1Efkx2zXaj4T2BzQrqYdT2V6QJVKRmBiaM4


printHeader("Test 1 bitgen-ftc multisig")
retval1 = os.system("../bitgen-ftc multisig 1 2 7ZSQvrW9mDs2j9znrThzCUS2RQNwYzHPpDjD9iEzaSq7Vzk8h2F 7XzdHaadESMBkGDmsF6NdDoNr4d3oVXd9z4N7MkM3naakJLZxn2")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

if not os.path.isfile("multi_redeem_ftc_3NQUGYeunCt1R2yJezvAm1CDBp1FVBAjZw.script"):
	print "test 1 file error"
	sys.exit()




printHeader("Test 2 bitgen-ftc transaction")
retval1 = os.system("../bitgen-ftc transaction 1342342342342342342342342342342342343234243423333242342342342349 0 multi_redeem_ftc_3NQUGYeunCt1R2yJezvAm1CDBp1FVBAjZw.script 11.000 out 6o78C5mAymXqjUDouwPmT2noij3kcUP4Vp 10.999")
if retval1 != 0:
	print "test 2 error"
	sys.exit()

if not os.path.isfile("ftc_nonsigned_CC2B4D5151C385AA7CB33E8F2E36307FD65064BBE5DA65E43008830EEC4EA227.trans"):
	print "test 2 file error"
	sys.exit()




printHeader("Test 3 bitgen-ftc signmultisig")
retval1 = os.system("../bitgen-ftc signmultisig ftc_nonsigned_CC2B4D5151C385AA7CB33E8F2E36307FD65064BBE5DA65E43008830EEC4EA227.trans N6FoXJwV5zL1j7xxQ1Efkx2zXaj4T2BzQrqYdT2V6QJVKRmBiaM4 fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("sig_ftc_7XzdHaadESMBkGDmsF6NdDoNr4d3oVXd9z4N7MkM3naakJLZxn2.sig"):
	print "test 3 file error"
	sys.exit()




printHeader("Test 4 bitgen-ftc signmultisig")
retval1 = os.system("../bitgen-ftc signmultisig ftc_nonsigned_CC2B4D5151C385AA7CB33E8F2E36307FD65064BBE5DA65E43008830EEC4EA227.trans N9ppEmwxwn1mKygDjwDuX5Rq7iAXAcEC96hYhXoCKGCFCwaZYMKc fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("sig_ftc_7ZSQvrW9mDs2j9znrThzCUS2RQNwYzHPpDjD9iEzaSq7Vzk8h2F.sig"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-ftc combinemultisig")
retval1 = os.system("../bitgen-ftc combinemultisig ftc_nonsigned_CC2B4D5151C385AA7CB33E8F2E36307FD65064BBE5DA65E43008830EEC4EA227.trans sig_ftc_7ZSQvrW9mDs2j9znrThzCUS2RQNwYzHPpDjD9iEzaSq7Vzk8h2F.sig")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("ftc_D8B46A2BAADD306422B339D76F013D299E481BB58593FD8C2D6C054C8B43DD6D.trans"):
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


