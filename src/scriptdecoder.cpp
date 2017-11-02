

#include "util.h"
#include "pubKey.h"
#include "RSVal.h"

#include <stdio.h>

#include "scriptdecoder.h"
#include "cryptoDecoder.h"



RSVal ScriptDecoder::decodeSignatureRS(ReadStream& stream)
{
	const int thirty = CryptoDecoder::decodeByteInteger(stream);	
	if(thirty != 0x30)
	{
		std::cout << "Got: " << thirty << std::endl;
		throw std::string("Incorrect signature, 30 not found");
	}


	const int rsSize = CryptoDecoder::decodeByteInteger(stream);
	//std::cout << "RSSize: " << rsSize << std::endl;
	
	const int two = CryptoDecoder::decodeByteInteger(stream);
	if(two != 0x02)
	{
		throw std::string("Incorrect signature, 02 not found");
	}

	const int rSize = CryptoDecoder::decodeByteInteger(stream);
	//std::cout << "RSize: " << rSize << std::endl;
	const std::string rStr = stream.getStr(rSize);
	const BigInt<1024> r(BigInt<1024>::fromBytes(rStr));
	
	//std::cout << "R: " << hexString(rStr) << std::endl;
	//std::cout << "R: " << r.strHex() << std::endl;
	
	const int twoAgain = CryptoDecoder::decodeByteInteger(stream);
	if(twoAgain != 0x02)
	{
		throw std::string("Incorrect signature, 02 #2 not found");
	}
	const int sSize = CryptoDecoder::decodeByteInteger(stream);
	//std::cout << "SSize: " << sSize << std::endl;
	const std::string sStr = stream.getStr(sSize);
	const BigInt<1024> s(BigInt<1024>::fromBytes(sStr));
	
	//std::cout << "S: " << hexString(sStr) << std::endl;
	//std::cout << "S: " << s.strHex() << std::endl;
	
	const int one = CryptoDecoder::decodeByteInteger(stream);
	if(one != 0x01)
	{
		throw std::string("Incorrect signature, 01 not found");
	}

	const RSVal rs(r, s);
	return rs;
}




RSVal ScriptDecoder::decodeSignatureRS(const std::string& str)
{
	ReadStream stream(str);
	
	return decodeSignatureRS(stream);
}





RSVal ScriptDecoder::decodeSignature(ReadStream& stream)
{	
	const int sigSize = CryptoDecoder::decodeByteInteger(stream);	
	//std::cout << "This size: " << sigSize << std::endl;
	const std::string signature = stream.getStr(sigSize);
	
	ReadStream sigStream(signature);
	const RSVal rs = decodeSignatureRS(sigStream);
	//std::cout << "Signature: " << hexString(signature) << std::endl;
	return rs;
}


/*
PubKey ScriptDecoder::decodePubkey(const std::string& str)
{
	ReadStream stream(str);
	
	return decodePubkey(stream);
}
*/


PubKey ScriptDecoder::decodePubkey(ReadStream& stream)
{	
	const int pubkeySize = CryptoDecoder::decodeByteInteger(stream);	
	const std::string pubkey = stream.getStr(pubkeySize);	
		
	const PubKey pubKey = PubKey::fromBin(pubkey);
	
	return pubKey;
}











RSAndPubkey ScriptDecoder::decodeSigAndPubkeyScript(const std::string& script)
{
	//std::cout << "In decodeInputScriptTotal, script size: " << script.size() << std::endl;
	
	ReadStream scriptStream(script);
	
	//std::cout << "Will get rs" << std::endl;	
	const RSVal rs = ScriptDecoder::decodeSignature(scriptStream);
	
	//std::cout << "Will get pubkey" << std::endl;						
	const PubKey pubkey = ScriptDecoder::decodePubkey(scriptStream);
	
	RSAndPubkey rsAndPubkey(rs, pubkey);
	return rsAndPubkey;
}





//Todo: Move to util?
std::string ScriptDecoder::printable(const std::string& str)
{
	std::string ret;
	for(int i = 0 ; i < str.size() ; i++)
	{
		const unsigned char c = str[i];
		if(c >= 32 && c <= 128)
		{
			ret += c;
		}
		else
		{
			ret += "_";
		}
	}
	
	return ret;
}









std::string BinaryScript::scriptTypeString() const
{
	const Script theScript = Script::parse(*this);
	
	return theScript.scriptTypeString();	
}










std::string BinaryScript::scriptToAddress() const
{
	const Script theScript = Script::parse(*this);
	
	return theScript.toAddressString();
}



