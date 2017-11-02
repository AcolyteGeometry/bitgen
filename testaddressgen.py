#!/usr/bin/python

#Usage: testaddressgen.py [Num iterations]

#Python library "pybitcoin" must be installed:
#   sudo pip install pybitcoin
#
# Also see:
# https://github.com/blockstack/pybitcoin

import os;
import subprocess;
import random
import sys
from pybitcoin import BitcoinPrivateKey



def toAddress(privKeyHex):
  privateKey = BitcoinPrivateKey(privKeyHex)
  return privateKey.public_key().address()


def toWif(privKeyHex):
  privateKey = BitcoinPrivateKey(privKeyHex)
  return privateKey.to_wif()


def testonce():
  myrand = random.getrandbits(256)
  myrandstr = format(myrand, '02X').zfill(64)

  callstring = './bitgen hex '  + myrandstr + ' uncompressed nops notxt info'
  output = subprocess.check_output(callstring, shell = True )

  lines = output.splitlines()
  wifRaw = lines[6]
  wif = wifRaw[9:]
  
  addrRaw = lines[8]
  addr = addrRaw[9:]
  
  calculatedAddress = toAddress(myrandstr)
  calculatedWif = toWif(myrandstr)

  print "HEX     : " + myrandstr
  print "WIF     : " + wif
  print "CALC WIF: " + calculatedWif
  print "ADR     : " + addr
  print "CALC ADR: " + calculatedAddress
	
  if addr == calculatedAddress and wif == calculatedWif:
	  print "OK, SAME RESULT"
  else:
	print "TEST ERROR: " + myrandstr
	sys.exit()

  print ""
  

#Main program
print "bitgen address generation test"
print "=============================="
print ""
print "Conversion from private key to bitcoin address will be performed"
print ""
print "The result will be verified with another implementation"
print "written in python"
print ""
print "Note: the bitgen binary must be present in the current folder"
print "The python library pybitcoin must be installed"
print ""


maxiter = 0
if len(sys.argv) > 1:
	maxiter = int(sys.argv[1])
	print "Iterations: ", maxiter


testnum = 0
while True:
	print "Test ", testnum
	testonce()
	testnum += 1
	if maxiter != 0 and testnum > maxiter:
		print "================ Test ready ================"
		sys.exit()

