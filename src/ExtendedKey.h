#ifndef BITGEN_EXTENDED_KEY_H
#define BITGEN_EXTENDED_KEY_H

/**
 * privKey.cpp - Bitcoin poscript generator
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


#include "bigint.h"
#include "privKey.h"
#include "util.h"






class ChildNum
{
public:
	ChildNum(const BigInt<32>& inVal) : val(inVal) {}
	ChildNum(const int inVal, const bool hardedend) : val(hardedend ? (BigInt<32>("80000000") + inVal) : inVal) {}
	
	
	std::string toString() const
	{
		if(isHardened())
		{
			const BigInt<32> limit("80000000");
			
			std::string ret;
			const BigInt<32> newVal = (val - limit);
			ret += newVal.strDec();
			ret += "H";
			return ret;
		}
		else
		{
			return val.strDec();
		}
	}
	
bool isHardened() const//const BigInt<32>& iNum)
{
	const BigInt<32> limit("80000000");
	
	if(val >= limit)
	{
		return true;
	}
	
	return false;
}
	
	
	const BigInt<32> val;
};




class DerivationPath
{
public:

	DerivationPath(const std::string& path) : 
		path(getDerivationPath(path).path), isPublic(getDerivationPath(path).isPublic) {}


	DerivationPath(const std::list<ChildNum >& inDerivationPath, const bool inIsPublic = false) : 
		path(inDerivationPath), isPublic(inIsPublic) 
	{
		for(std::list<ChildNum>::const_iterator it = path.begin() ; 
			it != path.end() ; 
			++it)
		{
			const ChildNum& num = (*it).val;
			
			if(isPublic && num.isHardened())
			{
				throw std::string("Error, hardened path not possible for public extended key");
			}
		}
			
	}


	DerivationPath getPub() const
	{
		//const std::list<ChildNum> path;
		
		const DerivationPath ret(path, true);
		return ret;
	}
	
	static std::string expandShorthands(const std::string& givenpath);
	

	static std::string getCurrencyDerivationPath();
	
	DerivationPath withoutLast() const
	{
		if(path.empty())
		{
			throw std::string("Can not remove from empty path");
		}
		
		std::list<ChildNum> newPath = path;	
		newPath.pop_back();
		
		const DerivationPath derivationPath(newPath, isPublic);
		return derivationPath;
	}
	
	ChildNum getLast() const
	{
		if(path.empty())
		{
			throw std::string("Can not get last child num in empty list");
		}
		
		return path.back();
	}

	
	std::string toString() const
	{
		std::string ret;		
		ret += (isPublic ? "M" : "m");
		
		for(std::list<ChildNum>::const_iterator it = path.begin() ; 
			it != path.end() ; 
			++it)
		{
			const ChildNum& num = (*it).val;

			ret += "/";			
			ret += num.toString();
		}
		
		return ret;
	}

private:
	static ChildNum getPart(const std::string& path, int& pos);
	static DerivationPath getDerivationPath(const std::string& path);

public:

	const std::list<ChildNum> path;	
	const bool isPublic;
};







class ExtendedKey
{
public:
	ExtendedKey(const BigInt<8>& inDepth, const BigInt<32>& inFingerprint, const BigInt<32>& inChildNum, const BigInt<256>& inChainCode,
		const BigInt<264>& inKeyData, const bool inIsPrivate) :
		depth(inDepth), fingerprint(inFingerprint), childNum(inChildNum), chainCode(inChainCode), keyData(inKeyData), isPrivate(inIsPrivate)  {}
		
	
	const BigInt<8> depth;
	const BigInt<32> fingerprint;
	const BigInt<32> childNum;
	const BigInt<256> chainCode;
	const BigInt<264> keyData;
	
	const bool isPrivate;
};




class ExtendedKeyData
{
public:
	ExtendedKeyData(const int inDepth,const BigInt<32>& inFingerprint, const BigInt<32> inChildNum, 	
		const BigInt<256> inChainCode) : 
	depth(inDepth), fingerprint(inFingerprint), childNum(inChildNum), chainCode(inChainCode) {}

	std::string toInfoString() const;
	
	
	BigInt<32> realChildnum() const
	{
		const BigInt<32> hardened("80000000");
		const BigInt<32> realChildnum = (childNum >= hardened) ? (childNum - hardened) : childNum;
		return realChildnum;
	}

	bool isHard() const
	{
		const BigInt<32> hardened("80000000");
		
		if(childNum >= hardened)
		{
			return true;
		}
		else
		{
			return false;
		}
	}



	std::string chainString() const
	{		
		std::string ret;
		ret += "m";
		for(int i = 0 ; i < depth ; i++)
		{
			ret += "/";
		}

		ret += realChildnum().strDec();
		if(isHard())
		{
			ret += "H";		
		}
		else
		{
			ret += "S";
		}
		
		return ret;
	}
	
	
	
	const int depth;
	const BigInt<32> fingerprint;
	const BigInt<32> childNum;
	const BigInt<256> chainCode;
};


class ExtendedPublicKey
{
public:
//Todo: Make ctor instead
	static ExtendedPublicKey getExtended(const std::string xub, const bool genericFormat = false);


	ExtendedPublicKey(const int inDepth,const BigInt<32>& inFingerprint, const ChildNum inChildNum, 	
		const BigInt<256> inChainCode, const PubKey& inPubKey) : 
			extendedData(inDepth, inFingerprint, inChildNum.val, inChainCode), pubKey(inPubKey) {}

	std::string infoString() const;

	std::string toString(const bool currencySpecific = false) const;
	
	ExtendedPublicKey derive(const DerivationPath& derivationPath) const;

	ExtendedPublicKey getWithoutDeptAndParent() const
	{
		const BigInt<32> zero;
		const ExtendedPublicKey cleanKey(0, zero, extendedData.childNum, extendedData.chainCode, pubKey);
		return cleanKey;
	}
	
	BigInt<32> getFingerPrint() const;
		
	
	ExtendedKeyData extendedData;
	
	const PubKey pubKey;	
};



class ExtendedPrivateKey
{
public:

	//Todo: Use genericFormat = true as default (Rename to currencySpecific ?)
	static ExtendedPrivateKey getExtended(const std::string xpriv, const bool genericFormat = false);

	ExtendedPrivateKey(const int inDepth,const BigInt<32>& inFingerprint, const ChildNum inChildNum, 	
		const BigInt<256> inChainCode, const PrivKey& inPrivKey) : 
			extendedData(inDepth, inFingerprint, inChildNum.val, inChainCode), privKey(inPrivKey) {}

	std::string infoString() const;
	std::string toString(const bool currencySpecific = false) const;

	ExtendedPrivateKey derive(const DerivationPath& derivationPath) const;

	ExtendedPublicKey getPub() const;
	BigInt<32> getFingerPrint() const;


	ExtendedKeyData extendedData;
	
	const PrivKey privKey;
	
	
};


ChildNum derivationStringToNum(const std::string& numStr);


ExtendedKey decodeHD(const std::string& extended, const bool genericFormat = false);

ExtendedPrivateKey decodePrivateHD(const ExtendedKey& extKey);
ExtendedPublicKey decodePublicHD(const ExtendedKey& extKey);

std::string calcHmacSha512(const std::string& Kin, const std::string& m);


ExtendedPrivateKey CKDpriv(const ExtendedPrivateKey& ek, const ChildNum& i);
ExtendedPublicKey CKDpub(const ExtendedPublicKey& pubKey, const ChildNum& iNum);


ExtendedPrivateKey calculateMasterKey(const std::string& privKey);

#endif
