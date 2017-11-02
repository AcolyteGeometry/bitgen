#ifndef BITGEN_CLI_H
#define BITGEN_CLI_H

/**
 * bitgencli.h - Bitcoin poscript generator
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

#include "privKey.h"
#include "RangeSearcher.h"


void doBitgenWork(int argc, char* argv[]);
void doBitCalcWork(int argc, char* argv[]);

void printBitgenUsage();
void printUsage();
void setLanguage(int argc, char* argv[], const std::string& thetime);
void generateBrainWallet(const std::vector<std::string>& args,
	const std::string& thetime);
void generateBulk(const std::vector<std::string>& args, const std::string& thetime);
void generateWithDice(const std::vector<std::string>& args, 
					const std::string& thetime);
void generateHash(const std::vector<std::string>& args,
	const std::string& thetime);
void generateHex(const std::vector<std::string>& args, const std::string& thetime);
void generateFromMnemonic(const std::vector<std::string>& args, const std::string& thetime);
void generateRandom(const std::vector<std::string>& args, const std::string& thetime);
void validatePrivateWif(int argc, char* argv[], const std::string& thetime);
void informationAboutEntity(const std::vector<std::string>& args, const std::string& thetime);
void verifyWif(int argc, char* argv[], const std::string& thetime);
void verifyAddress(int argc, char* argv[], const std::string& thetime);
void generateWifFromMiniPrivKey(int argc, char* argv[], const std::string& thetime);
void generateVanityAddress(/*const BigInt<256>& not_used_privKeyR, */const PrivKey& priv, const RangeSearcher& rangeSearcher,
	const bool compressed, const std::string& searchPatternRaw, const std::string& thetime,
	const std::string& accountName, const long int estimatedTries);
void generateVanity(int argc, char* argv[], const std::string& thetime, const bool usePool);
void generateVanityPoolResult(int argc, char* argv[], const std::string& thetime, const bool usePool);
void addPrivateKeys(int argc, char* argv[], const std::string& thetime);
void addPublicKeys(int argc, char* argv[], const std::string& thetime);
void multiplyPointScalar(int argc, char* argv[], const std::string& thetime);
void multiplyGenPointScalar(int argc, char* argv[], const std::string& thetime);
void addGeneratorPoint(int argc, char* argv[], const std::string& thetime);
void generateRootKey(const std::string& theTime, int argc, char* argv[]);
void generatePublicFromRoot(int argc, char* argv[], const std::string& thetime);
void generatePrivateFromRoot(int argc, char* argv[], const std::string& thetime);
void generatePostscriptKeypair(int argc, char* argv[], const std::string& thetime);
void generatePublicPart(int argc, char* argv[], const std::string& thetime);
void generatePaymentRequest(int argc, char* argv[], const std::string& thetime);
void uncompressPublicKey(int argc, char* argv[], const std::string& thetime);
void compressPublicKey(int argc, char* argv[], const std::string& thetime);
void brainwalletTests();


#endif

