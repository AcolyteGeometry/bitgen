/**
 * bitcoin.cpp - Bitcoin address generator
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
 
 
#include "VanitySearcher.h" 
#include "bitcoinKeyPair.h" 
#include "mnemonic.h"
#include "GenerationParameters.h"
#include "GenerationMethods.h"
#include "currencySpecific.h"
#include "base58.h"
#include "util.h"
#include "randomUtils.h"

#include <time.h>
#include <list>



std::string VanitySearcher::repeat(const char c, const int num)
{
	std::string ret;
	for(int i = 0 ; i < num ; i++)
	{
		ret += c;
	}
	
	return ret;
}






const std::vector<std::string> VanitySearcher::addFirstChar(const std::string first, const std::vector<std::string>& permutations)
{
	std::vector<std::string> ret;
	for(std::vector<std::string>::const_iterator it = permutations.begin();
		it != permutations.end();
		++it)
	{
		const std::string& str = (*it);
		const std::string full = first + str;
		ret.push_back(full);
	}
	
	return ret;
}



void VanitySearcher::generateVanityAddress(const PrivKey& startPointPriv, const RangeSearcher& rangeSearcher,
	const bool compressed, const std::string& searchPatternRaw, const std::string& thetime,
	const std::string& accountName, const BigInt<256> estimatedTriesBig)
{
	const long int estimatedTries = estimatedTriesBig.intVal();	//Todo: Use BigInt<256>!!
	
	std::cout << "Initializing generator" << std::endl;
	BitcoinEllipticCurve myec;
	
	const ECPoint PublicKeyRoot = startPointPriv.point(myec);

		
	std::cout << "Starting search" << std::endl;

	ECPoint myPublic(PublicKeyRoot);

	const long int startTime = time(NULL);


	for(long int i = 1 ;; i++)
	{
		myPublic = myec.addGenPoint(myPublic);

		const std::string uncompStr = myPublic.getRaw(false); //getRawNonCompressed();
		const std::string theSha = sha256(uncompStr);
		const std::string theRipe = ripemd160(theSha);
		const BigInt<256> ripe(theRipe, 'c');
		
		//std::cout << "RIPE: " << hexString(theRipe) << std::endl;
		//std::cout << "Ripe      : " << ripe << std::endl;

		if(rangeSearcher.isInRange(ripe))
		{
			const PubKey p(myPublic, compressed);
			const PubAddress pub = p.getAddress();			
			//const PubAddress pub = PubAddress::bitcoinAddresFromPubkey(myPublic, compressed);
			
			
			const long int endTime = time(NULL);
			const long int totalTime = (endTime - startTime);
			const int iterationsPerSec = (totalTime == 0) ? 0 : (i / totalTime);
			
			std::cout << std::endl;
			std::cout << "Found solution: " << pub << std::endl;
			std::cout << " while searching for: " << searchPatternRaw << std::endl;
			if(iterationsPerSec != 0)
			{
				std::cout << "Iterations per second: " << iterationsPerSec << std::endl;
			}
			std::cout << std::endl;

			const BigInt<257> N(BitcoinEllipticCurve::Nval());
				
			const BigInt<257> privKeyRLarger(BigInt<257>::asPositive(startPointPriv.key));
			std::cout << "The raw priv key: " << privKeyRLarger << std::endl;
			const BigInt<257> privKeyTotal = privKeyRLarger + i;
			const BigInt<257> privKeyMod = privKeyTotal % N; 	//Modulus
			const BigInt<256> privKey(privKeyMod);
				
			const PrivKey myPrivKey(privKey, compressed);
				
			const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey);

			std::cout << "PRIV: " << keypair.privaddr << std::endl;			
			std::cout << "PUB : " << keypair.pubaddr << std::endl;
				
			std::list<ExtraInfo> generationExtra;
				
			const Mnemonic myMnemonicObj(privKey);
				
				
			GenerationParameters params(compressed, true, true);				
			GenerationMethods::saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), "vanity", 
				generationExtra, thetime, params,				
				myMnemonicObj
				);			
							
			break;
		}
		
		if(i % 1000 == 0)
		{
			const long int thisTime = time(NULL);
			const long int elapsed = (thisTime - startTime);
			
			
			double it = i;
			double theDiff = estimatedTries;
			double frac = 100.0 * it / theDiff;
			std::cout << i << " Frac: " << frac << " %   ";
			
			if(elapsed > 10)
			{
				const int remainingTests = (estimatedTries - i);
				const int triesPerSecond = i / elapsed;
				
				
				const int remainingSeconds = (remainingTests / triesPerSecond);
				std::cout << "Est remaining time (s): " << remainingSeconds;
				if(remainingSeconds > 3600)
				{
					const int remainingHours = remainingSeconds / 3600;
					std::cout << " Hours: " << remainingHours;
				}
				
			}
			std::cout << std::endl;
		}		
	}
}





//Todo: Move to separate file for vanity generation
void VanitySearcher::generateVanityAddressPool(const Pkif& givenPkif, const PrivKey& priv, const RangeSearcher& rangeSearcher,
	const std::string& searchPatternRaw, const std::string& thetime,
	const std::string& accountName, const BigInt<256>& estimatedNumTries) //long int estimatedTries)
{
	BitcoinEllipticCurve myec;
	
	PubKey pubKey = givenPkif.getPubKey();
	const ECPoint givenPub = pubKey.point;
	
	std::cout << "Initializing generator" << std::endl;
	const ECPoint thePublic = priv.point(myec);
	
	const ECPoint PublicKeyRoot = myec.add(thePublic, givenPub);
		
	std::cout << "Starting search" << std::endl;

	ECPoint myPublic(PublicKeyRoot);

	const long int startTime = time(NULL);


	for(long int i = 1 ;; i++)
	{
		myPublic = myec.addGenPoint(myPublic);

		const std::string uncompStr = myPublic.getRaw(false); //getRawNonCompressed();
		const std::string theSha = sha256(uncompStr);
		const std::string theRipe = ripemd160(theSha);
		const BigInt<256> ripe(theRipe, 'c');
		
		//std::cout << "RIPE: " << hexString(theRipe) << std::endl;
		//std::cout << "Ripe      : " << ripe << std::endl;

		if(rangeSearcher.isInRange(ripe))
		{
			const PubKey p(myPublic, pubKey.compressed);
			const PubAddress pub = p.getAddress();			
			
			//const PubAddress pub = PubAddress::bitcoinAddresFromPubkey(myPublic, pubKey.compressed);
			
			
			const long int endTime = time(NULL);
			const long int totalTime = (endTime - startTime);
			const int iterationsPerSec = (totalTime == 0) ? 0 : (i / totalTime);
			
			std::cout << std::endl;
			std::cout << "Found solution: " << pub << std::endl;
			std::cout << " while searching for: " << searchPatternRaw << std::endl;
			if(iterationsPerSec != 0)
			{
				std::cout << "Iterations per second: " << iterationsPerSec << std::endl;
			}
			std::cout << std::endl;

			const BigInt<257> N(BitcoinEllipticCurve::Nval());
				
			const BigInt<257> privKeyRLarger(priv.key); //privKeyR);
			const BigInt<257> privKeyTotal = privKeyRLarger + i;
			const BigInt<257> privKeyMod = privKeyTotal % N; 	//Modulus
			const BigInt<256> privKey(privKeyMod);
				
			const PrivKey myPrivKey(privKey, pubKey.compressed);
			const Wif wif = myPrivKey.getWif();
			std::cout << "The intermediate privkey: " << myPrivKey << std::endl;
			std::cout << "Result WIF: " << wif.toString() << std::endl;
							
			break;
		}
		
		if(i % 1000 == 0)
		{
			const long int thisTime = time(NULL);
			const long int elapsed = (thisTime - startTime);
			
			
			//double it = i;
			//double theDiff = estimatedTries;
			const BigInt<256> numTries(i); //estimatedNumTries
			const BigInt<256> hundred(100);
			const BigInt<256> frac = hundred * numTries / estimatedNumTries; //theDiff;
			std::cout << i << " Frac: " << frac.strDec() << " %   ";
			
			if(elapsed > 10)
			{
				const BigInt<256> remainingTests = (estimatedNumTries - numTries);
				const BigInt<256> triesPerSecond(i / elapsed);
				
				
				const BigInt<256> remainingSeconds = (remainingTests / triesPerSecond);
				std::cout << "Est remaining time (s): " << remainingSeconds;
				const BigInt<256> secondsPerHour(3600);
				if(remainingSeconds > secondsPerHour)
				{
					const BigInt<256> remainingHours = remainingSeconds / secondsPerHour;
					std::cout << " Hours: " << remainingHours;
				}
				
			}
			std::cout << std::endl;
		}		
	}
}








bool VanitySearcher::tryAddPatternRange(RangeSearcher& rangeSearcher, 
	const std::string& startAdr, const std::string& endAdr)
{	
	const BigInt<256> one(1);
	const BigInt<256> maxVal160Bits((one << 160) - one);
	
	
		try
		{
		//Remove net version from number
		const char netVersion = getNetVersion();
		const BigInt<256> netVer(netVersion);
		const BigInt<256> netVerShift(netVer << 192);
		//std::cout << "Net version: " << netVer << std::endl;
		//std::cout << "Net ver shift: " << netVerShift << std::endl;
			
		const BigInt<256> startHashZZ = Base58Decode<256>(startAdr);
		const BigInt<256> startHashNoNet = startHashZZ - netVerShift;
		//std::cout << "StartNoNet: " << startHashNoNet << std::endl;		
		//std::cout << "StartHashZ: " << startHashZZ << std::endl;
		
		const BigInt<256> startHash = startHashNoNet >> 32;

		const BigInt<256> endHashZ = Base58Decode<256>(endAdr);
		const BigInt<256> endHashNoNet = endHashZ - netVerShift;		
		const BigInt<256> endHash = endHashNoNet >> 32;
		
		
		//std::cout << "===Start hash: " << startHash << std::endl;
		//std::cout << "===End hash  : " << endHash << std::endl;

		if(endHash > maxVal160Bits)
		{
			//std::cout << "Skipping: \"" << thisPattern << "\" gives no valid addresses" << std::endl;
			
			//std::cout << "EndHash: " << endHash << std::endl;
			//std::cout << "Max    : " << twoPowerOneSixty << std::endl;
		
			//std::cout << "INCORRECT: " << thisPattern << std::endl;
			return false;
			//throw std::string("Error, incorrect address start");
		}

//			std::cout << "Adding pattern: " << thisPattern << std::endl;
			rangeSearcher.add(Range(startHash, endHash));
			return true;
		}
		catch(const std::string& err)
		{
			//std::cout << "Can not use pattern: " << thisPattern << " " << err << std::endl;
		}
		
	return false;	
}




void VanitySearcher::searchForVanityAddress(const std::string& searchPatternRaw, const bool usePool,
	const std::string& pkifStr, const std::string& thetime, const std::string& accountName)
{
	if(searchPatternRaw.empty())
	{
		throw std::string("Empty search pattern not allowed");
	}

	const char specifiedFirstChar = searchPatternRaw[0];
	
	if(!possibleFirstChar(specifiedFirstChar))
	{
		std::cerr << "Incorrect first char: " << specifiedFirstChar << std::endl;
		throw std::string("Incorrect first char");
	}
		
	std::string searchPattern = toLower(searchPatternRaw);
	std::cout << "Specified first char: " << specifiedFirstChar << std::endl;
	
	const std::string specifiedFirstCharStr(1, specifiedFirstChar);
	std::string searchPatternEnd = searchPattern.substr(1);
	//std::cout << "THE END: " << searchPatternEnd << std::endl;

	
	
	//std::cout << "First char for coin address: " << firstCharForCoin << std::endl;
	const std::vector<std::string> permutationsEnd = RangeSearcher::permute(searchPatternEnd);
	const std::vector<std::string> permutations = addFirstChar(specifiedFirstCharStr /*firstCharForCoin*/, permutationsEnd);
	
	
	
	RangeSearcher rangeSearcher;

	const BigInt<256> one(1);
	const BigInt<256> maxVal160Bits((one << 160) - one);
	
	//std::cout << "Largest val: " << maxVal160Bits << std::endl;

	for(std::vector<std::string>::const_iterator it = permutations.begin() ;
		it != permutations.end() ;
		++it)
	{
		const std::string& thisPattern = (*it);
		//std::cout << "This pattern: " << thisPattern << std::endl;
		
		const std::string startAdr(thisPattern + repeat('1', 34 - searchPatternRaw.size()));
		//std::cout << "StartAddr: " << startAdr << std::endl;
		
		const std::string endAdr(thisPattern + repeat('z', 34 - searchPatternRaw.size()));                                                
		//std::cout << "EndAddr  : " << endAdr << std::endl;

		const std::string startAdr2(thisPattern + repeat('1', 33 - searchPatternRaw.size()));
		//std::cout << "StartAddr: " << startAdr << std::endl;
		
		const std::string endAdr2(thisPattern + repeat('z', 33 - searchPatternRaw.size()));                                                

	
		if(tryAddPatternRange(rangeSearcher, startAdr, endAdr))
		{
			//std::cout << "Adding pattern: " << thisPattern << std::endl;
		}
		
		if(tryAddPatternRange(rangeSearcher, startAdr2, endAdr2))
		{
			//std::cout << "Adding pattern: " << thisPattern << " (short)" << std::endl;
		}
	}


	

	if(rangeSearcher.empty())
	{
		std::cout << std::endl;
		std::cout << "Pattern generates no valid addresses: " << searchPatternRaw << std::endl;
		std::cout << "Try another pattern, for example \"1bit\"" << std::endl;
		std::cout << std::endl;
		std::cout << "Valid base58 chars: " << std::endl;
		std::cout << "  123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz" << std::endl;
		std::cout << std::endl;
		throw std::string("Error, nothing to search for");
	}


	//std::cout << "Search ranges: " << std::endl << rangeSearcher.toString() << std::endl;

	const BigInt<256> fiftyEight(58) ;
	
	BigInt<256> rangeSum = rangeSearcher.rangeSum();
	//std::cout << "RangeSum : " << rangeSum << std::endl;
	//std::cout << "Max Value: " << maxVal160Bits << std::endl;
	
	const BigInt<256> estimatedNumTries/*theDiff*/ = maxVal160Bits / rangeSum;
	//std::cout << "The difficulty: " << theDiff.intVal() << std::endl;
	
	//const BigInt<256> difficulty = fiftyEight.power(searchPatternRaw.size() - 1);
	//std::cout << "Raw difficulty: " << difficulty << std::endl;
	//const long int estimatedTries = theDiff.intVal(); // / rangeSearcher.size();
	
	std::cout << "Difficulty: " << estimatedNumTries /*estimatedTries*/ << std::endl;

	GenerationMethods::printRandomSourceWarning();

/*
	std::cout << "Warning: Random number generated outside of bitgen with /dev/random." << std::endl;
	std::cout << "         Ensure that your random number generator gives good random numbers." << std::endl;
	std::cout << "         Flaws in the random number generation " << std::endl;
	std::cout << "         might make the private key possible to guess." << std::endl;
	std::cout << "         Consider other random number generators, such as dice." << std::endl;				
*/
	
	std::cout << "Generating random bitcoin address" << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
		
		
	const std::string random = getFromDevRandom(32);
		
	const BigInt<256> privKeyR(random, 'c');

	std::cout << "Searching case-insensitive for: " << searchPatternRaw << std::endl;


	if(usePool)
	{		
		const Pkif pkif(pkifStr); // = Pkif::parsePkif(pkifStr);
		std::cout << "Using PKIF: " << pkif.toString() << std::endl;
		const PubKey pubKey = pkif.getPubKey();
		
		const PrivKey startPointPriv(privKeyR, pubKey.compressed);
		
		generateVanityAddressPool(pkif, startPointPriv, rangeSearcher,
							searchPatternRaw, thetime, accountName, estimatedNumTries /*estimatedTries*/);
		
	}
	else
	{
		const bool compressed(false);
		
		PrivKey startPointPriv(privKeyR, compressed);
		
		generateVanityAddress(startPointPriv, rangeSearcher, compressed, 
							searchPatternRaw, thetime, accountName, estimatedNumTries);
	}

}		





