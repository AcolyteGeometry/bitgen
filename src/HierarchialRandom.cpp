/**
 * HierarchialRandom.cpp - Bitcoin poscript generator
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

#include "HierarchialRandom.h"
#include "privKey.h"
#include "bitcoinKeyPair.h"
#include "printPS.h"
#include "FileUtil.h"
#include "currencySpecific.h"
#include "version.h"


void generateHierarchialRootKey(const BigInt<256>& privKeyR, const std::string& theTime, const std::string& label,
								const bool compressed)
{
	PrivKey priv(privKeyR, compressed);
	//std::cout << "The root private key: " << priv << std::endl;
		
	const PrivKey myPrivKey(privKeyR, compressed);
		
	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey);
		
	//std::cout << "Root private wif: " << keypair.privaddr.address << std::endl;
		
	const std::string digest = sha256(keypair.pubaddr.toString()).substr(0,4);
	
	const std::string currencyShortName = getCurrencyShortName();

	
	const std::string filePrefix = std::string("root_") + currencyShortName + "_" + hexString(digest);
		
	const std::string privateRootFile(filePrefix + "_priv.privroot");

	const std::string privFileContent(hexString(privKeyR.getBinaryLong()) + "\n" + keypair.privaddr.toString());

		
	std::cout << "Saving priv to: " << privateRootFile << std::endl;
	fileWrite(privateRootFile, privFileContent);
		 

	BitcoinEllipticCurve ec;

	//Save the public root
	const ECPoint PublicKeyRoot = priv.point(ec);
	//std::cout << "The root public key: " << PublicKeyRoot << std::endl;

	const std::string pubRootFile(filePrefix + "_pub.pubroot");
	
	BigInt<256> x(PublicKeyRoot.x);
	BigInt<256> y(PublicKeyRoot.y);
		
	std::cout << "Saving pub to: " << pubRootFile << std::endl;
	const std::string pubFileContent(hexString(x.getBinaryLong()) + "\n" + 
									 hexString(y.getBinaryLong()) + "\n"+ keypair.pubaddr.toString());
	fileWrite(pubRootFile, pubFileContent);


	const Language lang = getLanguage();
	const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, lang);

	const PublicPostscriptInfo pubInfo("dummypub", theTime, label);

	const HierarcialInfo hierarcialInfo(hexString(digest), hexString(x.getBinaryLong()), hexString(y.getBinaryLong()));

	//Generate the public ps information	
	const std::string psPublic = postscriptGenerator.generatePublicRoot(hierarcialInfo, pubInfo, label);

	const std::string psPubRootFile(filePrefix + "_pub.ps");
	std::cout << "Saving pub ps to: " << psPubRootFile << std::endl;	
	fileWrite(psPubRootFile, psPublic);



	//Generate the private ps information
	const std::string psPrivate = postscriptGenerator.generatePrivateRoot(hierarcialInfo, 
								theTime, label, hexString(privKeyR.getBinaryLong()));

	const std::string psPrivRootFile(filePrefix + "_priv.ps");
	std::cout << "Saving priv ps to: " << psPrivRootFile << std::endl;	
	fileWrite(psPrivRootFile, psPrivate);


		
	//std::cout << "===============================================" << std::endl;
}



