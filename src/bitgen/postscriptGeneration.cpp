/**
 * qrencode - QR Code encoder
 *
 * QR Code encoding tool
 * Copyright (C) 2006-2013 Kentaro Fukuchi <kentaro@fukuchi.org>
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
 
#include <iostream>
#include <string>
#include <list>
 

#include "url.h"

#include "qrencode.h"
#include "qrenc.h"

#include "FileUtil.h"
#include "printPS.h"
#include "currencySpecific.h"



std::string PostscriptGenerator::createEncString(const PublicPostscriptInfo& pubInfo, const std::string& amount, const std::string& message)
{
	const std::string urlPrefix = getUrlPrefix();
	std::string encStr(urlPrefix + pubInfo.pubkey);

	bool isFirst(true);

	if(!amount.empty())
	{
		encStr += isFirst ? "?" : "&";
		isFirst = false;
		
		encStr += "amount=";		
		encStr += amount;
	}

	if(!pubInfo.accountName.empty())
	{
		encStr += isFirst ? "?" : "&";
		isFirst = false;
		
		encStr += "label=";		
		encStr += urlEncode(pubInfo.accountName);
	}

	if(!message.empty())
	{
		encStr += isFirst ? "?" : "&";
		isFirst = false;
		
		encStr += "message=";		
		encStr += urlEncode(message);
	}
	
	return encStr;
}



std::string PostscriptGenerator::createPrivateEncString(const PrivatePostscriptInfo& privInfo)
{
	std::string urlPrefix;	//!!!!!!!!!!
	std::string encStr(urlPrefix + privInfo.privkey);

	return encStr;
}






/*
void PostscriptGenerator::saveInvoiceFile(const PublicPostscriptInfo& pubInfo,
	const std::string& amount, const std::string& message) const
{		
	const std::string psPublic = generateInvoice(pubInfo, amount, message);
	
	const std::string pubFilename = pubInfo.pubkey + "_invoice.ps";
	std::cout << "Saving publfile: " << pubFilename << std::endl;
	fileWrite(pubFilename, psPublic);
	
}
*/


/*
void PostscriptGenerator::savePrivateHierarcialFile(const std::string& signature, const std::string& privIndex, 
	const std::string& pubkey, const PublicPostscriptInfo& pubInfo, const HierarcialInfo& hierarcialInfo) const
{
	const std::string psPublic = privateHierarcialPrivate(signature, privIndex, pubInfo, hierarcialInfo);
	
	const std::string pubFilename = pubkey + "_priv.ps";
	std::cout << "Saving privfile: " << pubFilename << std::endl;
	fileWrite(pubFilename, psPublic);
	
}
*/



//Todo: Remove
//std::string PostscriptGenerator::generatePublic(const PublicPostscriptInfo& pubInfo) const
//{	
//	const std::string psPublic = postscriptPagePublic(pubInfo);		
//	return psPublic;	
//}


	
PsInfoFile
PostscriptGenerator::generate(
	const PrivatePostscriptInfo& privInfo,
	const PublicPostscriptInfo& pubInfo
	) const
{

	const std::string psPublic = postscriptPagePublic(pubInfo);

	const std::string psPrivate = generatePrivate(true, privInfo, pubInfo);
	
	const PsInfoFile pif(psPublic, psPrivate);
	return pif;
}
	
	
/*	
void oldGenerateBitcoinAddressPostscript(const std::string& pubkey, const std::string& privkey,
	const std::string& generationMethod, const std::list<std::string>& generationExtra,
	const std::string& thetime, const std::string& accountName, const std::string& mnemonic,
	const bool compressed, const std::string pubPath, const std::string privPath,
	const std::string& versionNumber, const std::string& applicationName)
{

	const std::string psPublic = generatePublicBitcoinAddressPostscript(pubkey, generationExtra, thetime, accountName,
																		versionNumber, applicationName);

	const std::string psPrivate = postscriptPage(pubkey, privkey, true, 
												generationMethod, generationExtra, thetime, accountName, 
												mnemonic, compressed,
												versionNumber, applicationName);

	const std::string privFilename = pubkey + "_private.ps";
	//const std::string fullPrivPath = privPath + "/" + privFilename;	
	const std::string fullPrivPath = (privPath == ".") ? privFilename : (privPath + "/" + privFilename);
	
	std::cout << "Saving privfile: " << fullPrivPath << std::endl;
	fileWrite(fullPrivPath, psPrivate);

	
	const std::string pubFilename = pubkey + "_share.ps";
	//const std::string fullPubPath = pubPath + "/" + pubFilename;
	const std::string fullPubPath = (pubPath == ".") ? pubFilename : (pubPath + "/" + pubFilename);	
	std::cout << "Saving publfile: " << fullPubPath << std::endl;
	fileWrite(fullPubPath, psPublic);
	
	




}
*/


//-----------------------------------------------------------


/*
void PostscriptGenerator::savePublicKeyPostscriptFile(const PublicPostscriptInfo& pubInfo) const
{	
	const std::string psPublic = postscriptPagePublicKey(pubInfo);
	
	const std::string pubFilename = pubInfo.pubkey + "_sharek.ps";
	std::cout << "Saving publfile: " << pubFilename << std::endl;
	fileWrite(pubFilename, psPublic);
}
*/




/*
void PostscriptGenerator::saveKeypairFile(
	const PrivatePostscriptInfo& privInfo,
	const PublicPostscriptInfo& pubInfo	
	) const
{
	
	std::string encStr("bitcoinpkif:" + pubInfo.pubkey);	//Todo!!!!!!!!!!!!!!!!!!!!!!

	if(!pubInfo.accountName.empty())
	{
		encStr += "?name=";
		encStr += urlEncode(pubInfo.accountName);
	}


	const QRInfo qrinfo = QRInfo::qrencode((const unsigned char*)encStr.c_str(), encStr.size());
	
	
	savePublicKeyPostscriptFile(pubInfo);
	
	const std::string privEncStr(privInfo.privkey);

	//std::cout << "Will output private" << std::endl;


	const QRInfo qrinfoPrivate = QRInfo::qrencode((const unsigned char*)privEncStr.c_str(), privEncStr.size());
	//std::cout << "Will output private ps" << std::endl;
	
	
	const std::string psPrivate = pubPrivPairPostscriptPage(true, privInfo, pubInfo);

	const std::string privFilename = pubInfo.pubkey + "_privk.ps";
	std::cout << "Saving privfile: " << privFilename << std::endl;
	fileWrite(privFilename, psPrivate);
}
*/








