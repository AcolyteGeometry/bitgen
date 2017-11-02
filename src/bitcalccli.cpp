/**
 * bitcalccli.cpp - Bitcoin address generator
 *
 * Copyright (C)2015 The bitgen developers <bitgen@openmailbox.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
 
/**
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
*/
 

#include <iostream>
#include <stdlib.h>


#include "util.h"

#include <set>
#include <map>
#include <stdio.h>

#include "bitcoinKeyPair.h"
#include "unittest.h"
#include "privKey.h"
#include "pubKey.h"

#include "commandLine.h"
#include "currencySpecific.h"
#include "license.h"
#include "usagePrintout.h"
#include "bitlibVersion.h"
#include "version.h"





void addPrivateKeys(int argc, char* argv[])
{
	bool compressed(false);
	
	if(argc < 3)
	{
		std::cout << "Usage: bitgen addPrivate wif1 wif2 ..." << std::endl;
		throw std::string("Incorrect usage");
	}

	std::cout << std::endl;

	const BigInt<1024> N(BitcoinEllipticCurve::Nval());

	BigInt<1024> priv;
	
	for(int i = 2 ; i < argc ; i++)
	{
		const std::string wifStr = argv[i];
		const Wif wif(wifStr);
		const PrivKey privKey = wif.getPriv();
		const BigInt<1024> thisPriv(BigInt<1024>::asPositive(privKey.key)) ;//Swif.toPrivKey());
	
		if(i == 2)
		{
			compressed = privKey.compressed;
			std::cout << "Format: " << (compressed ? "compressed" : "non-compressed") << std::endl;
		}

		std::cout << "This wif: " << wifStr << std::endl;
	
		priv += thisPriv;
		priv %= N;
	}


	
	const BigInt<256> privKey(priv);
				
	const PrivKey myPrivKey(privKey, compressed);
	const Wif wif = myPrivKey.getWif();
	
	BitcoinEllipticCurve ec;
	
	const PubKey pubKey = myPrivKey.getPubkey(ec);
	const Pkif pkif = pubKey.getPkif();
	
	const PubAddress address = myPrivKey.getAddress(ec);
	
	std::cout << std::endl;
	std::cout << "Wif    : " << wif << std::endl;	
	std::cout << "Pkif   : " << pkif.toString() << std::endl;
	std::cout << "Address: " << address << std::endl;	
}






void addGeneratorPoint(int argc, char* argv[])
{	
	if(argc < 3)
	{
		std::cout << "Usage: bitgen addGent pkif" << std::endl;
		throw std::string("Incorrect usage");
	}

	std::cout << std::endl;
	
	BitcoinEllipticCurve ec;	
	
	const std::string pkifStr = argv[2];	
	const PubKey pubKey = PubKey::fromPkif(pkifStr);
	
	std::cout << "In pkif: " << pkifStr << std::endl;
			
	const ECPoint resultPoint = ec.addGenPoint(pubKey.point);
		
	const PubKey resultPubKey(resultPoint, pubKey.compressed);
	const Pkif resultPkif = resultPubKey.getPkif();
		
	std::cout << std::endl;
	std::cout << "========== Result ========== " << std::endl;	
	std::cout << "Out pkif   : " << resultPkif.toString() << std::endl;	
}





void multiplyGenPointScalar(int argc, char* argv[])
{	
	if(argc < 3)
	{
		std::cout << "Usage: bitgen multGen wif" << std::endl;
		throw std::string("Incorrect usage");
	}

	std::cout << std::endl;
	
	
	const std::string wifStr = argv[2];
	const Wif wif(wifStr);
	std::cout << "In wif : " << wif.toString() << std::endl;
	
	
	
	const PrivKey privKey = wif.getPriv();
	const BigInt<1024> privNum(privKey.key);
	BitcoinEllipticCurve ec;	
	const ECPoint resultPoint = ec.multiplyGenPoint(privNum);
	const PubKey resultPubKey(resultPoint, privKey.compressed);
	
	
	const Pkif resultPkif = resultPubKey.getPkif();
		
	std::cout << std::endl;
	std::cout << "========== Result ========== " << std::endl;	
	std::cout << "Out pkif   : " << resultPkif.toString() << std::endl;	
}





void multiplyPointScalar(int argc, char* argv[])
{	
	if(argc < 4)
	{
		std::cout << "Usage: bitcalc mult pkif wif" << std::endl;
		throw std::string("Incorrect usage");
	}

	std::cout << std::endl;
	
	BitcoinEllipticCurve ec;	
	
	const std::string pkifStr = argv[2];
	const std::string wifStr = argv[3];

	
	const PubKey pubKey = PubKey::fromPkif(pkifStr);
	const bool compressed = pubKey.compressed;
	
	
	const ECPoint point = pubKey.point;

	const Wif wif(wifStr);
	std::cout << "In pkif: " << pkifStr << std::endl;
	std::cout << "In wif : " << wifStr << std::endl;
	
	
	const PrivKey privKey = wif.getPriv();

	
	const BigInt<1024> privNum(privKey.key);
	const ECPoint resultPoint = ec.multiply(point, privNum);
		

	
	const PubKey resultPubKey(resultPoint, compressed);
	const Pkif resultPkif = resultPubKey.getPkif();
		
	std::cout << std::endl;
	std::cout << "========== Result ========== " << std::endl;	
	std::cout << "Out pkif   : " << resultPkif.toString() << std::endl;	
}




void addPublicKeys(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen addPub pkif1 pkif2 ..." << std::endl;
		throw std::string("Incorrect usage");
	}

	std::cout << std::endl;
	
	BitcoinEllipticCurve ec;
	const std::string pkifStr1 = argv[2];
	const PubKey pubKey1 = PubKey::fromPkif(pkifStr1);
	const bool compressed = pubKey1.compressed;
	std::cout << "Format: " << (compressed ? "compressed" : "non-compressed") << std::endl;
	std::cout << "This pkif: " << pkifStr1 << std::endl;
	
	
	ECPoint point = pubKey1.point;

	
	for(int i = 3 ; i < argc ; i++)
	{
		const std::string pkifStr = argv[i];
		const PubKey pubKey = PubKey::fromPkif(pkifStr);
		ECPoint thispoint = pubKey.point;
		
		point = ec.add(point, thispoint);
		
		std::cout << "This pkif: " << pkifStr << std::endl;	
	}

	
	const PubKey pubKey(point, compressed);
	const Pkif pkif = pubKey.getPkif();
	
	const PubAddress address = pubKey.getAddress();
	
	std::cout << std::endl;
	std::cout << "========== Result ========== " << std::endl;	
	std::cout << "Pkif   : " << pkif.toString() << std::endl;
	std::cout << "Address: " << address << std::endl;	
	
}






void displayHelp()
{
	std::cout << "Usage: bitcalc command" << std::endl;
	std::cout << std::endl;
	std::cout << "Commands: " << std::endl;
	std::cout << "mult" << std::endl;
	std::cout << "multGen" << std::endl;
	std::cout << "addGen" << std::endl;
	std::cout << "addPriv" << std::endl;
	std::cout << "addPub" << std::endl;
	std::cout << std::endl;
	
}









void doBitCalcWork(int argc, char* argv[])
{				
	std::vector<std::string> args;
	for(int i = 1 ; i < argc ; i++)
	{
		args.push_back(argv[i]);		
	}

	if(argc < 2)
	{
		std::cout << "For help, use \"bitcalc help\"" << std::endl;
		throw std::string("Missing argument");
		return;
	}

	const std::string firstArg(argv[1]);


	if(firstArg == "version" || firstArg == "-v")
	{
		std::cout << "bitcalc version " << versionNumber << std::endl;
		std::cout << std::endl;
		std::cout << "using bigint " << BigInt<0>::version() << std::endl;
		std::cout << "using bitlib version " << bitlibVersionNumber << std::endl;
		
		std::cout << "http://www.bitgen.org/" << std::endl;		
	}
	else if(firstArg == "mult")
	{
		multiplyPointScalar(argc, argv);
	}
	else if(firstArg == "multGen")
	{
		multiplyGenPointScalar(argc, argv);
	}		
	else if(firstArg == "addGen")
	{
		addGeneratorPoint(argc, argv);
	}		
	else if(firstArg == "addPriv")
	{
		addPrivateKeys(argc, argv);
	}
	else if(firstArg == "addPub")
	{
		addPublicKeys(argc, argv);
	}	
	else if(firstArg == "help")
	{
		displayHelp();
	}		
	else
	{
		std::cerr << "Unknown command: " << argv[1] << std::endl;
	}


}










