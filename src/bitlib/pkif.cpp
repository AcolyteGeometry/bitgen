/**
 * pkif.cpp
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
 

#include "pkif.h"

#include "base58.h"
#include "pubKey.h"


bool operator==(const Pkif& a, const Pkif& b)
{	
	return (a.pkif == b.pkif);
}




PubKey Pkif::getPubKey() const
{
	bool compressed(false);
	bool onlyIndicatedUsage(false);
	bool signVerify(false);
	
	const ECPoint point = parsePkif(pkif, compressed, onlyIndicatedUsage, signVerify);
	PubKey pubkey(point, compressed);
	return pubkey;
}





bool Pkif::isPkif(const std::string& pkif)
{
	try
	{
		bool compressed(false);
		bool onlyIndicatedUsage(false);
		bool signVerify(false);
		
		parsePkif(pkif, compressed, onlyIndicatedUsage, signVerify);		
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}


const ECPoint Pkif::parsePkif(const std::string& pkif, bool& compressed, 
	bool& onlyIndicatedUsage, bool& signVerify)
{	
	const BigInt<512> decoded = Base58Decode<512>(pkif);	
	//std::cout << "Decoded: " << decoded << std::endl;
	
	//const BigInt<512> checkSum = calculateChecksum(decoded);
		
	//Take the last four bytes
	const BigInt<512> lastFour = decoded.returnMaskHighBits(32);
	
	const BigInt<512> fourBytesDropped = decoded >> 32;
	//std::cout << "Four bytes dropped: \n" << fourBytesDropped << std::endl;

	const BigInt<264> toHash(fourBytesDropped); 
	const std::string toHashStr = toHash.getBinaryLong(); //getBinaryVal(true);

	 
	const std::string hashedAgain = doubleSha256(toHashStr);

	const std::string firstFourBytes = hashedAgain.substr(0,4);
	const BigInt<512> checkSum(firstFourBytes, 'c');
	//std::cout << "Checksum: " << checkSum << std::endl;

	if(lastFour != checkSum)
	{
		
		std::string errMsg("Incorrect pkif checksum: ");		
		errMsg += pkif;
		errMsg += " Stated value: ";
		errMsg += hexString(lastFour.getBinaryLong() /*getBinaryVal()*/);
		errMsg += " Computed value: ";
		errMsg += hexString(checkSum.getBinaryLong() /*getBinaryVal()*/);
		
		throw errMsg;
	}


	const BigInt<512> highByte = decoded >> 288;
	//std::cout << "High byte: " << highByte << std::endl;


	const BigInt<512> highFourBits = highByte >> 4;
	

	const BigInt<512> highfourByteShould(0x0C);
	
	if(highFourBits != highfourByteShould)
	{
		std::cout << "High bits: " << highFourBits << std::endl;
		std::cout << "Should be: " << highfourByteShould << std::endl;
		
		throw std::string("Incorrect high byte");
	}
	

	//const bool 
	onlyIndicatedUsage = highByte[3];
	//std::cout << "Only indicated usage: " << (onlyIndicatedUsage ? "Yes" : "No") << std::endl;


	//const bool 
	signVerify = highByte[2];
	//std::cout << "SV             : " << (signVerify ? "SignVerify" : "Encrypt") << std::endl;


	compressed = highByte[1];
	//std::cout << "This is: " << (compressed ? "Compressed" : "Non-compressed") << std::endl;

	const bool parityOdd = highByte[0];


	
	const BigInt<1024> x(fourBytesDropped.returnMaskHighBits(256)); 
	//std::cout << "x: " << x << std::endl;

	BitcoinEllipticCurve ec;
	const ECPoint p = ec.recoverPointFromXAndParityOdd(x, parityOdd);
	
	//std::cout << "Recovered point: " << p << std::endl;
	
	return p;
}

