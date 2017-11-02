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




printHeader("Test 1 bitgen-ftc transaction")
retval1 = os.system("../bitgen-ftc transaction 6374263746324623746237463276423746273642736427364273642764234233 0 N3T9EtgmuAUQTsKozaoJARcUVkcgTjxsLVjxYMHk4ouLi8WFQufu 5.000 out 6to6GkC5BN5h5cXdoqAWLi27aDirrNWfsc 4.999 extra fixednounce")
if retval1 != 0:
	print "test 1 error"
	sys.exit()


if not os.path.isfile("ftc_FE9EDD80F95793A0E9DCF332A61DF5BC01753E6A7517367DB12262FE2ED593DB.trans"):
	print "test 1 file error"
	sys.exit()


printHeader("Test 2 bitgen-ftc transaction unknown input amount")
retval1 = os.system("../bitgen-ftc transaction 6534534534534534534555545353453458345835834853845834853845834233 0 N3T9EtgmuAUQTsKozaoJARcUVkcgTjxsLVjxYMHk4ouLi8WFQufu 0 out 6to6GkC5BN5h5cXdoqAWLi27aDirrNWfsc 4.999 extra fixednounce")
if retval1 != 0:
	print "test 2 error"
	sys.exit()


if not os.path.isfile("ftc_E02903DE6984BB7D49B69BC8F71EAC0E2EDB7A08EB579A88AB05A0B6DD0F6BAA.trans"):
	print "test 2 file error"
	sys.exit()


printHeader("Test 3 bitgen-ftc transaction unknown input amount")
retval1 = os.system("../bitgen-ftc transaction 653453453453453453455554535345345834583583485384583485384583423 0 N3T9EtgmuAUQTsKozaoJARcUVkcgTjxsLVjxYMHk4ouLi8WFQufu 0 out 6to6GkC5BN5h5cXdoqAWLi27aDirrNWfsc 4.999 extra fixednounce")
if retval1 != 0:
	print "test 3 error"
	sys.exit()


if not os.path.isfile("ftc_D4EC0A2E73922DE2EF41F64C3C8CB214C184EDD840978ADCF01C3CBB39E60BE7.trans"):
	print "test 3 file error"
	sys.exit()


printHeader("Test 4 bitgen-ftc transaction")
retval1 = os.system("../bitgen-ftc transaction 1231231231723712371273617236176237126371623716237162376127361722 0 N3T9EtgmuAUQTsKozaoJARcUVkcgTjxsLVjxYMHk4ouLi8WFQufu 1.00 out 7XpBAa9B8KT2ydTJA17kqpuzDUsHuX2ZZUopV67a34gEVCTr9EX 0.99 extra fixednounce")
if retval1 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("ftc_A79D3EB3B5927EF9EC666EB319D8D9FE77BBD58BFFB6F989CC34EB4360B73C1F.trans"):
	print "test 4 file error"
	sys.exit()




printHeader("Test 5 bitgen-ftc transaction")
retval1 = os.system("../bitgen-ftc transaction 9182918298129182928918982981928198219821982918291829182918291899 0 _N3T9EtgmuAUQTsKozaoJARcUVkcgTjxsLVjxYMHk4ouLi8WFQufu 2.01 out 6to6GkC5BN5h5cXdoqAWLi27aDirrNWfsc 2.00 extra fixednounce")
if retval1 != 0:
	print "test 5 error"
	sys.exit()


if not os.path.isfile("ftc_56B0FA0CDDEB8BEC22505B314CAEFF0865CA947E7FCF80E57C29B5F5692234F3.trans"):
	print "test 5 file error"
	sys.exit()


##printHeader("Test 6 bitgen-ftc multisig")
##retval1 = os.system("../bitgen-ftc multisig 1 2 7X683A56pxukLFzUXptcCZwBwKVtwL3ZjPW7v9jFTrGHvKvYY4Z 7XGxnoMbdtu1R5ZygY73ccSJRS7ohmpgGGeTdkTxHK3yfgzvDFS")
##if retval1 != 0:
##	print "test 6 error"
##	sys.exit()
##
##
##if not os.path.isfile("multi_redeem_ftc_3HPv5kyhdGbkbiXzgFvdmPodxvcSaZZyo2.script"):
##	print "test 6 file error"
##	sys.exit()
##
##
##printHeader("Test 7 bitgen-ftc multisig")
##retval1 = os.system("../bitgen-ftc transaction 7477474774747478484848488484848484848888348baaba77a777a777a77777 0 multi_redeem_ftc_3HPv5kyhdGbkbiXzgFvdmPodxvcSaZZyo2.script 10.01 out 6f5BhMMffYZtSkq68WqM1reh1FjPWFBnby 10.00")
##if retval1 != 0:
##	print "test 7 error"
##	sys.exit()
##
##
##if not os.path.isfile("ftc_nonsigned_45FD511D151DC54DF8A8CA772027F52E421F1E5F40B819B42FFDBBBF592449BC.trans"):
##	print "test 7 file error"
##	sys.exit()
##
##
##printHeader("Test 8 bitgen-ftc signmultisig")
##retval1 = os.system("../bitgen-ftc signmultisig ftc_nonsigned_45FD511D151DC54DF8A8CA772027F52E421F1E5F40B819B42FFDBBBF592449BC.trans N61QHJFhPMCV2bomDL9NBvDv9deP9bGoEopGtVbgsuVdCjVXQFKE fixednounce")
##if retval1 != 0:
##	print "test 8 error"
##	sys.exit()
##
##
##if not os.path.isfile("sig_ftc_7X683A56pxukLFzUXptcCZwBwKVtwL3ZjPW7v9jFTrGHvKvYY4Z.sig"):
##	print "test 8 file error"
##	sys.exit()
##
##
##printHeader("Test 9 bitgen-ftc combinemultisig")
##retval1 = os.system("../bitgen-ftc combinemultisig ftc_nonsigned_45FD511D151DC54DF8A8CA772027F52E421F1E5F40B819B42FFDBBBF592449BC.trans sig_ftc_7X683A56pxukLFzUXptcCZwBwKVtwL3ZjPW7v9jFTrGHvKvYY4Z.sig")
##if retval1 != 0:
##	print "test 9 error"
##	sys.exit()
##
##
##if not os.path.isfile("ftc_F4C1C90E4A42399B2208DA5D301AEC955D146134C80948D671F947C387B364DF.trans"):
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
