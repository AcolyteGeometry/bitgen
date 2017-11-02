#ifndef SCRIPT_DECODER_H
#define SCRIPT_DECODER_H

/**
 * transaction.h - Bitcoin poscript generator
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


#include <string>
#include <iostream>

#include "readStream.h"

#include "ScriptItem.h"
#include "Script.h"



class RSAndPubkey
{
public:
	RSAndPubkey(const RSVal inRs, const PubKey inPubkey) : rs(inRs), pubkey(inPubkey) {}
	
	const RSVal rs;
	const PubKey pubkey;
};



class UnsignedMultisig
{
public:
	UnsignedMultisig(const int numReq, const int numTotal, const std::list<PubKey>& pubkeys) :
		numReq(numReq), numTotal(numTotal), pubkeys(pubkeys) {}
	
	const int numReq;
	const int numTotal;
	const std::list<PubKey> pubkeys;
};


class ScriptDecoder
{
public:
	static RSAndPubkey decodeSigAndPubkeyScript(const std::string& script);
	
//	static std::string scriptToAddress(const BinaryScript& script);
//	static std::string scriptTypeString(const BinaryScript& script);
	
	static RSVal decodeSignatureRS(const std::string& str);

	static RSVal decodeSignature(ReadStream& stream);
	static RSVal decodeSignatureRS(ReadStream& stream);
	
	static PubKey decodePubkey(ReadStream& stream);
	//static PubKey decodePubkey(const std::string& str);

	
	
	static std::string printable(const std::string& str);	
	
private:
	
};



#endif
