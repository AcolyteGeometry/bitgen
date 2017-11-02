#!/usr/bin/python

import os
import sys
import os.path

def printHeader(str) :
   "Prints header text"
   print "===================== " + str + " ====================="
   return

if not os.path.isfile("../bitcry"):
	print "Error, application binary does not exist"
	sys.exit()


#if os.path.isfile("out_plain.txt"):
#	os.remove("out_plain.txt")



#============ Start of tests ============





printHeader("Test 1")
retval1 = os.system("../bitcry decrypt testfiles/plain.txt.bitcry 5KjdzDrcyBfXDsRjwX5Uy5yZmSnKDPEWFujXP29LTiZpoJxWqeA")
if retval1 != 0:
	print "test 1 error"
	sys.exit()

#Verify that out_plain.txt is present

if not os.path.isfile("out_plain.txt"):
	print "Error, file does not exist"
	sys.exit()

myfile = open("out_plain.txt", 'r')

plainfile = myfile.read()
print plainfile
if plainfile != "plain\n":
	print "Error, incorrect file content"
	sys.exit()
	

printHeader("Test 2")
retval2 = os.system("../bitcry decrypt testfiles/plain.txt.bitcry")
if retval2 != 0:
	print "test 2 error"
	sys.exit()

#Verify that out_plain.txt is present

if not os.path.isfile("out_plain.txt"):
	print "Error, file does not exist"
	sys.exit()

myfile = open("out_plain.txt", 'r')

plainfile = myfile.read()
print plainfile
if plainfile != "plain\n":
	print "Error, incorrect file content"
	sys.exit()


printHeader("Test 3 bitcry encrypt")
retval3 = os.system("../bitcry encrypt testfiles/toBeEncrypted.txt 7UTZisACfzPr19WUEN24gN3Hng9kEGWgnMwv5AM3DGoLh9UdUoE")
if retval3 != 0:
	print "test 3 error"
	sys.exit()

printHeader("Test 4 bitcry decrypt")
retval4 = os.system("../bitcry decrypt testfiles/toBeEncrypted.txt.bitcry")
if retval4 != 0:
	print "test 4 error"
	sys.exit()


if not os.path.isfile("out_toBeEncrypted.txt"):
	print "test 4 file error"
	sys.exit()



printHeader("Test 5 bitcry import")
retval5 = os.system("../bitcry import")
if retval5 == 0:
	print "test 5 error"
	sys.exit()


printHeader("Test 6 bitcry importPrivate")
retval6 = os.system("../bitcry importPrivate")
if retval6 == 0:
	print "test 6 error"
	sys.exit()


printHeader("Test 7 bitcry list")
retval7 = os.system("../bitcry list")
if retval7 != 0:
	print "test 7 error"
	sys.exit()



printHeader("Test 8 bitcry help")
retval8 = os.system("../bitcry help")
if retval8 != 0:
	print "test 8 error"
	sys.exit()

printHeader("Test 9 bitcry incorrect command")
retval9 = os.system("../bitcry dskjhfskdfh")
if retval9 == 0:
	print "test 9 error"
	sys.exit()



printHeader("Test 10 bitcry missing encrypt argument")
retval10 = os.system("../bitcry encrypt")
if retval10 == 0:
	print "test 10 error"
	sys.exit()



printHeader("Test 11 bitcry missing decrypt argument")
retval11 = os.system("../bitcry decrypt")
if retval11 == 0:
	print "test 11 error"
	sys.exit()



printHeader("Test 12 bitcry encryptElectrum")
retval12 = os.system("../bitcry encryptElectrum ThisIsThePlaintext 7UTZisACfzPr19WUEN24gN3Hng9kEGWgnMwv5AM3DGoLh9UdUoE")
if retval12 != 0:
	print "test 12 error"
	sys.exit()


printHeader("Test 13 bitcry decryptElectrum")
out13 = os.popen("../bitcry decryptElectrum QklFMQMS87GCjxUlHIxDkABEKjKAq4Tl5+b2FyJBuvSqGHlh2/cmitu1Lh8oIppAK6zUhYFI93dK1ennRotPG4HxxBSa6cFViokjwXZKSZBFRZpLKU5r1pzLZCcm0oMmLeFdZY4= 5KjdzDrcyBfXDsRjwX5Uy5yZmSnKDPEWFujXP29LTiZpoJxWqeA")
result13 = out13.read()
print "Result: " + result13

if result13 != "Message: ThisIsThePlaintext\n":
	print "test 13 error: " + result13
	sys.exit()



printHeader("Test 14 bitcry encryptElectrum")
out14 = os.popen("../bitcry encryptElectrum ThisIsPlainInput 7UTZisACfzPr19WUEN24gN3Hng9kEGWgnMwv5AM3DGoLh9UdUoE")
result14 = out14.read()
partres14 = result14[201:215]

shouldbe14 = "EncStr: QklFMQ"

if partres14 != shouldbe14:
	print "test 14 error: " + partres14
	print "should be    : " + shouldbe14
	
	sys.exit()



printHeader("Test 15 bitcry list")
retval15 = os.system("../bitcry list")
if retval15 != 0:
	print "test 15 error"
	sys.exit()



printHeader("Test 16 bitcry listPublic")
retval16 = os.system("../bitcry listPublic")
if retval16 != 0:
	print "test 16 error"
	sys.exit()



printHeader("Test 17 bitcry encryptSign")
retval17 = os.system("../bitcry encryptSign")
if retval17 == 0:
	print "test 17 error"
	sys.exit()


printHeader("Test 18 bitcry decryptVerify")
retval18 = os.system("../bitcry decryptVerify testfiles/es.txt.bitcrysig")
if retval18 != 0:
	print "test 18 error"
	sys.exit()

printHeader("Test 19 bitcry decryptVerify err")
retval19 = os.system("../bitcry decryptVerify NonExistingFile")
if retval19 == 0:
	print "test 19 error"
	sys.exit()


print ""
print ""
print ""
print "Cleaning up"
os.system("rm *.txt")

print ""
print ""
print ""
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++ All tests passed OK +++++++++++++++++++++++++++"
print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


