#ifndef PRINT_POSTSCIPT_H
#define PRINT_POSTSCIPT_H

/**
 * printPS.h - Bitcoin poscript generator
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


#include <set>
#include <vector>
#include <string>
#include <list>

#include "qrenc.h"

#include "language.h"
#include "extraInfo.h"


class Coordinate
{
public:
	Coordinate(const int inX, const int inY) : x(inX), y(inY) {}	
	
	const int x;
	const int y;	
};


//Todo: Move to file "postscriptGeneration.h"

class PsInfoFile
{
public:
	PsInfoFile(const std::string& inPublic, const std::string& inPrivate) :
		publicPs(inPublic), privatePs(inPrivate)  {}
	
	
	const std::string publicPs;
	const std::string privatePs;
};



class PublicPostscriptInfo
{
public:
	PublicPostscriptInfo(const std::string& inPubkey, const std::string& inThetime, const std::string& inAccountName) : 
		pubkey(inPubkey), thetime(inThetime), accountName(inAccountName) {}
	
	const std::string pubkey;	
	const std::string thetime;
	const std::string accountName;	
};


class PrivatePostscriptInfo
{
public:
	PrivatePostscriptInfo(const std::string& inPrivkey, const std::string& inMnemonic, 
		const std::string& inGenerationMethod, 
		const bool inCompressed, 
		const std::list<ExtraInfo>& inGenerationExtra) : 
		privkey(inPrivkey), mnemonic(inMnemonic), generationMethod(inGenerationMethod), compressed(inCompressed), generationExtra(inGenerationExtra) {}

	const std::string privkey;
	const std::string mnemonic;
	const std::string generationMethod;
	const bool compressed;
	const std::list<ExtraInfo> generationExtra;
};



class HierarcialInfo
{
public:
	HierarcialInfo(const std::string& inSignature, const std::string& inPubX, const std::string& inPubY) :
		signature(inSignature), pubX(inPubX), pubY(inPubY) {}
	
	const std::string signature;	
	const std::string pubX;
	const std::string pubY;
};







class PostscriptGenerator
{
public:
	enum AdressType { ADDRTYPE_NORMAL, ADDRTYPE_EXTENDED, ADDRTYPE_ELECTRUM };

	PostscriptGenerator(const std::string& inVersionNumber, const std::string& inApplicationName, 
						//const bool inExtended = false,
						const Language inLang = LANG_EN, 
						const AdressType inAddressType = ADDRTYPE_NORMAL,						
						const bool inCurrencySpecific = false) :
		versionNumber(inVersionNumber), applicationName(inApplicationName), lang(inLang), margin(70), 
		//extended(inExtended),
		addrType(inAddressType),
		currencySpecific(inCurrencySpecific) {}
	
std::string generatePrivateMnemonic(const std::string mnemonic, const int numWords, const std::string& accountName,	
											const std::string& generationMethod, const std::string& theTime, 
											const std::list<ExtraInfo>& generationExtra,
											const std::string& xpriv,
											const std::string& xpub,
											const std::string& derivedXpub) const;	

std::string generatePublicMnemonic(const std::string mnemonic, const int numWords, const std::string& accountName,	
											const std::string& generationMethod, const std::string& theTime, 
											const std::list<ExtraInfo>& generationExtra,
											const std::string& xpriv,
											const std::string& xpub,
											const std::string& derivedXpub) const;	

		
PsInfoFile generate(const PrivatePostscriptInfo& privInfo, const PublicPostscriptInfo& pubInfo) const;


std::string postscriptPagePublic(const PublicPostscriptInfo& pubInfo) const;


std::string generatePrivateRoot(const HierarcialInfo& hierarcialInfo,
	const std::string& thetime, const std::string& label, 
	const std::string& privIndex) const;


std::string generatePublicRoot(const HierarcialInfo& hierarcialInfo,
		const PublicPostscriptInfo& pubInfo, const std::string& label) const;




std::string generateInvoice(const PublicPostscriptInfo& pubInfo,
							const std::string amount, const std::string& message) const;



std::string privateHierarcialPrivate(const std::string& privIndex, 
							const PublicPostscriptInfo& pubInfo, const HierarcialInfo& hierarcialInfo) const;

static std::string generateQRPostscript(const std::string& qrStr, const std::string& txtStr, 
								const int x, const int y, const double scale);

static std::string generateQRPostscript(const QRInfo& qrinfo, const std::string& txtStr, 
											const int x, const int y, const double scale);


static std::list<std::string> splitMnemonic(const std::string& mnemonic, const int spacesPerPart);
static QRInfo qrEncodeAddress(const std::string& pubkeyVal, const std::string& accountName);
static QRInfo qrEncodeExtendedAddress(const std::string& pub);

static std::string textAt(const int x, const int y, const std::string& str);


private:
static std::string qrCode(const QRInfo& qrinfo, const int x, const int y, const double scale);

std::string privateMnemonic(const std::string mnemonic, /*const int numWords, */
				const std::string& generationMethod, const std::list<ExtraInfo>& generationExtra,
				const std::string& xpriv) const;

std::string generatePaymentRequest(const PublicPostscriptInfo& pubInfo,
									const std::string& amount, const std::string& message) const;


std::string generatePrivate(const bool isPrivate, const PrivatePostscriptInfo& privInfo, const PublicPostscriptInfo& pubInfo) const;


std::string pubPrivPairPostscriptPage(const bool isPrivate, 
									const PrivatePostscriptInfo& privInfo, const PublicPostscriptInfo& pubInfo) const;


std::string publicMnemonic(const PublicPostscriptInfo& pubInfo, const bool useSmallDescription, 
							const int numWords, const std::string& derivedXpub) const;

std::string publicPart(const PublicPostscriptInfo& pubInfo, const bool useSmallText = false) const;

std::string privatePart(const PrivatePostscriptInfo& privInfo) const;


std::string postscriptPagePublicKey(const PublicPostscriptInfo& pubInfo) const;


std::string privateKeyPart(const PrivatePostscriptInfo& privInfo) const;


std::string hierPriv(const std::string& privIndex, 
	const HierarcialInfo& hierarcialInfo, const std::string& pubAddr) const;
	
std::string hierPriv(const std::string& privIndex, const HierarcialInfo& hierarcialInfo) const;

std::string privateRoot(const std::string& privIndex, const std::string& signature) const;


std::string publicRoot(const HierarcialInfo& hierarcialInfo,
	const PublicPostscriptInfo& pubInfo, const std::string& label) const;


std::string postscriptRequestPaymentPage(const PublicPostscriptInfo& pubInfo,
										const std::string& amount,
										const std::string& message) const;
							
std::string publicKeyPart(const PublicPostscriptInfo& pubInfo) const;

std::string addExtraTextLine(const int yPos, const std::string& thisPart, const int extraXindent = 0) const;

static std::string beginPage();
static std::string endPage();
static std::string intro();


static std::string generateQR(const QRInfo& qrinfo);

static std::list<std::string> splitLongLine(const std::string& str, const bool splitWordBoundry = false);


static std::string createEncString(const PublicPostscriptInfo& pubInfo, const std::string& amount, const std::string& message);
static std::string createPrivateEncString(const PrivatePostscriptInfo& privInfo);

	bool isExtended() const { return (addrType != ADDRTYPE_NORMAL); }

private:		
	const std::string versionNumber;
	const std::string applicationName;
	const Language lang;
	const int margin;
	//const bool extended;
	const AdressType addrType;
	const bool currencySpecific;
};










#endif
