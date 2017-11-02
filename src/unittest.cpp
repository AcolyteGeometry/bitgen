/**
 * unittest.cpp - Bitcoin poscript generator
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


#include "unittest.h"


#include <iostream>
#include <string>

#include "bitcoinKeyPair.h"
#include "miniPriv.h"
#include "mnemonic.h"
#include "privKey.h"
#include "base58.h"
#include "randomUtils.h"
#include "addressConversion.h"
#include "signature.h"

#include "util.h"
#include "multiSig.h"
#include "Script.h"
#include "transactiondecoder.h"
#include "transactionSignatureChecker.h"
#include "ExtendedKey.h"
#include "hmac.h"


void testSign(const Wif& myWif)
{
	//BigInt<256> privKey = myWif.toPrivKey();
	const PrivKey pk = myWif.getPriv();
	const BigInt<256> tempKey(BigInt<256>::fromHex("83738783783738738787877d7d77d7d7d8888d88d88d8d8d888d8d98a88a7a89"));
	const BigInt<256> hashNum(BigInt<256>::fromHex("1276356356636663678388888383838838383883987239872893787f9f987f88"));


	BitcoinEllipticCurve ec;

	//const PrivKey p(privKey, compressed);
	const PubKey pub = pk.getPubkey(ec);	
	const ECPoint pubkey = pub.point;



	//Todo: Remove function getPublicPointFromPrivateBinary
	//const ECPoint pubkey = getPublicPointFromPrivateBinary(pk.key); //privKey);
	const PubKey p(pubkey, false);
	const PubAddress pubAddr = p.getAddress();
	//PubAddress pubAddr = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
	
	std::cout << "Test sig for: " << pubAddr;
		
	const RSVal rs = SignatureImplementation::sign(pk.key, tempKey, hashNum);
		
	//Verify signature to make sure it is valid		
		
		
	if(!SignatureImplementation::verifySign(rs, pubkey, hashNum))
	{
		std::cout << " Error in signature!!!!!" << std::endl;
		throw std::string("Error in signature!!!!!");
	}
	
	std::cout << "      OK" << std::endl;
}



void testGenerateAddresses(const BigInt<256>& privKeyR, const ECPoint& PublicKeyRoot)
{
	const bool compressed(true);
	
	for(int i = 1 ; i < 10 ; i++)
	{
	//Calculate   point( (parent_private_key + i) % G ) == parent_public_key + point(i)
			
			
	//Calculate the priv from the priv:

	const BigInt<256> privKey = privKeyR + i; //Todo: Modulus!!!!!!!
	
	const PrivKey myPrivKey(privKey, compressed);
	
	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKey, compressed);

	std::cout << "Test: " << i << std::endl;
	std::cout << "PRIV: " << keypair.privaddr << std::endl;			
	//std::cout << "PUB1: " << keypair.pubaddr << std::endl;

	BitcoinEllipticCurve myec;
		
	//Calculate the public from the public:
	const BigInt<256> pubAdd(i);
	const PrivKey myPublicPriv(pubAdd, compressed);
	const ECPoint PublicKeyAdd = myPublicPriv.point(/*pubAdd*/ myec);
			
	const ECPoint myPublic = myec.add(PublicKeyRoot, PublicKeyAdd);
	const PubKey p(myPublic, compressed);
	const PubAddress pub2 = p.getAddress();
	//PubAddress pub2 = PubAddress::bitcoinAddresFromPubkey(myPublic, compressed);
	if(keypair.pubaddr != pub2)
	{
		throw std::string("Internal error, public keys differ");
	}

	std::cout << "PUB: " << pub2 << std::endl;
		
	//std::list<std::string> generationExtra;
		
	//saveAllFiles(keypair.pubaddr.address, keypair.privaddr.address, "computer random generator", generationExtra, thetime, accountName);
	std::cout << std::endl;
	}
}




void testPrivToAddress(const PrivKey& myPrivKey, const std::string& myAddress)
{
	BitcoinEllipticCurve ec;
	
	const PubKey pubKey = myPrivKey.getPubkey(ec);
	const PubAddress pubAddr = pubKey.getAddress();


	if(pubAddr.toString() /*.address*/ != myAddress)
	{
		std::cout << "The pubaddr: " << pubAddr << std::endl;	
		std::cout << "Other addr : " << myAddress  << std::endl;	
			
		throw std::string("Error in address derivation");
	}
}




void testPrivToPkif(const PrivKey& myPrivKey, const Pkif& myPkif)
{
	BitcoinEllipticCurve ec;
	
	const PubKey pubKey = myPrivKey.getPubkey(ec);
	const Pkif pkif = pubKey.getPkif();
	


	if(pkif.toString() != myPkif.toString())
	{
		//std::cout << "The pubaddr: " << pubAddr << std::endl;		
		throw std::string("Error in pkif derivation");
	}
}


void printUnitTestCode(const BitcoinKeyPair& keypair)
{
	std::cout << std::endl;
	const PrivKey pk = keypair.privaddr.getPriv();
	const BigInt<257> privkey(BigInt<257>::asPositive(pk.key)); //keypair.privaddr.toPrivKey());
	
	std::cout << "\t\tconst BigInt<256> privKey12(\"" << privkey << "\");\n";
	std::cout << "\t\tconst std::string privVal12(\"" << keypair.privaddr << "\");\n";
	std::cout << "\t\tconst std::string pubVal12(\"" << keypair.pubaddr.toString() /*.address*/ << "\");\n";		
	std::cout << "\t\ttestGeneration(privKey12, privVal12, pubVal12);\n";
	std::cout << std::endl;
}




void testAddressGeneration(const std::string& wifStr, const std::string& address)
{
	std::cout << "Will test address generation... " << std::flush;
	const Wif wif(wifStr);	
	const PrivKey privKey = wif.getPriv();
	
	BitcoinEllipticCurve ec;	
	const PubAddress pubAddr = privKey.getAddress(ec);

	if(pubAddr.toString() != address)
	{
		throw std::string("Address creation error");
	}
	
	std::cout << address << " OK" << std::endl;

}


void testAddressGen()
{
	testAddressGeneration("5JBy6LUc7LprUf3HqCsdCkP2M3tWAofHyQTomx26iVEzWY75JeB", 
						"1EA1aFSu4wxVLVaDguJUX5j1jih2NhP5ed");
	testAddressGeneration("5J3X9hXTGFH3vkAhGKVDAbk6nYDioXFuMjv8Rac97mPxBRH8nqv",
						"1FYKRgMizxog8Z8Wgrew1734ShZD3iUGr5");
	testAddressGeneration("5Kb4FjszWwVX445Vz2cpeRgiPLWsVwRyEBKMXVwm93WVtUykhgA",
						"1AMu1gYVMdT6d1E2KFK7wZNJt5K9tV2pzc");
	testAddressGeneration("5JeX9dMwqB46T28MoMJnWhiQsaNXVwoYA92iXTcW1mtzLDvTMn1",
						"12ZoxV3okXBcQfkB2UuQmYNzaWN1FEfEHe");
	testAddressGeneration("5KbWqgk43NVNpATDT1vRfgGjKW6AgUr9UAPokCP7SXKAKHAhvVS",
						"1Go9WCLF9F5RMY8M8XvbGp7QefoM2cxSeq");
	testAddressGeneration("5KJG8vFDfs1kmLfWZiKSbCGh4CPJipRSDCFyb35X9WHgrBic793",
						"1LJSWv4YTGTMe9Mi3UuVYHPMLFiGgg1G8U");
	testAddressGeneration("5HqXc9UzHPFy1Fd3s9V6Vck5Twn1qhcKvNz8mGLzciaD31sQeGd",
						"1JeZKtmoGH5wjwjogxGzwFx6oVRQGwyTDB");
	testAddressGeneration("5HttQrUtZQyXxSyHWAuWMrXXLh4rPR8FfqfgRrigmwDgJ2dhLcZ",
						"1676ztjkjd7dBDvrqaNL8HVdDvtWMEDJDu");
	testAddressGeneration("5KjMysk3nFhLFkAqxE1GrR3HMRDt1DNxF9pxTfMnrBe4SHgsGrt",
						"1APTjgLq6cV37KamiMbP5d4Q17QRS9ATKq");
	testAddressGeneration("5Hv3EfQnmUqMUC3ZJDhKGJ52UTXEeeanETZfGiDs4iWrnQVrT13",
						"1EMbjZpADPt88KX7noN6mWjGnuQTAUTeUC");
	testAddressGeneration("5JZsewmB7kbd8XicGc397db5vebqfb4Pjo7m778qeFdLTAczqjM",
						"1LN6XwVZ41BepexZq7rd8hm5EFktvKatLX");
	testAddressGeneration("5JYpFYmqZNUY9nkX8BPZrRUWsRa37WCb9Lo5HLijrkUbs8fqPpd",
						"1FQX9XpQ3coaLBU8mof1ZtsEM8CTXrXSfx");
	testAddressGeneration("5J4eCt6g9iZmPbB55zfMzpLvg15iekdHYpqALM6Vt1hcToa68Fz",
						"1GKMF2m1SwYNbv1X5f7zjcP9sGQhqNehqP");	
	testAddressGeneration("5KN3R5v4p5k2oifXaosD2P12uYUHJnLdpspdHRjR2Yqjgn5jkhU",
						"1Baf2n5UYBYe9cAYi67HnaScXUnVmtZJrM");
	testAddressGeneration("5JdTzsqo132vwamdWYgLeAHjFR3hzf1begoBVquN71zXy1Y1HNB",
						"1MYxfi529TuEsi8YGKe8cY311aZSas2azR");
	testAddressGeneration("5Joffve52mgttHXpfQrf5uj2NmHzBzkyHLDt967Kh8bdkzMkVJg",
						"14BJCy7W1JRfDfD2NdBhd2Y9Jj78aNfVyZ");
	testAddressGeneration("5JZWDX1AEiGdDqUvARsr6Vjyszx9FRE394CHakAt7YyXW66BZBj",
						"12vPaAWsbwNQr2KEBBBDCJiMuA5SppNGD5");
	testAddressGeneration("5Jr1Ds46mxSHDfbCCfJGtsYDQwUBSDr7SXyPECxUzRodcuzNt2K",
						"1KUt9P8X9PQfBNfabLEMack8vQbNAm8fXs");
	testAddressGeneration("5K25h2Can9qgJbf7fpc5otCvowX6BCxBrvx1Mu7V6Jc9tf8C9hY",
						"16rWatXxomg3zE7s7rJnQRaHcSY51yCPze");
	testAddressGeneration("5J3bscQYxpUxbsMesJmrDyKwxE4THv1GGbrGz8n2TgKJvznpkT7",
						"1CBuhzwgs4b9nFNGf4EstDbUUdG9BSQYEP");
	testAddressGeneration("5JbZRp9fEWCSyV3PzKoU35GnjWLo9JDCL7YmPfT3FgUk5G48Rd4",
						"1KS99LvdE4r4EUPASSuSpzrcMb7bbhHfHh");
	testAddressGeneration("5J8mfjANXjw7LMHKYtGQhWtQg99T1WxWAt1bLckEgpuLNDZ6kUW",
						"1HLwZ7SSzRkJPjnQQjEH7VKbADNCa8k4qt");
	testAddressGeneration("5JsFNiHUReY4FAUDmXx33vQaAXQ71V7Wm2iCX4Loy2CRpaRfswD",
						"1E5KfmM1sPfmcGcgqwBRJf2zXrxZHmxR6r");
	testAddressGeneration("5JHjq9wGe24tbDaPPyXdpH9cMVdWvmaaSnni3Apb7F1e8StheKX",
						"1GKErcnu6xrfqhyTw17bLBTXYUz5hE8yYj");
	testAddressGeneration("5KPxqHESZg3zumnECksCitR3Ru22KxFRRwyuyzVJTxL77tJJBT8",
						"15cb65GZWX74XoEF8QUhzV6K9MJZcHYz3E");
	testAddressGeneration("5HvPjQsKHk4ebJPf6P5bCEQSWyrdXh1AsfSVV5UShHqyqeZBEPC",
						"1DGVLjFuCBN4ifQU562Tz9UQZQvoMPNhRJ");
	testAddressGeneration("5JgsUmTuvmFiZCHdUA8wmzFkNDBib8ibtGgYHc1sBSTiypcR2Hx",
						"17LfMLieVTN8dUxW8G32FLySWieDZKzjwz");
	testAddressGeneration("5K4CK1rTKQGa73T9P2HyCuGmFCvXc9T2JJv493Rd2EGPXqJzxm6",
						"189nfdCdDH6Zok3M3t8kk58AFPhhARCVt9");
	testAddressGeneration("5JbwjpkGaK7mXf1FeU2vMksYtG5rAJk36XrpHoUBxNKU5iXjUgU",
						"19YZB6L5WU1NkEXGi1gS63QrYxgGXLwhtL");
	testAddressGeneration("5KDMRLSDoFrcA3Ey3Sevhu5p359oFNK7CmFzj9bk5LTRMDY4i9H",
						"1BaHCZ24BDfPuEkf2gpQ2KYec88Buq8Gx3");
	testAddressGeneration("5KB5hGDjotemefDjGoZYWaZMcscvRn3YuAehQKRip1JCLUwTa4T",
						"1BTDHRHnMoVng4EfQh1sBVaVmpyv34PCxc");
	testAddressGeneration("5JKtnL7ndUZAdubYwDuvgjLkuZUJZwwhGcPwfeC5jjbqcJPw55f",
						"12vPQFCJHVV5UXdEcbu9SAVeP96z7PkzNi");
	testAddressGeneration("5JJ61RoTPBEsyHswe4qJgKbVMzLiNJu77iFBf5f7xq9x5QpU8Ly",
						"1FeqxWbjs8VrPL7F74uQ4eZoiK8A88VpD5");
	testAddressGeneration("5JdkTFhLF5WnSu6Jd1jHTuShG8VBXxGAi1bUaYtBmR5maWa7pvj",
						"1NKMrbukJVvs9cpMoRzrUZMqtmTRDC4h3q");
	testAddressGeneration("5Kjzfpf78xRdwe8KYKq9Uew9TKkDvRZLNxTCTHQcHryvhy4NYLF",
						"14ouCaUazAc96DBgdrsb9jNkjjqZUVr8KW");
	testAddressGeneration("5J8nbdN1QcsuQmFVXicX33Coev77cJbj4NtLtqQvWQR1dfnZCtA",
						"1Lxj8Pagz1hQwmwXn9boHLdVi5aK4jAdiU");
	testAddressGeneration("5JSmyHuyBKwGcUNJpeDrVhhYHbLSQKg6t6LHjCc9vfADr48kFqe",
						"17R6YtXeD898fTjeWP6VLn7TqhmBK7D8Jw");
	testAddressGeneration("5Kb7Fsy4icSYJ7hr7gpbfFsBZJVA8LAcVMNwV3QoW84tVXDxqM8",
						"16wxQFQCAo37R1wpczpQeFVwSKQ1i748Ta");
	testAddressGeneration("5KX9wqjTPRh1oqD3mBCTsn3zwJp8Fcq4uEp4dy8HNJ4Vrak3a2X",
						"1FrfJqprNPVya6q7Py6txty4MwEBmynH8F");
	testAddressGeneration("5JG2jQ7QHxekXpcHXsKUNCWdQpL22x7i6p46Zw6HtLMrquQLBc7",
						"12QhPTJTF3FtJmBe7Cbc5rTmaCymy73SbX");
	testAddressGeneration("5JmPWWbkCt3mpo2oSVunrK8pw9wum1YXJepMvFYPygLb9HUgvzA",
						"1PmnNvQZBsTvSLrtBwJECpjSz29wv2yG18");
	testAddressGeneration("5JFFhqQiTLkWQnMW2qZGc1sQGwG2Dvb1iMV4jjGZdPPVnSkrX8D",
						"18t8kKu5L5w4XPpxFuV6X15PhXaAaG7pjV");
	testAddressGeneration("5KdGJtHERANff5M5mo46UM5VmPjgkyNUE4Yt6nTiVSkNbbpxVu1",
						"17o5TppeTBJnJqmRsShy3AZjxruuZJa2bL");
	testAddressGeneration("5JRKGdqYtHgL2GwZrvtEfH7VHDLYr6ezUrjeDq53Pebwo1eYLH1",
						"1DUiFEjA9E2xUrdTR6hzQmkvhZvELNKD9o");
	testAddressGeneration("5J4nW8vioH76JyFyvdQJiZpiJ42aL2mq6CQGdbp65yBJxwV9Egs",
						"1F9c29uKpBRgmwGWvHuRNTffJXFgz2LeSd");
	testAddressGeneration("5KkcagKGKDMgSZEqQpuwNPgbsEACkaTD8H2M3Lt5S6Lxdc9NQjw",
						"1JgPbEQKQ7JgMUMtw4oBwR4p9Sa8Vy8oLv");
	testAddressGeneration("5JQqRtHrGCQwDuMdcbeVGDBzZZTbhUHuVDuj3HnwSKnM515vy6Y",
						"15UAYG9gYkBkCAwcYQCK1jk7QobHdKKN2q");
	testAddressGeneration("5KHxj2UZ3Xj4pDTDq2JvADF5XgfSeUPh2j775est3bU43YVshs5",
						"1MRCyMMaFYRndTa4XfZhuofJYhbDPZ5QFf");
	testAddressGeneration("5KKd4Jv7LpZLg3y5VmSMkbYuYqGjaqAkcvMdN8fugjVLXePNCZt",
						"1MqCcdKmByDPrJ32rsHpkNUdP3Kj2fxs8Q");
	testAddressGeneration("5JaiLmkQ6gWihx2iR8qNanxSDXbRmEqKdpnMALz3GDhD9GRBz5C",
						"1MbKMRMKoxfRupaVT5zaeKwWrNDcUkSAM");
	testAddressGeneration("5KT8XRFHvB9RF1uRuWGSFhZ7moDqD6uk1HnuWxUZsM7VS81XZdv",
						"18Pxts7SeeR6VtP5ERamgXJwXtE7q79U8N");
	testAddressGeneration("5KE5a8wVvyb2HK6oTdjDUAVM5uwj5tJqLcbj2fu78m5j5cDJmqq",
						"14TqnpQ7EFco1tcHzPJWvgDA4NQsoiQ7Lv");
	testAddressGeneration("5KKhpxV84tAZd8ndesA12Xuigkd4BoD83a6DX93prPofGRLjm7E",
						"1FEmYhebzDsQfbxfrp6qQ4hzY21LBHzgHc");
	testAddressGeneration("5J9f43QxgcF6jMyuUioyWrkyhi78sbEbcWjJHhdmLofB8MvMTdz",
						"1JK9F5AS6VsghLhFkTSi8rdq4T9G2R2tNo");
	testAddressGeneration("5JH17zxXTQYAXEUEKJeiVHb9mrJoaPqxtNtQ4i9GnAy83AVU63P",
						"112YFLypheLvSmVi3KUugU2G3BB9ZaoZ79");
	testAddressGeneration("5JZU6UCjSXYGL4vsBU6XrwmRHxn3NxNEiKo2PdYot3Rz9q9fytK",
						"17Dv6Kpfd7B11k7ZM4v1cdxNvr6iDBU6Gv");
	testAddressGeneration("5KJ28PJmwrndbVbqkFqUtuq89SVr3mQXx4uLehhJLBvoHSJd5t2",
						"14N8hUdpEXEenP2ExNg7RCrxjEeZCRzyFe");
	testAddressGeneration("5KidhtxqxXygunnn8teha7UkGVsSp8GZa5EMaP4DZwNjyA9bkHu",
						"17m8nCSR6fe3BLViHEQosDSfRUxwAbyKf5");
	testAddressGeneration("5HwLckT7MC9McSYZxfeA9E4ftG9oMSft6kQC4424KCpb9GMjYzp",
						"1M8AynKvdEYZ2tVzAqduYrAfGrTU4d9mgp");
	testAddressGeneration("5J5oDZDT5je32entvTNdPYxCpPefUBkanvU4iLoN1ktCcgRFCjc",
						"1Hd1YsYqkSd1rVQJyzKuXRqhyi73xTC54d");
	testAddressGeneration("5K3WxZeMLopKkYVnjC665GtTMf2MirwSQ6ZJUWEFSCTa9yCK5kj",
						"14hDTHjVJM24BrjQR6Q6WyY6dwnYh42RmW");
	testAddressGeneration("5HrCJoe7ivCvHBDKxoV8UAm6ZRgdJXSo4Zg2UMA55qxD5nTsmni",
						"1EyBjuh2fXLWv99jSfiFZVjB6zfKmfRoJQ");
	testAddressGeneration("5KBgxDfbQSmD2Hm1gcAzXUTx1SQqT8awZBkgPuWS9TtpX6B8qRL",
						"1HjFMcvTFJETLeYMn4gWzexRXZMAA16mxF");
	testAddressGeneration("5K7QQMiBmppKSK3LSGtfjAfqyFavyxu99DU5dp1NhU5rkvPMvCk",
						"1bpnMzCdCp2hyrhTSY6Wm86Xei48uWwqL");
	testAddressGeneration("5JCnZrNeYinrciyR1k5GZ9fZe3zUCnAYfXwpmbmDEpHk7Srx4bS",
						"14a1JnFoLFKdfpvGsVEMMCYx6Pvyrwy68G");
	testAddressGeneration("5JgX84GHEYEyt5hg3c6L3Wa6sNFrf9qqEeu94HFwYyckytTTmgH",
						"1Fc2NvLYghrUTnxZB12naB1EvFCD3FJvNF");
	testAddressGeneration("5J1PThMeHzdrYGUWBQdXS2y7hxbWf1bWHhRgR22jbb6dgrMxcDN",
						"18WHyJkncQNr1QXCA3Lfp8zsA5jpXyjHJE");
	testAddressGeneration("5JGkKejNd6WJuMBouJ1WM7oGtompRw8tyFmRjRkiTrsK4FkSzge",
						"1AUDBUFpQ5v9n6KjehCDpDBGKQYY1ybAxC");
	testAddressGeneration("5JCgFvP4dSe7BQ4mgBmR3K8ydZn1pdpQhDPhwDj1H8QRDFkH45R",
						"18kVCjunBJifVkw7WaPUusnWmzuEUwoB8G");
	testAddressGeneration("5JYttwwAP2LoA8zGxAcZd4XxKvW8kQyBJznzdGL98Pf53JLrhSr",
						"1JxEGAqA2PMkmVVNVJ7Bo4TymE7fK6nsaV");
	testAddressGeneration("5KNNw6GDYXbWLzNMQ3P48s64S2CAMbY82tCqfGE89M33xNuW51U",
						"1sDdfm19yDRMhMaZeQbBoEFE9BewZUb2q");
	testAddressGeneration("5JFiKzCVAUAeMTRaAYTAA4r3BsLgL4p77FMCPqMHJ7Gg8NaFcft",
						"1AaSCn5jZKXzBGCNCVBEXLeayekoorid5T");
	testAddressGeneration("5JgTYLhgo9PnPTrrMbc83HuACR5kcNv8ZYpsYnPTQk1d9atmP79",
						"1Ab32ZUqHKvUZh9qSWLR9MroWGVgNAPW3H");
	testAddressGeneration("5KheAHTa2ZrKYsx6jjp9Up8BgVC3haNsEgKkyatb4tHw3gtvHJs",
						"1GCECnLTKr5x1bVWxsHhTRBfZqR8bwJuwH");
	testAddressGeneration("5K3WjETof2YmkTnmip3A4N17j7XBNG9eibUp6WC1BStvtysoztc",
						"1AdT53AYXQZ8eVvygLzhnkeS3FEL8c2F7o");
	testAddressGeneration("5JmdjuWtz2svJ9tnpPYqLLp4FTK1eyeAPMtMNMowV6frpYDXCad",
						"1JKvrmdKaocH3ZNKUhp2cX9fjaE3HZuYQr");
	testAddressGeneration("5J3NpAXZRsVAszw9aAwdtFJMGUDN6raSu5Gn5ZjTeKxuxm4ozAs",
						"12fgyD6kpqgqidfVFiDjhiZv4tcVhZFS6g");
	testAddressGeneration("5HrShPYfNz3XardM9eEUYw5GKCkTFrGjLJBC2fBzmrWMfutQahH",
						"1BroeMjyV4UN5vdzKj6D85Jv4H73kDANUw");
	testAddressGeneration("5JBMEqur51bX86wvedZjB8hFkMXVGT4JXSycPSXjwi95QcCuHp1",
						"18jsMygW28qTTMgpZAvcWhJJDPZrTaekpL");
	testAddressGeneration("5JZhpoXd1AqHFSDJDSiwCYWWxtifmNPFBc6Aj7S91D37WGZ9ZzU",
						"19goXQr5N5FvdQtbe2u7nhrVpNqvj2ufQ9");
	testAddressGeneration("5Kb4WXivXSboFGmYEhAJPWP4DnVWc5UCAS2zNdzsetTGk2p42fg",
						"1C44ArEPE9C9CUdSuFHSeHFrAaveCFrpdZ");
	testAddressGeneration("5KUH4K6nWWo3UP36wyVrVBnT4QeZRTwh3gFm9kREy1pKRYBmmDh",
						"1FGKoaCUQMRtPdWkxxVdjufFwrHteTX88G");
	testAddressGeneration("5Hyr7RmD4ne3WQS5zVfffTRWLAE5usqBKPCKysoXfk1cdp1F6nu",
						"1PfaYAFvxTgHDQhB6yAfVS9pbCYWrpWUcg");
	testAddressGeneration("5J8CzkkxtdJegS71ZFMiF1nuHkdnnTBzLMZcQGJEAiC3cpg4pX8",
						"139gjNshw3pDEXUvbXafKtH8oewbmmB58o");
	testAddressGeneration("5K4JxsV9gDBUPWCsJqiEueJxiMWo5LXAN3b8BsAGw7Mwaqv7jNg",
						"18vAhdySJjnE6NuqhmK4Qy5AHRTJKYQXYp");
	testAddressGeneration("5K9wmCYvfksT5NxyYCRf7esj6srj3PWGnhVhHSPKbcEb6Z5TKD6",
						"1KCrUXFUKTpMs9qdZPw3vVssPWat6Frm1T");
	testAddressGeneration("5K8ykWoh5siD1r1ve4uXBr8TUEcrXLUy3qcKSGvzjPHKjUzXEpy",
						"19rxKMSpksLrtuPt6PxPeh58Gc3rKm5btH");
	testAddressGeneration("5JcMAeSh7FqWJGwmVZTaZEgwiRJtoXxHe9tkuFCap6MmRDmcgKp",
						"1BkjRxRs3b5TRnytEKdfqZyhppdeBZJ7XJ");
	testAddressGeneration("5Jc6M5FMgpAhwe7rLPsoBDBawk5A3WHBqeCbhKrQV19TkaST9PY",
						"1CZpwFun9yWVqQdCKxV4AdMHkTnRzxMsDQ");
	testAddressGeneration("5KfkbR2thx8Q8GEt4ptefSHRapGCjQAXwS6qK86Eg1ou1jzm6cL",
						"1B77E4wMbbFSqLzzEtuKGkYThXr5yHcSoN");
	testAddressGeneration("5HxZ1ZXKDuKTcYZCm8rCBycqPiebgwpW7SNAZ2g2ZkqnGYch7oW",
						"1N1Cr55DU7rESQ8aY1JVdHWU2HGQFoxiSV");
	testAddressGeneration("5JVUd9puYjrqaGT1uprvoKTTxCcNUz5UE8tBJn2HqMR4pLmbGBB",
						"1PakZnRLejDLBJMoJtmumjfH6VoMRf3qXf");
	testAddressGeneration("5JesBVQPgo6CBtu9bibcVunRL3wEDyL8MdA2ksvHGasMyajSQ7J",
						"1BrH2CuZm4smUbeXu9fZtR56C33MoPwoEF");
	testAddressGeneration("5KSMJ5teyvGHUDWWPs8zvEwJ5Ptx1SwrUfiFpwSdHdrk9SFkW2p",
						"17oL1HCtC9RsAS1bqB4qisFmfBjGZP1k6P");
	testAddressGeneration("5Jup2HrQ7h4ty6qKHcwHqJHmW4GyAxq8jcEChZLfc2ZhEDv514v",
						"14Zj2UB97kCTaEvn45yf8nAN5Ki8XYJ8sZ");
	testAddressGeneration("5JWRf453Wccz6TSb8SVVHkdGztaCFBafCrkMtT12dfGsD5nHBd5",
						"1C3Nfw8kKakZHVMAGZoJpVyKbohMUm5DsG");
	testAddressGeneration("5JBRwJNYF1a5zND8MQQB5Xc8sEivrnmda6BVoZGoL9W4cRQxZhf",
						"12bSp235gzGB4YGcxcpyAQAMrebGjsbRoY");
	testAddressGeneration("5JtQMcWNoY8WvV6mPPjJt7w8DjEiGqpZBniPMor8b4sKqZGMVfn",
						"1GbFuYM85MC1xSx8edATsKbQAUSbEPkRfv");
	testAddressGeneration("5JwhsUiWedk9WAxLgcK1fhbVBUpRFzwsiUpmL641GRDGf5EVJm4",
						"18qYgTWquLWMxDKYycmzMJ52pdWe3QJ4xS");
	testAddressGeneration("5JVLBJBmFUjLA8uwpsFZ3mkB98c7R2bdFjzcDR6sv7cMWu3GWSA",
						"17Kzqhm9pyhC2RGtoEkAHoDr69D6yiGBFZ");
	testAddressGeneration("5JRdA9Bp1Gva7yD76gMjmHhfzpjXBe2bspLaQ7q9v1GGWQJC2Kj",
						"1BiUU3puZkx2gjW2qqjZLrHHoE9ytf8kYH");
	testAddressGeneration("5KSNHAJUzaUfKfJe8JjuVZeLtG1gegq8sRtVfPSFRbAgtih5xH1",
						"15FmBkbigUALTxJfw9JEhWhHgM5PCd8yeE");
	testAddressGeneration("5HxoFBpbXMWVrSZNLjEPak83gkike4Wrm6qKD7dkNeVR2RizXdJ",
						"1NsbvkCuWPWSkRyTAZEP6iLWCdrXBj41VH");
	testAddressGeneration("5Kkdh3LPu4q2jgej3tmYrVRPwiwFUbYD93xPaAbqxJpHc83NtDm",
						"19wnmp9BshYjRMgnzUciEBbYnz9YTLN9cQ");
	testAddressGeneration("5KHUwLFCK6U6SPz1FK7En4HyEbjRuSJgUmiUZzCucgyvdXjsSMb",
						"1LoWodmoLAmo5gcG2ZZrbefdzUqo5qvVJB");
	testAddressGeneration("5J5zuCmvD6x9fSynoNo7a6BjgZGBdPq2Vu6etpvUX1nV2d8nk8m",
						"18U8qEQgsu7C6K5fziWR6W6KeRYY2fDacb");
	testAddressGeneration("5KR6i4TqSpkbRRYNNRsLaBn7HzM9afmDbzHBk4pS6v6kgDAzhY7",
						"12E5CqoxSQ5yoA96X9oXFpRpv9CyDB45mY");
	testAddressGeneration("5JFnNkyr3zZ28UDVyQWzTTTHQk8SJ57vLDqNRvbRPxcriSQ2tXM",
						"1AzQKFFKsgngkvEsPhVtRJBgctFr5Mwexm");
	testAddressGeneration("5JTqmf5paS5vSWrpGwMNpUm4SQ9T4Kdppw1hGSuJmjEoBYHrBcu",
						"14Ns76JMk3LDWXWtWE545EmnQ1ffsRoffL");
	testAddressGeneration("5K6w3sHSTGFdmNitbYUzYbNdWyYgoS8pGXbYqjX3FFZ3EfRVTei",
						"1FJMjRcpnh7N3uK9zTLB8QcR4xqUrQLwju");
	testAddressGeneration("5Jh3YgWzkzBeG9QQtBquiNdBDJuWDiNuv1kk6JjEdjMdT5X2Zdu",
						"17jWbFCVpZQKVMh92mdt9bhbLeAuquKBVu");
	testAddressGeneration("5JrPmwXk3bTWfr9MxRpZa54hvofhYND8ifN8uScG9vixB3VR1YS",
						"1J3GuzaUJ3qVhkTYUkJtSEKSF2LueY2s7S");
	testAddressGeneration("5JCTEA7yChv1epPdyJBfP3ZXfBURk46NTwWUrhq9jn4mrxsxU8c",
						"13HzL2z7NmaTh71ccnhW1SBPt8HT7sAkPY");
	testAddressGeneration("5JtCbeYqvee4UWMVr3W2XfAa8K15xddM4KkF6YHEdK29Y5TFoz3",
						"16opavpJzq13w9KJfCAacJUmoxLErrMUq");
	testAddressGeneration("5KjKeBoStXUG64cjRSPcjG9XQD4S3MQ7WqLkabN5AAJEEtZKk39",
						"18QKdNjpLyR4Kbd3dG8py6XAX25BwC9Ef5");
	testAddressGeneration("5KXoteiqSwq8EWeqtCoekerGo4cAEwQZtDt55x1nXWUpT9UcXm8",
						"1ASz5Pb31GABUBGCXqmqTzv3kHp14gGTCk");
	testAddressGeneration("5JpXZMTnxQAJE3LdcXXv3tH8RzxQKJ2uwQWgu7Wg2a4RisCecAz",
						"1245pv5uZxiejnVZaXrhmAKeVHEdx7ZzEA");
	testAddressGeneration("5KEMpskjgaCKjN2TfKiNvdQCDgejgvactrivpkWAtEjPZjkkUNL",
						"1Hyt36q48cAzUT4X5GspaiXKhMpMEsmxQg");
	testAddressGeneration("5K382piLLKqdkU1huEup4CrZ4UVtSyLug8eQT8SS4mehq5nud4s",
						"1Dy4HaQajcwgCQ7m1L22aubTQMREaeGSNs");
	testAddressGeneration("5JCVhhCwdJZ3vmcevFSx7UTHNHYV6ie52K4mJXpxA4QV2bQYTje",
						"17tyULrugAW2b4P8B555oWMVduF6VkBrRm");
	testAddressGeneration("5KL5zpQW6ocMei2zEwy2BR7iVuu5M9V3T1Xt9ZTtWttca4j4spf",
						"1MLWwkjpuJxkSeZZUAt6neYTWFm6MHDn3X");
	testAddressGeneration("5Hpmci88pJwToxKJczqM6c25wW7mP3ytgiP7Uoxr9v4esUc79J5",
						"1J2UzNLysyL2JzfTyfsQ39udprF7Ya6v9j");
	testAddressGeneration("5JyUNy1guNNH1uN4zWhPiirczgJECnmSwYDrSuEpFccdiEwzHyF",
						"1EUwvEm9db9a13M84VTXDcht92vRHCSiVT");
	testAddressGeneration("5KMpcNHQQqYXAbiVY6Bx9UeZHeak8a3NVcZasuzs8btLMj2WBw7",
						"1PhzXAFqULa1wC2ph9ca7wnBAxt6iPxnSk");
	testAddressGeneration("5Jyu49i1mTdomSgbTMb1uL5k4E5x9o5k3XdyK3WWqZkjAure7YS",
						"1JjwLngPg2Ysi3Ra8nXobJzKaGQJtMponC");
	testAddressGeneration("5Kbqi8WVhVs6CxrZdW7q1eLJfRAqxaaDCnS5ddaSZtKtJKsNB2g",
						"1PA5BnZTYkbF97E5XDDZgMExkmfMdCDTgc");
	testAddressGeneration("5HzFsYCKJaJpVQQGhGizqa5YtC4Q9XWvofBZimYNa4GiH6qA1B1",
						"1869n4xi9jfHukfoyvEvAzPQZR4x4YX3x9");
	testAddressGeneration("5JwrbWqf9i9adP76bXFrDAaDSqe9yYdErRDsa1aptdjbP9DdHAF",
						"1Mk6A9mb6VCFSsUFSiZmoyYDSBfxxBwYd6");
	testAddressGeneration("5KMJafgK9TaPAa8ig24XQE7XsG1dcmrtKnqcr7JWfiExqJsAKTT",
						"19ZPHW27A5ModoM4Sr556C21thWpMyvnnv");
	testAddressGeneration("5Jvp8DHJssqXLFTqfW751ka7b1iuRC2q93DCoRiP3YohMytXbQR",
						"1Mr758abHKnnTXT9QBoohp7KqTjbW4BiE5");
	testAddressGeneration("5HrDLxxWAbA6F5hrK4YaBf5cFBbbcjvBdKWiZetRVKyi4binnV6",
						"1DXEyQDU684DZxRoNH5tKyXuSScqY8bpZs");
	testAddressGeneration("5KMjQzbMfuUKVY6JyGpBJCZCpEqWhTMHJ4m8kp4ex7Rvzvrqi8v",
						"1LcV2LE8RxW51oofCLjdBc348HyeUqZyEw");
	testAddressGeneration("5JAma4nNX48HRkCdZTJJkJ7TAnjX8Y9Su827G6dSNfREPq2FCFR",
						"1Pw1Di7AeXCNLG14DFv9hcmTV5Nu6nswPR");
	testAddressGeneration("5JFiKUk7b8RChhM93cKFC7gQqQstgUT4aFfW1s7ops8D9mGL8Fo",
						"1EPVc7z779mCNtcrU7BqVokXJvC2Jocdxj");
	testAddressGeneration("5JAT4hiKVYqDWsiVpU2ejFZJkkC5kYB8n8Vh1DWnxSaw9WQt6bN",
						"1NHaUU3RHB8u6AKSs3Up2JyYsQhqEWGfyD");
	testAddressGeneration("5JF4CpHTYmuQhbPY2ukbgPxSdApSt9ixi6ZKPabYCYhYsbBRsYF",
						"1JJ1VqcH1YfiD3CHfoUH4yEFXddfk7a483");
	testAddressGeneration("5KGyNc68AgmaEErT9L12ZAdxzQHE1TC4X6yHghDZRgaYru94mYC",
						"1HnX4D5i5W8pUdGqWoUAD5dFrYtnGL5WRG");
	testAddressGeneration("5K6XkP3LE8ffZeiJuBrvoxyMFTQw7TLTAAQfQgadXjc4NGQ25bx",
						"13GbjtATSLjZSBgST3bSSHbLecAvctMN9C");
	testAddressGeneration("5KHzcWVn798Z5z6Uhp4MEZca6tNH4LbvBWPbi9v1Z2uRNi2pP85",
						"1An2U2VSfv4RksZdKk9oRcBKPh3hb1T1tb");
	testAddressGeneration("5J5aLmfncT22X8VpPicM1dCnVs1D18xbRStqCVe9pCfVGjKFghe",
						"1BKiD5Fhf5VooSm2jPp6ZveL5hXN4WN1Yq");
	testAddressGeneration("5KRj3Lihq4kWxBDt575oZNuK4RscKV6nakuN5QLVpMF8BfJSypH",
						"19r4p6m3DcuifQPFBbhXriiUm7oaynUwit");
	testAddressGeneration("5Kdqkm7F1uMXxymftN9XtxYoromirThR9ShFZcnBYYvRvxXswNx",
						"1AZQuH5wUQ6vHmxXsGSwTASXWPSTbby1Sa");
	testAddressGeneration("5Kjo2t2k1MJFbLwVDWtzJagcoCaLBa21mLj5T14RnA5Wfd8bLUa",
						"1LnaNpAHH2HNqbSLbDNEXN2NePuJWNgP37");
	testAddressGeneration("5KAppr9tngmL2xzewa6rkpDPYZv9Um3SxDLTYTa44zKnNpN394V",
						"12NawHog76ykumyu3wiUVGnYFP38yGBt4j");
	testAddressGeneration("5Jjk2Sk6WVgRu4Vc2cPAbVtmmYNLYbNfuD9iaurx1PTQEs4xfSc",
						"1D77ZjeQZXuPGiyVFj3o1na82wh7qY3ba6");
	testAddressGeneration("5JFG9y46eVateFSpsUmMFKqzEKgv9tdRSkzLqR1Jb4eXr1qL72n",
						"1j195D2o8tjPP5TPEyP7d2TVBKoEpum24");
	testAddressGeneration("5HyGuUfjTcP6j1K2DYCKbksZpHUqtxdga11LQHL4Z7i3aSYjno5",
						"1QJpmEKZE7Qans5dVJez9JBY9DkFKVdR5H");
	testAddressGeneration("5JvcWQBPButJvLrxHKvoVeoWLrPYaJKV239JRifdRk9MuyHLhjW",
						"1JNWwuC8Y6tKXoCrdXAY8uUAmXKcHBoiFL");
	testAddressGeneration("5Hwp1GQLjLabFM2jrv1qxMsyCCUhw7Y7YddzKQnsPNKdpUbRPVP",
						"1PXqCR6cryp3Tyv9juuvEyZbkJS7bAjNn5");
	testAddressGeneration("5HwFFJJLaPU6TLPXdkhkpdsuP5S8EWUmHivb1JFHvWkDHq1C463",
						"18r2XXaAHZYsV8DRdmUocpYW9VuDcmCu7g");
	testAddressGeneration("5JZz4z9JU2FpbBWYQqa8Vnj3pAwZqxW9cWptgTs54kcVokU7Uns",
						"19k5TecFZKZGyhDZWPbRPyjm6ihNTFJizb");
	testAddressGeneration("5J37zMawk4HPTzRo13zhHduwfVis3cs6FDFPaFUndkjJbKpPTw4",
						"1KgR1mwyQU4BAGctkbQ1gbwkzM9ai19f1L");
	testAddressGeneration("5J4jh29pUct8r2Uaqv64HEorDYrqbBG5Chzgs8Pkpc98rCFbYXw",
						"15oS7Vp3s2ZrC9SfTYUD2wfitTcok5G21j");
	testAddressGeneration("5KJ8mWAwmAc2nZutbWTosW3nCAVBGtj9L8LQ1cuLCxMAtqYfnmh",
						"1iktiMwjbh8ABunBmwWfTLkRVsZfybMqA");
	testAddressGeneration("5KR1naSpw92jCyQeB9acsciQJfMsmF45eFnTCAi4u4TWeKhALn8",
						"1GPrMsWf1phXpRsc2zXhcwEW5mMY2DPSD5");
	testAddressGeneration("5KeLX1YdFvTgzM3CEvcdbZLd3vyo7j1uRa9T1zsTv92jC3D1Rbv",
						"14iee1QDzE9QDvTdghNhRh4rgZcFbFbLt5");
	testAddressGeneration("5JshhCyShdgPFYJsRbWKf1GyhhqLkjSv4or99ProF5Dyn8hDDfk",
						"15EHN7jc2rxLLppBU6SKUCpYJUxUebfAkT");
	testAddressGeneration("5JPeiS7HPj9m2anATYJ6PrA4zbYCuoQ45C363f8DTWFCuiuMUWT",
						"1HDyeeRKsxdUCqFY4EQdCFjaongHEwTsma");
	testAddressGeneration("5JpM3ws8kVXfquAftU474R3CKxpgCHXztSJ2dWJkR7X5M8f9rLD",
						"16Qk3duZrJYZSn6MbcZDF535e5VLzdBi4f");
	testAddressGeneration("5KeV4VY93kSxGZVmXn2dtpuS5k4E4SFKM51oUfA7u3iCh5d7VER",
						"1HZWur3RaaaJmGeYL6y6dncsLkoqwBrSMK");
	testAddressGeneration("5KZHZ3jYz9n7UZSMoca9agGN69cMGEwCE9q6QodbxuarqLfKtDd",
						"18x9P4DZS6vUMb9YkuoSZmmuVv8k2PooAW");
	testAddressGeneration("5JvsMLV5cJSvu8jWECSk78AKb2Tq8MFXEHiZGQ8LLWkLWdSbQfG",
						"1DC1YK1Q7Zva5Dj1DWGo4bEFHWQujDyweb");
	testAddressGeneration("5JgddiAyf4SLrRAmQbKNT8kLiWVu1eTyFf7QDrSw9WYzbDyPNf6",
						"1PVSCTrWPPcm2YZmP3zuL9oWkJwboRxmMz");
	testAddressGeneration("5JNoyEGJxDtKWD7D8Ha5r7CTBQjnpcvdabR9yGhTZLjRnKempqn",
						"1HcWUiPMzzGX5itnNHbiPDXGJZAj77qxcK");
	testAddressGeneration("5KFPA6csW8qU9TyARkh3hSSCVR8EDoDcnZof9Wjx4pWH2sMJL1J",
						"1CNz1XK7aqZt5HjRSmzLkAegDBAym6QfJH");
	testAddressGeneration("5JjQCav29gUZJ4BJdycD2HCzN7iHRPGrrXMwCRnRdhYrdQSZDHH",
						"1KoujVVuCbNom76Fok3zMWxW1MEtojGwRv");
	testAddressGeneration("5Kfa3U8vdFrihxMgqsATk9jwNkDaFZtem4NeQnkvMqUf99DTiA5",
						"1ENCBWt3Kac1zSRM1VtkDMCVd2VyMmRvbn");
	testAddressGeneration("5HyudJEBK8viGXbRdc61icy8oBsQoYMfYZhKnVkbMEDJTRJPGsw",
						"1JNh9c8awnW1RBjvkB2PA3DDUnK5We15u3");
	testAddressGeneration("5HuCRNtyuqJ25MV7AoExx8AFoyRPon1PER54J9KDAJRSUDPhyWB",
						"13uWCVU5hSeJfvnzEicv2kgW23kD6QfT3p");
	testAddressGeneration("5KPDMsusJp1gV5FGevxZhw4dn7sMquYejMFY16ueiqTMrBLmbnJ",
						"1LL7tSLEgPbfwnUJrqbbix1Nz1ck8DtCjx");
	testAddressGeneration("5JUaCrbCbjWZsYpKzrfmXSm9oYRQDTVTKicqPyfbNRW12rQP41M",
						"1FrF9zfMWi7jdTYe84LtSvBbrFyd43naFX");
	testAddressGeneration("5Kb95a3Hp39S7K67ZDm5QRBhLSjUUq5arMgr9xNfaK6dBbiQJAi",
						"1Br1HMZM141FjC6BHHF5kpLywk9ZojPmpY");
	testAddressGeneration("5JHKbFXjWiK3uznP8HGy78gtW1mE9VLWyDB7TXhD5TnRpVoBcgA",
						"1FXV45qitHu4df8zKvFLcv9EMSYACSHZ7j");
	testAddressGeneration("5JCXwLbfWezZMpn4yY4q4hhx5A7Wb1FxvskBAbZe2SN21L2QsmB",
						"16RQj45NjE7HKSrv4E6JMU8kjLdaxdc9MC");
	testAddressGeneration("5JDeRxgXC6jNT2vuiH5hnEzVZmm1pSzbN9LyhamoWSHnoDdwumA",
						"1JoXP1eJbRP7F3TcqPfuWziuiPdxQn9e2f");
	testAddressGeneration("5JAdtvemK5KHcK2Vv4ThqoVvBUwAMZXEXZbiDroCqqi2jxYvZFE",
						"18j5KL7HHEdarWbq34bbZWECxzFdEzKhgV");
	testAddressGeneration("5Jb6RzY9jcJT4LTbSFd4C6SxkSpzgau8H1r7LnKUxPa9eHw9HQR",
						"1EYdXgu9mNL1Mvim5xva1p6Qn9ihjAHL2W");
	testAddressGeneration("5J3nhWkUX1XhKzDpLR6drFUknYtapzfVnGkA78we4E8FrfbCeKD",
						"14ABa7TJ3vN3Mg2yDyk7kmar4EwgvQn9D4");
	testAddressGeneration("5JQcuvTir8cfUGkBjY5WBWbUZoe3tJwd8uzYy4tYafZ3kp87ji2",
						"1JNGVo85P8eRLKz4gXYr1PEmC913Rhrras");
	testAddressGeneration("5JPRzUDi1f2Xg9MC81D5VqNeLiPFYHJVt8BdS2LL9vhamNJUTRQ",
						"162HQWXxduLq7NpwaJZ5DqWbVUZUTAZdJj");
	testAddressGeneration("5J1fNpVeq9f6xpuhuvd2kqevNLEFN2V9H4jupGmEu4XEt97Hudf",
						"18RXGRtXHRGdUCy99uKZ19yZ4PWx4qE27r");
	testAddressGeneration("5KWi6QNso5TQn3XgSoaQD3mgrXwBetDEdSJ3mCt1ZsEdEGU5t3o",
						"12bxCEFi5WwbcFn9UzqAZxA5RxvUuk9ynf");
	testAddressGeneration("5KGAAuq8Ed3tnq5gA5macMM3BixhrVszTMhup8p7gvjqzZN2a3N",
						"1G6ZUkXNpBTkTqLAdN4feDW677qEbSA3tf");
	testAddressGeneration("5JbxNRvpC6HqfF7GYRDFW6Ju3kW3ANWvmGw5VZ7J3kP1kCQb5rU",
						"13UYvHsPXbNrWFT3NPUvdMzLBfWYtgpH2h");
	testAddressGeneration("5KH4aZmEZ4A9GgStfVmzqvNJTnfDthyhLNaQSxxtPKstPSF8tji",
						"1KkdxsG6WWWk1ezp161wmqjHT8W1Hgbikg");
	testAddressGeneration("5K4mmih6hZnPoQmz4iUpmwhGTG24ivKRwZ9sELWtV7cFbCr4MeC",
						"1HSct5RmLkxwaa7mkv79UfhxyMb5APJZvZ");
	testAddressGeneration("5JdCjcZxBWuDGQ5NoUdDbtXkGikSQznAaA5xy9cbBx4MbLXAo6f",
						"1KjHhhwXdoLchoHTUxQhrM27jhjxfZKmqB");
	testAddressGeneration("5KWp7aqbcFWzUhS5TVh8JmxPGy3SHmoCqgU9nmFAPXMsQEfcV8B",
						"12s7Ea9BnXFirTnN2EXue5kugkvqway68C");
	testAddressGeneration("5JDV2butQugm4Lnd1hFsWh2UVkZdYw3xwNVtQFEh5V4aFQnQ3Lt",
						"1B4iP5ySt2EV2BJUyheS27e4KW4DSXR1o5");
	testAddressGeneration("5KXWdwsUkdyjWTWyHvuuuVr5W7DRNTRviDZ16YCQiQ2SR8tiYsT",
						"1FEMR6XuAmdFR7Hfdw6N3kdHB5EoDXARou");
	testAddressGeneration("5K7nF7UozCzo9vWqn2hWfnJmN5v7FLZWUd2MkBWjXmBnasbqjoy",
						"1F53rfyn6Atz2ye81MGFrgotHCguRPnWnP");
	testAddressGeneration("5JmxT7ybXfqJG7y7YfBHXj6Mcwgw2vRktNb7zo3qauxeUc9A4Aq",
						"1PjKgBNihwe46gmNkMxE5LGxPxhiFkGoBS");
	testAddressGeneration("5K6DCrqEttFP9gSjzPeUFxNzeKWML58Ut3Yh5yAhRhtnxBMypxc",
						"1KqJaXNqk2295ZiVwUu1CzFUS2PiUjyyJN");
	testAddressGeneration("5KU6qudNc76tq4RZyp2h5oYXFWoJpguRwMjRcss8BkhGBQjA5ys",
						"1KzebqXoFeirAbVAcZBqJGvqToPwbT9Lf8");
	testAddressGeneration("5JKrDrVRhV7SE111mzXKrc3ZMztk83UptpGFE24T9DJst6q8Pyj",
						"17LuMKoYj4WuPm1CSr6w1BDpWVqn5QaMKS");
	testAddressGeneration("5KfWDuWr49ZigWhqixzHpuzpPM4PVwaAJtmhK6pbtqWh2bzFcXs",
						"1568s5jpJUWMBqpbt2bszf4r6owCQry7gY");
	testAddressGeneration("5Hu3UzifcCkqfb42qZmFszChdY4EEoEYbXL61WRGCBHWRh6qNhr",
						"14F5yo6qiWTN7WmMvJLePjrookqqveMoey");
	testAddressGeneration("5JLSpcJoPooirfUfZBKEQm7bxzqKeJvtYGqi3brjQydEw65153E",
						"1K28HUuncHN3BLBYvN949XN4PhL2Ld6nAR");
	testAddressGeneration("5JXdWqkWd2ioZ2Gq4ozxHKJMKqLVFqe1spKxZ9pKHqJP6ZGBXr4",
						"17f6RPwSW6eeQ4ceRx6CSg5mqxbuUrYheq");
	testAddressGeneration("5JDN9R3jo5zQh2QyZesCRz3FmeBBFPZErGaa1pbtx5KSC94ihZM",
						"1DsuA2FpQxZmCFjkbZYPprAKmQ7PnuyuYv");
	testAddressGeneration("5JHd2kJ3HtQmy9xxnMMfsCAfbJ4m9niP4xCC6AsVmAurnTgistj",
						"1LxECeov1vzLrwZuvcdXukrYSBxooswUEG");
	testAddressGeneration("5HpXPECbjhKTHwuvQKFU4PL6kfGH3BCt45xjYvf8oA9eJNrmmF7",
						"13t68PDimTburzghg24ZGnogY3C1tQRk37");
	testAddressGeneration("5JL5Ab5qBsbQ6fFEMHCK9g3P1y9foRkKc63rRuepxftY7dZLqvN",
						"19Yw6LjWNsPeWuYHW365RaMdVpxh1dkYHm");
	testAddressGeneration("5JNXn6GZwXHcQgHzTUyFt4ePjyTkLUmWLYVcjUqVVnrPMWCovwF",
						"1NqULqDTaKYeCjZeDfvMHx9Psk9bDKN78m");
	testAddressGeneration("5KMGLyVdP89sLhbrtbToHo4PcWX3JzWkfz5xDVB3rwxkE57frJN",
						"1E7o6HgYq4vv4FCid6kPmt3s7JBqSFcSGj");
	testAddressGeneration("5JcZvu9gS67FDWSZGF7ALeZ6j4dcr3Lm3Qa3QgQZWhHFLEo9eKr",
						"1PknvJi1DPwTeT58m7tojMeFd1sdgFhvS");
	testAddressGeneration("5Jy5EHG8MLvhjeiSBLpw7Z7D9QjCtduXFZJtuVwypTthXP1GFNq",
						"1EZRUFSaH4dFBctDoDHWA96GVjZEG6y2Ai");
	testAddressGeneration("5KFk12JLe9985SokNGaJSn48rH5Af3qDYJBcifJsrpA1dPTKYtW",
						"1Aa1j8U5gXwZkyMpVnjejQjcmruYiZUnoi");
	testAddressGeneration("5KkF6rqBjD1uByPVZFd4cFUeLvUSvCYKLh8AVcpXvVaPotJjPZt",
						"16qFp28HhSPSGpKFs1H2MG5e6RhfH7KgDF");
	testAddressGeneration("5J5QoTfJLXMHP1nHZ8HjLzMTRUeETnxoPdGGCZw29fcNJ4JU427",
						"12HfGwpa86QcEasMesJsBkgQPca9SsdcYM");
	testAddressGeneration("5KBA2WW4S9HRe3GiWsnDuL52CosYRgGXw36vYBT89ztXfzxJWny",
						"1UCkCf7Wnm9CXoNsMFaJnoZuRFdLGee2M");
	testAddressGeneration("5JAV2xaXYVZgZtB5AXZU4rxgMA6FaqibAC1RNstJU7RiprDL5Yc",
						"1Q3Fu9AeFb1U5VZkzKBZ3N7EcntcHRY2eg");
	testAddressGeneration("5KKnDSHRp4MVkDZAjdBmgA81HxgY1josS453QPjXGd89mYMfnoc",
						"1GF9hvd1PbEimz4eSbgGShRAuKJjSPjx6D");
	testAddressGeneration("5JfBWYP7wTxLNZQpQxmv3yDaRXDuEZMNzP1hQTeT5wfx831ZDb8",
						"1GYe7Mnjnrg678XkvKq7oH4VDgCHWJHPpg");
	testAddressGeneration("5J2Ewer4FD6AsmYwMGWmyicvot5oE1Mq83gcZkJ27fUwdR2u1KA",
						"1PyFArGT9xsS8eCjAo8PCyBf68PrqXTAiT");
	testAddressGeneration("5JUPWKoWMFrSkAJepaKirmLrtLJKa1AC8UVxdRuxminpuWZBHyP",
						"1K2mg7KwFDmsKLaBHigs5j9aZr7Zf5D4uv");
	testAddressGeneration("5KYSxR7pnxqog6ywhFfHe5Gvuedvanzyo3dx1ArPEHYASPuYEAa",
						"16mFbiKJpU3dTtyrbzDbzin63F9mdrBaRU");
	testAddressGeneration("5KKQYu5EBrNTcvkQFRvGH43YWkxCVgT2mnPTZKj56AfSVmYr3Zu",
						"1DFhzQQaUoxS2v8P6WNLWRRA5aVyrF8oXM");
	testAddressGeneration("5Jd1EyK8xZtSBtUH99hok5XD7jUJXc6x7sTsuGHUTV7u7L4kDPg",
						"1Ndt6CKNtS2f5TBenHwy9VauuHPb4uVvJT");
	testAddressGeneration("5KSwQeZiUF9gspZ21UAVbDsTmiGfu8eHqNdGg9ck5n1Cu2yYs9h",
						"1GKXs2nN5La1EUgEhwzKbVoRhktJVm6aRf");
	testAddressGeneration("5Jn13EPLtq4R2QVFGN8vCQbxC8Xz1B97gaBwqArk8TCebUHjd8H",
						"1GqgtuQo5rcEBuGgYx6Lw7zA2gLLAqPXxq");
	testAddressGeneration("5JoDt8XkDxq5B5JVgjDJQe4GxWpv3o9UHTD6aEsuy4Bnvuewdff",
						"1LywoK1PpzLXkug3ZNU2pGHcd6vyBXiF5F");
	testAddressGeneration("5K6h24hekRABSaehapyJg3wH7Vuf7oc5ihYxQFtVhNbWjkkJhLq",
						"1Hu6YVk26o5EcNobo22pVkt5Fmbkec2v5i");
						
						
}






void testMultisig(const std::string& pkifStr1, const std::string& pkifStr2, 
					const int numReq, const std::string& pubAddrStr)
{
	std::cout << "Testing multisig: " << std::endl;
	std::cout << pkifStr1 << std::endl << pkifStr2 << std::endl;
	std::cout << " " << numReq << "/2" << std::endl;
	
	std::list<PubKey> pkifList;
	
	const Pkif pkif1(pkifStr1);
	const PubKey pubKey1 = pkif1.getPubKey();

	const Pkif pkif2(pkifStr2);
	const PubKey pubKey2 = pkif2.getPubKey();	
	
	
	pkifList.push_back(pubKey1);
	pkifList.push_back(pubKey2);
	
	
	const BinaryScript script = ScriptCreator::createMultiSigScript(numReq, pkifList);	
	const std::string scriptHex = hexString(script.raw());
		
	std::cout << "Redeem script: " << scriptHex << std::endl;	
	
	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(script);
	std::cout << "Multisig address: " << pubAddress << std::endl;
	
	if(pubAddress.toString() != pubAddrStr)
	{
		
		throw std::string("Error, incorrect multisig address");
	}
	std::cout << std::endl;
}





void testMultisigPubKeyGeneric(const std::list<std::string>& pubkeyStrList,  
					const int numReq, const std::string& pubAddrStr)
{
	std::cout << "Testing multisig: " << std::endl;
	std::cout << " " << numReq << "/" << pubkeyStrList.size() << std::endl;
	
	std::list<PubKey> pkifList;
	
	for(std::list<std::string>::const_iterator it = pubkeyStrList.begin() ;
		it != pubkeyStrList.end() ;
		++it)
	{
		const std::string& pubkeyStr = (*it);
		const PubKey pubKey = PubKey::fromHex(pubkeyStr);		
		pkifList.push_back(pubKey);		
	}
		

	
	
	const BinaryScript script = ScriptCreator::createMultiSigScript(numReq, pkifList);	
	const std::string scriptHex = hexString(script.raw());
	
	std::cout << std::endl;	
	std::cout << "Redeem script: " << scriptHex << std::endl;	
	
	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(script);
	std::cout << "Multisig address: " << pubAddress << std::endl;
	
	if(pubAddress.toString() != pubAddrStr)
	{
		std::cout << "Got        : " << pubAddress.toString() << std::endl;
		std::cout << "Should have: " << pubAddrStr << std::endl;
		
		throw std::string("Error, incorrect multisig address");
	}
	std::cout << std::endl;
}








void testMultisigGeneric(const std::list<std::string>& pkifStrList,  
					const int numReq, const std::string& pubAddrStr)
{
	std::cout << "Testing multisig: " << std::endl;
	std::cout << " " << numReq << "/" << pkifStrList.size() << std::endl;
	
	std::list<PubKey> pkifList;
	
	for(std::list<std::string>::const_iterator it = pkifStrList.begin() ;
		it != pkifStrList.end() ;
		++it)
	{
		const std::string& pkifStr = (*it);
		std::cout << "Pkif: " << pkifStr << std::endl;
		
		const Pkif pkif1(pkifStr);
		const PubKey pubKey1 = pkif1.getPubKey();
		pkifList.push_back(pubKey1);		
	}
		

	
	
	const BinaryScript script = ScriptCreator::createMultiSigScript(numReq, pkifList);	
	const std::string scriptHex = hexString(script.raw());
	
	std::cout << std::endl;	
	std::cout << "Redeem script: " << scriptHex << std::endl;	
	
	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(script);
	std::cout << "Multisig address: " << pubAddress << std::endl;
	
	if(pubAddress.toString() != pubAddrStr)
	{
		std::cout << "Got        : " << pubAddress.toString() << std::endl;
		std::cout << "Should have: " << pubAddrStr << std::endl;
		
		throw std::string("Error, incorrect multisig address");
	}
	std::cout << std::endl;
}





void unitTestsMultisig()
{	
	
	testMultisig("7TNNAM1DB7CWUm5zhm4ZddPwwrza6BiY7haU7croLo4LjxkMtwk", 
						"7Z3Qw63bF3sMHwH47KWht66CcFQvnaYVcpD7zRhzDmvqyJyt9R5", 
						2, "3AEGCMHnH6nq39cMZzYeuFiAS9b5b3myds");

	testMultisig("7Sd3fCzp6HyXBDFYpotinvEQKMiSkgbavXsJ3iRtr9ydMrtVS4r",
						"7TaviNuFzUedLyQdVZFvaLJrRLsfGSGCBo6i4GC2mVNwXM3VkXQ", 
						1, "3876VepYyBp7aY5v71odRtsJG4cznY3VTj");
	

	testMultisig("7UuHue9VKbjQDvakg7s36pFiQJLPnDqXnTh3L6AWDAu7x4QTGS2",
					"7TgxoFeJNgN6GT5nnkQ5aUw9dH4gBYrBbCmL71N3XQY9PyZyqu4",
					2, "33FZi9mKnbaSZqJFVhdwdpU1oA8V4wBD2a");

	testMultisig("7YDUKdjHj7a3q3b71rjk7qtXAiiQAjDM4u3DMSdX3QgzWNwRtNN",
					"7Xxi4yBdVEXGoKj7U8W4BNvUdS1gD84BhGnRy7WR56hrrbyRFBm",
					2, "3581wbs6yD6QVhb1prUkVn1AewD2n2DjUa");
					

	std::list<std::string> pkifList;
	pkifList.push_back("7WLyueyJvjSoiKhRJsijyVjMewhyXTEXTPj8VcScgKGBM96wQZH");
	pkifList.push_back("7XmPkAEPZzAe9obHvomDBnj2EvVfU3QfJxFgys33CpzTDzfh4gk");
	pkifList.push_back("7ZtxS1oCHdUFq23Lv15E2DyJpa2Pnff48XgV8uUMbFYSVRfaCxB");
	
	testMultisigGeneric(pkifList, 1, "3PXEwmaHbcwBo5Anoqvsw9WP92j85QvLRP");

	std::list<std::string> pkifList2;
	pkifList2.push_back("7WcUBG66anr6qCLcBXPtHodE7CNjbeZQH3HpNLaAsdhtjUTrFgZ");
	pkifList2.push_back("7WwwfJ6rfycWjULCXSkqTdQTwxudPZgah8v16BQwB7d4nb9evmZ");
	pkifList2.push_back("7ZiBiSGFJSwVpW46SZZHHAt6qAbNxUEqugFWaxe8Zf1ddMMctbR");
	pkifList2.push_back("7Zcx9k9ah8HrxuQmxTFSMWabZDKwJQnz1VqbqndLVdedbd2FgR4");
	pkifList2.push_back("7ZoPqhnXZW2Jm8FfKT7RNb9opt4M9GJhBG8wuzy57FQ2VDMisno");
	pkifList2.push_back("7WJrDk3nSwdFAZtx9ncbtBm2meKSRfND132jzewqBBqy1EjJavZ");
	pkifList2.push_back("7YcbDuWj6RGx1zv4F5kur48j8n8JabugPNGgWVdhRXymxLFxpWj");
	pkifList2.push_back("7ZLDmMz7D1XEyPahtaNGcpvuuAxdrzNcXt9cFVbRrvqHP7TeJYk");
	pkifList2.push_back("7Wk5g1HtnvvuGJEtJaxgNW5aZEzVX4VFS62Aw8x2atYmNPcFfCm");
	pkifList2.push_back("7X7dvUJDNGgSAP9DfEUK2NtxhoHm2DzYp2mRv5HVsLVjP3tt1vm");
	pkifList2.push_back("7Y7PUNNE65UX3gRzmZcx4N3fCw8VfYg6YSwdVGGdABuWCNKiYgb");
	pkifList2.push_back("7WdEp29SLhQ7AHxv83RktSaQBUSdsURgjHLtnYo4oM16kpXTjpA");
	pkifList2.push_back("7XWHMrq1cEVU1NCDZrpyMhERCWh4ankER19xtCqV1guJnDW5gLN");
	pkifList2.push_back("7YVwR4HZzQUChoX4CgL3zw2ddoUpV259ChzPYDWNqCXtS2PnUrL");
	pkifList2.push_back("7YxZpKmZh1gsQ83evwxZj9jzoaDWN9r5et6jnZaHYL8mTL1M5Lg");

	testMultisigGeneric(pkifList2, 1, "3HARFgXwpQvjw7naEx65E2rravduC2ezkf");


	std::list<std::string> pkifList3;
	pkifList3.push_back("7Zn1uaN6dA8xGzK1EMCsKPaMeU9VmYfK4aBnqUmJYcNFV4w7sbr");
	pkifList3.push_back("7Y9p3uvhhUJoJU94W2P7aiS1K4NXSd7kdgjv6cQcU2YKsPgNwQt");
	pkifList3.push_back("7Zg7PqSYw7G6jCPEu52Brb4UQj4KL6ouPrKfSmncwsTpz8uJLDH");

	testMultisigGeneric(pkifList3, 3, "39GXE4Qw3bpCJhbQnRwJKfWUZggFWJCgxD");


	std::list<std::string> pubkeyList;
	pubkeyList.push_back("0460C51FAD0D5B16773BE3F3772D569524D97E326FB071972E748914E513D1550C7C848BA504BA70128C418FC0BEF45116C151ECE7AE2FD3504E6D86A2FB9742F5");
	pubkeyList.push_back("04C1140C7338C195A813F08170866E9507364A5DFECE60FA44A697344F815677C985FB289A286D0952AC3F1FE1BDCA84AE3AFA5C935FB069795158C843B308F0EC");

	testMultisigPubKeyGeneric(pubkeyList, 2, "33FZi9mKnbaSZqJFVhdwdpU1oA8V4wBD2a");

	std::list<std::string> pubkeyList2;

	pubkeyList2.push_back("02199DF320999F3F127D211A0F4EC4925C934513050445B94293317DCE36CA2565");
	pubkeyList2.push_back("03A1E3F55225A8A5994EB2213553E5B398D22C7F448AD7A7243BD71004D9430C8C");
	pubkeyList2.push_back("02E6E61393CF5D09CBB5B8894D1D090EF7ADC66427F775610A8AA2895CEA55E4DC");
	pubkeyList2.push_back("025C5D825B8DB13141F6F4641F77ADBB95A38440F22B4099EADC5752CEB517EE3F");

	testMultisigPubKeyGeneric(pubkeyList2, 3, "3KoDgiDVepDxn4APVL5mZMsMakGeTW2ua6");


	std::list<std::string> pubkeyList3;
	pubkeyList3.push_back("02199DF320999F3F127D211A0F4EC4925C934513050445B94293317DCE36CA2565");
	pubkeyList3.push_back("03A1E3F55225A8A5994EB2213553E5B398D22C7F448AD7A7243BD71004D9430C8C");
	pubkeyList3.push_back("02E6E61393CF5D09CBB5B8894D1D090EF7ADC66427F775610A8AA2895CEA55E4DC");
	pubkeyList3.push_back("025C5D825B8DB13141F6F4641F77ADBB95A38440F22B4099EADC5752CEB517EE3F");
	pubkeyList3.push_back("028279C6BCD267C49EC8DEEA41A2BEAAA8BCFD4D6FAD8E8FFF96C6FB6CA4C8B05D");
	pubkeyList3.push_back("02ED82560A34BD78B89A817B3DCB88095C7E0FF80343CB632937B167B9B6D943BE");
	pubkeyList3.push_back("037AA205BA95BAB9678C3E7C7D13FA293B41BB960C397028D1B9E26DD8D1ADB7CD");
	pubkeyList3.push_back("02FDFE0DEE96F5D851B7058235A62E6EE61BF1CEF2582A64549DED250B4F3FAB70");
	pubkeyList3.push_back("0212A876B7790F5FEA44F069F7DFFEA64C8EB3B2C63DC212952B8007B6DDDAA98E");
	pubkeyList3.push_back("028E7F611925C81AA1B08CEE82EB17B3114B7AAF77B6F28E6708320077F46488BB");
	pubkeyList3.push_back("03CC2A61970A8FBEF3D18CAB11C59A94DBA22E1D8F62BF564C688BCBE7CEC042EA");
	pubkeyList3.push_back("02ACE2D20D5CF6960947E6B965C5F9B0CDA4F8BCBBB9B22128DA97AB7A41D908C8");
	pubkeyList3.push_back("03940EA11AB3A62DC9B0F24C5D41DDB7140ECD9108AB998928A48229F6629FD099");
	pubkeyList3.push_back("028A3B63668FCE845E593C501EEBA08B38C16CBF88DFD0507A5DC1B276321AEEB6");
	pubkeyList3.push_back("031B7E2355B18A8322A0464560F9B2DC1AD8A1A1CE0FACA376005BD02C9AC59B2D");

	testMultisigPubKeyGeneric(pubkeyList3, 8, "36zQvnC7tqx2ifUoJcsr6zUVs6LRQa22do");

}



//	calcTransactionTotOut(hexStr7, "1683692");
void calcTransactionTotOut(const std::string& hexStr, const std::string& totOutHexStr)
{
	//std::cout << "Will convert from hex" << std::endl;	
	//const std::string trans = convertFromHex(hexStr);
	
	const BinaryTransaction binaryTransaction = BinaryTransaction::fromHex(hexStr); //(trans);
	
	//std::cout << "Will parse" << std::endl;		
	const CryptoTransaction myTrans = CryptoTransaction::parse(binaryTransaction); //trans);
	
	//std::cout << "Will serialize" << std::endl;			
	const BinaryTransaction newTrans = myTrans.serialize();
	//std::cout << "Serialize ready" << std::endl;			
	
	if(binaryTransaction != newTrans)
	{
		throw std::string("Serialize erro");
	}

	//const std::string hash = doubleSha256(trans);			
	//std::cout << "Serialize OK for: " << hexString(Transaction::getReversedStr(hash)) << std::endl;
	
	const Amount outAmount = myTrans.getTotalOutAmount();
	
	const BigInt<128> shouldBeVal(BigInt<128>::fromDec(totOutHexStr));
	//std::cout << "Should be: " << shouldBeVal.strDec() << std::endl;
	
	if(outAmount.amount != shouldBeVal)
	{
		throw std::string("Incorrect total out");
	}
}



void testOneCryptoTransaction(const std::string& hexStr)
{
	//std::cout << "Will convert from hex: " << std::endl << hexStr << std::endl;	

	const BinaryTransaction binaryTransaction = BinaryTransaction::fromHex(hexStr); //(trans);	
	//std::cout << "Will parse: " << std::endl << binaryTransaction.asHex() << std::endl;		
	const CryptoTransaction myTrans = CryptoTransaction::parse(binaryTransaction); //trans);

	//std::cout << "Will serialize" << std::endl;			
	const BinaryTransaction newTrans = myTrans.serialize();
	//std::cout << "Serialize ready" << std::endl;			

	if(binaryTransaction != newTrans)
	{
		throw std::string("Serialize error");
	}

	std::cout << "Serialize OK for: " << binaryTransaction.hashStr()  << std::endl;	
}






void testCryptoTransaction()
{
	std::cout << "Testing first CryptoTransaction" << std::endl;
	const std::string hexStr1 = "010000000143a9423548ce957b298fe99c409aac9d890ac3f56c3664e34cc7425e4eb71855070000006a4730440220052f437fbe4e2c13edc017d075df7c7de2a3d00054eafed9b4bce683f0d799f402202dfb746a0b76f0d1b96db3f2f7f086090d207f925c176ed079c7ddd986ae65d20121037e7bf63d259f22aa608029c8b71fbb8960cf7c761d3abbfecddcd2e0cb125623feffffff03ced80701000000001976a914035c1f2ef3f624cbd141edb5a326c6144ff4e1a288ac37880c00000000001976a91482d84482c5279829de23e688face782ec0de74fb88ac75addf13000000001976a9145d1b842ca9ecc2c32a614c30f3b3c1fac7f9ad9088ac36ed0600";
	testOneCryptoTransaction(hexStr1);



const std::string hexStr2 = "\
01000000FD8502E1347D23399FD3A1F7727CA25D909A3EF4AA9878B9202452D2634353203761F30\
00000006B483045022100815A50A2539A30059BC6F3C75B8516C8EC32B25A1869BF4CE130546B9C\
4B045E022077E854F55F908B4614A3FBAAF29D7E5EEBC19EA53D0486C075BD2D56E417D4F401210\
36A58499FFA8B923C0AE701DB3DA4E72D6B277996FA541ECD8F42FC5185939A02FEFFFFFF666AD3\
4894B4D247D089EC4637B29CEBBB1733A14C6E8D2F7AADDF343BE34F27000000006B48304502210\
09310572BE9CDD6FA4C983045E8250E89D88EBE3C3AC81FE2FB63B442C717C70902203D599C459E\
5B9A7F124715C4834211776EAD5E88FAFFCE3438DB3F9BE0842FB90121027446B54DF6E3A97A651\
3DF3BA4CA45DF3F8A96C99E9B151DBFFFE61BEEB05001FEFFFFFF4AD6052B8100E294AED5C9AF9B\
C1ED70998A8F504070B2F864C26575BD9AC200000000006A473044022073D841E5BF81C04F6C1B9\
277744D4D4FF4862DFFAF84FDA4CAF96273B381E3D60220084B1677E66DE5233C5EC170636161EA\
B1B6DC4269D028015001989622D6CE61012103038554A075D7404F7334D69017790045AC79EF402\
31213E7CEFAE0B339653130FEFFFFFFF949DB31C8F209AFFB4D7A71BC3FEFDA5C82440A56E47B9D\
304FF18483A297EC010000006B483045022100E169131DCAF390884BA423521F5708B1DB157F430\
E67C01F4A154F2A66D719EB0220038773EC671D17ADCB164FF536285C1EBD6B41ADD305E6BD095F\
2C2FCDDD91230121037CADCE8C53C7F011B8CADEF863F6161A73AF71037B60C678BD02A9FC4B720\
479FEFFFFFFF3AE7D0CFFB9784ED2245D6256B79D11743BAEBA6424FA6FC3A8230F48CFB53A0000\
00006B483045022100BB9F2897945A6A71C0F9E92A6A5C71CCFCD73AD2FAA444341D0683C6C6AB0\
D1402205318BA2A042E67800E93AD1F5DDBEBDA945EC703A9EFCA031F32B5375425542001210255\
831F80FD1F41FBFE76F6BC339A4F2C0C790A42AAD63B1A3FDF154636010B17FEFFFFFF692C0D984\
97E6DA2C4204B68355F2EC5FB89DABD2812972F7920B877174B0DB0000000006A47304402204363\
AFAC5E458A24DF49E1E71881C59F722D4A94C720D8BEC12B2D5C9686567D02202B767924457A85C\
6EFCC84125DDD968C818D81BA6A76595E08736F7D90FA38410121031085F00BF01763D5179126CE\
C844C9FF27719DB0CA850BA48E7FFF2307886F10FEFFFFFFB5589C35EF054F908A2EBC740AFDAF9\
70A871D90F4FD4239ED435F36EAFCCA492A0000006B483045022100A2F0707E5F122EFF8463FC24\
FF57C05B01E683496B2C375C010C3DEB41F96D9202200C072933DF5B0112249B996AAA8CE79555B\
AA5DC9EFF96245081EA41A5A795AE0121025BEB09562B689F4ABEEB93FE4CAE25DC67B31B003B91\
E3C760600AF035CE794FFEFFFFFF6A1536CEC91D58300BB6606DB6ED8A0546C9523D9531FD667D6\
2BA2309234F00000000006B4830450221009D79CA3F59FC20FE28A45A1AFB41243E6D134C904869\
04C69A81FD84C09EB1960220121B826B1BE5905A80BF4090E04D9AF16E1E028914A856D98CD1C7E\
FD6FAA4D9012102D2664F8A1DF79CF6CFF79E9EE9A6451490B8552EA345C70B90DC9DF09ED25265\
FEFFFFFF86F8E0AA72872336E9C436EEF2BCD42C91D6D6FE87DDCAF33540FB464443D6CB0100000\
06A47304402201BFAF2ABB99B442128A2D1EE4D4C9BF76E2FDF0B679E1FE0265114AEE39FD6B402\
200D3F6AD4CDD6FBFBEB0DD36403D9654BC7609CB520DDA55381F31640B5D97938012102D7ED110\
37C72C93A499AB41EE27B4A413DAF92189805DF13C816130E7F3C20ABFEFFFFFF21A717A370AFA8\
5D843C4063510BCD9AD4517D9B0EABA02C086ABC9F45B3C6AA010000006B483045022100F3E7DA7\
E90C8EA1D518E629A9B257820AB8DE0279C23CD14CA97247C100204CE02201A86F90B1C66FBA8C2\
A3CF2CC0B411FD102EB1247F72EB36F72A5FAB28777065012103DFACCA3E0082FE75F00D371EC8B\
225870D9DF99EE0BA3BE45015CC776D7B3249FEFFFFFF98DBD98F62F2D1E76DDB12FA5E1DF64334\
58C3A05C78D439AB6EF85A5CB9E6E8010000006B483045022100F048398AA967274D1497298700E\
8D2221597762DB19E9D4945DDA8F3A5103AD40220281059EE7D9D7B30639379062A2C7C5EB57F57\
9627622B274868F278BB9423B70121034415E4BB1C962F61892EF5FDAF01F0555B813828AB7BF12\
55C822CE8C4EE1FAAFEFFFFFF1EE0B47203DC4F91FBB35DECAA7EE27171756A93925FAC93A08CE3\
BA086C65A1010000006B483045022100EE8906846004F29359A1FA10814D7811E2CAADB822EA69B\
40A1987936F7679D40220227DCD8739CAC4416D6EA8870611CA69C7ED32A47257DF3CAD04B1BC13\
5D8DCE0121024FEAAE7CF932B52DB9964B0705CA666B8BDC6A9DCE10945DE8387B83EAD2283BFEF\
FFFFFBEF21BC84B00173F1A7EDF792F80918174FBF8FF5488B94A6AFD740319DAC076030000006B\
483045022100E4B61C74DBF16F5B0E913FC08B153CB52C98CF04BDC52DB542D10687BD549309022\
0340D09DAEC13E62F6BC625F57BE081A5900A0304DA1F0CCD0C472BD3C04392CC01210333378235\
5FC8FBA9F29F153EAE2FB08CF72A49724457A72C3D4AC1832F26FB16FEFFFFFFAF10FA4E14D63F3\
D03FFF2207B404C043563E07AD4C70E217006C09C3578FA7E000000006B483045022100D4E38766\
A9ACF9BBB6851B846EFA7181D0F59FB21EEBC7DDE57C1E7DFF698C47022058A3EAC53A73168CFB7\
7D2318392ADF358B508533B5EE3265EDE88F42727F820012102BE22462E820576DA957C90D9B4D8\
EB0A78D2235712A328BADF2E45B315E0218BFEFFFFFF0192434F1E6B7C96F26541A3A4BB10A1BB7\
3174CC86676EA6D2C3FB8940B16AA010000006B4830450221009A6F761928452EB58FF6D1F0F5D9\
6FE8322CDCADD1F3D8CC6EBF88CA602E70D2022031C0AB7D3EA0451164ECE32A5310EDF0A7AA5E1\
137A18FD5F8D950CE4BE4B625012102C54D44712CFF0D8399933A10576ED75C1A62F3579BED748C\
7E8F29C780993B9DFEFFFFFF41AFA2470C678068FBFAF6B13245A0D657EA52171B5FF2D670BADEC\
69525F5BF010000006A473044022057E1D44A3FC477968070A9934BC725B42AB7541AC4C7217725\
DBFDDE116D7A42022033106F1D0C4C6C5463712461C01E319C661C6D85CB8CA7440F7E284EF49F9\
50001210200BC2D4F19FC6EC365BE69694F17A51F2EA4FFD814574A67D7624ADC563BBD96FEFFFF\
FF045140B6E183045DB8262905214BA5D53E40ACC17E49514BF6DE601BAF869585010000006A473\
0440220560084878F84E06E8C808C9B5D38785A7E0CFDE9584156CD1B0EF198DA08B844022054BF\
072B89D0D1A5BEAB682C0F235D62BCD496E0A7E49DB1F37391CCC339A0CA012102FF825AA2692D3\
B8552FD0D9E70A1510E1CCED1984E41E22D1663C10739D13DA7FEFFFFFFDDDB80918B264DFD8ACE\
28BC0992D2F34A3F5CDFCC313993E4A237A106B5B5EF000000006A473044022072E3542FDBBF1C0\
1E24C63B986FDCB1F40108FD002592919B355529F558BDF7702205F502ED3226FF39D102293E1B8\
02E4DFE408509E65CF9D99FE4A9ED484DA9C240121034F75FA30073B6C808692316341550CFF2C4\
DC9B77FC26A0820CFFF4666904708FEFFFFFFCD0CB00055DB123554D2F1D5AD6A0E763481C68116\
F6F3C8618DBAB883C3C544010000006B483045022100CBC1B8FA488B9C943DBC7B1B30568022AC8\
57471D59896F2F8519F0D7DE819D60220105D007C761102688F6ABD3F2306CF75B798CCB9C16675\
61928E901DD09E1BC30121020AFEFFF31A77A56EE76489E90ADA68DC41F78557DDED947C133C170\
A6E0F6E08FEFFFFFF60E52B66E79F5839E443128AE20B3F9BE2FCA3D5BDDC571F6B6F3D6E8DF747\
CD030000006B483045022100CBB3ED24EE248A247F73C30C14D7D6EC458C02781ED53E4CD27DBA8\
D745A0AA8022078986E17861CE9EDF8CC5904303187F69CBEBF80A9AA93E67D9BFDD2C8A7E85501\
2102808BA9046A0BE383DD2ECF8484B29F5A4FAAE288DFDA241CC1CCC7C11164067FFEFFFFFF9B9\
112BF9B08C261435C939D0314FBA4846650AA5F5C7CD81A1C436DC97E7047000000006B48304502\
2100B5898A99E5523979B14CE4221E776B7FF7C944787CD927D867D6BF2A48602EB202200EEA49F\
264443E1B86E95A8FC0A7C3816DD7D2945E20A27CDD01BA55F1C1E8C70121036A58499FFA8B923C\
0AE701DB3DA4E72D6B277996FA541ECD8F42FC5185939A02FEFFFFFF33F769A9E404BE5A754C171\
52456DE1032A1B039D8293759CCC6B7F37583B365020000006B48304502210081DA4946F81E9A3D\
562E22A24A611A1F46C458F55B4CB91B5E2E10CD45A438870220090DE2A7218B08BE2B9DB4ACC36\
ACFA49E4ACBADEDE26C33EE951F5000C284F7012103A791139B5432DA6E8CEC38C1FC150E07E4C1\
591740CD457F248235E5ED04019BFEFFFFFFA8E463215F8921977F839E504A239860D317CE456A0\
5CEC827BBCF9E2A0BA90F0F0000006B483045022100D5F293F1FC991263E94847D25B8E1B462DDD\
E7DE76651D0CC8825584603E5DF802203F6C99899CEC1073866DED6604B77E42065C4808ABAE188\
11C4BF8EC244F0359012102D8A99E12A627F434C4FC319C827FAECD459724C0F4CD89915F0EA93F\
B8F40ED0FEFFFFFF5C7E5D431C83CD75E77B6022FF98CE2F919914361146EF2A38FE3F530AF6B14\
B000000006A473044022017E32E5FBAB81E44D1954511310A1376CD7E5F68C2B69766A186E5FDDF\
84AF4B02204733CB6EB4813B5AFA8A878A033574EFB254C5A166F0265A7DFD31011C2541D801210\
3EEAC46F6C9CAA056B8494B2EA39F34FC2114FEC7FAC29CD121AEA92873652C40FEFFFFFF08793A\
1A289A262B8DC7CB683BCD180FB9B87C339ABAF3597FCEEF6094DE36A0000000006B48304502210\
0CD8EEB12AC62D91D75EA46B0FA5AFF69CF7AE599E536B3A1CC9D7A0ADB4053D302202125C5890A\
2F5B047B656B4E7081301DC42B1A50FBD5F80E72401D92D76FCA8601210395DC1EFB7C3CB3CA772\
4E28C5F181E4901D57AC560A6A6D9917A444A27B3F0BEFEFFFFFF74F8E69896B75FEF06CAC8B167\
01BC64C92C99D4ADC34C46C3D002AA2A6AB671010000006A4730440220186BCC9891709C78C5E72\
3C71586D5B85F5B3042819CE2BD6169A84549815200022015151F78E92FC1BC11FDAFE770917AE6\
F683CAC9B4C08511838F617E61CDDA4E0121034D2981EB5F31317437C59B14337921F0ACD3AB131\
1D70BA118F7AEF0B3CE5636FEFFFFFF4CD1446BDA2FBEAD69F0CFDC424F079B43310D312B1ACBC7\
8E5E04741A3B9675010000006B483045022100C49B5C075D9910962023AF473BA9D8AC973392CBC\
F46EB2E0731D06EE709AD000220226A6C9C53944FC029E69D9106CD52343A26EDA702409CFD09A0\
DDD40AF661320121037346721F571AFAAB5E467523250FA01A6F8DF7F34CD14576530576CC8D29C\
A51FEFFFFFF6F9F1918F0056BB93DA3BC53922D4AB08F339B07FC0231EC31057A7D9FFC79850100\
00006B483045022100EB42108623F3BB0CAAC54E134D0F65ED747AF844B06E44DAF119E2B0A745C\
90202205C8C0F1655937360B96B36B293CC75A971F06F588B20D689E98DD17BE0DC83C6012102D5\
7FB0B846458E33E9705E6095D2D50630445EF4F3C5B3C495F0CCD0A915A422FEFFFFFFCA7B17448\
2DBF037A395BF4C6B7D928AA673C29C3B7B71478267600B20CA658B010000006A473044022036BE\
CCBFFC0BBBBEB44B00334B01ABB2CCE4BA41AA26DDF8F74EEDF7AD7ABF74022035C0D913772A47C\
89079524AF84DD98C567D9051570CBFCF2EA32A32DDC0815F0121038A48D1A23AF4232AF471D6DC\
1A191F19C411B9B4549FF53BB72D8B836FB38A80FEFFFFFF17DB83A9663F40EC68C030FEDC6949F\
70A40CC7D0134B10BFD3B8D813EB1A561000000006A473044022051395A8D8A7CF9DCC210FCCAF0\
F8A88F97E9ACCF3E81791E7AE0D6401DC12234022055F13A86B54F10C3D8D3480EAC455411840CE\
CBA21C1E94A55FDE1447595B3340121035EC054B822DF3134B448370FE74F0C68DC1AEE82C33F76\
CF8EB22758EF83F6EEFEFFFFFF41C5E8C7705F2C63635DCB08E3B78D5317252E94CB89207D38978\
FBE7B0A6D31160000006A47304402200BE91BA2297673CB4104FD669F67AF506725CB1B6C84A19D\
140EF27501C3213002205ED5DD30AED331CF948378F05D520BFDB5AC1E0CCDDDF2F1A6465B5D8A2\
0307001210215F39D45615814FD1B2EE5EFCC08E3326C3E729A2530EF9100431BC2932DA855FEFF\
FFFF6296EABE8434FF2CCE8F9744158D82DDDC87D1A44A45B45454FA80DDF41238E5010000006B4\
83045022100B3DFC9AA9AE51EFE48AEDB0DBF69F46F02ED3910B7B61F89B99340C7559CA5580220\
517F598C5E88B7758188D34572831080A8646E3589E948FF27BD282BB9B160E90121021F90132BC\
555A2DC49CD423BE2F2A7AF339CEB78779A6E630CF81A0C63E59EC4FEFFFFFF8EDC8A878DB52D3D\
5E3E33D435EFCF84741DF27581FF0281A35DBE630BF45E80070000006A47304402200F562839168\
4BA07DDF6A5D8863958ABD1B22AE14C204F7DDC79557496E5358302205EABA3A5634D3AAF86EBAF\
BAB7DC432940FF068B92E1034D520C14D6C69A2218012102B35AAE1B8A55B9DE247583AF79C9D99\
29CDC924155EC68AF161795FC41FE450DFEFFFFFFE433A74B85AF2770F6EAA02DA55CF2F11BA219\
30A350B174B3E90CF22E9E4A57000000006A473044022019A8C896E6599ADAFF7FB316B1754D16E\
7CB8FABFB61C4E41A488D587BB167BE022003D2B6EB798957897D1BA0E5DEA58E25EBEF31D9613C\
3937D9D647CF55E32BEE012102162BB464DF6712B35BEF47572064ABD980E74B6BC405DF9BD3951\
CA1FA77AC22FEFFFFFFE0F6B2E1303F84BA0A94FA52732499E2E7170D12225F97C7F3F86B2660D3\
EB3C000000006A47304402200920C0C2458599CF9DF583266B793953777CB9DD89BBD92FDE31492\
E8D8466E10220691891BE9419EE86AAC668E776151E508ADBF85F905716708EF732B9901DD18B01\
21030050CB51D19960BE55FF759A4296DBD791636120803B62B04785351C3295CF10FEFFFFFFD29\
F70009C20B15B61929A7D792470111ABEC5470738422C3D610DA2C0AFC973060000006A47304402\
20298BDFB9D6891AC88DD7AB2BBD971D5915463FA5E30E10F87943E511A02D67BD02203E35B337F\
D7A531C5449F135E6D5752D3D354A37C3C8106AC92DD18EA5F889A50121027BD15982EB587DACF7\
A64BEAFC4B944DE8A8643AA121034307354CF45734C488FEFFFFFFF82C1CA905E1C85E480DA323A\
07B4FA2C22363E03F0703E75B9674A74311AFB8000000006B483045022100ED9BFCBED6DC816EE1\
9512B356C2FB72C87F9017621C4D49BD99C4E7FB90CDCB022003DDB0A7BD8622038F982948C4B89\
FAA11E20FACC9633388BCF0440B3063AE0E0121035BEBB33573FD09D8FD01B2D66442823DDF9F03\
466DC242C18A5EDB46D08FC2A2FEFFFFFF1A95A844A50B70997CB2660AB1D142F642AE25970AA22\
94D87C17BA38692E7B7010000006B483045022100C752246DE1061B5D6C5576108D9E5C67A4D644\
F3618DB688A2F64FAB9A82066B0220348A66539EB983867F7C6E79FAA65663550D00DFFBF0FB031\
8C182738EABB75C012103A7748BABE56541A7EDF65EB8EC9CDE644FD4B0D275361D6A6C515E2999\
403144FEFFFFFF841F6726F6E9376C412C633BB726838073B76B3D505848851279FDD98AAFA31E0\
00000006A47304402204ECD1B12AD4B8CA21845CEA0B1A60514B07BC58C6EDDDA548CF3FC92398A\
F42902200A7BDCEC61055493FA620FD4C1E93CB437F732C6FCF3633021EE877027A92FE30121031\
1CC0FBA4AAFDA59AA3E64BEEDFC29B2CAD5F158D22C16406C8A5A91184A26C4FEFFFFFF56348716\
58AAB7C0F5766D892D8B39BB43F1C36A2662A52342479EA4C1CB4DB1010000006A47304402205FB\
52575439453F061767C7C096958421F5308CF35600488F6A1626E4B7755CC02206C074B9C38000A\
6A8737E2D45F90F262F735871E277528167047891E69CFAB210121022F5CE1C4C6A2715CF1017BE\
F088411D882653EC964D1CCB2EFC826A4DDC2825AFEFFFFFFDB2378B254C95AA1627DEA186B0CD3\
C22DCBB0957823A8CF5D6FD037B1960D3F010000006B483045022100B511436D8FA4C3BC598DA45\
823FE35D05B5D1B8F686F96761245235A65555192022053EE283298952FA0D815ED6E3FDCF5954A\
B7529CF17BAA468D02E833F77A10710121021CBD3F41F174F6E7916EA259FC6548B26A2EC4085DE\
0A16FB2963AFB82361223FEFFFFFF49ECED507414C8F9645673B792CBFBA19273BC4E69249C75A7\
A190F75CD92CD2000000006A473044022015171F6AA28EEAF3014E9E668A5D433DCA9A48851AEAA\
6F28D8F51A6311D971E0220117BFE1F09EAFDA4AF432D5DE32977F1A6657D67964728875A40C264\
237F4B60012102DE0F8CDDA4A52033CC674F9E0605A18DAC834C232F3C5F5ED5FB0F889260D4E0F\
EFFFFFF5286A43AE6D07ED0040263F0F5F11ABC462CDA60BDBC56945314A9E970241DBC1D000000\
6B48304502210082CF4ACA82D55DA9AC4476E8F1D394EF6A7088B97C9799D5A71E340215E11A2C0\
22010896BFAE4CE0260CC9BF47AFF5070FDAD59C4035625B53B7672F15128257CC301210346C072\
E77776D9B2BFE64013494360FFA287B1769026EE575E42C54EB6091DAEFEFFFFFF2EF03E4842369\
03BDDE95693F360A6E001BC4A55E8B2E86FFF08ACC7C0236EBF000000006B483045022100F2C5B8\
33018C3939DD8D246135C7BA861EEFA654D8467AA8D2E735ADADDEF5D5022057EC8119C50AB196C\
62FAFCD57B654B1BB0E39248579EA8EB10597579D1243A901210366401298DC13541C2AFB00ECAB\
7A628077CA2B8CE7F43E82FD3D29D1D65F8943FEFFFFFFD8D829E29FDA73F8AB3844B269E325CEB\
2DD7E3A590098CF2667693193776BFF000000006A473044022072095A7AACB3DA9DA284DED51086\
3A41D0CA788226BE94DDF0D5D30CD97ED224022055BCE498D669166F3EB6BA45075F4D7B3FD8D3D\
E4DB7F715E022B6FC10660692012103A8535A6B4512B971D57F9164214FFC595EDE693373977424\
10B4000D2A335C33FEFFFFFFBF27CC066E97812285E8052DAA14486213DE6BA961A3F49EA3126F0\
BE96272C7000000006B483045022100DBA2233597D211FA94FDFD6D8D7EE8E9E0F6A85F5255CF5E\
465C0380F1681C0202206617CF724A6CA10CE760D68CAC83F97385632B1EA0FC8C90D99FCFDE22A\
A4A5E01210277E67AB5D0109632285573E3E3254EE3990EB67F776EA174E8096DA926F85F51FEFF\
FFFF584C6A4E6917DD5603EA72AFA5CB1654A6377D77D5951FF740B1F99B28C214DB000000006B4\
83045022100A264FEE7E6FD741BAED96278B3CC22D40C7B26D935DC0B77DD9F1F33AB2529540220\
053373775AC62A0BCB0B89E4F878FF101BD508347F74177BD2F891A6695BF1AE01210326249630B\
E413B523AF79F16CA3652BD9283B5E35AA29B18B96CD36B3A21A6FAFEFFFFFFB5413A356AD5FFC5\
7C65B43FF507E0408CAB51F6FACB7865EAD8819DB1015A6C010000006B483045022100A1626A93F\
BB5BA1A44FB7D0BE76A10056758148C61405B62E1A54BECF26EF6B30220269CBCEB835F79716A72\
5A6BDD1A2E03841AE3AA2331433EF889CB394E11FFB50121023349EC5AEA87BA7377A8710701B49\
26EE481A88B53349A9E38427BC1FBE2448EFEFFFFFF7401CE1C5190000B2747C177CB161493A880\
3E7F45093BB6A62955FEB83BB0F7000000006B4830450221009384780B0D5D0BE1D35D7AC09043C\
81FAB00F7106CB60A71A6E30689B990C2C70220211ABF6E30021195C8EA5990973E5E615A84FE2E\
117A5F18FB78571AAE5CB7F10121032740C0ABB716B6C2D5F3C3BE36CB8FC277CD44C02CE658447\
E0119AFA6192A86FEFFFFFF45C74CE67E25DBC781B5E1B8055EFF69D258D2493BB79759DC266824\
3F1D20F4060000006B4830450221008132A5768F275B3FB84A2C751E3B494328403C8E7805E2ABB\
79B0D4A2835016F02200FB491905CF5BEA1F3CD956E0E94FF441558B01F16FE37ACD3AE643FBD9D\
05D4012102D8A99E12A627F434C4FC319C827FAECD459724C0F4CD89915F0EA93FB8F40ED0FEFFF\
FFF93C2E9BA036C62AD149246A7B09149B54E5729731E5C2476B1D9AD2F9593554D010000006B48\
3045022100929555F6B97123964396476B0C751E7D9F8F962B72F446A15A3F1F43722515D602201\
A2ED0E3B8A2BC3204BF9AEE692B6708DAA2F0CB779FA41D54B89B32AB64B1DB012102302C154968\
D2AA02042326F66D0950B5B1629AE1325E939012AFFA1889BD66BBFEFFFFFFC9E9F4E5E7A327898\
578A4264795BB76EFCD51863B65023D4B545B24840E7328010000006B483045022100C583D6DB7B\
03A3EF5FF845683A43B277694C06B056C3426CD4D379D66A0138F2022076BB94E07DCADEEE198B3\
11E336068A0E4C10A311525B717EB0487F56F71D0000121020656FC1C1FA0EDB218E001C458D419\
59DF223A42279B8F84399AD7E35BD4327EFEFFFFFFC168A35B37CEEE17DF49866161FB4AC726F13\
F8F306726342CD4590E87763188010000006B483045022100F7182E7293D54FBA6770EBE99AFBFD\
CD152BA82AFDD5836A94DA5A81DFA5109D022035C7B45C241E55AA1E19706BBB715CA8E3244138E\
1AA1FE3FBC6497C92A504B2012102CF14A5CDC7D3D584F7C7A3D50E69A31C13EC44BC4E11AB3AFE\
9E4C7BC2581CC8FEFFFFFFEA1FC888C122A8319284A131FFB778617DEC62328435E030C138628B2\
B9AAA31000000006A47304402207158BB94A658C86E54F6D3AE19857E3125FA18D2626B114EE8B7\
494F40049E1A022012477D829E6AB6808660A5A18AE385EC46470EB0CF706CCC41C9EB4D553900A\
0012103889EDA6BED724CB4C145E581581CF6686DDE6E7A0D64BAC374A61999F8574BD7FEFFFFFF\
F06F9649735AB51E28213C192C213369824572832715DEAA8EDF9B316EDF4BD7000000006B48304\
5022100DB86BCC6999C57D2938FDE4806D1B410C801B67B10C8BE1F4633CBD71704457602206B87\
B792AC57F7EBCB23A68A9D8D198821A1EE91114DE8F43B96E9C63CBA0A32012103A60D7BC397DAF\
7F4DE7CC2D7597E0B165FB1088B90D11AEE247E9C1E98D779E6FEFFFFFFC71A66961FE3A30A5BF6\
DCD3CB642F2F4AD635FC8B99F3F27B023302681CD6DA000000006B483045022100C23568A8399C5\
EFF95392B344B43C179DE362DE43CDA8693DDDA2D4940AF4F3F02205F96F37C70595203439A03AC\
DB14E594C46D8198FB5ABBE5EF3652C572038A1A012103C42BACAF1855C4D2FC371F5A42B507E9D\
663E418BD361C5DAE948793298D0989FEFFFFFF90F501119CDE05E3FBACBD7C145C3717B8A804B0\
59C44D51D493B12682FE49B2000000006A47304402202C9DFE3148653860A46207307E2CEFF9218\
52C5EB8DE80D3B317D8BB11957CD902204F72EBDB9498A96BD0B8CD43E5E4A075BDF833F4D2A0CD\
16E214BA3C29EFFF5901210307175B77A0D2692D679143FFFB27AE4BA703B869DCF770A8511CCFC\
F2EE256C1FEFFFFFFA3EFA481CD9E98B7D19B9C59DAC3387B6D7D8B3FADBD8795B0D5F39F979E5D\
0A080000006B483045022100AB38B7B237C0F25D3BA568A395287D1AA73069A049E6E8C1E30DAED\
EA780B42F02203D382AC6C9753B0AA887D496DAFAECA1EAFC9DE7102E58C47AD8D0DC9771D31C01\
210259FB805DC3043660FB49DF1E493EFEB82124DAD38DB8D4565CD0ABE8719481B3FEFFFFFF770\
014D3B4EA4EBA2AA9C1509C21F6DBC15C2A83ECB5CD060BFCE0C1B8F9DA31010000006B48304502\
2100B7303681B9DC130027E8F41C333D6595F23BFF4D1CCE4C0F2FE949A047F578F70220386A16A\
0354857F45F52BEFAA47EBCBC08C8E6FCEB8340BC05E8BAF7FDCD6388012102B6564D51A9EEBF69\
FC07054C910AFA2731F28524B1F5A8F2D6025E2874215A8AFEFFFFFFF12574F0BEFA84C8721A8C6\
F5718B3AC3C54153CAA3995B03135AF80B8D8E9F1010000006B483045022100E23C98D42CA42A54\
938252C19024262F1FBD998D856824A5A78B7EF452BE530F022013942BE05888929EEAD9F6D8E9C\
4B2AFB0F6B3C5342D57376988F43513C49E9E0121022C5C931F4AE3C8DED179367154AF7B326E9D\
03ABC55A2E7D1249F65AC17C079DFEFFFFFF7E7C3E0846C729C24E1A6B6F1412F5D2A7343FEEE53\
303E9AB12F56A993E36670B0000006A473044022010697079E3B7C5285501718F9268C0C362A64F\
AC219A8F7A6EE93262051386E202201F7B58E81854FA78134D6B75D6A9ACDD6824E17BB0825607A\
9601DD878AF407E01210261AD022D2777266108676E5772B38F165164A20B115240CECF51E297A0\
A92E2EFEFFFFFF2EA538DDA05C0E64F881FE3EEEFBAD62BC32F34ADC886D04415EC24188B0419F0\
10000006A47304402207FCD84209172763AD371D488EAC254D1722BA00C332B1F3631BCF0D2A540\
1EE20220322F55986080B39C0282ABD7E7B10C2E8002B14BA4A821AE2AD34E59989534D30121038\
E2C93CCDE23CCBB15E95FF177B8405136DD62DE7D6FB8074422875A11B54024FEFFFFFF06C697E2\
53DD01270E6EFBEC46435A4988EF1459E367BDC27D947CDEBF62EAB0090000006B483045022100B\
48EE3072BF4F02618FFBEA1C40664999490D0FEADC15A69C57F173821A1A38202207C2792A3F423\
B15CE07717D3A742D14EDBA78940F46EA3A692EB0B4BEAD7A4E6012102B0CC1C99217063AC054A3\
33DE6F536D4FEFAD556E6890324B7B2B665AD359D21FEFFFFFF72E5D73BA143838EC2408C0B684B\
E64BE9F3034410F87A86F636F17750112C68000000006A473044022007A23B7ABCDCF2F8480634B\
87ABBCEA0CF8AC5BB7F418F01872FE18056001318022004302F7A1691096184A9F6F8B652097E47\
A299E1077EDB5C6A44FA71C61B98EA012103C8CF5F2E5055F0CDE0111FE44180AA8114D249C3E8A\
C66F24963A301177A13E8FEFFFFFF2E0ABEF536D6F21388EEBB25D4693F1FAA59A2487FBFE9830E\
97580F4781EBFA010000006B483045022100DE74B5BB0A1FFB294AEB4FB94992556C1E389B2A5D7\
F37279939433AB0B68D6302206DD7CBE8478A00E5389924DF5BDE28A6AF47D4777AC50FCCF2B3E4\
F169F762C3012103A292F01D89C288FDF83FBF90E9C817CB65077734D83C32A0CCB6B0DD3B0388F\
8FEFFFFFF7FB40FF82D918AB058130F586AA75BA2585F8BE17457C1A6F8396FE17C80690B010000\
006B48304502210095FD56C6D1E03DDB7FD9F9C61FAD3500CCF0E32E770971AF0A7CBDEAE015402\
902207941A53FF626AD95CFEAF8D75D25200FFCA8F8772E8F6A326B26C0809425133D01210394AE\
100704D1DEA5BC787BB6B0A1DF90C76A71E6C81C05B1FF892E42DAEAF1D7FEFFFFFFF3D9697AF11\
9B3A5A80E19B964E4B7B922387F07B491F5499AB6AFFE65B5FD24100000006B483045022100CC0B\
66384803A6BA19D988A9A6DDB188C4229AD543E6D03E3702502C28A3865002204AB6442E6C887CD\
9ABB06643BA0E576495A43BB9832FDB6C4AB8942C363DFEBA012102831CF5A785A36667F3DBE4BD\
129B72ED201E066285B58B909F3DA937A39207CAFEFFFFFF81668096AD5910B2343237FF46E6C1C\
5657C8620AA9415FB0F8D3B43563892CB000000006A473044022012A4DDE7872F5C9E6752486840\
BFC0CE3BC4300B8A6D5F8BDF2708384B27C202022071632B437629100F3D48767D32A368EEFCE0C\
525D582EAF33C920AAD55CF7E0E012103B365A9D3F3B367EAE9858FC0835F2A40754B376C24A5A1\
8C472654EB2F5DD398FEFFFFFF2A47967FB09659CA55EE0FB4A61DC6761C1AC94F7AEFB7621588A\
C713778B3BE020000006B483045022100DCABB838E9FD4AFFF0707340599F3C7C6F63A8B065C296\
A5E8AA72BB22A0D12002204A32222FA722ABAE7F2B1D50CAB26959967991CC3A4E278B990DE2658\
F930F390121027C09FA05B16C65FC4B44072DC2FFD6292037FDE75412DCAF9FF93879AA51BC36FE\
FFFFFF01CA5811C0FED6DFFB7D0D672CBD3DC1B56FEBF431E9247332B3BD868EE6151D000000006\
A473044022063D6715C8E6BB065EFFC9B09E7EE58A731395332A1BF7931952843407FA0827C0220\
623E4567BC986B2393DA81E8FE9F587ED1F2141C284DDEBD2847626F79F2392001210383015D273\
D1FF9A8C2FB2F6135ED5B5CC5F193D2F2EFFAD335088A3FC36D61EFFEFFFFFFFC212DC2C4A46CFE\
837B91279FE2F0256F11D77BF7502286E5CFBBCC5F57E4AE000000006B483045022100EA7BEE0B3\
D205512202F3A4A82D1AFE31531335CFCC58494AA1EEAC90A8B5D350220084E42CB3A6E138E381A\
0382662A0B91FE3076D300F7C4F8C2127F3C916AD15201210357B5124C520A275A9C77006D30D2D\
46D122442A7A956B7B85D2F7B4DC9A53A25FEFFFFFFC562672A77E9CD03D3B9BBD6682DF22C1277\
0CFE02599C658BC1B8CBC4CABD38070000006B483045022100A670BDED19323C9620D4CB85165E5\
207C393E7B81433673A185291B4831DEF13022031990047E7196B9EAEA05D4A52AAD357B6F352B4\
0059F4E48F130AD2CEDE825F01210229D741CAD5147BF60FF32C7C9D65CD748520C0F8E2DA3A5CC\
08F72357DC1CCA0FEFFFFFFA771BB3455C5FF3DB3A9B8140D825CDA0C1A68E4BD485A4109B6B91A\
2DD16610010000006A473044022077E1FA470EAA63C1AB9EC97B8A6E12A17B5547BB35D5B113F3F\
E7D4490BC3D8D02203782D01B3DDADBFE2AC15A12EB6EF7FA5FE0FA765B99A35C4489D608E51E2F\
C601210375E54E6C8B0C42D810BED4E2333FF4906EA3D6A7345393B9D725432FE6AF63A1FEFFFFF\
F556936D637C6784A184EA0606E2F3B4DBA7F78CE8B974EF4F2C7F08C9F0C02A6010000006B4830\
45022100FA259E1A536FCFBC114EBEF89FF86D5C68B9FE1C2F1EC9C2EF2FAECD65D073B7022040D\
14CB954F12E4CAA0675F3ACB091BECC6327EF81C48C17545685DC2871210C012103C6010E074F8D\
29085BB94ABF276405057434B75C755B584C849AF14FD47B90C5FEFFFFFF49197A864BA42378589\
91F43C41F71AEB6D0DE3EEB72E69A56A2F2D0387D62FD010000006B483045022100BBDEA0A9FABA\
6F1286076C3EE0C74A127ABF13A696AB5BF3B1B35F0F119143A502207F4FE07457AFAA88731AA9B\
078C42ACC064F1A738C86ED7ABE385F7D4DDE5671012103246384F8031C9FC972522BC20AD84021\
CC0B13CACBA839B50271DCA2D4C4A767FEFFFFFF1FF0CE8A020E0ABA5BAC57F8E70FE8F380C5AE5\
4DFB922361DBE53BE4672C9B8000000006A47304402201E00BBE6004D2D247B2A3DAD3D437A403A\
7577F576E1E6526BB108230939607F022008F3033BE9BB13DFCCC52FA5D79E099B06C1CB91924FA\
A95AD7907D241C5C653012103632ACC08C3F0270D7F80A11480E1796AF7CE276D9A086DA9CCEB89\
3941B2AAF5FEFFFFFF8CCB63E50B4FA313EEEAD2EEE3320D11AF1D726A33CD43D9BC5466B43F4B3\
A740C0000006B483045022100990C9A7A9C355BD86E5FD33C43CCB57E9D6ABED536D61EE293FFB6\
5F6E2B23840220082D962186FDF22BFD61185F2BF87E78AD672F427F28E79BF49B8AFC1EBC14970\
12102079D9889F6DA1F8329598BB9BAA45DA1AF08716FE5D3322C1D631228E93E4A7AFEFFFFFFB9\
7887E79248AF509844A150EA8552C097E68A93766E47CF61A73D7767B7BB61000000006A4730440\
2206A691AD9E3D565E1DE196EC2FD61EA23F85518E65C9AD782FAA168CC2E6AE1F102203D61B39E\
0AB2CAC32BFE618D2A8C989D95126F3D38C7981925406B727436C0EA012103615EBCEE084CB3FA8\
DE38AF91E5726BDF33053EB28E5C0A3365A11971CE2D9FDFEFFFFFFC46C28AFC72B959203018B8A\
62CC6F7C32414BA60E19DB608B2886872D4420F7010000006B4830450221008E3EC874758CBBBB3\
FF95CA51B28A9BF46ABCB4189F06E06E12F4319592DFCEC0220518EFA869738845C4C6BB2548B4B\
4DAD1957FE0FF34AB142900E3CDBF169FAB201210288926817429C419EB1A318E761D116553E7EF\
5FB60082587C935C29C10D7B639FEFFFFFF5ED327F16CFFB6B04472147B7F8B02DD2F002E96739B\
32E4DA43908D48C2C4F9010000006B483045022100D8DD4EF3DE03CF91E7564575EE25DF4A8CF1C\
05C7F94E65DE299645BEF84163702205A882C1BBBC5588EB8E8AEB7ECA5745713DFCBCB0DB172D4\
8680ABD0160F72AE012102302C154968D2AA02042326F66D0950B5B1629AE1325E939012AFFA188\
9BD66BBFEFFFFFF3979D7BB7D8A29D5F8A0E514AB31D434C0EABE9ED4F055EC7F561B25325B7BB7\
010000006B4830450221008A1DA1830DCB0BF61D876AA12ED9CDD960DA109B227033D4800378C05\
D0445B4022050068B5013AE3BDAC4CDB8ED4B11DB0862B96A1645C39768927D2973A1DEEB480121\
037298D173E9BD6B0146C41FEA6639036F89774FF48DC2C38C1296314E80082B92FEFFFFFF39CE3\
074B233D3DB918F32298E4E40287B70B37F3EFCD10B953EBA0102EF2641100000006A4730440220\
2ED04D03CD4DD3DD55C255BD576556DCE9926DF99E1B6408DC8B86A6F8F0986F022042104329DB5\
5F8FA25179E56C2C24828F9841850802546037D131294254A25BD0121020611EE0C5BF1CAE9CB95\
FCB908D67B18088C543BA1A0BE6B212E399BDCA4FCBCFEFFFFFFC3F178C8D297B291BC6BADF9762\
1372BF6798267DC2BDC891A637210E37BC5CE000000006A473044022026B611CB3F032544E28B3D\
D6AAB7EE47820FA291F082D875E9F201F82A55B7D302204E400E4DFEA34052C7673BD0A94B84BA8\
C29DF5A134DF6D6AFB5D7CAE903EF5801210369D1BF569AF6EBCD224EC753DC8AF26817321A4C19\
B668C855110B46D536F133FEFFFFFF03E2386384B5AF076EA504536707758F109C03FC523F15CB0\
758FCB917E341F50A0000006A47304402203218626FB1AF7A5970CD1352F52C2AAA909D2CBD231C\
343B898FC4786086343E022064ADCEE3ABAC5AC371626259A04AC849258710C5A489D36792892B2\
F997100B9012102B25A36AAE7BB38C390F00FFBB8AD89E8733A469CE344866AA1149D0B6C206369\
FEFFFFFFC4C064BD21AFC98819C228A33B7C9FCDD426C4A17E8CCBF840C224E195E25AF60000000\
06A47304402207DEB1A646645F7E3C2D13ED2E693254E12231C8D96822CE5170D86F4F4205ADD02\
205C7A4300DC55BCE067CE7E43B65E8B15652B72B5413408CAB9047613D510C9AC01210267BB9D8\
F4F93F1EC7BE91C25CEDC6BEBDDCAEF3382A5B13DF40EB6029F51840CFEFFFFFF2873E7C395B780\
975E76471796C47B028F146A286008133F4DD5DA6469FA0377000000006A4730440220608B8F002\
622791334972983477EACC352E6CD60091D25D811A5FAD3BA9D212002203737905875BFB0874473\
ACD732DC4C68C6698670D05DB2FD67F9479C35CD305D012102ADC90D04FCBDCB3EF2C7AED59EF53\
A22A23105D8B8766395EDBE9E89C4F7E8E8FEFFFFFF4C2A976935EAD2946A5E0E522F5474098CC6\
4F651B38AF97386F059DC133163B000000006B483045022100819EEEB2759CDB333F0FB57897226\
FF73B77BFA53B47C1B75DA99A02C6333E720220781BFD4C0AAF86EDBC1491473D50CEB6E57B07D3\
96FF16E1E3929656A13781C30121032B7C054FBBC06C41ED3443259BD15A9CF9A37BDF7A5A65E4D\
7FECC606E615595FEFFFFFF8B3A72B91F771689B1BF5FDA6D22E7347EAA9C3A9F2A030980E2FD4B\
EEF47E63010000006A473044022011839F4871D7A9719949532222CD069731AF4B6F7D74CAB25B1\
2668B9ADE21D30220169A5ACDAF6921D247DCB132C6EB1F4BBE8A0013ED4DAC849F1DE8E9BBE15D\
3E0121039CF25B642EB6BABC5225BC7DAC64257D19652544C33A5D69C8FC444C9A817F17FEFFFFF\
F1C837DAFE65CD221491E6008922B6DB2543941D34D036BAA01924180A7920A2B0F0000006A4730\
4402206F60D3C19F000BA11F9778F2575FA97594339E85A4A266CB10A0C4423B33CC62022052778\
F921FA5B492100F9687984B106E30DAD6706733642321BE4102E5373C3D0121032F2288BC860217\
4C0E1F2B063474F6B28B0DBD821C8C37912B0493E6756B199AFEFFFFFFDB74D000F359660AE9BC2\
7BE8531EF5F156EFB9B7C199A6C47F14916404C2F8F010000006A47304402206249B74A52075895\
392E583EB020B33A9121479F4CDE9F565264C26F065EA9EC02205CB43477E318991E9B998D609B6\
8D9CB898751571C209CA0EE4F740E2B4C0D0901210359C1C5808A144E0D72CD83AC652FDEE42A5E\
B681E3686DFD70ED3D46A16B6579FEFFFFFF136D9111E537ABE2F5E54C7B0EFC028A5EC3B1DA299\
AA4445715AF26AE60BDEC000000006A473044022019B961558A36CF54BAEA4B372794E6E355DF5A\
AADA59B09D3CDC1D8C181C627102206D2F66A5EFCF700BE6259479DB5D02B07FCC2831700AB9471\
88C73036BDF43B3012102B35AAE1B8A55B9DE247583AF79C9D9929CDC924155EC68AF161795FC41\
FE450DFEFFFFFFF4185512D249E6928C4A9023BC25FAE0F6B73B4AD8805599CBDCAF79222DE7050\
10000006A47304402205CF358B150AA42F770D4D28F54C0C19A08F384C0003CFA93F456F0FAF93C\
FACB02204D3B4AF22E14A0E3F8D7330B667E25F4175D83CDE64DB760970778F3601130E20121027\
B76AFFA76C59FD4F0220C942D367676B6AD873251164A911653FBC5307B2550FEFFFFFF04622450\
B41066C6344D9D09AE8A0352AA4D747093F68217461FDDC209631E7C000000006A473044022031C\
B0A3CEDF6BD743436A9E29A8F855BEB75E587EAF071B88C9ED70498BE194A0220753D3FD7F95F12\
625EB57354974DFA860DAF284F897170156B23AC92DBE82888012103A08C4DF68BC4947725D173B\
8EA9D90EEA59856EE70C8308841B954EF855885E2FEFFFFFF30EF400D8BBC0F319301B7A9F5BCC8\
9E36FB506AA6696DAD7A9F775A17ED0198000000006A473044022076CCCC713AB5591EF93891CCD\
67CAF24A08DBE9F563C8A7F5FC6564647E5D05B0220221C1424AA558B61EB10EEF9576D32D30BD8\
92CA5208C9642CC2DC1357B7D738012103089B10086D9B86B1956FFF0E5DCCE2D8AD265BA001EDF\
D4D06CE4774E1ADE091FEFFFFFF9FF7059F015B9C7294CFB57AEA89374DB3CD6B1E6E56EAAECDC1\
9CAA656F4B10030000006B48304502210082D0CBC635CB1FCB05C3285FDA1FC31655A7087B0065B\
83C32A8385294B61CC402204903E82B8C6517F46BB07A78A9F817C7FA084B92E737EEEACC36799F\
A6580150012103230252564FBBDA512744E6C68D8E0356E856529EFEB61B94B42FC417B824263CF\
EFFFFFF9FF7059F015B9C7294CFB57AEA89374DB3CD6B1E6E56EAAECDC19CAA656F4B1013000000\
6B483045022100C0BD4795E1011485276A26A1A69AE6802458E926EDB75113C62054F5902B1C5B0\
2207D6961A636EE794B05BF19F406C454140937DD18627D40DD26B0FA006D3849690121024E2C13\
CF61CB6AD3AC48101E93786E431D260A4DDF2E1A64E95946CA9E196CDCFEFFFFFF929BD3BDCAEF8\
9947A90E4747C469B5F4B26A496AACBAAE1E30D2525EB5AE674010000006B483045022100A17937\
DF55409E1EC8E8A55E19C12C73A9A4AE1D74F3723F30279FFAC13B04010220164207CCBEDA01C1B\
AF923C37886C79C83B40C81D3A7AD90C2F332926BC67AE2012102E2E078AC79E57449AECB8992CB\
FA95E67A7A141567D54F5BC8BE021BD066C1CFFEFFFFFF26B3AF256A8B76778DE81BE4D611E20EF\
7171AC99B804FBCB1CF48BB21125B76010000006B483045022100C333DE3045A431B50892528C1D\
7FBCC604C08FBF523898C8C44D5EF65A26C6CF02201446E9E2A720103D6A88EB025690E5B4C5FE7\
0B9917A3682229C37DD7A906FAC012102DB9C1BD4C5CA2ACB094C88821285E77A144B9F169E9C4E\
10A9146508176706E3FEFFFFFF7E15825B09847AE699FF5B73F53A22D52A4653502EF932E90CAAB\
6F3AC265FCB060000006B483045022100C0A12622EB35D1164CDBF94989075EB67F9F565941A58F\
AD551F17DD5092C44C02207EC2425D655C5E23E015298533B7F5C638C57CB915978743994AF2851\
531806E012102973331FC9BEA471B144218719606741BE5E31656031FC32A4662BF3001850FF0FE\
FFFFFFF3AD414F90528D1863BEA87966902BBF180F27928E8A19DDEFE2366F3FC0DB31020000006\
B483045022100A9E5B5165785A5AFAE41413A5505F827AF6A55C7FC96C779971E787A330616BC02\
20668957DF8313B3DE38CAE62C003BACEA016D3D45B0315A3A5CB00FE1DF93D1EE0121030B539DE\
63EF912B693871E9FB583C0B2047429264205EB0C10FD2BA41C144C78FEFFFFFF04875653E7E398\
6A3EA99DEA4ECA48AA4D4243A2DEC3F4953E730E4592276816080000006B483045022100B8EB3B3\
35C600D2B483F366F8CA48F4A12954136EB6902EA2DDCEB98CE5BB57D02201A97F41DF3A63375FC\
7C7CF258A3EF2ED8E72AFDFC9FAE2B3231EFF9CCEBF8A8012103CE2605C58574B3A66156B0CCD0F\
3ABFF2892EF3E9FB33F4AB074AD4184FE1B4FFEFFFFFF1B80566DB3C5FB5BA3BEE3C77C02D3BAB4\
8F0E3D887B0A91F8C1A2E0C3B6F337010000006A47304402204EBAD21C0E04F1CD9547D3AEBDE5F\
223A3F8625C79060E25D44E6ECE7E0E59E702201CE0BE51D36FA1FAE003EF88BDF2077095A02981\
33CD3A0924738D0DB42E4A510121020FEAB5EB5056F4A8793A340D712D1E835B225DA22D903BE10\
57EAA7AB754C5B7FEFFFFFF6A3CBD4AB7CC00CC680F61FCA2633C6619319966055A2FAA3A67E10D\
D651D198000000006B483045022100C057082C7211FD737A80E167A46E9FCC7E9E64C5A72D91CE1\
C569411DC35273C0220231913A7BFC79C5DA0C412D56F6D8F897E83A7B4FEFD6B39C63AF3C0D8F0\
54EB012102D3AC7D51CF639AA97F72AE13ECA389DEEC8227BC112C2AA0DF9C488DC6AC8458FEFFF\
FFF812C7B41C15506C35319B2AA6EE3AE9640593B6FA73BE731FDE7637696D5B2A00E0000006A47\
304402201920EB3770D9119F2A81EAADEB4ED465A3356F88238842B59DE89173C31920A80220241\
7C0F9BD21D05AE46812F299FA639EA70011C30A0F2C19373BB47BBEFE74EB012103F53F7DC6FFB0\
3F6B6F25B2D37BDF479D5B315BAF84F5927397C32852D5F4F93EFEFFFFFF4AC032A34B4618C2BB9\
38B6E526940A897950D7812DBE6180C868B69E6427C81010000006A473044022002184858A625FD\
4D93D89ABF2AA7C8A8EE5BADAE95245B303C0709A78D4F4F65022068AD7EC7E00E75FDE7E41FF83\
506E461E614D22F49FFD8E20155A4AE368B5FF301210290C71DFE9572915F57B4C1EE871AB9A1BC\
34A9405083E5B0AC5D01EDFCEF2EC4FEFFFFFFCE83C936740D5633BD0E17F71EC78321C94C6867D\
9D931398359BF99375E4C47010000006B483045022100B4D3B8EF20A8F889CA66653E3B4AA69054\
B45F2C313A03E0FF235D759E67B72202203A11DF5514BB9BCE83BFEE7EF05BEA44215C698B4F0F5\
E075E6E28D3ACC212F60121023A4DA8EAF5075648F044643E07708E746E06EC9C14F7019E889F7F\
911760EA58FEFFFFFF97E78B46794C9027371A198FE131E39F7DDFC4A7E80CD575DF50951FED527\
C43000000006A473044022018348C7425F0134EDB6D891A1B92B25DA93A57C8AF83B93015D57F0D\
4147234002203A33AEE46AD96CD70357A6200409445BD5D744F65476C33BC50B689B4244C3A1012\
10259C66F8DCF65138EF6A8E4D52873CF86E7DF2C71CE513A85184BAD320AF54A00FEFFFFFFFE26\
A47672EEDAEC46E17E0D84B2AB63B47CEAAFC71DC80106F285FAA30A336A000000006B483045022\
100BC38213D9364063F08426AB82CAC2A7D2BD83087149A8557FFFE3EF15E6F90AC02203571C5ED\
83CDF9441F713C1360312081F403D876556580FC2AD7E0CFB60B0EC0012102523A8186E0F629B61\
81CB5345B183064833CBA7D73F84A0034F252789E494B60FEFFFFFFF877AF14F462F88FB8661261\
CABF548A21326DAF3CDDC9B3E96186C6D44F2D20010000006B483045022100AA48F18312EEA9F2C\
44860D01D43B1BDB83C5BEB67C08C705089541A1B9A307D02201902AB2C55C2F31A09255D6C7A77\
7C288D7C6BD182E04384801E19C78C91603A0121023D73CB1DBACED72E1DD15D6881A37A95BF2B5\
301AD35B8E0BD0D056A673F7758FEFFFFFFC3BB056B1574F274B01F653B77956B02858418958D5C\
5E597C750AA0EE3ADC52010000006A4730440220643AAC158D74BC8854AEDDD18E284FA720658C8\
CB0BAADEF0CF3D035AD4BA83602207800E516CA795C837BB4F6B7F143E3745C8428582EF576F8A2\
2B8A39FDF222E6012102175768057B7CEAA54D559782E5D031B8AC48EF8E5736CC8F9624590F058\
0D433FEFFFFFF6803F5C9C6C2700B1D8847E4D3AE1CCFB44FABB9A4E6D01AED69EB34B95A2AAE01\
0000006B483045022100EF83301398A0871189CDBC4897BE80410686A7EAE39CED51ECDBFE71ED1\
EF4D9022069AFD05AF968FB638BB14E44B4FC7CB8506BF11E35BB88E54BB1CFC3BAC80788012102\
9DA9FF2863E4D4F31EFD4C8A19FC4131843C8CFC25CC79DBDFB943645CF42846FEFFFFFF5F2E294\
C3DFAC0E5C0F7DE7E2BAE0397AC0CB83F468C3F1528ED1D51CF198C5F010000006A473044022003\
F6CAE5453E0DF686E4F8714925A705F1A2F78E604D2B2F572D4863AB877F7D022042A0D4E0253BA\
60809D985632C48E24EA33D0C84B23C598A89A74DB2F92A7B4B0121038016A15C474DFCD819EF6E\
20E95108BA8AEF42F0AB7EB08E01D346AFAC1F1FB3FEFFFFFFF91E29BFB9444E437F401775C4A48\
67DBA76397C0FA58F709D68D9077FF58D5D010000006A47304402200EA696F04E8763481E725414\
405FED754FED102F2CC26114073BFE3CC9ABC4C702200BC90D58AF99F0351BD7A8581F9D870A4D4\
BD4C9D7A21B1EA69A0EDA381E4FC1012103CDCB4BDB14A5C41126BF4D6455BBE3C15EC3E4211E1C\
5DE4BEEC80870827D04FFEFFFFFF5A8A8D62073F594294FA68E9DFA260785A53D32B7891DA2BF95\
00603A3BBD1B9010000006B48304502210088C9A212CB87CD0620E477B2C2F5FE20EA87B02968D7\
8C137DC7151FC9323B9B02205824CB301529F5896B3F5809394277A1E59ED51122CB8D83A77BDC2\
3A84F1BC5012102878557E6375644DE56C85EB3A30D6B8DBB7DE6A190C79EC18C9659C12252EB56\
FEFFFFFF38453F3160A35E5738F4BC676A6CE09CF9BFB1A2A29B764768909275E55682521700000\
06A4730440220466EA17FD0A803CD4BDDE719E3E47150441BE516EFD33F1F8C92F1E1974B86D802\
207E42BD425C26D0EEA68D0199CBE09CF96674DC7410B3EFE733584CA4808B828901210243C9F24\
24FEFEB1C34C36A3048852A6E60B91B21747443FABC034427652C0494FEFFFFFF0410617F224E16\
B72850A17273530BC65F3547F6071C13980D55EE49E4B756DA000000006A47304402200D18BA33B\
73601836159EFFF4DFDA436EC2C3BBDBED464D24B8128B0E1B5156F02205DE9B8607D125C460A71\
8199A40859FBDAAF37DB50DDE74D7613C511986AA4EB0121032E33DF1284BB0E1FC4967F29CA77A\
80056DC75C5120852717881D3C4BBE249EAFEFFFFFF45D5159122AA72B5C18772A14DD310028A43\
F8EDF5C2DA8EC49F05AF1FD89ECF000000006A4730440220726F43531E14C34D7368DB253B1B208\
1263D9A08CE76CEFDF0F359CB54EB06530220354A954E4D098182834BFD162F3E3D2EEA0EB6DE52\
F0BC0C0C2F57364207F77D0121039FC6C5D34CBF156E592840AA77342BF05B7A276FBFD6EB17680\
0D054FC159917FEFFFFFF845E8E5D97E9CB6EE75148FF62488BAFD89BDE71F53286E00F28FD2030\
1B4156010000006A473044022045184C8898959C3EEAA6A20E839B5868D23EBB4C7740E060888C3\
D841BF1DF80022053AAB1CDB1398F54957E497FFA94AAF9954CADE5EEDC0368BD4214B5511C7670\
012103292E15B6604856E0B7BC18D0C732D0D4E4B960F7039A9F66442CF90E261D9407FEFFFFFF1\
50F996C59FE05B0FA241B066F26AC179D6D6A1BD6998B45358085E389C6F7B1010000006B483045\
022100BFDC9947C935031E1F04AE4616C3A1BF888B36C50C2BD3B7BB07BCCB77F16EE5022018C19\
C01D9E683D9C08E7A8AF9C16D6E44DE8CAF87A8B79941B46DB2360655C60121021D45613E9DDF2D\
09133BD5A3D40B677CC8AEDE1076C5C9E385E49070468F93A5FEFFFFFF82C32133130C9B3AC0B50\
E35D7CE886523171CD386E6BDA2785145BC9436B465060000006B483045022100E95C96C56756D4\
7940345974D00348A33361FB67C4B529E34ED7F59C11C84FF102204F590F0F6640480CB5A3F80A6\
B66DA227BA137E39F6F4C5283DCA64D62C08D4801210303D085D83072E910642247DEE1DB57D0B9\
9B033376BA579D16E8A2544177531BFEFFFFFFEC77DD9BA77EB6C59EC73295E7D5C5B5AFE4F01C3\
6B264104F12404A256741DB000000006A473044022055B0CE552DCCF82F578D5C22FD4B4BC0B190\
FE5E66FE9087A82763F524D13AEB0220438E361F334A57ABCFB9360EF94D3DB885E7FA3D8BDC808\
7900E840D7A8C782D012102940B8C2AD56D35CD1EAE4A50A0669893D20CD3498D1B4CB3345ADB25\
5BAB1B15FEFFFFFFF7D2D00FF8D110B06A019A70C33AA69A4BDB5447404CD5327D3A1DB7E014ED5\
C000000006B4830450221008A535A1009AF7D1EC56139C08799D6F9309CEF77A5622CB0353419A0\
D214F7F40220409F08D48FDEAC2F6DCD2B0A0685DD111E09048E839CB42D5D2AB6BB538BB0FC012\
103A921E3AA4120FA6F014EF696916AE53442DAADEBF567C8E369B2E3C12737CA70FEFFFFFF6DC0\
7C714D5C02C0B914CFE7A1FD0A01A7CA0F53182F4B87BB73DF420C2BE56A000000006A473044022\
0191764F314A88B61B37910E338E0C99FE8BD74DDA33F498B4C4B226818E4535F02206B6A249952\
4323ADBE18BB923ED3E05E7956EFE6F38103E2AB10146C3A06927B0121029606E5273AA4C961E7E\
D72154DD72C6FB37EAAACC5E8D1FC6D84A782E1C4F66FFEFFFFFFE2A651B361B3AE8DC994A80054\
0F91395B26677FF19090C5AD8CF33C5E7E180B000000006B483045022100DAE43EC61C3549B68A1\
6FD16146DE2F5B77D95930E7589A478AADC18A5248B13022008DA6F17B75F0BC00AD3675B1B6F88\
4B67ADBCC4A04E36ACBB376A9588C6372501210396055992CE2F4F9D1BCBB05BEC8513E94C12654\
F17ED8A4578495B6E897B4099FEFFFFFF27C7CA0F9E86F291131BC76726C076CB28369AB009FB48\
BD37063F01F5F274EE000000006B4830450221009464987EE57F2E08534AE716B3FB73FADCA6188\
992E4FB0977248BDEF5707BA602200580CF87DCB824BB4FA35E039B74FD5055834B86890BA72BFD\
3BE79A66AD0255012103D99B25B910966C2F6C859C9F848D4E985C0A64B153427106C5B8CD1F724\
9826BFEFFFFFF0F16BA9196D0D3D5FE73F066CD3932160319CC3C9036D7D454935A186490791000\
0000006B483045022100A381B7460A6AB26F0671C30C24B42CE8711AEB588539E01BFF9E360E4F4\
9B3C802207F210D286547F565ED51B91BE9B7A14B69838A15EEEC47E297F299E20974CFA5012102\
CEDD133F2468641B61927D5E28388C27215698CA4FC961A94F6E244D46F0126BFEFFFFFFC0D0CF2\
C1897E7911DA09E2B99A1BF2BE971B454DFA55D615915FE56859B86B5060000006A473044022051\
430C605BAB1D705243CC2DF7196492F4F066789CF3F7D3C418261A346AD33B02201814E71600988\
B5C10B8CF5F293DD63C2991F8094FD49A571CCB20DE640B020D0121028FCC0984D3651076730E07\
5CBF6F4E826B19EC758F902CEDA0ACC2D2497283D4FEFFFFFF0F9CA4A7CD53468922D226BA638B2\
EC968C7C5130545AB6807EB5C79AED16265000000006A47304402207C53D3229F95DCD3883E83DC\
CFE06A5C13159DC5EEE364A707BBE27EA430B4DB0220474C56102A01E3E18227253C648B12938F0\
3C95C015976206A8C43CDA45D28D2012103E0FB513C63514A030A65660B89E08741301A96CEFAE7\
55C14835EF405573B3F2FEFFFFFF5B7BBE4DFFCF249DC491EAFCA60368223B99ED3AF3EE9A3627F\
A0A1E57C0DB06100000006B4830450221008FB30EC6E6CFB270668CA34AA1518696CD876E685856\
394B84C6A4C122825B9902205388D880614860F1A732E28AE68BE8695C455F5E1C1E536AEF40B7F\
765D0019601210251ECE2837971FF8574B593D1BB02F740AAD7778FA3BE3FE277E744730E215AC2\
FEFFFFFFB9F5EB3D262CFAAFFC794EC222E2C235D432E429903AA01BD827FCB8A5D5E5780000000\
06B483045022100DB2C44D43FE7BB9DB981655EE75CD0527F269F1B83BA463483767D00F2E3353B\
022076D8CD40E08E431DFAB63C9CF53F4427D1474960ECF7646A8CCF907BA4387A68012102C9DDF\
76ACCF8FED4D61ABC440E46A206BC099BEFA42B12D6D66A71362E5DA157FEFFFFFFEBBD772C9A4C\
7B6F8CBD5670093A91D5B264EBB4F43E0E6395A06CFFB9C57D29020000006B483045022100E7452\
B615C851C245FB3BFAFD51C809A738CD5DD5F75AF7A4373EB14870A8F3C0220377432B0634E3D84\
83C5849A75D3B0C187BDC1BEB57589EBFF60DA248D6DBDE10121030002FDA0478661C94EF7466AF\
A46E91AD2F479A23D17DA3B4469DD9FA6496D02FEFFFFFF32F26EDF8347A35CF7690A23ED96A9EF\
1749AE8284997F9BF45A8F5A9570567B000000006B483045022100BD7C9B79C285F89D6AB3A975C\
5B00B863B4A11518C8B6512489F6E7DC49997A0022040292689DBEF0F9401BB2C6BAC03A61EB59F\
20E35F560901E0F8BF3BF30B1C01012102B8A82FE975E8A695E52DD8B6A4A931144681872F4E310\
F5C657B02DFFD3A6D2AFEFFFFFF0B110349296CF409C80DA930D0EAFD3132AE9D1C3DB55326C5CD\
AC97274DC0D8000000006B483045022100C64D2732AC93A9F62C98CDA99BB6F3898C3A22FFF0C04\
D77EE5536940889B90602203E6A7E9C371475B1C4611E309A4811DEBD3055C535A1C6630584ABBC\
31F5300A012102E097BE265A2BE3F3AAFD482C8852F536884D10699D0144CA7F2CECDEE5F6AE38F\
EFFFFFF5AF0E085BFED433F54F87FFF063525CD005B329CB43B0F57D5325ACFE949F9E600000000\
6A473044022056DFCC3369B22325DEC6A1B6DB66A9F312B6FFF732F6172B346476D3FC8E7932022\
05DFF76FB508574E21F1A2BDB72771E3F3062193FCCCF57F11B303E5F547B51C7012103F794C5F6\
FDEDDC6FA7E3F21930D58D31E7AD5A4084B3F65ED034EC34157FB711FEFFFFFF7CF5A9BA79400B4\
74B20E1E69E08BECA1D679AB6F686DAFB64182E1BF70B567D010000006B483045022100DBA23EDC\
1003D5C5314478BA83D86F16BE053B0DBF28F24E89A493CF93B0D9F202203CA254D42A82F0DE1B3\
DC3A8327CCC945D300C94EC6294E5B5BA1C3627A1C1170121023E47C5E17A1D465E5FD4FCF57C98\
C1C1F3751268B292A8A56794F96834B37DF5FEFFFFFF98CA80275CA1484F3C2F830FCFD9E288D97\
6A6C80C8B607D71BDB1403ED14C29000000006A473044022059A8DC28391D2664AD164A4F521B12\
7352EE5F6639BD4ACACE882C31FC8C997E02202CB605EF919DE64B8A9B69060EC91D2EAD010E5BE\
AD19C723F3E3CA1983A9FFB01210204A8C01B87607843CD3CA6767F295E4EA1DF00349A08D8A2BB\
AD8F3DB2271D65FEFFFFFF14E84C3F0552E9508FA584585CD7389443EE88288DCD8FD12EC7706A7\
5B7834D240000006A47304402204BB75BDB86EE53093839CE16E792ABCD3024C46CDDE5C8879536\
0E33EB2D13C702203730AC7B511DC8CBA2B31FEA8D3B3E048FF4488D109514CF20C52EF3762E5C6\
3012102DCEECB2104F7D9CF47F637EED322FA20B5F618AF037486F19EC59DEFA177F561FEFFFFFF\
CD9E58B39424FD07ED9FDDD539ABC0E3C98C6A387A9F4A43BCEA953DC3B4C9E50D0000006B48304\
50221008E018E0AE2BAAA5E6173B5B193C7C1109FCD819A29D2BA9951041E45AD32C4C302203DDD\
006FFC6E8F9DF8C16A116E30AA38671D126401FCAAFCC1474A6C407FE7BC012102986E7492D3597\
68F99A7056B52573AFA082BE45D7545CEA6D54CA1396A243ED9FEFFFFFFAF579081879197541FCF\
14BB5FE5902AD449897E7E10AFF1E85AE28ECBE6E8DB150000006B483045022100BB86BE140D42D\
CD25C3DBEB046213F3A91822B0B9129E604377BE18D0478BEC802206696EE5E39ABCB817C29A370\
9166AC2EB0AE898FBECA4E2F56F2FFD57B86164D012103BA02EDBDB0C69572F93729E435E1C65FB\
FD22EDE4F90BD843D2F5D529214BE06FEFFFFFFD31489449CA13CA17E99A82DCDBF72423FA97C52\
9EBC4515AFCD41B0CC6AD972080000006A47304402204297F7D06AD0BDD7EC93A4D35523D63B616\
9B8B71DC3357DDAF4350CB9F7B30B02206869EC34CB8B70E24566520B936438D82962DBF19CD543\
3162F35782082D820801210334930C37EA515BFEC83D6AD9726A03B9CE5EDEA18794B1CC847AD53\
A1A488ECFFEFFFFFF3E843E80F9AC995F6357C2CCBB8F41AA4F7A73E2E839CC08E4734927BC95DD\
D1030000006B48304502210095907405D0F6D8A690DC701F898F14946ECE249763842366FE06870\
EE995457402207AF5AC6528FD566620C1451501A96EAB6B70905C82EE524E299FF8D59EF4E85101\
21022F55F49F3E9C345B1D813A4616488128599624AC7C89E3E39AFEBB16CE325DA3FEFFFFFFBE0\
E798A397568C92BB9175C42ABC47D7CABA0EA6951FBEEA4CB6D6D5E4598AB190000006B48304502\
2100EEEF17139815EB9083F0B2FE0144F05DC7F1F37C81BDC345BB20C1C3488587B60220365A667\
845D7383C95814214CFD66FAB9987FE76B4CD1F4571AB08F780E2E3CB0121027C923EDA0AE55FCD\
1BBD41E5C889EC96EB9A730BC6461B1B72E34B12FF6D63F1FEFFFFFF72B3BFA64A014CB8785CE89\
65556A42600BD3010731B96C19EA60348BB04B631010000006B483045022100AB6F760BF478A480\
9708BE72A9DB0BD79845FCEDFBE02931AFC269C77FDE67C0022017C7D240D318D8F4BEB8806393D\
DEB5899C8B6D080AD54FB3A1CB13D7C4A17880121037EB509076FDDF7982B12F97537C0E6513B8E\
B2DF6C9AF19A6C6B02C5AA0004C8FEFFFFFF563CE78A25F9C1461F5622989FB70BDB0FFC3505D6A\
166AD4CFF5429D2C4D0B7010000006B483045022100BE60BF2774B4F8D00CF7DA5F745E86B0591B\
4FF2DE2E75170C4219808663A18302205C6AC5C5ABD40CF90AAE99DD594841B34A4E04FD01D78C3\
72EC6A0958F220A350121031B9540FA3396EE7E2012B7DBF644CD4649D87BEE59AFAB2E7349CC92\
7DF5923EFEFFFFFFF62557EE421BCAA28194708E3AF2C778DE7F2DE7AD16286B404A59CE3A958AB\
0000000006B483045022100A92D812207DA7BDCD59F34C8DCD255E296A212365BD0D08BB9ACD94D\
1336A79802200455B96230EBD30A812F0687CFDB7E9805D90EA41A4CD45754910571014125D9012\
1026DEC40A26A8ACF8D269EFBD2E639473056BE0AAA6C4E7C39A2F49C7CA2628207FEFFFFFF63AE\
6921698B6F5A3D8F9EB627F6CE189824BC469CF7B7D65F2CAC744B270EC1000000006A473044022\
045B7411AF6D70A5081B793D67162C88901B14D089531580234408E3653A0973E022015298966BB\
95A2876AD313CAAC5C8C5D12079D7538CE210112BF3347F12EF30B0121025464EA09F416E6E9C04\
B372B6E9425D74E2863138E688711FF266A24246B5A19FEFFFFFF8C1758DCC89536F5BE364083F9\
D3C3CBDBFA42915F158537554F7E3458E6A3DE000000006A4730440220074FC0D0412DE62FBA656\
115995E758159AD105C270306C84BAF94A6761C084802206CB6F5D5AD56F3D4B0062637C6C91864\
BFE434D537A6A517E1559B66D1B1E5B901210342269D177D6569F3A0775B8D8A4A7DA96D003D1A7\
D96F7000AEF1E131B028104FEFFFFFFDFC8E57366010864F25306CD842F8198B9FE16B176BA8D02\
59421E766B2DE2D5000000006B483045022100E42750A125E8DD802AA8253A5BCCC2692F5682260\
C3749338DD0D41C0C3D12FB02205213B41C0B510F409075454F15235923CEFAC9D20E61B88C9A3D\
1E54275312680121032908AD7318DF47ED41A59E660152EC85385DFF845ABA6A341172B5EDE0D60\
E3BFEFFFFFFE2242589233BB5565BA213B67E55BF883546ADC8612E5F4A4BFC974DEFEA3C580000\
00006B483045022100D1DDACA80FCAEE07FC67483EAF2BBB3977535EA21B08B8C12A70ABEEFC9C3\
43102202CD93AEDA9039102FB287FFF46C2415D80891F51C6CC3B6486B3EB3CD4547D0F0121032C\
6D12D166604466F9CC6BD2F244648B630B5E3F0D3F31684861021C1BF7E0BDFEFFFFFFA682BEAA0\
F2AD3BEFC1CBBF44773AE067E3BAC799F4543F38AB2700608D3CCF6010000006B4830450221009A\
2B2B00E9F717A7486A4ED409D0FD588B98A0346B7388EB849CEBEDD5FEB53802200D852A8D2CF77\
151B85FD994C10F8C9EBD759E2891ECC5F8481B7AE8EECA80000121024C21770DBE3BFA49E14220\
42FFC01153176C96B89D64DCE8F416A152C53DF66FFEFFFFFF835A8AFEBC897CB10B401D4589E89\
E264DB25DEAE4335C6D2EFC0CB7CA31C199070000006B483045022100FB65ED1B9AE95151689F1D\
0C37F2B08F12A0F6263E0B2E3CC41EE25F80432A7F02201508DCB0C671777C62F1CDE2FF7241104\
A855796B7D4A2414F4080CAC2503E4001210234DBF6C8886135DBD167C34A408D1A2D8B9F5EAF2E\
B9AD3C6885453D50F397F8FEFFFFFF2E191C036D1FD0A3CA65880C609552A6B57A0358A31179A3B\
00147636EF0F8AF000000006A47304402200790FD1992FE33F81DA462B913CD5FFFB2ADC36438F1\
CC900CE1ABFF818F88D70220791EE88256DBF925BF922771D3D31D9E2D859ED672D2CB8464F8815\
890E39D01012103D0E37B4CFF40A35431713F2291955EB44CC9A736AADF326E735C0946A7963C75\
FEFFFFFF4E4200903430558253EE7922563AC66C1415ACC1CDF989798306621ECD4B50FD0D00000\
06A47304402202427C5D1351469F56EE3871A387E6E2ADFF06FE92E3C0CBF8278652A562E815A02\
205BDE8FBFAA97B91251C059A0D3F87837400204BB891FD942F1ED4AFA4EE03FD3012103033B2FC\
94F0B22CC6716CC50FB6C7BC5315BF9B9E92B7B557423FC344EC915DCFEFFFFFF6A5F11447837D7\
02CEAEE5CA21351F0E62B15EA4F6E323EA9FAEA4A2280C2D3F000000006A4730440220444B70C58\
0497821A04B6580A6F3BB4944D26989EA823AAD91C4F446B20E421E02206F725A28DED10C1551FA\
66995350D8C4C5FAF737A2670856A455F512B9C59EB5012102BE7F5CF747F2E85DA97CBD8678A02\
374E0ABD6B6CDE837BCB60E25D5315CDFE0FEFFFFFF20F145326AABF7BF020A55B3696FA48F07BA\
BEB14F8687ADEC6CE8A5DBFC3C83040000006A4730440220696D2A11C031E154D4182CE4090FD2B\
838336D025D82DE354A696C5A76F52135022023125A1F4A828521F1C26DCDE3D31F3C301A6424A4\
89DDA790488D4789DDD4B4012103C5735B11726CC53F4A24E48E16B6A44FC2977B618CD42749213\
C3156D72FBB9EFEFFFFFF2DB8F5E12C538404F1840691A4D62D35FD693BF12CA37843146E3206B4\
466645000000006A473044022010A3AA2695065E4F2ACD141B300DD69A3D9B022CFD431F722AAAF\
C1FE4F23D97022052A9761D3BE12E0E2A85F713C02115AB0E757092CE0D39553A7340D8849606D7\
0121022D34B3E6464A0ED1543C5D912EC8E446E4F68BE7941622B97C9892C7D1267F79FEFFFFFF9\
47B6E805E09CA844C8206357CD2E252E251C8CF497215925D23DCCA3B616781000000006A473044\
02204E5500BEF00742ABE9975CE5A3C81F5D8D78B81595F70B957F5D3025B9F0FCF8022029FD3F2\
45BC90ED87C93EBC42227E56241173725A3B3958ACFE06A61565D1D350121035736ADD861574C51\
F212E629307C36795D6C5E6E055E3F8DDD23BCD2E2B545FEFEFFFFFFAAD6AD8E3EC142D4D785412\
0436A2842788B40CC9B5FEDC605B2ECA69F29EA53010000006B483045022100BBEF8F5340488E3E\
5E9610C9290F560A1968C5349FE0DFC2CBB1BB7F05D8DBA302203ECA9BF308B44F77C8C7ED0696A\
2BD2D5016D2A711BA3B2703DB61B1F61B27A20121037A6606082F73CBD18F373253AE40DDD550DC\
DEE7E28F6B05107546325DEEF18CFEFFFFFF4AF37120BA9F538161B16BF4537988ECF5BBB740C2D\
0E30C1F3D2FF0F3DDBB45020000006A473044022051D8E783AF619A50F15059F7AA5CF20E9C25D0\
FAAEB7315EFB902528A620C98902205A115F01A4D4F739C5D5C6C6D88DA9D13433936E4BFC7DA1A\
032C53FC0EF484D01210215F39D45615814FD1B2EE5EFCC08E3326C3E729A2530EF9100431BC293\
2DA855FEFFFFFF78836A93AB0296AF53734AE551AC2F74E3530CC868273269167F0E075CF3E9A51\
A0000006A473044022003EB95BDC2B55943F6C1812A51C55DDFA8BA259DBF017625F87A45496DA8\
86720220581F64D43522A4E6C20E514D6E1EBEFB3414FD25C799C29C9B42A37EDAE9EBC30121021\
232F2E26670ED75C6D05FFF50D4345503F7633DD70197991228248DD942F6E3FEFFFFFFE4F793CA\
5B37543893F02D100B5DEA08EAF1430301088EAE1BE28045FA267FD9010000006B483045022100F\
7A7207C9CFE6CD5C3CC69DD6A2DEADC263104EEBB468E3B04971B317A3BC64E02204CBC08178DB2\
643E192F64920DE8FB943E1C6EF4BDE2EF6DA4F69A22B3DAF22C0121024B0E0CDF76DBA647675AE\
B0DF7801A31AD752936BDFF47D57F9915E016DD5069FEFFFFFFC02BEFD7AC9E7AEB430F895BD7FC\
407B2550501FA8B9A6DC39DE3E34AE9406FD120000006B483045022100C479490FFC7E60E9F3034\
CE9DF453967D734B1C7263FD6B582832B5CA34917D602203778A7BEBAF261E628BFDD418651FACB\
2D414F1D9637B8AA49790B6F0E6A114F0121022783D4F25039935CD298080FA0FF90C459F82FCA5\
B40DF39D35DA0A90042C0BAFEFFFFFFF9749CDA36EAF0386476D54ABC94A9ADC77C68C05B8ED174\
388C6826FB8E76AE000000006A47304402200D35A514EFF960B185404E6ABFDC50882363CE228C8\
3BD1D86B763FEA34F58EE02204EB13682452F125B2ABD713F023BBDF7960A54900FA2F2D28935D1\
10F8A2A781012103212D81DD01E080D46FDEE416E7FF3C9126BCF92DAA60BE750DA801360D1BBE9\
2FEFFFFFF45F1C361F4A369698B1F1CEB92BB919B4B5CAC9D18E9208EB23D6A107AA33AA0000000\
006A47304402204494796AE3C1D9FC99EB75DDDACED6EFEF4CA4F8464F6B2472B41A5A632116DC0\
2203ACD32231399C9482302CDC5796E98EE514DB0CB8CC0DE665F4308AD65754FE5012103F60D54\
B29E195CDCEA03B74714284C131217B43AC176A90A61A906190198326EFEFFFFFFFF2FEFCE01ADA\
E7BBC69D28CB3DB550E4EB6CE06455C4D1F363988CD8A123A95010000006B483045022100946437\
D228465F3F81D369BF8E194CE390CAE32655B0A00FD8AAB081D6B99340022072E4D68EDDE9A9FC7\
B69ACF0AE7C488864285884895FE3CDFAFA87596509417D0121025FD15BDEE7CB3B054DF64F616A\
C6F53D9BF152C5589BEBB702DDD935E863076BFEFFFFFFEFDC6C4EFF6175770D2131F0BB7510A2D\
6D8E4A8271819F75BE388A414F879C2090000006A473044022043ED5BD4B5809A9684DFE3835BD2\
96536D2495E9A47934C50D09A4147268777202205C89BF2BBA7B057FFE8EBC8FD820D46F39690F4\
9F3D9B5A04D2D01F92A3643F7012103D96E44397FEA57AA45E07FEF4422F538EFA1B45E8F384106\
505550298C9B53C3FEFFFFFF02181BA8A148450C77E2E2BC3FE1EF0089DAA7586D5EC4A34450820\
71C537D9B000000006B483045022100D506FB97EEEADE4718C3C781702CF9F71D1FCBD769AC7D16\
6978784B083D55B102207B48B4EB07569C51B8F10FA5602F69D0113E1050881AF2504C581EA6E20\
B2EEF012102009E7CE741367C5B9DF1048FCC2B7035A4F11C85D1144F9DB62F4FCE06350900FEFF\
FFFFADF6805B0B945ADE19CFA40D87871100A0CC69B87A1B7844F9CA3F29AC578A67010000006A4\
73044022013FCFB9E1D221CB9974C0213B5053DE4C96AA0194E7D97C64FE324B4E9E3E04502201D\
472EDEE30D2AEED55909869F6367FCE42FDE4E60AF2C4E212E8747C53341F8012102A231D68B58D\
FC68A68329869B8CF909D05614D8C67EB9B012E18BC45A640A105FEFFFFFF5EF48728AD3DCC66AD\
39C7EB5E5049347E4FBC4CBA3B1A67E9DBD9E66E1D52B3000000006A473044022058CEF214F8219\
8F1ECACB3612D961284C5F02FB709594C3CE78ED34ACB58A073022078F09CAB28FF3ED86A113267\
166301F85B48C3D75365A72C00A524F93958223A0121020C3419ECDD82885EC0D3FC34391887553\
D0E2774431592F2EE5F073C0E38BFC8FEFFFFFFA7B14E385BA649F0334DDD4AEE9BF6918E6BF750\
0EEF866C26CBEBFF6535798D000000006A47304402206177551D1EA11954FD54A320902DB609400\
C903D54019F274919276CD2CF0CDE02203E014A0D6DBF029CF582052522E288B87FB55B9E02206A\
4B0CE9894B2499C4E301210332A2150494B004C44F232A93B90C55CEA1AAE8533BE415ECEE2DC55\
8D9A91AB4FEFFFFFF859C39D89AE54AA3F21AF589E25F87ADC04EE6147B909D9A88F2B83BBAE685\
85000000006A4730440220718C9234ED8910EECDF523DFAD452217D4CB1A2EFBAB484216A724FAC\
AD75BB90220279C5FD6082F686264B105C42E456A31CAF07193042F0A2E58888B62839443220121\
03D545FDA0B4F35816EFB3A435D9AB14798F247175F1B3DAD5CCAABB759994F45DFEFFFFFFB4A8D\
7CAD2C20CF94B663CCE69212923CC86FF3E7E0C802B9389C96E5D278D94010000006B4830450221\
00D2FC857623E974C1763151A315DA6B329B3D75BFEA1F6C1EFDC32D072BC41627022031902A7C8\
8F6E000C76E6D9E5D7FC3C89EB99AF4CD7A5A014909B19ACB7266CA01210290804881BF495F59EA\
796050B1421BE6702FD2D0BBDC58E4891408378A6C3B7DFEFFFFFFD004589A44600B320B33599A6\
1C99FEA9C57B7FBE528DBB863AA2F69E55F1991000000006A4730440220282D6F4A5DAA13C51F5B\
6255202CF757134204D5D9BA8A8F7B280ABBBDC4289F02200B2DF91D1899862F6778CE4AB20BBDF\
1D871E4A178834B9BFACB6A855A3AE81E012103A1AEFBAF15DE8452F3DEC52E9AF433B0558B1CE3\
0CBAD3FACF0BF8DF165CEFE4FEFFFFFF2368DB4FC284AC6D64CDF9CD32057506FC9394FB5823263\
1834F36D725409F9E010000006A47304402206710DE7E32C563581D798BEDB8751A96BABB62B92F\
8435677E1EDF3B6455E6DE02202E28B3F71FF723A7E8B4A10006EDAB309B93170ED6E5EC2491314\
B99EB4B08B7012102E9262B7897DE5E0B53F84ED7E66095BDDB7E6728B50EB449B72162B048A449\
4CFEFFFFFF941953930AC7D219531030822CF298D965FB712D4871536CF6EC5413D7F3E8CA00000\
0006A47304402206AC950C9ED9FA2601E9928D0A67B1BB110ED534F96F39BEA96B40445C34C05DB\
02204B3A8BA3FBACD166B106DA57362C6284C9FAE7B70490B799287F3BF78981BD4F012103EC701\
474680D40D5B5296E8551571E05A8C7B12376E80F40DB629785B7F9F6DFFEFFFFFF31BCC480F29D\
78D589957F98467FBEBC033E1F6B28327CCF9397F919C561E49F000000006A4730440220052F821\
8CAB6BA095CACC0259B77D6E37387CC9D00A350D985A88FA3270959AE022061846BBFE44085DACC\
3096CBCF06D5B7BFE4059905BAD2E65AC50DFBB675D268012102117D9BBCBBCFB2C76F3D2CE59C7\
25259CE814B69D74391BB13AA31309ABFC9BAFEFFFFFF6C3E9850FCD3945165E9BA22E57E2C5237\
615F73EDAAA6C6ABF8A6821EC4FC49000000006B483045022100E8FEA9D8AF8EC910963FE2EAC0C\
15EDFFB724E49099898399DB619771D361DD1022044AEE59BF8AA336F8111740C629A862D8161A9\
6D09891654339F86B5F6DFE89D012102B8B4885309E97591BF29B7BD52437E36D3D675098D40FA3\
F1179A840DBE84B0BFEFFFFFF8536C47F1576503E13AD5342FCF9DC0A3CE98ED354F58EFA8D7608\
914C6367140F0000006A47304402206473097F28FB472408C63026D07D41C522C42A64CEB15EC6E\
B6EEAC173E5E5B302201E58ADFBBA6141EC4161A268005FD97B3E870BFDA90748738DA81592DD62\
F9CE0121032D2E640C90484346A26C562319476E28D47123F50AFC6B7822CED1335D9933B5FEFFF\
FFF3F24BA2D8D1189D910C38C45465002E8700C04C281A147588D18FCD136BE27DA160000006B48\
30450221009FC2BFE729BB287E4ADC5FBE5344AC1AF8425FB1223DEBFEC47F5F3B1BB4EF7802205\
92BE42B3D691BC8D3A466072BC6DD586216D5F357F1224DBE8E3C4F1C39A55501210283DB4E87B1\
86BC3ED8D37C03F3A80CD1B6F749ABC23F83E1516B5DD000FF0FCDFEFFFFFFECC8EEFD580A2C438\
B9FA8BBCB0C18AFD966DF539A2861253686A879CF0126A2020000006B483045022100A437D59B3F\
97E0E7EE83B05788A1812BF47486CAAB26056A1949AADFCAD3FD6602205926FBD463629DE794AFA\
77BC77D10D82AE0DD755B714BD965A53798167F745A012103311C2C585650448B1CFE16B4F5902C\
F5F8BE89E46F2DB6838850944E31CCDFB1FEFFFFFFF6480DEC2DD422A1C1006355F46F72290E47F\
D57AC11F0C5B4F156A5A6836E4D090000006B483045022100EB9025DAF6D7886614E4A0C9306D8E\
A10DA5E0552F63550F7C6FFEB49D4E153702205D666BAF85D2886719E10186AF0CDCB3D0C49CBF8\
AD8171102670278A3049C7B01210281F3D3E43F1219180FB7FC78BBF0C58B719E368218370B165C\
459239D01D0ABCFEFFFFFF44B8B8A3050BB3BAD4AE906846F4B306BAC23E0A0059A9D6C93F36BE8\
B95085A090000006B483045022100DB123802C39B3AA18C733062881C4FD29E5BE901F8A425B8D9\
0CED03795AEFF002204161A6A8E795C5CD6B9A5D0588A1F83489EF98C1D8402F94A44D81FF3B688\
BC3012102067BCB178C3D95D4534F76903DBB5A256D39D87A7D49C0DE2B04ADDBFD280EC3FEFFFF\
FF49816B03DC982363F22FB46EE85915B27E252F7285E74B71A0F8D25CD3935DDB000000006A473\
04402200C8FB514A1E23B6D64B08F4A8D3BE45DAEA253210B9FA3C98AFCFE53B0962FFF022038A7\
0FB92881D6124CE50182EC3840FB0362B2C2D285F0610AE060BC9E09CF6E0121037C6BC6C44B414\
01672F9F8E9B951A3F7F9BE8429C4F1400D838D3BE2D80340AEFEFFFFFFB3A50D6E6640B5F0C82A\
D07DC031075B323C1699DCA6B75F66E08EF98F633888050000006B483045022100C2CB4E0CF0E1E\
6B51F7B5F4607DBC72655E8DD3141968D3E6E54402C928659D8022071011DF5C0C0843F62296FF2\
9432FCBBCC5C65647718A2DB39C39EE5401C2FD6012103D4694C4B26DDAF5082151DA4C0734FB75\
AA7D7AD180767BCBC1CE244381C108BFEFFFFFFAE37538A69B54F4FC364450A5254E3DF797A694B\
7CAFC292E0A536977A05782E000000006B483045022100B3D001AED2784AFFD0910C7AE99D8C729\
CA9E35D1D96AB20886739571A6E4E1A02201090D1D005EE9B3B501B5E4095BA2B56168280468A21\
99BB7766880ADEF88003012102C9EA6837E91818F1940A14E925845F242C1F0CA63A2EB5D664781\
60FD93B93A2FEFFFFFFD2D8FB11A87CA8A4321F916E7F3F12618A4F2A59D5756382D52B9B59A7C3\
D497000000006A47304402202FD26BEB8D9EDA89D1C3A1CADFE553FD94EC869450609F5CD67D9F2\
465FE1F16022047C93CD4E0B0A65BC4019C4A3881AD6FB8990AD0027D32625A88CCF3AE6142AE01\
21034D2981EB5F31317437C59B14337921F0ACD3AB1311D70BA118F7AEF0B3CE5636FEFFFFFF113\
2730048191F9D3E55E5F7A0A950A6757F9BF3EBAF3B57C1F2A1FD733730E3010000006A47304402\
20709C8D37691F44B28A175582B1FD79D0F57524D61FDB628289782F214F66EF0202201533BCACC\
564641BCFE74E2C3EA437D5E1179A8DFCCB01A438B573C0ECBCE0CF012102426C33A705F9732752\
1887FDBA89934991B6BD0DFBB6395CC003A4DEA7AD6260FEFFFFFFE51667E2AB30CEF8F28D35CB0\
D3692CA8ADCBB2CC4AD2D5C9810EF4EA5E7E80B000000006A47304402204F3573995C0CE3C14002\
1C7F609EBDF1203CCFC2EE13D981C8D82B7A255B54B302200FA768D6C556284343EBD6BBC1EDED4\
A9682BFE61824865D1A8C3510CC205F5A01210290AC90B860BB357E9A44A0FA1E506C06E9074C5B\
45D66847DA99CD6892DA1206FEFFFFFF72FA9E26061BEF818619C3C3F6816EB2E191637B76D0E6B\
CCEF64ED4C408EBB2090000006B483045022100AEF87FCDDFDD63F9F2921BA82A7C6E9F82769E28\
ADA8EA8A159AEC58650C83E10220246C9A3A47BFE37B9D0117DCFCC817B7D2A62835059E0A55C14\
4D8C2C3F323D2012102DEDE0815931A21C6BAA0ABDAE0B61899CEA323D45C13DAE895C584A01396\
11D9FEFFFFFF31B857921B4EFA307D4EE78CBDDF5A3046A569AAA4392B8227C8CC82BE657CC6000\
000006B483045022100FD8DA17256074C5CB457E3D4F3871025BFFEF207F4744691A79E642D8322\
A560022008992FF00FD490684146676A83A58B1DF79C0D3D9F7E0500E46B0C367AD6FD77012103D\
BF122F8DF4722D25645856360E3593380C379DF75FD3418C860FAFAEEFE3F67FEFFFFFFBB5D9F81\
9D45E45E30F8C660824DA082F6A7F4A76D6C91BA7F90F480D0368D07000000006A473044022035A\
7FC7FD667D7BEC1C9CB5888812D2F2D6B5CF597DB4AB82C844AAE408FE155022047FA15FDFD5A34\
EAC75E0088F2DF731F2B9ED1E75CB8AF7238E6CAC535228D63012102C3841FA6CC5D9FD65FC4C9B\
5689B4679838833CDF9937530307E1E9356207030FEFFFFFF9D561D7831F8E26812D07458F31BA5\
C93FE8E2DED81779BCDE958414EAEFD111000000006A4730440220340B804F181CB7E2102F591FD\
8707FBE4C1AC20E52CCEEC43F3609B5DC0AD91B02207F45A98C67265629538FD913C5757A6AC64A\
851F3F65BE0601DAB350550DAE0F0121039FC6C5D34CBF156E592840AA77342BF05B7A276FBFD6E\
B176800D054FC159917FEFFFFFF6DE86F06DA38909EC245835921274F6E0D407A276B903DFB954E\
4CDE701D000E000000006A47304402205EE3459AE51A2411889C5605D17C1A12E7B4EDB23A056AD\
50295B2F86AE048DB022070A1236D90E4B2BA667BA0B845566803A586A91409BD5AAA4529F82FB5\
8B96D9012102FF616509C4B0F585E841CBE28D2D9B1DE7FC513052AE644F9E21E2534110AA29FEF\
FFFFF5393065B10DFEDCDD854F209513AFE56FF5F26A21699420477FDA7C580695EED010000006B\
48304502210081B48E4C8A799BDB85AB03124B41A00CEA1CEF231F5F3CDE1DACE67F58C0331A022\
07667B18022B67E66168F6528F72A5FEA7C88E41DDCF3915B71DEA074D5776AD7012103D9BB478D\
2D6195CD4FA1DA5FE7245F02F6534291BDEE5C3C7F6A2FED4EFA8981FEFFFFFFFFF4AC588E7D5CA\
76F57F45E79EBC70E732D4896831AF11B953D7724EC6BCBB3010000006A47304402204130212199\
B86BF934896F8234C99FD17BD485AE8DD55801CE5044F8870880C302205C464C9C5B6D6A1ADB000\
FB13C17D6A4179DF00A6FC1C24940184B6304DD09AF012103618786FB1F2CC985C1967CB20C2185\
6F453C4121D409A7CB19D15F36983EB96AFEFFFFFF8F20A23F8DDB047DC615F925FC65954DC83A6\
5C22A3AB96EBADFF9B402B0A281000000006A4730440220031D801C2BA7272F183A1D115FF0B8D6\
9A769AF47A24B17C32DE04FB1337A79902204F3AD2D6A6FFE1E93A76B56BD477E8087C46A178E45\
35F6BFB0BE69D0461E47401210295D0CE9907344488448FD6996DC1C499DAA05B250DEF854A1B0E\
C24F502CEC65FEFFFFFFB8394B2282C09D0F1E1C5659C42CACC1803345CFA6960A784C53A26D224\
6EC4B070000006B483045022100831185D62DC6CA15BEEAFF79028968E44E08AF937C55BE9600E0\
AF7E4427C4CA02205E168563A1F706AF16EAFCE66DC716F778658B94ED01A23A4272101337EEAA5\
6012103D5D892B85A08B41E2E305B5E9D443227EED5DC715501134B3D44CB4B85A48802FEFFFFFF\
DE3BAAE1F72591A177D332AEB277F2166888ED9C39627830F6249B134E279973000000006B48304\
5022100B4C4FE1B76BEBEA6CB4B347AFEFA9AABE9BE365687165571507DDABD75A25B7302200EBE\
BAB772935F560F1401A8C376BA7CA957B944385AAA6CE40FD78972E83FC50121027F085885D4514\
28F6DF631B6656153362603AB2D42BB2A0BA665D970650DC943FEFFFFFF74B35DA0694312BEA99D\
A31C847F8371A7A6993AB4A64BD98BFD9B8EBBA487F3020000006A47304402206199F4C5D9BF01B\
71E0811BDDE9217E46EEB1B095ED4309DFDF359524C79947502207B0910B9EC77601BC35CB1C1F0\
EC5E309EA68CD831ABB5DF01761811ACFB9CE8012103D96E44397FEA57AA45E07FEF4422F538EFA\
1B45E8F384106505550298C9B53C3FEFFFFFF962826F513ECBCB19558D14D94C48BC4041891167F\
E610F16B3CB3245E5DD295030000006B483045022100BC11CB0D2ADBE79BFB16F1377B1FA71864E\
9A5DA1A04D0F562AD9381A20B269F022068E3421C7D9E48AC8611D4FBEC329398C48A7B26B88DBF\
BA91C9837A3E361BE7012102387397562A8DEA1394FB1FE93881DB40B22F13E20186CCC67792153\
92E1FD12AFEFFFFFF09B7A3FF13DF3A74504BA8B8D92B56C7D8411F0C7CFA3C496DEB5A01D69C0C\
C7010000006A47304402203E907B5A4B48A360816A545E2A792D54F8808FCB991CD098DF40431F8\
F26A67F022059744A37E789EB002B0A879666EB2BEC62E0D62BBDDC66822194E18A920D5FCE0121\
03CC871C47E4FFADD324E06BA7C163EB534E82716F0EB3A32C142CA3FD78F19CAFFEFFFFFF8357B\
F1718413DA797F07F0DEC362562C023883BAD6A4B7B02FB267FBFC0C8BE080000006A4730440220\
57B6394BC3C8CA12B9E92B8E77FE662B275AB8CF84436860297C04818AFBA1DB0220288B5A8B1B6\
66CC4E909F8D5815A71AE922855CFF1A6E26D3C5C2BF740FC8A800121034B5FF1BBC5904D8154AE\
498543BAF1BC1D7517558EF042E28FFAF765D3D10F6FFEFFFFFFC6528AD5DF0757BD499354573F9\
0F1F0B6540F71FDBF3B339A382C974D51BF91000000006A47304402204E0787A8927084C34450CA\
4FCD6C7B11F3D81E570DBA01F2FA71D57927BE035E0220302CD44EFC8D48574049305BC5B0695E3\
A1FF49CAFE7A531765E161E7097E0640121032A4C41C96456D940F1AC97C735BF8264D0FCA55E8F\
5824AC39774D2F1B0E4230FEFFFFFFDE01A3BABCFE868F12E1363CC08C69F475A91B5FE752D6CE9\
6A10FD723028F5D000000006B483045022100EDA9117DB7731E56B7FB5895B1C07621219543C26A\
D985E81658877CD2ED97E502205C5DBDC64A329335181A5915CFD9FDC7B4B5DB4324BDEF68F7147\
86FF246D13101210375D15C46D40ED1D5E6C905FC9CBD8FBA5A2DCDD6DFD699E299E8B05D0E02DB\
02FEFFFFFF5B03EEB0CD055BC4123BD62CD2F946DCDFB4ADE5C12AAFCE326C0E1DF403A47500000\
0006A47304402202D726EC5DB050A9CB92CF6B96C1D0463EE7992F1644CE5348C46CE23B0F2EA74\
022044AB50DC3DC15B3340C7C47C48AD28308DEC29560EAEB3089FF8E772CB3CCADE0121022832A\
99934BEB0B69F8D7C5632C3EA2F32333915A93FCABA15D6CD8B3A407B91FEFFFFFFDBB6E1AD7275\
8FBB2062D20347230C50799D4A02D59959ACB671349E2F30F059000000006A473044022044BA544\
3BD574BCA9067C51FE73C644AE2B3B7863B8D3FD6DBE1E8716E817BD40220171634909E2A13A617\
BEC71CD96E6E9A4183FF4B3BCD3A4AE6DF3546F4ADA4FC0121034530D4D919159689113254AEB83\
56F1532552F68BDEAE09FA7795D22C172141BFEFFFFFFB5FF3D941482578749CB1C1FF379B51143\
6C73B29BE1B5B9C5BF9B5AB295D8EB000000006B483045022100AFDE56B60C06B15FE347ED5005C\
69D595F2BBCB1CC4873B1A5DF61A62CD39334022038DC0376F43A0C8D3EB50711128DBAAD7511DF\
3C746846514DE7390AD1A32623012102F353C5A38C69E08C87E311F79D6AA1902C545694FC3025A\
DC127AD75DB8F6493FEFFFFFFCD4121D4D3A42875AFED6AE2A1283707B505ED8CAD730E237462EB\
365877C533010000006B4830450221008ED48051FA2CA8C8343B717ABF421AA5190320E6CE7049F\
0B02186927319E7B302207324454CBBE31E138E02380B1750F4E5F40A5CD818FCD3AB91CFD377FF\
C9546C01210208901EC0F2528B0C31D9BCF718172C509DFC823EFA739A004C94F9224E7B97F7FEF\
FFFFF7C272A4BCD63A856B8BA95872E588DAC1E290C0412D5AD2E07392DDB0401DB7D000000006A\
47304402206C8F3C9060FD4E51AC53BE35C99A99E7772F7B668D6A1441D9F55144F1E0855402204\
AD91E32EB002889E2C0131B3CA558669BB25E66AB262101420D7F1CC716410A012102B4600DC321\
333C53FB18C6FEEF8E0D7E19D1B8FDF0AEE1D9CF9846BD67171D29FEFFFFFF182F50816DF49C98D\
E718D4E23E948AAAA7D636A384E1C5F8DA0B14CB7406E71050000006A473044022072AC3529A9CA\
A63C0D7D971F240FDFB237E3EC5BC7D777E4CBF5CB18BAFAE69002206B2E3D45EFA4B53154CE9B6\
1A95D29795126626A9D08933F82C876B353059B28012103A791139B5432DA6E8CEC38C1FC150E07\
E4C1591740CD457F248235E5ED04019BFEFFFFFF12E08EEE141FFC0429DA8CAFC1DD73279580F8F\
40AE4DA24C4C3319A7DBAB733000000006A473044022001432C2A7574445C18EB087AF1ACAD7C41\
B36D45EC607242C76D8C0FFDA80077022066559B0F77CD3B0267812D67258FE3159A6A8566A69C9\
F2210E50BB3E417942A0121037ACD9FC4D9758FC696BCA0C63A368A5F226BDCD8300CFD33BEDCEC\
8DF240D214FEFFFFFF45EE9F19DC83C3BE5C6909A8637F11B718BDEB3204B6229F9158FD079A914\
F5E000000006A4730440220045A6F68F3ECCFDA778C8F5B383AF16AF68C2D684E35938CA2ACD4B8\
2F33504402203B673A956A8BAF87DB705BFBB0C60CF32B2F6C18B3CB9D7324A2E7D00538906D012\
10318B0F22DFFAB8CF6A3A7E0EE277E151C1968247D8FDA3C44D50472D2829396A4FEFFFFFFBFEB\
A6707BABF8E7AF97B8B9ECD01D433114240AEBBA8AE8A7BBFF29FB1C365B180000006A473044022\
02A5E3A06BA571E732409BB34A1F4BF08AC5A37F1748FFEBD0BFC8639F0439196022051784AC546\
2E74E6D71A1577F75D2490B6B5471F7D25993289AB29D977F1C1950121023189E24B98CCBA8FEA5\
0A30274A79F25846E13C434988FDDCD658C29349504FBFEFFFFFF44DC60040224BDB0E3C23D8ED1\
D30F0DFE1DA540F843615DA8513294DA726C7E070000006B483045022100B3C0A2B4F8E2D2B6E6D\
DF17136B6A2B608223BE8EA58286B4D31AD5041BA5063022025FFC46CE2F9080F6BCBE623F69976\
DBF5C2CD3C8BC595DB680AC1D0797E6270012102676DACF3619F24444010366C0C28FE9E19B5D75\
DA24C18E7951B9323CCCB642AFEFFFFFF4C4A6DB927D7021E1B8D31BFC4812942519222C8F0D71D\
216EECE217501833DD010000006A4730440220756AC28C11EE85C4EDC258672AD6B5F7B75E0DFAF\
036D931C0EE9EC23836F1F002206ADD83BFD3FBE8858A2D79949D6F73C0470027564954FBB87B26\
7A4C88F30D29012102A4B4B5EA77C171FC31CA4634A7382A990DE75F6CFF9A202D4E10CBFF36DED\
0F7FEFFFFFFBA50F95978354A8B9FC9A0A0DF5882CBE60CB88DC0175260B78EF7DFF83B7A311400\
00006B483045022100FAAE6C5C1C2F6627FDB3CEA70D15E367F7134C58A3A5C8BBBF97DC598ED2E\
6D8022052AFED64819BC4E3214FF7B1D5B3243FE9F0B496B9E74756A8A296D819B0D5B6012102B5\
218D293F5C6FFC152DDDF4E28AD723FE7392E7DA70144A848F09CD9EB4AD5BFEFFFFFFD17BD2AD8\
D291BFB918420DD72AE703D62C729563C52D9321CA767F2F0614FB9000000006B483045022100D9\
6B788E7AD8628C282D126E104C2A894E30E6D827A18250AEECDDC1A54E4AE9022045937FE8A41BE\
2F11F968FDE0DEB760A1F8F38EDF43C6527ABBE5C14C4B7EEAA012102E4FD211FF38FE9210E084D\
A7004DAF1E56D8F75D8400211B9E7CACD082598905FEFFFFFFCAD2EA15F9633F78B1060071C7D2C\
5BBEABAEF69A1FEC802F1B9F549D2518977000000006A473044022038ACF54E4BC16713D5DBE5B8\
BA67C9135F0E2B0DCB1CC432088427DA8EF302C50220286EE63BCC9CFC9EF832EED35433A97241E\
56486E49E114FC70634C3E4FBC475012102657086B297667ED63D1E7020E8056F3EABF3A302AE9E\
DFC54A1EA8A7E013569BFEFFFFFF7A0BE22FC1106A9430BAF3F2DDF770AEDE99471EE4704E2222A\
DE231757246CE010000006B48304502210088C68586E48564B5F138DC73024ABAD4F7F53C58DDA2\
739CF302308E45F14C3D02201D151A059AF9E7E11A74882E044157D4D337B6922A979EACB9A5CA5\
18BFC951001210388DBFF8EAAE0142C05B635E9816F71AF9DC97CF03C0B049A125AA5E7D131B67E\
FEFFFFFFECAB3AE2D79340085AE7623493114D7F169DC99CF730FA62D315DFD7BFD83B780000000\
06A47304402203B20E519F4FAC7D0CFBC15C3E5B9CA863EFE00A71F50847EF635338A8AAC2DB602\
2003816861A3347A6966559E22DC5D2A35E051E9B7D3ACDB407BF19895BBF72589012103D3B36B8\
92B76DC3009FDF98DB1F7847D96440E2395AF3B793AA023D6384423D6FEFFFFFFB0508578FEF62F\
68AD4B12BB6BB306D9B47C0618696E7F0DB790D1B90D740B42010000006B483045022100862EF68\
4C8C8A39A31FF8D94D6D0EFB133DF0DA036FAAD523AEBD9C47B1C4AFD022071C421339B919E2F95\
2362834B26B7206B998286C78ABE28111CEA89295A5B8501210268E015ADEE45B459649C5B6EFC1\
30ECC720725B8A6E0C282F9573A120022791AFEFFFFFF3C55807F64CA79B4079C62537BF31F057B\
1696A2FB1824341476CDDF8E0432B6000000006A473044022047117C883211D5ABFC4595DD1D670\
303942ED02728CF6704FC125452706F4F9C02206A2BA9322851E3315FB985B6C242ABE144374C9B\
EF3AAD1DAA2737A74DF5567301210202F6E8F327C0983CFEB2B555A7EF324AE2118514921D0086A\
872EE792C030AA3FEFFFFFF9B298F90DF4CFC247EF5610ECD183412C1B6A8D03727399309F9A646\
D2EC8BD70C0000006B483045022100AEB145CCDD06E957C1E335EDE8613AFF556E3DB88D76E7C9D\
6F56DD0845B70A602204395AEC585FA4BE54B9C59D4A1F8324BBA050198A62DFDED0D2C8BA90834\
CC0A012103C6154B31565CD4D9882A54539AED53D920780A0E964BEF1C1D5E0D46943CCF29FEFFF\
FFFEB51776BF09E0399D6E0012A1AA8A7F4DDFF31B20A90B56EEE69F6263264E9FC130000006B48\
3045022100BDBC028D0033F0B4E0C63D723A6ABD8E3F6F338F8D2085A570238E064B7414AF02203\
9A376CD7BAEF2636B07BDEE5DCFFE838799A6EFDC55BC8052A3BA2C6A51F5A00121030A7D542DFF\
102198E17C9B9FCE10579719AE08F4802CA3FF8B08C2DF5F8B2FE2FEFFFFFFB53D69DBFA69E67B8\
9956844CE9F3F8ED0873DE7453D4A0B716663C31B07A853010000006B483045022100CDD07EB560\
17DA9234F844585CE9A575E02AB68E77DFF96EC2EF26629D74CBEC02200D18815E64499DD48DC24\
6AA0874839E7871D4DE712F840BFE032A2465A076530121028CA565ABC55E5650238AA386260E73\
CC24BBAE9298AA2F7F5DC76DDDC7DCF7EBFEFFFFFF54B70B307D0366613EE94CA618E15695D0A21\
B22300464E999059AD7490D796D050000006B4830450221009E2252A0D613CD0D747C51577E682F\
019F17C8F11C05275A6D155A7B243CC69602201B8CFF1E9BB8F9C883513C02D5307529F8F0803C5\
B119CF1CF88AC39CD6C762E0121028DA848971C2A53113762BB3CE1C28EE2B6E78F0B4EBFF66955\
D86289AAD29D2AFEFFFFFF923D15733E52F9C4E763275C2BF0B3E1C4BE49D00B081182495EF1BA8\
947B61E000000006A47304402201D181CF4E3AFE4E6F227C18BC27CF059E9EF8C116CCAFAFCF7BD\
0860BE1CF4D50220062669B34ED1CBCE226E69E0D0B27DB34782AD0C8A1DBC5F97866A2B483A25A\
A0121039D76EADE28DF61B251D2C1A42373BD0AACDE14D55D1D82777A220097EA4C257EFEFFFFFF\
56949200B4BFB49E169A7C28A62305D52E56BCFBCEEB2713E432AEECB8275E35000000006B48304\
5022100B16705CC9D211069B9FF5F02FF716375905E7E71795922C5D3FB1FA30B1DE7740220312B\
2BAF0BBB8A9AD41E3B5B241EBF64DC7989AA946CA69954380244DFFA2CFF0121027E08979EF4593\
FC6FBF5C539BB3A6DB0FAEEC735AE9EF26A6C3251B98AC4D20BFEFFFFFF8A6B71CA3C2A2AD1E6CC\
DC695DF78975B678EAAF4C95FD2991BC00E98E15D17B000000006B483045022100B1B40589B6440\
1A125E807D80E1E15D2D9EAD2CB8727E1870DD9F0DE28AA95A4022035F47C3E52A542CCDF0697A5\
27033CFE545C9CA771C1EF5CEB1BC20BB72315AE012102152A7621D82AED9954D0433511A22C7D4\
9A14B65A9726A680A17A3E8835DE970FEFFFFFFFEC662131C729248C621D562561C899816D822B6\
889D6388959CC967F836C5CC000000006B483045022100FF6FDAACBBF4F29B825A97BB590812573\
40A3AD5AC8444AF06555659501E797002200C6BD0EC0B36516DC1972C23B49D3DF9E88D08BAD4AA\
3C9E7CEECE322C7215770121021B71E6B5EE17F1ED5DBF7267096ABC9D567BF559C2BE9BDF8B52E\
EAC7E2B6476FEFFFFFF388C16BA811298ADE48D2C37489016D55B483000A2E96716108E5C4C529A\
A56F000000006B483045022100AC42C12E975BE7C9B854F9B6A691BFC016B80475559990A4D7FDB\
A50B19155900220405CA5C5BC52ADE58469102A5421019D321D26F9CDB39597BA58E6A4DFB35949\
012102EFE3DC2F68AB759D3F1EAA5DBC53A112D67893940A07D9DA9978DA04FE1BC17AFEFFFFFFE\
5F5C16B0EE3C27DE51869453E0D6596149AB896D88909B9C00644DE5F1D3B07010000006A473044\
022077AC39BF02E2727383735A4FF1092E27FCF5628113E90AF1ED97D0E443E87CF1022073ABF64\
5689E26419C0CC80115DDAABE7B2BD955C19C794FFB46A7D937DC083A0121036C134EBA17D33CC9\
31E8EDC004FBC3767F909AA12BC8A932ED07FD752BF8482DFEFFFFFF122F6181D7B9BFE323F2AC3\
F122432D3840B3990553B18A63C83CB60EE77B305000000006A47304402206709374F0613D03A2A\
CE548BC166CDEE1AE7F871088EF8AC025D2BD44C7C8F7D022042D73593A78DCB127824427714D32\
35250C1F537E23E547F0D6C9B8B8717471901210374A4ADFD6B7993939D435C24577A91899A9205\
44E84E1EC599A44D08C6210079FEFFFFFF2AD212598D4FAE8431D97CC1F960A3224B2358B83C94F\
DE4822A70433DE8372E000000006A47304402200B897F2A5316370CFF0CC7022ABF9F1C2AC4985D\
89D1F9023FBE2D1346D9BA7C02200605C8C6342D598FAF42BBE0E003B83395DA6D23B51F3533B3E\
DD25D6950F940012102C2461FF26E545E09EF7808D90D4E62E769C50D3D3A9106EC790AFCEC3CAA\
A205FEFFFFFF39F8CA3F798919BD551A22803611D9A814A47FA661D1F19E3624E4D12B34BA24000\
000006A473044022014EC32C8023CEA9BFDD0EB2DD4CE90A4A8322B7089FC2F4BF17EE1535BC3C1\
2C022068C0FFED335B6EE0CDC693D985F1D2DE4001130529E90459200374BD249E9095012102CB1\
BF316B60358854C0A562C4187C386A1BCBE2089E3C0870217762F4B6F336FFEFFFFFF90F597F235\
86E671FC984DEEFD91A71A5F1582501F197B49080FC46E9BA5D51B000000006A47304402203D62B\
CDBBAFE0E8F802F3ECF29B40C9863041AFAF13E423D19D822FCFBB53C9402203B9B5D845C5DB6A8\
113E637778E8C9682E9404437B576A36265D33D3B7C1471C0121023B9C6A1C693EBAA08FB4A0135\
8FF30D51771B39E1FD3A70BDD5F7A84ADDE48E0FEFFFFFF0461D8ADBBAC2BD1C1A3CE1B8AF12FF9\
48574ABAAE29A36E4EB6A39A1429B2FC000000006A473044022026C4698EB0806CB80D5375BE276\
409354512B9FCA56391D961714F56DEE8C0C002201CE42017AAF7DF7FA4DA5242F929324B0DA709\
07705942DA1A7CC06F87A52DCC01210324105118680F23ED4C3B38E7A3AA06149EB9419EF6ADB6D\
399C75CEE1AB22A0FFEFFFFFF45BC53BEBA41BD4B3D65CF8B7B77981DDDC95629E3A8CDC6A7E430\
4AEA1DA530010000006B483045022100BFBC078AADBE413BBA5E75D10E477FD9F2F8DEB8A0DF4A0\
C1CFC5FDCE4FFF88C02200189996EFE987DD559E1EFB4AA735D70D865A92F90AB17AF324396FAD5\
77A657012102596CDD57A6D0A36EC503366E632A2A89F0EC98839EC451AEBFF2E8F05C2CD7A3FEF\
FFFFF1A9761016B8BA2AD65F0C9BE687842BF2DC509F951FBE3B76468BD6B1ED6C05C040000006A\
473044022059597655AB9384093BCF62315D415693C938B8645FCC665EF3133774FC2FF99702203\
508B98EAA48564AB7D219D013ED9B061CABAF15F2F35DE2B9DFB082D82627A4012102082F45107B\
ECA99185E1F9FFE4755A4CAA1F0DD826B82702D67E99FA901C5823FEFFFFFF236FECFD00D3FAF04\
81AC489465D3A1F8770D5809F680D7CBD0F3B46AF74E08C040000006A4730440220753A0A15F178\
4DC804E436CFA2C2B64D8524E971E5CFBF1561380572435BC4B002202CFE7D9B1CAF03A04E7EB07\
CBBCF4F7C2A0DEA57841255E58DD2CA2F3451614F0121025230E87AE898C95389B971B64AD73585\
C6482B065DE1E8FBA4B90BCBAB2E2E9AFEFFFFFFC33C47B967AB3AB84E85527090CD253260374A6\
61B9B8829AA2F1D670B9E601C000000006A473044022058CB9713A82035B34B4CB8BDE07FCDD597\
14F5D9B435C564231296E284DBBC1802200125715D2D8A407DDB267D0F2834C3947165C1C226E86\
9B084FE23FB27885B7D012103D44B58132A99E6A2B9DB66F4ECDF713A477E36962ECE7BA439B72D\
BF1A541596FEFFFFFFAB4F57A65CCB55311F84E718157A34B013F09FD47B0B09B4C1F5A014F94D0\
707010000006A473044022067B2109D3C1561025D9221FB0E95095639B96EA6FA42539ADD5F4AE0\
02F5D83C02206F222486E569510D6D966E2B70F7CFA666FA24857F581FBAABE79A8FB3FA7479012\
103321DFFC5A6B12B41AD212C23A6B99BF7F67CAC6BB67E5D2A033673139CE072ACFEFFFFFF4A85\
12004C9B9C8A51068099C70CF2519BA9ED25963DBA2586EDABBED720CB02000000006B483045022\
100E927C1E41350E2D62F4FCDD3FD76332BD76517A4A761F425D8108900B9953E7602200B585B3E\
431F705865C020B7C0BFEF194D5942ECBFD09361866D1FE5DC285E8C01210313598F516D5B798E1\
DDFE23E502C19F86F44B006C00A0E142CA15A175EC07F78FEFFFFFF214FD0556BC1D310F5C22B15\
9AA42880A06B054A5B2CEE2605863A9AD6919ADA000000006B4830450221008DCEA9D369ABFDC4D\
6AA20B6207990080997B8E28ADBF3CCB50B4BFED7B832D702203AE6851209533F8F7D9AB5586E6C\
7246FE845E8936F698C683C2D85A7DC2559C012102808621DED0D2E09697F3CAC2DC2BC57019873\
271B21CDB560D0B2DA75A4F334BFEFFFFFF0F3C5584950EEBC188FE51B6C5BCC4134FF99785BF69\
BF016E46651F32D24221010000006A47304402207F27F77ED80CB7963589067458E67CC320CF91C\
A099610BB437FFBF25747BF44022018BE28E432044BD7C6A4971FB8746709EDD1BA0FC00BF662B3\
70F766A6750F1A0121028D8088B0FE9F32351EF274DD9F85BDB45A35AA757AD8FD49B3A8D70C0BD\
4E5ABFEFFFFFF20A7CB6D5F724B08B1E4A51872EFCB3E82D5F799EE131B6881E846ACBCEBDF0201\
0000006B483045022100B4E4FFEEDE4AFE0852102A83D3409EE14B0490FFFCF8315CF9D27D7B47C\
39B4F0220630C8077BA5FA7C866A41BA60EC634CD49440E4651901D40DD0C78562D5A4798012103\
F57571A6B026C7BF742464D74B41FAE8632FFF93C155793D304718A116C5013AFEFFFFFF410B4E2\
0CAD54FD6D478234B038E973A13527AC7F140A80EE05F4AC96130DB35000000006A473044022007\
1C3D9BD5C301D7CF6CF79BC0FB91DBEEF655707C28B7C03129B3E7B9B7D29702206094A320B0293\
55F2F108C5FC08E80EA3AA64E89963B287A86FA5B1660274833012103B7DF3FBA1265385D044AA2\
0FA8ECED8AE679E8AD99B4238FD821D21F069B4861FEFFFFFF250567A535F8A442033DA50E7E969\
1381F2F8C98DC51EE22CB4F2B45E079C129010000006A47304402201463B0C4877357BD3B6B4384\
57CDF97F590A9804342554E2E89C6FA2B34E4CCD02205CA40B7A3E64C9998E21EE8F1589912E99D\
96E263171F0F2F140E04294FF5024012103225F904D24115CD6C3883CAD928CD0E97E1103D4D52B\
73AFEC2FDED3D502019BFEFFFFFF35B97E3FF27EB1E288373CCE620FA1B984103148678B53FD611\
401DB9729D456000000006A473044022017595C2418101C3886CA8C3E843A0D7B17E685D06EA621\
9DC3791673A9A642730220510902F5F3E0253842F4942FB384F2F509B1215F09E87F58199EEF6BB\
46CB7FF01210206C628D4C5466C7130ED055F2C64A34FB4504DCD7B9DE5E48B4082DBEF22028DFE\
FFFFFFB4F6B8C70C4D6EE07EF6C9CC6F70C8681DE8C7427268360F8C6F15E31D559E4F000000006\
B483045022100872E867F8632C5B199E525696C53EEA17D52904DB157B2269D9AC5EEE63328C502\
201249762A21C8B9348603A5E950F334AC7CB284101C4221A918BDDDA90AC9A97A012102914D281\
146E47CC66929CDB63A1B3918E7054A30B7D7AEA1A6FD025762C16D42FEFFFFFF967B024E492C53\
DF1B26E37C68E57B84036690E529CC999D570CC29AEB8761840100000069463043022021418FFAC\
5E0F58FAEF9A2C0957A131B3A778FCC71BB855815917BA55D8645CF021F664EC1C36AD7A6A54F6B\
C400F530E48E10B1FCFF7402F121FF230D11CA7A2F012102D607CD7A2C06CD640408DAB9E4DB26D\
8D5559F3EB894FF6B96E233F9A9118BFCFEFFFFFF8C791A95C902A6A3142CF1A137169D182435AB\
28F72271A82CD454EDFD379879000000006B483045022100DB440B4AC8227A7B57A6F6BA7A7B1F3\
DBF06FE41C95E446C5A6F4EB5BAFEC7C902200B7B5B9E8C3754EEE30FC98E9A4BB2AFC43C97933F\
C8363A8A930D4BFF43B57D012102723CE38C23C79FDB5FEA546EE1D588422BCD3CD6926D698EF4C\
25C2E56064A39FEFFFFFFFCB009459924D613AD493FE915FE80683FE155973E67AB840D2D684514\
3B43A9010000006B4830450221008D69CBF42FECE56D94BC9AEB1451601D82033328B605859BB43\
B7A7FB41B19B1022019D7BD88C62FEFBCEDDFBE5B6DB4975521B39DCE68EBB42FFBF84E9D09753E\
6D012102AE78C7E7EF7B85968421EAA02DEEF3ABB17EA302EE2620C9C0F0ADA5507A62A7FEFFFFF\
FF8E28B03CD9CA93CA2861674A0472B2C6263BB0557D294A4EBD69CB23B32B41A000000006B4830\
45022100F9448E418BC9A97CDCE5ED38C4A0250D7A24FC2EB46C8DB1E554D4773CED7F8B022030B\
E886C2AD98FCB7950D50CEF0651D742BCF337E558553E6CBA4AB81C7332A1012102210DCAFDB434\
BA0CEFB8EA356F614E7534E429365354E6D917A069D37A484793FEFFFFFF290B47D2021DE0525EC\
8DCA417AC7B6BC712A12BBF86791F9778415CC6E2985C000000006A4730440220729246F2EFD19D\
9E6814526E5A64AFA2A461F4D52C88C6F89005A7AA4F23773902207F00BFCCC2092ABB551B89F43\
6189E3F12A8A45E47928A3D8F4DE0A51C8B7402012102ED7C96A6E78BBF09C566C35C30A3382549\
72A4BA4D635BA20E2AB0596068B653FEFFFFFFECA91C827FC9BE5A0619B19C71EE090EB879007E1\
BD32623BA91AF7D583684EA010000006B48304502210090B2642E90577E315DB6BFBBDE6AC5654A\
A5378D6A711017B686B811549F72BC0220749E726653A4773294B183984F5A206740C766A9642F8\
8AC24398DFCE239933A0121036ACD3F6C4A7EC67D28642F7DDEDDA18A38E7FD842B5527C95F703D\
669050F6B5FEFFFFFF012FA064608E8A38F28930AD6C4782036E10CB1E7878AC312C87FD594FAA8\
F66000000006B483045022100C5477A33498FAE7061AE69983B8429EE517973E02122F01B7C33A5\
A2E72D3AB30220187647757D1D36F64B5BD406B93A13CB4255D57E5CEC6FEB86CFEE2C19B0B25B0\
12102C21428A2CDAD647C11C56C129985AE7D1195422B79C2FAD5909A2515C01B23DAFEFFFFFF57\
42681668B8532808BCCB1AE1743528EA37457B3C9B6F86CFEA80E8A98F1D2C010000006B4830450\
22100C840E31888E77C725EB86951D22E5CB05125EAC54C429B7182B9F978A5D7D8EC022016B4E0\
769745986BDF512F31FD243AE737F9249115AD381881EBD138A20ED55C0121026A965839F8842A3\
D43DB455559AF29B675A4F8C0DCF9B18859A2B03099B2979FFEFFFFFF015440A5F2B952AE532DEF\
FAAD5F03B5B35D702FA769D54626A92A639BD9A850000000006B483045022100A2E0D80E703AD37\
ED5E3B52A315FE82C505290E9A79585ACE884BC3E5DE92CD0022033A98D87843CCC99E5D150B7E1\
ACCAA89E3CDF620A34DBA2048C11BB21E08BF9012102C655FBCF0E9E3050391E2235E92AA0BCEAF\
5ADC818D84FADA6968EAA2CE57CDCFEFFFFFF9654308655209C175CDCC90413DADD295044A67E44\
1A425D8C5796D14E9C8F73010000006A473044022079C8662FE47D03AD0BFC857065760CCDE6168\
DB59E5E51A725E54D4D17F9D39302200087A6DF4DCE724963E9B400EBB34D64C854C6DE91268081\
4FADE12D43E8D4EF012103BEABD05B55B374A1047F8BA01AC3420968D08DFD560FE46A0D956FA7A\
5B66463FEFFFFFFF3E43967718094C25A5743DBF40E2935CD1B18A6B0A4C77C6CFE8E544A797FBE\
010000006B483045022100FFAA976BB4207CBF318099B6D3876A0A87921BFFE3396862EC5B2062E\
92F1997022077AE778DD05ABF2F13CB5391EC8D008F29B6DAF7B8CB26E00093829DFEF8B3FB0121\
037ACD9FC4D9758FC696BCA0C63A368A5F226BDCD8300CFD33BEDCEC8DF240D214FEFFFFFF964EA\
2F11C26E97FD5ECA25E0ACF3B9CE35F63718BB99283F045EDD048D628C7000000006B4830450221\
00AEE3670F80ED5E80D85DC89BD5902FCAC2B1D8A8246EBED31D05D8BD09AE4FE20220476D48519\
6781E41BAFEA1B3BBAD512CC5E0CA4C2D99678DC4AA8828CD0DC477012103F57571A6B026C7BF74\
2464D74B41FAE8632FFF93C155793D304718A116C5013AFEFFFFFF847A757BFF05385C72E7B62B7\
75F9D248A751395211E74EED10DE27F0BA99EC0010000006B483045022100C24F1311F72C14D9A2\
308ACBE4AC77370B4BE0F65BAB2EBE70495849BFFE1CDE022070FB343608FF3A518159EBE5623F1\
817A795FB0BEBD9F4DEC33838790565B0F5012102162BB464DF6712B35BEF47572064ABD980E74B\
6BC405DF9BD3951CA1FA77AC22FEFFFFFFC7144822C3A418BF792FA3D440C6FA03F0F866A81F8BB\
D284823DC7CF66BA1C1000000006A47304402205B0208687D76D680D6ED52B90816BE13B35ADBD8\
2F2E2BACBC041EBD2B6B2BEA0220675DB5DB7A6B190A88A997630BC95FAEB1C1BA22D02726851CD\
11C10B08F6A4A012102EAA51ADC7497575D44FC9DB8D336D5CE2EE31CB79384F8B89C63E7B2D1F5\
7071FEFFFFFF6FCCD68DA25383D52F5F2B0D2EC66F58F9C06D9A9167E4875F7782E08FE3C7BE010\
000006A4730440220072776F6AABA4B96FCB02271725B33567325E52D70AC963369D22D7C47FA5D\
2B022001B7C9ECBA52094353A57EB9149267501D0AEFC7B47C1E5B313BD5A182D44B900121039A0\
98B4CEDAEDAA5CB743EDF7F8B237E94FCA813F1CFD6A1D7FEDC1E263D1DE2FEFFFFFFCD2E639242\
49AA1B893DEFC1E164F92A622E04C7EE1288447CB2BF5067227DD0010000006A473044022011FAF\
3D953C44D4EC3B98936617C21CF3D46E5B3D2D6BA73F6F20C6BF8EA4E2902200CF49DFF6334E671\
9A970A34CCFBAEF316054E33AF2BFA7466C3F3470AC9EC5A012102A06A9F85CAEB1F18D87F83231\
68E78F4FF208884B91F8804EBD838669E9A4D00FEFFFFFF057D6A9CB3C179E997AD65BB811300ED\
AC5FFEEF292A230A041D3555209AE7E6010000006B48304502210096ED07D6B8CED75A1B070C1FB\
F413467222DE3799AA7ADB5AE9796C3AF113DB60220319F6552042D904A9E81AC791E250E514989\
53EC921C7DF7FAE850FDD766C3C00121027F085885D451428F6DF631B6656153362603AB2D42BB2\
A0BA665D970650DC943FEFFFFFF6619AB548CFF5D5E983F10C74A64A5362B71960EDF4ED1ECA7A8\
32CC0320BAC2010000006A47304402207B12EAC1297349144DBFB9FB1DC14AD217BD853976417FB\
43DFC4F297A3A7EA902207C8DC3636B6EC9DF6B860FAC9642EC0B41244005615A5AE3A2C411D234\
F0B56B012102C7B9AEA78B44C5C91DA36BC1456AABFD284D55D3D521AC3B6DC4660D3505D106FEF\
FFFFFD5BDBDAC6F16B87085178194B00CC1A9CB6B749194B4DF7551C00B5AA3442ADC000000006B\
483045022100DB1509B6BD7E4B989D0A60D4672696DFEC2ED95130E6045743382ACC4C8495B1022\
05200C239D7AE444AD71A7B9CC2224E012904B7BE3880A572818E8B0DD61C5A960121038C8B9BC1\
6231E1B2C5D650D7811D34360BF67A442C03FA15D7A1912462F53270FEFFFFFFF31355CFBDCBF6B\
9BC54E5A068EE8C3721753710CC0EE20603F32D6AE9B78CB9060000006A4730440220766A9B7B6E\
3947DF7A735765BC579649A4927BFA26845DB9F478C83DA595EF040220332635EE24B3D7AF145E3\
7B3EFCD450EE249A94243D7E64A1556DC8CCF42A03F012102082F45107BECA99185E1F9FFE4755A\
4CAA1F0DD826B82702D67E99FA901C5823FEFFFFFF7F8B15BD41F41D887818F47F08A763B3FD9DB\
10775B9A302744F61DB44D37300010000006A473044022037B907A7952C89D3F7E61211D742238B\
CEA51E5E1D1535F1C1ACC5CB7ADD3A5B02201202CD95272CFFA5541E9904920B60A014304A18705\
B75CFA5B67ABB3FB3267F0121032DD65D6DDB9E89067DF082FC3ACA325F869CF20F349269E57209\
9D846330D876FEFFFFFFDA013CAF3B76B946177800996E66A5C28BEF7701CD70B1BB05AE345192B\
5A03B040000006B483045022100BC51F422B4206C33CC89A18606A0D5E3B386DBD12416DE75FDE5\
C8B5D45F154F02201DAFE6EA7D3408447ABBD589839A26C44B59911CD6B1D634BCD372A834D9A3E\
3012102574CC53D7D3350F9AFFA20D858B4A8C9CEB7B1E1479201272160D8F950D4604DFEFFFFFF\
1960436CC01289D41F2B604AA0681C235E56EC45D66BDB9625D3E13AB47A074E000000006A47304\
40220674B592EF5C73C9D6B0A8D83AACFC277C9AD34F46DAEE88AEED92DC6B2CD3AAD022033ED7A\
F83C7C3B0260CADBC2B3E8DB3D2B105B9A8E424C6961E7EF9904BCB1920121022A03844A951E7E5\
29010EF1E82C2CB883CE3322EAFC63389FE5323C9C422DBD4FEFFFFFF6F432C77D656CE8C1C9A85\
B1D7AC09932A58F9DA3EB118EF1CCFAD7244ACB4D4010000006A473044022006F63EE2E3DDDAF6E\
603DCF037192A9BE0D08E02A8ACE4D9BAD460FE8115A93D02207743C975DE84A84F44A682A67844\
3AC5A6706ECFC7FF7884B71362FD62734AA40121036A941DD6D1F26828C1B2C9C7A924F3579E462\
4F4D3D0E52F279B5327D55388E2FEFFFFFF98DE1F1AA82F606AF9562D6AFC3E64B08BA727F66B9D\
B837635CB66C425057A2010000006B483045022100E8F34B201C994F00686F907E878F476CFA2A8\
6623A19755E218D3377E9E8DA2202205BC3C3DCF30A61A6AFF7942A09A6A1A7D73199ADAAE31E57\
5FAEBE0DAED7BBB101210266EA8A597FED955F1A3A73FE8DAEBA9599EE7E566401C64573734454B\
BA6BFEEFEFFFFFF378A2D9BBC9CB847F12DFAC62437AFA6961FB28A469186AC120514978B5BFFDB\
000000006B483045022100FEDA5705C99A7F51167497FF3EC65DC687AC820C9B421F744AE52401A\
9349142022025388CB6E6EF907C4D82A887683049D2580C2D31FBE5B08EB7EAB717A993D7220121\
0246BA06D6633EE9E725C2005F66DF40A8CDA1BE65A7B5CCEB94A5B093C5B9CA80FEFFFFFFC318B\
3CB6B60B2CF984FD7EE9B44C8FFA1998EC780C6513914AA0C520FF5C225010000006B4830450221\
0087C4F3F72D895EE8A45F081C278AC18EAD89A7EC174DE95266B3AF73708AE360022074833811F\
9B49BA2F020BD52FCFFD88945A6D669218A7BBF2DA84B8C068E5615012102F496DFDEB9886B2CB4\
7AD1F5E9D6991902F587C60B27CE7426ECA44290616A28FEFFFFFFC2D73A76320F68749D54607D2\
80E67DE82D5006A820FC9DC441FF59FB1C0FB9D000000006B483045022100A69A8DDB6EEFCDF936\
E277CD0990DB749AD2C9D54E8EC1F933E103F48F30500102203FC906928BB8CC0C66293616074E3\
3DA5040FC5BE70A62C4D5A8F0169281301C01210377CE09AC40147EC2101D68F9BAACE949D3689F\
2527708952820676212871534BFEFFFFFF0A5430CDCEE890AEC1B187EE4C2F945C04B6AE1B11046\
FA186A525719822CF53000000006B483045022100FDF76D18EA18906E3603649E890A1329C3E506\
00CDAE7D5DE43BD565EEDE472002200AEBD212941054FB03126A570465AD9F502C1F06986491CFC\
319DEC42B51B2140121039590D5BFEF85BF24C48D29E2CBC91338DA2456B78E291CE52B416C4E52\
533457FEFFFFFFD42B31663A08368E89B0A731CF602A5B358D20B99BA85BD57DD5BC9F955EB8270\
F0000006B483045022100C1516657F9BA68488E919BD5CB1EEB0EE231A2246CB5BCD812B2E748B7\
570EFC02204721B43D7004C324AAABAA8C824FE54E1D0D0F1687B14CD1D1CBAAF6C5656CB801210\
379BAE4E27D1912653C36AC6A3649C16B0E005FC1AAB28A3909F6B152B4BE015DFEFFFFFF5C0623\
4334F697B2CA72E3C2397EFD899CB1762C57FA52BAC2904763E624A129010000006B48304502210\
0A0E02EDF0C47A9A0BE056AF1EAF8BD12FA409924C315BD44EC13C45F2576A88B02204FC5F79CBE\
15BF9DDD3BC7DB388E798440A6788FCB54FD5C7942ED267354C2230121033B439A47F8781363C94\
CC42132A4F5179A05FCE98974F2F9E3A3F8785BD823B6FEFFFFFF562CF5BB4627EBEC2BC4615465\
EC04FA15C19CF017439B13121B796284317C02000000006A473044022036D7895673DF78AC9EC08\
DC3127092AA4DA440269D3FD97EF4B46089EF10D1E60220752A8DFA7EB6E0A370556928F6F5D71B\
3F29443900E80B9948598C2C67C91AB301210384A143C6C08A359D0FEA028EE9CC1E94CC96CACAD\
7836CE1D9DA49EF9CC45EA0FEFFFFFF73D9F6D7EEF457CC853EA096230EFFD0F15266669BCEEFD6\
C0B5F818561DE640000000006A473044022071AE948D63516BA07EC228469A1B4DE44EA989383E0\
B228109CC325F692425D202205B0E72972EC283BF99A92CA0BC78CEE8D9137652A5DE3C040BA04B\
5251EC3D8D012102E9262B7897DE5E0B53F84ED7E66095BDDB7E6728B50EB449B72162B048A4494\
CFEFFFFFF0B4960092D6A493F41CC3F3AD2152003B071B5E4E5283045CFEF53815A8F733C010000\
006A473044022027438B90A46ADFF8CFECD5D9B280B7CF12F773A6DBF3BA8EF841F4471F1A19850\
2207E8E942A1FFE84B8E64C6873D6931F086E902C6C8496A0372687B89B9455D36101210341FC0F\
2380A88F0EEDD544B1336B42CF58F417F29A92E91C69AF6DFC0151D897FEFFFFFF5191ABD7EC72E\
78CAE37549C2026BB4379F4D1E32F48FED97583E2B6D54A5FFD010000006B483045022100E77382\
812CA4A7B4D4ADDB61129144A37CA796C39151069D08C0BA76A4F1B3820220660ACE0B429BF7947\
487790C73FE5A3CBF4AFF80D3E254433E653F023C5B9D8A012102D3AC7D51CF639AA97F72AE13EC\
A389DEEC8227BC112C2AA0DF9C488DC6AC8458FEFFFFFF26231FF12079B84BDE83486255AFD79CE\
F68C2803860FDD80783E02A2DBDD94E190000006B483045022100D0C7FCC8B4824B1D7BD75CF1BC\
F8A8D319781C041BD7EA09FE18E64718BCBA160220546A2D7EF9C2A53954A341E453B162BCD3E25\
7CA6001B8CF8C3DDCC1374A95740121021CD59CAD1604B2321BD3B60C3635592141C8F8EFED57B9\
DFFAFFE47EB73DE919FEFFFFFFEFAC9429CA6351E5EF49292FCF19896075CF41C2291AB2E007E48\
FDBBC667697030000006B4830450221008C4F7F8D90E5B5595657B1EFE7AD8A42923CE918A3262A\
30529612FCE2462D8502204AB3BFAD03B328389262A16CAB427F98D35213B4EDB61E3AA0F2FB840\
F9B7BBF0121025230E87AE898C95389B971B64AD73585C6482B065DE1E8FBA4B90BCBAB2E2E9AFE\
FFFFFF6D2FBA183AA171A61C3231F1A06F01D160183D1E241D8F5943557C90B9368BA1010000006\
A4730440220060245C6A499ED0CD15E3738F4E8E0735D8095D3EF791D5525A2D0A4C6E8C0240220\
6527CA0516FF4C9F1C3FF2A3156B1AB4C441844F06DF1A38D8F04F8AF5BDD429012102CA3D2FF73\
EC8FEF9771147CFE056C4E0DE12C77AA7175EB9BD4B403BA3210104FEFFFFFF4409CEFAC3C93BDB\
4C0FB1EFF31F8E2EC3CED0D35931449FC6879DCF8E08A11B010000006B4830450221009DFFF7963\
2F541E7DA1C6371AB47EDAFC3C12E40E02550E0551FEABE024E5E5B02205D0A6C56B107565568BB\
8CEA3DB5BDD7090EC2045AF5E96AE1696FD1E02ADFE90121024F0B89AB8389188791D26AC79D7C3\
0DF013975C2F39CD7D7BF344563741A1064FEFFFFFFDBE9A2278973252E226FA496EBC38BC58B60\
FEFB00B80A77DB6E5BC7152DE955010000006A47304402204C199941F4467905B022EDD16FC7769\
596789DDB157B200DCF6EB3D9D3ABD0AE02203A79C932926730B4A9E574B61C67FFB0729821388E\
FDDAD7E563A8E7F7C9604E0121025AF1FF50765534697C0D7367C0235994599EEB2B6EDAE56CD5D\
85220EA4821E1FEFFFFFF80B6FA19FBB3285AA1DB8E0853A47506E897AA57CA10D29081FF392D7E\
744F01000000006A473044022013B1C5C034DC3507A9442450CCBF6FEB74DAAE64D18A3373239C2\
1CF7DA42D0402205114F005A41CB8C2D6E793FC404305695B11412E52D167CA4581AC67675260EC\
0121027A57187F6C189FB2C226A5630FD11600CFD9F3DB2B14D49D18230A4D692C479EFEFFFFFF3\
EFB0DDEBD0F24E05B4B7161AFDA472290F416D2E0D0D4D253F2F6969B3F3470030000006A473044\
02205D489E45C4D311CF38069F95C775026FACA58B1073EE05CB19E12C6A4ECE57610220729397F\
285EDFBFA645C4C6DB9E47CFDCC153AFFAEE4ED4AD9BF29B2E2B085F201210362A927BEFA23FCA5\
27EE75D42B4CB6451D6BCE10A422DE3CC60FB7799FE46388FEFFFFFF6D4B5B4F699B9A1EFD77692\
46874CD799A56E6F7A6B28118EB3AD434E5830B5D0F0000006B483045022100EA1532C876E7528C\
7F97CD9A4CC1DDF5FC999051CFC679BDF9C397A6467E845A022001CAEE11F334CA2E7674C67F0BE\
7E004B206CA0F4760667554294556F4CDCD59012103D5D90793FDE49387997455B7A77935BCE692\
7AEBF5ED017433BB7B55D4FF1330FEFFFFFFEB4F2CDDBDBFD0E2EFA1D6F307D7940C176079A5E5D\
513DBCAE78886E6FF3A70010000006B483045022100DF8AB38FFF7989DE925BE9349909F81D5052\
0E0EC038C5575802DACD5C7A9BD2022051DF9B00CEBF18A0A46BDF329FA0D18E41E220FCA2D6A46\
F2DDA673FD6EA64FC012103D44B58132A99E6A2B9DB66F4ECDF713A477E36962ECE7BA439B72DBF\
1A541596FEFFFFFF109EF709E8951156E81ADA7B478BF24702A6EAE534E90BA8CCDB2E193900F32\
A1B0000006B483045022100B824AC5C86E11DAE0F4673F20F29F292EFF269141A0C3EB935388DB8\
61A16A4C022009E5A9B4716499D1EE60C4776AFDAFA80F51B0FA8D3CD9BB698BD2A95C63DE9B012\
102D55272E9C658A3D1FB4A1D940DB305B365BF3D3CFCE137CA38F68FDA5CED4CE1FEFFFFFF5E14\
57E5169E4423F4663A2B9C6B3EB0B6862C05CECA9AF43D80A48E05BCECD1000000006B483045022\
100ABF96C41AD63A77F1BE81AA9D803A367B51C9321DCFBA94896FCA3AC735CBA5302207367B545\
274A3EEBE06B83B9421916B8FCDB8DBA3C6FF45845E6FAA19C45F3040121020FE05D3F377252E3E\
A7EF4F70A34480313B07BCA3E6155E008FA5A58576F2F57FEFFFFFF15A478DF98B68CFC45363098\
8A6ED2A54E662DBDB9345104113B9CB79409FC14050000006A47304402201873244622AF0FBC1CB\
4BD7E72FA9CC3A19D6B817B1DE043E987E269698CF7DB022070B7EC5810CF5B814E8DD3610B0E97\
3E737FC61B8BF107439CA981BDE7F413380121038BCD0A668C09902FFEA903699F3AF471E2C1A05\
9C5015A21492C3BDE7E9EC1B3FEFFFFFF36CDA2BE89B260FC62DFCB50DD5934EB362F3E4A599B84\
4F1A335110ED7F6F92000000006A4730440220472944CA73EB48460F19A735633735DC9CFCB8BA0\
9B193B3FEF000CA82608C5302204BE3351E2A6D5B2942C96A5EBB4A35748D2858FD343BC34BECCD\
B2FAE490D938012102C246085A8A479480A64BBAF0306413767E7035E8D39E9F9DA2FD29E42439C\
FE1FEFFFFFF3ACEA1B8D0F469E872261B4E89F030998A9E5CD3E3E7A5D4B998693346884DFD0000\
00006A4730440220563A31753FBA1C18A16DB6D60BF62776249E6AB167146026816ACD7FE7A32AD\
4022053ED6728D8689C6705C5B1A469AE0954AA06023F424B131916A7D96F8A131F5A01210209DE\
F75C196B48FAB85164389ECC0544031A9B2676B111F5FCFAB7DE1897A69AFEFFFFFF5C84FFD4A8C\
57DCA8D805380930A5C92B0CBA498D03B00D1409D8EB77CC8B8FC1A0000006B483045022100D780\
2EDFF09397FFA0600C84AAB0171CCC83C8F556E50AF30DC8694BFA9627280220077CB1AEF0C4BD7\
EBA8D36FA86EA82BC301999FCDE58AEC6E0315E2BAADDA8B2012102F9FC2B0719F1FF5C3FFD9BF7\
8C78B857346332D81FB3A07DCCFC3C05F53EFEB4FEFFFFFF55823A7A46A6D54DF3CF731EE0D6BD9\
60ACD09BD5E1390208C0A0C337258CEE3010000006A47304402202E9EC16832F0318D7F3E8EE678\
4F7867BB9679EBCEF6FA5B79928D30E8DE131202200AA0B7E8522449CF786458C6ECD830CC145B6\
3B05EF3BE7633119314F42F4E85012103F25A3D56142DDE8B43D9834B36E8944E6FAFC6CC843FC2\
6198C4B629F3DA126AFEFFFFFFA48E758B8BB9ADCBCF033F17CDE1C74D6D480307D639649B46F9C\
8E5CF5150CA000000006B483045022100AE6BF85CEF351DE08DC3314DB99CD29E0044AA8BA35002\
F7B8B14646A61C4564022048503F0EB1E8446240B0551C2CFCC147FB5F176C79FAE5367EAE096D1\
0233A7301210268E015ADEE45B459649C5B6EFC130ECC720725B8A6E0C282F9573A120022791AFE\
FFFFFFC35B2813C3D1F5A5344C8B65FEC2379DEDF48307AAA3B8914DC952431B2402EF010000006\
A473044022079D1BBEB4BF5B61327DCDC7BA39CEF19F3F85587E64EE4F5691876D89CFA6FAB0220\
19F9734793EE605021514167EEBCF277E9948E7E473CDFF64BBB2BC45EB15FCA0121023ED6AE78F\
B0EE7B60EEC6A993A59CC132828D0E9A677B01978F0EAF285191CCFFEFFFFFF94E4C563E4D299DC\
4F1D14C5BCA0BA8417170F120D74887A1B2D88B8D7A850DF010000006A4730440220576063B21C7\
EAD0EC6D540CD56CBE62232C384DADD04CC3018A659726148675A02200D8429814AC6EEE06FD8DE\
0A90D54EDDEC57340ADECD77D564848CB6030EE96D012102C677FCAAD9412020A10878249B46AFC\
56299998E9CE08FC1AF6237FE500A2B06FEFFFFFF89861B114EB75795B0D5441CB07E44F58C040A\
A0AB7C790B7F9E48E788EF418C010000006B483045022100D76D5B40973F67260E873F855EE65B8\
A364E855B887CE64D47B6FEF4579F0FAC022056D9550967ED983DA78062D4316FAD0917A69CD2C3\
4412B7FD251B45B30CC40701210254887F80D91D959426CF9057D318B16020D351B20AA9F41528D\
380FF43ADA7B8FEFFFFFF1367C9AD233C8C0B2FABE33585CF6F919B3023C7FB7ABCCF4E52B3BD66\
893D3A010000006A473044022045BE979F4AF84EED3B6CD52FB4BCC6469A9852A2BBE8261682361\
1C244FA02800220391ADB16FB723E33AF724B2D5534C3072E36D5DCE92678BBDBF73AA90912A9C7\
01210398D06500E33B14200D425C9B0E45684F0CA383F84B3825E25979AEF11CE1E231FEFFFFFF7\
4705204CFE72EEA2670341D557A7370964465752B0D22BCD5925FA45EE9B975030000006A473044\
02207995CD6DFB00BE2E22F75FCB822B3720526F4F04D5EF8A6D9787210D80EC06A1022049F149B\
53563A099E6BA92B5716B88802496F7E56AD022A7CF775E457128B75E0121039DBB2CBBE184BD87\
7A5F8D17A5EEF2E64A16D03AB73B66A16AC3B3EEA0967F4DFEFFFFFFAFEE9786554AA98E2D8D88F\
8D516016BADF9403D248FBBAEB10D519C0AA0DA3C010000006A473044022012E06F8DB951E79557\
46B9423C8FE85D2CA1E37200C4C009DE089A06CAB832D2022049EFD2405041CFE16940EFCB411F7\
1CC1000FC3F93698D02C656198749A3B54D01210228A3C27B2C41A1F59D80A2F161101DC3B5026D\
196717E088678FF8B231A67143FEFFFFFF0561299EA65FDD9AF97C196BDDC4B93C312CC5F798D5F\
844D1C6913D1EABD2F3010000006B483045022100870A610164E0DCB090FEA7FA6478B1F0AA8654\
2A636E8DF46AE1A91C686AD0DC02205C3212C5E10B14614D2F766491A66D4EBFBD77B0F90F74CEA\
4E90A001E42059D012103DA12ADD469738B606AE93A3D7A4F3F0501336757E5AE0DC4016D363A6F\
5A70BDFEFFFFFF22B4F7E216A049DDC9FD7C6FB257EC30828E52859C4B465049862D4619848E8B0\
10000006A473044022008220C4E4B9C96409524C0B7DDF1445176FB0CF6E62B88ABD006853B4988\
5C4302206E3591AAA91B124E7ACB34DE1FA07F8D4319900A84E6FC70A17BD4CCFE9B8F15012102A\
11354C6CB7BFF5FF6188CE42D3D5DC93EC8F9404EDA3430416BCC72129FF231FEFFFFFFDCBB291A\
7C18716025CED373DC57C0680FDD58C222E90C7C09DE1CDAEBBA0EBD010000006B4830450221008\
B7D1DCA028EA6A032CAF3D862E5A9A084A6F8E70581B13357C35F7000478104022040EDD2997849\
5F8BC4FC99F121EA963C21B529AA62734A901A7B987191A3549A012103739EB3A069BBB3C8FF1B7\
B51C1BC185229C750356C53A4C0FBD3927881A23961FEFFFFFF87ED5C1F21B1AE3037AE0EC68CFF\
12447523AF825E130C8AA0F868B64807F6A6010000006B4830450221008C91153273E9128BD9CB7\
049F1844B50D2B5825AF1E61799125C3DC79FE1199302203DA34F21DD1BEFF24DB4F5835978FDD7\
BE749F0F7B4DF91777F2417ED1340095012102D25603CDAE1103A4A601387DBB1D4C7DF7829AD37\
490E4458BCCFE39093CEDEEFEFFFFFF683AE07FE0DC8C894F9E7C2B6070860D07FA0CC69791A7BE\
BA98A65AF702FE83000000006A47304402201DEADEC36595ACDA619FF32A312E2B5FFDBC70C73ED\
58F46E2EB2851A233E979022047774AF0064E25386CCEC7904C72240E780E1699D9FF7CD9EC70A3\
ABC9454653012103879A51B9438F86AC38D6F2990E536C0D0745A8907710BD105F87D372754B043\
1FEFFFFFF5AFDDA6672D5B196B8259F8E884856EAA076F47E23E012777B938E8B9E7715BA000000\
006A473044022067BFCE6ACCB6597820EC62769EFCC29A440EA19C26520943321E265C06CCECC20\
2203D9CCA7399D90072F2F4B5021DF23194963DE5EB68720595D754385EEE9F67E1012103766BAB\
25024DA74BA00847369D81D893C09C50BFD15B6719EA2D861F2BEB740DFEFFFFFF75FF5661EA3B7\
6DC9ECEE530575D06BBB2CF32D9214318156F02E9197CC4092A000000006B483045022100CBE951\
E99C698F580BE50D5319D47F0A08BB643933E41BFDAE4FEC45605E69FD022035D432591261A6159\
8869AA1AEBC6CE4C1BA114726595F93E7435822ECFEEBCD012103E0AC96BDB14D17BE846E2B0504\
68A4B24FB7CA23C2D9A2BFD839F5F5997C29B6FEFFFFFF18C8C5F3D9E9CB6A002AB691EC774EE9D\
A0E9CF9A3A2FE937D2B03747E94A2E4010000006B483045022100D36BA815371015D45003CB99C6\
94C3997A7AAF33D18851DE2B2B7044CF4030CD02203839DB353BA237F11E80672906523538BCFF7\
87D3E5EC66A60CDB3E4AF953B110121037334A74AA29BFE7AB699700276A6E285B7DFE9F557D6AD\
A1833E02A8A77366F1FEFFFFFF417BFC6C4CA0A6310508069EAAE2974D01C6C114CFBC5C859243D\
DAF41BA3DDC000000006B483045022100B74EEB98C3331FDA11FD7E8A945F0B8010A1C84EBCE8D2\
E399823D1834E4186D02203D738C75690A6CCCA038AE738327F252F69BF75181840F0F2F8DE14E2\
4214352012103E91F6D79CB4CC2EB850F93F20D91C1691A5AB0B775EB6AB8950AA5E707BC2220FE\
FFFFFF644C7CB732ABA06410B24656BCAC51072D92A3C6C2C682C7D10600F58DF217E9010000006\
A47304402206EA43856560F39551B5696E1EE9288A2DF9C41FDE2F57CE1E011ED4898EF60A20220\
2C892F9E74A78E43FFDF52ACF736832252248F1760D4433BAD2FF527290963500121026B80778FE\
4A9CDF1A84535D30FF113B8FD0EEBE7B3CDE169D4CB236E17421389FEFFFFFF05B6381E55E2ADC7\
0A77ED7C79D44D854671D945EE2516035C17D5BB98896F3B000000006A473044022003AFEDBF08A\
AF93DBA7384CDCF048ECE82A4C60BEC50B991DD1403A8EC2FA7B5022070E342FBC96CABA313C030\
E45C55B9CB63657CE5CCD461ADB3F09FF6E74B083B0121024B0E0CDF76DBA647675AEB0DF7801A3\
1AD752936BDFF47D57F9915E016DD5069FEFFFFFFF6280322EF0227CA615DD595EB257D1F656CC9\
D7E5178417FA084CA77BA7FABF000000006A47304402200A7853B4E6CBED858A47B1DFF15AED754\
6984F41917385A2854A97DC71D0BF24022037AC82403F01331164A3877FF886BA1D234C15A8C82A\
7F8F44BC4DE71DB335BF012102ACC914FE1D1ED7FCFA3B4961DCDC9DF4CC135B8E0A22496628DFD\
205AF7FCF17FEFFFFFF3D167ECBF7F1AE6734895BBCF051EA1172ABB2099FD938D9A10A8EA2ECE0\
FA3E010000006A473044022076DF20F715CB0D0047339D96A0D5150EC8BEF5BD47234D4F4A7F436\
B9E03A520022071DBAC94F08C21868AFFD190DAD50142F9674766E59DB84BCEEABE1F91CE4ED901\
2102259AE783E60DF2E5E285F74D51010FB8A20AE417D678446F3C970AB0A0E066D0FEFFFFFF468\
AA4D2127EB18434E492488D764B3845DFE436D3F5E4590A031545EBECFCC0020000006A47304402\
206B391B36B5736D381A642FFE03356ACB825DEFF530BCA3DF5346034C96F1C169022076247AF54\
6D8F53C9E7C2D5D9C6AD76DAD498E197C463BD3B961D780310F5B7F012103A1C47DC5CD9232687E\
FF984D49A31F235C06D1091C204A0293C214AA080708EDFEFFFFFF58B39144F58122EDDC4D1E4E5\
C5851FC351F57F905DE8867C3675DC5629DC702070000006B48304502210094E1851FAB4E7BED88\
691F43EF3995C2AACADB8BCA4CC3CECF7C7263481DD8B90220302FCEC88F6D5335A824A3BCAA472\
E22ACD274CAE76ED0678E71011792B55BC0012102552EAABAE846FE5F3608451E528DA396026942\
ACEF7D6B1C6B76F5B008BD7829FEFFFFFF2B667B0FA994960753FD862C029616D90FCB52C09D406\
FE10727D076940F677E000000006B483045022100C37BAE93DC73ED82ECCDE6AE2757CE578080C9\
1CF7B9412F636EC507FAA46E0D022065BEB9421C9DCEF98BB4A3B86F9897B579634F77118659863\
F72A1A1EAC2B7490121039046CC567FE667C9E1004B7711F3330594D3E60D38E7CFD3E455265E21\
2283DCFEFFFFFFD02D960668F858936831D81E7DBB71D41C71DBCB0B2DD8E931F49D1C55D33FD00\
90000006A473044022030DAC9C8FCBD6B60B50037FB645815C54CDC53CC7BFC3155FC2DBDE4BD37\
529C022011FDEB07487426F0CC2DFE9A3B8AE6AD5C4FA43F42289AD1A3D3762E5B7595D60121022\
C59981B104A4BFA7DF64529731CAD26C0A7A6C40D00AAB1D103CE626C7A403CFEFFFFFF891A4541\
306087FB90E42696C3071B742B7030163A3366CEB4055D821C36234F010000006A4730440220505\
786076082A1BDA11456C8B2C9B029F7A459BCCDA997E3D1B3ADAB7EF1BB2B02205A20047A869562\
6D07255AFC32D56EDF03291E86772434CE1837A04E8841944E012103B53BE05FECA259841E01053\
A43BAB3C86A76EFEE54D207747EA8FD1BA20FA590FEFFFFFF0CE8D94C324AB9756905E364121479\
35FF2F617809837D82AC8DAEC8D4807BF7010000006B483045022100CA7BF90CD1B8BEB2B7F8BDF\
5FADC960B20656E4EAF0EC8495C6915A6563E0BD80220114C2B6756256E7D233A2245592E7600D8\
A106D69BB5392980076A755AA2E9880121032740C0ABB716B6C2D5F3C3BE36CB8FC277CD44C02CE\
658447E0119AFA6192A86FEFFFFFF21449B803B1691CF3594A66EA9A1BB84E65601EA4832BA8E7A\
039AE46B0BFC340B0000006B483045022100D67D32B837DC30A47EDDCF32702E848C592AF3B83E9\
0254CC5C43D4E5B7884A402206738DA0932EC9DBFF6623BF7BE24DDE6A31F6148A986A714C4860A\
2691F19DE50121027BD15982EB587DACF7A64BEAFC4B944DE8A8643AA121034307354CF45734C48\
8FEFFFFFF19E237152A17AD2AC891EC24A8CBE5C5989B6F2DEF4914263F682916ACD32D9A000000\
006B483045022100C6E20EE7B44BDC1F051E9C0F23C4FA27C318AD0241B2A34D93924397439EBB0\
802202381A084821F77EC06D8F2884F9769927A18A81AC69C96D056E095F3918D39CA0121025FC9\
8803AE5B95FE01AF6FB76F052079C4F3CCE4871A7187AC0ED1BD411E13B1FEFFFFFF3554DE48879\
2B6E0B4AC12E32C3E45BFB006336A5C6911E730E6A11CEB2BEAF9000000006B483045022100831B\
8CB552A10225174DA4F90A61996635BBBEE9B65C72E800732186B963A06E02202C7C4667709FE3E\
1CBF53D595873A27BF42A9DDD5FAA434C1D2B9FCE0E227ECB0121025DE9509348FAF70521AAF971\
E1A8E7D943D3EF9B7E1B29A18851A6D787EAFDCCFEFFFFFF8E53F3DA5DB83D3051DEEBC317A8853\
B7B15B012D9906C835FA75EE9AF8B0FEA000000006A47304402202C9C270654ED162074016BBBA2\
F4052E7D63EF5DE5D17BE147E12064213F9BD302202CFF039E96F866E39D36EFC096B067889F490\
2059183485853B2D7D45DBF4117012102B9396D56B81FE4E4DA088D1DB9574F5E68195D6BEE7E04\
FEDE262EFA7269CEA4FEFFFFFF1CF2E6562D803772BF98BBFB8C3724254727A916195569F8C83F8\
A65F02E7C70010000006A47304402203C18B4F7A4C19AC1A3575A827915D23503153088FD9C4A40\
78C0ABF4054766D902207ADFD844894F2CCC30ECDFA587D9401D5E1B422A1409C55AF05147CB34E\
EB88401210218622AD18B028A0E47A0E9CA7122B492C0253B53FF93AC2A473ABD2D620C7928FEFF\
FFFF5994BEFD40CD16C949E50584C6A74F07113499560E1F531FAEC221D2D0E2CFA9000000006B4\
830450221008EA030624A11C5A7AC7EC7A2F36D96C92367D58A0C848FF8333F7751214F49A10220\
478CD38488AD9C7BE168DFB5F43B0C9A571A00A61121B17C1ACC6F8705D695D90121037575A01A9\
A4FD11EC790DE789742BB5FADEA1B7DB4853F74AD9FFE2933B8E650FEFFFFFFC47BD05F4DB7FCB0\
140FC667B30E63A4E6EA0F380FDAC78452B1FF428EDF97F3140000006A473044022019C3CCFF882\
D5D3E609C92B4066DF00D8D167B03FA8A7D8A01FBBFFB8C239F520220724801B02156767631E9F8\
66962529AEF721ED6C6342C923DF0DA072AEBB5A61012102B0CC1C99217063AC054A333DE6F536D\
4FEFAD556E6890324B7B2B665AD359D21FEFFFFFFF55BC01C90DEABBED852CFEC12D6B96980C666\
E8C32C95C0449443AC8E26588F000000006A4730440220112B7FD06FF5C0A58D241BA4EB02E4BD8\
48B08E4DBBA49011DCCB3B87AC5340202205E75E64F464ABD6551B65EEB5E3655C77B92223CEBC7\
80C9E48898B3AF3624530121025400B6BEAAFEFE92E59D6B811BF2A3F0DCD51C51FF2D66D7C0707\
760658410B8FEFFFFFF9C5586DCA1791EDFBA1C5939EEADBB44786EBE7EFF13DFB1C9223D4EF3AD\
021D010000006B483045022100FEE816D0E9816FE18F170FB4970795C9447F7F341237EB53E8E90\
0C8DCF80C9702202319977805E0ECDD615EC1347DB3FBABD218B98760470243685CF2E0A66C2343\
0121037FD4E0045DADED697A944C2CA12ED13EB48562738F4D98207E59D9B6E113F3A6FEFFFFFFD\
46419E29E0F511B43DC205A2A77AC35868DFE5DBD7BD55BC0DEEF335462C37D070000006B483045\
022100F7F4757E3C742D85DAF330E036F16CC1D5627B0B1FDD03E3F87221F0403CE15B022059EDA\
737927AAC3117A6C3734FC353E1E5F635F90A6F18DEA0DF7875737D867F0121026D5C5C63F245CA\
8BFEBA85A1395137707FBCE9B827F4B2C22F9EB58EF41EF6B0FEFFFFFFE68661DC376F75F56D71D\
E357D27EA2C474E1384A0378307D5F6C6731162830D010000006B483045022100F2A7FC51DCF65B\
0B39F60D51A2338B688C1E9418A336087FB9D3E7699AE86F6C0220591BB0A21813FAA7F8E6C9B2F\
ED5FD5FDFA3B4B8F7D27C3ADC043E011B8121EE0121031D07552140E962E9452788D3F7D9C50D34\
3E2A2C849CC6B68F3FBA89C85181DAFEFFFFFFE7A400208C81BD362680DB219B44BA22101F8F2BB\
02B7BD9D4299F18FF7ED45E080000006A47304402206A179035A8EC90EA62724DCCF0E2526E614F\
06B8ACAEBFF9CF79BBB477DF6D34022060D006AC00F89CDCA843BF1BB2ECB9F046C1599088B8756\
EE0DBCDD64098F4FD01210208C545ECDF288D30346841F7BF26E580E4CAC93721C5AE8C99C7E4DC\
A6B1096FFEFFFFFF995CC41A32CB78E49C04312552237AC77CC8ACE19E611454BAC04346D1343E3\
0000000006A473044022018A532FC27934D10A80A14EA5BD9C1BD40D2ECCE247726899AFA83C77F\
B7437502201ADCA883026DF650142BC69A49AD5968BFB5DE3E2499652A16944A3EF6299A2801210\
36C23EEAABFD8746ABA23EF87438651B29DD1AC3BF85DA4C54E85A5A6745C66E8FEFFFFFF16FC0A\
A6F323E216CC5F1475BB7AF470F134413E7ADAC5D29394215F14995604150000006B48304502210\
0EFA4CC8E8CBA68BAEDA18ABD0D2FE7E8B58D7820EE253EA587D803CE83E3F245022052BF18B377\
E52CD46820739E65E3735F869A2D3C1BDF359F7F628768B13419FA012102AEF0B4246C74908DEDA\
541394B8F1BA3BE6854F28495AAA23B4869EBA282057FFEFFFFFF47678A0E1BDE86DBCB0D82F30C\
D527587F5D8323CB4F29A3BF1EAF888F208AFC000000006A47304402207F46360E3A7B0A3346A24\
1070BEB32650E43FBD0743E4C9C32F544017F5A9E9A02207229BA9A96A0F9D91C323A6B40F69A52\
C221EA81B76D57F81CF8A856610FC74E012102DD998703743C254D28ADC29BF245BEC81DD3C6E2E\
14141F4D309326AD72C4166FEFFFFFF3BB12F10C78FE03C1832AEB2DEA4F42335158FF274476BC4\
4C3BDBD40C25D25A000000006B483045022100FCD77AE3978F03D26A848E2589CE5606B66D17940\
E0C127AAB8C2108C5F83F4D022038C7A72BA24CE4CEE624C561B01735D8AD9020DF7E9C6E62B34B\
6D7772B4DF2A012102132F5FDD86FF2CC0BBAB1D23E95AE99A6D65CC5C0073564DC0E95E8CD5FFB\
9F0FEFFFFFF37C301D11D8FDE11FAFF9F7251B8816CEAB299823F357F084D50C3CF8500344B1400\
00006A47304402207E617D80F1C6631A9BDC2CB6D9FCAC470B91EC53D53C25E84FF5CE609DAEDD7\
F02201B23EF39BCE8660A2A750319EC7AA11492205BB7442E3C42A4BC054164C422B6012102F4FD\
07D4E0BDF1039A279311E95AA3AE6FD53781BCFC2354EEFFFE66A45824F0FEFFFFFFC32CBAB87B7\
241D5572EC718F0694C7BB580C87A64AD646D4796D5C817F13D76000000006A4730440220330820\
C4DF5758BB3B8CB8CB79B050A96A1DF40A98B230FF49EC5475899EE24B02204DBCC6389EAAD496A\
2215C75DA1292568F5E08A0913AD2A6B93954AB05FD5C65012103C17012DB5F40BE38BCC95EE2A6\
D11EBAC473BABF115A56267175A8D04C8B3086FEFFFFFFDEDAA87846744B95ADDF61A0C0A1D3034\
FBDA332FE00E15191029B925C358414000000006B4830450221008A115015ED82417E031FE1A753\
4AF91111608E153653BE672299376AF146987B02205461ED5DB7BDE2707F34A1B536B86817B4524\
86B673408BE58C13A18442E6740012102E3E63B8743EB94862BD72A2F7F94427EAA269FF41A3896\
3508484E5B50205C0EFEFFFFFFDDCE6AF3BFF1E43BF38B289996B7B1478A981FBCB1F7812EF8AA7\
CC0351525A3010000006A473044022100E6B9389FE035B9A4C82E14F96DF565ED225F97E2735CF7\
E5AB42AB22F653E848021F22511ABF446745BFA0BC3A7CB142CA133D655345BE58687A3500DFB2B\
87F0E0121038CE98C9B799CBBD832DAFF5CB69400C243ABFD250ED1FD3E48965E0EEEB31A55FEFF\
FFFF76FFB6EDA9F20204BF447626F906666FA248EBCEE2D3D86222A046A17979D752050000006A4\
7304402201102046ADAEBEA25A82D935A127DDE4264E1BC9A2F13FEF9B72344E2D6B47D8F02206B\
D03A6F0511001D75E1F8F7BB5A15BF65CF99838C2BC6ADDF76CD4C2B9A237C01210331D5C4EC1DB\
AC43515B260F3B6349C72CF03378BB3EB5F6803EDD7B7FF412D70FEFFFFFF30F2C68263EE05AB88\
9F0FA0FA8C0BC1D46EA4E8349717CDA89B9739C7D62D0C000000006A473044022079448A2685016\
7BE110F33B80FD2D899E95BE1D36DC0EA540085DDF94244CCD302206DEFD0F55434D5C3ACA54632\
6ADB9641F954792657522FAA3499DFDB0C58B530012102009E7CE741367C5B9DF1048FCC2B7035A\
4F11C85D1144F9DB62F4FCE06350900FEFFFFFF9E2258153298A451D6A311611BA7D79364754A22\
DA0E7A81C6254670A03B9EED010000006B48304502210082622E39951A15470082D333583231D05\
E3C1500EE0D442F21504FE2805BEA9902205A984A95594732349B4A86CDD4585CC7D6461D3EF71C\
6193AC0D5254971981B401210265098C2CCCB59E6E8BAEC8E6C0C2126DD42E68E81FEC918791057\
47BA2B32CCDFEFFFFFF96BFDE80CBF86879124C8716196B71AB52D0B79842D8603260FD15FDD2ED\
BF1F010000006A47304402200D4EEF32E12FE36B93BF39067D1A1616ED900077336F4963FFC0E83\
51F22D6460220766B65658133F9FB4987FDF77995A2D7823E548B0ACD5DE90EAF39CEBA0C0DEB01\
2103C770D266EA126DCDEC62DD44186E4353B589EE76B4D2C1766D2B002ACA73DE81FEFFFFFFC2B\
CC008E7D424D957AE81ED02ED9AD1B38911D9AC7B845B68DDB3267039CAAC010000006A47304402\
2034BAC062D015B22C2115AF04F6B22A8F7C643EC589DD1A1FDEDDD32F077F2D3802201CEBA7F49\
9F0BD700989C2245663EFE145EBBC4E7BF27B2F6FF0929F3BB9444F0121027D373A0E7E99C5B0FC\
A49C4B5DE5137B2FA1DD5B51D0F86DA193EF374055EA06FEFFFFFF02108191DCDA55D796EC36DEE\
786F664E6D10ED64061D590EEE9BA13CA4215CD010000006B483045022100B6B1C1417457FC9C43\
672D05DB3260DCF4D2AC600834CAE2093A12F2EA363E69022051AADCD5B2D4470734C748B7280FD\
73EEE6BA213DC0099E54D8CCA01426CDED0012103333782355FC8FBA9F29F153EAE2FB08CF72A49\
724457A72C3D4AC1832F26FB16FEFFFFFFD3D0FEA1A496FB64B133BCDAF3001F0A836B615795DAC\
48D9E581360C34E1AAB000000006B483045022100857BC8FCFBF7C78C16051C1AAE2BEBC78E5783\
32A5C287D77911C1B037A404190220336CCEBE9E48114C3DB49AABFF4771B487A8E02DBF360EB31\
C09DE8AE015694B0121025A24E573881A72B2438F59A23021BD40CE77BFF05D53F05AF5BDD933B2\
25B6A2FEFFFFFF89C401FDAADE418F723853A37EC52B614D45E4A50514FB8ACE9478BDDB06E9D60\
10000006B483045022100E0A539FC455F52C30A7A3A868A77EA88107A3024EDC3B57CB96E129C9C\
7A1F4E022011F35E716C288AEA8C6116BCC693A79A3295E86BC03B226AC831CFBD13EB3DF601210\
3BEF28C5A92D219640BF7885F748C5DD0FAA93C03AE2CC8095440CC49D51B7067FEFFFFFFD5C452\
989D6B749715B3B966BE83026A0002E64AB3D9F4AB0FD61A2C9378D4D5000000006A47304402200\
50EAA77297D56CCAC6636D9023F5B8676329CE9D08AC5152D8F670B8B6C9B3202201901CCBE550E\
AC6C8DB2360861F8AA0C2844BD47051AAE65F4D2A3EDDDEBB22E0121037EDB4238FFFBB08ABC650\
221361376DC4ED936989893560433AF3A67BD5633C2FEFFFFFF28E360CE54778BA4D1606296AFC8\
A4962BB29BD0A55B375CF3CD8157F40A89A1000000006A47304402202281BC34C0A4FAD4E4A3189\
954A3DEDA7E790FFDC591AFC81E9FF3134C6C49F202201C57797EB7EA4F0C9A859008385C689BE6\
783C744EB6B73F756E07F27731DC42012102F012AD4379BF1AC14395BACA93143FFAEC500CE413E\
5127F2EF14CE74DD4AE81FEFFFFFF6D25FE4FCF1FB1D5D126BC3AC1F94DA900CF56B56B937B2260\
B278ED0E913FE6010000006A473044022072F1DC81BD6A1C6DFED1FB73AC72818D0D7259CFB31CD\
8005A2B56244E19F2420220429487F365135C0AE0399BB065DE39F61AC4A74A21073F6EFFB759FA\
3513FD020121039D4C29A6DC5BEAC03952B2F0C332405E9DDEB4CAADF644DB92E6CFC1344D5C3AF\
EFFFFFFDF4F882B5AA58160EA137C262347740D57A368BCE845A06B3B16F0111280C63300000000\
6A4730440220171C8F0A89C195445B46A64A1BBD1E2C27C951F1D44BAFFFF4A69AE347A26A9D022\
010C2296DE865A5C2819FF084A4E8769FDC61F7BFD06A82906008EB4121E8014B012102C27ECCB4\
E1DF353AF7AAC8E136B946933E53AB0A2B7367AD683191F05DC0EB7FFEFFFFFFFD319F3265F97B0\
C54F5DB1797EF4FBE324B69FA93A0879FD1BE3ABD2A740A28010000006A47304402207385688773\
9B3CBD546B60D12B1084370C40E229154001B576673706EF5498500220250FA12139D1BE277DBB8\
681CF6895F0F2B9C26D3580B40E2960B57FFB0047860121029380E4824859865ABE6922B4747D1C\
0A9FA622B71F912B380286D04C44CC398CFEFFFFFFA31C49A747EADFC96565D51728E98ACBF687E\
D507B2C52CF99404A46657743E4010000006B48304502210095A762252512F552183170784F6471\
FB51E4894D4BEB7A3C0CC848BAAAEF927B02205F01B2A41FB3B0C474C13ACFB4893239FE92030FC\
E39A28BFFFE76849F371718012103E845635E5B59C3888C99441B05FC7938B44A8E87CEE85A9B80\
0301DA5EFA66A4FEFFFFFF573D7E43F009995EBFC8F31F7EA64AAA9B66EDAB7392A2A7E00167CFE\
98FDD1D010000006B483045022100CF8382AA810BDA4D80CBF9FCE7E23EC0631FDB201EDE45D2C7\
065545F0CBEE760220650FACC1C2D573D3A9F115578E07353FBF9CF4D8313EC458FF12089CA8A5C\
4BB012103C76116728F2C5EB87B338F8464CB021058C9A3D938293E767C56A8526720BCABFEFFFF\
FF501779CF1DF19E63F99D7FD14F462F5FCCA5A9827C5E13FF2BBEECAE80846FA3000000006B483\
045022100D2B1312F6937873E7B01ABE38771017B010D55E957CF0E548E14008046890FE4022039\
E1E07519A4DBDDE5DEFAE0E293E8FBAFDD1F08CDEB6C009CA051E49C1ED508012103E645364750C\
2F3C76553C0F44FB8209896C80B8698E30337BB6F466006E054B6FEFFFFFFF6EA7EB42AACC09B43\
ED6DCF9A1D0C98D589AD0B2596523EAEA1BE797A6A9FFF000000006A47304402205536ED569CD23\
3FEFEB5028B8B5AA74B2CC7E203895803C02D8DE576A700846F02207D90E3231BBFE6C1C963ED53\
C1212FA9E748DB1C2E0596A7C1BC432988254D9A0121038E0E88AE26275ACA22490DBD4F7923173\
FE25B0A2CC9061BA1C0D2F49770B540FEFFFFFFED1AC2063591FF9FA065F1E7FFF85109889FED65\
A47E2A9CAE61E9A8D9F2A077000000006A473044022055D8AD09423A5E668FE19EE085860512A89\
2FA1DAACC507CC391D8CD795BF8C3022064C6C7B5244FE25D995862F91FD01AA48FEB34C5918058\
D4BD7C6F2DDCABDF2601210336972D89D2CE13D7F95C93EBE3010C12811804A2760D2E2EDD2EC28\
73C270AD2FEFFFFFF5CC5DA182EE5FF288F5017553D2B0615146A08405A5B947A65B6B2B6FD9C57\
54010000006A4730440220263322FF94488B67DC8ABAD4EF3E65433250C8C4F88106BC53C1103C7\
7C7CB4902200C8C5408395F4CA33C922E4ADE389DC1B2B3B06CA8D786CBE822072FEFC5D9030121\
02A9F18796D50C4BCA187B5BF741F3B5980C5544246C33CBA4310D6351B431F46CFEFFFFFFB0D81\
CF687911989A07BC81CA302E0E5F6A3D1096CD326CC63CAC79131FBBEB20B0000006A4730440220\
7AFE48BCE2842CD387BADAE90B0FB725F753521B5A04B317AF242C7AD23A8C1E022053B68A4C484\
2F749457D163A8267D6E76FCE26CB015C8CB0DB6E299BCC53995A012102067BCB178C3D95D4534F\
76903DBB5A256D39D87A7D49C0DE2B04ADDBFD280EC3FEFFFFFFCBB33F6D9E365ECAE8D1322CCE9\
628322E885708914A920F578D8CA9680437F8180000006B483045022100D0A9B09BAD722315B253\
42DC86E5EC54993ADD99EBED2949C375B13C142D1F7002201A19C5C4AD9E007914676009D2FD2CE\
3F40B542378361500F8E7FD1CB7A317AF012103CF28851DF85C40486B16687515B07420B57E0AAA\
0C3C1A67E2A845FB61A0EC50FEFFFFFFD0A44F16F191B7E6FDBBD8109D1D13E3C969A35D69C013F\
E0FEC00942878D522070000006B483045022100A09B94EC6474E4BE9DDE1318FE30EEC85F49E884\
2850C69699B1D70978C99DA602201390802BAE8B9C3FB29E3C82F218A03AC00CF82DAE3EA099634\
A7A67794E6E32012102443389DD9C52D6D062B503283F5EE1E4092ACD3A8318FF1E717085762E9F\
0DBCFEFFFFFF2FF4E5FFF3ABF721F3046F818F53B915939D335CA9866D97FED405FF86764D8F000\
000006B48304502210097DE78F17CB7A11FA7179F4DD77A7EF569C9BC61638364EE32BF1218BAD8\
E50302203BD549D1B977E5F7A531957067625A8E1DB724F286041136B7CEB1CC2DAEBC2E0121030\
7DB913D2EBCCF80892306A1661E5F730335FCACCF2F242640A7B11A465810AEFEFFFFFFF298E7EF\
E1D2B5137D0B5692CD931188B34743F4F3687BBC16A463A14A64864A010000006A473044022008F\
1C2E9018E049EA532E1C0A7FEE0DD6E3E031DD5CAF85D12129ED34E6840CC0220377B92B20CFC01\
D16782E350EC4571F5D594E33F66071CBD1375E7C0C8E7C9B3012103BFF5C70175ECFD9AFD72982\
1C4874E8FD61C2133DB785CB32DCEE4355380E0F0FEFFFFFF02E57648B2ECEF8D9CF4DB1297B5FF\
8D7C639AD41B1C45CF25C20FA4E1D5DCCF010000006B483045022100F5BD6E1343E6905B284BF35\
B823BAF3CD718B665185BF85CD88159CAC763A07402207E2118A3571D534048C96EB1C2D1B79341\
8AB5B136BF55715C5F4741D56BD19D01210381A8D780A094980CF25EBB10DC345540AF2C6EC3DB2\
F0B683532BC70010B6660FEFFFFFFD2B93130AE2EB7149E2B2B397E54978781A679A2209EE5D755\
5E33FCB7475107000000006A4730440220523AC5DA1314697352CDE15292AF0F61F3D7D0709A069\
75C49C752D2CF3E1D630220132872C23004F7DC2BAE1D6479111F092F992DFDFBD6BE897CA032F1\
F6EFCB50012103B5392153A6F09D264CE4140829A3039C299E793BC59E9234C77D1C75A1966B5BF\
EFFFFFF8C16B207A9FEC33451DA26A35C07505506668DC66C2933BF4A5B5B24A7365B7701000000\
6A47304402205520AB5CED42939673578CC4247140BCDBBCEF281BAB530F0C2DCEDEDB69C890022\
054AA33A3ED3F21CF3F24897BCF5B498D6D0307EB5B8F02C8120CDDC61C7CFE24012103E81D6ABE\
447D0CA8A07C937ECC4CE2ED5D5FB2312F62804A7F18585C73628D98FEFFFFFF161CDEBE441843A\
072C6CDE0474946BEE8D7A6CA151D6768572D5A78A7F1E6BB010000006A47304402206DB73FF685\
3B0A95996C9AE0A1A4A631703549BC3E1956BDDAB886992D412915022009B6478A76382EB3B8108\
586424B214814411C8C8852F51C5AA6838A387EEB5C01210345C1D21EE744D61376D390EA739027\
606280E6ED45EE0C5663441C0F04B94D32FEFFFFFF3F95A312635DFB3FCC7C6D877295B51142703\
33EB38B4F7FA3D207D3BF1F9A77040000006A473044022026E306A6D351A045087EBF0E6952A94B\
7A93D7079A0D3A5CF78EEC7ACE9C766E02207DEB4EC133982F86FA8C81DEE185F288698CA1670D6\
598BF5D82B94AEB451D250121022FE57E77C24CD57D47A49D7F0907092301E60A92750CF434B622\
5468CAB95C84FEFFFFFF652933BA2AA6694CCAAC18878189728FC1E513836A7B8DEB1F65D82659E\
68629000000006A47304402203B9157360B22104BCF4009880329AFB250B9568BB9AEFD8C69DEA8\
20EE5ECBB6022032CD59BC9830E751A3A5D599FF8DF10F87E1A2483C974D527839500F4E4F94AB0\
121021D3EBB166611A12F61FFCE0388DA09106FF36B303C0BD5F081D07D644A053B14FEFFFFFFC4\
4621A4C4012DE3C926508C8DF62ACD36FC2398766BEA52AC720F8ABB0A0179010000006B4830450\
22100BF08F3011B17D5553C83EC5B63AA85C6E7F6BA511AA720F33DCA464AB0D3E184022011003B\
E76A5976C03BC7E6766FC52415E42E28A5842309FE762121B59418095D012103FDF12B51F8EAC63\
9E5D6ACF79A1F3E5BC5926ED43EAB9957CD086F4BF5D44ABAFEFFFFFF099AF152E3DBE428A784B0\
A9DDC79A6BCED4433EE50542E375CA629A35C7017C010000006B483045022100B0F90C7C27DA9D5\
E2489DFB8F5FA027B67013A74635A5210E3D339C9B3BFD80202203359CB71E2FBFFCE3636F350D8\
102E3C50EAC5DBAC8C8E07BC1C7F43604FC1520121033654DA1F2E0811BFB4FA8C031F66A59F8D9\
9549F07AC2C8EC9AE7CD90AC5F01AFEFFFFFF3F6FEA19F33E598C76556C60FF9641616468B3C8DE\
FBB43DDB0EC533C9C8678D140000006A47304402207EC3F4BE4C90903133C58CD7D25BED5EC8DF7\
BFF2523DDFDB25CCE183786DF4402201B90C74A89D1822D43DE1DED2F772854A96E73B8D7E850C0\
5FD13686111AD0F601210396D7A12F14DFF12C19A50CFF8FDE7C5F88AD2D997BB3D972C1CBCA1B0\
AD4DAC0FEFFFFFF15457BA18CF51C401926FBF843F9A28B97C7BA4262BB915BEF32EB8BC71F88F5\
000000006A47304402206A7E8A30E0D90B00A5CAFEE756D9863CEFA02B258637AE8D0459211D024\
B771C022030B18AF1D4D99B5E3A29BE933775A423CF3C6781B5D38393F6E025E3E77C2BFD012102\
4BCF307B8C12CDEC37A3872287DA616CEEC4E21895B97BAF02A41C3ABCFE5B88FEFFFFFF9DEAAA7\
1E970C82AB2493BA59FAEFA46E45077D347D0A3E7A080AE8212F21001000000006B483045022100\
8DCA2F6AE08591C9E36143FA5F4870AF122EE0F4DC5731EBA7C9412FE611229A022027192F965DC\
71032EFDDD4F31B97C11A2AB76231BE01D7834F06CE4D0A17E97F012103E1B66B9DE39F1737DF0B\
3CAF1DD69C6E32419B6DED224BD08800C45C317A9659FEFFFFFFF2F6D358CDEBA04F3ABBFAEB93B\
DD584B98DA52B686C3EB2355C07EECC146487010000006B483045022100B0D39ED41FD374C99E1B\
52355DFFFD078C122B4E2938B6E72BAC80AB385AD846022067F6D0B0114937824FFA9B214CBA47A\
D710EC041EE0AC0E2C24ED6E4A0C60ED8012102C554B46D76DE0487FC69B4C483ED00B1E86DCC83\
A8B63986BD83D41D246EBCEAFEFFFFFF2FB6B731C44DD05EEABCB031E76C39CEFFF44C7F11B289D\
E64188A323DC49388000000006A47304402205974716CE83A43801A979A6A916EBA0DFA8C73248F\
2927C3F29CC82C9AC6CA72022011D82CE40BA470839CAAA8538C98EA4616DABE0D7A9DAC1BF3C56\
31C531B0E11012102750497EB4E3A07937A7E80AA562C9D75D60D436671273EB7ACA3D211EC8A33\
E3FEFFFFFF69D0609AC7D311C2BBAEC7D8F66D9A889C0CCEF8377E21E6FABB2181C77EA53901000\
0006B483045022100C2DED1B6D541CD173124E3ECEA863D107B37F02DE5F37F2B60C1218250F292\
350220279BDEB858C06B167A66209AE7E8DF0C7232988769DC6E3AA6B8C920CC94B3CB0121035E4\
C3600866BD21F3C5111CE54FF46B4CDCBC498118FE7DD22220BBE42E119E0FEFFFFFF3F744DF296\
8379A1BE8D037D8E0BFBEA75FEED90DE87F1E8A15E3ECAF62DFDC3010000006A473044022008865\
C7F50209939D29194C073479BDF665520DBC978AF9B0F12A5D53DE9E5640220490F6161BBDB589E\
8A373BB101188C722DA8A0B4FC11EFED36D346693B18BB160121021FF88538CF406BA64BBD13BFE\
4659526AC99257787676FF8CD9CF9EA654A09D3FEFFFFFF84CBBB38DE5487DD22FC4C9FE01D96A2\
0448F397145BB2B9E370FE3DE11F3D34000000006B483045022100AC01E9E7887592A6BC8E32BA4\
C0D1A40AA4F3AA8A8AF43360328C5FC5A56F9BE0220128C2E2A2940806A8E9976A0A5556A9EF2B9\
3629D7CA1AF57D4C44C5C75B4B15012103A8C23B6C54011B27B8BCDF91E3ADBA8E5E7FE17FCD099\
F3132221C9B3D33B42EFEFFFFFF9CABFDC08DF616FC9EBC64E987EC459D59B317E9498B3CDD1EF0\
9C9C21F6EAC6000000006A47304402203713C8A2C03E2D3D1A09846308B98DB15FD631E6C3F44D2\
F2914653B4AD568DC022025EE9A375FC9F66504629F1CB704854C93F6891E36AC5EDED4DE27F13A\
40B945012102B52E38A1AC8BDA7E0B212D60DB639D857BB55C47286C7BA27B04B331E2224F16FEF\
FFFFF54C5AB5031DB05B46D84D87FEDE50DBB06473CB992A50B515BBDFCDA6250B8C6000000006A\
473044022025483D2B6A7D089B3FC55613C166C113A486C64A821711F9DB58A258261F859B02200\
549E300AB5CEDA1444BA926124F545568403CEF4CEA0AD4DB8935C6F8BBD0C60121023D845434C7\
3F80D9D56FAAAD38F627AF0C5BB288CA29674001BDD6849BCAFED6FEFFFFFFB78C9F522A4A84DDD\
E2C41CE06FA6C8398E15767B8E472FA5F0395D9079B18BA010000006A4730440220727C1281E872\
9388F5F1531D258791248A41187E628C87D7F760BCF93508197D0220706FFFC394B102B5C6D4B31\
71A6DBED56E357697D955DAECD5416DBC226AB97F0121023346BFD5E11991DF8AAA5459EB64B782\
5DF1C3B1BBDCFAFE43C02F53EE266CD3FEFFFFFFFA2CD1C20344663693BADDC3119CEAF1CF2CD7D\
A2F64CFFD170DF9C4B6CAE817000000006B483045022100818E70C4291D74171B59436F73ACD762\
F8D821AFBD789E9CF10D35EC6EABB631022036E07BBD5CDC283A3E4A0D1065984B3033AB21B891C\
0E3933D30F0B278ED7A3E0121036156883F77671207A4FF8F5806A00C2473ECC9146615322370BA\
A1B7F1834FBBFEFFFFFF87F2463856FDF37336E56681289C07403267287170AF7EE9712B2BCC144\
579EF000000006B483045022100A17731F5214938525EAD16D941FAC526906531FC2F46916180B1\
3033881A53AC022012E1EB84983E8B48966B796716A389C6886A08425D1E809A78394ECF3959430\
D0121032325527279DECE93C851652FDE48805D19EE694FDE9134AF39199CE868DB5990FEFFFFFF\
C867471EB4FD802031E2270EE8DF7A7469EC74F747731BB662A7FCDF88EE3A7C000000006A47304\
4022047820D47BC3B366F095F5662C4D75F05AF0CA9CA1B5DAF037B266305A54B93AE022051D0BA\
3DE7E63187543B322B94C5748B8E936FBF821BE9F8A1CCCE771A2FF38301210267A88EE893203BF\
67C004CABA71018BA9EFA04BDC7C6D78F54BFC37EFA88427AFEFFFFFF73EC49CBC698FD50CBAB5C\
7AC67E56A3D36FF93AE66EB803024086251FAACC07010000006A47304402206257F920AD2E39786\
B9BCC69DFAF66A84DD3D258F218EF5C08F703BCF781C936022026FD6A92D0BC3C7B8F66D0A95AE2\
70718C8D7CEC6E242CE4806BCCEF0C41D18501210269B7CC8E119AEE509BF6E10553A8C8E4F0F61\
1AC063BB197DE3ED99AE4095BBAFEFFFFFFC645FBBC25E233E2DDDD00A3F71454C6BEBCD0EEB9E6\
348224A39BDC716C888F000000006A47304402207E50AD648FAE894F50C48DED29ABEAE206DDB65\
91C08FF05FEB3A6A095F9104D0220536F74022015857D7F1F88FDD19EDFDED96C6AEA348A058999\
289AFFED8F5D5B01210269A737067EA3A071136DDFB2EDE6FDC2AB01B7B316EFB7E6EF812C13545\
608B6FEFFFFFFD554564989CB06CE19F5164D38FEF2A2B79C5F0AC75A5FF128E423363D3F0D5100\
0000006B483045022100A97989150E84D86789B83C25EF2A15CEE2216601E9794C22212027C8278\
A2BC302200B824282BA013207E025C865E6846759A45935D50D7772D261DD8688239559FD012103\
F87B4CBE6F1F629157C517D3A91FD2CA64DA64916CF18C7084E1C20CE5DA5B10FEFFFFFF018ABA3\
07C226355C5DB18573DC8CCF1121727B999B4455675620833D9DA13CE000000006A473044022027\
26D4EB24056F016A9F5817059AFD5919D205DC4FAEA00A38AD45D0F22AC11E02205E0193B77C66B\
54D25015405DFE2BCCDAF98BA3576F8F727609ECD1D10842738012103632ACC08C3F0270D7F80A1\
1480E1796AF7CE276D9A086DA9CCEB893941B2AAF5FEFFFFFFFDA860F7193BB977FE7895FFF5769\
52984636057AC0FF3D3E8E3597522202805000000006A47304402205B712A1E7AF7FDD89A321DA3\
B20B21F76D046DBB5C96FB36670F9C200950995802201A6AD5C005BA0DB41244F209EEC65403125\
99D3B942D7FDAAEC70953B32C95BA0121027A293214F63A0787DC1C1DE3D6EF89A69350B99B1CB9\
EA4BA3A62B68324C3212FEFFFFFF6CB200FBB60135A5EA4FC30DF4F0348A0956CB188E8A2430907\
25B08F0B9B2F7010000006B483045022100CDE41AA353276226D02B5EAE96099B9A01C112DD5E1F\
B048AC4A979EFEBB449D022005A3417D6D1942FB7C012366A2F868268700780F6478050692D74E4\
AAD9B3FAD012103ECC1EE28CEFD50E14E8319D2AA58350B8D30FF70620E85E9603C5E2DFF1FD5E6\
FEFFFFFFE0B8A4E95069967D57CCD7B98D066A6B73D6F13822C8E46D55550FEB8287A88A0000000\
06A47304402200227FDB94FFA186F4ECFB41920034455E9EE7F630214657F67AF62B2056465FE02\
202C398A59875F2471765F01017CAEFE7BFCBC18EE4A192F2F667EF53A27BAA73E0121036276DAB\
B7DDE3338D93A8DD2615DF74E5A5E0A4797963C3189730370AB9D05C1FEFFFFFF61522C58A248E1\
E4DBBA6292AA78E672C04F427C4FC9843DFB8E9CA1B8EC37ED010000006A473044022006062587C\
254927C10ED39EF6F531E7BA65163B2BDF5958893EAF57FDA53340902201CF296FF465E8EA6B0F9\
B4CA0F5DDFB3A6DDD3C736D4997634FB341BDF86E7CD012103150438FBDE47767EE12B0893CE6C5\
3925DF88366BE70A771B47093F0A9EFD20BFEFFFFFF3E907FCE86F71A03FAEE579F0CFB79AA9AAE\
5E5474BDE59213328F1762894A57000000006A47304402201F7BF624281CC08BC9B0B9A0BBAA346\
526D8C66CE3F2F2D5EF8F63D862F03ED402200305BE5DD8681283EF5C3C072FBD75D7AA5B35D36E\
996E85C66382A430D1142601210223459F9FF562D9E4EF58D27FE6BE9C5A49B02EB9BB7C8159915\
1633A54470553FEFFFFFF797CA1A79510B9DF8EBE62C2B647617808C3FE4103D1D4F8B0780A9C91\
8EF9DC020000006A47304402205A6887D76B779BDA030651AF2B788F3B0AFC5B9D8BA8354987074\
0CDB78260BC0220541769E5044652B1D0B17A93E5BD641A647D3EBA53EAC3707DA165F3C2542FCD\
012102B09A6E8E42E46414A3C4AB9FF530423A1EBAC437A6111F8A04F4D66847E19229FEFFFFFFD\
F9DFC737318152714A397B128CC2DE4C16C2E106469ACE94263071917E14E78000000006B483045\
02210080B24C28C7546738D362C0D59BC4F2B73CCD28B852F31004FA2FAE2C353953230220314DC\
E8AC786C07E95950CEE7E40315454857598D0E70E1779E172321E2B0C38012102C60BF058E74FAC\
4B20EB085AA2ABC9FFDA1E437BE212E9BB2E9C145F934952B4FEFFFFFF7F69F0E83F64ACE3EDDF6\
3A257116F2B6DBFFD5D362AE8D9DEE9729950084565000000006B483045022100B138F7FE60AD0B\
C7C9D4EA0DB1EA0B508106AEEE8B283FBE0BA413A3EECC9E0602204B91C3469820C84E026083A50\
E3CA00150C11FDDE149E1FCEAF1BA8DC542DB56012102EB420EFF6DD6A59E5804D256FCC77B5FD3\
1FAD37C12049FDFE119D44CB8D45A2FEFFFFFF962B84D9F09FEDEEBE817654B4F8898E965E3C8DA\
F9AF16292FDF2D9822F8868000000006B483045022100C1E8F83DFA23182ED6AF438F4004A5EE16\
E71F10E420F50B186C1D52B5662772022014D9E8E0A5A049044D86E8BCE575654778E647D2777CF\
3BC74B5E162366CA92B01210315300C555CBB0FC526DC81BE4134936B364633142DDB5243D7081B\
B316119C13FEFFFFFF8C93E8E1D917AD5A72D71C56931D5C8353DA7A60EAE996D2B39131E02CAC0\
F9D000000006B483045022100FA8EC55BAC1AB03F6456F5CAFE2CFC7BC6B5A310EFC9C04ABC5DEE\
EF549F3FDD02202F1FB458E09830ED08828DC3C0ADB138D9C494371EDCC7FADF81BA05C417EC460\
121030520D5ED89F1210BFEC45590B5293F9FC373E5C1B2576A627347F3DDCEDD4D88FEFFFFFF9E\
DD8E09FB452D10C12CDD03CE757DD14CE7AB15B586A0187F66ECFFEBAD53D5010000006B4830450\
22100B60754B0CC635C8F93EE5410B1B15660DBF034571954D3F6B65460137CAFCB640220028C5B\
AC8D9F6E7DA436C0FD74ABB10FF7A17052BEE25905133148FFF6AAD34E012102815B1DE93DC4AEE\
395E7A38CCA09F1199E87178FE9E0A56E967205C0021AE8D2FEFFFFFFEFFF7CCCF5157F7DD85803\
F8CCE2033AE33312EAD238E971A5EE5B8D0CA1DD79010000006B483045022100B9EE631C7CFACD7\
EE9BC2093A074141F21DD22664047AB516DDAAD86B636E84A02203AA691954451E6AD1DE5DF9949\
40D86959367D46CB55C7C7AA0658334E1B87AE0121032C60792D0B0AD80D1AE5EA41D5E5264D274\
443D696D91E8E1C52538BBEB403C2FEFFFFFF4AA25EF4363E0741B87760DB332631D3E99D565127\
DC5149AACD0E75547CF995010000006B483045022100896524F146E855EA0318787DD7A553D2D11\
29AED3E2EF7136BFC90EEECC6DC5202204F7B1DCD6326D435C7D3C6339C36E223C76EBDE5A226FA\
F70857BA37A29A081A0121021DC23E7572F498C575610CD7B58B21EEDC776654BFE2FC5F23FC099\
67BFBC39FFEFFFFFF5D13F2C34DC4CE9286FB3F6A8E12A1F607034842E1734FACBC960F033CE1CB\
71010000006A47304402205B71CD7B1745C79FAC156B6AEA62B22C70BCEE3905E54DFF1259A7A9F\
B3BFF1802200AD6DAB2098332326C8D55C9708520B5FEA9F1040EA7B29C5F0EC5E57C1EE8F80121\
028E6A9AD0984AC0CDCD635E75CBF6E425C7E8CE5DD177D4CC6CB1BE5AACF15E61FEFFFFFFA6B46\
4EE98FA19D61BC4C4C0862EBFE280D18159A17633592543F0AECFF54F82010000006A4730440220\
78068BDFEA000DC348C08F796CE644FB17247643E72115A1D55EC0BAC8FE30B2022063A8A80ADCF\
13F7EFFAE4497FDE93AB12782B11E6B050534CB517066ECE75E210121027F3F2B651363EE85E73A\
5A8C2EF3CB6057732E90E142DCAFAE76206491FB564BFEFFFFFF35A6C071C7533F65EA3DD316CE7\
61B232A4C25BBA960424383A2BD1124A485D40C0000006B483045022100B405764E535D6EE1A563\
F609CDABF7126F542B81D023FD74B85FAE85D9F2997D02200BF7ADB85677ED67C6886FD6A7C87EF\
876ABF4D4701BF8F4D6DCEEE7342E01B20121036FAF1541749605EDF1039FE4997644CC2A943501\
28ADDB5E211AB7B8C56DC36EFEFFFFFF648A19E159DC4B69B2AEB6B244ACA230D2EC01DF2553BCD\
39C7B7BF5C824942D000000006B483045022100AA2AD90B6951689EDF26997B3643CABD1DF3CC8B\
E38F99670EA0B119CDEB58CE02204BBAD3F0822CBAAFD19485684C223E696B780ADBC9D84E496B8\
906E4654589F5012102D70B1D111AD482DB75E482EA9E2A6801BF73840CD161FC30512FA3838B55\
326DFEFFFFFFF4D79F34F19CF7051C9FBED376C285606BEC17E5D40677E4E3E8D0596C2EBF6B010\
000006A473044022011C884577CDC14D9ACE586F11B7537203FBB2FA4F4D6B47D2766F9AF9F693F\
8C02202304BADD47C7E30C18D732D59530D6E8B784C90218833C38FC039EA1DB9CDADA0121027D0\
DF13015F8D3603403DE3A2E567D50039A676C95A2C0010C052FCBFCEB8D40FEFFFFFF86272EFF7D\
C2A919DFF68D38E9074CE69DA94D8D40786C5F14F7CADDC59559A3000000006B4830450221009B1\
989471254E42975CB7D208F4C07F4AC33A5EABD817F692CAC24DA22A9AFFF022013322ACFF8B75F\
5FDA905AAC73DEB32D233D59FCDBA00F80177BA8C03C01E48E0121023E87305896B4CAFD14E5CFE\
9C07E180A350C39884021BBDA92377D43710A24D7FEFFFFFF3C7DCCADA7489633E4BF1EACFB7F92\
342DDE8E464838A81F3A473773A857A2D1050000006A47304402203F2F720E4F44D21004454072A\
DADD81951BCC0304CE5DEEF4051C476A9A84B6202202910047646AECF0F486235DA98EC6D74FCF9\
2ACDD5A75F8BA56F09EF3E3C6B870121021CBD3F41F174F6E7916EA259FC6548B26A2EC4085DE0A\
16FB2963AFB82361223FEFFFFFF089AC3ADD7614DD05525037B7B9FF44790344325A125734F570D\
959EDDEF3F28010000006B483045022100CB8BC26B55E382C1AE0865EEA4453EE43FF0783DACE23\
39B7981843047F58CF40220244778290BFFE344C735B328B617A813B555B15B5577679ECE26872D\
CA526907012102A003D0738384AF301A3E87DA66C7B9B81DD71AAF079401813B96D6111B1C1EE6F\
EFFFFFF3C7B50E5398DB20239292DDD3D4F21BFDC4198903F2252AE61EABDC513716F7501000000\
6A473044022019F4674508674676695493E2DE973A4E82070017E837147860BD146EF3E56838022\
002268B2B3AC3445379587AF7C59F239C892E757E30F23A9AE20DBC1EDF49DC0A0121021D45613E\
9DDF2D09133BD5A3D40B677CC8AEDE1076C5C9E385E49070468F93A5FEFFFFFF3EB749D098A159E\
2CEA3C2160F499B814ECDB05E72C609FFAA59D019AEAF9120000000006A473044022062C1D52D3E\
DCCF5582894E5069EA19B7768F5415D69066EC58A75DA42BECE70F02202600210D739F9277BBDE5\
073DBEB3AB2A44A6DE24704F224E204744B88BD96B4012103D0E37B4CFF40A35431713F2291955E\
B44CC9A736AADF326E735C0946A7963C75FEFFFFFFC3100FAACDA3FEC78F24BC308C8279482C02B\
AFFF87AD2D4E528A6336D1DCAF1000000006A47304402203047E54D06B08D681C577F7AB7A8F6E1\
9A5FAC5B459814A2730AB8B3A4177B17022019742EF36C34AABE74D8D38B8CF00F64F099263654C\
8EB7A978783BF0F08028C012102B41C83F2F156F12C4733632DB53FF0E452ADBD2D5F249F6A6490\
C7E0F5CD253CFEFFFFFFD4AD0174549C964916682C3116BB2DA2478B8B5DA9E2B7F06B557F07679\
25871000000006A473044022033F3F37EB3656B07EA1A120398C81C44A143BA66AE8CEFFC61A082\
30F4C37EE802206906310B2359310442E45D249C60F33C5A45C5E337D274CD0034130DB9D912D00\
1210284AF274E9ADF943873C96D7F8F8310AF9C16124C2290520FED9D139BF6A199BBFEFFFFFF1A\
38119B9999AF67C041E4A19AEC001442A43A5123C0AF74AE29E8A2D0FCA97C000000006A4730440\
2204E627A7F0CAB9E75F2223608C1507FFA6AEAD7B93C97B3221BD672D2671165D902207EFDF525\
222CCF3C15CAAF4152763F2D65CA7C1806E0A8106D8FC607B9E34D5D01210282F88983CBBFC84CA\
5DE44F9EEC94334B07615B2B0456AFDF2D06296E03C6B03FEFFFFFFF07D2BFA513E49EF251AF82D\
E284CE4014DA0F3F57CD39749E7F5BECB790B6D8010000006B4830450221009AA1E811C4C885A67\
6A2B6BEB7971E59EED48C443C103FE45EE49F7E4C95BC30022030CC1A5F91250BB70C8413C00C32\
1FD27B1F09C59895A8FBD8B8BD0E275B61050121020EABA4271B990B101C4A7AF33FB695CC6F768\
D715F5BFC1918CBA1367FEC5D3CFEFFFFFF38F0F09EFC71979D4D9AEC5678EC0412B89DC8BF4E3D\
99C394EE1F67C6E0A930010000006B483045022100915092953C60BE3E73E7242FADB19091A8034\
B519D24032E0B1F4E2BAC2F51A8022051B186238C67D2A797DD8B35A6DD3C88D2C4A06128D181C4\
E088AB9D5EA90862012102FDAEF02FE336F66B87A25BD05B61AC927D514647CAD8238E08C064F84\
F63F67CFEFFFFFF2522A5219F5A69D45D2E6F8268B1AEF85092D71AA181F53AD93E571CBDD541F1\
010000006A47304402202F8CF76D860ECB108142AE6D0239686F2D8C4F6F02547DCAC3DDD5E3684\
A6E07022039DE2A80330AF2F6AC728674CC6A95095080CFCA83380E5EF202B7474A0304A4012102\
AF5FFC28549B4AAB27D3F0B3856A1F5341297F08A868767604D572ED679D614AFEFFFFFF1267FD3\
7BFAECED16A84E42207ADA3AA4EF351CE46E54F4060E00DA9F1B34FE4000000006A473044022077\
E8D9C45E66772DC3BFB6988CF2BF45F6747CFE05CB7BD4536F423F220940AE022067344308898AA\
B85071F81F4240BCEFDA05188C0BA24BDC835E548B98803C40B0121034126F8043AD183226178D0\
9774435423687EA4D082A917714442B5F72E9CFF5AFEFFFFFF90955FA6C79E310969D2707146E77\
3FAE5D4480BCD2EDFC9535E6C516EF68045000000006B483045022100B49C1B0963D24FBA29900B\
C14E9D349CE9BC1A82EA9A83B9AFFEE9712374B907022062B2CDF19AB7D5CB2127A7CC1EBC56C17\
2A112F59A2C3A11918ECDAE5C79889D012103ED670AC554896CBA14B218769A2D943E1BEC19577D\
9F8CD061235C38784A78A6FEFFFFFFB18B79F597148AD8FB05483A5FAD068B456FDA006C68C644E\
179721FBE0A2293000000006A47304402204F099AEF30476E22B227CB6E5B89B2C6E57DD6215894\
83F978EDEAA05F075E8F02207F7A09EA673F4EA58B9BCDDEBEB8CF734B4A5B8AFE7E3D42323AFB2\
A49FFF2860121036E9CDB56A8E77C41084FD5BDD7991DBCDD1AFEC1D1A984F9A150679F8A1DDF94\
FEFFFFFF7B58E17137115F9629E86E6E7FAB9DA79BC9E90A2E23D11B4D0FEF3C958E0C3C1100000\
06B483045022100F0314189AFDEB36256A59527E4AAA3989EEFA890F616D49D47F0667BD01951DF\
0220321433B3AD9243C70EB883E662313380CF71974F09EEDCE0DAAD11B7360AC9A6012102067BC\
B178C3D95D4534F76903DBB5A256D39D87A7D49C0DE2B04ADDBFD280EC3FEFFFFFFE89EA2A89EB5\
5E496666858B5D1EDB4821672C623D18CDAE6C83F44E7966050C010000006B483045022100B1BCE\
E8401009F9C0A3BFC37719B99747E0D4E0211F13D89C40424AF750B25640220108FCC15131F1E52\
EB38A844C1276CF74B06231A44ADE56C812390362597D7830121039BD96C061BD43205AC290A998\
522BD39DED1F1F929EE33712C3F09F2AEC937EDFEFFFFFF1996C8C61779263C95349F460977BF39\
256CC940DDF38993FD3B94F9AB7B77A1010000006B483045022100BE02317185975ACA7356D3CE8\
3CE9272C126C0EF62BD9D6E1706C5122A51A7F2022076E126FB6F7147CC01BD58550E8983984B5D\
0A2ACAF10B74E83D050D11A8BC11012102F363AC532AB1D2E153BADC98977E9680FF820BDB8B906\
3FF1A8169AEF8D736DFFEFFFFFFB1BDF129A2F609D123C6C947CDC0FD2F0A812DAAF8681C2E789E\
A5DC6E29CC370F0000006B4830450221008EAFDA8478A3550B5703C97632DD5E1BDDEF0A2207DC8\
CD6F91EED264EFD163E02206AE94D0F46999C3D7A7ED1628517A9317DC2A0F8BF836857D4D18718\
33659FDD012103C063F768C4ED63C092D51556499223AAF475A49DA0CF5AE501A9F54C8E815545F\
EFFFFFFA4D2186FB83BD8E2A006082E0AFEE94C7D0B2D9BF7D83FA9C955E5FF350DF33100000000\
6A47304402203EB74BE373F4B2A9F1BEF409A2E48FE6B8312EDAAF37A12493533A8E29B37F88022\
0369669F1E5B12B036902CDCDEE70361000816D72B0F678238C11E68DECF4ED2C01210396E0D154\
88B6397BB961F82584D34A820DC90903D7EB52D9F74AA465D8CB79D5FEFFFFFF91CD397F5B4E1E3\
8CEAE2013FC9EE1752911CD0982C2019F1B304370147EABD60C0000006B483045022100B6CFFCEF\
2EBE38F1BA6993811AD4E80B578DC72133138B4A0556784374C2FAB10220658B4F3B2DD84378257\
7F34AFB08A6412036888F663F8CFC13B382EC4516A9E80121032F2288BC8602174C0E1F2B063474\
F6B28B0DBD821C8C37912B0493E6756B199AFEFFFFFF49470346E3014E2CCB1877EC38BD17FEB8C\
2A83CB7B8CDAC0A27368DF4BD9BBB000000006A47304402206EE9FDC7398D8020F1AD55764BD314\
95B03063644177B025CA92EA9C6CA5CB6D0220637A69766E61AC99276AA2BC2108187F63B4514DF\
EDB62EEF665C6EF27B409B601210324B9CE64803D6EF664DFF288F220496A4ADE424952D8FF9B44\
6E002E7C233662FEFFFFFF102B94CF8F3646EA7CCAFFC8E991BF8B264FA6E2AB2A00D48754E796B\
E5C7D62070000006A47304402201A14A5E2F36BB78094C79467F1269C9ADD6BD0802A0BE336B512\
858A5EC07459022057DCBB4287C06C4C78A854228654BB6B20007BA904CB5F9A00369E2F3F4E025\
8012102B0CC1C99217063AC054A333DE6F536D4FEFAD556E6890324B7B2B665AD359D21FEFFFFFF\
9D4ED7AE63AB73A360E5A2D53F18D306094A6CFC46640F815A0E2711114D635A020000006A47304\
4022034411366B25297B9344811E8E7AB41BF75B75F45E00842C8924522090A24D9A702201E04F0\
5656937FCB5072079874E727F219178B81F4170B9BA4DB058B2ADDB407012103C6CFA14A48CC326\
EEE161C5DA420C97305FA2DE32CDFE9FC8D4B3EA45406372BFEFFFFFF03C3879FF84A2AE15F6FD7\
988842D5744D717A4BC1ED02F22A400CBC2C9431A9010000006B483045022100E8FD3071180E14B\
F5107CE1990E564D37194D6843CB2067FA68814B3C1326F6102200B7E5F553FC8CAEA3EED8B6224\
C47245C91A6D88BC642E9DC0E71CF56ABFC651012103EF1985B61A38525C0433F8E2172A35CD981\
D1545FE9BE2602D75D002CE021C9DFEFFFFFF0253F74136B35B848338B012666BD516386BE4A237\
538531833F6ED4274E1812010000006B4830450221009FD9C2BED9EBAD0A17D8281BC8D7BA1BB2D\
9F0C0F243B0AAAED4F6B04AC406D302203B0D4A9E3E7ED8C5378299C94611CDF4950A12DBACAA8C\
A1C973B40C343EAB3401210310D73B46A585B3A4732C4A78423789B37897AAAE16EED6D8D74A6F7\
1F72CD7CAFEFFFFFFCEBA71104DE82B496B8D516F247D9CA5BA689B609D8302B4991AEF941FCC38\
09000000006B483045022100DA8F79302F4D6ECB6AD8A692763241083A71B83017C7739B43109E4\
3420C9D34022020E1153016F76E40420A32389ACEBA3E8589F9DB493D6F20F1BAAEA0EEF9CFA101\
21031FE691794765AD1BDB4527EC2940EF06E17C77B1A9706313E6B5815AF30DD555FEFFFFFF341\
07E00F13C6B8057992D20EBB415E30A9B9052F5D604DCFCF00C9117CA2761010000006B48304502\
21008318D0ACCDE9C0373E52C23FE26A67A2699B0EE3D6FC1DCDFBEA4FA4D6540D9F02201C0A1A4\
E0B980FB8E1180347161E2921FB3954309A6AB42FEDEBDBCEC0953874012102CCA799F51D792B06\
B9DAD1FA1FBF3C42D5F6AB85C3A1E79619E2FEB2488728F3FEFFFFFFD40AF70AE4566B2EF0F739D\
D1CFFCCE36BAA00087C4A26975F9E2826B8B204E5010000006946304302203FD052D8DA0A4AE7B5\
DB6F84E688C7548BDAD6E1092E54AA24BA0ECEFD9F2629021F6CD1A73037EC613014D78FBF6DD07\
12401627B691F151923941B5DB9409076012103292E15B6604856E0B7BC18D0C732D0D4E4B960F7\
039A9F66442CF90E261D9407FEFFFFFF0B8B314281B78C59370532EF365BD748169C1EBB806A517\
E1038EA2A60BE430A010000006A473044022008BEA69E1C6472ACC553D6D81900974EAFC85D2743\
8F5E76041EC64A6E63C7C2022042FFB51DAD50447B73CBF3A9BEE30B0A439E97F819EDB211A3D25\
34293000720012102B0B7A0D7D7D754F9C3ADFB55B44CD7F394322A86FCE31FD1ACF4161891B1FC\
EAFEFFFFFF955A3114E4CC3F0228C7DFCC245BA95CDC6284F9969EAAC53888AD687C9DF04600000\
0006B483045022100E0F3C5F146C29B53B8DD2B27F8E94AA4CC57C96EDA021B2EF0EA67D5E3D7C8\
E002207FFE1DB71726F2232AE77F2BEBE59066C3DB5866B3B9100195AA32813D3A4B760121032BB\
6F78355916E26D1AB84E24BF5B4A1252F4037D430ABADD7E06695937F4FD9FEFFFFFFE44E6F13EB\
064062BE2C16C5606AAA3666A354D20ED146EDB9B5445EF9292BF6010000006A4730440220626AD\
E342244254798911761C45452F74654778DC1D2402A01DA5CCBD0CC092602200E3D71C27D3B1004\
2778FA2914AE9F2F04F8C3E06D08BD09C654013636818EF501210233E1980169F857EC89E115900\
272E5BA4E1424285F0C1A2C29609D52CEEBABAFFEFFFFFFE31B1D7555A9A2F5918DCB466730471C\
360B88D0A80562144EF5BC0850BA51B5010000006A473044022076F9B8B0CE00045B9DC3BD0230B\
52B1527977C6682757E853B0FA979B1B6BC8302201F5D9865A0CAB367198275BB7FABAF4922421B\
87313BE50435C26B5E03E0BF25012102F42C3B76A4587E021B9A3C5377981E19CF4A6A7EF4E479E\
24910ED042D4E6660FEFFFFFF72E26BBC6068F541F68FEE11F6897F90952AD5C7BBD5E1B6F959E5\
A4BF00930D000000006A47304402205D3F0BD18DCE36AFA1330BD9A80C30836688CFE932F94F906\
5EBE29FEB27133702202142FCDD4264083665DF796C0FFC472239DEBF0B20C3F381EBD233536165\
06E90121026426153DC52356D2AA89ECA0CFD81BCDD20136F83057D4EF552B0E86708681B8FEFFF\
FFF72C0AF73C71CB3130A023805CBCE805486FC6CBF2CA3AFBAE14FC968EF725131010000006A47\
304402203DB62737D5A752415614C9F334D3DF7F76FACDC2DE0EA1BAD9EDB5E23E8A15350220322\
0E3A90C18A5800A58B1513F3CAC890159489D78F9EE970A765834AF32305F01210357D1CBDF2EF2\
26C1F13826EBE1AA1F77113EBC6D3A40622D57607B5A1A818A97FEFFFFFF6772AB63ACD75211D59\
26E2A8ADF14619BD3A2AF76E5C1BD9967CF506C8A0E970C0000006A473044022027144E0605C1DF\
13F24DDFFAEB1F9735AB423A6E60BFBBD1480D76FBBE95C53A02200E6DDBED1247D2A23CE390977\
AA64BCBEDE54553DC1D152B0EEEDC5C2C08000A012103F19E48B60E456419C20F425684E4FC0396\
AE95E04277E21F3F6F7485E6A01C91FEFFFFFFEA2FA782ADFC3637D48D9DADAD44E91F76C7F3B6F\
59DB86F18088108BD7068F6050000006A47304402200952D2957ACFDBFE603182C0B90827910B58\
78ED7B00C23D02ECCC7090602B7E0220796F738CF71676C4C7C7112B2AA4352C95E6BAA14B2C3C6\
FF1199CFD8479ED69012102BA8F658A2B335B1591F69A931196B87F4F173B3B325069152612ED97\
9620B86BFEFFFFFFC89B48D9B27D0CD8D756DCCAAEC1A55264AD5DBBA098C1EA6EC08FD3171E61A\
2000000006B483045022100AB3E4E4AC92A6F560951E86F5C1CE8F9492F09EA557C8BC99E194DA7\
88F5E0F402205D19D5E3D812D28BF2AD80B45B05F3415878B3435579922908BF70F9A73E7AD6012\
102BC8F8861BBD02F2DF6BCA9F2DB522B0CA2CA2AE81AE6739E97A391C147FF0A6DFEFFFFFFC234\
207369068A62F0E617069DB798790DFFC8E839E919260021CBFAD8EF69E0010000006B483045022\
100E54504CFF187344128F0CD71245C6C6F9628A8BB1BD618EFA0B3B76540DAA8D80220205E1B4C\
19334CBF6BF38B65D72859AE2860331D9062F316C3B34AB5C18A16E4012102E714EC57B59E3172A\
478BD86391FF129DCECB6D5015D24DCFEF6A88887030400FEFFFFFF21FE07A8D8E3A816E7F2B84B\
7BB3A9BB0F93387BD37534775880530504F8E63F000000006B483045022100C0C16C5BD158C7A9C\
09F47322F1D9ABD7E9D754F08A0E0EE8F438540F10C473802206164897E9A1BB45455A8F21A617D\
6527C444345704DA158DDE4B48DF61D533240121032BB6F78355916E26D1AB84E24BF5B4A1252F4\
037D430ABADD7E06695937F4FD9FEFFFFFF4348EDAB6FA896B8F26DA7B23142CF8845BA14CEA338\
30E7913231EC6A3FC871010000006A47304402207BB1E25D0FBF6B9A02FC309A4553740700A15FD\
E7FE8A824AC85ECD37768070B0220558267C434F97840F99F7F48C5AC1394015C18D8BA5EE79777\
5E5030213B3A310121031C21CFF572E27F5ACB3C72AEADD056B1D385860B16A043D9622BDCA3FC2\
F5D17FEFFFFFF0D03DDA2D954B0CACB030C8347857813B88103A561C62B7E07B303F745A7941008\
0000006A4730440220327A82591B658B2C9EAF5741B007F9A993070E1758C33D1E85B9CF03EB81C\
ABD0220135E7FA19DC2719F6ACDD690AF32056FDE3B5E987711AE7ED075BEBCBF266E7F012103D9\
6E44397FEA57AA45E07FEF4422F538EFA1B45E8F384106505550298C9B53C3FEFFFFFF3B906E988\
30162B35D24E9FCFD80D2C4434D779D0EC158435D25F8F4B751ABA7010000006A473044022002A9\
C4A8F0172EF04C8EEC5C2648497E6759DFDD19C5E3DC3B6AC5B7DE2A7DE202202803B75CA4E0A2D\
68FB44152A3F249D37ACA3891CFC654E3A88C5011E5E827470121021519CBE66EC5C812E65F4AEE\
2F9527A0F931B97530F5504668A1BE1B1D4E2BA1FEFFFFFFD56FE4237BF8418CB091F90C57BE069\
3D2874A65D18851062F20811B0E576DF8010000006A47304402203003716C7422D15E8D6F604B59\
057BCC66907E29C00ADD1F32BF6E8AF51DAF38022016DDE1C3A89139DBBF376322B31992F702CB9\
FDBB4BD9C4D1562BD6D147F4EFB012102FAB023966F4E78CAF721DC5547EB6994088D9712740CB1\
46F7408A6B382BE488FEFFFFFF81CA022F8BDEA9854EEB252B2D9983FC0957558E1DE2BD630FD5B\
11CCFFB391E000000006A4730440220261A6C3628DDA12007C69868F130C29CD6E0220A9882EBC5\
DD2582D2B187FDC5022038F2C5A57F8A3603E58DB13866557D5B0B545B655E5D99031973A17D16C\
BE9890121039CD2654FF4875D5C97E4FE2E4B5592947DAA732F170DA1DDA9842743510A7158FEFF\
FFFFEDC30345616CAA9E18F062C5183F5AC20DF3AF1BC5AB9711288D0649D641B8CA100000006A4\
7304402203FE9564D226B003575C486992B9FA4DBFDC59AA7A7670785FED3BB3E4F0535FC022035\
FFECF414A5836FA9A934F7896F0DEEC108432A1D1F4020EC5929D02FFCD38B0121027F11A82AB92\
5C38F67D100122E97FA5B8AEAE19097E42B3B0C7CE3EC2AFF8637FEFFFFFFF3240E1ECEB9472DF7\
E882840442878D6E6826E34D35563D69D5146E2FC0A02A070000006B483045022100CCC2A7FE025\
4D0B820EB061812F341C77C0F674B1DE5EC51196FBF348A9CDB21022076C7AA9FC10962BC716E97\
CA1005DBF6D11A881C36DC155BFE97EA85C4553283012102B5218D293F5C6FFC152DDDF4E28AD72\
3FE7392E7DA70144A848F09CD9EB4AD5BFEFFFFFF74C30FF0BBDF4FFB72E490F663674726B4C575\
F3CDA4C8E2E59FA3AD8234832E010000006A473044022073225D8142D198D3D82A4D6F0BE06DED1\
43BEB8CDF423312F652D336B8C38A0E02203AB12CDEA911DE4CC2559FE6761A07295E595F200674\
B70B45F0A8BA733CB59C012102A4224C16DA3C5A6F8593EEF6EAFEC8F8138FFF75B2C69CF2E6953\
E96DAF6CE16FEFFFFFF199BA17AD5DCB54E19810D3A9C7868BD826208D5B4948A615939E870611A\
E326010000006A47304402206B1F558B88D1950268AF8778DF7543BC8417FAC64AA3945FB74893D\
20E744EB2022077699A1CC5BBBA3E49A6C95FDB3643F6364B452CB814FC7AE09AA6C510AA6EF801\
21038B38B65CD5DA15901AF12750BDD603E25FD10BDADEDDBC16AB1DD6FD99F1FF2DFEFFFFFFBF4\
D70D6CDBB7B742964A698F6C205D70E65E92F127788641A64D942CBC2AA7D000000006B48304502\
2100D971AF2589180881E79EAA1D6879A72F00E333F8F8CE894BB908305990BD3E760220135B5CC\
A97850560ADA23FEC24CF4877F579263E6D1A5044F4A733A6084653620121022D713EA92ABA6965\
B0E5DE88E2C6C343DC363CAB47D976C1827C12CF085571C0FEFFFFFF78448EF94A2FA95C38260B6\
E831F5F6230D408B3E30F05402139DB00C45FE773010000006A47304402205FAAAEEFF0345ED7B6\
54AFB95B8B7DAE4C81A20EE8B718DFB18EB9C490FE914102201E368219490ECD7E9EBB82C1362F7\
EF79065BE1075D408A893AF82F621AFAD18012102EA1E701E081DA726F02D5FAFE1B0211C220A15\
7A3CD1CB6D63F43AA9E8539022FEFFFFFFF09DB46A3161FE6B4AECEE51E71B8E717A76B34B9327F\
C74087437BF80A7C9AB000000006B483045022100BA2D092A3A1C64BE3898680782A2D9033001E8\
2A89A36513F13E714E3C07AB6C02207AA8F3B2F6BECA78AAE17AC1CB7F0D8B36C9006E4131C6BD2\
1FDC34ED6FC41B2012102DB00A0C7E87645881356F9C016B6E9EFA8CBD7A261327F32A3BD49358C\
759CB4FEFFFFFF222C67C6AC689F2878BD75161E013E01E012B317D2CF08CEF6BCA78C3BAC815A0\
00000006B483045022100FA1612A83C530594B16EF0FB1065CFCEC814C58601EEB4895047775213\
696CEA02206C0BA3528B83E0B75C86E1578133BA53CBE25F75A95276883477AAA52B8089EC01210\
36A58499FFA8B923C0AE701DB3DA4E72D6B277996FA541ECD8F42FC5185939A02FEFFFFFFE86796\
1C1DE43BF9C924B5740BA52363CC8ECD535489C62F8DF5BE792CD8505D010000006A47304402206\
1FD435A07453615EF2A86AB614AEF2757F79AD3DCAC05CFFE3B85CCEC3433C8022043E83D4DBDEA\
94C5FCA7A0453B07175B94827130A328121FB8FA9C08F02CD3D5012102C04CB93CFA54951A85924\
EDDC2AB89ABFB51D7F34E12B0189A76953F1D678C76FEFFFFFF34FBDD720E59AE4123C22559352A\
926045F6620ADAEEFBD1BEE1CCBF10E6F2BF0000000069463043021F3825DB4678E19FF1887231E\
9FEC863C9143B92A2CB9AEA5E7E689084BBA1470220617A7BAB0E94D9EC822BB0A80BE43FD31CF4\
33F34664552E1110B60A6F1226710121034F7CCEC02FCAF611AC0431CBEA0077F768C9116A54633\
18DB8D3D52E0BAA73BCFEFFFFFF46D3255389EF6C66A61721DAF1D4B5E15233DBBE517673FCA56D\
A7EFBAD52E90010000006A47304402204B3BC88227319E092AAC7D56871EC993D7C34197D6CDB3A\
E83AEDEA913EE7AFF02205EDF52BFBF6D319AA5517D979EA7E0C451D5C84FAE342AC07CD67E51FE\
F1F927012102426568656B0FEC1E95698225CADB417EF8C6ED96F95D598C83B6A5C7AE27CC0AFEF\
FFFFF08C04F47F8219BCA1699A5CF7EE3C45E44660E619A8EF241E4863C8B26D8FCBE0A0000006A\
4730440220773033025826E6B76DF1732DB475621D8D5E87B6AA487B74F125903BAEF087AC02203\
F7C7EC764D2F350EC264003FEA8E6CD9094CF6EC229F7299DFBEE2C64E3C26E012103C3290E2BDC\
1FEE11F721B74A4682E557783EC6E52DFB21B95199810FD5FDA854FEFFFFFF4DA72E5EF1BFA92E8\
054676C41A9B417D0C98C4CEA441B5FB82B1A8DA2C2990C010000006B483045022100C81AFB2DAD\
3978E1B4DB4C40AD721A1A3C4498914E6CB79B5B4D38F1A0FDFF7402206E579DE3FEBA49EA646FC\
FCB8A0C86BC18277F29F82171CF0B0DFE042E27BD30012102F4C5C8BCB9B9C605B2EB132EDC747A\
5F3466DA18EEEE652241839A94540130FFFEFFFFFF40111B33A90DDB5073897B0279F1021B1C67A\
1B5ED0B748A978D3539926AFF39000000006A4730440220297B41994075157DA8737ACF3AB0AC9E\
51EAA53D46EB8ECBDBA109D571270166022008DDC79A497FD7AF6511FA44126C4155CC612227516\
738065656B4D2550CCC1B0121034C3BF7ED8A595C85EF64CCA58B2BB32519FEBE826C67455EE0DF\
BBE9FEF2C1C0FEFFFFFF2368EA030710C5C0E3BFA1F32F104B23FC8C8DCEAD61193B49731574A14\
6632E010000006B48304502210094EBC535B6F13D7D54776736CB1E7D418DE8A41672437001B543\
57B7F3DADA9402203754A026B5D54EEA8F2B47DDA5EDC0B75187F264A9C3845A9946791C8008CEF\
D0121034D037DC824C054106E961BFA3121D6B1FB7F0BDAE8B34028B9486A851D366FB1FEFFFFFF\
40899D956BB910B14909EB78476CD60930108866983A65C781F8834A211C2FB9010000006A47304\
40220792240E8B0A3CFEB701A45AEDBE6F1A3126702F510CA4CC63997A3EE4864AAB6022068396E\
4D011DEC2184DB6C140978A7463C947AE6F57F1F9E342C3E7536EB50C8012103B6EA43A5E20BE17\
B869D97DF9CA07AA1A46F57227E4564A056948A56CF745D80FEFFFFFF18A24EB443E84419617C90\
0B8230DBAD0246BEB705795B1736D0B5057F87CBA9000000006B483045022100EAB777C6D4E1871\
B993A7FCDA9885BC808B19DD30583B76494B8694F82343A98022040F7C62A5599D464E913910474\
736ECDBBAAB53DB22D75B1D0C17F54B75621F6012103E068A17A66AA5F1C2C369BF43BF9744B507\
8491794B10D620E11D8F3012B3081FEFFFFFF7DAC5D82246F45F254730DED12CCD7C491FF5FD597\
C4FB66E8058EEEFE3C117A000000006B483045022100FC7DB7493A83710D1414B4CF693F6F21605\
A0FFD148286B8AD2AA9215E87044F022006DACAF1AEC78B9DEC66CA4970149A01FEBF0AD6274001\
05083CAD6A15E3D86C012102D8FA56D157E56B9BFE90229B51E815157B34C2B705A50386F570923\
9E5E1BE14FEFFFFFF855F110832FDA19CB611FF441D8FD1E1C4B972C79DC9E0225F7E4926D0772D\
23000000006A47304402202224191993A563C117265E596B9C47EACEF1A21F53A8641AF8C66C79F\
356E0890220156229AD0B36FB095E01E486F27CA14F51AC20EC7AD53015CBC06A3AD48868580121\
0313598F516D5B798E1DDFE23E502C19F86F44B006C00A0E142CA15A175EC07F78FEFFFFFFE95B1\
43FFACF163ADF0D351E558EAF1FF9F5544A1BB02921CFE2B6C39C171E870700000069463043021F\
20EC4E716C762229D79FD7669FFB43FF87525321A9412A540268D68783E6D002207EB6EA59763CD\
F4F1CCB70D872FC5347330F3DA1184DFFB7D1F249E966FB815A012103D96E44397FEA57AA45E07F\
EF4422F538EFA1B45E8F384106505550298C9B53C3FEFFFFFF0EF5DDFDC0E047A4B5542016CF894\
4FFB4A86E58AE8BA8BCA7A10765983DB095010000006B483045022100AA6A4EAB40C6BF1A17F5B2\
92666BE9DB1B3022C8630C96F7A4E983F4D6ADCEAF02202EF016141FB330138769E0E13F5CEE997\
34E4CCD09A63099470A0527F58F341B0121038CD053D6F80D8BCCC33B6259DF6A752A1AF7677E2F\
89E2C7E4422F98E9240FFBFEFFFFFFD5DBCA93D355214FEC104538CB8DDCFC742BECC9D95106A7C\
ED5D1D7AA666298010000006B483045022100DF35B657041937F9DAD3F4A36463B34018618D1D8F\
E30EDCEE249D5FC6D81E7F02202972EE4CB8126A3714D0CB111A08F2A50806AA97F6F0E831D530B\
AE777C2B2AD012103AB26D2E235D7720B6E7119EF1644E9D4A81A421C0EDAC280ED4450BF041100\
59FEFFFFFF299FDCFAF0F9F65CE2E38657C98E0EC7D8B1FEC270E1168121609723D24D820F0C000\
0006B483045022100D4DA4DE9AC8938A87FDE8157DD3A2023FC0DCF9C3B54302D9778FA89004962\
C602207F6878B4062854821D635AE21D8B7154E4D894E2A0028DB6890784C348FAFBFB0121023C3\
AFBD904215EBC4D2FA87DECE5988178B1407B141FEC1E58BCF623518A6A4CFEFFFFFF6CC958C795\
AD5C9B9F8F87F8E0668D34973C5082042CC2D6E6723FE7C9727B31010000006B483045022100E29\
E0127C4151F850AE3527DF64D7DCAC2CD67B475FD49BEDC73E7EDC2A6AEDF02204158372F34F4E4\
F2B95BEF3067EFD70309526200A9939E4E53BDD324641C8EBC012102BD583AF54E815D5F4828C59\
F77A3DF08872EF52BB05D2C85A964A8D319479732FEFFFFFF63C08BE812C3056EC8E7D6DD9F7AF2\
64C4E1907CF75470500B3F4CCEE3283BDF010000006A47304402201A1636EAF6C917E96763BB9A8\
28097A00D341609F72733522A067F10CBE2AD6A02203B76392C99F1B856F60A94549524CF228209\
9127DD83CD0B581EFBDFC0528546012102D8D78A255850FDE188273BD8CB8C31C0E3554099046BC\
A7AD0EBDA031DD2115BFEFFFFFF4B6D6F1A3B277D3E501A7B3EAA355F90F5AD4E2733ADECF460A4\
ACFC00FA6E2E000000006B483045022100EF48D426B5D41CBA2EA28A9456E098C33170BB76C4EBB\
4F4BE722655ABBDF9850220264698613BF66BD153198156252B77F5D25512175AF011308DD15645\
4DEC3528012102504F6A22626FD6EF37BE47F8B15E8B797354B90890B0F1A7E15C3D54D54A8E06F\
EFFFFFFE723FEE25A03524C4EF2643CC1CF7A87394746AF4EB778CE77798305DAE7F88E11000000\
6B483045022100F67FDAC4C3744AC4129523A93546FD3926770855F43A2E7F054911DADCE909E60\
22069580FBA84263D212D92E4B42C9841F812D65B7F6F2B18E20EEDB51D0547907E012103CB655B\
D0ADAEFF001B8A05B1A30E8C4DF40BC48C621582B71C6E37E7F68A5850FEFFFFFF08590C7088ED9\
984326FEAC4201BD322851D8CCB08F9338919323D6DBF79A20B0A0000006A473044022009646A7D\
C73FA2EB0EC499039B931ABBD6017035CF96884637D754F84BD657F1022050D83D93398B14D560B\
F969EB15335FF2A74A62B5941A24F32984A5945D30C0B01210215F39D45615814FD1B2EE5EFCC08\
E3326C3E729A2530EF9100431BC2932DA855FEFFFFFFAD48298619A167F5B9F893247B9A45E5E27\
05EB5DBB325102F537D9F9BFDC863010000006A47304402205E4AE13D3A522643CE7229D83C3595\
2EA6500D4F89C08C52E2A2EC4A681C673C02201B74F018513FCC1D0C4904C772AE4F8E61C0A348C\
8D46345441ADC2926A216CC01210295EF99AE7FF84B87994D11ED351596BFE98DE0FBD985C28B6C\
E5AFB3920AAEDFFEFFFFFFC8DE5A9ED1ED26B9C46574B449EEE382CF81F37AE715774D460F87898\
E49B5AE010000006B483045022100E2A7C096A809859094F1E52D52EE5EFF1FEBB5FF779432B029\
774D67EDBC0CAC022010C0A2E600D76821D39E803431CE0046DB9FB3640E755A213B5F3EBE4F057\
41B0121021CBD3F41F174F6E7916EA259FC6548B26A2EC4085DE0A16FB2963AFB82361223FEFFFF\
FFD24B010C33AF63B35CF88B48E1124A8C0994B84AD6EEAF766C85C12D0782B6F2000000006B483\
045022100D381AB24CDC16F8B0F13670EB59F6C3CC0223B21B52FAB8446B65C38B66FA5EA022068\
69261F934CC3AE1B1940D244C9EBD307A13F3F8400FE575530B99502473B910121031FE69179476\
5AD1BDB4527EC2940EF06E17C77B1A9706313E6B5815AF30DD555FEFFFFFFC0713D34BC0C04D797\
E27FEBBC3ABA3D9283FAB2826860E6DF9CE62A619B8E4D000000006A473044022015DCBD892DF26\
FF94BF48C6DEDD17F55042C9F58B16918015E6A2584968BEACF02206C80F42A03FD18A42AE8BF68\
7785FDD0CDA6C1D09EE33938F9E25636719F2A1701210351B37463264A5FE2D08FC3D72A1DD8ADF\
315B504AF136613E5481D6BE99A58B0FEFFFFFF0CE1F246002360686FEEC293A6E006C0A8C71647\
D5A90ACBC6B87FE7E44F6FFC000000006A4730440220399F43E79EBE9EB0AE37CA73A84DD8CD47A\
6E70B34C68CA3B2330A7BE3F6B8730220604338BB9E0BA703E842DD304E2D5BADE203E9DE4AC002\
E25210366AD03A85E4012103F224BC7E5CE767A7EE5CCE358914D33F8BDDCC855402B1A7EE90DAE\
862C52CB5FEFFFFFF6132729836EEDCC9CA93B59052A522DC89D52847243102110DF956D71DF332\
8B000000006A47304402205874B188FB5763147D8FBC043535E7E69F60C6C22DE13222337EFF6A1\
726374B02207308ED9E0192F9728A3AB5C9EEF92BF1A5355C461B582261C5E1C34498BB1B3F0121\
02F3AB6F4FF4619D544E929BFF8E37BFBDF0241751536033192B8C37637981ECC1FEFFFFFF5F754\
240237B02961131068B3F24B3272F6D96AE577EF3A24DB93A51DEF2C63D000000006B4830450221\
00FC7CF2BE1B0C9D2C20D352AAFF0616BA3AFD141D65732BECF0571A8F992405090220399F571BD\
254C4C40831B1C7A2D41B7BA30FFCDEA4F3BFFBB3E641F10CC8A4530121039F671637DB54EEE99C\
EE0CD31A873832D32B5A9429636836588F07D06E83711AFEFFFFFF0537D0933C7957D9AE6F94822\
438D316A0E417D7989623576179FD43262A6ECD000000006A47304402205A35F8C372DCDB787075\
831B004F0815F8ED2FE0F88E73C377D6DF0CED415B6802205316EF3A300B6DFC588DAAD6419A586\
310A9A23CD44E3B5BFF453FA94F67B4350121034A1F1230D93E803B5FDC5B2FA2B665FB442B5BF1\
516FAB9BC599E605A429FD32FEFFFFFFA7C43D4E358BF7F3060B7E2A921065A4A936ACEB2EB1129\
8B5C8DF69755B60F3010000006A47304402205CCE79C07E55512B5E0D949148DA7140A34BB1A233\
3433A1387E8BB420E0FBA202207FB267E8E230370C1F849B1C1AB2345DE8760C576E5F29E46C399\
D8D933036C2012102D3AE61AF42F74711248BF9C8C129E4972AF3487F89A7DD819889947890F2A1\
3BFEFFFFFF25B45EAEB108C6008ABB5D16517509D11D578B138EFCC21856011EE62B4226AB01000\
0006A473044022074FB6C3146CE825FD1CF9B851A441832B0F6C9EADD4649B593B22FB1636B4F70\
0220116ED9ED873426512434E8BFDEEFFF9D5975C216095388753C74754963C7E7E2012102378F4\
1AB746455674C94968A2728BE1B43AD3703109BD707381C8767A0EAC546FEFFFFFFAA28BB1548B9\
D0AAC57B58729F56A80D37B3D094B10EBCE117ACF232A7F96880010000006A47304402206896300\
734E95C5362E0FA5AF2003D01E8F4978E2AA1C979B45052DBA94BB8B402203A15DFB4D89CD35BFA\
A12AC71C4F52340BD699465CC7BD449D2D09E0158111BC01210202F6E8F327C0983CFEB2B555A7E\
F324AE2118514921D0086A872EE792C030AA3FEFFFFFF0642D7DAC9EA17B8EE9B528A67144516A0\
A7846D2D3712117E537B40243954CA000000006A47304402203267A52EAAF6D71BBF589AF810802\
2655E362D48C7EB4380AD3ECE17739D371F02205FF6EC5674B38C6CBD3782EEDEA305E831F3D3C1\
A57D8FB685545DCFD8EDFA48012102C3BAF25029F3D9C9E796FB69F2B59B5F95FB33FB4A84BA531\
3C642A6B7F9A588FEFFFFFF4E23A2514960286F7DEB531789FA3D4AA5D16BD13F96C8A17CC9284D\
5A0F7AFF0D0000006B483045022100CCFBDA35084EA51F50FF4C7189934294AC07602A7A4FF486E\
B63C8B70493CACA02204C79E69BE05E7200DFAB8FED03AE1FA42866389058E72C8753B9416911A2\
2082012103AA183FD3B5E50F7BE45BDEFB816C78F9A8D71E154E97C2391D43C751E86124EDFEFFF\
FFFDADA04CD364CC218C722A38B470614489885504C2DEFC65DC90381B6F96BA721050000006A47\
30440220534A5C51CCCFADC35271EC4C0F8607812F46EC861B8D7618EBFC27A3ECA2E72202202C8\
39E9C8813D810E95517C8344F5D6AF90448B6815E4E5A7F566C2A101ABFF901210343602C3682DC\
973CB2A89A52574D23C345CB86825DBF19947D3F2CD57CE8F2CDFEFFFFFF053D97655F892B2F287\
5DEE6090CC032F0DE02216B67A017B1660A4819DAE208000000006B483045022100C6B7A4A64A27\
01085397FF445A8761620477B174C547F02E72DDE3BC0DD09EE702200A8A917B4D2D9F9E8C131CD\
2B998A2A935E8CBC1F90ADD3E8E16EABD09F4F19C0121027268BEEE07DC0B9E4D37987E9239C2AE\
5DBA71EDFBC482CB44766FDE05D1DC75FEFFFFFF3131D93043792F4FA43AA207D48C167CE720459\
858951926F760AF5DFC553D970A0000006B483045022100BB1EA12B28EE72B2606FE3E5C48F2579\
A25474E0D1F57FAC106B036A7FC0510B022003BD6D4F260B5813229288F74FD66B82E24BDBDDA6B\
2FCC291654C42D79DDE3A01210303D085D83072E910642247DEE1DB57D0B99B033376BA579D16E8\
A2544177531BFEFFFFFF65DE0E46F7887D6492F79F22510114B432EC0E120127F2A91D8BE7E9A2D\
46885010000006B483045022100CBD0DFE4989A24FE412D105BAA326C7DCF6C71A4322D7357B81E\
453C09D447DD02201B7BABE287572DEB85E0CDEED79B057B54C4E67B667A306B13C7DA95134ACF6\
F01210389A71C0DB9ECFFC4E83822C44169C3E0B4B9FDAA55D978E8FA6F1AF2FE9FAE03FEFFFFFF\
D53452200C0422452D46D09627CD2116115D86647B599FAD484541B8B5286C96070000006B48304\
5022100ED36DD994DF843DE25A1D9313D030FFCFC306EB197BBFA4968C6AAF356948C3102201C2D\
1227E779FE32B49613C3CE558721F68EE673C2B81856CA820120D1A18A44012102AAB9D1FF96C48\
10611695F9838847C3AC553D5757D112A86F8F4AA1418050EEBFEFFFFFFE7822F67E6E589C2188F\
0BBA43F6AAC3BAAA8099E6ADDFD03BE54DFC0B977388000000006B483045022100A73C4F587F1D6\
103430798ABF970EA78AC30A86CCAFA52E0ADEFF11DCFC584C702205C72E0441EC9E1D40F406BCB\
F3F1009278D4CA94CD54AD606E52F602361F875F01210383015D273D1FF9A8C2FB2F6135ED5B5CC\
5F193D2F2EFFAD335088A3FC36D61EFFEFFFFFF1331975AF0FFD2FBF2981352536662E7C2ABF80D\
A971F4B7C6F2FD8D3161A399010000006A473044022027FB20AC01F317F6ED6FA460041E8358C0E\
98CF013211F70B2D16360FB40B0E20220555763E942ABB299E41A087155A9B812F6BFFA6AF87F76\
1454C8426EABA5509E0121025DE9509348FAF70521AAF971E1A8E7D943D3EF9B7E1B29A18851A6D\
787EAFDCCFEFFFFFFA75B6D97D59DD8044612FBA76E2792C90EC86EB6AD3157D5FE0B99E7A42EC9\
B7000000006B483045022100C9EFDEBE3645344FCB50166FAFF7A8526396295D40EC5184617597D\
8625814F0022004D16A6D140E3CAC15CD308CAE166FE2B11C31355CB8D4B453AE4C9AF89780E501\
21037EDB4238FFFBB08ABC650221361376DC4ED936989893560433AF3A67BD5633C2FEFFFFFF916\
6931A515FF52876B4E55D70C977F757641833EC63CCE6E05356369EE907D1010000006A47304402\
2029D132E99D7D6F5C15C26DADC8603ABA75CA0F4CF1172977CCC2FD00496B393602203B1089AF9\
679962CA2BEA8986D13CECCFE33464345FF3FFAA0D7B3E1F2248786012103A1F0609D6AA3828D17\
E68A0FA2A7368BDC6171D8402E330A95347F15EF5494C7FEFFFFFF7F28F37888EC2C8982F7D96A4\
84CE9A2CD1EBA9D3C4FC7A8936065C0EA95FC9F000000006B483045022100C394FC59D19408A204\
2C53EA24644C04969D6384FD8D468E04D6BBC4B9356A060220270CAD61132AD6B00F7CB0BF1ABBB\
AD6CD61FB6BA85C5CA7FD89FB742E06EEF301210290278CEB8EC2DB6E58DB862F697CBDC03DE833\
312C4141D471F29CB7555F3531FEFFFFFFF4320B4D3EE2518DF6342094BE83DD426BD47E319AE6E\
E56CDC7969A5C73B30E000000006B483045022100CA08DB70D077857E6CDDBCAD0306C3D448F645\
F2A275EEC6DDB600A3A19561960220393A80D104DBE603FE1A61ABD517EAA5D534D43EAE7AD7AA5\
1FC8C2BBAEE089201210247D8352FDDB0A83473354B85092C7347B352880CF66893735BE88E6398\
43F4D0FEFFFFFFB8F8AFED8DA0E931F12AAD8175C03C8C2D114689ECA04BD5B6F007C464A7E95C0\
10000006B4830450221009B6F62C5DAA0E20341FD43F5E7632FF471287C43C5CE949689B22EA510\
94B9CC02201625A37A98CA8963B1497DBB0ECFCE698159D0E66FB82C7B2E66BBE4CA25B40701210\
32DE3B95669361FD5FC44523B0EAF936A424815771556A8804898FE52A822F032FEFFFFFFB4AC09\
7693EF6C9AA7997631CE393A9D56CF5E2815A5DE101909003868AFC17F000000006A47304402201\
D163153B81F3E842D8C43626D7A3F645F13F6699B5CF1DB2BD3D113F9DE62EA0220777376D62494\
937A27EE7E006EFF9194B518911BE3BA2101BC166FF74F7A243B012102BB2F111EF550BCBF8D80F\
F9C499B8AFFC3D9529647A95AA727DA53972CC87351FEFFFFFFBE2FFB59D293BE43875A71B12D6F\
40B70E2EA00A74BC679D90DE3063953BAA2F0B0000006B483045022100F1D79F9E05C74A30348AA\
5FCF4DC8263B65668591BD6F25556CD78614BFCA506022059F2C8C4C724E914DF50E8097D4E0C6F\
06ABA4210EDAD16FCCCEFC6529D003E201210215F39D45615814FD1B2EE5EFCC08E3326C3E729A2\
530EF9100431BC2932DA855FEFFFFFF461FCBA1FCA6DF6F2561ED332FEB3C912556FB56369A8CD5\
75A0D8EDCD69DF6C010000006B483045022100BA32BAA56B70DAA3ECA5AD68377343568EEF5E0D8\
5D69964C6FD90090214060902200240646BB9FEFB004DE32CF219300DA95EF7DB3A0A70558ABCB7\
97CE2AD4AF4601210282B8423469AE69EE5FF41D6532AC12FECF4E41A316DD4898906B4E68ED2DB\
743FEFFFFFF84FCE76F5E531188483BE0DE2CC4C2C1EC8A35FFBDE159F9286FEEB9207A67400000\
00006B4830450221008220517043B751B8EA94A4B99F4D5FB3AEDABB9E973A7CE3C7B7470B092FB\
23B02205FCE4140BEEA50582CDB11AE8FAC8BAEE4512D2A698365D872F3F599B8A8F34401210242\
F491406ECF70FBFAC2D2CFAC6031C2C54551AEEF50B331F49F6483A2B16154FEFFFFFF851689BD7\
594155615FF88695C6B3A71F9115DFF22B49ECA81D4025EDE762BF2010000006B483045022100E7\
81D339C5698D95C02F48F4B46FC6F3B87BD94D7D8B6D4B7A115799CB97B816022000E9E33E8DF63\
4FC4C8B95ADEDA59C7F427C0333670EEBFFC4EEBAFE3475B12D012103BAAE9024A4A0ECC4DCEC67\
F71421B1B33FF85E3D18CFB0F773326D7349D26744FEFFFFFF440CFAB1EB73D0CBF2819A3CBBF92\
600F54AC6D97E76DBCB9D7A914FC686FF07010000006B483045022100F3D1AF14F435556F5ED2F8\
9CD033D93A97A4D7996E2866ABD72A9B22FF1312E502206A4771A2C7EFEBACD90497C1B151F9B32\
AECE64320CA1DB181F4D3CBA065087A012102BA46AF8949946E00945D6291CF1420E135E49372BE\
3E201D6F1E9865919B0F76FEFFFFFF34A1F3E3506A0EE8B0E6A1F34C9B678CB6A31B92B00B531A9\
2A29EBDBC250E3F000000006A47304402206D48BDB82CFFA8FB260CB4D8986683A1D0E5EACA6555\
DD3231C011D4AC9884A102203A6A88FF0BB3C48799CD1DDBC75E76433E5ACC4CA329A26A61BB3DA\
5967E7DF3012102A543E085CF8A9EED5ED92287CE75143B1B1FBDE0B1F2CB6AF214827898EEEEA9\
FEFFFFFF0CF8D43856674426D1757DBF82BACE5C43C70BD1A8F05578818EAF594040CF3E0000000\
06A47304402206593D5AE2D823F95BDF98E863BC6AF39976F741098B80FFC2075A956E3F18E7E02\
206C532312B1B330BCAFC48036F5C558AABFC8ADB0998C7DAF1980DF6C13855A75012103A315291\
45ECF771D4DDFE5908A08BEBE8D3056E42E606D033B2BA2112E504214FEFFFFFF0663EFB164AA30\
400472D30959AF50978E3E132098ACEAFC7DF43DF7DA9E54F0030000006A4730440220028279569\
68D4AB0E1FE4115F1382A582A6B7CFC705F5DFA775C8D4F587C5C62022012BCFF80DD9B00A624F8\
415F528BEE771105EF1BF91ADC4D663627D8622D36580121026E5E8ECD72BAAC6C226B17EF1B882\
2F64A2A58F0F02328EC9A8AD9EBE2CB2CB8FEFFFFFF8FBC8C0DF31049EE080E3A7DB7A31030A653\
AB8DCD6B381DB6214AE5C192FB6A010000006A473044022013F58AAB964B23463AED64A12B4355C\
62BB996249CAEE70DB43AD17C230137490220278ADB89BF42248949BA3305AB833AA6CD32D21FE3\
7D253BC180BE213E96DA10012103AAFE6E0B6CCD5CD4FADBB019F6F22DDBC457A4CCF3F6C4446ED\
BF9A9AA7BA5E9FEFFFFFFD60EFAB79B8C2EE144AEA6059A62EF7D39F083B3F3DA2121EA1442262A\
226F94000000006A473044022035F041A5490C788B61367CF4F5A9FBE88A3B043742F87EA55AB68\
1C51931626202204BB394AEE6DB8099C5C65C86055A7F09CF81F739EA2360E4B2EFE0574F837837\
012103C68C4377A3B9B74C4DEED0210F442DD1391BED9FEA6239D890BD28F9B1240FF3FEFFFFFF0\
E0B5EA8E1C18C7CCF83E10B47BB40B5C5ED4026E23E482E5D6704C160ACC805010000006A473044\
02203F3A4356F1AF5E6FA11DC2791CAF65C86BB59375C020B84E61FF1B38CC3CC210022022C6396\
F543A810FC149D05E99184C697010CBAFF2A7AE7FFDBFDC61266E8E48012102C6EBFF959FC4EAB6\
F2B1B8B86E3A660288FB0ED0DEA3C0704EB9EF1E8B180D12FEFFFFFFCC8059F50A518C2D297D9CF\
EE4068AF8076BDFD48D392D8F1C5406FA3BA858AF000000006B4830450221008C19301D4CC9FB56\
2C1F1B4A6CB88C5BBDBC4373F4B65A40635BE933125E06EF02202AEF99D57089D9E941296FCEE5B\
DF5062E066B1779E2B61B0A0CF55F714F8CA30121034EDC1C80ED87D515B024DF26139458537404\
166767DECDAB56E3D607672A6302FEFFFFFF5936FAE11F51CE2E0363A39A92D96B7BD423CF8577D\
C6C21D645BC76DE571E98010000006A47304402202290218737F36902A5AB11BCDA0976CCD81B36\
0396F63D07DCB2F8A27D153E7802206CA0A1CE6564B6C30798C349A684C5BE88ED3F9B107503FD6\
F23F78084CA056C012102CCF9ACC63BA9055A3300DC4C17A18DB5504310110CD00BFDF73714F47A\
4555B0FEFFFFFF4A0B8D854729969CA7A6082428DEB2A9D1DD0E620AE2E42E82529FBB4521F01D0\
00000006B483045022100C6E0EF718650ED36089135321E61D71B94922DE1F6176EA9BF72D264C6\
B306F9022077339F137FB06FC36A5F0B99D4A5F66A52A73827684E5368DCE6EE2D8D5D109301210\
2E101D113C34AADEFC27AA5494B57FC83C3258B5D92FAF99E312B0F0D22D5EAC1FEFFFFFF82A029\
40257BEF9C9B1D291D9BDD799B92AE9FF842918A0486B74065F821BA97000000006A47304402200\
A64999A6D41FEB5426D985C65B976C556F1182E5987F7AF312DE8E620043B31022054E9826EF07D\
BCB44430C1597C1F70F7344C53E9F4D3CA7D040919833D85E3F5012103B5074CFA24E3D33702081\
6B44F318E7275C62D0F6481C9197392734B66F51C60FEFFFFFF1E8F4EA594699051D8ABDF3F588E\
8BF70867C2CADC33D87D36DEE980E5CDF82C010000006B483045022100C1A38911A0451A720FF3A\
F9BC0540B11346F0E9B6B549D656017B780776F558202202DC6FC430799E9C64C2CA2FC06225F57\
79EAA84C26417A6597F757C1BA6661B3012102F500328B7D6465BC4A83520050A435F588D1DBF32\
CE5DF385BA19C30F91A2FE3FEFFFFFF3585179883BD0B28EEEB320A0C1F15FE928FF8FB989980B6\
CE3FCABBD294AF7F010000006B483045022100F81620B1AD484E1ACFCF96364851BC51E5AEB0B35\
410E4B513C86DDE7435369E02207CF85E134596F31660A149D7D30F22CC82993D28F8F0A96611F1\
81633FCF6B4A0121022C7036F5CDDABD725DE49394841A492623D8B5B832B0E57C108C3EA3F683A\
508FEFFFFFF2E96BD780CCB1A6CE52BADE8EC0D0E99B9876CB9C891828A6FEFA9022C0777520900\
00006A47304402203CA06B1867F3E51F86A977D3144B7A03FE18F7098013DBFC36C8243DD6B7AA4\
E02206BC2E714CF239CBFE2190C7120DCF4D52DAE4180406E182A6B0B116D06825EDE012103A726\
B9A04603B834F906AD84DC0B32CB064772FD8B33A1F0C08D8411AB9E164EFEFFFFFF89F5CF12D19\
7D28DD8366B8CC0F57F41EC29E1311B3357027582780526C335FD010000006B483045022100A9E4\
EE1140B7E129B16BE97231B1F5D168EB5355DF2976DB6747D0C00C47276B022007CD79180025A52\
5B2FBAE060C3889719277D5053C401051D48157C094837DAC0121022B568821C5CDC40D313CA10F\
621EE1FDE7015AFB1DD9E97848D329D6FA7C4FEFFEFFFFFF8EA48A3F8E942B7BE4E64230F966E30\
6F525FF5ED0A705BBBD7142F6CE861051010000006B483045022100D73A7CD699DEB2F86C482741\
B035038E74C6528DCE881045F1F75D6476529E1D02201CAA85448CB13FD76056B19C81158E47844\
5D1099AD64437684C3BA81F6C2452012103E42C4A9088CCC39F2A079132B27F030A5B80E94D6137\
150AD1E68505E013FCACFEFFFFFFEFFDD7F6CC9D9008B224A5846E760C4E32AF087C012BBACECA9\
D44901F90443D010000006A47304402202FF71206E16BF9F049DB618306F1FA4BD01E6D8EB02E63\
21EF871FBCC6B0E55E02206F4DF579B33716D76DD8D7A23FCC8424D08C1EC0C30E6A8C991FA17A0\
E5BC928012102AD1B571745F6E19E3BD1FC8F6220F326045DEAFE74B96DFADD3E0DC53CE7F63BFE\
FFFFFF4D11A13BC50459DE9DFF41031E8B92B1044F1C230D9FB8F14E4AE16DFA9BACCC000000006\
A47304402201BF652D3BAE07BC7C2FFA8EFFDE383F6C76096DA7EFE55125122BD2069D47A030220\
490A25F953550A329EA2ECB2ADCA956B3CF5F4B5374F950A0A2040738FC9DE91012103D9DE5C3C8\
68F5B0582B69752BD16A69F20696CB42FF7FB906D09AEDADB8833B5FEFFFFFF717DC95E57CD59CC\
FFE1C08F476356F74FD9CE7A2AB951DB24FE06528F11003C000000006A47304402204E934F31252\
0404BB61CF2F60E3839927DED92A985768C0A3B122EDF9E7DFDC702207589C55D314AF0E250FC51\
566CDEBBB458B15F94415891998DAC90BF0EA92E55012102CCC0653105D8818079F3730009A02C5\
1F0408BF9B3D96D5CF663C7946351D1E6FEFFFFFF73CE0CF7AC453BD66772B9D736945B829BE1EE\
091EE8638DCFD41BBE0012DC15010000006A473044022077DCB5D3AAAD2ED276920250349FD4038\
69510F7A091F647A14298795B5B0CFF022079C808BCBFD3A01241C1DA37177DD4688D6717514554\
BC91512E47A19A4A82260121034EDC1C80ED87D515B024DF26139458537404166767DECDAB56E3D\
607672A6302FEFFFFFFEDC3FFDA87252159302D2D95982F40562EA2B01A7D36E5B12307C2F2B3CB\
BCAB000000006A47304402204798788AA88FDF52AA78713AB30487812C7AC0615A013B86D9C4841\
742E057A502204E2B87F2360E3AEF723F8A9700C2FBEB1833F51D4AC5AB8486ECCB1B5A6817E001\
21039B61A54DA529EE151787295877BD0652718ADF41FFED7C38CC9B899B19F9B1B0FEFFFFFFAC3\
B243C92C20EBCB318F391DE9D4CD78C4768CAB3DCCB7AB83A0E53E991CAA2010000006A47304402\
201E67A8D031EA1599349B6B68DCCD3EB38C66EC1F54C1EF4996503046F71DE77202205D14C9A2E\
E6988BABD6A6C079F118E81D9E45A36BC7E5986CDAC8BF8741FDA0E012102B8F1FD3E98CD845869\
EA50659D79039EF9B1D6E3D5E40C2FB79C5182A4824AFFFEFFFFFF0D55D1A8AAFCD8E3F26668ECE\
B9B644257F9CDDDB821D6282950264C76A62171000000006B483045022100D0229D62C0B0807050\
AC483B6FA17D6689D883A61EC88074C973584BEAD9D58B02201E3E6AFEFC78F946CEA01EF040A40\
8AE71D13C12D04DFC0816AA2B5B307EFF4A0121029C859F0E86DA537D441D5CC5699BE6447B7F15\
445E05F444773C9132278D069BFEFFFFFFFB77D5B68863A59BAC0889AEB2341D73F4573E1DAA952\
9FFCBD21EE022F63F04000000006B483045022100ACDD3509126112D3A00E0CF282B7C0CFC8A0B6\
5604156A9232F2C1F3E167AB5902203AF24889FDD4E819306829C85440129EA9A23C221EB67F3A5\
FB2587A32E2C823012102C897C352C6227ED26A8BA1610073816FC1B13E6A8F44D7960D7476E4A0\
14D354FEFFFFFF225EF9F60FA1DA5B7B4AEEF534810FBE4DA2684F4C3F9DBC6EA6B6E602D92B230\
10000006A473044022047DEFBD1D60287F6B2963DCEA846374632E9ECD26E302734A29E0B6E97F5\
052502205C8D51884FE6B9E6F82E44875892E6868D2824C686EBEAAE4CB380C11CC808930121037\
3BD9CAA507DC116F0FC6265419E2418DDA070009117EC982AD007EE2FD66FA4FEFFFFFFC1B2BFAA\
4CFF7D9DDD9C3AE5DC40772FA44F8A839EA9644496F02077518B6DE8060000006B4830450221008\
A5D7DCAC8E6593AB453F950016A1D230277ED13088D4E762CB6F95C3C87E23202201FA7FB3EF0FB\
33EBB693A598FB2DCDE89F132AF10C03269EBA51164F4019D99501210265343F615FC0C5A6A6969\
8043237C5B2A86FEB903577D7A93C770A14AE6C7469FEFFFFFF9859CD5A05797CAF96FE6D5EA627\
516945C2EF7897ACAA49A7847025039138A4010000006B483045022100BAE683C01611C521019B6\
009FAC65B5F73D2113E0ED930C5549E693B3268C832022047873C6539D351FD1D491D62589B0CF7\
A1EC302551467BE4CE2800C79BF7DC3B012103F66CFD25A61F25EAF556F8EB2DF5B36993CE7C270\
DCC58DEC5850204C19BB818FEFFFFFF6EED2115615ACA4EDF959AEEA8CA00085032E9970C6D49D7\
8A53414630E667B3090000006A47304402201B86A236C492763FC5D8F99A5FC5070BEC72EC5639F\
72DE9E6E8F8A16E347BB6022057D661210109260C8BBA5AD40C91E5580B822377D80E9AFD916CBA\
435B26AE8C0121024BB8CD49ACBBBD5A467E2A23D97CBCCD999D2FDFA43DFA0905A0866952743F6\
3FEFFFFFF6728B65822E60E4830B84423A61D5DC608FE1DAC6C4D5AB56992ECA7A5B2D8CD000000\
006B483045022100BED7CABE98C66FA9A3F76F1FBB2B1AD89013AE2E5B2A91EA54DD684AA9C31E8\
B022057552C4A50571A4BFB9E5845B4E1A6432B394464C4A4F2DCF7C906928BBEEC460121039E0F\
3684CE23D0841D6FBF567073F4CC9BD4EC99526C63FD0A2F92EE6D64DAEBFEFFFFFFB464FA9E2BC\
BFD307F4CD3D45372486EBB65CA32245F438E9BC46A267553A397010000006A4730440220492B07\
57607693FD2D95DA2491C524C9D2243A4B014C38F43AE605F8E9EDF88E022009B4D28A874EE8D77\
4D04915C29ECB097DD332B00CC6B1E8B020F10822C4239701210280B7E1F23D458B771AF500F49B\
BE24074B507274CBE65A2A026695F043D7BDBEFEFFFFFF98B2EC15467EF58A0A8790433C987627A\
5C6DF06A1059834467F4595A57ECF6A000000006B483045022100EEAA17D816C661D5CF1C1D38C6\
6DA10F42D015A60B049D841802ACD39E08B0340220068806F92E85D40B417C21C1092E1D9077604\
BFCE72C95B99A837FE175ECD69F012103A8C23B6C54011B27B8BCDF91E3ADBA8E5E7FE17FCD099F\
3132221C9B3D33B42EFEFFFFFF6C85B53C8007D23B25D2D216897AE45B5601486AFAB86C7171D73\
BB577FFE007010000006B483045022100804588A3313DC9600687ACE26A6447280A1408DF52BBF9\
A70F405926E928C88102203F61DD8E0E3A0D47E1AF57DA96FAFB9BF59736F51AB236C8609841B3A\
6B8CA8C0121033082C9B8F84CB1D3A81520D9EE50CE4C9EC4468265D4732D1CFCD4ECF7217938FE\
FFFFFF8DDBF5FA206F66BEF1CE54CBA250C153243930B61C15BEA92C36CD6E7B6102EE000000006\
B483045022100F1A2F86A0881763CF8B3125A15FFC18EE7AE9BEA6C0478E53B40E9D0ACC5F08902\
2013DAC1CD287E99496AEFD45F8BAD6CACD23B81548D8108A9C3D368D72C51A495012103F88A54B\
493C8516A4FF1F479CD84B71E03DD4DE34FB381FF7FCE8186B1494EAFFEFFFFFF0B88F3871F734F\
EAFC50583CD1C68C091603C6574278D45B94558C589A608351000000006A47304402202E12B3830\
AA4CEF8E614A59102E7FFAFA317C3FDE0D844B4E061C896BDC4C865022079F44A9EB076EDA4317B\
BFBF3EF3EEDDB7075C9F70386976B61C9DD8AFFE9C42012102297A1B28619882508CE2917B74ACB\
A0EE7BE8DD4085C805B5A0E96B0EA42974AFEFFFFFF1A72990E956B659DEE5B57C112228541497C\
8F2F35D58E0D98446E51A61B6D83010000006B483045022100F56C97C8255033FFDF3EC80914D4D\
4045CD1E6480ECCD1E8136C863CCD8A10AE02201B851E04556A8288BCF72C00A31486CC198523D0\
47C61827A5900FE89C3D8D59012103615CF2FB91FFDBFBD7266A2AE35CC0BF41F48BE33BE6D826A\
C59D7B80A25887DFEFFFFFF54E1B1222B080AEA4971B980FDA7FF31224820F9828A7527DC8FB82E\
E9A34E0A000000006A47304402200F820740F5B4D9E6AFA12A4105280DB2A25EB724279BA25747C\
548AED63BCD240220571CDE675F56ED2330A7C0AC0DE0D6A117E7EF43F75C59A5DE7F44932762A6\
13012103DB5C9513D0B1C1CAAFAAA24B952493F5844BF62FBBD49E3B5FFA0C420BE2C1CCFEFFFFF\
FA89697F416DA69629AC60F88C9A37DF374884557D35183940EA70813C8ABDCB5010000006B4830\
45022100D1FCF9F4834B8D02FD1046646F09E10687BEE12E951766C4A386F2828696D94E02203DA\
DE87101CC174C41796A39B82B73854CA7D46751200D4AD7077080465F6A510121021ED9BA96FD88\
C0DC4C61A8651954DDBFC3D10CF36A10E1A4F2243D8CD7C57B74FEFFFFFF22F9A098117C2913E23\
68BB8EC2868CB9E1838B02A2AD1BACFF8A3B9C87DEA57000000006B483045022100ED7C4D154BB8\
FC337A8312E95F5DBDB2A873EB0287270CE0CC0E4578ADAAD36B0220391139990BCF49829BDBDDD\
E22A639A5DD0223FBB9743AE30288EEC23512538F012103B06A72D1BC40608E383E66E2B6621307\
63796867F0C9ADA3EE4E4F15F95875C6FEFFFFFFDE842C5306F5B894132A7400F4B12022A47EB1E\
10251E4E59863A93178BFE694010000006B483045022100FD438224F6249A4C45A7DE02F82FF7B2\
09D949F89D31459F6452E2DF052806E70220269BDB35953A2EEDCB5CD22280425202D107ED487BF\
AE8BB62C0D31289B2FF5001210223EBB25E17AAAEB0FFC15787701965707FE5E6CA9435A17B98CA\
94032E7110CDFEFFFFFF2999440CEEE212D489A7BD1E97A8B5F596E5B1C45255234581DA512244A\
D6CDA000000006A47304402201D37BA992C8AAD600E33E5276C80F9B0D1340F33BFC6B79DAC09DC\
48D9D95F070220277191D72FA519070E6297C3EBBB8B7C04B164FFAC9464DE31D12DD8F608F7B20\
12102F89CF53E3F5096ABC4A35E5E57B68B9B1F5C5D4714066EA86E44CA52CE154285FEFFFFFFFE\
300A392A6EC3A0E7DEF72C8DF9459451523CA8CE99D2EAC5357BF6F33BFCC7010000006A4730440\
220459746791B0C75FA4EF838E463D62DE3C2B889F190CF4FF042E53EFA75421EE8022005CF1461\
B0B9273420C101EC97BBB5A2869526ED18A32589EA1FE62530D358E00121029691514F4DF0923E0\
87FAF9BBB6DD5C83D14C6E5A75B9B89FEA411EA770844BEFEFFFFFF77746C613A689F6E6061A144\
AA5B53EDFF6E7CEDCCFE51883D521557B43EC36D0B0000006A4730440220779C23CB938272E75FB\
EFBDAA45FDE95EE20CF8990159A0CE94C19F9B0A9E721022001D0A81FE16258C8FCCE2F93BB5392\
0A4AF9B75C1F80A98A4526D9E39D002FEE012102CB177C97AF667E60A7BCF938BA51B18EEFFCA0F\
6F14546D0A07AF57A8FD7DBD8FEFFFFFFE15912C4BD8F843B8C8B7D4D455214AE6FDF25B0EC9E34\
71894725CC44187F1B000000006B4830450221008FAE6B55CFB7044E4DFADA906F5D36004EF0247\
F79B8FEEC0407F102DC6D6D1B022058C2C729000CB2BC83742F768646463DD0A768A5E2DCAB85B3\
90F9988FBF5CC70121037C2FA38987AFF6813C7EEEC01CF72A039FA7C2122F54C9F3B28845DD4D5\
4EC4AFEFFFFFF89AD86F59CC0DA3F36E8E4D89930E679E2F2B502B6E262E98704A21EE0B6BC7900\
0000006A4730440220515DBFB317E1C9D39B75DE6E7368A764C3014D4709A22846013BE50826F08\
EE7022072E7ED03BC3FDDC499B9E798B5172B56696C3B425A7C5094AD3A7E70BEB65F6101210362\
0E9016636C0FB92C092B17DC47ACC45159853DD3BD742AC1EA1044A2600520FEFFFFFF41A353F8A\
525A0A619643F35FE61F8491B194D330C1F0F2D0F04A3DBF012E4F1090000006A4730440220600E\
30EC390A0C58AAC2B9962A3372C0B68550969DD791C8D9FA9DAFCD0DCB1D022066565D202C7D679\
F0354A23F5D4D31A18AADD8E43C8E1CB613580D8EE155C68E012102CA10D95055EED8EB5DB7D49B\
83E50299D2E9A8995384EB1F41FAC43273E53D8CFEFFFFFF4A3978DA93185EABFDD01B6C1682C95\
5F886AFD8128220BAAB1A3231A5F82F4C000000006B483045022100875101FEC8BB3BFA118063D9\
B127B06620774CB6A54373ED98814B314A8466010220746B8020DE93DE8DE05EAB2DA781C34327E\
096E8BA7B2A774835B1D23F28ADA001210262BC874EA350E98F9C09947CA9144C686500F7F6FB17\
ECDD3A7D5D1346FE3B0AFEFFFFFFAD32E49995DE888F58193BE0FBF43793C35AC7921C26C87192C\
A14E27F4152AF000000006A47304402203639E76CF683B9DE619E5701AD97ACADC379795C2E9B9D\
0BBF12CE0B2270772B02201359ED878E9581E8CCF2211E66F2FDEA217F710A95B8E4245C83648FF\
47125050121024391EAC5430958B78C5D863D9E337E6C79CC26DB465E3ECC2407543361A8C2F1FE\
FFFFFFAA86FF56B253D315CD4C8BF106D06427234D670C1E15F5BFE6049CB1B9B5B876010000006\
A47304402203C6A937A5C6B943300E4B26D16D8200BE69CA7417EC1F5F9486C739C3602BB6A0220\
2916E9763E6D6BD635FF1272A1B30733A18A0ECE2D2462CF52D41AB8D18666AF012103C6010E074\
F8D29085BB94ABF276405057434B75C755B584C849AF14FD47B90C5FEFFFFFF5FC1760C8365662F\
9DCD74F4CA5D4ED7D02F0D4B542CC9B9A58980A7258CE945000000006A4730440220209D1E7C1E1\
160E25BD27EF9D77591CF8715875601BAF71DB5EB60A40BA0880D022002FA12F248008F5041B866\
AD34CBF5A15F527FE34387B222A850F155A9F35AB4012102F3D2FCDB433F52A211332D70A579579\
A871EF3B37E587ABC0A5B1A84EBEEE1BBFEFFFFFF4F90842397B30546D74565D1E5511C95FF8F2A\
A32397314A8C7547008A52610C000000006A4730440220394893509E09930468151580419C2523C\
A546B1C6D90C97B5017BADD75F76C2D02206E7BD35B309CA5FA277A0F2925BB4DB872F69E932DE9\
8AB7774A9C6A7B997369012102D996DAF5BE4D399F3D668FDFDD2D146EDEFA40281954E6ABC135E\
4B7AAC16E56FEFFFFFFA006BD2E45DC977E8A810884AB219BB9112032389766FA4A608D353226E1\
FE5E010000006B4830450221009105CC7F3AA57221C0B50B818F08E4200EB04E170E85065241A98\
88450D5951102203E98AC2CA99A85DA850DB4F5A1DD5010E11D11AFB89E80D31184AAE63DD89855\
01210389A71C0DB9ECFFC4E83822C44169C3E0B4B9FDAA55D978E8FA6F1AF2FE9FAE03FEFFFFFF6\
907464536139CF331C1BB5E65BE7521B6A5BB578C18CE03549F99B7EFBFE452000000006B483045\
02210082EC7009D8C84B5426E610CE3CD5B6FD9D69EEE4E6A698E5AC39CCCA613CC9A3022027A0F\
BF3DC31E369EEDD586ED9FC6DD93F449762A83FEE93B0AFD6C9E51A6BA3012103E42C4A9088CCC3\
9F2A079132B27F030A5B80E94D6137150AD1E68505E013FCACFEFFFFFFC2F530D80E8173DE0598C\
7C33BCA6FAE2B56EB3DFF5EE31C4AC801596172A21C010000006B483045022100CD699438D42133\
CD1B997799AEB5EFB5906E7ED471B663F5D2F0815AFD6E985F02201A2D7DE14EB938F32C07F8668\
041B084AB2EF1119C762AB1501C0C6557F6EB8201210279A5066F2C01E4067FA8BE12EFDD68C71B\
93A00BA94B5DA94E203BCDFA63ACFFFEFFFFFFAFE41B9C68031E3FABEA2BD69C769CE21D0FA9F73\
4D523CCA3228EC9ACA18B56000000006B483045022100A7C261461065399D8F9FA0B77D2C2FC4EA\
178A2AEF6C1B0EE46C4A0FB5DCABD402206A11907DB1227FBF39677395E818CC2C193F5024A86A6\
644291B94F8F4F4EE730121037E67687FAFCEE19C6161996324233A9B37CC4DC3F8E69155A733C6\
2CEFBD5DDEFEFFFFFF294DEB8D213FD8F1A37FBC07A7EB76CFF380D6A96D24817E2E285C63DF68A\
20C010000006A47304402207E5ABDA699ECDABF21B48455C442B2F6F464C63003715841B0CF91DA\
0E1489E9022034E0413ED6B250A87387C26B2982C965874183BD844AB161F41A21E4D252AADB012\
1036D01AFD379CC8123C503EC851C99B4A9846DE915B7AE8EEAC276E991C3791478FEFFFFFF0B48\
409289DF3C15023EAF51DB1FECD9C7C106F56B1020BF4FC9AF4576CD6BD2010000006B483045022\
100FF89C7D2D8DD0274E33577A06E042CE856A421514C98ABAE35090D52CA54A49D022070997A5A\
3FB2EE95FD618881F28E367FE5F6FE43262234F501F90D83FD88D04A012102171CFC1A829CF87AC\
0225694D6F21A6FF9233C81414F869327EA85FC7601FF50FEFFFFFF78820DE905C314E72EC1F1DA\
A6BC955D6776F636A286210FFE2E4399B9AD6048000000006A4730440220506CCCA4E769B100768\
43EEB1D8F00C3AC727F334668ABF7FECDB00632F70FDA022028A513BCF4E9B938C416F40459D5E5\
3005A5DFEC2BE61DA1D1BA64EA4AFEB594012103E56D012F6ECD107B3A92056F0FAAD2B1411BBDA\
BFC8E973BF9713F8A84B3B0F5FEFFFFFFD645F1620876DC2E6473798958D9FEFC99A39B6F5BD6D6\
092B9BEB2BE1DBF209150000006B483045022100F5DF19FDA3EE55F87F1C37235AC36F391CB6381\
A85D29C9BED6180A1F29B2CE202204C2D93D9FB5D1DB00A90DB18527CCF1E5D1CC173916A66CBFE\
B342AB31D6AB5E012102D77C741C3032BFAFC864F3FD9A54E26EBA193F874CAFACB72DF49001BB6\
78938FEFFFFFF9FB94C0A0427BC9EEEA1E7400AF44642C9462980CCD4581B5479EE673BC35BE300\
0000006B483045022100CB5CBA1C3FD0EF7EC91BDA783E2164EC8BCE0C2EF37AB0882DA0B5006F2\
41490022048FBCD54FC478AED615E427B14716EA2BE9062343286071B47B4398F19CD730A012103\
1EB1676E69C7D4038A1194798E4E5BBE52FF141A1C7E6AFEA4E6ECB56E615932FEFFFFFF6552707\
DD5C797CA1F26885420684AE8D1972AD7E27FF360DE0D0A4C77F68EA8000000006A47304402203C\
F8288DEE2D372E6837E5B5785E8B529DEC978E68B03B71762AA6826394C3780220436BCD4A5B8A1\
A8554FFD166B5534339FBEA0425BC4181DED742044ED76582B1012103BA930847591F763F93F6F3\
77B25DA279F1AE5BA446179639383EE66ACEACE247FEFFFFFF6127DBFE1E032906AE26A817736B8\
DB4C0E5A075201A0EEA6397BBD6B73CDBDE000000006A47304402204481828C981D3438001C3417\
12800B2519D61E310FAA223D6561657F10419F82022019DE9CFC1996CC795488DCCB8C30020010C\
CABDC66BBA6D23BEE88BBBC7B50A70121032FFA88C9A91387A0458E41190AC64295775FD5609E78\
6685518380EF3F23EBBEFEFFFFFF2ECB917DBD882F0A2A82541EE8808FD30E4B227D7B0273C3DB5\
C01603AAF7B9C000000006B483045022100F9CF1D2F8AD08347192D71865C1F2CCC66A02C4C14D3\
3A4E2629DBFE46F2E48F02201A91889E88B82EBEE2961CAA7020EE4C52FD005F850ADF6F58DF517\
FF5E67FC3012103799C088BCEA9E46CAB530977422AB8413B60CE88CA6243DD246101261C06E78A\
FEFFFFFF186D3F35E5ECFA7133A33A7499B2A9016415694BD28ECC3D75D43D06B9F6FB8A0000000\
06A47304402205B30E4EDC875E5FFC6158C3ADDC02B3A044C57255C14195DAD8A17380D05160E02\
200A83AF2C77D613523CDD8BAE7C5F25FD69709B6D7258251ACFC8BAF4E3F33BFC012102FC1427A\
6286B32DE5DECAC26420B9FA4529844397BC6966CBE555402203599DEFEFFFFFFF907AEF5DDCB88\
646A9D613893E4152846A78C91FA67CAA059F0A48441E3B4AD0A0000006A4730440220651327027\
A7F7634628BA9C58A8895CD3CB07B074842DD14BE6ABE99726284EF02206C5E3546FC189E5B57DC\
5C6D3AB7114756946622B9BDC90E8D371FE173D81FD20121036FAF1541749605EDF1039FE499764\
4CC2A94350128ADDB5E211AB7B8C56DC36EFEFFFFFF5B6A30C5AD6481FBB4397D0A739DE9612272\
0BF45461445F0A99B9DE2752880F0F0000006B483045022100F5E817536F764E491344FD52842BC\
AB3DA3F045C7C8A30953C4CB28A06DC0A650220451FB0B5D7198948931CB5A0E930EAE272298DBF\
E5A9DE9476A0E160CEA9FE2F012102D2AB99730F5B1099306207DB2C9C142F92210F4C598FF5D5F\
3F7EEE8BD585632FEFFFFFF974135B25B378239D654B85486403F0D5132C90269D05167F1F46C30\
4D873523060000006B48304502210097951D156119717C04826646D0B3F6662B3B8EDC54CC629A7\
A4AA6AD54E517ED02200A0A09ADCAFB59F2DEE40F330E8AE7E29AE5D0B6BD96571001FA9B9D3A27\
9CAB0121022E1244F5D2953B0031E15A15BD431F3D35EA1D753D162F6139FAB381C1AE5879FEFFF\
FFF9E4858ACE697A3C9222E58760B7F1474635B5A58F2C61C269C6B6AE713082479090000006A47\
304402203AB142F641B81C88E9851E7EFF0B3CC8044B9F00C212327D6B3C0FCEFB849FE0022058B\
0BA09EC10F9A7039A51A95825E38E58CC46E41D820A495A1729441CDFEDB4012102082F45107BEC\
A99185E1F9FFE4755A4CAA1F0DD826B82702D67E99FA901C5823FEFFFFFFE8B534C26245B46D714\
59072F691730535071C7878720AA19E81E42BB4111F86010000006A473044022005E7EC0614D3EC\
0223C38501B94A142BB990D2A11B3494EFD556D41B153C2DCA02204BADCB8E5677540DD56F9D294\
98171B5A4EBF865A3921BC29041DEBBF6FF3FC101210291C298A2A9457F49A066B4B777A49CF1A2\
801A86B85C294CD3C2A2DDF2DF0825FEFFFFFFB4BF5DE37BD98749A95A6A7B0FC346A761A14BE71\
50ACF4FF49C2262D8CA8280010000006A47304402200D415EB057973AFEBCB4F39498A46EEA30C1\
F8BE941940A30B0C4EA3DC66A1CA022050E516C2BB14934FCE320674EFC3C946DA77065EA52E471\
B793D52BCD64CC744012102BE991605FDE177CCD48DF9E7D6DBA51F6F2A9129C792E99AA7FB85F4\
5102D746FEFFFFFF7501263214F5367E302580485585191F5B8A9229A47DA6EE8F3FC87DF337AD2\
60000000069463043021F316237D50E8650C3319140A9D563EC9859E0D3060D521B7D95103A331F\
50BC022008C2EF6F467A731AFC0D8B247503433D64BCE461FE5DBCAC45F864F470E5E9C30121038\
0F7E8D1C2799542F5F77C40F5A9EEE847AA0E9F4321F5AECCF5B23A446A641DFEFFFFFFB7B03023\
1569DB58DE4836937078BF3ABF82708FAFBC5BD4631C8DADED4F11A1000000006A47304402201A5\
C6AAC6577D931DFAA6D48DF7B998A6518CA63105B15DFBE7611F8DEA828BC0220249FF2B5B7777C\
F0C3AD8B2EF59EEE61981BDD0FE7E47F168CD384B5551386A9012102E94172320BEEAB7960A7D20\
5B412F51E09FA53C9A983ACFB928C51963B446DA4FEFFFFFF9F662C8D85A44B93E332099BB5F4F7\
5D3E8A4630FC58F77A581811D02598031B010000006A47304402200CC0EC6122236EB78F83A9262\
99DA114E4A781AE4DD7B1DAC5FDFD4290FA530F022041A30B045F7D1401DED0FA9546AA2EF77662\
E64C259F7F41B388105E848F1A1D012103641B783381D0BC6C3DB949BC8482693920A2744A94A68\
8D32D12EFA7B6ADF781FEFFFFFF75F5ED7FEA6663AD569DC68EB953533ECD7F6CD16F2048684F88\
53303C1A1F32070000006B483045022100E1FCDE502947A8240A9E8B71E55DA1E57C9A9B5FDD64D\
C13152E46D6912E2D4702206724F56FDE73E30BEE436C586FB096217C4BB4A2F929E5D3D2A2330E\
2F8F3DBF012103061600E68CFFE8461351BD42EED8C9AE67D2398E6546977472120D35E7822EA7F\
EFFFFFF064EBC7BBAC7F4B9B262B0D8D46221644DD3786792F7D623E9C26339D6B77DEC01000000\
6B483045022100800C2E53043667396C069C7E6EEB1EA379D72C9E4B57BCFC31598C57D22F86E40\
2202D8CBB09B9F6101DDB12B6D548614FDF40290F7D5E8400D0E6FC1C0D71AE0531012102D0B42D\
07BBB9073888FDFAC7048F03613700AC2FB38337EE08174A70EAF6C3FBFEFFFFFFB53989548D09B\
49DA28A4D63EA18770ACBAA2A7A975932D3ACF67B2F89AE44DD040000006A473044022034A849D0\
2A587F84C5E0CA938DBB0E1E2310B83FAB6010C5A0B8B9399F2EE92F022053644A25BBA77406211\
DF385460F151A951C653399EB50F64E6FA761079827060121034114C00A599FA7A9F3E2AC53FC6D\
967739615011BCC8C59CFD8739D9C9F0B878FEFFFFFFB2FCB774467A071C73270BB00F486B1C7F5\
D8D8F592B59931E33A76B3247F53F010000006B483045022100D801FAE16D2F622F9076A0F1AA4B\
7FC13C7B7F354506137C4784F7FCAF5CDD7A02205D5E072E4445F977262E2A321592B90A1E22F05\
D1CD4D955817A0A36C752DDEA012103C5C0E3C0154C0B909A1686F764E74164BACD6F22DD17F75E\
E0EFC224A4778FA6FEFFFFFF4CDA65592063CE9D4028B39D94B49AF93EE38067078FC10D4AFA92B\
5478E8689000000006B48304502210099D1E5C3E4866820938049214242F1495DE3DC7CD2F92DC5\
C1F2D36A7900534402204EA8903F1F2441E8AA80F96E38F2AF26C20D8B271206FB6BB00F7CC569B\
9DE5A012102B3A864CBA3DF8AF841C412BA04F294596730B908A77A7E54D9773F30E4154F51FEFF\
FFFFF538BEA6068E3F5F11731F3FAFDDB51F20E6BE37B404FF3B928FEF72C42F3FB6000000006A4\
73044022042448ECE068B39AD20E8312654933B1829B2B88A11F6837BE15A6E60A069552302205F\
A4B810677BE70006F77ECBD74C77DD00AF64FCF0965FF29CE0AE37667FD7CF012102A2B26463161\
4C61F120C17789FE730D9C3A783A0BDD772C4667DE372B91CEAF1FEFFFFFF7F192EE5188BA810E4\
061EB2BA51E60EA9284328F39C9B8ECCA26C7B75FA4009000000006A47304402200A8059E0512D2\
13CFE0ECED4007EE5762083DD2AFAD4819AEF1165F6B2AD10E802201C134ECC63DAD9F20C030825\
69AFB1CBCA8DA7C0727D2CAA3667C893C7C741470121038F9D49569E9FE3F3F21894069AF5B0D04\
9EC7997E865C394ADAA6DA49631135DFEFFFFFF800EBF1CCEF5A6AB7CCCA6964042D39C524E429D\
8593353F14DCD9F9637B11DB090000006A4730440220021A46613FFCEBEDBE762BC53C1ADABC124\
D2B912464ADAAC9BB7BB9ACE04E6D02206EA3E7952A6FD5ECA6445D4F6329FB4B55874E2371C425\
0D64C49D850C46B22601210385F75D15D27426450E39B677E5F496754D69399EDA2928D7EF1058B\
42DF38413FEFFFFFFA1E1EBA953759C805AC560B896B082B3892471051983DCD2E28B47A39903CA\
1B010000006A47304402207C2FF2BF9BEED82FBE08D4C2C1A996C10B8138567AE43B2A36984DEAD\
81CA14F02200FCE6CE20FD7FA4A8BAA0DD00A65BD1BD45EABFB40C90F31E949AAE297784F5F0121\
0254A94F84B345C38F0F8F824BBF3BC526536B8240C9D4DF7035C85FE5FB4B636AFEFFFFFFDF34A\
566A68608E96999CA7DB269621317501510EEF00262E39A8FBD03099D08000000006B4830450221\
00CD0B31C68B74819329A9D4AC399134DEDD3F4B85DC9D2E293573F5E8919F3C6D02205AF3598B9\
1D7DE5A5E149820E5171E81D8D9504880FD1ACA75A0D03362EA764B0121038A48D1A23AF4232AF4\
71D6DC1A191F19C411B9B4549FF53BB72D8B836FB38A80FEFFFFFF205CFA963635BDC39FDA58266\
654C413987C703DAE7CC0592FB71457E57872610E0000006A473044022075D8B67CED25516CEA06\
3043872C6DB11FAD4BA75A3916AF4BE920384A1AE6EF022058C8878B44B315D2DFB6DD67BAC5B11\
668AE20E4DD62CB10AA50A67CA97844E4012102B5218D293F5C6FFC152DDDF4E28AD723FE7392E7\
DA70144A848F09CD9EB4AD5BFEFFFFFF578E91A6A797CA5797978AC3CA67C282DA1772F81D9383E\
989B47FFCC782A081010000006B4830450221008B39E7F21ACE82F796226BAE35BC9AFDAB555831\
4BF5C85333633EB2CD8B14B6022031353816600CBD693AAE9410A7AC37C87E928E05D9CACD5C8C1\
D06535A968C5701210328654505F4C8A9D4C70BD52DF5829B9D9E849041CF31DAED4392B94051C5\
98F3FEFFFFFF4857C139A3B62BA8D141DE5E9B600F8EE1F279EB518C456F5AFF862EFA18C2BF000\
000006B483045022100C30EEB0E65965CC2EFB7EA6A650F6F1142436349EEF69B243489F795771F\
A50F0220464B31DFD59C45CE072A479AEE0F0D8DFBA8155B98E94CE2D21C5AACA1F266DB012102B\
902E7E262F3E84100090F3D1866658B06C47757DE9CB6C3A061E8BBEE90DEEBFEFFFFFFF4873B0C\
CB6D0A7D701663C82A946504F294989BDFAE192C17548172977207D5000000006A4730440220705\
49024E307BA6CA776ECEF7E819195025AE8ACC187E289404FF72443F56A2B02202B270288F7F533\
B458DAEE29318D73CC09C437116DFA46BFF87E3D47CAFF348C012102EF518E5C704600AA41D3659\
F2DF8E83C45782C59C42BD48C8106004E27B43948FEFFFFFF14CFFE31E6F08B887D62C0A0B64B65\
BC6068F1C9ECBF1735DDF90373308EED09130000006B4830450221008F5D0C3DC6D882B5EFF92D0\
C302CDFD7CFDC084CB03B50B0FEAEF9ABAF6162720220509FE16EED0997BF282C908CD65EFD651B\
19DBFC404D0D3F8BBBC8AECC982D810121025AF6F419BAC6116A9B767EC6FA5B9DF24A8E6C6041C\
BA260D25FC54C077215B5FEFFFFFF9BF24365AE67BA79DB0533076450E31522170493D78EF1A826\
3DDA15EF29B190010000006B483045022100F037A4F57D102C456E79E281A2A92E0644AA51BDA5A\
B37CA757062E4D0FB1B2E02204D7022F4DB74A0D4D7059161891CBDF0377BD2BC7F507751891513\
714DF0051B012102B3FF9D6F853034A929C21F7648A1AFA154DCA63765122AA2F62BE731AA1656A\
2FEFFFFFFDA670764FBF08AF948B31EB63F5DCA6A823CBBC3FD29973A236B0712627939D2000000\
006A47304402202E41FDD796BB72B6E12AF50E04D435310881A674AB0CF49F44441892EA23F56F0\
22023EAE7217996BDAFEDF62FDB71182A64DB0C3438BC263EE586B708B09B4364F70121038F9D49\
569E9FE3F3F21894069AF5B0D049EC7997E865C394ADAA6DA49631135DFEFFFFFFE9501F8C2A669\
EC372A48D8929D580939FB186F6F95ACABE1DC3FAD8C20F661F000000006B483045022100D1CEBC\
50BA64E589560D762C5F4AF44996D0BF8E9E69672C1EAF51046F1E76DE0220700F546ABA509B729\
B6803D33091ABA06CC43B0CBE9D1A67D13027522C53D20F012103291527591FF006CE11000DD02D\
506A153BFE17AD4E4A4FC2DFF1EDC17F38FE8AFEFFFFFF2B9C2B32BAF3DB2B90607662A5A17FB62\
9AA34FA28E7A25975C92880F9323451000000006B483045022100B6152ED42FA176A7462918CD7C\
D1D0AFE5B579B3896C3E28A6AB24D77E304E9F02200EEF35806AE0A9B7B1B6055DC150D0A8D7940\
111E7621F6911256BBBA8AB2FEE012103BAB2513CF3CA4722EED87C70942AF6A8945CD84E4963C8\
2253C50B1FAD5779DAFEFFFFFFB9915774C349FDFA103BF7B48AD48B4B4CF782A5E8CFCD61DA633\
287662A33CB1B0000006B483045022100F104BB5FBA85BE878ED9BCC293B195FC10A326CC954231\
E1B7E1246DDD9E2C5702202CCCF325EBCD37C87470CC64ABE19C29CC1EDF27B3BDBF870098ED4BC\
70DB24901210363DCA42DFDCC7FFF94580E269EFC758C681F49420BF2AC944B076CA58502B99DFE\
FFFFFF0DE6970C5B2BC19FEABFA0C9D579B43569214C797E9ED21B4C72611DCC0E11A7000000006\
B483045022100AC25A81EAF1917B7A5F0A9BEDBFDF2F80DB4225BF6667D780F7038F995687D3E02\
203EC775276C7385DC9DAAFC6C29EFD7C3210C56EE47FE33E37A44BA549559C52A01210391AD1AD\
41B29A6D5C6F646576FB9836E53111FBF7189922C3FE09627B66B9CCCFEFFFFFF5F83CF803992CC\
AD527856B37F8820E0B5E1BBCD78BAFC5D77FD9DA206C61149100000006A47304402200568D675D\
94860C0F60A60E0E20AE5EA757E0D61FABA083EDD958BB06BC941FC022042C2EB9B00FB1254506E\
B5DFE91BB43B75F89CBBB695E9FF94B9907D394B5F4D01210303E349CC0F55D1CC3FE5192A9B2FE\
EB1B08CE823B3CBFBC8EA3F93031F5FCF4BFEFFFFFF7A8B6939B3D9A86CCF9EB8CA9AE31D33112E\
3A2819983E42C42D4B0550FC2AAC150000006A47304402202191536927F60BA11396B0AEA901AC7\
2F2217991552F64D7BD542910C8823D7E02201A9721ABF634C38F589055DB834B833096C5237A73\
47387AA95508D4162D444001210373B3773B34EF8DE00042CA2BB5D7D720E3532C5D34E781548C6\
9421F72A722FFFEFFFFFF6568982ED0EE554BB5F51D01DE9C634A3D5D4D83B794FE3C3D864290BE\
B779C51A0000006A473044022072174AAD35D7E2A8C0181D6C063906224C210064E62F4E5711D1F\
36E54B55C4D022040E3059A0FBFF93F5DA9DC57ACBCB46C67973FF9207996FC2A9E78A5C4033074\
0121036FAF1541749605EDF1039FE4997644CC2A94350128ADDB5E211AB7B8C56DC36EFEFFFFFF7\
63E11DEA21E5BA053990427F45C7ADFA9ADCB5306B767EFD2B5CF359151F7AB000000006A473044\
02207129794DBAA91ECDBFB71CBA07B2D07BFACB87AB8F501FADE818FA8AD90D3C0E02204AB9D3C\
AD7FAEAAE3B0F2D347C882CB2D940C38B1EC3B0ABA5B56DC8B9E83730012103C16273E7C5A01D7E\
09FE14A662A78828B9E868981634F34F7B8999D6D9078DF3FEFFFFFF6B60F07277BC6BE54B76C0D\
CA8031B50FAC9EE48A59A8C5603C56CE8DDE08BB4110000006A47304402206BBFF163CA8D17AA4B\
3EB8CAE4729C0E87869C0427C8F9D6BABCB279A8946966022062D3E16132744A5CEE5DD86B00E95\
62FE94D4B792B471BD863AF74A68F0501420121027F11A82AB925C38F67D100122E97FA5B8AEAE1\
9097E42B3B0C7CE3EC2AFF8637FEFFFFFF6D2A3887A1404292BDFCD25676FC50614FF1550CCC449\
5BDB6D9BC349F1E4D04000000006B483045022100BBFCD6AE1BB0A8E575DB1E7CD2065BB4769EAF\
D6DD5E6D1FA058125C77956CA4022058D1EB7C6BFA155AD92CDBA778D0570D16BE4DA136FC565C9\
F50FC29A1B008A30121031D39BA3C56A7FB2CA13CFACAF53B5D75128F84776760BA0CC9AF68642B\
F7AAA2FEFFFFFF0B0F915CA1180466F933A77F4BFE7A96EF735DA0C55C2419AED1AB5EBF78C9020\
10000006B483045022100DFE006516F75E6C40E480FC4194B260C69405C584B53A71D08EECC778B\
F28C4B0220719093D06EF8532DCADB26F7DA3FB7D13D25FCC390DD1845E9B8233F4AE905BD01210\
3854DC9491514DDACDD9255A73ABD16F116410E02C28D6FCE1D7362C898FB894AFEFFFFFF7C4E25\
62BA06DA90E4378F46D371EA175EF0B82596E5223E8DC382C05726B425010000006A47304402202\
36885034CFF3FA4153B5C9C40DB42587AB143E1092BE06547F6A9AB42EB463602201C373A4CC7D2\
3BDDA2B3A4FAB1ED31C906DBADBCADBFD97B5169A43131F58281012103DE5BC8D53DD68C5596570\
0760FDF5341A5188590968C5614F2B465633BC9C77DFEFFFFFF2363AFC09BB392AA46AB95C1F934\
251ED4B65C2FC062280ABE1A895AED7B9FE8010000006A473044022057598DA10D7B64AB76327E5\
7A479122742A898FB916528BE8546A4FA00DFDC19022062A4C8CAF1F0103CF3A83EBDF7B873B582\
EF9CFAE0890D6F2E32266DAF7019770121028DCF573A4BD354874B8EB989F4A102D4FACE9FEE5F9\
C2A07F5C3A60AAF59F0C4FEFFFFFFFC455F5B69873E7A37425EE54DB57F21A36D405418B5624239\
AD8C2F0BCF21CB000000006A47304402203ED0315F78A9EE90F22F6C785F23A459F3E3B09A6D004\
4D43CB1ED5CFEB88C5302205E5D2D5AB5F185D4786F7EDE0D92228068860915BBF27ED3BE2AE255\
4FE093CB01210264C40473153D46F20E5F0F097142A641D55C83A7816E6F8669332795A9C8B4F0F\
EFFFFFFD69952543110A4A84DB6BFB5EA3E706787BD5A84FC048A9AC24C0BCC569C429700000000\
6A473044022013FB588612E859757BEA88EB5BF2FCB41D96FC4965B6C8465A6BAE50152374A9022\
00C8B08D63906CDC48D5E08284DDDFACB6688951F1FB9F7928384492CF85E486701210324B9CE64\
803D6EF664DFF288F220496A4ADE424952D8FF9B446E002E7C233662FEFFFFFF88D63CB9D000785\
F316D4B0BF054CA29F8414203520CE2DF915CC2F5AE0337D3010000006B483045022100C8AE93AA\
F88ECE29140BDEE74C884D4FDCB6BDABD757EE55DBC4AF38EDC631AE022037400588ED2EF57E3FA\
9D8312FC20B6BDD3E72EDDFCD377300AAFAE30D941EB8012103A892576E3BDFDA18EEA853D824CB\
1AD6F055743068936D5D9AB871506EF72BCEFEFFFFFF4E7F3EF85FDB4E5814A241DBA01755E0551\
27DDCFB0CD1997662009B2475BA0D010000006B483045022100F405036071EA902B88014413683B\
39B7A39F6B8AA5A2E3C7A936DD059056A43202202CDE6A63D485CA9AACBB6133797B13E330C9F32\
FD8847B9763FD3B18D5574E1D012102BF43681C36DD60D09EB0C2F1BA1E5BC29720917EDE09A93E\
62069B9D90CF4CD8FEFFFFFF8E6F7C935D0B9EDEDF8C80AE74A5DA0B36B93867BB8BC7CF43A5240\
D4ACE1F25000000006A47304402202E60AF2CA6FFE432D47A95D02D2E20DC9F7054B0821B7B1F93\
CE9DC5C2D59DDA02201AD53B9EE7DEF87BAD31DAFCAB0D3B8265B76110F5B39A60B1D2B28298C64\
6250121035736ADD861574C51F212E629307C36795D6C5E6E055E3F8DDD23BCD2E2B545FEFEFFFF\
FF8197C49308BCA746356604F494BB3B5BDAF9D29293A1B7EA2038F5621580CDFD010000006B483\
04502210080473DBCD16DD3E52E7C76383D88E70389AF4F6AA433407D94C4155D2F561FD9022068\
6869C524B5686C223D360B5F9DF313A415D1D6A8306C36110865EFE76AB50D0121026129A1D22A8\
D12009A9C5FC5F318B5F75E0D52D8C3CA0D4CB7C3874B9CA938E9FEFFFFFFD8BB53D908D38A33D2\
E40706909CDAD5702CD376C25C6F01CA0FC15FBA323884010000006A473044022050D42732CD1BF\
53C4A6100857C54F881779E6274B4FFF9F3A30AD6F9ABA9CC5302202E32DD8FB0C1DB92F8D6CE38\
F69101F3B6B5514AC04C7A9CD22FFE057043D44F0121028660770CE2FDE143A4C55BCFA6B6126A3\
4B5DB0C9A064267C3D50F3F11A16019FEFFFFFF907CFAB32BED703C9CCEF6EB7105318B5FEE7C72\
0CFE8051553CC872FECCC9BF000000006B4830450221008D9934A548FFC278D8478A09AB521250F\
EAB65E55278663BF71858336FAD0DB30220234D9B26B93918D848F719024F4CD6EB760ED313A762\
43FC0FFDE0FE91DA31F0012103FAC4C49AFEDE3093B7CFCB052383C0EF3BF39C7F6CBAE2710C70A\
AE683758C15FEFFFFFF17FA2101D785A23CA9803543670C71D525257311F33A4CC8A1CE0C906A44\
AB90000000006A47304402206F89D25F8CF7F9D43970B740C63D69B2C2B20E4FF5DD2E61EA81C71\
1D757967D022001C170B8AC43BA5A465E08EEC701116A9B0391F74343FC49BC37952BAB2368EF01\
2103E61BC8A3C7664D0B27405C1B5B313AF44A76C995A74D84D6C500342B6A9A2E07FEFFFFFF9FE\
271AC5F81BBF8469B535F115FCF9E4C6CDEE6827920266E2E2975D21552CA010000006B48304502\
2100D8484850EE9C3E3CF044F4D9E50D168EA0594F10736F2F3F06DE8498B6B8B945022061EA49A\
ABF625AD4C1627A511247BF31B2A5DE5F3E719759868EAD8023FF301F0121036A9FD0974463E20D\
8DD466F4105DAB6A88CCFC8079A0D38337ED66C8934111E5FEFFFFFF52D488E94B092B903D04076\
D5D876039D9EEAC0D34B9EADDB3F1D9E48EBC7B9A000000006A473044022047B0BC4E1ADBB1C2DF\
FC6F8EF15AD37C8628FE3093753175452A8DF53213BF1302200EFA6165D6AB9F1120C20FCFD83FF\
8EBDC1E580B7CA0A28B12CB9CD70507A4F2012102914D281146E47CC66929CDB63A1B3918E7054A\
30B7D7AEA1A6FD025762C16D42FEFFFFFF3A86A13B2CDA2E3706D82FA09CED5C318EB3AD649849A\
E366D2CB280317138D0000000006A473044022037F752A49DD669EDD231C4F128145DBD243F6A5B\
074028EC01B4D96CB967CBEE022030445D49044501BCEE8D80D31764099360C5B29841F721EC2B9\
C2DB020CF0F3F012103CC64DFA94426C937068711088D5CA5D39D1AF8105B0B8C2E70BE27D3C904\
60D3FEFFFFFF4AB12FCCED364771BA522A2BEBC959B397E1A56D5D0912F997F450C469E8A7F9000\
000006B483045022100E90951E242554F509604139D2414232A70009F9CEA855EF546151FC054CA\
D612022049BA80FAD6A619501A5B4F4ED8E71B638F01DCCB98715A4E89F046B15E1473B10121029\
1C298A2A9457F49A066B4B777A49CF1A2801A86B85C294CD3C2A2DDF2DF0825FEFFFFFF1E00402D\
586592CAB550F08A52B0796D2E21B04BD372ADC4E67A541BE6B2BEB8110000006A47304402203F6\
EAB9AD4D8E4431A9433EB8816BCDFDD06A5D71084E37B45B02CE975E7F05D022042170D7D76DAD2\
072C850912E2647BCC52565C3FC7E60C858DE32DA0F3305D38012102D55272E9C658A3D1FB4A1D9\
40DB305B365BF3D3CFCE137CA38F68FDA5CED4CE1FEFFFFFF00B854A4D9AED6AC1762E239CFE18B\
F5807B29ECE0485D1543373FF3B218788C010000006A47304402205A3861695A2C3EAE280922897\
A1A76872B175AC3CCB51A9FB81165C657E40E7D02205E42BB48DB683C193C79586270F8AC06EC6C\
5CC25CBB2C359021607C08442FFD012102FF45CA33FA72DAC565E3DE9897CE8B786A3A295A6E07C\
50C4416404524FC7D60FEFFFFFF50AD20BF3DE88B2A4F1797054100CAD0A7F039F13AE9BADAF33F\
486431F0A265000000006A47304402206561051B70DC4041A7442F948A705F7EA88B4D79935DE7F\
449EE44DF33B994940220799AFEF8EC6424FEE721F623277AF118E6624F77AF9A5222D9C169087B\
028099012103A28D27C8907989696B983A5F0C8E58FC45ECDFE62CD251C804589D3545289D23FEF\
FFFFF72FAE0C29845F8028BAF08A85046C3907EF3458ED73BE2D61D98A05998454BE0010000006A\
47304402207F5CD6A8B7A17F81EB3CF93A1E7AE78E772624BDEC8A592F0F38B644843A180902202\
CBC83EA4F5449BC5BD511A8AEB4822F1CFCD04996DE6CF013EC4E0CBA81D47A012102B61BD2F0E9\
F908E934C9A6EF93AF69AEF4876828FF277D6B1C2E62FBD3A5F8FDFEFFFFFF27F58236666420B44\
4296A8B8F844706647E3395C14A1685DC56A4D923DA5483000000006A473044022039A06F8F6F47\
E72830964693D367F7E4033B75A0FDE524B946BA49658307702702201062DA849676436163E9650\
E4DA843350BC69C6736BB82D2E4F7DB0ADE3441AB0121029325EF4727DEFF3AB03C59C6FF8EA9DA\
61169D75982AF7FDA459F7BB89075C3EFEFFFFFF4B767357B33657D3828051D189975D42358E997\
3A512E88A35F0B7052476589D000000006A473044022039A729EF9F5AE790CE2E4887F10E7AA194\
F6FD249749525418A1A1EC2D28597D022003DAFB2BBAAFC6A18AF8946B02CD3CFC1CD6E4A46D9D8\
AC3B2F7291A8A668F800121038CE98C9B799CBBD832DAFF5CB69400C243ABFD250ED1FD3E48965E\
0EEEB31A55FEFFFFFF36F5072F453209DDFF26F1A29D941C678492257577C477448CEFF12531D76\
66D000000006A47304402201BCFFB6CF764A6C06B9043ED43D690B83BE04111EC6A90959EA51E60\
A6F0767B022026E820899DA50C1F5BA13F55D1CFCDD38FE9EA66EEE8E3719B50CBCBCB12E61A012\
1026426153DC52356D2AA89ECA0CFD81BCDD20136F83057D4EF552B0E86708681B8FEFFFFFF055A\
1F83C8473114B57FF61493C4BA71E300BE079506A7C994015E1BBD884AFD140000006B483045022\
100A881695C6F2A4EAD39794D81344F4CC0B0A007A0E527C8FA042B95792D4050BF022066A3494A\
0143E7DC99D0D360DAC300252EA043602BBDE7614759D9A505E598070121025245A03C0CD98F258\
7F2DFE983CF33F32E59129651872BBA6DD5A2825141DCE4FEFFFFFFB92CAE3D7CC71B0E9C003014\
03CFA5650F4551E5C1F5B6BDDA0971FB04FAD919010000006B483045022100834DDAA55990595B0\
D8C3F859100CD45064D8987855E52A3BE9A087490E8997702207EA3E0F4F15862E8E128CDA8766D\
3F96371CA465D50DCADC7F2D2B6F0574C2F20121035862B5BD7848FA5A8E1F32C3997F49E90E3BD\
7693C02FE530C9E4B4A6EC446C4FEFFFFFFCDA21A1369723AD5DEA21322DA22B5EE9F6D7348F8CE\
E37E8E3601448A3158B9000000006A4730440220377131DC2FE51D1538952ECA40365549E6DE445\
690D9D6A4ED227B9222F3E75E022047D66A2207BF3E2E84D8FBF32DDD7ED0708F31ACB271BDB1CC\
4FFC7B22D6A1E8012102DFD2C92CA83EB8A57324DF88336BC63E16E445B75C472B7BF0722F73C2D\
054A5FEFFFFFFF211FA43144F037C842F6FA99C7323AD0A7CF6C834DCE24327E16A5EA95B483900\
0000006B483045022100A9BDFC77E6AF9D70BCD53C74348070A3EE6E898D86971F08FF25F96B43F\
32CD102206AB544852D0E4367BD0B5D38837B3A2C3967674F4E5F827D598BD25D4BC1FE4A012103\
34EC0A54CBF90C5B462373893C722B1D2CE49F8FD48DA2CDFCD58E849E260098FEFFFFFFF2DAD77\
81292C26474E73A13D5755F818D81CA5388492B10EF4B248FD7F3A86C010000006A473044022035\
22CBFEFAAF13B193EB62D683DB48DDB0ECD935E7A50D7B28784041CE7C56C302202E62A279B6F29\
62431F823D382C33870708634D1C7C3B15F333A15D67EFFEC8D012103BECA0622B751931759FAD5\
DD9174BAFCFBAF4727A84FA1AA28E95D95F39AC06EFEFFFFFFFD9DC81B279682CFB271BCBA77678\
8F82C75A3EFAAE9CD25FBB9319305D5BC0D010000006B483045022100E7CBD96C28CD4030A70830\
95387D4B6DD2FAA366C508429041E921889C5A1D780220227D1BAE5AB4DE25478C71D8AFBAD1145\
7355FA481653A9BA0A00A0CCBB36DF7012102B6DF72A2D36691B312B36FCCFFD7B353827527703B\
1C426FE858EF62A298800FFEFFFFFF0100105E5F000000001976A9145BD22BC99CB27C2DE30E30A\
8BC2F4ED875AC470588AC36ED0600";

	testOneCryptoTransaction(hexStr2);
	
	const std::string hexStr3 = "01000000012ff9f51a40b6de34869479010a6c06fcdee14d49b541e4e7a0300e87c1d9cb7201000000da00483045022100f267a790abc6a204b52f378a8e9dc56d04a448ef00b250b52125f0a663ff7aaa02204587adb0d4e738a3bd45428b95736bf920257020c2ecc08b173b72cf46871a0e01473044022064f9be9df69b8642dcf89946aca675aff8775d172e6ac5726f4a70e5d818745202202575dd2b399d34f106cb34911e639cf057dc6bc2ad844008b9a21e7c95d1b9630147522102cb983c378bdc64eca19b3f165e191730df705e256fcf708ac5870502455c759b2102cc413f3c5513da82feb6346054b80d4e6ffdff01d54375f0aabc61bf40cb60d952aeffffffff0200ac0d00000000001976a91468f84083493f0ec3fb458f2dc58f1d0cd620c56188aceaf123080000000017a9148007e9e237e1e56fb41b253434ad14c80a8941f28700000000";
	testOneCryptoTransaction(hexStr3);

	const std::string hexStr4 = "010000001026b7ee35b9ab9e522d3f23291c806aae99e349f5ca3e3d2ed5578293c1d51a57010000006a47304402203edd86b5111e66b40d2729505fe06dd1988f4302bb81dbcf134ef7b0afda52cb0220402d1e09898551827166c3c174a0361ab91712c67dec10260713d4b6350a02eb012103873edba53ae49612c58e205b74336a205d10ad7305bc00276a28ab3889918b1bfeffffffa87d705de9ce9e0b08ea3400a684e6a76641b586fb68c265e440c5f4b1ab230e000000006b483045022100bbbc0cd5035321e2f35dccb55e7f1bb5fe12e0930f6a956f51eaf01feaa34869022001d6848b97ffa96e7001117279edfb2435d6f871c3c225858c35471fdc3d46f6012103d8c5f036b31a2d6348899ab21d8bfdcb82f918e661f70b1252b23c2142051de4feffffff92999650ee39a8e1679e6112d2b7025e9450c945521ac79355f8aa791d63c207000000006a4730440220693354f0418385286c68923cdb171987bd7e51a9059744dd76e04ec6d99a7dd4022020122655b90665906793b34743cf147f873fe31e6cf49a56c8fe4f6e252e652b01210367aa4ec0c96df74853d4fce9d39a042e671c46c162bc9092a0653037d8c8d14bfeffffffcb4a754fe50c4de234b9934318800d8a9f83adff12635644b154783ef4728bb8220000006a4730440220706a72545078f08e1f8321f38088fff943a509c99090819a4ca3c885442a631002203c3eef630ea9fa6554b2e8b03a3585349eb922be2e4467a95636ef90c103c7d4012102fcfd9ed44b028a22b4580369e2c35ffc796a8a959aa37997c3dfda74f12ce8a3feffffffa6d455c1619a7eb0a1f61fafb1dda9c91ae894cdab9c0b9a760407677917ad42010000006b483045022100c32bf0343928d8fc639c2d618dcc116ecb216251f949829aa123c2a48b62c03102200a68dfe16e1d9da70503c6416476ff66d01fa870e91c5e9f81c7c12c027100d501210374d45d80a2acbd52b987b34b5b38c0399242e7d99e13d68cfc20f9cb4bc7b696feffffff98b9427f5acb20efb06b81baa73ab859b118b33f786aa1b9d29229ae3e82cb10010000006a47304402201fe6807e35b0b77fb884ce59f4c8a6a8372b33fbaff90a218049fd3edf5f666102204581a28e550ac8f0411a8b589861c0f33d9a1337e5e0ff88ed04ce3f98a81c7301210214490da3a2730b3ca5290c7276ce591d34986ed3289052b5617c48c084544872feffffff3433c2aef0cd1e685bc30088f62a08f75333ca031df2aeadab692059740b11a5000000006b4830450221009c33defac76d2d4dd0c2e7484643d2c14f845a4d95d055c94f863f78ea22a22602205750772e09f1f740149c10fcec7c92dd4194297b046e9bf6b71aa5b34376624c01210353091e88de04df5e9cfbe9ec979c0b0130505ac3df3c12e63a4cb1982dceb3c9feffffff4f66acb19d7976a9b06f75365c093dda2b3bb952d4ff30064f6c49ff1bca94a3000000006a47304402200259398b10aa177ac02d5344306942e6057cc60f7d75eb64a7d30ddd2bcf7cca02203161ced0992bdf693304f254745d8b4735fdd100e52077194dd8f9e89d854df40121039ddc4d81dc1e0bb9768a8ca5ad55888177c96021cf43419592f80b641c31e6cdfeffffff1da97495c6e3026acde4a57832f3b8403cec2970e1ee1d58013f377e31ae649b010000006a47304402201a51dec1c897b634f0331bc939b475ae9fdf6c5097ec9aaeae867204fcc60f40022076110fe522e0eae7be44aa31c8345c5245413702e27fe682c18d301257f738cd012102f1505fee9cd8633ea29c6c0f70dde2f6e52dff734583904a6268c3dd2531c92bfeffffffe312e104dfec2348841e24d0382eecd38ce1bc1aca505ec834d028a40cf8a12a000000006a47304402204be09a798ff382b2a0f576bd3d7514eae5b7527f727c42a8c286e2ef0b32b514022012673fb039a3d2b55e8d6f86153e8eb3fb6d5f253683771351b66932ad77e5f40121037f82cd3f0e655840c59fadae8feaf47e5cb71c5350c2b72655e349a5d7f09adffefffffffa3e0860dd5eb89f30ddac06df747c38b97c75990a111c878bc9cc97824fe0ce000000006b483045022100dd4019878119303a6cb13008265edb7e65bc62c799e23ebe2b7b0dbdf670a98f02207178dc575e1fbd7b72493788d3de16aa7e32981b109e19f73c2193c3ee48308501210239137d628df27fc41a2096c42507453f804704c4aa465fca5a325f0e0e47b995fefffffff844010028974023d83708f1a706f70aff8ebb89165eb1b1b98acc75a7fc3526010000006b483045022100fdcbb4225d4d66735276e1a7399486b82b3f1a91e986b858b4506a0e3f480c0102205634ecf26492913ed7973725a48c68f726b306e4ee1c59a88fa9d1f0afdcf3c801210308d2785a71af5aa30a9e353e8875837b68e6875bd3f6886c919aebeb573f4db7feffffffaeba9db48266a75757553b21c08de1b643e190c0bc4572bd84eb0c6167478cec010000006b483045022100d34317b7c7359f219aa15b150e919c2522a407f96afbf6254bdf4c69598345ef0220580d20b5f607dbc5dc3b92c55c17a58590ce1fc7edeb7dc966171bc74df01ca90121039857849843a49e02ffffd5ce6a44e665f36c7bbbec90b8a9d76a04c9866e883cfeffffffaec7db4c1c6a779d91c48c871bd981ab2f118b0780183f47a9cbfd13bfb6b231000000006b483045022100c6639d5818ec4e5880e33ef27ef68342947044c6272014bee54d519049a6ef9102206b86287bdb05377ecd7c5bd405ae24c3012f35042ba98e3b7b43aab07df08191012102aba9bc4f610ed22e060be7ef7415cb70410f140485b0e43fefb19e122f8aea3afeffffff26aedbfbef96c4de29dd6149aba1cbc41faadc552f1b36eb2c9a40be9ec135cb010000006b483045022100f43767326b6817c42805e7fd848fc29b94bf1951007f9e1ff32854756080782a0220676d1f70b24ddbca4acac062def38014a6552e830a9ce6ae1786cff6c9eb8d300121033fa7e89ac66698f1f795b14b91300b32e6f39d2e782eabedd7f2d3e254094373feffffff254c9e9d433e61d5a64d9a293a2f651624b393f18056a146686d7e6611b26bd3010000006b483045022100a30e8ec7c9628b99d467839c2f28b4f93565a19cbb0971a01d7deb8e245a30b902200c6712aef8fba136b1ef5b06b12568f99f68014d409609b04ae3418fcdd5cca3012103dc95e50c0fa7eb0a1e0ada983d78d3ddb4e045319151e6801120f3a592286328feffffff0253420f00000000001976a91459b460742e8a2c8b49b7940c4c541b53b01b8d7a88ace0dd3903000000001976a914a3c8ba876cd305502ecb99958d4897bd7f3329e988ac3ced0600";
	testOneCryptoTransaction(hexStr4);

	const std::string hexStr5 = "\
01000000015E4D5D0183B4DA7A41B77D547D5FF1C62CA8264A5E023B2D3D1CFC6DC6D6724B03000\
0006B483045022100D7494EA41119CDCA42EFA987039FED255F79B5BFBC8185C3EBEBD1E97C4C9B\
5F02204730B2A7D452FEBA499161C5A8CA63CD673D65F4685A3688FE4D7D02CFCF9839012103D3B\
311BC8764486803DC82A56A1ADA49FFDB5DC70DA1087776A096FB0D47C491FEFFFFFF448BE24B00\
000000001976A914188263F9118BDFAA2F27FB13EA287A8FE4CF45A288ACA2341A0000000000197\
6A914B9F0C61798E77494130E813B73BD957625ECAB6588AC30960F00000000001976A9144AC79F\
AD5C5952F9AFD1B02B0C1120C73DAC4B6C88AC8B012000000000001976A91448E68D70781AEFA96\
0DB2EA4E8A65828E4FD416888AC95BB1E01000000001976A91429A92C99DA5EC335DA6D1A0DED09\
7999544A6D1988ACA5CC1800000000001976A9140278E37CCC3A3C433163DFA2F3782F41981793A\
488AC4C129C00000000001976A914ADC41B7B6247FD42667F25CB29B59D863640094688AC541805\
00000000001976A914E1BB8A3B2F7B3EBA989CFB50968751F8E2DB14BE88AC3F600F00000000001\
976A914A572AF6D8CE00803BB46490D2F9FAC0483CDA63988ACC25F0100000000001976A9147445\
2EFF146C226ED34C4DD6F5A09D860B6F984188AC9B689F00000000001976A914F5635F81279AC75\
80F767255AD137EDA14CB268088AC0E8B9E00000000001976A914E2134793E2CEBCD52DF47FC8F4\
7530E9D5E3914788AC84D11000000000001976A9146B69CF466EEC7C25E605183D445E3B8669F3A\
79B88ACEDCA0F000000000017A9148B692FC197FADE324CACAE13780F5AE4E2B29B508749CA0F00\
000000001976A914CFAFF1172073C74F5E33E0B838E5B70A7AE4287988ACC033290000000000197\
6A9148D02E370486C28B9C572638AA4EE6FC42F3C9B9588ACD8BB2100000000001976A9141C1B1A\
AE8AB086BD71DAE3813E764401BA67C03488AC2FD30F00000000001976A9148CAC0AEFDE8229AD2\
588D353A77F72680D2A0ED888AC966401000000000017A914E203275C857D8ADCC39F1C8A83642C\
A346E0BA50878918F404000000001976A914F7A5BB397B2FE81471875EACE87853CA8E8F21DE88A\
CFA825901000000001976A9144E80395395798969ED1548AC6F88757EB48DFB3888ACECB56B0000\
0000001976A914161FD4ED3465CCC62263E4D6A721F5F9FEC4E87788ACB6670F00000000001976A\
914083AF4EB00DEBD2A9DAE2562DC78BFDBD48BF5BC88AC68C81B00000000001976A914C449F1A5\
EFE7BE1123862AD3CBDA136EA4E96A8088ACA2930100000000001976A91452AD76B663AD1596CCB\
462902C21308248ECD5CD88AC15626124000000001976A9140C49C510835CD35FF9EFCF4A2AC6CD\
3FBEFA5A7E88AC90760100000000001976A914383818F3514D80388E131964BC74E40D6B7E10F98\
8ACA1B20F00000000001976A914135724A81E42F483BF7E792AFD8705DEF891A50488ACE17D1800\
000000001976A9148AAFC9D9C814D7FF1C739B0B4456D615BA2B433088ACC7600F0000000000197\
6A914F4C49A12534308C42611374B72FBD3B5E8BD1D5488AC333CFB05000000001976A9148B7023\
22E67E5CA7F71D22AD2F03B92726E33B5388ACAD6C0100000000001976A914B9AD609D074976F3F\
6ACA03F8CDAB3AEB7E1D90F88AC91111000000000001976A9140A00B4349F4AF6F6CC0B14207718\
50EC59FAB18988AC5D600100000000001976A914D06457F39050AE8D42FB6FCEA73854FC4741B8B\
988AC74951600000000001976A914163B9FA8F5D03D8F74B9BEE183DBEFF163DFA3BF88ACDF924D\
00000000001976A91484A887FAD85272D9FD32AEF4BE03B845BFF2368388AC6C430F00000000001\
976A914DFD20CA2083D9365DFCF628D6722386BFC85AB7B88ACBCBF0700000000001976A9141EE7\
DE2F03F60B0DF683EF4B0BBF6FBFF1CB3FC588ACAFD19900000000001976A914AFBBDD8E6C172C8\
0A701607B782CFDA31F655C6188AC1E090300000000001976A9147830539DD1F88516D2F8AA3F86\
83C9A8AA38468388AC29DB8100000000001976A914F63BB9B910AD32EFE937E5206C697EF7F5FC1\
3B888AC9F740F00000000001976A914BB337FF4FE242B34BAD7FB0191E763A371CDA93488ACCD11\
9C00000000001976A9149A2F344BB40DA7B311C929A01D6F20F18EDF29AD88AC642714000000000\
01976A914498252EB9F4D6C7CC3D449A799DC6E79B8D8D8A788AC665E0A00000000001976A91492\
421A3D31E6D8F3F2B141DC7FEC11E92E751A5B88ACAACF0F00000000001976A914A64F1259E7CC0\
910D34B3943073EA3A7681D006388ACFA351800000000001976A914215535B094713887C968B2E0\
9491FB8AE883C6DA88AC232F1100000000001976A91445ABD4B464453DFC2E9ED130A61EA2159F6\
10B9488ACC99B0F00000000001976A91448C1B6A4F7A570762EDF3AE3AA847DF393E4BCE588AC41\
171F00000000001976A914673AED87F7D4E5168D39DC2FBE1A60D0E3A6409788AC8CA30F0000000\
0001976A91463F908E11F1EBC96BDD625514735D95D237F48A988AC1CB60F000000000017A91416\
043309E053379DFEA0295D07761C817C7EAB028719E30F00000000001976A914CF9B4C071F78143\
0A108F5CF1EF2A29E0E0FF01588AC97359B00000000001976A91467F70A5253CBADFD8CDBAABA7C\
FC2202CC1AC1D688AC3BD20100000000001976A91449969044080BFB65B7BBD804403CA5B8379F5\
A3988AC520F1000000000001976A9146B6BFCA34A7DCB2ADB8061A48C18FE18DA5EBC5988AC899D\
9E00000000001976A914F9021FFF5ED155BAB621ECFE7F6A411E4686DF4188ACC6A398000000000\
01976A9145819150F10E1B8F7CDBAD96A1525425C0B36CE0288ACC829B101000000001976A91464\
CD9D34C761B248919278E1EC7F6EC7970835EB88AC46800F00000000001976A91491F23EA158A00\
B5BB7F177E4BE676DA666A7D0BB88AC1F450F00000000001976A914590B57FF47EC68595CD3BEE2\
5BD8A4B2A048106288ACF7990F00000000001976A91486086F564AFFBEA4AFAD538CCE37FBDEE1B\
90C0C88ACC8704C00000000001976A914B883957EF4FE5380BA978CF09D2695EF018542FD88AC52\
C80F00000000001976A914C82192BDBFA222914614F62651C7523C7E861CBA88ACAF5A0F0000000\
0001976A9149006D2169BA7F93909AAA8F498ABCAF3A622AEE888AC5F4F0F00000000001976A914\
3C97FEBEEF86F956F100504FCEE72944B91FFE8988ACB1CC9800000000001976A9143307733C09A\
20EBE27157DC9F5D6DB945485223E88AC2E011000000000001976A9143518A6B615998B89AE86B8\
FBCF31ABF4A4AA202688AC2FED0600";

	testOneCryptoTransaction(hexStr5);




	const std::string hexStr6 = "\
0100000001DB99E72B0F0770BDBC6B580147597299B171D9B9898E32D1E769649D0DC8E605020A0\
0006B483045022052ADF1FB2CABF87B68ACBE316A7B3058B4D045D4CAA8EA2C016C0E7766CB4615\
0221008BA79AEF42ECDEDFDE983FBDD8CA4092C480A410AF9601C0EC7FBA42260A577001210276E\
F24540AE005C5F6F21B2B3B7124253386FDC9273DD43C789D5F61BF2D6836FFFFFFFFFD030C0100\
00000000000017A91400F00000DB52B66E6CF3AC21B4CDB88B1F66392087010000000000000017A\
91477F0F55EA565B617317AB5CA2711A9DAB8501A8587010000000000000017A9141255CACEDD46\
336E4C0E3E4AF3C6EA981289B00887010000000000000017A914DCC3B216AA5C7CF58B9E127E4DE\
5E4BE7E9258D987010000000000000017A914361293682EA4BCD3D29E76BF8CCC1FE088C297F187\
010000000000000017A914860F69EDFDD46FF1CA88F55A2C672C82110854FD87010000000000000\
017A91435EEDA674E4B78BC62A25C42E347EF6B5BA5A7A387010000000000000017A914407197B1\
D56877CC03BBDB15004F8C71521B47F187010000000000000017A914437228A4B17E6FD89D460FC\
DE231D8AF36A41A2A87010000000000000017A914BCE327B9060D9E438C290EFD0266D86055B34F\
7987010000000000000017A91494C3DF7D49CF0B496F2C22F431C89B10785B7E358701000000000\
0000017A914B13CCF51AC320D772539BCCF1B08214092E1896D87010000000000000017A9143327\
7820273B7C83FCCF407127D1A3E1C692E05687010000000000000017A91422B6C4591C4CB1458FB\
2BEF663880629B54A031287010000000000000017A914CF6DFBCF1C6A49CA887115E9F0BEF1B7A0\
55C82087010000000000000017A914F8F7984D64E7898AFB8F4AA7A6A0935483551ABB870100000\
00000000017A91485325EC70754196B9E092F8A331BB295057AF6DB87010000000000000017A914\
A325D9A417A1663B6E562B2B9D83762E2B61E8B987010000000000000017A9145ABC88398CB6092\
3FC36B3BC06E567661739F56887010000000000000017A914325DD77F1D75D5A1F5126869C8B5C3\
F1252FFC9987010000000000000017A9142F827FBC2F326C2916E57D047785733651414C4B87010\
000000000000017A914764F04829381CCE050919B7C231646EB84A5E17F87010000000000000017\
A914028D995F1437858FBF07B8C2E657873D558B892987010000000000000017A91499D7E95491F\
83081BF6D97FC6DF4EBD2EF25FD3E87010000000000000017A91431C52C77A2F7837E51A1A423C0\
A44E322DE63F5187010000000000000017A91430DE4982379690BEAC30B84AA086EA655D06842A8\
7010000000000000017A914F61689E8F39CFEDAD8C0C9165F51CF7D7245AF338701000000000000\
0017A91409887544107ED8212725B898E3E455B4A286BB9187010000000000000017A914A3CD038\
2C420932D9C18D8D107DC7D08BBEFB18287010000000000000017A914346005F0262AE1D88A2332\
AE4DF72B2511B1138587010000000000000017A91478252AA5E9D395BDF2BF194D2A0035B11BEBF\
A2487010000000000000017A91465022866745121E9D3D2C3EC6890AC7F2E68D082870100000000\
00000017A91435CEDCF2EA7204EE555E1D681ED329BF593038A787010000000000000017A914648\
ECAADA4346869C2AEF32941586F41EDBB4F0887010000000000000017A91476261E772DD5457C60\
96A6603853E3C9DC8A0AEA87010000000000000017A914C5F29FD075ADBD6D6A4AB9136278997BA\
81A833B87010000000000000017A9140C9EAC6918E2C2E639CFB8FDE63FB3A0FC86828D87010000\
000000000017A91405E68C3AA9D589279F1077615D7B9481003115D887010000000000000017A91\
4A22C5C95469E3D84ED9C9D267A3D6272B2ACA4ED87010000000000000017A914ED9117BE6BF4AA\
C0E4676F8A5FDF8462DDCA2CAA87010000000000000017A9147434C16E8C18D7F9EB5354802F283\
4F9C8D4131187010000000000000017A914E6EF6986AAE8FC8950449039DD881971705796D78701\
0000000000000017A914586D4FB5B2C9905C87B0AD87042B2B854C21C0428701000000000000001\
7A91484E16DD735E0F5E3B954827CAF698F3128E1ADB987010000000000000017A914586CCF5598\
B3373022D2242B5FCB79FBA9A45E6A87010000000000000017A914B2B1A056D71BA4EFD57848EE3\
68CB0723A5C1C4187010000000000000017A914839FC3A39A22B856F9B4C5277A40C4975C2E8F7E\
87010000000000000017A9144C19EF57982D5FBFFB04DED77BF7F5CAFC465633870100000000000\
00017A914C16976615B89467F373D3BB224696FFCB76F1A8F87010000000000000017A914DB0F5C\
0CDAC5BDD3AC194C62938F25885BD6539487010000000000000017A914C228DC95C9C3FE560260F\
71353051C47A43E928D87010000000000000017A9148819F2E83D581FD9B6C6EF71D3207CBA047E\
558587010000000000000017A9148A66EE4A3F17FCA846FA848C204F2F2C0BDE251987010000000\
000000017A91425A22EB0EE941A25F9E63E2D587BC3FAFD5BCD8287010000000000000017A914E4\
51830C57ADF506C52DD127E14FB78043AA383B87010000000000000017A9146A5ED4EA0596B1EA0\
640684C0629F0AA9B05A01387010000000000000017A914890CBD12EC8CDC81A9738C172EFE5704\
E88F29A187010000000000000017A9144D23D35A0DD0B8FCE588B4E40399C0739610A3D18701000\
0000000000017A914B6A16379DEDF41D4B980B4CBA3A0D325FBA58DA687010000000000000017A9\
14D42F22A7EE1F2EC96098DF5B4E557327C4077E6A87010000000000000017A91429A989A7E4D75\
EF3AF8E32820AB7B906435E25E787010000000000000017A9146076E7C73BBE8DA6E54C5CF9FC9B\
4FF885FA530187010000000000000017A914115519191AE0B16038D3CDDF2FF09D1746E1ECA2870\
10000000000000017A9142725F70021AE40D289B618033A48E4A502F3EB74870100000000000000\
17A9143C0C3D8E87B28A129101F1B1448161342075C76487010000000000000017A914CCDA4E517\
5C79C6FBC3E30F8DC98FDD48C67866087010000000000000017A914B1AC7052C52621AA6F274FD5\
E26167BE28A10BF187010000000000000017A914AF7424FDEB2C3C7FDE44934C6ECD782E4F8C5B0\
687010000000000000017A914C4337FCAD3C4838D66517400543CB85F58A116F787010000000000\
000017A9140BD3F611AB62F9DBCD92052E5D0C4A2E911B165887010000000000000017A9144A363\
F758F4DD8852C46CD32E959BBF66ACB989287010000000000000017A9146D05204AB1E6C24BBB3F\
6C732CCCEA08EA49AED387010000000000000017A9140D0EDFFD1653F3C8D8BF5DD247AA9FB2FEE\
0ADC887010000000000000017A914A34555DE30BD1070287EAF5BC545D04AF53214D18701000000\
0000000017A91472B0A283C79E342A1723822C4DB4C753FF14C15B87010000000000000017A9148\
28FC4134DAEF6FE3C06D77D5FD17B069A6A29CB87010000000000000017A91472D9B72111CD6A37\
50958ABEF7465D0416ADAC1187010000000000000017A914F83CDD3683B84F2E0C091915B628DEE\
46437C2BE87010000000000000017A914EB520DA71EAAC0158F515F6488310831C51FEBF6870100\
00000000000017A914106EFFD032C6EACD065FA6C68B8FA543619C28A587010000000000000017A\
914167AC36D642DC2D0DDC3C0D7A0BC5276F6D5DCBB87010000000000000017A914089A44E6773C\
949E8C043E13834DD43A7E5381D187010000000000000017A91467F2DF9021094C2428E5F7E03A6\
18967FB45BE6387010000000000000017A914BB51DC1974064BF0BB09A3B2FB670A0051EB79D987\
010000000000000017A9143CDC0125AE411C393C6A492A09A18E8FFC8553CE87010000000000000\
017A9145CFCB7819F9CD5A306F7E0A415F72B08460ACEBF87010000000000000017A914B27853AD\
71BDF2B68DED5D46E53584206F2D286A87010000000000000017A91439F4B879FFA1B4E83E3DAE0\
E5972E2E3AF56B3D787010000000000000017A914131DFAB4D1AE8DE5F27083987E186992649E79\
1F87010000000000000017A9146D871642BAD5AA4512583E7F554FA34D153ED3A38701000000000\
0000017A914036CCDBA6FD13D04B65FF021CA89CA31DFA3E7C087010000000000000017A914347F\
60DE15AB5AE86FDB4C789087506D4446650487010000000000000017A914E67EF024FA2AE03A148\
69864787B8DCE8385AB6987010000000000000017A914DBF37D7C619A5537DBFA153849836CADF3\
0E658987010000000000000017A914F6891031B8F136FD5A1193C66AD77C077B77A766870100000\
00000000017A914220977990257E73D530235D29DC0FF69D3DBB0C087010000000000000017A914\
585F1621CD0931356EC0B7F67AAAF7396A3D045487010000000000000017A914C4526B7E6FEA973\
F19A77AF89F91CBCBC08D96DC87010000000000000017A914A42E94E759CEB1D2DBBD1E60171502\
CD32CB878C87010000000000000017A91420637451E5A7F6A9C9FA642F565F20C450472F0A87010\
000000000000017A9144967591527104A1D41083ED8940AB3618ECF527A87010000000000000017\
A914669B105DBCA934DE7A8A6DE8C81E013A6767678A87010000000000000017A9146C8E2246609\
A69871CD75EE19B69EF2FA10574B387010000000000000017A914EE44EE64F0096DDE310D81B5AC\
9927897146B56387010000000000000017A9142388F39ACB365E5709CAF4BCD653CACABC6283098\
7010000000000000017A91466414324D7558CAFE342AB856C3D102D03E035478701000000000000\
0017A914088B5FB822A12E771F8C6DDC55A6C524FCDAAB0187010000000000000017A91442E7CFE\
663594A2C54EDA317E7CD1E8EC3CBF72887010000000000000017A914C241758090E1E3C5C1A1AB\
85BFC6C41951B999C587010000000000000017A914D45ABED87AF28BAEA369D83B264FDFD8B61EE\
D1787010000000000000017A9149B3378A601A933CB3C38ED25C00FC57CF7300A10870100000000\
00000017A914416E1B947A9C718496887D0D5C81EFB9EE08356A87010000000000000017A914ACD\
EB0A1AD288738F2D6637181517DFE4F79AFC887010000000000000017A914BDDBD349D2A0720F31\
DB8CDB2907A482B8AD2D3687010000000000000017A9147497877BC4F5A23CE6A9D9B6A27070884\
19F920D87010000000000000017A914EED9FA9DA17E5C034F22099758E26C698A2D7F0187010000\
000000000017A914D3DD93EE63DC5D040CD7F015B347818EFF5C360287010000000000000017A91\
46B259877919FF0AB865EC1370C164F54975BAD5187010000000000000017A9140D097602E15BE0\
A2EEC60995A186DF60B0AE993A87010000000000000017A914A2480B02B098B6973B20DD78B675A\
D5D4820867687010000000000000017A9148108ED88FAF507D59063A9D3608913ED4513C56F8701\
0000000000000017A914ED3E27EF1D26A3CB33C420C931606BE972C1F8148701000000000000001\
7A914742C48CDCF178209085D8FA3737D6D2C5A7D39A787010000000000000017A914C16802F17B\
ADB2C1F49A08E2ABD6828183D8C8D787010000000000000017A91480A2A1DF3C388D1E24A33F156\
2DAA63D93ECDB1C87010000000000000017A914D89359E096EB8F18BF83126FDE156B34DCEED440\
87010000000000000017A9148BC654191BD7AA0AEBE060B0DC503E35D6ABC23D870100000000000\
00017A91473B504950A8487E7E1A44A170EAB20AC32BA5D4687010000000000000017A9141FBBC3\
F979AFB3E0AE47ACF19F7B6B1C54F73D4B87010000000000000017A9145F834F78CEC676EB15FDC\
5E2397DA8F8FB4CD0B887010000000000000017A914F849B29B6CEC3450B36C2FD0E945C0345977\
D47D87010000000000000017A914EAB687C471F4ABFF92687937D077200D9527BA0587010000000\
000000017A914D0355E3E4350CF143E26149C4104E34DCE089EB487010000000000000017A91459\
B494D2E214DE7731E03AF6DABB28606E2FDB7A87010000000000000017A914D32ABB581BF8DAB2E\
8BEC0252A86BC319BFCC06087010000000000000017A914CDE37ADE697D483913E79D972F5E1D94\
77095CF087010000000000000017A9146303F0A63638E6D8D1A910EF902941FCF5C2C8548701000\
0000000000017A9148631EC4E372197F4E87F7E1D38D008033887ABD387010000000000000017A9\
140267F200ABF981BDB40E8DF9A3A5039AA97DEFE587010000000000000017A9140EF1AC2FB0EC1\
A1F166806D13C0CC547F0D6CE0B87010000000000000017A914F54C53420F6BFFC45D4944B981C0\
C4A24D9FDBD387010000000000000017A914435942E1D73C2DDC868BDDF86CA69B2A58299F9B870\
10000000000000017A91442FF11E0642EE4F9830C9C3A78507B663D89361E870100000000000000\
17A9147B75D53D06D65A2B683A435D7F367B3623B0C2BC87010000000000000017A9148093DED4F\
EB75E8825E08003ACEEF810CBE13C8987010000000000000017A91478577FF650A627496A193D2A\
D573EE0F49E122F387010000000000000017A9148D4F7207F39754238DBB0F6939AA6929E248DDB\
087010000000000000017A914C644AE900DBC992C93001FE061148F665FCE2D6587010000000000\
000017A9141EE6FC1879619C1004EE1A30BD723FE27FA882AF87010000000000000017A9144D638\
AEAB56CC4574F2826208BB83DA305A9261A87010000000000000017A9140C333717E3E2C08C36FC\
260ED576E1C8AABDD86287010000000000000017A91429CC9938D88C24C0DAC72715FA64AA82332\
D060687010000000000000017A9142A989DEC87E611F7FFFDB981A89E5EC9E8D5FA018701000000\
0000000017A914B81994AB2D2165437F12E67018F229894496F80487010000000000000017A914C\
E7EA9194AF33A7C0F23D0B86A4D644EC455B59887010000000000000017A914CC01CE9B0A5A5079\
834A4C0FB50F8B988DB5D09987010000000000000017A914B9EE2C3CC90295CEBCA9BD45864B3DD\
9AB1A4A0787010000000000000017A914806C18BAFFEDB95BDE1E23520840F0CAB3ACDD72870100\
00000000000017A914EF7F3AF20534C44F5B55CFC0F468C6BEAA9DA4B487010000000000000017A\
914A24B36365E1FC22848188AD4A5427D8654A3BBCA87010000000000000017A914A45B356D723A\
6683144AF2630048107285174DF187010000000000000017A9147B6E3F2952472DCA30624D78180\
91545D6B16A5387010000000000000017A9143927DFD41C69B14D49F2477A6297ECA0FEC458CD87\
010000000000000017A91452B4670540BEC2AF88A2466A0AD791C9D0B86B9987010000000000000\
017A914C99F4BC649B2389C0918DE131311F90758D117E087010000000000000017A914E0CC4CDB\
B001AE93D48AEE2766C7AB46001F623987010000000000000017A914D998E0B0BC717957C88AC9E\
E68B17F95E03E71B687010000000000000017A914124D6508A78B6105D0CF07472A85E366A21715\
BA87010000000000000017A9149A1E64306270DE5E2C726374D5B130F232E7F0C48701000000000\
0000017A914016A8AC960BEEFA0C8670D48F5E9CF4894F907D087010000000000000017A9147C40\
4E92E7A67B6CEF77234BD488CD5AA699607A87010000000000000017A91433FAC59C852167569DC\
E4169CCE84771EF50343987010000000000000017A91404F8B26FAD3984D5C991B8ABA8E3B42B79\
3C93C387010000000000000017A914B3F07D40E3AB8174EC747723964DBABBCD5619F8870100000\
00000000017A91473E09423D2D8870CDAAC197A0CAAFD2B4907907487010000000000000017A914\
0060B3031A91DB3C34B82B941527C098803C33AD87010000000000000017A914DF895373096F2D0\
BC7723BD28FF3EF64A74E2A7D87010000000000000017A914D3CF1A453BA9B2D814A23E637D3A74\
6C8B18F33F87010000000000000017A914CE768730C8D154CEAEA208111253FD15E402B2EF87010\
000000000000017A914D353CC312827449118ED27ADB909A172AA5EB24B87010000000000000017\
A9141C8F5E7FD28F2E68C2120187E195300CEF1A3E5587010000000000000017A914CC93CB2E571\
A4195A1A98BABF62D26A17AA4D7D587010000000000000017A9149519F4EA31EBE483FE60CCA4F0\
82DC849259E32787010000000000000017A9143A9BA658C3F1F16D43266A3FB84644410A35A1648\
7010000000000000017A914D4553E6F4CCC7EDED529B75E510D4CAFDBD2A4988701000000000000\
0017A9144D407BE435AAA9F36B8EFEDD740A6539A49927A287010000000000000017A91421DBED7\
17085EB9E4840695474029B857754B96C87010000000000000017A9148375AD523FA2A5EB5BCD26\
CA5E99960A99A9727187010000000000000017A9142203731E82FEB1B26BDB4911ADD186DB6AF8D\
3A887010000000000000017A914A7C80FA8F47C40F8BF60A916EA720A351211BD6B870100000000\
00000017A91478052580C19384CC181BA7BCFE48A3F05AF13DB287010000000000000017A9140E2\
563422F7806082EBE3C5B2B948DFDEF042D4787010000000000000017A9145FB52E16CCA6592850\
28172EE5D979F75DB4F1DD87010000000000000017A914CFCDDA7D12223B4B351A26A217E94AE3A\
90A1ADF87010000000000000017A914F1C747DBD0F2A8C82FC50024FED828A2282E204E87010000\
000000000017A914534D9CC04609316D5F647AA281AC53E786977F2087010000000000000017A91\
4696DC3C457DD305C65B07A9F1278C092103423C887010000000000000017A914F74A69E47A662D\
332A1D5B1795ACF3AD0F53BCA387010000000000000017A914D03DA927FB192D29CAB33C49BE092\
A8A1A9C4C5187010000000000000017A914D4907044E9ACA5D7A989BCBC7F53BF108A5ACE1E8701\
0000000000000017A91445D3024C31F96C76FD64D61580A39BEDE048101C8701000000000000001\
7A9140EB1E3D0AA251B44F0DB42403841F3732B64E56687010000000000000017A9147F4B8F5636\
D594BA09AE5E3D4CA6DDB80D4EA92A87010000000000000017A91421B541B9130D6919173CB7FC6\
8F412FC0F4A6BEF87010000000000000017A9147A8F6E1D03853599E6B967865B3835171248A509\
87010000000000000017A9140CA3E66636EEAC4EFDA3D0FE83EBC1432287886B870100000000000\
00017A914D5ED4640178D38FE1FD16F4269CC081B8B95B93F87010000000000000017A914DECBA9\
3446553ECA252D3ADA343061615E4723F987010000000000000017A9143552009A1F213153769F2\
422F427E1DF681CED4F87010000000000000017A9149F753DE02EC9CD6021110DC74788C0AE3B5A\
44D687010000000000000017A914A6E370DBCCF2900D5D07AFD186D56DF65105854987010000000\
000000017A9144FEEE18A2E20FD865531139097EDE8FA2B7C107C87010000000000000017A914FA\
F8BFCC24D358F3CA22892A653702AD2BD39A8787010000000000000017A9144829DDAAC7F6E77DD\
9CC516B7FEF35EE7852137787010000000000000017A914136A9A68CD77101DFA90D259E88098F0\
98FAF89787010000000000000017A91448F50067442AF2027BFB0ECA46A65194D072D27C8701000\
0000000000017A91434189F13EA5EEB31CD14EA52F617EF50DD8E777E87010000000000000017A9\
14064A1850F49CF06F44C8C7ADB4CB86A5FCE6640F87010000000000000017A914A84DB898D2DC0\
D3157EECF9574D17B8781865B8987010000000000000017A914BAEE4E8CC26E2538B5581C5DA0F0\
D9A4BB30EF0087010000000000000017A9149E4FD082F5DDFFA9B409FA664685210707974F17870\
10000000000000017A91400E5D3F0457D2C0179A9DB3334657622DA7BD2A7870100000000000000\
17A9142A81862979F295E9D4CB6B286D8C8D897643686387010000000000000017A914536F0C194\
D64DBC19DA47D475E28174CD32D520887010000000000000017A91430F717EAF1CB45AFD485BFA6\
8FAC0A3783FC18DC87010000000000000017A91403156430E62664AE8B4D6EE72FC0BB2F988899A\
887010000000000000017A914BD6AEB9DD48D29B47A4C1918AA285B2DB81B12D287010000000000\
000017A9148F018A1C82B78698D569F286DBBC3F152AED99A287010000000000000017A9147471E\
00BEF930A6F4642C0A5E00428B4976C5AE587010000000000000017A9145AC4C6A84946DD5A2D0E\
CA15D48B374E34A0346D87010000000000000017A914963960FCE400E1F6147DE838B4C1D9130E0\
B694A87010000000000000017A9142B6DC2D41EA68016AC8BF0FFCF64FECB16D4E0D08701000000\
0000000017A9148AD6853FE08AB5BFDBC952672B54C840063F6EAA87010000000000000017A914C\
A750F776661FB7D6CA417E5692097C76E4627A587010000000000000017A914A895368C7B588790\
6AC6D4126101354DEE61E81087010000000000000017A9145EF74EFB72EDE1391AF386C706A7FAC\
2BE2D322687010000000000000017A914986761A07352EA829D18A2EADBAA6A48D03628F3870100\
00000000000017A91459931346AA7E3F708F1A6B6B8E0C4F75675A675787010000000000000017A\
9142B23F348971D140E25258DDE0EA3BCBB121741BA87010000000000000017A914FF5083717F60\
297CF44B542B2030D385DF2F3CEA87010000000000000017A9141482DB343D5465D9AC07F492807\
7FA5C4790E19987010000000000000017A9144A80A6743D708452A5AB9DC6795BBCCEF32BD32F87\
010000000000000017A9145E1CCAC3DCD808D53C6B24438B7CE5E002DD12B687010000000000000\
017A914675DC040805D2079980ADAD1BFDB40065A0A3DEB87010000000000000017A914C1F6BE0C\
5C29BAF6E087E8B2E124820A4995027387010000000000000017A91442E8DD31345430566D2A98E\
0560BD3CB6B7574D287010000000000000017A914F129DFC98AD6E291A3CEDAB1AA5AAA9DC5E1E9\
AB87010000000000000017A914A6138B7CC7B9B42A83819860636509E87E357D6C8701000000000\
0000017A9148D6A4CEF49C51835AB52EFE6753A90BB9AFAA56A87010000000000000017A91469A5\
E7A44B557EC520AD456943945D01DF4641C487010000000000000017A91496371C75DD941810B8C\
CD16652831FF1D968125387010000000000000017A91461CA58787DE0BF1434FA06F69640688593\
D8BF2587010000000000000017A914D24B3883026A92343B8A03F33E592D663451AA02870100000\
00000000017A914DE5B1D966ABBB9BD57E6CE328358A1E57E0B1EBE87010000000000000017A914\
BEC0EB28234BA9C9CE18BEE761A1033FB5AB434F87010000000000000017A9140B744BC7801B6EA\
F1D7E81E39CA8812550F6DAEE87010000000000000017A9144E88D8EC84511E086E97AFA7A22365\
BA03F3AC0A87010000000000000017A9147874075D65997EFE470C415DC18B2E721ABBE14687010\
000000000000017A91429EFA1C44A127B42779981B3785F845607BE607887010000000000000017\
A91409217C5671427F058DD69F668FC5224DDED5071587010000000000000017A914708ED1216CA\
4802D03202A7B23106494C42E921C87010000000000000017A9148400A7389766B76B2C8B4A6127\
DBCF5AEFFB443187010000000000000017A91424C99FDC1EF25B2E4FBA58A866D5550B217CB9648\
7010000000000000017A914E35653B79A939524BB4CFAD3465C5B97C3AAF8458701000000000000\
0017A914543841DAA1387BF5A77C9DF07735211C2D87E39087010000000000000017A9145722DDA\
ACE89FB390693CDD2C699A197847EEE8387010000000000000017A9141CCA9AFEBEB76D3E3DE1B7\
484CDE8BFDD90BC68B87010000000000000017A9140C999A01CE6BFA7E35C9215CABA48D4F9E063\
3F787010000000000000017A91441C5944D20C3FBA175E01421B5EC7C04A3BE1511870100000000\
00000017A9140FBEC0C91D5E07C99641DD3D182D1C5D286533E687010000000000000017A914304\
7C34760E7A27A7CC7E103B9DEE76B30DEEC5587010000000000000017A914F6397B6F8932AEA4DE\
E0B7E8B65209AFE603AD0F87010000000000000017A914A0005DF47524080BD06FF0AF969D2BEF5\
DD61EAD87010000000000000017A91406586342CD04F7F441D2B19296062C9D6C05386F87010000\
000000000017A9141E03666852AF26F209D195FD75732420734B7F2587010000000000000017A91\
40C750A12391C11BCEC776BCC9418140CC8BFC49587010000000000000017A91429F308D299F919\
27C0DAC0E54187003FDB872C4D87010000000000000017A91403AC6AE2D1160AB26C12D7374B4B3\
586A8C2806187010000000000000017A9145A2E896094E2B595F72EE7DAB373D73D4A939F618701\
0000000000000017A9145273D395CE68DF1D0B1BB98F3CDAE7612B3095908701000000000000001\
7A91420B6F2CA0534313C0D4D2322332881B8E42B812D87010000000000000017A91408B24B9539\
22462F4B51D6F696471470432694B987010000000000000017A9148EBA36B92F5CC0446117D9147\
76105269D45740187010000000000000017A914BCDFCE9F2365AD9293F8E43310CD7197FA7E5C8F\
87010000000000000017A914B9368563004729A2EF97ADEAFA0ED8E26BE75D10870100000000000\
00017A9147F0C7A015C080FE0E87F4C284D5FF65208A7F25E87010000000000000017A91474A5D4\
976CAE9CD3B116A58E1C0C3DFB933C252087010000000000000017A914CB527895F4347237474D7\
EBFA5AAFB6BF9CC72B687010000000000000017A9149C0FBD67146C6814393172DE274380994556\
CF4D87010000000000000017A914ABBF1ECE52E7958946F1910B9D08341859526D0A87010000000\
000000017A91446713EB69A399A7197B00C73822FC9527B6A065187010000000000000017A9141C\
2B6AA0956A3D305B93CC3B9023629DD44C91C287010000000000000017A9140675572DF77FA3E06\
BE93B449E6DD4DB716AA83C87010000000000000017A914820BD0970B8A33C5D592AE50E2C58D9A\
34B222C687010000000000000017A914BCFA48B3EC56CF2F1942B8A2B7C2E277D01CBC418701000\
0000000000017A9143EC0E0A552F7E2149CEA917C88A55C1CBEE04F6C87010000000000000017A9\
145BFDFF6D290B8E23D7B75C656E159A943178E79B87010000000000000017A914DD60AE72B1E4F\
E13A26BC21035C1453A4B03D41987010000000000000017A914DEAB7E44E59DE03F839E02C1F122\
DB1528C25FAD87010000000000000017A9142C0151C7F09398DFFE745EC2F2F1DF7313DE2735870\
10000000000000017A9141187F386E18257202013D65ADE6BC7F949433315870100000000000000\
17A914801234949211996F13F5FB10AC0FD2A8154374E787010000000000000017A91458DAD8329\
0A57045F27889FCFFF34E807F86044887010000000000000017A914EC5FF81ABE39CED068CDA988\
367DB03F9F78927487010000000000000017A914A6705EE67D057C729EB181566A8E455F7342482\
387010000000000000017A914220356FAD20524E9A6A1B664BBE9A7C8A394875E87010000000000\
000017A9147D327ACA58CCC5818718EA4C9047799BB062986087010000000000000017A9143D65F\
9F07A0C07ECD9CC465497169D14E47E05A987010000000000000017A914D43657BF46A983BEC05C\
7AC79221B88B32599F8A87010000000000000017A914C5FAD0FD8F5DC85B431B9BB6E01F913460B\
6A25D87010000000000000017A9146A92D8B186B414C96E446AAC0253E9310A9F5AA78701000000\
0000000017A91454178DF151A62CF90C0507EEBEEABE3C536847A187010000000000000017A9147\
E9774DC5EA2AE0A9D92AEE3C25A31BE5FC4091987010000000000000017A914DB098B6B3FD5B85D\
54D3CD0CAB93ED96B0959AD187010000000000000017A9142154483983CA01C0E852F77090108D4\
F9A56D0F087010000000000000017A914E2CB9B05E1EB8532177C95F467B17A9C6795BD89870100\
00000000000017A914F1AF868B2B8E13A6CCF42B38D2408C8A51CAA0A387010000000000000017A\
9149CC3ABE9C05E306B84FA870431DAE1CB6DF36F3387010000000000000017A9143B14423F987E\
C3660321B8EB2D865B9527BA22E187010000000000000017A9145DA8002C09F90F3F67861B34C60\
5B997E7D89D2A87010000000000000017A914D10F0567A646706DE2BEC9B8B09E610E6B11BCAD87\
010000000000000017A914B3DC1A5E1B361FE7C591A8661916418E5187A05087010000000000000\
017A914C0987AD4D208B7D8EB312FD060FE730404E1D6EF87010000000000000017A914E9044C1B\
4C82235BF89D609E1AB302E78FC3336B87010000000000000017A9142295A2D75525520B9C52576\
E9DAAA8BC757F6BF687010000000000000017A914A1EE0A770B51ED6F2DD8539BB5FC56BFA2EE6A\
0B87010000000000000017A91405E17CA32912A332484170278943F032B846B1078701000000000\
0000017A9148F6F31536F6EF1BE91B48E9D89143D46D16E1B2A87010000000000000017A914B4BE\
F8C90FA64EA2437AD1D349C4476FA8D0755287010000000000000017A914FF731C8E00420522EAD\
4D420C9417E3B7204FAAE87010000000000000017A914E378344D51097FDF6A68AC47242A4C95C1\
118FE787010000000000000017A91431F95855FDD15FB0CBDC8615D81A2D93DD611CDD870100000\
00000000017A914047DDFAA1EBA5CEEF00E1D9AFBE70CE03FB84F6287010000000000000017A914\
3CD61B7B5FB6F7C3BA6643E8A9BAA626ABE4966F87010000000000000017A9141A9C4CCA571C63D\
5C867BED6AD8CF26F3D74B61087010000000000000017A91451AA21452D04EC8D4E7FD54DCE450C\
A411CFAF7487010000000000000017A9143EC45F97466F5563E905735DEF401CDFBF7134FC87010\
000000000000017A9146A5CDFA90F741D9499CB521D8B8F1841B4DABAEF87010000000000000017\
A9145ADF159EC902D962F706087856F6B3DA4B0605D687010000000000000017A91486376192F21\
EBBBE7985D0A71EB542D785B20BD587010000000000000017A914E527A78E2460052976E7495663\
4F57AB95DC4F7E87010000000000000017A9149A44781B4F5BD50F01EDBF24705E087149E1BD048\
7010000000000000017A914B9407E27985408CAC350BC494323DC2FA40573058701000000000000\
0017A91477C94113960B1A7D0DEEF717909056B59F836AFE87010000000000000017A9145E2DB0D\
A9F661697E14D450C127B181019235D7E87010000000000000017A91435E3B8CB315822067ACE08\
7621BBFE42CCE3675A87010000000000000017A914B7D36E3F2F89C47DA68984FEEE2D8431298DA\
3D387010000000000000017A914C301AC0461E2D2B3A02753839DD6A62788AEEBA7870100000000\
00000017A9149392096212C665B485CFF842192CAF55654EB8F287010000000000000017A914DFA\
0A4613E19DF1FF1E8F7262847377D9D51EAF487010000000000000017A914617220519D3A218AF9\
717718150C3411DE06622D87010000000000000017A91450E108F2FDAC13595FE0B9747252E10BD\
F6BCBAC87010000000000000017A91452D4EA7FE0D4F6F35B75C695C3F7A714E9B9ECEE87010000\
000000000017A914F0E70F4EF1A058DDE59C78666C521965551E603087010000000000000017A91\
412AE6FEC795A438E6B83EDB28BD038CFA3416F1487010000000000000017A914984488A71EC052\
2B6AEAA8EE5F575EA8E1114E6887010000000000000017A91423EE004E6B73C434862B5B79E7E70\
E2F4EDD852B87010000000000000017A9144F0DEBBE864431FAFEB3B9B9E76A2ECB2492E9318701\
0000000000000017A914B47E0FEFE4278B4629CC69AED8BC4D7A9EF53A4A8701000000000000001\
7A9142DEEA6674592DCFA435D213C47CED3BFF793774C87010000000000000017A9142CB9F835FC\
024E0F531AB6A24EAA4424AFBECCAE87010000000000000017A914539C7CF977AE9CFA681394F96\
6C1A6E773C792C287010000000000000017A9148FD4245A5B2D245F9B15A7E408342A0920C0CC34\
87010000000000000017A91434FB0FB90E3EBE5F24FF6162C13E7F7F9BC720E7870100000000000\
00017A914BB890EADC52C5CBED93BDBFB1D1AA119C4AC49DC87010000000000000017A914637988\
C69930976F07CAD569805C4CF3A1B87EB087010000000000000017A914283AD9B98A9C83EE84369\
4C31D1B002C0FF939D387010000000000000017A91498BE7CDFF21AF3D5636D77CF303D876E82BD\
59CF87010000000000000017A91473E24548ABB862F52AC6610358E053071BAD7EAC87010000000\
000000017A914E7A5DADB36AF5A9939CEEA69A0BDD3E1B921B86487010000000000000017A91413\
91EBEAC168AAD46BC41A7ED06B3E8DDD7DB2E787010000000000000017A91457ACAFAC7265F425C\
04B589ECEB1A0395B14C3EC87010000000000000017A9147D50D635737988C186BE2F80AC4FC3B9\
05562DC087010000000000000017A914AAEF610A84BECE52B73BD22DF2F27141B3CEC41D8701000\
0000000000017A914E35B51C15B7C225260A60418C9A12854A595844287010000000000000017A9\
148099CF5E472F75A9DF53F0C51ED6EFA86E061A5D87010000000000000017A91420F44B14A3752\
2AEF61277C168DF1F8EEC444CBE87010000000000000017A914E68381786906BEC7B6E7B634FB6A\
DF672E3735D987010000000000000017A914479BF4A8487B28933745DECB7B6A0C8F69ABE89C870\
10000000000000017A9145048276D9822CF33611F9B3322CB4B56EBA8DED7870100000000000000\
17A914ABAFD7FF63C27818094B0B109E705CE8C1A8F94C87010000000000000017A914F8F1426A9\
8F9632CE919D83560129BCDADA1C7B487010000000000000017A914A9D70A0B556ACFD41F0BFF77\
E1613C72A570E87C87010000000000000017A9144FA6B93247E162A7FB9DF500225E5540D152387\
A87010000000000000017A914CCECFE4C3F9B89B238853757FAC2433E4245863A87010000000000\
000017A914E5CD47B27270BA47CED964963B51E14F9A1FBF7C87010000000000000017A9144AF41\
AD6C1511ACF75F9C8616293F1E1C095744887010000000000000017A91406D58A65B5C34A1DA45D\
796D2165FA7D4F52B17587010000000000000017A914EB3DE117D417CD2A1B67736B6FA482F4384\
3148B87010000000000000017A914BFB8194B47B0862F364E9A3E1AD94D501DF60E0C8701000000\
0000000017A914273A06A53636E2AE68BDB0ACD5D03D8D231C8CA487010000000000000017A914C\
9282949C6394C528136B1334BFE914F1D1919A587010000000000000017A914D19F9A4BF5F87B51\
E4577FB6504390A1B94211BF87010000000000000017A914626741FF0951DFE42408576B4FF23B0\
A090D5A6F87010000000000000017A914C6E5AF04AF1258DF8B95164100772E242E95C6EE870100\
00000000000017A914669F65D2CCE8363219D69819206A8A3DD3A5B57087010000000000000017A\
914493D7B8E8F54DA022DA1F5EF5815D572DC25213787010000000000000017A914D7ACAA1A389F\
CCF637908B32DBCEB64EC1BA7E1387010000000000000017A914FD02951DDF551FCAA8905141C55\
BC6D74428065187010000000000000017A914C26E76D64870698DF6FD8236D0B48F7CAA9EB0FC87\
010000000000000017A91441636DC702E51CD0CB50582D9D0B434B754BBE4F87010000000000000\
017A91480D83C330602E8E18E42C15ABCAF065AC3A7B5F487010000000000000017A91477E4649E\
65D3F78F5729D1F3EDCDDDC2624E06FD87010000000000000017A9147157F7240CBCBFB36C47788\
289E2910CC13CDFC287010000000000000017A914317C338F6C12DDC86EE94AC526EBA46EAC26BA\
FC87010000000000000017A91408554EB1E9B137F1634B4FBC80CAF0D41062979F8701000000000\
0000017A914D10DC0C9F3F3705FE3C00C3DCBA6979D6D896BD987010000000000000017A9144201\
DE623AC47F7DF116ACAB55C6AF3767FFF78B87010000000000000017A91490E503C698219893627\
3148F35C85CB5D1EB543887010000000000000017A914D7421A07FECD3278E1EF32F0C34924E827\
9D4F1987010000000000000017A9147F478C2F4EF31251FD80F850383A1EEEC68A71A9870100000\
00000000017A9148F94E6D782246731E4DA02B17A985D6D5F3A302387010000000000000017A914\
1E9293AAF49E64BAE05E9105375103706C296E1B87010000000000000017A91429D2CC6F02A0DCB\
9F90105D3D7EA122D2872C8F587010000000000000017A9143EF2FD5826FFA31E8CAAF51A72356B\
A97B2457D087010000000000000017A914592E7D16FD6E195682DAB4BBF4A2D82ABCCB282D87010\
000000000000017A9146735B96FE02B26BC7E882EFE7995412DDBD06A3587010000000000000017\
A914CCFB71C887B01F65D7DAC8F9C2A58C2B448A0D7787010000000000000017A9149774AC693F2\
6D9105FC2A36FC2A82AB101875CF787010000000000000017A914EE850F9066423C85A2558D328C\
D6E39AA6A3A5B287010000000000000017A9149B3E3EBD9A5D33021D834E51888B81D7343E38C68\
7010000000000000017A914CDDBDA5FEC31F1A1D017A5E5C29CC6E162BBB61D8701000000000000\
0017A91421FDAEE46A61EB5F940F9953F456C2D058F71FBC87010000000000000017A914C496592\
C468FDA01ADD54EA98438AC9F6669E6A587010000000000000017A914CC08D42AD28E8DE6AFAB79\
B025AAA1C9CB8560CC87010000000000000017A914806C4191B7C80F40402C0ED9D08E111F06444\
33B87010000000000000017A914DBEF64826AACAD46B28573E3FFFDB5BA47D7AC76870100000000\
00000017A91431FA6F837F282E583D3A9B8C0022FDD4C908ABF687010000000000000017A914F8C\
23D5EF83573613EFF4A46F86BB084C1C545F487010000000000000017A914F025A413C43A4DAB49\
869E2E4E9B31DF5E99BEB387010000000000000017A914CDF2A520B0298A592F6B8C968885DA6E4\
C9BFDA487010000000000000017A9140D1EC73D39148F3406676BC62170247B1FF3BE7587010000\
000000000017A9142C59AF3FA3724FBC2A2B9342692964E0B81C83A687010000000000000017A91\
4E08832F05E469A8DE5AD88F976CA0534580DECBB87010000000000000017A9142C0380553E92A5\
FE5DA57FC7A557EF525776739687010000000000000017A914F402E8807B20A7F1DC90055BF6FB8\
1C3DE291E2387010000000000000017A9149E03B98E1B22D658ED71B274416FA4F0169EC3BB8701\
0000000000000017A914270249739ACF07401FBA6F69790FF28216A691F78701000000000000001\
7A9144CB26BC83A5521D37320CCA3EB0957EABD55059E87010000000000000017A914222C7DC99D\
92F2828662AF906A332BEB1A392C3487010000000000000017A914CE4804A6BCD462B43B2AF148B\
94D43049B41766D87010000000000000017A914B168305D91BCF501285D2DDF3EACCE95488820C0\
87010000000000000017A914E748EE3E15C63825BBC431B5832F155C913A1328870100000000000\
00017A9148EA522E0F6DBF39839C4BCD92296476950C5018387010000000000000017A91474AA13\
93DD551DBBF26296202151C62B3B83533587010000000000000017A9140FD20E66B98811D51F77A\
0E06205D95D8143BA4287010000000000000017A91460C8740B3BAA62AA895BA678D5489F6B15A6\
218487010000000000000017A914279EC29DAE5D6D059CE73F4A30CC9F62DF1F148D87010000000\
000000017A9143B601263E644A61FBC3394BEE837EF5780D02ABE87010000000000000017A91487\
102808B5F4E77D769F348BA5A26808DD73EFA487010000000000000017A914F45A910D02857DE73\
207DE01DC8DD1F34085B5D987010000000000000017A91492FF91D2459D34B7849E2921DCCA1D0A\
5516600087010000000000000017A9142EDD074CA2819840AF8E0F977E6E448DDE8326998701000\
0000000000017A9146532F83556909D6EB58783E2CF6EC1A3A2D52D9387010000000000000017A9\
145FEB61D4B371585E70081D39169B95B84F076AD887010000000000000017A914E161646B923E8\
95DBB893DF809BDF980ED717DA787010000000000000017A914A7972746FFAD9227C9202197BD2B\
7216D781150E87010000000000000017A91407E326E65DD97CA8F7A17541C29D2D7549AA6FE9870\
10000000000000017A914909CD066E865768F1033BFDF4F6DDE2D4FB8F4F8870100000000000000\
17A914E8312477AF621B3D3538C9B9655863A7EC62185A87010000000000000017A9145E8BD03E3\
3C072446119A96D7DB34476BF92162E87010000000000000017A91469BEB3267D65CCEED5EF9A1A\
AE3DA01367D6D8E887010000000000000017A914C7068FB6375D56CEE8B6BA7F192A90565221246\
387010000000000000017A914F3B180CF2384852E444EAD7DA6E82EC656FB05C787010000000000\
000017A914393025E30200051540D06FC899D939A85920CA0087010000000000000017A9140D4B3\
BEA13D263442F525A6BDAA1898E17F3C0E487010000000000000017A91407ED117376D3062C9608\
956DA1D4EF5A818F1CD187010000000000000017A914967FCCB93BD94E0E2CCBF3AE7471B5F12DB\
4511D87010000000000000017A914619DD2563C7F94787D113B3B1333DCFC008CA7938701000000\
0000000017A9148EB0C511F33CB7191694B7FC114CF8B92F10157A87010000000000000017A9140\
E7081E0307404B7FC6BAF8E425FFC48387016BA87010000000000000017A914E77745AD33838ADD\
3FA8D459684B8C547559C1A487010000000000000017A91422B9F04FDAC651AE727FCB3333184D6\
CDB219E0487010000000000000017A9148F3F763CE87A2CFB118ED67AB80B23F1266FF2AC870100\
00000000000017A914A9963BEA7E8E9D25092585A16F7A55ACE2A5A26C87010000000000000017A\
914AA1316A3B41E5DD0DD8B639C584095A3CC7E0B8287010000000000000017A9141F9169B273BD\
2D8666021250DEBD39313EBF83BC87010000000000000017A91480C50C7E6BB54267A9DF044E6A0\
AF435C14530BD87010000000000000017A91477BB35444C18A3CEDE000DCB6C4CAB3FF46881EB87\
010000000000000017A914BF03272272F36DF8493E23906E4DC243D92F269F87010000000000000\
017A914F39C574FECC8DDD993D3E85F647AD833400A9A7B87010000000000000017A91491953F3F\
DCFD565453D26C297A858068B6521B4687010000000000000017A914016EEFD7196BE0322281A5D\
6256E66377280506487010000000000000017A91486D5319EE50E2F6A5B7D7BF15AB94183F8D911\
1C87010000000000000017A914551719E5A7B2D6A21E4C631BE9DE92DF072193E28701000000000\
0000017A914E8B3ED661E9F56D83E763CC843356B0ADEC5760A87010000000000000017A9148A45\
90F91EA015E314CD361987BD4DB8D2BE6C0187010000000000000017A9144FA6DB63B7802B0D86B\
97225B55CDFE52874040087010000000000000017A914D8E26DBD18FC2FBE681CDDD2F99441C9C0\
D4A6A187010000000000000017A914E911426A38F0D453D99251E79635724DAB6A68D7870100000\
00000000017A914C601190C35A41002E20AD2D057ACB5BC77B6CAC487010000000000000017A914\
847347826D4E00EAC300A883FDEB33D2EA3171DC87010000000000000017A914C901C63FD5A734C\
F180BDF8C46B932CBAB90D7B087010000000000000017A914E786E52A1F5633329C5B1E6E2CD3F6\
D4A1F5F12E87010000000000000017A914D3D416F841B0DCDB8C72B85FCAB8166354DB0ABE87010\
000000000000017A91436A050E2310B561DD99AD011C1C093A2D2FAEBAB87010000000000000017\
A9140BB41AFCD7FBC2D9B4F01AAEB784BA845120CFE687010000000000000017A91466ECDFC771B\
365DCC1A2ABB8FDC2D4C0E20849E487010000000000000017A9144C8F68F15429B67FF2AD701BCC\
B469FDF807CCC087010000000000000017A9144DC5E52E5BC66C0A97EFCC6E8AF42345378AE1B88\
7010000000000000017A9148E41EC0CBC2CD0E675B1F863AE6798945381B50F8701000000000000\
0017A9146808180AB37257D78F6EEDBD9FA10FE02A53A29A87010000000000000017A91431866BA\
B7BCC51184FE1EA3F5A4340713B4A9B1587010000000000000017A914D36BE3D61C387E832DB82A\
8FFF3EC1A838421DB987010000000000000017A914704EAC5F13ED5025622AC52B71916EE3991A3\
45B87010000000000000017A914878EFB2E28251B5738EA0346813236731C79953C870100000000\
00000017A914804A1D816CA8FA4CDC2B3CDF1C90EBB05D18500987010000000000000017A9141E1\
CEC80B75C18FEBA63ED725AC9BBC5407C246587010000000000000017A91450401779B97F46D8D2\
AF4ECB5EA2741E02351F2A87010000000000000017A914D9D8DC9FC8239E6CED3B3E1DA121AC96A\
103D83E87010000000000000017A91491571327F8E9A7954E41B0622E9526DC569FCD6A87010000\
000000000017A9141C2CD71FE0A925F631EFF7FFC1E59F47D9049AD187010000000000000017A91\
4BB4433CC84AD1194552D757DBDD14579FBC79BC587010000000000000017A914C5ED9E0BCEE449\
208F91625DA4A2D160321F011E87010000000000000017A914F6B24D46CC7CA23EC7A1F8010F62E\
0406073734487010000000000000017A914E67D7CC364795906B43445ADAA4B54C7A11895DA8701\
0000000000000017A914FC40E13C747F9E9FB02FFAF9A4E8E51C0ED065B98701000000000000001\
7A9147053C9226F10328DA2F1FF9FAB104F156B692BC187010000000000000017A914735BB69E77\
2A08DBFBFC7AF6357D38B2AA24B90887010000000000000017A914E7DF267793A044C8ABC9A60FA\
B87A7137889B21A87010000000000000017A9142E51543A7F419073CE6CED421DCFA65CDA52B29E\
87010000000000000017A914BD5A1A47DA62E65E617474AAB1B119E09EA9ED3F870100000000000\
00017A914FA0E405358851D8CD35024A27B4C220A3B962E7487010000000000000017A914F43480\
456680D12FB87A1345B46BBA1B887F97E487010000000000000017A914DA3D8C0AE13461F226CEE\
D941A46F56889B090CB87010000000000000017A9144D3E9239E8F1193752EEBC9DF9E032B64355\
88C787010000000000000017A914AE54F91E48C80F1BBD1D466BDE07F0DDAD324E1787010000000\
000000017A9140040E32330FD80D01F7BE6373A1272BAEE1242EE87010000000000000017A91439\
3AAD605A78960C55F1AB67C204C1B5E563794387010000000000000017A914B506FECDAD42B1C87\
CA124EE72CCFAFA2BD8159187010000000000000017A914C272747D10BE47FDF01B1EF630357E1E\
76BDB04587010000000000000017A914735543C69536CF57E5A8C34C8D17E3616D5627DB8701000\
0000000000017A9140C62B4744E0C935773B4859EF0FCA539A2C6125A87010000000000000017A9\
14F3E2384FC36FD2FF2CB0F62276E7BC0987CF1EA887010000000000000017A914CFF2BF0FE3C5B\
C29F11D83882195A619E903536687010000000000000017A91469256E299D0FA2C1550F527F0AE7\
D24096BF0EE287010000000000000017A9143116E1BDFE077C0B351CA6800CD63A3056367605870\
10000000000000017A9144C78904642EF87DCC2613322391C1721F061DDED870100000000000000\
17A9145DCF18254574E78D6A1ED6EE74DD224BA31CB29487010000000000000017A914D15E62FB6\
0B320E816CDE25FCE01B4A73DA78D3487010000000000000017A914D6FFF3520D8D4AF968F5AB84\
1F110E8ED8E0D74687010000000000000017A9145C53DD8DDC7756A18C420F322D7321CACA511FC\
987010000000000000017A914249B5C9C59EBA13CF6D1C010A42C4F9C370130F887010000000000\
000017A9140DB8FC80BE91FA7863AAE463B393EDC31217018E87010000000000000017A914AD220\
72ACF17934086D3E7C2468ADF7707DB595A87010000000000000017A9148989C128BBF25F8E74DB\
3EC62685BF7A246D2E3F87010000000000000017A91465ECF803094DB9A48A3ADC0CAFEE6A7FDD2\
4330087010000000000000017A9144ABD276C22FC80AECC057FC8CEF016E75CD0CE338701000000\
0000000017A914AD9223ECEDF715C6E5EA1ED63DEE886C3DC016C987010000000000000017A9145\
8978BE226EFFDC90DD496288753ED8FD907193C87010000000000000017A9147AE77847F1B60543\
9F506CF8F27E80392153735787010000000000000017A91456FD682F6C76CAD8D3625DC17276AA2\
2DBD5431D87010000000000000017A91486489F74C089BEBB0301958BB3815D97D44F46D8870100\
00000000000017A914A0D4F677919432BB2CEC80495E3C3196C8BAE66287010000000000000017A\
91438FCB47B4CB819826AF7FDBA5059651F84E4CAFC87010000000000000017A9143D0038AE5EF8\
6C6254E97343B2642D27B5EB001887010000000000000017A914F625F9C5A77FDB83978428740FD\
D4F48E82CA6AC87010000000000000017A914FE726BB4E3F5CEB2790DA88ACCC538A8D08A8F9B87\
010000000000000017A9140F9CE33813AAC28A53CFE1B492B6BA2ECE32B5BF87010000000000000\
017A91402373F6AFB7362CAD3C8428697DA57C70310D9F087010000000000000017A9149AE9EE70\
115C5D0870B38A6514D5BC1D1B268F2C87010000000000000017A91400352D4CD65BB548DA1CA00\
5CF4AC5D150ECCCDE87010000000000000017A914972A76A9A3615B40274ED54D0182A5A2828DEF\
9587010000000000000017A914E14358181C5956EE8B6C33DD20FAEF84968D4F4C8701000000000\
0000017A9144AFE6B615094082335E7086988E38861B0AEE2E087010000000000000017A91427DC\
7B8A57B593856C0AB84D8B949962D04D8B2187010000000000000017A9149953B9715216ECF3B11\
AB3991693332FC04B976A87010000000000000017A91453CE79B6816B01AB5C86BA21DB1674B38B\
C22B8487010000000000000017A9142DD9EBCE75E8103B79CA41D9B8EAD3B3C895286F870100000\
00000000017A914BD4328C6D42D02A6EE813ECE8510AC97140FD50887010000000000000017A914\
959D411EB3D168FEFFB16CBBD8E5EC4841C7D0DF87010000000000000017A914E6D0098B178E57D\
46A393C3DAE91BCA444C9C8F587010000000000000017A9145E1D0EEEF7DCF85DD546164806B209\
16F53B688487010000000000000017A9140D26393315CDCD4C240801C85CEFD2948C4DDFEE87010\
000000000000017A914390493B0B509C02716122A8F422B25096D60F4A187010000000000000017\
A91443F86E3AA414480237D7CA68CA49A3395BCADF9287010000000000000017A914A52577A27B4\
4ABA838152FE899EDB25E3DDF437C87010000000000000017A9148C471A025376FA3BF8FFB8D977\
398A79FF7A327A87010000000000000017A914C58E2AEA7701CCC37B4EAA0ED267F589ADD805778\
7010000000000000017A91401B0EC0E7E0CB869862055129C6E4E257FEA2C498701000000000000\
0017A91429291CF0334994EC69322B882F6E60700047C6AF87010000000000000017A914DD89695\
7981F87E7879EBB3BEDB211C661E49A5087010000000000000017A914C60A2F7DB398968B43C765\
BEEAD29CF5C366E3D387010000000000000017A914B9DA32E3C2C68281EF8AB6B6BC6554E89F4E6\
4C887010000000000000017A91427E7385BE76B241FEF1DC1DC1B4489589CCC5667870100000000\
00000017A914B34556284F6CFB0A526C5AEAF7C1DA4E1E02B8AE87010000000000000017A9145F3\
2F012D8E18E4DE34E9002A7DDA317B44B743687010000000000000017A914FC87F44F0375C50805\
E1CC43F3F6E2324DEC0CB487010000000000000017A9149C62D78D8DF44896D5A314BFD9105A205\
206E6BF87010000000000000017A91463A9894C383D9DE9261496D9F2AA9974312EAF6F87010000\
000000000017A914A779447A5A947B8A68C0F6EA7DC7182B7BD6E3BB87010000000000000017A91\
46AFEC44EEE324346FCD66B2A96DEA46CFDB1587B87010000000000000017A9141762BF743F75EF\
795C700CDDEE152EC8B421530B87010000000000000017A91426440C3AAE4AF43263721F8B56F11\
EA34FA86D1B87010000000000000017A914F839C55EEF4D9DEDBC4D938F633E0E06B4DF1CEB8701\
0000000000000017A914BD7E977424031C493B91759EE78A407A2032357C8701000000000000001\
7A9140DCF08AEE10D5B8ADA5B48A7CACFBFDAB510212987010000000000000017A914D194F417AF\
692AEF0791647198EA17AB4D315F3F87010000000000000017A91448F72542B2E193EA029E70C0F\
10BFE985AC80CCB87010000000000000017A9147E323F26F9B86BBD445E72DBF8A562CA4B3267E2\
87010000000000000017A91415C2C9D227818C04E54365260D6B42C2F77F159C870100000000000\
00017A9141FB30E549FAD9454CFD90A4B598F9359F54F498F87010000000000000017A9142E5847\
00C657C3BD1F0335F40F8AF866D335802687010000000000000017A9147D3F49C36CAAC5BAB737F\
141A2C3C256F3A6826A87010000000000000017A9147DE7A0B0AB0C0075009107F97F4019DB41BB\
A98487010000000000000017A91472B83714C077F278D9DAE0110270058B62D92B0B87010000000\
000000017A9144A15C1474B7777931A5E0BB9C353DE5B9C657B5187010000000000000017A91474\
ADEFA3FF430F344A2E30920C0F43DBD7B393E287010000000000000017A914B57F0327F902D8652\
D80036B3D89497C90803E7987010000000000000017A914E6C62A5DE5C7C16D5A4E5BB186A4D8B1\
9450B9FD87010000000000000017A91407A2660363769F93C2A9F0FD7332BA0FFFA27EBA8701000\
0000000000017A91471F27A9430272A35C096DD644D50E3142C1ED11987010000000000000017A9\
1446B331F35EDC6CFBC0553A4476F56CB666A5B2BD87010000000000000017A914DD2D92720C7A1\
437AC59C73D604DA6EE5922AF6887010000000000000017A9142D25F541E2AB1C93CC0EFA8F954C\
955E5CEB8FC587010000000000000017A914FDFA9B09E279CED5324F16AC33560FD3EA1B7706870\
10000000000000017A914BCF1010C63B94B6E050873857F5587D3805EAE57870100000000000000\
17A914677F21C19AE64E2A7A5BC204F554FD2F97C6353687010000000000000017A914D3AF75956\
EED15B509A518F9BD096AB3E0B272A687010000000000000017A9141914A7C800F9176C8B240798\
8E6C1DA3849630A387010000000000000017A914D4D06666D63C9C0DA88A570DCAE7126C50B6824\
687010000000000000017A91467B8E82ED70D26D94A53CC1FDE54E541BBFE8A4987010000000000\
000017A914C2346FCDFE0358A819731D1E12CF5EAD2A0538BC87010000000000000017A914B492A\
BE76F484FA2C0284039D955C08084271F7C87010000000000000017A914B75AC1133CDD4DEB0FA8\
687981871DEE98D58CF587010000000000000017A914299918E6DD26D24D2E73B9B5F1956835A9D\
66B6787010000000000000017A91483274D3CAADAE807F6206F318DA6BD0FFCBBDCA78701000000\
0000000017A91455D425E464932696F6BB10758A57EADE9D881F6687010000000000000017A914D\
D2FAD013965A4C277456AE6FEB10166C243FCBB87010000000000000017A914C357B8C2339D134B\
5ACB0F6EF5D1148CC4E7F7FA87010000000000000017A914049DC7F4B742B6787F455641CD246BE\
846B15EA387010000000000000017A91442FEC3B13373FF9E9B7A709162C3A2A23FB6EDEC870100\
00000000000017A914176E0B9DD5F5B2FD2271263BC1BFA43B5C6DADD487010000000000000017A\
914DFF9C4BE61DB3C44B603F25685EBB89FB913BD6A87010000000000000017A91406218FD89F5E\
8CAC678B1F5C0DEB825CB1FC83A287010000000000000017A9144DC3805B4CB06D685CD6C5BBE21\
D6DF69BF5D1FC87010000000000000017A91486509181FDD93D986B1FD69EB496ADCC10516D2B87\
010000000000000017A9147300A6B398B4867E6C291EF322F1538B61C50AE687010000000000000\
017A914B6250A58601F34D7A2D33FA26A2EF2CBD909377E87010000000000000017A914E057022F\
E842371721CDC7097A9859EA83664A4A87010000000000000017A914F94B5E6895CA5016B51FA0B\
4282A80571F84717787010000000000000017A9148BE3125AF3131C5213C7329A523337249C7A17\
E387010000000000000017A9146B97F3C02F0B131AA71B93D0DBC3F4EAF65FC2EC8701000000000\
0000017A914E7473650319FD1B98CFE7AA2EC012391502CB3BA87010000000000000017A914A21C\
0A00347B3178BAEA4BCA77D9EE52F6743F1B87010000000000000017A914B2495BEB217DBCA705A\
BCDC2A73586D03AC809E287010000000000000017A9141CB3051189B6E954E20FFE131FFC25CD08\
D5011987010000000000000017A9149AD9C657ADA0FD3C119280679DF924666A06832D870100000\
00000000017A9145F0C44BFA9FD6DB86C122F551F093CDB2C6C047D87010000000000000017A914\
19E97468B2E9DB4207FC5C552DF536AB6CB8C6B487010000000000000017A91477BF3285EB23BD3\
75731984D518D8F8FC49C5E1487010000000000000017A914844B6DE976919AB70A79778FB85B1A\
A0FD6F1B9B87010000000000000017A914FA77F61AEC64F67B8DC38AEA6F3DA1E11A089DCA87010\
000000000000017A914A3993D7412950A5D7C7B37CA773B9A084C069A5387010000000000000017\
A91450A708C38BAAF6CC186ED92E7537BC9FFCA5B8D887010000000000000017A914B4894D04D88\
4F7DD169026CF52F63858C895602E87010000000000000017A9140586D78099EAB1F71E881DBE1E\
57823DD98CE65087010000000000000017A914985382DE66BB3AA4432D3282EF6E8FC7EED2945A8\
7010000000000000017A914DC90A69B45EE9D9DEA2B4A0F141E40BB75BF8B078701000000000000\
0017A9141635FCFD47DC94F8070655C7377DC317E71F96EE87010000000000000017A914DFF303D\
DD9AA0F96A2C67167AEA6F126EC16F6AF87010000000000000017A914E9B0C6A1DA45C73AB11AA0\
0CB74F46027A8BDD6D87010000000000000017A914C9BFAEEE7B6C23E891AE3DE2E8227F89ABD03\
0A387010000000000000017A914B5B3127BC19DD8E52738846738341E903B4F6586870100000000\
00000017A91410E4B81B1E43E9B42F4DE2BF8F245A9B280DB9D187010000000000000017A914637\
F18A62CACCDDF376D339E22B248F56ACFF42587010000000000000017A9147B66DFC69967936964\
11003BCFE1108BEB64A21487010000000000000017A9141F5CB5F746BC140361AF56C9AECA0994B\
1FF84CF87010000000000000017A914C1428AD17A3B4084A6F5231EF2555A43FE0DA2D787010000\
000000000017A91426C2B11EB31D62A09350378B3AE1C5447B971E3687010000000000000017A91\
439BF58341047617BF861067DAAB54926D157CB5887010000000000000017A914F4AEAC2DD6CA48\
545D3628D5475E8775D7F918A587010000000000000017A91426A649FB38858A7AD7D4E82481740\
A9430FCDAB487010000000000000017A91498478DD8E89D889C5E21157A4AF25747FC5664BD8701\
0000000000000017A914ECD54AD9D085797932DE76E4562172578F27D0AA8701000000000000001\
7A914628EFF48260E62EDAA1010D076492764719BF73287010000000000000017A9148F7CA99C57\
17ECD770224ABC64F8157934FA5DC587010000000000000017A9141DEB5D52CE0913E11F5D00856\
2756D0E1C2CBF2E87010000000000000017A914DC598440AE402C9ACEA767CED3D4CF1FB1F67455\
87010000000000000017A9148D05574794BBE2E5494F279FC5B88717F04698AB870100000000000\
00017A914B43D7CEA03D9C3903C9C1C73B064907AB5D1383C87010000000000000017A914F5E61C\
9F55A54AB8BAD745B2757C1F8FE86BE62387010000000000000017A9145AA73F4CD711790862A32\
09681BC86C58E1A363087010000000000000017A914F47944F092E7B76FADC27419F5919078F40A\
72B487010000000000000017A9148F64878D08550E334232E7861AD39C837D14D3F887010000000\
000000017A914A0C95F346C06D0E953AA63F1C39C77A23EEB1AE487010000000000000017A9149C\
10CE5CD389CF4570396F2CEF0CC9DF93CC58A987010000000000000017A91491A6DF967F6ABDBFE\
CECE916DF544125238B4C1A87010000000000000017A914B6216C9C596E1CA31D871A1A9FED8264\
005DA54D87010000000000000017A914C898E72622FC6D11428725279B04C5E23DD4E0458701000\
0000000000017A9143CCE016B3DC2FE70DAF27575B852EE4D012F773887010000000000000017A9\
149C9CD1B05636D41C02415555AB6C055C3433E30E87010000000000000017A91400A9CCF4ECBC9\
712ABA6384C429961CEC91F8A9B87010000000000000017A914772E2FC85C45384B4A2BE8C14298\
47DCFF2E01AC87010000000000000017A91487497DA6943991691D24063D5C6E6D2EBC0057E8870\
10000000000000017A914C98003B75BA82A39E6371B44A3883B9F42C5CF53870100000000000000\
17A9144770DC88BFA37700D9554E4D033DA3BBCA84E07D87010000000000000017A9144070E60FA\
63AB12A4D69268252F34E224642AEDC87010000000000000017A914E870CFACE9BE75F37FB006DE\
888BF4B43BE42D9187010000000000000017A914A42A95FC9073090BED642B12FE698A14BADDA90\
A87010000000000000017A9146EE4AE2AE5423482435461BDA5C874561E46C95887010000000000\
000017A914AC645D69283DB639CD18A549FC2BA1071C68CFE887010000000000000017A91427984\
9EF7E05E35B9B8353736A74C8901E46A2CB87010000000000000017A914A7FCFB7FDBBBFF4AA9AB\
0203E62F9DC1E2D0328987010000000000000017A9140D22C242B2A3AF8851D78DD73D29740F3FB\
1DD9687010000000000000017A91400DB426954E2AFD7D29BCE6757724047D90A45808701000000\
0000000017A914ACDCBA160AA8219B9A80BCE20284E8B79591F11E87010000000000000017A9141\
96094A0165ED4B0986E2B88B2FD3B134F578B0287010000000000000017A9148DC4F5BB008E3830\
D652998FDE07A9B9C252376F87010000000000000017A9148F1CEDDD6994F55FE0AB606E03B19CA\
11AD8719987010000000000000017A91480770AEA069CA076431C58F40D27EB88B4A707F0870100\
00000000000017A9146D45A0121E79E76946C1E430D098DAE81CBC8D9B87010000000000000017A\
9142974852C0EE8D5B5DB8A098FDAA9AC679A32A15C87010000000000000017A9141F52A8D5ADB5\
FAB947E536EB7DD754AEACEDB0ED87010000000000000017A91450014B0F602B638F4542D3781EB\
8F098ED6624B987010000000000000017A914ADD2048E3ECFB296BC0709B0BE1BCDB3287CDA6387\
010000000000000017A9143E012862D10F6B18E4C3D17E61912A2EE977249C87010000000000000\
017A9142093D4320622FC02A862C8D06005C59D035CE6A687010000000000000017A914A6FED149\
348D8A926D72B12C90A4E271CB4CBB1387010000000000000017A914E0B670FACE3F20CCB1EFBDB\
F8C259A576C02099B87010000000000000017A914505103B7B589EC16F7912DC0E111B3433F6639\
A787010000000000000017A914D6395CBC5A2A96F06F66A852F71178F88046A6E88701000000000\
0000017A914A9354CB349C2DCAABB4FA986EC43C2E4B32B6ABF87010000000000000017A9142B14\
E9F392F6CB62302C1F7615882A01BB56499187010000000000000017A914157DB21AD7AE4F002C4\
AC05942E4D146014FFD1087010000000000000017A9143444E4CDA2260547D2D312E3EC1BC98AD7\
A9C1F487010000000000000017A9142C8EF903648B25440232A29D740222FCD60FA171870100000\
00000000017A91489EE5F7EA74B6F869FCD9A0729C082BC81E9FA6F87010000000000000017A914\
2EEB433BE91F931F59BEA49DB52C29003EA6BA8187010000000000000017A91413301C25FA1DBA4\
2A94E1043BE02C664460B019F87010000000000000017A914E227929B9D9EC58783DDD58F143AB4\
2AB2ABC4A187010000000000000017A9146D9D0E38147B9409712AEBB251F45CD79E3E444B87010\
000000000000017A914795AAD517C993A3504F5626A827AA5B96D67615C87010000000000000017\
A91457208E1B55B219E2C06C307F977BB555EC161E1887010000000000000017A914CC57F999819\
9B85A6EB864E0EDDA4FC4A2C79D7F87010000000000000017A914131A8A44B327ED107822F62C04\
15C21F96D25BF087010000000000000017A9149F92D3037A52FFCADFDD578CA5911938B21C935E8\
7010000000000000017A914D675A2C9D2024233C1813A3F35DB9F65E92E3A378701000000000000\
0017A914EF8A5A00FFC03487284B1B4AF81C7C9C30FD26B387010000000000000017A914910A5B0\
F9B8E4F6E75AC68606083F37437B4E45B87010000000000000017A914085D65334C6E94CF3CCDC9\
32327B3F9B19BBB54987010000000000000017A91429217EA6CCD0ED4DDF9AD93F430218A2F8536\
CE787010000000000000017A914950FF737F548D00D0F02FFE25B302FCF2E8F0AA5870100000000\
00000017A914B2990F0695A84B05BADBFF83AFEDD995B9F6AA7387010000000000000017A914C2F\
99C2D5715E588B962B15CE5825A799EE9848487010000000000000017A914B32ADB32C4A1C0077C\
A171B04965101AD586599A87010000000000000017A91462DFDD90AB1A021BE642DB87CFE715172\
39F24CF87010000000000000017A9148D89D4F95A27E70FBBE1F9BCFFFE3FC329C7BFC287010000\
000000000017A91478BF6146E0C663010DC6B3F199A2DF03B32E0E6287010000000000000017A91\
4B343E5BC34E95FAD19E03AFA2F97C613FB0EAC8087010000000000000017A9146CE67446DD3B73\
3D768B162A5732FAB05588B22D87010000000000000017A914A5B9B9050DC2116ECA50CEF9DCD43\
E01C003440887010000000000000017A9140EA7E024FFF3683C93989A3E2C6F322C0D386AC18701\
0000000000000017A914CABF032F915E26AAB904CB6AE60FD2DC7015713E8701000000000000001\
7A914FE86315B98E6663EB9BFC970A22E7C920344182487010000000000000017A91401EF2EC177\
3A27C3A35E90E029AEF805258C1D7087010000000000000017A914ED2AD0B5000186157DCEC5D80\
140953FE7447FC187010000000000000017A914F13E99BAE6AB0976472079E4245B3412306B6350\
87010000000000000017A914D00D6FFD8B3FF68B3CF29BFEC488E86B8AF70C27870100000000000\
00017A914047B3C18198C3F8FE7CBE0BF9DE2D41C3BD7F99B87010000000000000017A9143090EE\
6692374506A0EDD505376DE6042536646C87010000000000000017A91418802952ADC2AC4B7F9FD\
0423DC5872F22244F6A87010000000000000017A91449076DD406E56B9EC40EED497BA04ED614BF\
BD2187010000000000000017A914938CF0ED17153E03956461D3E8874109E7AB64EF87010000000\
000000017A914FE858A1A8C39E93D6B906035206E81ED8EEB356587010000000000000017A91415\
EF45EB38629C01B4432E2FF5026FEF82FD58BE87010000000000000017A9145B7F9193EFDAF5378\
AD478947893C61F000B645587010000000000000017A914B4566DA9F228B5EBFF408D86C3DD81BA\
E8FF5A3287010000000000000017A914E4A0CD68E86302C7A879052F9D3A7DE92D0921FC8701000\
0000000000017A914FFA9B420BCEE17B1C79E34E807FC9A3CEA79D1A287010000000000000017A9\
14FB47C0C79571D52B0A2C0C49DC42B71D26B00EDB87010000000000000017A91434FB3EDB3BA52\
2EBF8F99CDDBDB35039BD9E941887010000000000000017A9145D51DC8BD30C7A339F5DFAF16019\
3CAA169F72CD87010000000000000017A914F946C42710C47016815925CB7B14791C68EC6CCA870\
10000000000000017A914FC6A323F2BA555659FDD3CC6598138927D973030870100000000000000\
17A914227E1232DA047AD649018E87F933CEE6ADCFE3B587010000000000000017A914FDA9990BE\
4844143DD20F9E2DFB9A6E124B067D687010000000000000017A9141630D9BFA62B74A28FDCD360\
E3ADA342546F59A787010000000000000017A9147D956916A217BA195E5F019E066F4CA45EA11A7\
387010000000000000017A9142CBF757807D87310A781FA5C5AE1B9FF50D378F087010000000000\
000017A91404FA5EEF690C752F4E7C16A1A5EFC4452076C0E787010000000000000017A914B9CB6\
4F67643ECF7B752ECD8C5258A93E7ED323587010000000000000017A9145136EB203CE26356FAF5\
788454FBEAA4F0DC9F9987010000000000000017A9143F8723145731052339D0FF9F24232C71A68\
0EF2087010000000000000017A9143756A9E16015487D77C0BA63F8F3E31F3703BDFB8701000000\
0000000017A91453270F69485E07D0D9E7B6C45AC760DA2971BF3987010000000000000017A9144\
92EE85C3143C9096862B55B7E8748F6C97C674E87010000000000000017A914C0614A45D8664F15\
44004B694D54402494DC981F87010000000000000017A9142D098B4BCDE838E6DB992A84BBBC71F\
20E0190ED87010000000000000017A914402EA7A9CDA2F2ED34805B0CDE10E37A8E4AFD3C870100\
00000000000017A9140180BE4F03E1BDFBB04C48FADFDDEAC83A72BC6E87010000000000000017A\
914E95E21CA80CDCE73671965060FC09B8B0BB1938287010000000000000017A9146DF89BFC393F\
02E3D341E3FECB4F3C326970E91887010000000000000017A9145841819A93E34238F5160FCB75A\
CEE10D37D429D87010000000000000017A9145E66622802F2E02133FC252CB98365A654BE434C87\
010000000000000017A914B7BD4FDB79218AB9EF6F50DEEFAC0EE14514101C87010000000000000\
017A914A868426D7097D40FE8675175BDCC71029BE7BDEF87010000000000000017A9142D5E5999\
FFB5D459B63E1EF9394ACEB8EDA1984F87010000000000000017A914A7FE14A4966B9E3F5E33238\
EEF857252373D2F7687010000000000000017A91456F2CF1864B7722BD062DEFB80B4C3EB609C80\
D987010000000000000017A914B52B4752EF50FA28B934663B2C0CD26DF341E1508701000000000\
0000017A914C0F1F926B35B3F8D0CBB3D05486BA3BCCDB6ED3F87010000000000000017A914EEC9\
5B7A031411B2A82DC75D1254C95B41EED61587010000000000000017A9145E2D4DDC0F50D6A5452\
1783B4D8B6E341381B6EE87010000000000000017A9147450BE97D296BB77BFD9B90C15C7EC437D\
8FF18787010000000000000017A914DD3111C79A4BDB7D3FA461386A99E93393985845870100000\
00000000017A9146D63E58EF8D177AB36650FDAE61B5D01194A3CF987010000000000000017A914\
F89448D91BDE950F17913CFC96E8662E2250F20387010000000000000017A914767280552CEFB5B\
22D229BC543076A656EA7D73587010000000000000017A914BA0EFBB2BDBA27E81D329D77623794\
6B64C0CD3587010000000000000017A914085EAE3BBE4506318D9CF388E6C545BAE982374D87010\
000000000000017A9143318F3189EEEEBA13E4C1DAF4C1CF67B11CE95D687010000000000000017\
A914ECD4ADB4552A446BB0A0C216A4060398348CA50987010000000000000017A91403AEEF14B81\
9968C939508B9040803D2D18249EF87010000000000000017A9140AEF24652EE99AB7E73A2696C1\
4C96D12B6900FB87010000000000000017A914314F55590BCA5672E5B8CB56EB271941D5E915668\
7010000000000000017A9144FEC536E3B1DB0909942ABAC656EE3669AC0E60C8701000000000000\
0017A91400423E9F20AFE3F2DEF58598175A78AD38B3EEA787010000000000000017A91492878D8\
4A79249F6B4F052D3F2FC22D83A9AE4E487010000000000000017A91462B9088FC028A7CE315A64\
222ECA6EEB8A2D4EBC87010000000000000017A91469C5623278E83742CC5F3D9E228AF4C24317A\
6BA87010000000000000017A9141FB0B178E95E50B74B7E189453761613CAEBE3E8870100000000\
00000017A91476E696B4367A41D1B558F12833975BE19385E55787010000000000000017A9148DF\
8161C73C4D796057BA4FEB27E2B4796BBBC3D87010000000000000017A9142D347FF111EB061C01\
D27A0E008568438EAFF13C87010000000000000017A914C8462F47309E7150923B4D5EA7D0BE08D\
E3FE73D87010000000000000017A91412D294F9A4EE6722533B817EF3765A4508F3280587010000\
000000000017A914D4D5F034B4D40E45A228501A326B79A4F42543DE87010000000000000017A91\
4062F7E5033C0CA8A86888B8E57305FC10020AB9987010000000000000017A914A5759A1A2E2767\
E3FF23E2A2D1D02EB926ECF18087010000000000000017A914526B7BE8E797FB647E4B753E2FF42\
00B9E594F8287010000000000000017A9149C46CA60BCF291C1AE73F1C985B83AB472924B8D8701\
0000000000000017A91418B6D1471A05191E3C9B4010771FC01E1F381AB28701000000000000001\
7A91478F16BADADB41B597441A6B02F2EBB6C60ACD66F87010000000000000017A914304C4562F5\
6335B4BC5157DAA9F5AF61EBDFB6C187010000000000000017A914FC0549182C7E200BC49142772\
44FDFC82F4E004387010000000000000017A91456036807588D2268A33CA97399C84978425289A8\
87010000000000000017A91407C88E7F582485E4834A1EDDC0DE1803A11010AC870100000000000\
00017A9147C8AA00274B1B7A3B5A8860601EBEB548EE3285B87010000000000000017A914EBE1B6\
53CEDC752319E3D24CF545EE4A343A97C387010000000000000017A9147819A8407780FC972C7C6\
D44DCD460664F27003A87010000000000000017A914FCB383BF17E695E28BDF4F633AA3007E016C\
64B787010000000000000017A914A9D5DE77FB9DCF59CF0CE2DA7F75CF56F8E227CB87010000000\
000000017A914B8B8135AF6277450EBF282B6802823F31EA1DD3687010000000000000017A914E2\
589547F08671A97AC340874AB60ED329D9A69187010000000000000017A91488CD1BB13B834D364\
0F5FD03083F81B0A247E38787010000000000000017A914400B77689F7DD516C0A224FB573B7B53\
3DB29C7187010000000000000017A91486BD5C90DE2E8A0FF85CABBEF8E67239CF4364018701000\
0000000000017A914C1EC427CEF4E9FFA98BD9312496F0F1B9D8CE06087010000000000000017A9\
14009CFC4AE2EF30E3AA40130FE62F82D0CC69C8AD87010000000000000017A9149C91F148CD5ED\
CFCB6FAE0A2F6A1F49A82BCE71687010000000000000017A914C9E862AB082F2BF57CB8E00B9E26\
31EDC43B34F787010000000000000017A9146979096BA107A42A6FC7AA61AE40D8A3947810B9870\
10000000000000017A914EFAE1B9A0937615E226C8F3124C0AFBD8E1668D7870100000000000000\
17A914CF265559B8FAA9901C98E71F4300AA0EA7AFD1A287010000000000000017A914976801CF3\
F5774FB1727D66172C5EB54A23B768F87010000000000000017A9148087726CE7B3B25EEAAB0594\
1CFC37F4878E460587010000000000000017A9145319D57AD4C95AB1C588E2896E7D2C40F62F5F9\
B87010000000000000017A914601D6A2264D5B710A1B4858ADDB2EBD45F282EF087010000000000\
000017A914C5446C03419F5C97C2A4BA712F6AD48F8F36D3C787010000000000000017A91415D48\
AFDC4A4896D9D42A54BA70F9236933E516E87010000000000000017A91404DD56F06FA8084FF21D\
880DE616D510E207230487010000000000000017A914EB49EA61FEA50DEEBD73318DE7EADEE8673\
B9FD487010000000000000017A914BD766650C7CF0CE27B6C3E6B0F3BE5579AFD75DE8701000000\
0000000017A914CFD58B8135EC82ECD2558A3A9815EBFB4D1EA62087010000000000000017A9149\
D33B3A27DB66643DEC558C683A8172451D4E06387010000000000000017A91418833C2845ECD3AD\
C6E58ED01FBD2A5048D416B687010000000000000017A9149378051DD716049B8AF18F02E4BA5D6\
5E8140F2A87010000000000000017A91484052795C906E57600C39B5C10E29B3714A7768E870100\
00000000000017A9147BD8FD724D6067785418BBE9BC296B517E4427AF87010000000000000017A\
9144ED590B0A930452916B9D109E65B296D9B17A6F787010000000000000017A914CD8CFE60CE7E\
F6156E378C7159EC9FED21F6AB8B87010000000000000017A914787CCCDFF6BED398179BFBA0421\
1425ECD2B598887010000000000000017A9148CA678F52C2FFC7E5014FD2ED37CEC1A8F7F5B6687\
010000000000000017A91466F53BF7DA699661D9C09516219869D4C0B04DAE87010000000000000\
017A914C97ACF8A312513E6A9A74DCF7AD4BC2298733B4C87010000000000000017A91437CC18C5\
CAC53D646F5513D5ECCD07458338453687010000000000000017A9147CB8016E174E0F1EA995B5F\
AA8C3E8AEACBD198F87010000000000000017A91488E9259DB4568738E7317066CDED6E6C40D74D\
B287010000000000000017A91443CBE22EC94858C7F60D3CA8529F12E1439585228701000000000\
0000017A91471E2512E9336967F12E3BE90E64FA8957352FED987010000000000000017A914980C\
ED8E4A0C8681703193F4755E3A8014E52B0387010000000000000017A914E5E27981ABCAE35EC0B\
3592DCF3E748D764D6F2087010000000000000017A9141FB5E6566692867579981ACCD3432D2C2B\
AEE45C87010000000000000017A914D732DFE91B5D8AE16695CEE3591F408CF8104292870100000\
00000000017A9145032681EEE114E8FC233F93E2F7D173ED66027A387010000000000000017A914\
17430E1804B12F1930E6B8790FA91651F6EDAE2487010000000000000017A914BDC6A6E709A18BC\
23AE6C3FD8FFB1680D0BC150387010000000000000017A9143E13D9A60A64778F876E4CE467DAE5\
7A33804BA487010000000000000017A9144B67FDBB67E1A5E574A08F9713A70402BE6D37C787010\
000000000000017A914AAAF8C6618CFBF952514AB85CC2BF7A86E206E6187010000000000000017\
A914141F8431BC76B25C0000A0DEF2C6B4F658B4F8DF87010000000000000017A914EDAC5A2C40A\
6EC36ED02FAED9594FD20DE24FAC687010000000000000017A914A038AF1F8717D8B269C5B6F425\
43505F5423950387010000000000000017A9146B96463E962A31B5C6874E1D3896E763D3F03B318\
7010000000000000017A91429DC7BBBEE95F7146219ED3EF227C292F107D1648701000000000000\
0017A914141576203B501E4C16AE680F5E3D7538BBABCDE187010000000000000017A914B2D24E6\
A56D8DBF5AD6F6E0A55338D86DCF6F0F887010000000000000017A9143CF8AFE2D23B0138526F52\
99094536378335320387010000000000000017A9145F1DC9E4000D1F758689FC65BE22C2FA8151D\
50787010000000000000017A914C3528A9F92C13ADD326FA0D0549BFE8946B9A8CA870100000000\
00000017A914BA47757EA6D0DD2B050D11AEE5D4A7A7CBB095FD87010000000000000017A914B84\
E6EE90C4EC4B06F76543C069BB105F547557087010000000000000017A9148DB69A7A8ED7139700\
ADBFCD905E3C226C4D4F0387010000000000000017A91478FFEBA0C2D4E2B9D64E7A2C9C075CD4C\
0B2D63D87010000000000000017A9142DFC6B7B202646C45C82B1DFA82C6CE16718A03687010000\
000000000017A9144016E987740D0E3B1AF322AF48D24C53A5D6377987010000000000000017A91\
4BBBA0E79E1FE4051401081225C6EC2686127C24E87010000000000000017A9147B05C23FED2AAA\
5DAC0B372F0EDD2D63184F36FB87010000000000000017A914141CB499EDF8F222905565B52A0F6\
1A2484F03B987010000000000000017A91431BE42363D972759C892AF4BFD5BB91E4CE9C5B28701\
0000000000000017A9142AAE67069BBE3FE888CE32064C73EF5885AE5C788701000000000000001\
7A914A0F91E5BC05C70EBD3B0D84D645648F581C11A1287010000000000000017A914801F6742EC\
CBC96ACD72BEF3CC40FD8D1E5948C787010000000000000017A914B174D4547CC98F05FBC86C5C5\
9F16296317F769D87010000000000000017A914FB1E4245983A02A778E60F6C8B01C37FF0CFA6E2\
87010000000000000017A9145B8C336115F104A5124A2F2C735C9338ED848525870100000000000\
00017A9146D7F8CCDB3E8DEE51C780ED7F751BFF5F19632E187010000000000000017A9145A44E1\
5CD5BA36AED5A9E6B13E608378BD352B8887010000000000000017A9143B17C9A258A21D98223DF\
A5277797782CBCDE70587010000000000000017A914FCA65D842B7013BAF144B8EEE1669AA448BB\
C25187010000000000000017A914CD265F270AD3FC32BDE0F0C98FD4F4C80DB1118887010000000\
000000017A914A1EA9BCDDF9EDB71EE3C2AA38DE649EA6C53295587010000000000000017A91417\
83F632A0D1F7F45AE510F9D4C5F2ED67E0505B87010000000000000017A914201C21E627D7FC66D\
9BBE443D3B50F3FF494E1EE87010000000000000017A9143CCEE5F4F4D3C4D7B9E4A8B3BF880758\
6AA2AE0387010000000000000017A914DB2ADB210E5E82FE4282B2CCFF9F647F4BC2E1EC8701000\
0000000000017A91443A74A5680C29CA72D5621C37BBB940DE19286C387010000000000000017A9\
145F36D0FF8AA3FEE04844D7E78F0CD27BB02A509387010000000000000017A914587C092D3A8DE\
6C707ED55CB286732633CFC5ABB87010000000000000017A9143CF55E912A34E6204EAA0C180A4F\
DF5B6DC222DF87010000000000000017A91434D6EE687209DF70197E45F69051E4B3AAE33CDB870\
10000000000000017A9140D1E0BEC13F8604FB9C2479502B4157DF664E13D870100000000000000\
17A914866C4341AC933389EA79535896AA91ED574C1DBF87010000000000000017A914C3328032F\
77AF48C5C02A2C3CF90998AC50A1CD087010000000000000017A914ED70DD226A325314BE9DC183\
1D85E7E07F53189787010000000000000017A914B417D2C6CD4F55244F3CDBEBAEA7CC9FAE7742F\
F87010000000000000017A9149F35EFB4878BF8931A482CD7AE6AFC35D657981E87010000000000\
000017A914F592E4632966C37E799DF75C2EB4B7217B6018F287010000000000000017A914C19C8\
65638B279A2CFAF770DC2EC31631CF0D4BD87010000000000000017A91474B743293F3FA16C70A8\
9462BF09AE69D917B7D687010000000000000017A91479FD7B1730B090377F8E829C6A08B94A515\
DBDAA87010000000000000017A914051C691F4AEE9F80DAE90ADDFAA7B32E0B5FC3068701000000\
0000000017A914263749EA0CE01905FC86EFC55F98C6B9ADD4F74687010000000000000017A914A\
25EE03CB612B3297BCBECED58BA7FD745552F0A87010000000000000017A914056A330865C61540\
3CAA7B2EB54031E32CDE9F0F87010000000000000017A9143B72785E85E7A1824B5E5DF32F27C7D\
0920468C987010000000000000017A914A3CE7215298379738BB0EE0586D1402597281C2B870100\
00000000000017A91439772EB056AAAA3949EBBD960A3331652BFC62FA87010000000000000017A\
914C699F1C02D416945EE46ED8CF8F82FA6CADC320287010000000000000017A9145CB4D062F624\
2B33E5BF69A224DA7C4F4CA53B7C87010000000000000017A9147E2FFF779028BDACCF8C8BCF077\
C7DB0C8C8A60887010000000000000017A9142365900D91D5DF8627520EA661B46982490F4D8E87\
010000000000000017A914C75E2DF053169E4E0301ABBDDDC911A0CB935B5587010000000000000\
017A9145CEE010AA6A0EEDC7351BD0F1026BDDCA4E1026287010000000000000017A91452C36F7E\
CE4A13BA232AC3C4C0552C8D1742D6CA87010000000000000017A914B66FA1B4ACE4CFAF3BE7BE6\
BD6097DAFB5FC951D87010000000000000017A914CDAD2F066B13086E28C262F2DCC078F2A6DD0E\
8187010000000000000017A914279AED2FBBFDD025DD9A1E6F7B373136EBD81D118701000000000\
0000017A914611D5F0E50A8AD946C840F512AF1E9432C180E7287010000000000000017A9143473\
382912D356364BD174FEBD13BB65F536098A87010000000000000017A91467BA9399CA62D9A5E14\
56A9AEF45888CA795F17E87010000000000000017A914AC1C314C4BCFBD892BCC7B95D3E392C5D7\
37CD3387010000000000000017A914EF640C1A7AA891E7F865B673B0EC5BB6E8B321C8870100000\
00000000017A914938F719BCA5166335EEC75D1D1100AE720C4699C87010000000000000017A914\
CDFDE93DFAE38F95A03A73F0114084A87E23F65A87010000000000000017A914E1AE292A123170C\
AA5B492258053C4550BF5791187010000000000000017A914D19EBD0897E71333FD23F08703B13D\
8DFCEF648287010000000000000017A914E36FDA736C1B930A2378B535FF414C1E74BBFFF087010\
000000000000017A914E384F4BE69D2134CC9FFD512D9F5CC08C0EFB00B87010000000000000017\
A914EF765EBE4BC606798B3F31FC1EBE38C6502F110587010000000000000017A91458B8470CB86\
1BE7399E68229F45B3B3CF07ECFE687010000000000000017A914C1114F266341D00B46AD1EE7FD\
683B5B11BD030C87010000000000000017A914EC2AEFB238A2454CE01BD435F41060C5C603D1AE8\
7010000000000000017A91461DE5D67869EF4C946B3C00698236EAC58B8A5E88701000000000000\
0017A91411DCF71B56C22DF052236314D4B39231A78B8C9387010000000000000017A91448BE202\
049116403F272214C6EC87010DDC7FAEB87010000000000000017A914E28CD0E4D20D6F08A648ED\
15E719669EEB4B182187010000000000000017A9142E19550B2FF719A0E4BB9237389617B219D23\
25487010000000000000017A914460377C49BB4BEEB8F5AE54568DA2309CD529058870100000000\
00000017A914D0F18723681AD531CA2FDBE18729A56B1569AD4987010000000000000017A914D30\
34F3D5F413A81D21F4DE02AB76DD2C2DED42D87010000000000000017A914714FE6DE15EB1D579C\
ECFDC802401940409648BE87010000000000000017A91488FD9E04C454B4A06BBDB59CF8B5BFF44\
E178BFD87010000000000000017A9146AA84C36F87A67F4820B35E7C79786A6FE28FC0F87010000\
000000000017A9148D8B7A4E819BA7D658D3839D972FC8355C8D580B87010000000000000017A91\
4A2C6F1B65108049D3A7D32AFC5E5FB79D124E6A687010000000000000017A9144E40B8C2BE29D3\
F81995A2F5634A6975B218AF4487010000000000000017A914A324E0F4C6A495AC1EC17D0EF20FB\
888FE27EA3987010000000000000017A914367FEE3616D5E3812202510F2346E1DC5DDD673C8701\
0000000000000017A9149CB66E7219CF303914C9E1580513C697C65888A88701000000000000001\
7A9148330F9D45DCA10B1AFD03F82DF9C4B2BCB46775087010000000000000017A914C6CAD87010\
58695BC07528EA20570B2DCF1E622187010000000000000017A914CEBD0B2734E7D87FE7C1A1A7A\
11E13B93D00807087010000000000000017A91434CF651F498095B41624C5C8E3E94D16649A4DBC\
87010000000000000017A9142254F276DD514A2D438FE87855BF28F0E1279EB0870100000000000\
00017A91473968844508F31941F0C0291E46C708FE225B48887010000000000000017A9148C513C\
9D8E15921BEC168B1A1B52F6A1B9ECD11287010000000000000017A9148FA5BA33AF260C80A36CD\
1CE251F1E36EA5B150587010000000000000017A91414F4F8C700DE32AF74A8E34F6C327090D61F\
DE8D87010000000000000017A9145C4B4DD58D68CF45AD6D8F5CF57FF37B59CE0EC887010000000\
000000017A9148D678E1D691BD3E8F7A4937B0C551ED8D4BB1D6087010000000000000017A91485\
7570AF1358EF358A3B0EED8D4D4D4F73DC205587010000000000000017A91434B712A7B87F41B9E\
8A2759A3213697AF94327B187010000000000000017A914BEFCE21AACB939FCB34547B9234D24C5\
40C936B487010000000000000017A91493EC30C499C1AC232BF1B7FC23DD29B87CC34FF98701000\
0000000000017A91451AED2DEAF96E2F32774AE2EDD7F63212714D85987010000000000000017A9\
146F6FED6823AC5166F337FF40604B3957D2758B3787010000000000000017A9143210E35E92824\
725B2E491D562DBD5AB52C2A51F87010000000000000017A914992D8D5E928B19141F5810C89A1B\
CAFA7EB755E887010000000000000017A914B7C62C7EF8429044C59AA2F78827769869ACF05E870\
10000000000000017A91477CB6EEDDA0EB728C8AC3FB019F0F81C248C6DB4870100000000000000\
17A914BA9EF9D3F0DBF14B0B4B47AB4018CFD57ECD5C3387010000000000000017A91469A199689\
58A5995D2121EDF4C4C92CB9E53A11087010000000000000017A914CE606CF9CDED1C87449B1555\
8E0A6457D1AB82BB87010000000000000017A914CE45ECB5BDE9B1E3B26ABDC3BF8B5F93A4EB8A7\
587010000000000000017A91406E8DE60E3E777CE75F3EA14CA8FD8D6511FBE4E87010000000000\
000017A914C6E04FF33572CA26FE539A7F374650CEB381078E87010000000000000017A914AD525\
BFDD453C88D7D375D4560DA2C3A0326628587010000000000000017A914661D9A59CC84AFDCBF03\
DFCA120FDA2AFB247AD787010000000000000017A91422B183AB0471857F92C501CE1EEE7883ACE\
2C2EB87010000000000000017A914514B21A48C1C895787823B450BE35FCE8B27953F8701000000\
0000000017A9141212D0B2C9ED77143103C7849AA38885AE48A24C87010000000000000017A914F\
7C4E4E3D5008B584F0760BC7BC3F06217482AA987010000000000000017A914A4911EB0480DDA53\
87EC7BDF68FD4E8EB384B64187010000000000000017A9140A05B71B43D1C31CC8C0364BED1575E\
7408B04F287010000000000000017A9145C624982F8A7398C68039BD6FD9F21897C034253870100\
00000000000017A914B7A82E7862C6F6B1135D1210112C03FC1D22C90187010000000000000017A\
9144F897681AB8AD0069685336041725E562EB58AE887010000000000000017A9147F62783310D6\
59B7D1ED71C2952EC71E9FAFC40287010000000000000017A914958D407AB35B6B73BCB727E5600\
7C7F1B6BCB64787010000000000000017A9144DA6E7E00250134B76B4906CE15C0696F684024587\
010000000000000017A91422875080387C3952C4E052C4AE56DC4E08B15F0187010000000000000\
017A914D506ED49C3DE221517873FB011020534F8E25DAB87010000000000000017A9146334B14A\
C4CC780DF0B98FA4738DD7D4FE31B11887010000000000000017A914E973EA0B61D183BA8034DBB\
225B68A3C55713D3887010000000000000017A91447E0BC7CD2AF6A2953F00B7E15FD659AEEB4D4\
C687010000000000000017A914CAA704418C798070AA63AEA216CD7B3F0FB7BBD88701000000000\
0000017A914A0505344931EEAA620259A0AA5B71B937F9D385E87010000000000000017A914F82C\
0F272A6288DFF0489812D7841056BC24334487010000000000000017A914667571C4004733BB89E\
9FD090B354F8F8815EF3B87010000000000000017A9146D6F07BB9F8E73475F236051F849643232\
2E96BF87010000000000000017A9142198205DD3C01188A97B540CE13D519D34E51E6A870100000\
00000000017A914A88B405A2E5CABDEE883026BC3BBAD27A251CBD387010000000000000017A914\
24758612EA0B880C0C0A7577144C3689B72DFBDA87010000000000000017A914AEF56201EEE7AFE\
943786DBC110E4F6EE095EB6387010000000000000017A9146C7859392A869A65FEFD5383B6DAA1\
9A3034F61687010000000000000017A9142D4866F644861EDD5EC898279A1DFAF04B3375DA87010\
000000000000017A9148B5ADBB7C490ED11FFCCB50BDB242AF51F37E50D87010000000000000017\
A91457267024D80A0DB13353ABC45069B7BE8E0A241687010000000000000017A9148893D86BBE0\
A671F4C633645CB19B31C268C091787010000000000000017A91416BECECD69E14DE3D25D9AF2DA\
59065B57E6D19187010000000000000017A91420F29BD40D3C5004DE95616B16ADA487E9B0388A8\
7010000000000000017A91434BA06EDD7A716774787E7212BEB3975CA3C04398701000000000000\
0017A91460DABD0524040F4B2CC468BC726F256E6F3E250487010000000000000017A914127DC1E\
5D776035909FC7095AA273AE003D410FC87010000000000000017A914A62B5788011748DEB375C1\
1681707DD14E58175687010000000000000017A914C2672F91E87FDD02A9B8C5AE4DF3362B5DF69\
BE487010000000000000017A914079A749FB6293D8A5E495F6511BEBC2267CC725A870100000000\
00000017A914327B720963A044FEF6F4CB927278726C495EF22A87010000000000000017A914E21\
CA3BC6502BD5B2807C74AE62891470CE25D8487010000000000000017A914887B6A5FD3BB6FFC17\
ECA9A85E67A9A291C7E5FC87010000000000000017A914CE6893A9E2B9BF4544EBE52689FF44238\
8292D2287010000000000000017A914429A8A1C52E799E1F0198E363B1CA730A820C59C87010000\
000000000017A914F75CA28E24BFB92633810950592D1973A46C210E87010000000000000017A91\
476D62E6AB6FDA839AA666A5512BB55B96A7F503B87010000000000000017A9143F61BBF310E9F8\
8363A41E6B7AD210445FFABD0987010000000000000017A91452E22137BD1CDC7251FDA6AA98033\
62ECF51E66687010000000000000017A914FD8C38620BB91521E98F14E10FB1B4DD488A5EF98701\
0000000000000017A914A2F5716516D5F48613EA3BCF18D48EA96ED7ABAA8701000000000000001\
7A914C2A5DF6BC316CB962CE7F6ADCD7045F37C90FEAD87010000000000000017A914957C4C7BCF\
199ED5E752BD8A55E23B7EC35B352587010000000000000017A914917754359F2AEF96C6A1D6E86\
3CAC89300C3845787010000000000000017A914352D3D4AFC03AD24324F637FACD5A42D2C6DA940\
87010000000000000017A914841C36CA67F3DEA9EEC3CE8F169BB7E8F85EFAF7870100000000000\
00017A9148B0E6E1C76747FB9794F6B80D54F76557A920B4C87010000000000000017A91485B2BB\
73CF5F5726C7B38C285B73CE0D848E5F4E87010000000000000017A9144D754D5792C4A3AF6755B\
563B55ADE65DC7925FA87010000000000000017A9144B19726C0AD882A50AA63A602E46C7E0BE1A\
E41E87010000000000000017A914C9CE73C39C60A064002693283D81B2618BFE6CC287010000000\
000000017A91431CA9F7FDB3703E6C50A39B1CF3A9E2FAFBCD68787010000000000000017A91475\
0A5724030C1BAC957E4AC3D138830D7C569EC787010000000000000017A914779F716F92286085B\
3B10D52A06236913CFF656987010000000000000017A9147A91BA9AC32ED11F0FF45CCD96697B32\
45D0FB8F87010000000000000017A914640ED099DE52392BE7C1BBCCE6AF182744BA7D3A8701000\
0000000000017A91494254B9D8134EFDD45B10EE65F9D61430281DB5C87010000000000000017A9\
143F8CB997A170391655BBF95D29DC996D4E02C08787010000000000000017A9141534537F0668C\
45C49C4F515E4A31EAAC5ADAE5587010000000000000017A9140FDF6865F7652DD847015C096FFC\
6DF7CF5CB31D87010000000000000017A91462725116E3A0FA156E32AF63E88369A9BF1A2181870\
10000000000000017A914FDD4E606F26BF63F27EC2E9E10CB3C4078B048AF870100000000000000\
17A914F8B199A56C399F2295A5513728DF20D154E4DC6587010000000000000017A9143C4497D24\
7E7D736DFA7466C08B48E3D6B96463F87010000000000000017A914E9CF188B73B62DFA25A52148\
C767E590DFBA169687010000000000000017A9140FD0489B8BEBA0A91A52532D95C5B02B0BDED63\
087010000000000000017A914AA32671B8446E94DB390F9BBACBF6F603811FF4187010000000000\
000017A91453FD2E1055D0DFB2DC021BD34DA7F8AB0C4A6EA787010000000000000017A914D8AE2\
0C7A622206EEF069FE240D32750DBEAF5EB87010000000000000017A9148EED1E4559066E3AE3DB\
02B4FFB6AF26EABE006887010000000000000017A914F5771F0C96CFC0C8EB1FAC4FB03AA32617E\
E13EF87010000000000000017A9149FC4E2859ACD385724336A9742FA1DB90F1C9FE08701000000\
0000000017A91402903795F02E7B1C0F02FC4E53785107AF9FB05E87010000000000000017A9144\
D7689676CAD1210F2FF51B082258EEA64A03EB087010000000000000017A91453967C9F45E97BEC\
7348E34B9D4A35FF8C7344D387010000000000000017A91403E3230ECF3687EF7DA5CEFAFE937A1\
A8DF44C1187010000000000000017A9147833F029D0AB263D1096BB37983EEFE0B614264D870100\
00000000000017A9144DAB457425F699667F3EACD11C2462606B863BDF87010000000000000017A\
91429E3D684A0B31D0BF97EC3B8B18754F517D753B687010000000000000017A914140574D8640B\
744B1B0A66236E8334EC6B36650B87010000000000000017A9148577AF4C7B80A150C69E7015DF3\
C1C810EF4969A87010000000000000017A9143C60A99D4725C6A842759D6DFC118F5B49F9C14F87\
010000000000000017A91480DF6B54AEDFBCEB08ADC4444D9BAFE1EDFB732587010000000000000\
017A9149F95A1C1D49875780E78AA3E7ECA5A5D9EF273B087010000000000000017A914786D679A\
698AE221EDBB32A403E09B1D4AB0E0B587010000000000000017A9142EC0EA021CDA9175924D225\
B67F6F7F5596A9C9287010000000000000017A9143E1023136A149FEB41B4CBAF9C575F108E14DB\
B187010000000000000017A914FE61BE88A4EB2276871B927935F459552C62C0818701000000000\
0000017A9148588F327C7CDF867AF7D248B286723DD6708F95387010000000000000017A9145A28\
B7F89C58576F1A45F050837C83D5A67A50CA87010000000000000017A914099C259293B1E18AF57\
FCAB0AE662ABFAAF0A14087010000000000000017A9149249954BCD21D41A69CF0071F3AF0AD301\
6214B387010000000000000017A9145CE54A50652BF7DA76A190D824BC2709C1E84F56870100000\
00000000017A9141289B94DAB7A5D8F1E2E02209B60089F064DB01E87010000000000000017A914\
FDDAD25DD763D10AE342CE33330F7555E39D7C7B87010000000000000017A914A74FA15D9D77CCE\
740659C6E4562E7EA6B1A8E9787010000000000000017A9142A6090F691025D622971CFC9471A2D\
B2043F3DCE87010000000000000017A914707FD39753CD87E78CE1F14691A1B31C7184FC6E87010\
000000000000017A914CB93E1B3DCF92C9F589946880C06089EF84C68A987010000000000000017\
A9149A42155C636CA71677940A51A315F1CABC4E0BD887010000000000000017A91422ED49889B6\
18AD04DCB6BEE7CD05E52190A4D3787010000000000000017A91445EFA701F79D5C702B87E7CFEB\
8E1EC20D31ADCF87010000000000000017A914AB17579F75FE5B14F2D0F5A77A9529F0979A9EDC8\
7010000000000000017A91482EE96183ED4B531DF618DFEA8FB2373AF151E098701000000000000\
0017A914587DB40F5726CEA845A67C629EAA7B0089E7095887010000000000000017A914B9EAD30\
4EC2BAC80A8FB6DC79558B206308F93BC87010000000000000017A91433527592AB30DEE2AC7AEA\
217C7680C3BBA2358287010000000000000017A914294E3A3593FED54F86B87759744CDDCC12CD9\
32A87010000000000000017A914D141DDFC92E13E9D4AE495973309CEA6D1918DC4870100000000\
00000017A91408D20A5F79157641A49BEC266C968C1A83D574DE87010000000000000017A914368\
9BD8386B320BDCC2C5EA1A1765D38B0C6795187010000000000000017A914E5A3FA39525FC76F8A\
3F745ED080DDBE8DA2027187010000000000000017A914D3D8E97770B8142BDCCD2B06B773A3EDA\
56C3E1A87010000000000000017A914929D41EED256F3D5504D8F6952AFF81FB86933AC87010000\
000000000017A9149DB27C7FF05327C31609C21A0AEDC653220E42CF87010000000000000017A91\
4FE5492B901D693D7DDE8A3940533467533E5C2B587010000000000000017A9142B01FD2E3D6EA2\
42EEFD75B6858D7377E50541E587010000000000000017A91492EB1A0DA6E8FE2D66077E6D8DA9B\
DACA55E3DDA87010000000000000017A9149600A7E542559D8477DDCAB711642E5CFE32B49E8701\
0000000000000017A9142B5AD867219ACEA629FAF248B0AF3B90C75A8D5E8701000000000000001\
7A914DBCE70DEFAF4F78AAA38165FE723CA5090F8C8D487010000000000000017A914935196A855\
6FA4E17E6C5CC6BF6BBDE445434C0887010000000000000017A914BD591F55C896AB4332A635DC7\
25768CAAFC49EB687010000000000000017A9140655920B9CD7B0856DEDA65F1906F1F6E08513A1\
87010000000000000017A914F0D8389C4B800DE983825DC93830A2355C07E8E1870100000000000\
00017A91490F33F31C273EA050ED7CB98E7DFEEF74B3679BB87010000000000000017A914BD8B8E\
56BCA1039E865E35F0171C2689680E006D87010000000000000017A9149C66AB4259C1B15D9CDAF\
973C3D9C9E9599A719487010000000000000017A9140935FB4C10B07B7544892C26001035D303B7\
FFD787010000000000000017A91419BE321EDBCB2102D1118125245B93159974C74087010000000\
000000017A914F174E8D7A3E71627EC021A78C5AE31EEAD481C4B87010000000000000017A914D4\
7C76FD9DBE1228B2CC27988FBE671F973A263587010000000000000017A9144E688687F82D0A9D6\
5C6EEC87CC7AB407179CCA987010000000000000017A91458F7EAD8674C7FD02C7538636BBFD6A0\
90F0EAED87010000000000000017A9142CFAD14BC0C3B5E3D07A44A478E1718269C7550B8701000\
0000000000017A914ADE667DD5B6EE41BD06E68726B9467D09C0D6E1087010000000000000017A9\
14E8D77C30B1B1C6633E833A55D0B65FC6598A0FB787010000000000000017A914CECD7E21DB045\
7C7C870F05169B525B97C82C22287010000000000000017A91417DD3BBAF95D2273FDF27936BADF\
37F7F0A621BB87010000000000000017A914693C92A964F52AA98880004BCCAAF721F596711A870\
10000000000000017A9143CE5AFA413E140553F75F35A6C97F97A2FAE90DD870100000000000000\
17A9148B8F9ABBB6BF9C85D3E14DDCB5C6ACEF0138A4A687010000000000000017A9147640B913A\
F7EE8B541F9DD57EC3FA9733B13BD7A87010000000000000017A914E021C649E2C74F80F611F3DE\
1DD8B17E514CD80587010000000000000017A9145C50BEBFEBB39DA083D43AAB3304CF69E3AD17F\
E87010000000000000017A914F14D910850F3486D4EBF5866C3EB58B0D3F6164A87010000000000\
000017A914A3A96AC5BD29BC2D215713B6DB75E5346F1DCC3A87010000000000000017A9141DB58\
601F91F7F895F22170A38B4E064DE4558B487010000000000000017A91455194281151BFB458CF0\
59EF4183E7C153CF873487010000000000000017A91486B46E0B152F4630DBB45911E19144933E0\
CE19D87010000000000000017A91471AC00E5C0F9A001DC9D88AFD629C609D6F235EE8701000000\
0000000017A914B4C752224AE968725087683A131A9FC40DA6166F87010000000000000017A9142\
4E72AC3616D67A427007210A6D4BF79080F3FA787010000000000000017A914C04DD68D9632B841\
36760B505AC1222BB54751F387010000000000000017A91410B0AA10B87D32CD240E36F32AF1AC0\
FFB0C0BB187010000000000000017A914776DD49D29637A085DD5E47402E57BAB0C75AEA9870100\
00000000000017A9141948D62DEEAA7BC2790EC8DD9A925C6FE7A7686987010000000000000017A\
9146786DD5E849A3FD4D0825DEB375EADF44F635CA087010000000000000017A9145E7D1AC80708\
B76E782BB9127C2D530A1857D55587010000000000000017A914DD9128537D9744BFD70F634E88C\
C1CD5D78E335F87010000000000000017A914897A747A98679EFF232F78AE9C115F939E32B9E387\
010000000000000017A914738ACDA03B872EDF5A58905607737D21AE1CFDA587010000000000000\
017A914543959E0C5DB865774885EC3A8221B3937C31A9087010000000000000017A914275E945D\
608641841BC083BE6C05B31F8E6F4B6587010000000000000017A914572B130B3AFEE0CCE8EDB93\
00938AD5FFACBF77C87010000000000000017A914A8D006ABBCDAFB99FF7F10EC05045E3D9A9B50\
5087010000000000000017A9142DCE2C1CD9AFD71AF6098A4925443A8C814FDB3C8701000000000\
0000017A91460945CC9E27B42B0FC9FBBE5B8D2A2AEF604FC4B87010000000000000017A9146AD6\
728EB9C562EBF926682DC9153386D0238C8687010000000000000017A914937275F44907C7BD54E\
6409A097F78E5DC2CA64F87010000000000000017A914C926ACA45C268B613EC94E9588C2BAD120\
5E8B9587010000000000000017A91485E2D41C21C01F0A7DEBCA9F99D9FFB14F9E0F67870100000\
00000000017A9142B894B51690E50440704920E9DD6960F7A33B44F87010000000000000017A914\
84414BFE6B57F14D3E0C66C166D638C6D174648087010000000000000017A91463B8238798771E6\
78FCA91DD1DC869267675B96287010000000000000017A914A4776CF66A10AA6E9BA679BA20C256\
5CFFB7AF0987010000000000000017A9148659EC7604FDD6937EC98ECB093F0722CF1B23BD87010\
000000000000017A91442C1866F7121BF729517C2ADCA90260C6A25AAE787010000000000000017\
A914E700ABBA1D302D452999A1A92AC4982C9E85594387010000000000000017A914BE58887ECE1\
3FC1DFAB468731F0B2430A6E52A2D87010000000000000017A914725111C5F2172A27DE887100A6\
9AD91DA1863B3887010000000000000017A9140F274566E6B3CDC2A23AC9BC5F4069591768C29D8\
7010000000000000017A9142C0BCF21C5E9870CF1BA782A9379A49CB404199B8701000000000000\
0017A914F7DCC2AB065853FEF0B51512DAB8CF387C0140F287010000000000000017A914FA2F6CD\
D86357B4AFA992720D0E84883689128DC87010000000000000017A91491057963676F93D444C478\
886AD8538CEF7B029887010000000000000017A914FA5B03B4DEF2757F493BFBBD5E8CBE719E459\
8A987010000000000000017A914D3F98342A279B8C437DEC05788B904892F24A663870100000000\
00000017A914DC7F339C70D1CFDA193EB09D219C094058CC3D3087010000000000000017A914791\
AD8837C7777EE8629D9E587EC56715162B1F287010000000000000017A9149B90867D85730E0F44\
7EBD6A4DF4C1C82C2A405D87010000000000000017A91482BD4879A5344FEE836D9C8A7A6DF3292\
E3AADDB87010000000000000017A91442E88959022F8F6C1FCCB44FB5247EC99B6EF61C87010000\
000000000017A9146DE6C1BBF03767F7047D0AA0D8966E6F272B2FBB87010000000000000017A91\
41A58D81F5EC0890E28F412D58A100FA88A47AC1387010000000000000017A914F77FC126650579\
7D32B946B72C7552BF922123E187010000000000000017A91466CC063126AA56D233256260396D3\
063482568A587010000000000000017A914F8E6F8D0A69EC22EC5E08EAD23A2EC0FEEFDE1AA8701\
0000000000000017A91448EC6BF8EA5C94F71A7E20F4F37FC337BDF206E78701000000000000001\
7A914F0608B723212E08918AE2DFBA68D5E37BCD2BE3287010000000000000017A9140FA8F91832\
F35E425ABABBB4C3F73605780B1DD887010000000000000017A914DA18236E87076148B745C1603\
0CB0492DC16A9E387010000000000000017A91447C5BCE2471F9511254E3ECEDC38A46DAC568577\
87010000000000000017A914836662F99111A97E63CFF380987008D6D9BE34A9870100000000000\
00017A914DAD3AC4C4A5B9C916338CEE22AB3D0563D0698E187010000000000000017A9140FBCC8\
D5ED25C108EB0F97DB6FD3B42D1B60FEF487010000000000000017A91456FBF3A3E957593E173A9\
D2E0D4B0960FC13A43B87010000000000000017A914EE1BCBD19ED97E7AAAF632B3C7732EB217FE\
5E6787010000000000000017A914D85F7C99529539E6A219B1B7DAE024650D28C99F87010000000\
000000017A914329085BB35D643CF296DDDBE8061ECBD6940A4BF87010000000000000017A914D1\
D5DE257E314A03EF31568875B8D42A7D32A74F87010000000000000017A91438286DF5107A0929B\
1A00CD4F7B60C56A1B6798987010000000000000017A91460BA7DF66C536C3D8A69AEF7DED2FE95\
38B4CB1E87010000000000000017A914E7BAABD909D3E91D855C1DA2C3FF388D0D7A459B8701000\
0000000000017A914C505AB78F33B42136EBA2232E03FD8F3D1E7686587010000000000000017A9\
142B9F516F6FAE2CC3F0ADB553EB067FDB94C419AE87010000000000000017A9140E1D8B0D7E3C4\
E779A5DA22780758DC87CFC0EE687010000000000000017A914A6B1EBA7091B3119E4007C482C7B\
101669ECB92A87010000000000000017A914638324028F37352A97776EA8242A8D8C6C717EF2870\
10000000000000017A91480CBABCCAF22A54DEE41475DD79F684A74BA08A3870100000000000000\
17A914DA45A0A1341B4AD2E19F10579C7EFDDEDF9109DE87010000000000000017A914CE4CBDD58\
6E9D82099B03B46DD7115A4D3E02FFE87010000000000000017A914BE04C5791A7672A09557CFD3\
A7B80227B4708D7687010000000000000017A914A44953C533AAF8143B0199C148B39AF7E552CE8\
187010000000000000017A914EC98DFC6FDFD4C87E233653F974B0DE478B4853987010000000000\
000017A9142C221D5B56F88AC0603B16B3B042D5ECD6F2137787010000000000000017A914D90D8\
97DF37A9C25BEA4532E4C14FCD8A7645E3187010000000000000017A914FF9CC305B03EE6A90F5C\
D61C7584CC689BFE98C087010000000000000017A91461A638FD399FB02FAB07987528672F37FD9\
BB3A487010000000000000017A914A9A9DC27859A2689DF9319C83B70EA41FE9A68598701000000\
0000000017A9147B29809E10B7C2B179CDDB40956701E1426AB82787010000000000000017A9142\
AB620EF3642A1C62EA2B9CC5FC8A9A934EDDFBF87010000000000000017A914D6CB298D1DF12ED0\
78A12B809852F00C9801973C87010000000000000017A914BBD5CC6B16A2AFA086C1D338E60B1A9\
35E7B886B87010000000000000017A9143C732D834EF7B44195FFE66B6703C93841FD3047870100\
00000000000017A91405B229E10C2696239608514ED89E4FF8874B017287010000000000000017A\
914131FCCF3F3881E007E3F602F7A1769EA3F52D60487010000000000000017A914F0F4E1EC55FA\
30BEA7BDB43B3CC8355FB32D263887010000000000000017A91466F8ABB97AF67CB3D8558D0CE48\
1C15CB9B45D2987010000000000000017A914992B28BB75282C6B266FBDC1535D79CF4FCF618F87\
010000000000000017A9143ECDCDC5A3A5CF2EB1D2B5A9EAF83ECDF495A9C087010000000000000\
017A9146AED8555D2EFC4B36DECBE0C10742580579CA0C587010000000000000017A914B9D7DBF5\
B7096C1CDCEAABC4DCF8F16C5F8A5AC687010000000000000017A91459E17F08B6E571DB61354B7\
CA3864CBABB4A233B87010000000000000017A91484C820594B2819EEDEFB3296AE9DE678C703FE\
7D87010000000000000017A914F5DE43B333D75E344C1E8355AD5F1EFF11559D768701000000000\
0000017A91485C6A228378988CDF5BCE3D8916EF2CA8FFBF19187010000000000000017A914FF75\
C169EC30C9881A931BD05467FC39F3743BB087010000000000000017A9145883322AAC0164AE4B9\
4A486B0D1E74A4B3375C487010000000000000017A9148DAAC3A14D029A5BC5D70B3DBBAAEED1DA\
9696B887010000000000000017A9149B5BCBF79839736CF77ED79B72D583242E6C537B870100000\
00000000017A9141BF7FF1FB02673F8389CA0160C4A73D72A35547987010000000000000017A914\
13F79E70983FCC4FC4477835C3888351B4A33B8787010000000000000017A914566024505094A85\
4292F3C1A048E6C6A421A655C87010000000000000017A9148707C7F7B0C8B348EE2452E48514FE\
9A3E0356E087010000000000000017A9140DC37F107F74007D08183D6AB0CD39E803D9C97387010\
000000000000017A914813DF89684846B5D4FA70100084B823A2DC65BC087010000000000000017\
A914354651797062640D2D8A51D47A7BDFCD1183AD1487010000000000000017A91468FEB1118FD\
00488A26BD52D55FE054CCF5ACDE487010000000000000017A914F454B6F10E3AB892942F6DDF12\
CB0AB53CC4FF2987010000000000000017A914879827BD61E7115DB0E3D71ED91D9A89DD9C24548\
7010000000000000017A914069939228B9F8552CAFDB342A2D4ADC36C5D42E58701000000000000\
0017A914F53768A5F7015C410284BF34D81D069D56212BF487010000000000000017A9141130271\
C57FA25A6DBC4914CF9C6C715B24AA8FF87010000000000000017A914CA02A253D125DF68138BA5\
32A54BCC1F4393F6B687010000000000000017A9144C02EC89C196BF6FB1A6A937BE80B8F3D0C5C\
D4D87010000000000000017A914151C239258C77CA61B940879E2191311B2C2BDC4870100000000\
00000017A91423CB99F2678856017F192282466E8F39A0B3742487010000000000000017A91486D\
3071E8D778E6C9687DC6E7B6C63EF4B29421A87010000000000000017A91449D2D2085C97C7F20C\
AF9946F0B826FE7BD9487A87010000000000000017A914EC0141B1F87CBA8B8F71084F5F098816D\
FF9C8BE87010000000000000017A914792D8C36F81FDB86F6DA745EEAE60FA74A561A6287010000\
000000000017A914FE75A23BECA0327F2F18C8D7506C7A5B4D0D5A9C87010000000000000017A91\
4535E236E739837FB1FB03FC70C2C0EEC0BEE3CE887010000000000000017A9149FC7C2533033B6\
6610598C8B47D7E399A221491087010000000000000017A914DFEF49A8B9FA29B6E868A18679E05\
AE6DC8D542687010000000000000017A914896EAD6AE5A56DCEA439B1EE182FD1D42F864E448701\
0000000000000017A914EBF9A98C8E5F2D911350400E6CE6FBBDE98F4F1F8701000000000000001\
7A914F46333DA99C9F493C5095DDFF97591E3175F2CEB87010000000000000017A914561C7E3B38\
AD890636900FFE723FD1AA55F653EB87010000000000000017A9140C4DC19F561FA6129D1B348EF\
9CC61D4D349AACF87010000000000000017A9148A499B3F7CCAC41E76490C347F4720FC29BF7909\
87010000000000000017A914D42F3D9576F7EEEB741E447C5D62294932C51B5E870100000000000\
00017A9146F950C36117D6CD593A70C668EB5524085BF7AD387010000000000000017A914BAF319\
BB0C6C13D075D3499D4D6D80AA8C1F4F1887010000000000000017A914FD8CC753CBF4A3A4951DB\
FFDE5A996200382830C87010000000000000017A91427C3575E54428F1D0CC365255B6B29E043CA\
B1E387010000000000000017A91462585F7F687823F9A0AD8E9FE584ECECFC4C859F87010000000\
000000017A9141CC50B44DFFEC97BAB7185A74F5906C93FEE90CA87010000000000000017A91451\
17ED8687D57317FCB5F52137BE60BA0A5CD68387010000000000000017A914BA532C13CE3A5DCB2\
F5EE31D9086F71CCF63FB1087010000000000000017A9149B1CF18C19E3AF2DB203966F8D4624FF\
C5ED585987010000000000000017A91401C3CDFBC8EB3F6A3D1DC7F052345B13194509948701000\
0000000000017A9140777051FDB8F1139CE0AA31B5404054B5B3242D587010000000000000017A9\
142E7329B8A0AC40CAFD66CBBB1D1DF5352EB7796687010000000000000017A914F18AA10C1D5EC\
3CD5B60BDBF2630A85CA73D10F287010000000000000017A914B25088885E9019050BC5E694839F\
B9B36066B25C87010000000000000017A91475F4168BBC42185FF467C471D470C96A3D067434870\
10000000000000017A9143B79F70DE0FD98F02D54ED6428CBEFB16DBF1950870100000000000000\
17A914384256778F8A4E5E2D98AC07869E7B501A32A5B487010000000000000017A914D160849F9\
D965BD923B3747F4A5D124F9234CD0B87010000000000000017A914EED4EF0AA43DAF2976C00B09\
637F6CC4A80B416987010000000000000017A914FC63C3AC1371FBE1BFE4B8DF953ABA86B3ADBD3\
F87010000000000000017A9145BF52B1ED998B3409D5CF70A761F9322094B930F87010000000000\
000017A9146069C35545B4BF48AD6693EA236DF86F4392DB8087010000000000000017A9148195E\
AF4DA72766F2F5B7D6408DAB0742CBD820487010000000000000017A914E0546209CBD2B1794595\
D3C6F826E829A78D9E3987010000000000000017A914F0616ED79ECABA10A6EA5521877FBFE3959\
9761A87010000000000000017A9143EB43AD68181EEEFDB47E01C932755E2529852008701000000\
0000000017A914782FAF51379BA8A72FF70DD16B2D4A98C9CF850687010000000000000017A9149\
E5DD00EE16CD41A8FFB62AF1D022D1636DEFF7F87010000000000000017A914E12A0995550ED6A2\
0C18E9C566D7C9A6AF4EE3C687010000000000000017A9142B727A65D1C9A1580660E91E06768D5\
8A1EEEE8687010000000000000017A914BEC9FAF1B72E67EE932E88C9633BEA96B96D47A2870100\
00000000000017A91455103A738C5ED1A166C12E687E0EE44A770ED8CF87010000000000000017A\
9141721F4CB1C5EAF5966C3FED3108781812579E26D87010000000000000017A9142C09E0A9168E\
2699ADD2E2A42861BB10C1E4D42587010000000000000017A914A48F36236AEBB6B57CBE83A34B8\
6887AD383283587010000000000000017A914ED70756A830FAB711496D90C4D6D2F241A2947A487\
010000000000000017A91461226D7E0A8BE90200BFA6B1DC4B51718492411687010000000000000\
017A91409485DFCC0A855C1520FA17B0615C8FA3830EB7D87010000000000000017A914C157A7F0\
3789E8EBB09FFC5BD7B8D0FD912E9B2187010000000000000017A914D9C8F19633D0C0704B20A13\
A9427690B966E94CE87010000000000000017A91461853CCC081FE48B42D519E845AB42C87CCF59\
DC87010000000000000017A914EFFF877BE3FB8C81D9F9150FC00D62CFBCED32D18701000000000\
0000017A914023BF01E87B6AF0780E4C27D88C9CB7153B85C8387010000000000000017A914E02A\
AB8851E75459DE8A3C881E073229A38DD42587010000000000000017A9141C0415F8649B7FA1F0D\
E6A14B531C3DC6847CF2287010000000000000017A9144914EA9E08D2916FB9D22A08E441C867DA\
2BA2CE87010000000000000017A9147DA81B221BB6232F5CE45F69E6DC5535EE495F43870100000\
00000000017A91426B01C4028A43ACB9A88499EA685EAADE9066C5287010000000000000017A914\
6FA6B7701DCE884F6F2BE11A602FFA826D50B6FD87010000000000000017A91458CCA4A8DED4D49\
C57627BD93B6D4ABC1FE1611287010000000000000017A914FA9DAE24A87C6CEB67768AD0BF43D3\
77C3C0D67787010000000000000017A91453F9B4FD520F203F6A850707E002EC43D854634D87010\
000000000000017A9142FC3D15470F61DED9A24B44A512CEDE88B7D32D087010000000000000017\
A9142877FF503F582E555A09433A8C875F35C50F2C9387010000000000000017A914C78A058101A\
2955C169D8376E44434D543E71C2A87010000000000000017A9148C8DCC35B9ACF1C8B5B583E649\
2CCC7105B0790187010000000000000017A91411A5519961B0393EB5230D1CE19DFDE159F7BCB78\
7010000000000000017A914025DAE29646B29BF59B3289B6874F66DC078D1A38701000000000000\
0017A914410E3093C186D2C7AB6592BE24352E97D6EFC70487010000000000000017A914D213E93\
83E1C566C7344BA465EF49FFFA47459F987010000000000000017A914B5046A9214C88B627B9404\
CC2B3B84BEA7BC15C387010000000000000017A914743E4091B37208C66C3BC1FD0CFB8F641766F\
78D87010000000000000017A914400C7D90DB75C625E527F1019A1CE883FA2331F6870100000000\
00000017A9147BF29BB803B1795AB04EE1EED36BB3AA40D6173B87010000000000000017A914E2C\
E98751B3659C9B951DED8ABD59978C907A80B87010000000000000017A914DCC5E0475C416803E6\
229FFF75F84920E6664C2787010000000000000017A914FFA40160797466B59CB388996DDCA4666\
5422DCF87010000000000000017A914680242D636B0B0B46BBD01890E8AB48230803D5D87010000\
000000000017A91456D00D570C72BED21D600BA847C9E26EF116852A87010000000000000017A91\
4BF2B04C213E50A4C591BC103DA6E989C3AFF930C87010000000000000017A9145B528097410FA8\
88F48E46E07FCFB676977543B087010000000000000017A9144BB0A2F65A12443F19ACDD7284169\
B7BA1C5B09087010000000000000017A914F89A1D2A0625441E4B5B85CD846E6383794E05E68701\
0000000000000017A914265662E12F2FC105EAE00F9CB4D7C5570A471D0F8701000000000000001\
7A91402EA02F13C247DA8E9B4A8CEB8B9089699D5B9A987010000000000000017A9143D7C8A20EF\
E0C97A5984A4712F432999900C486E87010000000000000017A9146948E09733D9835722C142EED\
13A929EE3D4509287010000000000000017A914FE6932AB9209795E7A4E360347A0F22F9205F019\
87010000000000000017A914E6AD539BFA5107546B97A9355983256B8F596934870100000000000\
00017A91476A7B6E295F223E2F8AB227DDE1D28A65CA7CEEE87010000000000000017A914D1263D\
73297F73ED3E50A373C1AD7CED349873AD87010000000000000017A914626DD83497A89AE56081B\
D3837D6CCF67BFBD07C87010000000000000017A9141C072127F5B65A5116C93DFA92104A42C358\
250087010000000000000017A914E50BC669C97057E90902FF3854733A7CD1922A2787010000000\
000000017A914B369F53F761B562FB8AF714BF576DE7D6B49A87B87010000000000000017A914A3\
6D6A919536BC051142AD6A343DAFEBB27B419787010000000000000017A914F0FF3961C662FF80A\
A126547F2FB766DFE8858C987010000000000000017A914F9214815CC7A62FDE47B29646CA282C2\
4422EBDD87010000000000000017A9149995ABE94873BFC0EF95DAC2169DDECDC18F55D08701000\
0000000000017A9140FF1B09234099C5F23E4C9C1541A65EACBB0321687010000000000000017A9\
14636027D530C109FCE989D272D2B2C4B834A3CFAF87010000000000000017A914076305F9F241D\
DDB3445E18FE26D589C868B128A87010000000000000017A9143E5FDF67F8019B354CE2291BE7CD\
96E48D8CC9B287010000000000000017A91408A2556992E0627A0A537B8914652CAF9B5D1236870\
10000000000000017A914CC8A63E118FE94E1CB8816810BB627B26EF3EB42870100000000000000\
17A91413DFC3FC40501792B73749574F2DFD35AD22185487010000000000000017A9144E9FEA9BA\
BF84487C6547ADD0DFB397E78D624A187010000000000000017A91413C87808BD15B676F2473A1B\
C7D37EE106C2789E87010000000000000017A914775B22DAE51CCB705FC7907D238505F90B338BA\
187010000000000000017A91478A9C44A3F64F03B0945F12591A77CFF02E0465C87010000000000\
000017A914423E592D2627227275C69D673778510F0C41BCC487010000000000000017A9142B1AD\
794B1F2F0C94901E8111B88C7403BC2618387010000000000000017A91484B3F0C808EDF83125F1\
51B689EF924608BD7E6487010000000000000017A914E068DFE1B9BA0C5CEF14305C630FF95B841\
37CD187010000000000000017A914801601B3DDC17DB05A94AC1430620BACD426021A8701000000\
0000000017A914220B36D2E4BE08D27E3F67828170A1B85ED6CC6D87010000000000000017A9144\
BD61AF314876D9205F43535E59F3E78D7C2FDF187010000000000000017A914E5B14CE087786387\
5AD1943A8433AE5B189CB65A87010000000000000017A9146900307C8DFA45A63C80AF434C297B6\
8F6ABE97687010000000000000017A914E5E0F91D10588B31085265EB76877C3EC6079E11870100\
00000000000017A9149C5D40F534DD89BD7E6C469AF091EE5AFAFE090F87010000000000000017A\
9143AB99B5774D9D8770C3188715749B0790900CFC887010000000000000017A9144FC7416961DC\
FFEAE12D49C09272EE19F1B287A587010000000000000017A914875DEA1776B00C00299133A0A83\
F2FE9B598DD0187010000000000000017A914F694979ADCEDBCE16422FC93EC4633E499701A4387\
010000000000000017A9145BFC149F4D1D17BD9E360253BEC0159E7938CC8987010000000000000\
017A914804579181697C3D2A352AF6CCB4573645D4E009487010000000000000017A914E1B0A752\
078704387FDE4F3C84EFC80F168D693687010000000000000017A9141757BEBE3D1AB6FE142E8D2\
F81DAF8AC19C690F687010000000000000017A91409E8741EFE0850710D30933A21DFC6B732C364\
8E87010000000000000017A914A3961E66A1B7ACB41C4F5EB005E26270FBB2666F8701000000000\
0000017A9142CCC47AA9EF1AD3078DE65B2FFB26D44FC8C6A1A87010000000000000017A914B9CA\
A56D6F184E22FFBA37E6CB5212706774328387010000000000000017A9145BF11F638C2993D5B72\
838855F93007F074DD3EE87010000000000000017A9141C1AC9394BDE411F88FF5F9BA3877E14A4\
D9AB4287010000000000000017A914BDEB54E8CCCF307B04133B0867A03FA7E9C068EA870100000\
00000000017A9141D6BC1324EBD85854ECA8E9BEF288CFCD2DF7D8887010000000000000017A914\
7ADC2C7BC42DE278DCF86F1A64F283B491BABDAA87010000000000000017A9146D018C7DBBE4D45\
C4E2486EC7BC8A3DC601450CF87010000000000000017A914444A182D20959828151095F321B981\
8380EEE1EF87010000000000000017A914BFEAF903164FED99BF8CE6767A206CD9200D04FC87010\
000000000000017A9146A7ABAF676472CF14330D7DA4407E1688DD7F10587010000000000000017\
A914E53321A79F7F7B069DFD87E9F48C5744B963F64387010000000000000017A9141C4477A7B29\
B9AEAF43A5A19F864CAE27A2791C387010000000000000017A9148F5F1479852E7C898D674F7EBC\
A8AA0E75778E7787010000000000000017A914DE4690F0228CBE45C1DE5B9E4BF9EF2A50AC681E8\
7010000000000000017A914C436CF2DE8ED8613D309560DF35C09ECE4BE40538701000000000000\
0017A9146098F931188CAF0C5729DE566D5D58CD433206B987010000000000000017A914342DADE\
5FB44125FF68E683D9DC5260A2198019887010000000000000017A914959D1BF79838582AE7383C\
0F145CE6C87AA1773D87010000000000000017A914083A77753E0D885E6A3A8819E03BD2B0C1EFF\
50D87010000000000000017A914E5949AD229510033F8E8F64AE5B887D8E216A46E870100000000\
00000017A9148D3290E1FCA40610AD145342190D8DF4B28DC60A87010000000000000017A9147EA\
0249305271DADD7A52B982172C95727A0610487010000000000000017A9148FDBBC07E79777DB54\
5547C07AD3B520B9F7068087010000000000000017A914D109F6F7FC0E29012552FF120B8788CCA\
2400BFF87010000000000000017A914F9E725DDD7F7A31D0CB73F3E20E2FE896231DB6B87010000\
000000000017A914EE823810BDB46D7F866E794C211F4A44AA44120787010000000000000017A91\
49D6584EDF80201DB6AA925B1F547AF9F5511098587010000000000000017A91498CD4CC77C62C5\
EE5D18116580D14F00698DAF5E87010000000000000017A914C0CD613B81185FB56C3B773789AA4\
70F7A9B5BD187010000000000000017A91475B4B6F61D48B5D3634AC1600876DC1AFBA241368701\
0000000000000017A91432A004EE8FACF0F8732EF56241EB54E1E7A590008701000000000000001\
7A914AC3D0108A7D9C4A353F04ED18FAA49C77FDC542E87010000000000000017A91472853C151F\
7CCFC978256468B7196137ED57AEE787010000000000000017A91487618A09A8EBA3F0E282A0174\
6F8A481CD10929087010000000000000017A914B3568378E377F9186387BF04E0B60D0D7985D1A6\
87010000000000000017A9141F0E344B8013CC1A2D06715D3C16EFA8ADA989E6870100000000000\
00017A9146A86332867F5C58E2459014ADEA8DB0D074BEB1A87010000000000000017A914B67878\
8C01B2735D7B5BDACF4F668417F11AFF5A87010000000000000017A914C1E16546C9E4434CE4713\
9B77C68C943DCACB51887010000000000000017A9142534D0E36F951FFC1DCA5FEF540BC03285E4\
A68487010000000000000017A914BD02E736E96EF1BCCD4E4A9940D5DEF35A6326E587010000000\
000000017A914777047E526C5819BD84D85BBBF3972EDD175DBE387010000000000000017A914D3\
BB48AD6022D65C7AC1C9E6F77E6D321F2D6BD587010000000000000017A91498AD0A9E146F159DA\
27E510E727FEDF6FB69168487010000000000000017A914406D0E86265BF3831EDAB685DC717548\
3AF15FDC87010000000000000017A914E51C8B0CE01ACCCE24466D64515AC8BF15DAC32A8701000\
0000000000017A914EFB87A4D9BA25975C88032127F96B2F246DA2D5787010000000000000017A9\
14DFF90461731F16F7F99A21B1EDCB39B4E885936A87010000000000000017A9145B9E86B9814FA\
9FEE5B7B0D62BEA2B3AA246BF0F87010000000000000017A9142510C3180E7F128A1957BA41B4FA\
00E83974342B87010000000000000017A914D0FB318043EEBE73C82C54D38A4D4BFD51FDBE94870\
10000000000000017A914FF609CA7C4E234E50E5F1879295905274252D884870100000000000000\
17A9140FD96F296ABE5029276936A4D929E9EC13650E0287010000000000000017A9145EE2DB6B1\
F9F56E4CD75744CBF3D21DDE5B423FD87010000000000000017A9144918DB882E795A56AD806837\
B106DE87AC3A749F87010000000000000017A9140BABCABFB554540D50F60984D948FC1A07C73DC\
E87010000000000000017A9142E229C6A86ACFFCDCE7F5DCCB515537F40BE914187010000000000\
000017A9149F6171A994B119E57C866612B0EA5DCA68B6E0B587010000000000000017A914FA42D\
1F7B691880294026C6CC58373A85ED4B70A87010000000000000017A91418D3C57BF5018B441D50\
DF5BF72EF703343F9DE087010000000000000017A9146A68ECB00BB8A34B424251F4485549461B1\
BD13387010000000000000017A9141D9230736FF36D419AF62C1DCA7A93E5603CB7698701000000\
0000000017A9142B5D677798CDB36AD42369D396663F8BA9E4541687010000000000000017A9144\
64ADCE08EED3C1DCCBF919B1890E041F9102F9A87010000000000000017A9149F1AFDDC8FB58434\
A5FE3FB7FAABEDF9E70B5BDC87010000000000000017A9141BB3DA73F8DFCA1D4CFD6C454EDDE56\
A70AABD4987010000000000000017A914D8281D2B6FD392BD79EFFD9620125B8FDA2634B0870100\
00000000000017A91465E7344956D5A50A298769B958E9F90A90EF4EE987010000000000000017A\
9142E07EED544BEC450A05B1BB4A25DE1F5C28582D487010000000000000017A914C5C676F49DBA\
EB6813CA5311C7791ED0804B453687010000000000000017A914EB4E128700B7E4B53B04833487D\
3A589D109D82287010000000000000017A9143EE5791D0CEF3BCF8AEBB851589CB558B80672C887\
010000000000000017A9148D01B86B862F87D6554326D029D075A3F1D6CE4387010000000000000\
017A914AA7483863F9B48468A4E3B4415A1A2A6CAC44A9787010000000000000017A91479E59F93\
264D7A43E260112E48A678ED8FAE864B87010000000000000017A914E838409C15E8ED33F8FF82B\
1DB5792CBAB15283787010000000000000017A91457E27A4576D7E3A5DA79FD6A8A6817F5C7224E\
5187010000000000000017A914D0CB1D339D35B145FA03AC5E6DBE15CFD739AD058701000000000\
0000017A914CE0DAE60944EAFBB43144FF2ED78AB38840D74BA87010000000000000017A914C89E\
260FE5A340826769EE5F1D7514AF4C34FF2987010000000000000017A914A62C4BCECC6CC13B4B1\
5ED8163776B72BAA4486A87010000000000000017A91428C998BAF132E13B1FA8025BCE00129ECC\
5A00F187010000000000000017A914C89FE790A3BA4AC08CD4B4B8F6AC6C153A0FDB0C870100000\
00000000017A91429C222B957CEF67957AE8C548AAC60CD83A4AE7B87010000000000000017A914\
4387154A18F46566D82781E37D667F607894D4CB87010000000000000017A9144AD6F5C253808C7\
9C404E6431D724472E2817BD287010000000000000017A9142D05E2F1672BBA10777A0CCB1FCEE9\
1022B4F04E87010000000000000017A9149FEDD4064EC521F05C1C1AB5A180931335AF3FD287010\
000000000000017A914228F86BC997489FB0C1F74DFC667B8030A5EBAD787010000000000000017\
A914FDCEE997413BBCA29C8884B5FF853B952C1ED97487010000000000000017A914B31483DC4BB\
FB0ECF5B09986385BFC234CB2A14587010000000000000017A914A27F7620A9BF32467DFA5FCF00\
CD7C7BAA84623F87010000000000000017A914B5EB7CF0FA7E0E74FCBE98AFAFFDFD4A2601B8DE8\
7010000000000000017A9142801BA05FC10935EF49534CE31A3441D2B6F0DAE8701000000000000\
0017A914EE71C8303CC972AEB959D940B66A0B3C1C45EA0387010000000000000017A914D3EF8D0\
28D3467670DEFB25CBF4B7FCB225DB75E87010000000000000017A914C1D7402F5CC6FD0D81A7EB\
FB89AFB1E5B2DA433B87010000000000000017A9149DB933E5491C00C0646987E9F77480C85851A\
03B87010000000000000017A91454C434AE8210B5585140974F9BFE98A47012C30F870100000000\
00000017A914852D57D13F9FEED4769958D8C7696B701C1BF2D487010000000000000017A91406F\
96851818E0D76C4B2BE28509679478AA8BC8787010000000000000017A914DA333AA56F867050EF\
F11990F156239DBFF1B25187010000000000000017A914D104ABF7FB3A7E92D51154FB471BEEFA3\
4340C6187010000000000000017A91454276643610FC3DA414FDECABF966F563D3AABF287010000\
000000000017A91463007EEA8786EC0B91C4A141726CAEA662D6ACBF87010000000000000017A91\
475EE24807FF2A35D325D5456C53595C27D083E7887010000000000000017A914B200E7B206EE82\
F1DD4B1469E2E4B933C085770587010000000000000017A914AE1A711B63F501A2C91CB6D33437F\
CA7CA9C5B0A87010000000000000017A914763B11EE745160601814207F4A230B8B915641378701\
0000000000000017A914726F1E2295D78027470126FD56CFF61F73FC36C68701000000000000001\
7A9148489CAF5559E23CC47DBE21A90F57AA8A3EB55C287010000000000000017A914D360685111\
C99041B1045487431569E2EDF8F46A87010000000000000017A9149C59762215EEE7EB7E1C10882\
C0D05E7A89615F487010000000000000017A91405E77423FD9E8E112ACD501844B5D27E439859FD\
87010000000000000017A91436E026B2FF527B2477097B1E868AC59D5DBE1FBC870100000000000\
00017A9143BDED6FB272D492129F4DF9FAB11669598C7707787010000000000000017A914CD554C\
8E89DE39A640214057EA8265D583495C4D87010000000000000017A9147F3D5D6D726C67FEB1C44\
00EBB9E74AD60EC32C587010000000000000017A91440AE90E1506B37C0170C9411CE7A66975171\
CA2387010000000000000017A9141740768159D9B689731EC249C69BDB0DA89C67A087010000000\
000000017A9142852EFA320C505BD67F5E9C041E63E6EF244F3F487010000000000000017A9142C\
82F1EE0BB5E1D880B013C0E481DCC3D90732D687010000000000000017A9141EDD2B22D2BAA3D99\
B7DCD301688BEC2188B745087010000000000000017A914A27CB47BD3DAD6BBDF933EA1C85DED13\
9163E5A387010000000000000017A9145E8C03F8AEE1C48EE8BFE1D3427C2368419DC7848701000\
0000000000017A91473A7A217C81C10A6C9668807F75B7870A11F813387010000000000000017A9\
144EB2F576C766D0EC232726DB74368ECC2073285587010000000000000017A9146A3CA944214DE\
B2282560F0DB5A5E533BA285E0187010000000000000017A9148661F904BC4454DFC6615442C0F9\
1E29F4B3C23287010000000000000017A91410E1E8C8176BFC5AD6DE6A9DC90A4F8598A61DDF870\
10000000000000017A9143CFD82DBC612C04AB679C25EA4F7E5A0552A0A0D870100000000000000\
17A914ABED8054940DE09333142A38C06ECA38BC635C5D87010000000000000017A9140BD80F21B\
B6D8E0261409E5D0532BFF43BC17D7A87010000000000000017A914B6BEF3A79D81B0A8923E9431\
701B1C81974848E787010000000000000017A914A5B33C26B1154A21A68577501F7EE9B83A3D3CC\
987010000000000000017A9142B3535A7EB358C66873B7A27AF40587A1C85058487010000000000\
000017A914B4C82D525A1B71285E4D95AB0BCCB6B40269521D87010000000000000017A914EC216\
CB399E2FE10158CFA75A9D10CAA4A74DE8C87010000000000000017A914196F4119E52EAC2F6582\
2D5B100B9BFB61899EE787010000000000000017A9140014D33A73089B42ECD1F9F65BDC41CCB7C\
610F887010000000000000017A914F22AF3BA2947E4E5E001A507E69302852DB623C68701000000\
0000000017A9141F5E3A527A1F32EE6F72EC855622FF95F5D1956387010000000000000017A9142\
174F3CB5F4B0CB204A69645E90E14F1945D615587010000000000000017A9146D41A94A0958C484\
734021B793F031638031110B87010000000000000017A9141B346BD3A2B5D50B36DCC93A38FB1A4\
6F4AB913B87010000000000000017A914EA74A8AE7F4A31930AC2707991639B3EC2CBBE87870100\
00000000000017A914D54627A1C29C81A08787C95A5D74519CEC9F9AEC87010000000000000017A\
91475E48771B18B925763C7485D8ECE2DA795D79B1A87010000000000000017A914D44FB3809B38\
D4A16EDEA7A2986D664B7BCBDE4287010000000000000017A914839EB670C1D01AE7E5EBF1F2243\
AACEEAC48876F87010000000000000017A91404552FDD7468BA9DF66470F834DCD693B7185A7987\
010000000000000017A914526712BB9949E8D131BE93DBDDF1F3BB4EC1B7DB87010000000000000\
017A914AE68F2A02759FD5AE0D6D731BD830F1BB80328DC87010000000000000017A914346CAAA6\
E23783B8A6AC25B15DB9633524ACF82F87010000000000000017A914A0A2022B787822CABC12506\
9659AF19C312F249687010000000000000017A9143FEFBB3A6A5467BEC814670AB5E6577C00FBC9\
3B87010000000000000017A9148679DDC75C88C2AA83A937E62F352808055E5FD88701000000000\
0000017A9144AAF7DBF67C244B5959001F98D945770EE6FFE1887010000000000000017A9143134\
6D8D5CE0F7299529E6A435CA5B885609978D87010000000000000017A914CB0A630B2C8812E001B\
98DE881F119F914341FD087010000000000000017A91440FBC5D11344068C50678E8A5C0047FB0F\
68C88387010000000000000017A9144213EDEDC7D7EAFAF2576C4E6C233C6454F52001870100000\
00000000017A91449851F89F6271F4A4DBCD09A9810786DEF50D04E87010000000000000017A914\
0FA759C29CF91516DEAB4E1FAF0EF40EF58B055587010000000000000017A9147D24774D381F492\
4BEDBDB4C571148F56AF3370A87010000000000000017A914C403ABEAFBCF2A60C33A06FC7F2264\
BB69D9E50487010000000000000017A9140373DDAACA0DE35700B3B616AE9EBF42135E914787010\
000000000000017A914BBA8C61C66900F1D9C109112198DA94031B15B4387010000000000000017\
A914392BFCA5733616ECE676AD646C49959F27A0B9A987010000000000000017A9145C78EA3A349\
92D4D798F0CDC87D168BB523C78FC87010000000000000017A9140B4771683952D954845F014EAA\
2EA8C9B470229587010000000000000017A914977893E7F87EB0C1A515D57AC53F35517DDA33048\
7010000000000000017A914AEAE7B0FAFA29AE1B4CDF1E24A0DFAE8923B79968701000000000000\
0017A914E41CA6C9EB54746E8188610E1633AD0C072543F087010000000000000017A914B874698\
4210B5802EAC98D2C33688CB1A70BE30A87010000000000000017A91471B104D4DB00BE4910D84F\
33E2981DC58F687DE087010000000000000017A914FC35D42141BDA5199BF12703D7F0D16677A03\
10687010000000000000017A9140784BF3462AC6B6F0501375317BC1CF5775C4A8F870100000000\
00000017A914EAF708E5A9670F901B4C8F30606180190DDE8CFB87010000000000000017A9143C7\
56DEBE682AD861217A6D450B6B9F4C1B5FB5287010000000000000017A9145CEA445675E69C8A5A\
5F21452605123B8547838487010000000000000017A9141EC079831F9BEA565C07F2F8FB4337B9E\
6DC4D6087010000000000000017A91401D188B9AA65535EDF0AEE3CAC6B47DDF0ED95B687010000\
000000000017A914F96F9B79576DFBFB00AABC1AB355A8D4650A75F387010000000000000017A91\
4D9890566B7F752485A76DFA65B62E594116B82CC87010000000000000017A9144E13F7FE37F6E3\
709FD6CF64874FFF0FF784201A87010000000000000017A914555828820C6DE87009AF4D117B5BB\
E63405220A087010000000000000017A91423E3F0A7DEE15E5A166349BBFD45697B4E0EBAE58701\
0000000000000017A914901C519C6D2EB60C4531FFE1D763A5DDE2AD358C8701000000000000001\
7A91445F62B73EB42F92851B253875FEE7E666FA1D1CA87010000000000000017A914E674071912\
C06C1153C7374721467BB9E35BCDFB87010000000000000017A9149B6F57A4A42198DD7DD47E1B4\
64818EECBD3C39F87010000000000000017A914D05F5F9B971D36D220D51F720F8AB5373A2A5571\
87010000000000000017A914D48332EF264CA82820844159E06D89A7AC9E33BE870100000000000\
00017A914207F527B8EFA9536C994CE9265DB6494384DF3CE87010000000000000017A914D149C7\
E5F5926D75FB29AADD521EA6B83311B53C87010000000000000017A9143D07AC412614E2AB7E0F4\
69DB566E00F439BD6F987010000000000000017A9142741657E3DC7C3CF7596ABB60EA2A514A523\
97DA87010000000000000017A9145A148CD42F77DE92B73CCCBC4D00D23DC531279B87010000000\
000000017A914C66D1B849766931DE12161B0DB77900DB16DD49E87010000000000000017A91457\
77B45C19C9C173B03ED4D4AB58C2B8F736CADB87010000000000000017A9140E63DC4D981F2AFAD\
ED6CBE005279FD69C31CA0987010000000000000017A914E50BA345281F259EC1C01772B8F76DA0\
A4DD15AD87010000000000000017A9146C422317E44DECC9A9DB7BCA255E44EB7D6E14578701000\
0000000000017A91482204A671713CCDD95D0369367DC2ABA2FA686F187010000000000000017A9\
14F7C936ADD2F97F20FB69C894BF9F04564631DB9C87010000000000000017A9148C4DD4C22D87E\
DFCAD32F5D80558EDDFC2A545ED87010000000000000017A914DBB3024CA31B56F4D6F756328631\
FDB588D0C57487010000000000000017A91466C6D56531C5BD18876854F96E7D6121162628BF870\
10000000000000017A91476DF223EB56FE9DCFE61118286D71C5B5A13F03B870100000000000000\
17A9144CBE03464AAB92CB0B3B787732049AB9C10CA4C087010000000000000017A91421D79C913\
BF392A2DD1F088840886F3BE2482E9B87010000000000000017A91458E27A2FC656FE68013F0AE7\
4238BED4179E832987010000000000000017A914A21333D2D0EA60DEECA3A99DB2B5F544683D4BC\
B87010000000000000017A914469D33266E0131074AE25E73405297FD111865D787010000000000\
000017A9148FFC681A186D7CA02EFE7AE9421459EAF86F133087010000000000000017A914BC9FC\
F5F534FEBAC3AB083082FE44C1D98203EF187010000000000000017A9141C7DEEB6BEAE59A0B53F\
53E5476233AF928D871E87010000000000000017A914138D3DF8BCD233E80B05971090EAAC31EEA\
3AF0787010000000000000017A914935D30621D55BFE95D1F50641B58FDACFB89B59F8701000000\
0000000017A91436ED5BCC8498D4D00B9FCEC86C7A621E46E8293287010000000000000017A914F\
911E386D8818BC517DB45BCE4FC26F20EDDE09487010000000000000017A914E7556C786C9C1529\
41C260078FC9879DA01BF6CA87010000000000000017A914398C0718A57A0AD0D9A702063480097\
A8E98BBDA87010000000000000017A9141B059FD36C09F4C743F64CA3B7C3275908B35B24870100\
00000000000017A9148DD3E8D8A795FB6A8C981DB0C9085A371FE7DE1F87010000000000000017A\
9143CC34D15DC1BC66839BB75456C4D236C5E98108E87010000000000000017A914B2887E9E87FD\
DF0C18E1582B81849B9BBE94909487010000000000000017A9148BA5107CCFCB290EEEEFFDB9CC4\
F1AD46AFF133C87010000000000000017A914945C22D3CC9F5988DD98F20872E36170B7BAA32F87\
010000000000000017A914CB71A03AE28ED931FA53F22EDD0FA17C2AB2608187010000000000000\
017A91465AF46DE19CB85193BD404FAE9E0FFEEC2FBB99987010000000000000017A91450A14636\
5F7F2FA0A07D2EA7EC27C5C5F4E751F887010000000000000017A91430DA184A3865A08323FE663\
4DA6EF24604D9C44E87010000000000000017A914F2E431BE093EA02584B43C3B6481C418EC9633\
9787010000000000000017A914D0C70C2C2C79715F306A27D839384935BC315A7E8701000000000\
0000017A914F6BA7A8069E17D12B4B698C60B6BA2CE1831F89187010000000000000017A9140D48\
AFADBF34E679D97FE8C439DC3F01144A200887010000000000000017A9144E093C9F28E7A8228D8\
D273E404D629B3507BFE487010000000000000017A914228B11166EF097A04A5F9078DD35FD1870\
81894487010000000000000017A91466629A128795C04A9B60C59F6C3FB0C92703C16C870100000\
00000000017A914AEF726A18FC55A3217A637ED87E18F9E8B9FC14687010000000000000017A914\
296F436AFA3045E11CE6A1A70F5AFE36A51E1F4487010000000000000017A914E55896EE4C2994E\
1A92AE697351E34930B5C5E5487010000000000000017A91489A644798729D6525D948BB61AFFDE\
97B572862987010000000000000017A9141CD3D6C0E7BEFA24043DFD7F8FBF95975F6938B287010\
000000000000017A914EEDA49C3E8753949F728CCB251926151BD3F2D9A87010000000000000017\
A914F76565EE45C7D3CFA31E2D2C5BCC86FA4A4CF2B787010000000000000017A914FB221015786\
8DC2D056DA31E35A0BA26A31228F287010000000000000017A914A3B3AEC36B3741BA73FDC60C2F\
124218632C9D6B87010000000000000017A9141516293A7BBBAFE2135FF3E492C1E36FCB2976748\
7010000000000000017A9141BF847E083862F4F4C362078A3EA092FD6E31A8D8701000000000000\
0017A914BAEFF91172168231A39D940B0E7F834115020BE187010000000000000017A91485A47CA\
436B43A1858EA42FF5DEC77B41D60F0E587010000000000000017A914031C2D3E08675EF373ED3B\
B3B7A5EB542AC721D887010000000000000017A9145ED8ACFAE442F22D5BD29FC2619EBFE3CAC34\
93187010000000000000017A91404936DCDB5741694346B47B4E901CB22B9170400870100000000\
00000017A914EC0951CCEAC343FC8054450F95A2030E75E831D187010000000000000017A91402D\
0E5002891FA70010F6BD9E77057D57F26B68087010000000000000017A91449E7361105D60958F1\
9E07D5BA3C70194074CFA687010000000000000017A914D68E938669B71F49F77D68C6CDD296EA0\
A42BFAD87010000000000000017A91453594F8789143F7BDE56CCD2E8A9809ECDAF230787010000\
000000000017A91469ECF70CD7E00F57D53AB3E990364E71B4BE911287010000000000000017A91\
49C650F8E21C0BEE24C91126C17EE37C13D7F396887010000000000000017A914DB01A1658F6549\
C038669244A7B4CE5BE2B732FB87010000000000000017A9143B2BA7710D30E4221F98EA1D5C534\
E4FAFCE1BB287010000000000000017A91490DDA6F540AC940237654193CE347BF9CA020D078701\
0000000000000017A914F75285E92D0610E15A229CD3DA14A91FE4C534C98701000000000000001\
7A914EE17A675ABF92D728CB2885698B3495B63AFDBA087010000000000000017A914300C60E934\
59D52D18F8C26633ACA18410122C5B87010000000000000017A914A705BD69BDE6E6E1DBAD98C5F\
3ED15C8428E57E387010000000000000017A914D7F120FC99CEA261AE32EFFD81BAE2EED586BD57\
87010000000000000017A914B2B74D0F547581BBEDD14FCA6B9272DA36581D7C870100000000000\
00017A9144684383B7FC611517EAFF85992BDEC5E5E39F5D987010000000000000017A914B9DD71\
A74F419DDF8F82E07757EE8B318BC2BC4E87010000000000000017A9146E701669F77BA74C909EC\
CBC7B76A4B3CF1A8D3D87010000000000000017A9142C4576787E39540385512F4984AD3CEE3EAE\
E5E787010000000000000017A91425084DC2B69B4C23308B8934E4B171D65148274E87010000000\
000000017A914013DA253911807A6B9CB562FFD190AAF9218763C87010000000000000017A91447\
1C68E567FCA9EBABB4ABD734FB421008A88B2287010000000000000017A9141DDB609BEA95772E0\
36515623D6BE81CFE4B5C1E87010000000000000017A914D4F116A72F451E765025151E3C2240AF\
87E8733087010000000000000017A914444D73407C8B3513754ADCCEA7D8209C2CDAA5748701000\
0000000000017A914762899DFDC3FA28AC3949EB6E0423E95266BBF0287010000000000000017A9\
14F3BE54FDD28545C2313CCD40E4834CB9C677202087010000000000000017A9145750CB204D72D\
70607A26F476E5B0B7D74D4DDF887010000000000000017A914AB8705CDE4D94F8C25B45A366880\
DCABEC023EED87010000000000000017A91422BA410DF598112AB03189DBD401231CB28CEDD9870\
10000000000000017A914EC647E847E60BAE948B52762E774C5C752FCA78B870100000000000000\
17A9144CD1975A3C1B667AE413CCF2487122A1C4E6CE0587010000000000000017A914629755514\
CE31B1028605603F01AE6EA63B2302487010000000000000017A914FB6D10A59C46049588CDF9DB\
48B680392D4CB33387010000000000000017A9146CB305AD899721D91C09C114F87CA17CDACAE56\
787010000000000000017A914B48F0F733A446B96B5455DD48843CA8ECB2D97BD87010000000000\
000017A914C046890455C2D37ACE1939C4A7889521D6BBEA3A87010000000000000017A91426B7B\
52FCD74EF82D2AFAFF00859E9730C14CFAA87010000000000000017A914EA02274EC25EB66280DB\
ED2F515C403E2A8BE3FD87010000000000000017A914AFD5281573C8D6B593429DE6E4355681002\
5C93A87010000000000000017A914FDE18950C3EE6F4005B9EFBA92227D94488B49D18701000000\
0000000017A914AE74B41DEA4638D0A8341B0A13D801B8107DE71B87010000000000000017A9141\
944ED6089193AB9C66590A1EFDE40B4B0E6F5F587010000000000000017A914E0E9BB2384CC08B3\
3737CAF455033500CB51A6BC87010000000000000017A914080A62AA5B73FDAE1B2E290B7BFB3D3\
DD4034E8987010000000000000017A91477B0C9905E8F2DC27E17038893E1B7115620E383870100\
00000000000017A914A027366E953B4C0748F94CD678291921F2F4903787010000000000000017A\
914A9396AD2BCB105B3D63AF5857A5927E88B9D0E1987010000000000000017A91457E2E31FD7AE\
5BE30EF596CDE7A5F2636ED4DEBC87010000000000000017A914E89CDFEC56FDA1FA09C4FDAF1D0\
9E6B4699D7E0A87010000000000000017A914C7E59EA6463E843B7D04F39C1FE451A9D162EC9A87\
010000000000000017A914C85EAD9DE806D26A0145B1B63C1BDC3633800D5187010000000000000\
017A914D97B037CCEBC2519C5401D54BCABAA935E25CD7087010000000000000017A914031886B4\
95504BF450D6C7FE8A56F3C0784C733087010000000000000017A914451DCBF51175B9B5C343878\
876277EE4A472602087010000000000000017A91461F81B0CD1AFFA669720136124210EBD0D10F9\
8D87010000000000000017A914BE2BE776DAE1E150DBD816B8CE5C46AE172C9C878701000000000\
0000017A914D6384851C1394658AAD99FC544ADDAF34610178A87010000000000000017A914B316\
E9307F595F2CDD7AB952557543F3C59B611887010000000000000017A914B6DB03FE246545CFB99\
DAC45D6E824106027743F87010000000000000017A914313176485A14E94DE630FE1EC7FC62BE16\
6AAE0A87010000000000000017A91444CA0323745B4FCCB349F44B5AF2EFC8505F8699870100000\
00000000017A914E1F39ACCEA31E0DB4C3D055369A2F3A12C86527587010000000000000017A914\
231411A8B5F307133F3EBC5A7AD6305CDE6AA00C87010000000000000017A914F6CC7685BBF25C8\
B982A43F9852A8E3499FA992587010000000000000017A9147D2D5D20D0B5A7C6DC007F03566DBB\
D03ED904F587010000000000000017A914043CCA8FE5D8CF4EA8994D3A5F804D02CBA5EFA787010\
000000000000017A914057210C5CF7626B952B72CA48A6BC03D52D5CE9187010000000000000017\
A9141319EEA27EE0B0F018CAFC2E9BD43B4F5DCE886587010000000000000017A914DCA74CCC649\
50051FB6781433DBBBCDEAB89BDC687010000000000000017A91444096F296C027E4F32E5EFED7A\
67B9B8D438D9FF87010000000000000017A9143BDEEE1B55AC12A306CF602DA165B3574CEAF8608\
7010000000000000017A9141E99990FECB064D709FF58912A169B92469DF6E88701000000000000\
0017A914B331FA01BC905E2B11FD4111E404B12895FBA2E987010000000000000017A91486A0A06\
E2B8FA1A7462CC2D1F8499D78EA29DBFE87010000000000000017A91478E1D17E6271CA51C01D22\
5C579FF0379C01A65287010000000000000017A914CB4570108C6F4601BAFDA8A64531E5E145196\
EDF87010000000000000017A914902510E5B965B0D3F81B4E2294C15CE10D9F50EF870100000000\
00000017A914479055639B705D21502E620AC022A38B9526727587010000000000000017A914BC0\
FBD4C3A06990858245330C96514CCA1EFBC1887010000000000000017A91436A66342833DA032E6\
06582477C979EA32F79CBD87010000000000000017A9140775C3B1E836CAA2A91CD581B6625CA2B\
3CF8C8B87010000000000000017A914AF86148ACBCF5C47D35C260049F5C1EC0B27A3D987010000\
000000000017A9143559552FBB3246795F40EFCACD7C120B6210E70187010000000000000017A91\
4663C33D80DBD58375F9924C92C45EB6DAC898E4687010000000000000017A914AD0052C34B0076\
925C2E24C2ACC146740BFCFA2C87010000000000000017A914ECBC6F3EFBEBF0ED39E5E333E535A\
8C5DE2613EA87010000000000000017A9149CFAAC6C2CEF96472C8C2D61AFCA9D999DF1A9E18701\
0000000000000017A91436953B51C58CE6F0AF9DF423E3C8CD7D6833FB8C8701000000000000001\
7A914276D270A367D4CBE15C93825755A7A41828E063B87010000000000000017A914AADC7E619C\
15DADB3F7A1F703B2E89027C0C6CEA87010000000000000017A91412DF377611F42FFCE6A61AD31\
6556C7175A7BF5F87010000000000000017A91430BA0C6DD6FC5BD4566B0306C90D30660D4EE2C1\
87010000000000000017A9149B437915B5A3AE8F51F2A513CF36F5052B61CD24870100000000000\
00017A914B1A72A2A23FB3F527120857D7F1112D40EE36E1587010000000000000017A914031D4F\
B7A086E7B313F72F1D7E44F825954E31F887010000000000000017A914534758C6C1E9E7640AA0F\
361B0E31541AD1712E687010000000000000017A9146B3D27986DCCE07A8F1E03DB6C2BFCECA876\
BF9F87010000000000000017A914C19BC7D7F73693AF70379CB23884393E29F2550087010000000\
000000017A914C1F2FCE6C290142AC856D5E313186BD057B7677287010000000000000017A91427\
1297BC65CEFCF441CD87AA99609B69D1616ECB87010000000000000017A914BE515FC0CF45AB834\
BDCBF7979B859BC902A798687010000000000000017A9141C5BE1805080CE508149671B346C2A83\
66D8B8A387010000000000000017A9140D89D39CC22DA0ECC0E797183DB5B787299AEC0C8701000\
0000000000017A914093F0B5BF54567C671A243BA93927F8422D1504887010000000000000017A9\
145DA3D7DFF33D81ED5150DEF461859BB3F049C67787010000000000000017A914506B899775935\
447E343B077F0A40D862A0B8CB487010000000000000017A9143E6A8D5757D3C674753326EC3DF9\
B4CCCAF1835087010000000000000017A914730AEA819F31FFCACE31BE9D1B4CB3D1D0C60E8D870\
10000000000000017A914E8979777479784467C986343FB6345D0DAF121F3870100000000000000\
17A914D003C3DA301D31081E2A3B10F05B2703AC6A312687010000000000000017A914FC0EA893D\
050FED1B0C5E33DE4C6660CEF81A84387010000000000000017A91462F9876C90DDCA33ED736AD4\
E464C795772730FE87010000000000000017A914CCAB919076EB0C7EDE631E9F5BBC1DD63064896\
C87010000000000000017A914354C7A82FFB642CA0C12D594A339D8E17FF21DA287010000000000\
000017A91483B3DCBDF2CB8EAF16A5B115650F4592BA71D65587010000000000000017A914BEB63\
74044C64DD8E4BB591C1A857BB7DF12ABE387010000000000000017A91400DE029BB7FEAFDF5E05\
04BCF38A1F51C960B85A87010000000000000017A91408B256B3CC16B0FADB4A9A367891DB8AFA9\
93C3687010000000000000017A9142D9BC3F08E83C4BBB88EFC8A2155A0BE289992418701000000\
0000000017A91423C74B1C1101ADD2174E6C4C552C42EA75934B4787010000000000000017A9147\
A9E159B7BEDA88753316DC742E3E42ABBAA014787010000000000000017A914E5CAFFCBAFDD8980\
77DC8323F66747BE9A6AA99787010000000000000017A91402D53B902983134591D82260B10A0FE\
4CDDAF93787010000000000000017A9146872E2AB643694B3F602313E09CECD7B575EB33C870100\
00000000000017A914D9578CCBDD31A4EA80A9C4EFBB6B15126909AF7A87010000000000000017A\
914B6578BBF3ACF3F9E3D6EA4929ED9CEEE2411020487010000000000000017A914F9845E352988\
DCC7B6CEC8C93C639560D620A76D87010000000000000017A914125D8CC8B512DB3000B230D88D0\
F9B8783702FBB87010000000000000017A91498C4C53C5C2CFCAB20C542612D769B338032F4E787\
010000000000000017A914B2CC1565BE20D89404DE728CE26ECA1DAF4D9EEE87010000000000000\
017A91496ED24A7BE1F8B3E924C1F0C16EA9430012A0A0A87010000000000000017A9146216B378\
B6BE9D88F47E8D532917C3CA9153F2C387010000000000000017A914EECAE4DCDCD0D3C83EEBCB0\
181D3D7031D004F4487010000000000000017A914875F0D33BAAEF86D950BFF87E534FE5EFA8F52\
9B87010000000000000017A9143EF181F432809340590EC84EEE58D16A938599B68701000000000\
0000017A914CF816C237284C9EB93CF5861E010DBE63F3D3D1287010000000000000017A914ADEB\
A66EAE21D852A5A47215265977D20441203187010000000000000017A91490B2C86B5251E17CA69\
EB5DA009F2658184634C287010000000000000017A914513A4EA7EEAC277800333C3EC8EA7519AB\
0E7BCC87010000000000000017A914B74FA320BD1E175C04281B220F4B7D294E1D2C5B870100000\
00000000017A914886A14D4D5CB10675B54ED8CA12CBD32FD0F70A787010000000000000017A914\
C977D060CCD0B3A26783994487C513537D8E675F87010000000000000017A9146ED1EE24BACD7AB\
2CEE61286263346DB826A2A0887010000000000000017A9141078A8EFF1BE877551A1E50222D6C2\
40A4488B3D87010000000000000017A914B68CD83B7D740A545A73862598833437B70EC94887010\
000000000000017A914C341F11883FE4DC6E53F1213D4E6E1437CF22C6987010000000000000017\
A914D8F275F38221CC2C4F1EA3836D5CE1BB18C91B8587010000000000000017A914F1703EECDD4\
A3CC0FBB0E1F9A27E3DAE98D5A3C987010000000000000017A9140650E23A7B02F22175FD0005DF\
4B6CC287949AC787010000000000000017A91436899901735892FC51621C76C8D444A2A9CA52C18\
7010000000000000017A914AB751BF4B58597C23A6BEBA857B12A78011EA9998701000000000000\
0017A91447B3BACD8FF1C9658B50A7D2ADDFD437EBA4FD1487010000000000000017A914500C04A\
52177D8901C7861BBD1D7BB6633EC528787010000000000000017A914CECC0E568B989804718739\
9CDFFCF1630BAD25F187010000000000000017A9143CF9D1D6FF946E9DC0B69B74274C8BB1465D8\
02D87010000000000000017A9143FCFB21DDDD39579B6F0B9353CE89F210730B992870100000000\
00000017A9143B01C6E9A723BD1CC680BA50EE1353C69959C49B87010000000000000017A914E15\
E94D4D85E381C59D5352623DD1C05B75F24E687010000000000000017A914F5ED3C5A539A9762E2\
03030ECB4B39BED629294987010000000000000017A914380B9146D942EB0BEDFACBDABDE1F3C04\
7ECA79087010000000000000017A9146E11457F179A1E58CE7065171F4C63417CF6425087010000\
000000000017A9149C9EEA72582F452F8AB6A5DCF6B2B4087335B34B87010000000000000017A91\
464446207E438E222DC2F24C82207C720F485F61C87010000000000000017A9143E7C569EA8664F\
1177A37A65A2494ECD32F0B9E487010000000000000017A914DC2B05B8C180656DEF9489392DD96\
B55FF9B72FF87010000000000000017A914CFAEAAD250D424536C6412D2974B4C5372B2800F8701\
0000000000000017A9144A75B6A9336BF8D2157C759BC8A969A29CE49A838701000000000000001\
7A9141F4E3C0F78730BEBDA205D4EB23C96769738B39687010000000000000017A914D9A51F0C49\
3FFC43DDE673546AFD966F04B10EAD87010000000000000017A9140D56C6310A9D33323FDC977AE\
8C691341A745A3187010000000000000017A914FF3E77C7BE4950F02B388A45A4C8B47C7AC591B9\
87010000000000000017A9149C528365155F0DCE32771EBDF0645D81999547A0870100000000000\
00017A91480398771ED131AC4F6D0689D990F6285553D3FB887010000000000000017A914A149CB\
0FA195829BA499C5DDFB3918B2D1AFF88E87010000000000000017A9143DFCA0855718D8181D814\
958F6FA4B7BBFD2E85C87010000000000000017A9148AAEDFD98253E809653EECBD06E9FE5720A6\
A5D487010000000000000017A91484685F3C3CBE55437796EEB0A05DAF510E55FA1887010000000\
000000017A914724D6BD788C77719471C94B9955420DDDB0FC8BB87010000000000000017A91402\
758F1F503550C08842C85BA757A4A493A981EB87010000000000000017A914261500A78A0D097AA\
600FE902200489E62600F9187010000000000000017A9149CD3919DBDAD08D8CE1F3E3FB1810E00\
91A93D7287010000000000000017A9145006FA2542EA46A62058BDD66414FB65DDBD6C298701000\
0000000000017A91446717A2B72DAE760CD56C7543CC19947F34949EB87010000000000000017A9\
14942354EB2837377ED745A503D08FAF82E057359A87010000000000000017A91493C6144A992E5\
F96C0ABFF3C30E2EAD427BF4CDE87010000000000000017A91449723DD4924A9770FDA9F222F43A\
905FAE78EB5087010000000000000017A914AD0DB1BEDE9F11B54795EFB28B57203BA8BE9317870\
10000000000000017A914EC3256E255373B8C6C937CE77AC82953E23826F3870100000000000000\
17A914B922F95D7455C69D5229B6374057569C3CBF88B887010000000000000017A9140719F33B4\
B936A0DB53948DB02F717FC106FFB6A87010000000000000017A914BE1A645DCE546A0F11CD2030\
89E7A00A60E9E26687010000000000000017A91442D4494B1326A5C06D814E3874F36898143A0C8\
B87010000000000000017A914E0219E152801074218BDBAFE9900144D2904D55E87010000000000\
000017A914101E4C9DD6B7A6DBB6BE200074D51C611414392787010000000000000017A914837F6\
CA1C6E64BC05C3FEAEFEEB12CCF5B41408587010000000000000017A9140C93149B3D0C068672D3\
B6F06612DAE99F32B5DD87010000000000000017A914F676DCC19AADF01396E0243F10565BA489F\
A070087010000000000000017A914B587EBE8CDBE105617A96B5E83DF7F4FF42560408701000000\
0000000017A91479A23B8FD06AFA0877EBABBE4185BCF92A4568EF87010000000000000017A914D\
737B9BB3C286F6A775E45C40C93F0CBE400479087010000000000000017A9146037176D79D4A96A\
436CA7CFDCC7B5F3661ADB2487010000000000000017A9146A0FEAAC0EAD5182D211DD0FD870655\
C6621989E87010000000000000017A9148BB5976B9859A7496042BE87721903605843C642870100\
00000000000017A914FD468F5749D0BCD45F3DA426A3E8E2787962AE6A87010000000000000017A\
9142136B54101C2F25BCDD6908B9FA09C805DD2236B87010000000000000017A91433C618A83585\
8FB72D5FB3F7F014415D923988BB87010000000000000017A914DA517D4ADA6EE0CABFB0F179058\
50456E1C2C2AC87010000000000000017A9140E7EFCE3F6A543974F5A8ACF470D16DB626997FC87\
010000000000000017A9142BC928BC83DA4CEF2E6A2B2FB0F5E2D13299474387010000000000000\
017A9142102CCB1C627CC3C0AC6EEF415511612D0EC35C387010000000000000017A914C6208072\
FB91FB90EE72976018133C7C58E3350787010000000000000017A914983F824BA709E1F5B2F453D\
623F9DF97107A624487010000000000000017A914537DBAEE41EFF90B075F9F10642CF02FEF5D59\
7D87010000000000000017A914D1BC4829758025D73DF692F162FF3760364D08CB8701000000000\
0000017A914BBF77DF62BA50BF23D3F39084EBA3355B1AD98BF87010000000000000017A914B352\
16B30EBA162B3749795E75F05F595DC898F987010000000000000017A9143D9EB732343BE945D5F\
4DD50AEC609C93BB7ECE887010000000000000017A9143CF97A32E7D6AD8D3380A13FD1B9386285\
53BCB687010000000000000017A914FEC1286C25FFDF8386EB382DAE65E95EC7FA96EE870100000\
00000000017A91410CF8F18480F1CC62B8863A65D1673F5428448CA87010000000000000017A914\
4AABF304AB82F453E6ABCA588706B82B1E1AD6AD87010000000000000017A91417AF448E656EC99\
B849184D30283EEA8F68F9C0187010000000000000017A914E777E41DA4B9EB70A8D46000718333\
6AC9387D8B87010000000000000017A914626B9627148EA46863234C8AD90A206F1BFAC5A087010\
000000000000017A914C4C1FCE7EBA6F795E284094DC4C85315E7B35ABF87010000000000000017\
A914DDBAC92117CBFDBD824B5D9262C353D8BBCD82A187010000000000000017A91496E1433F275\
01AA38BCE30A73A2B05615169A26087010000000000000017A91400CC4FDEE78F7DBA4AF02EFC48\
EB72366CF8C71787010000000000000017A914E8B02956A1126277DA2A260EC3F5E45C68327ABF8\
7010000000000000017A9147689E9F177B8EFF53D7AB837D41AC0B7C5B48A098701000000000000\
0017A9148F1E0CD495FAA5B0A7F482ADF6605E03D24B935187010000000000000017A914A9616AD\
12E055C6008C719033F271C378983F45187010000000000000017A914F8579AD9948E21A166905D\
D7723E0EEFF198F11E87010000000000000017A914694E3148E513B626EC9C223239100362BD0BD\
09A87010000000000000017A914DFBB7A79A8895079BB77EF534CFC37888A09F8A5870100000000\
00000017A91415BC0B4AF741302A19D258E0CA470D6F3A3B414287010000000000000017A9149BF\
24963C0B895B3EC4B504C28D2924AB1A4534E87010000000000000017A9147E7920FC3FD4F0943F\
95347533589A04CBD9C18587010000000000000017A9148137DF75E393F15E36CA4466091E1A7DC\
5B9985487010000000000000017A9147C3D492E71F01820BE217863A12B666EB8D4D1D087010000\
000000000017A91490D05BE8E74C653A450F84C8A9608119B130CBCB87010000000000000017A91\
49AF9DD0E9EB1B7B59F84516634AC60D04528A90F87010000000000000017A914CFB4B03BB3DEE6\
A9187EC8B6DB9FDCE9F14D17C387010000000000000017A914D242D03278DC8795EDBFB2AF95DA1\
1D72AF861F487010000000000000017A914D21B90AD3641DB1C3B92BE596755B0120B3518F08701\
0000000000000017A9148E74DDE04C9AD7491A794013D234D930A1A36E908701000000000000001\
7A914218E0C29D8C02FCEC2E3A577F14B4BCF3557983D87010000000000000017A91495A233AFD4\
2FC4162F86DCACD93886F16F1FF36987010000000000000017A9144B79D9BE9D052B442468EC74B\
B2B1CC97384AFFC87010000000000000017A9142CF679E9CBD680CCA5FA0E9B8A58F1B536699A18\
87010000000000000017A914600166E1EE39AE7E35BD35D3D7477557FACFDB58870100000000000\
00017A914551321E7311A44257C070D83D76995F25F9C697F87010000000000000017A914B795D7\
14DFBEA1A41211B44B9BBDB9C3E2DFB75387010000000000000017A914C49029CC72A67F29BD3AC\
8C0F6A03A8287DDD1C387010000000000000017A914F7D6D2D5FDF3C0C348B2E233CFBAA6FF7A5C\
0B3C87010000000000000017A91456937D1E273C6916EE4086D938D404F5CDC2D70D87010000000\
000000017A91480A07E8A44EAAAB027E8E85945933E559691FD6487010000000000000017A91453\
3B07BB67B6DAC448BE8E875792CD3A64E471B287010000000000000017A914D34DA492611728C5A\
02DD85AE991180715540AD087010000000000000017A9142A07C25F70C4F118CE7D31D33F437B82\
29E3C2E587010000000000000017A914CF852E44297FF4A50F5E8895C852DA2B5EADDC958701000\
0000000000017A91441664E462583E134622E85B02187005BBD0E8C4E87010000000000000017A9\
144AF1C272AA8AE6BBABD5DD2863F4F174F42C533387010000000000000017A91470E1A67FA539C\
82E1D906E6AE3CB887A04FABD5C87010000000000000017A9141575CE216147906896BBB4273C74\
8B60D4F9837D87010000000000000017A9145AAF833761727D0B4352EF0622C802EAC9C2FB25870\
10000000000000017A9143F2F5A9BBFA64F49F7E79D0375CFB989CAEDC685870100000000000000\
17A9148231F72C9DE9754A843C35BF89C437BED30146FD87010000000000000017A914B52BB0D66\
9A2E3AFC4F59FA47F93CF63BD9827D687010000000000000017A914F981D332B33AD55287F64158\
A2E8FC0FF1A32E3287010000000000000017A91450F9D23ECDDBC07C5472FE9926D5CEA89F2FBC4\
087010000000000000017A9146FD99C4B1F51D892DD9DECD3D1158AAE622D761B87010000000000\
000017A914BBB9F4C2EA2924688B0E8A53F1DC933AB84491DB87010000000000000017A9142ADE3\
F7A0864022C4B125C4D6ED13B4AD4FCC75887010000000000000017A9140461B9B9BD77C9DB3D87\
87E2C8816DAE8F6D912E87010000000000000017A914F3A5322BCED88C02C6D5E64FD28B573E909\
D69EB87010000000000000017A9140A2C485DF406710E559A419361A2BF718B4AAD728701000000\
0000000017A914A9377DAF3630AE27D3AF39EB97738B7B64267E1787010000000000000017A914E\
426F43746F9BC3016378AABB0E0AD2AD1D278B987010000000000000017A914CDB64066E52A00E0\
402001328B84D459240A9F3387010000000000000017A914E1BAE6DD5364343F6A653A6B2539F46\
557BE35D987010000000000000017A91463D59BDCE39ABFD2C6C8940817DF458D87B3535C870100\
00000000000017A9143E876A6E50AD95D5423E5C3D6CD13C2EFEB9529587010000000000000017A\
9144A5BE0AA30D3557F13D32B8FC00822E546DE684487010000000000000017A914EE8E261431BB\
81241E7781EE7F0E61B07428A0C487010000000000000017A914CD855A8EE390381D0CCD5C5A4B2\
A6DB8AA3193C587010000000000000017A9144795AF0DE4D407F60F7B1B2CDFE38B68DE85B45687\
010000000000000017A9141C67539FB02E6446ECAA6494C581817D3944625F87010000000000000\
017A914A86401073B3D870447339349F89F9DF0DB91C3AF87010000000000000017A91492744A87\
E0F15C292614E8A5D0645B6A44A962F187010000000000000017A9142B88C8D0E76678D491517B1\
6A81964FC38CFDA7287010000000000000017A9141E49D11388D1B1FB62C239446ED8A50056C910\
B987010000000000000017A9145DB25BF9A1436C41B4B7E19550DAA6832EA14F918701000000000\
0000017A914F23227BDCC0B4E3AE3F046D1B476EF77156BA2D387010000000000000017A914BDF5\
8F9F8673ABE63618B71EDEBF91CF9B9E1BFA87010000000000000017A914162CCBB61F1FE40A3D4\
E625B43F4E6B65562D78787010000000000000017A9147503D023ABC813CA88F99B13898DB6A805\
58C3F787010000000000000017A91420F55A826DD4D3B18F5569DCD96FCFA0BF7045DC870100000\
00000000017A914F10208018EC32D5654F5769958ECE89D4CC19B9287010000000000000017A914\
FFC5E2694AEBC6BB7BF81AE9DDBA3E396DCFE0C187010000000000000017A914F8875AF15518701\
B76A0C9ED2E5BA451A6EDE79F87010000000000000017A914E264C530EF780606BA665A44E28FF9\
6C14E37DCE87010000000000000017A914E3C99D0B9D830A3C1D5B7979EFF427E48C1C4C5287010\
000000000000017A914DF2118A919B77FE3887FDC5212B3240D88F7F91087010000000000000017\
A914CEA93475DB124AAC14E16A52FFB5A06E1466C22287010000000000000017A91436E21FF6AA7\
2F8130E5E672C750B753BE8F8A47987010000000000000017A9142BA75C18CEFC45968590854FBE\
CD080B0F30B74F87010000000000000017A914B6825E706E0DCE5774945F73757D3124C565C5CB8\
7010000000000000017A9147CBEA6AA6130EED9E826B0E66744D199FB16EB978701000000000000\
0017A914CDD10D5C79A5482242484465B17D4CAE53A5B04187010000000000000017A914CD4B51D\
155FBED6F64FEB6FC79A020E4E5752D9D87010000000000000017A914B6756294AA580CEA173192\
0530C2F91A6DFABF0287010000000000000017A91465A06BDEFA0E4B828DC2F4A89111304343352\
0F287010000000000000017A91402D6F7E3FF904CFBAEC5E4082C80F28CC709B1DB870100000000\
00000017A91421A92C6E434030CF7D24D0DF3F3502E92C0C217E87010000000000000017A914AFB\
644BFBB585412ACF71C23BD6E36832C05D82287010000000000000017A914DE0BE5ED2E55E22CCB\
814EE12E45A2E2933517A287010000000000000017A9142086F147BFE2DDA09D9A1E202AAAA7620\
D31291187010000000000000017A91453E6C930A57B33E8FE53B306AB1FAAD02501C58A87010000\
000000000017A914DDCF06DD5973802E70DBA6C1C9407BB70AF92BD087010000000000000017A91\
42E862EBB575AFC244C301A2E17FC883DD1596B1687010000000000000017A91434E95A4668CD1F\
4120A3FD27CC0365BEA9DB28DA87010000000000000017A91432AF9CBF69BF946851CA1C006B17B\
28FAF9B344F87010000000000000017A914CEA0E56AAC0D063F0525BA86C1171A539DFBAE318701\
0000000000000017A91403CA06E4905AB5D0B9A614C7D96F52744953F54A8701000000000000001\
7A91443D55B90B91F9FF0D8BD6790800CAC0081A73EBB87010000000000000017A914C422E0DC7B\
DCA851169FE333E20631D86DFEEB8187010000000000000017A91488F502B66FBB09A43FEF219A3\
9463707F0D9BB2987010000000000000017A9145902262F8498B61BCE587C9050EDFA8C501A2D93\
87010000000000000017A9144248DAC372C364E7590213B417D0EA29B206302A870100000000000\
00017A914AFB6D082967310E0539B88DFC0415A87D001472487010000000000000017A914E03932\
60E320023850EBB2A91E4A9F628D877F4887010000000000000017A914437C3413F5E0B9F5A4780\
8F0746A4CCC745E32DD87010000000000000017A9140CCAEF3E4B9990A6732147DD0D3E6F60B6AE\
D6BD87010000000000000017A914734313ED3188D7D1FE8FE7361DF1F4901071DEDD87010000000\
000000017A914DD966B34EACFFF4139191D4C1D1E78FE2CDED64987010000000000000017A9140C\
122DA3DE4CC7A2C6DBEAF89DA54DF436F29E3587010000000000000017A914ADB88D7AC90DE05ED\
29045A3DC5F27ECC33540ED87010000000000000017A914A4147F7FE889C34DE699E5435487836C\
ED890FB787010000000000000017A914FC9DEB6C93C43B217F6E53F09478326654E5A8EF8701000\
0000000000017A9143314A882FD64F23D2AEE0C73D958E86A3ABEA9E187010000000000000017A9\
14B659EFEA9BAA37DE8528F49D8AF0D9A8165F3CA987010000000000000017A914F08764B5FDF89\
C332E1E25599A93D0C98CFB99F287010000000000000017A914998BC99504BB37292A53A672C1D3\
033C08E4F6BA87010000000000000017A91408F35ECB4808209AD64CB68459A7915E3697DAA7870\
10000000000000017A9141FB8A5A18FBCC50F88DC34174CF338BD1F643B98870100000000000000\
17A914923D4A85D0E63F9585B5322420F875DA8C5993C587010000000000000017A91480EBFCFD6\
86C0F70E05F68FA9C9DEA8F226F99E587010000000000000017A9143B652B88912D39A454D507E2\
2A47D19608EE45DF87010000000000000017A914AFE768E171CC4704004DAC5C70FBE09DAE06986\
387010000000000000017A914F4AB02FDFA7BDAB132D7A475C3581CFB84BC3A7187010000000000\
000017A914854FAD304A434F645E7142C695524EFB7B1912A187010000000000000017A914B638D\
7DFE9F2E12CCCCB2F63577FEBD30225FE5787010000000000000017A914A9FDFCB004A51E361E05\
EAB13E783518D87E719C87010000000000000017A91440A767DFD8EA136C33A207DEC5607628CB6\
ED10987010000000000000017A91444401E906AC03363E69519E568E8B15BD5CC40B38701000000\
0000000017A914F8E46757E4EA283A120578B9418F7B9BD291B0E587010000000000000017A914A\
3BBC48D60960626AECE9DBF0B014062FF94467687010000000000000017A914A355A5939B0FCC6C\
B543EB66928DD020AF9A7BA087010000000000000017A9145CC5483D603596224840687671B197A\
823FB8FA887010000000000000017A9141EC6140A8F610C5712A4A067A3B882D60C55406B870100\
00000000000017A914BCB3D75241288B47662DD8C4C961E70297CEB40787010000000000000017A\
914B3D99DFA36E5624E8778CA05ACFB7E9BC9C524C987010000000000000017A914274ECC8C2D93\
76FB5BF074D30715EBB794E0D3B487010000000000000017A9145FD86C0ECDD3F5619DE3827CDCA\
DF9A174D71D5B87010000000000000017A91488E5973EDF98FE2FA43E52F5A3704F9ED086B0A987\
010000000000000017A914F4A1C104C786339E3E28292FFE362234C38A199087010000000000000\
017A914D5E45FFED0A6102EA70A8C4DA57469641B35305887010000000000000017A914C84339AB\
F7F71946A5BB678162F4DD5502542A0B87010000000000000017A9144C46B428AD8BAF8AFD1F9A8\
6E7909B4F0209BA3387010000000000000017A9149745D0484FC314FAB9DB52A99E26C5757482C9\
BD87010000000000000017A914F6C020CC7452FF18A07D4EF3996DA3CE19FC5C288701000000000\
0000017A914616325B2960DCB75E7BB59ECDB69E90B564B293A87010000000000000017A914D279\
83A495A731B5AE37A6E8B7566B68393EC03B87010000000000000017A914CC187E54D0DCBA30D1E\
6CC1244AB4EDC693421FD87010000000000000017A914146E33C0E0E3EA2CE6B2CCC8C423CC8ADF\
C5A10D87010000000000000017A914C4D1961D99175BD785CDF837F3ED90025633FFBF870100000\
00000000017A914913DF288343776168392ED4D51DA9B2DADB0C10887010000000000000017A914\
830AC726CF395F4AEBD65235976CB0DD00E6955987010000000000000017A914943B8CA006CECA1\
36D09DC056D119282EB8128F187010000000000000017A9144C02FBF1404FE956560A75EACBF6ED\
2E67255D9B87010000000000000017A914633DE8D0BAC7ECB7A9F72A83234DA6D6B3B78E1C87010\
000000000000017A9146A3FF24D8F190C66180170B6CAA61EF07A39608E87010000000000000017\
A9147E73B61449C334318AA5C7CB9038EAFD2072E98187010000000000000017A9142EAFF40929A\
A540D7117DF8A6C956EC37635F70C87010000000000000017A914B7CB4E86D47DECB4E1870CD8EE\
70C7CEAB1C582187010000000000000017A914DC15793E5C665BF03CCA9AAB810F132B2B7895918\
7010000000000000017A91489835E757DC7BC4C71632CA97285D79BC22C5BEF8701000000000000\
0017A9144E4F2E19C67E92F4C662584651DBF8051E2FDDED87010000000000000017A9142B7512C\
F358FEEE4429B525D3D43A4DADC9292D287010000000000000017A9141DFD46B38F7275020EA14B\
5C4F7F9B6EC77A503D87010000000000000017A914862A12AD2166A405EDBB080778060407B393A\
4C787010000000000000017A9149A14EDB83F293F4D848350A0C8EB85C4FE1C9732870100000000\
00000017A914006A1FD41E90E5FE40B9C72865A1C5A1622AB1FE87010000000000000017A914455\
F5EC44A764B0F252F1E8AE513CF11D28A7B1F87010000000000000017A914817A3EC7FC711EBF5F\
3A2B11FF7834FD1FCFE0B887010000000000000017A91435B669ADFC8C2C34FF6701AD64BDD8811\
D5527A187010000000000000017A9144D387638D4234B46FF94DF43FCE418F2E539B98687010000\
000000000017A914D7ED4DFA2B11042E6C0F20131BC6647E8748DCA887010000000000000017A91\
419642E738C2CCC7278A1A00DCD4A39B59971D2E587010000000000000017A91451A4CF4C38CAA5\
C97856E1D9DC845F26FB815C9A87010000000000000017A914E082E717AD6BCFC96912E8E198FCC\
9E55F7872D487010000000000000017A914FA4C640B2281E6E54D9670BB41B932666E4E56B28701\
0000000000000017A9149CCA812BB1B968990B3D801E2D524E9AB2770C618701000000000000001\
7A914BC8BC5A0E6BD747ECBE66DFFED735F86AAC5776B87010000000000000017A91464840D95F9\
C6242B9D9BC3F091013C0C4EB0C3C587010000000000000017A914000C025CD2C6A5CAB75439C65\
B75EB7193D8158287010000000000000017A9146499C0EB2178782AEEA3773D0B43C75C140C4922\
87010000000000000017A914EB29E54481AD119F53378786209A6F1DCC9E0A42870100000000000\
00017A914AC1900A8D46FEC4AF57C77F3F04083C72DAAF10787010000000000000017A914FE7434\
E34282D48B959F1F1DFCE774786DEDC6EF87010000000000000017A9142560856931F70812192E5\
8753378D050F7B8433187010000000000000017A914543AD76296898919A7E496497607E5514899\
288787010000000000000017A914A5B6343F2C88B751B4CAB8EA28C53D72E252448287010000000\
000000017A9144A4C4FFED2A795B4CAD510023385454488C7E83287010000000000000017A91477\
AD8F25246526FFE46CF3A7B123A58F4213C4B087010000000000000017A9147D02090F0268E1FD9\
E3D8DA80A6DCE4C46112CDC87010000000000000017A9141820B37015770DD406C79793FDDC87D9\
E944FEE787010000000000000017A914B459AD5BAAA6D6DEA732C2BD76B90FFDBE86C9C58701000\
0000000000017A914A8065BE35FFC7DE915326E0BEA80F6C690E91E5B87010000000000000017A9\
14E6F8F274CEB0707CAF127840EF723224D20A6EA187010000000000000017A914577743FAE2917\
2E6D44ABCA5680072BF0DA6A8CF87010000000000000017A91407EAB582DD15874B5C661CF3F4F4\
60E487217BD987010000000000000017A9144ACBD291EA63B066BAA0E8DDD890D36622DFCE35870\
10000000000000017A9140D65C9BEE30934ED8A31D7CA429C49B0BDB9F29D870100000000000000\
17A9141D44278167C0DA00CC275877D8BE420550079E6787010000000000000017A91427629A1AD\
9092A4F535747CA173E526A23BE29D587010000000000000017A91405C5FA1CCA88DA9E3D80CBA1\
91E1AEA7A1A2241A87010000000000000017A91456229D4BA4FF5A1CAE4B07C1D7F446D21A93E75\
B87010000000000000017A91459F86A2081B713D04B941E3E13ACAC286738CE9D87010000000000\
000017A91415C349F586E061817F9CCB59E7A3589ED4B4B16487010000000000000017A9149FD37\
45CB24B477E94BCD2CA01E3CCC7EBE3ED3E87010000000000000017A914E0B313D7A9B4B1B2955D\
BB2AC429344EC541A55287010000000000000017A914123F5F718D46D50D34754010EBAC558A5A5\
CCD3387010000000000000017A91454308D2F421B7D6C8A0C80F54532A87E9C2AA7858701000000\
0000000017A914EB8C47D4B4BD9DEEFD5835CA38B575E24EB9652887010000000000000017A9143\
38B4C82A17455AD689C8BEDBD8F8C8E390CF60387010000000000000017A9142E6F6A87CFE6E2D9\
564E286876285F7693F2321487010000000000000017A9141FAE22E0F8457658CF1B045682AA451\
EF7FF893487010000000000000017A914EDA5AE785874C29F8E6BB106AA406F193AF4B2F6870100\
00000000000017A91414E41D5ED7107B1104E5B2245F38C7D515A3B8E587010000000000000017A\
9147BB1460148A52E0DAA6CEE8B76937429CA81D2E887010000000000000017A91445D7925B6DE8\
F64C25DA5F17D74B1B74C7D02E6B87010000000000000017A914386E28B7F2EFC8F75F5432EA187\
7F4B20694EC0687010000000000000017A9143E82B62502D87F00FD639D70F52629C581B44E6687\
010000000000000017A91451C15BC2DAD27D02747FD5358464E3D1F141C1DA87010000000000000\
017A914FB4C7C1012CDA980FC23AC3AB72945EDC7DC8CD087010000000000000017A914675EA41C\
AFF6B17205E4BC4892D5C66769418D3987010000000000000017A9146792B7913E22940C966F121\
D4A7A7420C296D6A287010000000000000017A914BAB699ABFF5D7B2BCF49355DEBD3FC4A5A1C73\
2D87010000000000000017A914A902D750F2D1D054225A6ED14AA512A46831E6AE8701000000000\
0000017A914E1063BD55D5FBD27BC799D4D41C82CB5390AB9C087010000000000000017A914C108\
6680B1038F9D67BB1E29CD90867356EA55DC87010000000000000017A91414AA8BE05A5B1C1AA5F\
6F5A092B23B5D80C62A6387010000000000000017A914B6D4344A66E6712153AF1655B173731284\
A44BF287010000000000000017A9143CD5E3B2B29E97F5A6CC4121B5B19D010C5AD480870100000\
00000000017A91410D2BB180348D86E66060BF767F4C028F076066187010000000000000017A914\
0C80B33E19A2A15F9F2FC2A99F511F697C4A710787010000000000000017A914BEFC989E3AFEA6C\
E14E22934C32DC2D5F82F2BAB87010000000000000017A9145539F2CF1D9EFC2EF903332D68DF3D\
666000B36C87010000000000000017A914B8042DD9DBFB6F0C6C53E88BB318B3671D975EC287010\
000000000000017A9146E685A3BC40786018AE6A15C34A6F50F9FE1C5EB87010000000000000017\
A914348CF4E9AA6C7F836AFC0B01563921937A62AF3887010000000000000017A9141DDC96FEB96\
3BE9D76EF9F523FF89C9AA7DABDC087010000000000000017A914A94097FA7C0836651E192189F8\
16DD5F48E3FF3287010000000000000017A914E4DAE61DD3DFFA3BA6E8CF2F0EEF802A01FABAB38\
7010000000000000017A914416AF2073526FD3E9615B946AFAF14FF0FF55C648701000000000000\
0017A9146818D370F16DB7CB389AB85F3A316190BFDC902487010000000000000017A914DDDB419\
E9C1D33FE37A61C70B3B150FF85A9C28087010000000000000017A91446D35C17C85D7F79CCDC7F\
48EF09DD6AD218191E87010000000000000017A914D3B3B441BE78BBCCD8D2C015220BDDBCC37EC\
DDD87010000000000000017A914BBD7D78BB4688BC78509DBA43D532486ECCA8CC6870100000000\
00000017A914D300B8FD6112A628A6AE8448497CD8935925BCB587010000000000000017A914518\
2611E3A752EBA6CD1B93190576225CB4F683387010000000000000017A914D719A0C7F27109E74C\
DB0B063F83B963217FB03587010000000000000017A914AD6D92E59285B0FE1429209CC6AD0EE6F\
2390F9E87010000000000000017A9147ECD76119A4389090A629474DD259FC01508876D87010000\
000000000017A914627A20EF9CE6085030FF05DB7145148EC3EB60CB87010000000000000017A91\
462DFAB3A4B3F2212B87007117E9A05DFE9E29D0E87010000000000000017A914FE64416796EB06\
42DD9F0DC3677823E2D15DC91187010000000000000017A9141CC6089ACB9232DAEEBC2174B842F\
4317C6654D887010000000000000017A9146A91F5D5DD685CFA5036516FD3253D98A50FBC6A8701\
0000000000000017A914CC9CB91CF69EF264D574F8692C59FD927D22B3988701000000000000001\
7A91444B01E0E38FD7B20CF79421D60EBBE2FCA49E07687010000000000000017A91456504EF39A\
FFFD77E6C83B38709FEA00A37FE7A487010000000000000017A91496762E94437C518D9BD75C69B\
EFC992DF9A1638687010000000000000017A91402BE7DCBC72AC40E77C27A2260439A1A27F817E7\
87010000000000000017A9145AD8F2EC6CE1D9E8E1F94B0DBEDA2E666DDC2626870100000000000\
00017A914C7ECECF3977802FEDA39F3CCDCF736F943EDFE6187010000000000000017A91475CAFD\
EA78A50C6082105D140247068252C2C6DC87010000000000000017A914CFA38831C2FDF29EB120B\
1FB9A5342CB691184C087010000000000000017A914062C6F8183527F73286A98D47997E3D241D5\
073487010000000000000017A9148A1C580A5316AD14D66787B51E1C2B58733A574087010000000\
000000017A914DA72936AFD65E0757CF94CB619E9AE0FF61841F987010000000000000017A9144E\
153BF38BF5E9187348323837225A21593FAA9087010000000000000017A91410D81D90196AB52A8\
D410CE0C38C9BD86D1E462B87010000000000000017A9143724AF055146C7D4C2F037CAB3D0B3C9\
B0548ED287010000000000000017A9143F6F031813CA835BEC4B6632B1FC97268F6AD72D8701000\
0000000000017A9144F84B2B7ACF383D9D2B7BB0D013B50528BF98B1587010000000000000017A9\
145D60C5BCE0F2CFCC559C22782F7121C887FD2F5D87010000000000000017A9149F34F2CB84059\
83100A24A48CC0D2CC4378B457887010000000000000017A9148BC2C2120824C7F0B25F5359381E\
D1D4E88657B787010000000000000017A914F37F4CA083211585106BFA272F69C843F3D3D0C2870\
10000000000000017A9140D627FD75C14486EB978F081ED2DE076F049A7EC870100000000000000\
17A914D194347F6E1A6BAF97E9124EC10CB93D5AFE4AB387010000000000000017A914C31EDBC14\
40F4532A11AC3C879FB526607EC25C887010000000000000017A914B267DD886B0FD78126D42215\
627DA31D86DFB4A787010000000000000017A91413AF9F5DBC3849D4B421913E0EE26B982CD3058\
087010000000000000017A914A483B76C20500D7457A41133FF74DE298F8433E987010000000000\
000017A9141D43BD605F729422D5FBC1DEC68179682A441B5E87010000000000000017A9144D647\
05131B079BEE91F25A7DFCA64CAA13D394B87010000000000000017A914B54CC757411B2610C45F\
7462E2FE47A87EBBFE0C87010000000000000017A914181D6349BEC24BF93E6646BCC9187D692CE\
1886687010000000000000017A914685A05D7DFA7B6BBA6894DFDD628B0A535D9EAA18701000000\
0000000017A914714DA2DD32FFC3983E57B1B2F332C6992DC8643187010000000000000017A9146\
5CFF7E97101CCDA0E2235531D05161F36D237F787010000000000000017A9149191A63DD90BA0AB\
1C29DFBF9577E28F146A590087010000000000000017A914947E6C4A557C86D94179F02B2DA2FC3\
FBE05688087010000000000000017A914AFCE26C9D48E311125DB27F901A04A4579C746D9870100\
00000000000017A914DCFB582157544147312DFD77B5FA84A57C5835DF87010000000000000017A\
9142F7115A3A2EEFC8FCF9C7FA5AE3CA9546718802287010000000000000017A91490574D3121BF\
FCE548614994AFDAC46EE4B4C14387010000000000000017A9144C3B832A34709F4C1A56BF6C569\
72A73C9C88B1987010000000000000017A91446D7A9612C5AA03EFE1FDDBE1A4D621582F88D7387\
010000000000000017A914F85EFA13142B80C66BE2C6F3ACB88CE42978DBE987010000000000000\
017A9140EB76AC31743CA55246602883994903C6F0284F487010000000000000017A914C3175D26\
A293A14FE8A392527DEDFB094957E66287010000000000000017A914497AEC47FD517C46AC37BD2\
A965F0BDCB33F0E0E87010000000000000017A914B08059D4F89683F2032EB0AFF108B0B415F6F0\
5187010000000000000017A914F22F17681189915E032E47BCA2DB416AF20217FB8701000000000\
0000017A91480A441B593098B76C8829BE4410279BD323E815787010000000000000017A914152F\
65A3C26F46E3D4CCD9426B141CA96833607087010000000000000017A914D593719D452AF81141F\
02127F8581D8D9B7CB6D887010000000000000017A9144342192F2786F85A3E42C4BC3B0BB4347C\
4884CF87010000000000000017A914CD4BAF178EF2C994A782BC1EBFB0CB2253B9BDB8870100000\
00000000017A914CBFB543D13D31A48F0FEB1626E16B9053F916A4B87010000000000000017A914\
9D94E809E2560C84A447285ABF106B61D2E96B5F87010000000000000017A914CED2F35A8A6616F\
D27C0BF9BF1E001CD718A693387010000000000000017A914ACF2EA24BA7F3F4745D5E0DC2C48A0\
F046811FE787010000000000000017A914762CA88FC1C44859A15C6A8D7A8A093A6CD6114687010\
000000000000017A9146CAD788FB5B876919FF0F1171EB6AB0032E7CAB487010000000000000017\
A91429CB68F7F69EE06F7F6A545B5241483D180A51D287010000000000000017A914B3374B539E9\
12D610D9679CBB13DCA0BB88F8EB187010000000000000017A914040650E44EF9A91FA8DA16B542\
7FA5D3D06C69EA87010000000000000017A914E7FCA6980A0B555AE47DCB145BA1877703D1E4E58\
7010000000000000017A9149E47F84BE6E1832C3B1C1F416A950ED4403BF8C68701000000000000\
0017A914DA6C91DAD121A7459D6E74953EE683EB58C9AD1987010000000000000017A9149D06457\
D6D296E2456169C5A39F1F3C3B704D89087010000000000000017A914B041695125883E73F5055E\
0F1158BDFFB6CBF95C87010000000000000017A9146E9E106D6417B35E559BA3418360891A9DABF\
90587010000000000000017A914ADEA7B49B292DA0D5142F994F320438540197E45870100000000\
00000017A914633A81394A22BEBC86F30499A002CB6DDC371EB887010000000000000017A9143FD\
B44594EC1025D48382ABF43FD86905B8E420887010000000000000017A91439FD04E24B596CE429\
D156915C78D8A3E235235F87010000000000000017A9140D5CF75907FB479CD408A47EC696A000F\
16F5F4187010000000000000017A914EF0E8F9C3179B8BF528110B607F6092B21295F3087010000\
000000000017A914DF7A21521CA97C2B165139E3F3C2B5ED6C89AC4D87010000000000000017A91\
4BC7EE606AE08C8D8CD193BE2BF2DB0FE2500828D87010000000000000017A9147769A06BDFFC18\
0874C88FBE679D3258CFAA608787010000000000000017A914963822F6908AB591FA018C4EB6AA0\
CB8C409E68787010000000000000017A91463C20F8FFFD742BF8734E7FF8D79CC24EC009CA18701\
0000000000000017A914A148BE92BB3C45FFB1FA98BB7AD4E95C38DFE4368701000000000000001\
7A9142DF743DBB9A4FB542157F8A33302CCF1A2F2DD8087010000000000000017A91498BD847EFA\
E63AF64C94A142BE05313DC7156F7887010000000000000017A914AD065543AD08E799DA4026ADE\
969163EB1E2E36587010000000000000017A9141B61FB010D8BF3EB22A37A34E8B675E587B8B68B\
87010000000000000017A914837C40BC18B7A2DAACF56753B3E0E53BE55907D6870100000000000\
00017A914783D21140B0ABC39A4487CA391C41D1A8F17034B87010000000000000017A91491C097\
A1272395409370D387BD0D6A691F293AE987010000000000000017A914DE4AAD70DBD32D1ADDDA9\
05B50E992D41DC6525287010000000000000017A9148729419154CB271C17ACE5CB36DD22161C20\
FAD487010000000000000017A914DA9BA780049CCECB3CB1755A7623D92A886FA11D87010000000\
000000017A91427DCC3A5206562F961BE5AD5AD18E38B589098B987010000000000000017A914E9\
9987664421DD9452556376CDEA8036BB5952F587010000000000000017A914EB5171C3A804F4CE9\
DBEB493CC64DC1C37B518D887010000000000000017A914AC0965CBF934C0C82DC4814ED1E68B0C\
0747983D87010000000000000017A914B1B3C9787DD5A170A1C91CF07B9D57FDB54753268701000\
0000000000017A914B79EEE7370FA9D3010FC93AF706A586CC0641FFB87010000000000000017A9\
143E347E70D57117D4FA22B42CD64F6EA4C372B6CC87010000000000000017A9145487C40C8F22E\
BBEFB25999CFCC3FA8D078EE70E87010000000000000017A9147A06B943B5EB4D001DCEA5277200\
EA57141D776287010000000000000017A9142D89AD3DD291E1CFDF6F1E3B68570F73833CBEC4870\
10000000000000017A914E13B1533782B57841602C585281FC54FE174D37B870100000000000000\
17A9140E2A661880A3BFF724740662FB9EFB00457D552E87010000000000000017A914BA515FBF6\
971F3ED0B3D245062564ADF5796EB5C87010000000000000017A914729A57F43A0E168DF2B9EBDA\
8124D9123B9C02D787010000000000000017A914FC3617C9D966B72AA1A175213B7BAB3255535BA\
087010000000000000017A914636E557EBBD509C20A04AB1601F2506F44DBD6C687010000000000\
000017A914166517BB3B7F8719FE2D155A6577EEA10D2B5B9887010000000000000017A91469485\
152CF207274EB2602683341B3DABA7786C187010000000000000017A9148434B7730B22CE75CC36\
F417399FA522B67B6C4387010000000000000017A914CA98D619FF343BF2D0548656CC4CDEFBBCD\
08BBF87010000000000000017A9142D8C415CCA8647E897D7B1291436AF1D4CCD35798701000000\
0000000017A914B07A8019304032B13A8042587C12582A8E62928887010000000000000017A9144\
EED0A7FE29A1B76F5937A3D9D2A0C085E5A10AA87010000000000000017A914466146ABD480B644\
84EECE8FAE9C1BDB5163E17087010000000000000017A9149B7F802B4E07D421533B09B0E85884E\
A3144A26D87010000000000000017A9146D58CED969A7AC735DAB62259E777869C632A963870100\
00000000000017A914345DFF697E6C677211A2EB78E73F72C1568FEA0887010000000000000017A\
914444E9C42AA1E9C0FF8D0F0F7F9A624BB5D7ADF4887010000000000000017A914246E1B77E475\
478B44057493DE5103ED8627981F87010000000000000017A914F928AA84513C8A33CCEEC56428A\
7C75D9533DD4987010000000000000017A914928D3BD63C32828FD5DEBC954C091F7EC054A27F87\
010000000000000017A914664DE41F408BFC4269C373E2F178FA2019116C3487010000000000000\
017A914CA4E435799E20C611A3A6F03F13106EE49AE432D87010000000000000017A91418E14AB5\
42CD49CCDAA39A0E92CE9E3F542980C287010000000000000017A914BF5C4EE48D3F839C0B1ECFB\
36BAA132FF5BBA38A87010000000000000017A9146C48D6FFD6B4BC58B60BA0E1746926341FA186\
4B87010000000000000017A914E9CCB498B3AE71F0E65608848449B996E66E5F958701000000000\
0000017A9148B50077693D276AC48106ABA2EC530D0D3D9CA7487010000000000000017A914E7C0\
C3A924B0C261EB1C8E98222B28E49B27675887010000000000000017A914E3C494FC993E08E7167\
B1AE6CEB4B019F2A80E3E87010000000000000017A914E390A4AE34275AADBE65DBC78EED1FCCD9\
2E2EB387010000000000000017A9144B7B0D39A603795375B4923C47439F39F39A7F42870100000\
00000000017A914F446BBFBA815B5CF6067A52FE3F3BA3583ADE7DD87010000000000000017A914\
C86299BE1E76DED86B604F7FADE70BB2B139944687010000000000000017A914FC7C00AC5ECD3A6\
52DE06879FCF654248B0F926087010000000000000017A9142DDE2BEA7AF6D3714C5BD1B4E59856\
D1EEED159087010000000000000017A914064BEF6AF220A20688EB7C552928F22AFFC67F5587010\
000000000000017A914B047DBA2A990502C8B28B80F052F2A43F3AC33C887010000000000000017\
A9140C76F792BEAAF2513877216E2DA7B208D1D1344D87010000000000000017A914735B6135E4A\
EFB696906655D712DCC738AB46F9587010000000000000017A91474A652D79CAE24B8E4B387C959\
7CD573E1BA3AA787010000000000000017A9144383AB830A2F2D9F2F61D9B8A794DF80A1130B268\
7010000000000000017A91403C5DBB933FF6FAC66B453B6C8537B6F37FD3B178701000000000000\
0017A91446F6E55EE966A134FE3F3414B685BFEE0EA2735987010000000000000017A9140A38D5F\
3141448BD9EBE95DF049F1CFD59FECB5887010000000000000017A9143D56CA3D1AE77E8E2E43B1\
58DF651A61A4E9BE3D87010000000000000017A914DAA2B9E30DA92F43EF6476A5C156E1845D9D7\
EC487010000000000000017A914E98AF2879342533EF70E619EDF55E1F49A99F13C870100000000\
00000017A914A2EE8E83D0AE526BA8D46025ABF3D2DF8B5AD64987010000000000000017A914906\
E116DBB9B23EB2F7E64DDF5174CBD7B240AD987010000000000000017A914DCD77B7D5E9F2AF0C4\
03628B0E0A1816E384DA8187010000000000000017A914BB3519F9C6B74A2E3363E62AAF41D926A\
F9E044987010000000000000017A9141C460AEBA3EBD0F576B570EC8B115AE94B80767C87010000\
000000000017A914A0E6676F9E9819801AF75C2BC868850D59DEA2D787010000000000000017A91\
4CACFB9CF91B10BBF6943C63F6929585AC3FE96A287010000000000000017A914AD37E4678B937A\
190CC552204D72DE36BB41A51687010000000000000017A914D4E64123D34ADF790E66014B0B975\
14C680EBB0687010000000000000017A914DC21E83DC73851358E8A47C59FA3760FD566AD198701\
0000000000000017A91409464C67E9AC8787C86A0617C30F91406C4C23048701000000000000001\
7A914D5B9414AA22351B8D72DD78AC2B9E45D7EDE1B9887010000000000000017A9147F7DEEB86D\
E2A2888539F6927FF414084DC7EBC087010000000000000017A914A4FAC8EC8AFB126C5EAA32519\
ADC25433DBD27FF87010000000000000017A9147FEAA0B9A84BD7492671A8B52DC40E5476BA2503\
87010000000000000017A9146453D67BE428F756E7D9841BCCE053BC0A1E8D58870100000000000\
00017A914B31C7060D0E90F38F1131FAE1B834B35BB0F283987010000000000000017A914C27689\
8BBC639418158711F79588FD9AA98BE60287010000000000000017A914D558353FEF3F9AFECD24C\
6D92EA0EB586ACB87EA87010000000000000017A9144366BBE5771375D1042633E0296179E59D51\
7ECD87010000000000000017A91469B7DD388BA8DB3CD2925D9271E2E39BB302FE9587010000000\
000000017A914C3C70A0A59A5C3622F5D4625854173725DBF764E87010000000000000017A914A1\
E46DAE89BEC8A41AF076895A83103B7E1B24B087010000000000000017A914781AAFA66B48884C2\
AE3EEC3A74788A338A3A9E687010000000000000017A914436765A659CC3AF68EFF0B1343FD312A\
DA85A8D687010000000000000017A9149E769C69AA6FEF95A081CDA28BD37A5384314DD58701000\
0000000000017A91465BB4A6A6544DE3DAF22356F462C2C57867D31EE87010000000000000017A9\
146BF3F3BBB6BCE6A53596C599076702F40000527587010000000000000017A914EAF173D355819\
3527236D4C46C856D54CC8FB0AC87010000000000000017A9146513DEF9A0D799E3E09C5D01112E\
0DDFC34FE15D87010000000000000017A914F88A8C542D2C4694CB927A41FEE094CA5FED0F53870\
10000000000000017A914ADB193965DA656037E56DAF97B36964F2A9D9566870100000000000000\
17A914E9A48D7F403148365C13C7BC3701F411C3010A6487010000000000000017A914CE9023B27\
FCD5DA172753D4071DFC835D43235A987010000000000000017A9142D01939FC2ECEF272798FE0E\
21182D78E37B07CD87010000000000000017A914AB6729BE0AFA5DBE4ADA9D3D9FA0DD6086E55D2\
787010000000000000017A9149B464CB6F73124C4B16DFE31BE2670DA623ECAA487010000000000\
000017A914F67DE0A4514B5C550B6B0A2DDEBAF91F6A04B33087010000000000000017A914E3098\
043DC5320389EF0B244BBA6E7CD5845764487010000000000000017A914DC316E1E10C37CCC8274\
F037C08F6964804A44E587010000000000000017A91430AC4BCC660266A93B763F0560FEBD8A340\
7142B87010000000000000017A9148F5D4809F7039830AFEA4B99645A2A5410297A198701000000\
0000000017A914A81404FBB331C634B49098551ADF713430A2DAD487010000000000000017A9145\
1EDCF9B5D6D3493EDFA439FE30D0938552C8E5287010000000000000017A91444D906B6786E12D1\
6A656817DD6DAA5F229D925687010000000000000017A9141297465AE313CCBD5DD407212E3A9A6\
8F5AE7D5F87010000000000000017A914156013BE429BBFC4AE84551BB7F997D0AA772B07870100\
00000000000017A914E62925B60346E5672E48DF2779C6CCD81CD367D687010000000000000017A\
91440557CD7CE88B9EDA16069E6306CD78149EF513387010000000000000017A9146234A27F0BE4\
D24DA9DAF0CDC5FB0AD39D76CEF187010000000000000017A9148976C12F36BB70820412AF2F638\
16ADC76BA28DF87010000000000000017A914742325087589F3355FF598CD5ED4CDCAC803528287\
010000000000000017A9147C7E9E8A5B230A680B7339D0DB99FF0F7CB3F72287010000000000000\
017A91442B659CB5A009F34FF59F48E06A457F60F142FAF87010000000000000017A914C4D769C1\
8811DBC662D924BCC9559CCD1267BC7687010000000000000017A9144466D281B000E494A7F4B7E\
8EAD05ABFCF0C13FB87010000000000000017A914E0747ECADE9E74AD7B7EE516CD31AC85A973DE\
C687010000000000000017A9145181C4B8F993A67E4CB532A1BAAF77F623F4D8A28701000000000\
0000017A914D04FE7F5902961B9203FAA25771E006C671589CA87010000000000000017A9144B39\
4AB534443DD00708C462E81F5D7AEA91343D87010000000000000017A9149E08A403686A6937AF5\
E5A4A808B3642F94C053687010000000000000017A9142F3D68F4E7FCC9C3D4D0F5C21A7FF1D6DF\
F6C0F387010000000000000017A9148E6DBAA1930B9CAAE20710547DF32654FF6B7266870100000\
00000000017A9147186A76A4B3E14EA8A401B76FD8FA2EEC9A14EB587010000000000000017A914\
A94A19B13D005FF5C3B157A22279F129C2EC3FD287010000000000000017A9140C4076C8D091180\
8B6A760ACA26EC24A33D21FC687010000000000000017A914446B3A6C7E6C895DCC6451280B6125\
2A54AE0FD887010000000000000017A914C951E74A277D1FE5427663FE982C7B5C0BF06C4B87010\
000000000000017A914F9EA0055CFA0EBA5FFA263D35E16DAD24233F0E087010000000000000017\
A91442DCA20209D5FE12EBBF13995DE24FF8AAC1B65387010000000000000017A914E3235657009\
1881EF1730CB6A7B271CCB43F70D787010000000000000017A914229B73E2E98B8DA1108C8864AB\
46699AE370787287010000000000000017A914AE8659E67E2DB3F607176455E5B613D92194F6758\
7010000000000000017A9147E9174567AAE5551B5C5CB0559FD4EDBAB187E778701000000000000\
0017A9144D949D08C34EF766DF40DB46B1B8DDC5288C275B87010000000000000017A91496169B5\
CC7250E74959516911C36E89EB649052C87010000000000000017A91476A3873D6893E258EC76F5\
65E92C81BCCCDC190487010000000000000017A91478E8D29137AF2B5D866216EDFA08A8B7EA898\
91E87010000000000000017A914E232A3A53B7C1F045D14F588C8E12622A56BFB11870100000000\
00000017A914E0E6FC9A27BC0C334BBC2029DCCDE63A44EE425A87010000000000000017A914BD3\
0D2A05B50E7D85408FCADB74072A8837CBE3587010000000000000017A914998C9F623C69E96C8E\
1A2472EBA576E856905A8C87010000000000000017A91498547E1F2DB1DE2A71FF7D086BA2B393F\
0D8B94187010000000000000017A914959A75B717090705C83B881E2D9F024B62CC7A7087010000\
000000000017A914257112FB145EC89265B73FE164D68473DF61EA1087010000000000000017A91\
4AA332914461A28295C4E9B9B21DA1A252466C57587010000000000000017A914C1353CB9ED307B\
6D0E98310134D483D585BE27CC87010000000000000017A914E2BCFD3E888911BD58661AE2F6A04\
82CA0BE6C0B87010000000000000017A914B48B0E36BC295F552BFE91F79A00B0EEA153ACD98701\
0000000000000017A91459A2433A988D545B485E0504D54416D7EB3BC9E98701000000000000001\
7A914F878F62D822D87238813307A2B86227B95E23E0087010000000000000017A914F82F5102C6\
F746A2D061129F02B105181E54451A87010000000000000017A914B7827C1C7154210FBD576F80E\
F5FB2BFDAD4480787010000000000000017A9144AA530E3076DAA81AB8E886DF9F840089566F88F\
87010000000000000017A9147D0BBEDAFFD8F5573ACF3E63441D55C47FA84646870100000000000\
00017A914B33C8D52B797F04CE66DFC1DFF936A27CD57F03287010000000000000017A9142F0045\
6DB65695E0223FAE91C0454E4896DD9EB887010000000000000017A9144AA331CF49B6245A8B74C\
DB2FE54D87E728918BC87010000000000000017A91441B819D241FCE3B5E26A6F9524F80C50CF1D\
69B287010000000000000017A914D9B8C9AB77361FB26AED81DF23449969517BF87B87010000000\
000000017A91407A7E3541BBF979B8840FDEC86BF72DB0886B3D587010000000000000017A914A1\
48D68CD435622125A856AA38C3169516C06F7587010000000000000017A91450C4A6338AEDC270C\
DD9517E42FB30C2BF634BF287010000000000000017A914EF8D2B85FCE38B04CEF38B3356552C20\
9E028EDE87010000000000000017A914879BB7CE20818F2BD7005E7BC75B5045BE2E08928701000\
0000000000017A91439B875FA228AFCCB079A5636DDAE13D3084BFC3687010000000000000017A9\
14FC98CC77811D41CC677B34AAA8FB0829FFAEE8B487010000000000000017A91436600DCC00068\
24D6E637A85473ECC3BCCC4F25D87010000000000000017A914E80162156BE6C0951F618F7D03CD\
6F4880792CC587010000000000000017A914FDD0F286B41814D4ED430C3ED8D3F9D82B3A07BE870\
10000000000000017A9144254ADF53EA46FB17753F0ED33F2A7E1D7B36C8D870100000000000000\
17A9142ECC9BD5EB36CD8BCF57ED7CE3083B5FAD89D6CC87010000000000000017A91477B8076EB\
DDD21B72127C23A6C44132E3B72D00087010000000000000017A914F433E71A39365F8D12E09420\
0E28DB9245B1FECE87010000000000000017A914CC230C47C0665FC8D407440D4DDF743E186D577\
887010000000000000017A914D6E630F0E4749F5D1CA3947B0F79FB087A92B7FC87010000000000\
000017A914E340270703A3658ABECA639B6CF0F19F6CFCA01C87010000000000000017A914A488B\
B803A41E0CB6578D656ED35C1F0BDF431E387010000000000000017A914CAFBF857F673126918AD\
9160CB13D4DC7C8AD65387010000000000000017A9141162E48D25D94BE17B73FDF10A86145B5F4\
AEC2487010000000000000017A914B85B505DC2E7A8F9802BDD27748A711553D892008701000000\
0000000017A9143A6247D1E10174439BC98294229E5D71E4DF351087010000000000000017A9146\
55BD62D15F62C0B03D417AA5B0ABC697C63CBF287010000000000000017A914F03E1A72B019D910\
1E45620031071E0D799A087287010000000000000017A914A46B792C5D9B2DE0E07B5612E620813\
E0D9881AF87010000000000000017A914179A8C723DFC869C1DBA2DB09F0FB91839407EF7870100\
00000000000017A9146A89F504FAC254AA90B9BD77F381F75692E7A70B87010000000000000017A\
9147743D052A23F59016539840F120823D567485FAE87010000000000000017A914C7F45293974C\
DA971F0DE30FAC5245C9338FFD1687010000000000000017A914D7E3B6856BD31B79D4DA790EEF4\
A80A708F5067F87010000000000000017A914029DBA28181606CC051976335F75D0A5E276CB0587\
010000000000000017A914F640CD06AFF190C4DE3390B4DB2544E2D84037E987010000000000000\
017A914A1B4F658226B26EEE8A28FB4B252DED201E1E4BA87010000000000000017A914B466CF93\
C75ABE74E39F36C0FFF574651DBEE09C87010000000000000017A914CD854BB3C1D0AFDB876DAEC\
461170738898703B487010000000000000017A9145BF8C48BD3B3BA0FAD4749939B8529A41885DB\
3C87010000000000000017A9144EEF0942473E71518700D43B6A4FD86D01AD50348701000000000\
0000017A914402F8096244A7B0F65F0287F6E6D3E9183DB69E487010000000000000017A9142FF3\
7D0FDBFC28EA3CCDB00F4FC686BF8F94FDAF87010000000000000017A9146DB665E58BFEB4BEEAB\
55B3E95C7AB60325151C587010000000000000017A914CB3A97B8B260ACD5EBC587B00C33F06D42\
F6589E87010000000000000017A914CCF1577DE1621D5760BA8D0C64C403AD31977462870100000\
00000000017A91479A3A8BC49161A8434A50659EDE5174BE06A3ACB87010000000000000017A914\
EEC0BA653BBEDA4D0137A017B4B9E0DE37D3351987010000000000000017A914B5D817001B84B5B\
D89F812B114922BA401CB026C87010000000000000017A914F3B0C76340EA90334BE2B01E3F4CB0\
C222C5CDB987010000000000000017A9142058273EE91C90AC5CBB7CE19450C4EC98F305E487010\
000000000000017A914DB70306A3FEBD364744FE5C4082514FAB4F5D65387010000000000000017\
A914E5E0DAADDB859DBB5CA142B1A71E7F75008FA67887010000000000000017A914B0DC712F464\
F5E0FC25D7D9B7F27FFAB899F9B8287010000000000000017A914E09C491CDC8177A365E22E9EBA\
E15F17A1829A1087010000000000000017A914F9CD790A89F2DE84ADE134E433C904CA89B0F3038\
7010000000000000017A914CFCAC5767B7BAD164295FCAED139BB630F55EB328701000000000000\
0017A9147365B414EB79E7FF095A9AABC354F7FD8C4770BA87010000000000000017A9145C32EC4\
69E0852E57637166E5B81168082366C6D87010000000000000017A9140E3792FEB2042FB0B78F10\
44EBE5879CAC9AA07E87010000000000000017A91447AF21B935A978247E65F79ABD71C6B9CD6BE\
07887010000000000000017A9145792A9118122AFAAB038A96A41AC56B4D127B7A6870100000000\
00000017A914F120DE00253A9C52AB529DA1BBCAD297759C707987010000000000000017A9143E3\
FCFCD074B45514B372EDC99EDE3D5637F5CC687010000000000000017A914139DD40FE7DD7EFE4F\
13888263C9C849F9CCC2F687010000000000000017A914D31109D88C8E4E35AF1BBF57B6EEDE5EA\
B20D8C087010000000000000017A91411A5AF615E0E1BE556A4B03B8E5CD80FAF8C6AE987010000\
000000000017A91453F38D034CE6D894146AF54C97176A83340C9DBD87010000000000000017A91\
47489A326BC956919CBC6CF1190C114C61426473C87010000000000000017A914B5E0D741B0A279\
ABCE46C082CF969B926FF0923487010000000000000017A914BEBE6C7DD46CBAB9BA00A76B2EAE6\
3301A029DEF87010000000000000017A9145B762B013FC779A1B961B67F1E19355305DE83D18701\
0000000000000017A914986E45012A5BF32B5EB2E481B691F8FAA9BB08AB8701000000000000001\
7A91494ED51B70E460C346F9F87CBF5DBC75A732BB17287010000000000000017A914CC79157A8C\
684C7B53AF3810326A7DFF974BEBFA87010000000000000017A914A1C8A35FE806172886218512C\
C973A44FFEF2D7E87010000000000000017A914F740C5351F49BEB422E61DF0B72B6D5FDECC75C4\
87010000000000000017A914BDDFD76FE81AF1E7CA0343040DD1DBF513995E40870100000000000\
00017A914FCC618B26E94813B58C8C88EB88CD6AAF8E9228587010000000000000017A9145EA6FC\
DD08369AC21708BB1D5C8D8FEDF6FAF0A487010000000000000017A914E70B9A4A1108046C88E6D\
D616371BBD78B1BD9B587010000000000000017A914D029F95B5F66EBE00D24869830B8ACA07E82\
B10487010000000000000017A91497D9077CF3E3DD2FC1D80B53E23471F05F764CFC87010000000\
000000017A914F8DAB016F8B8CBAEF40412766D5DE5FC2B65702287010000000000000017A91436\
DAEBB476B2CB6FD1A78832B2E807FBDE84326D87010000000000000017A9149CA85A77AAE6356D5\
84367F68032947996AB274F87010000000000000017A914AA777A5D11C80FEBC3866CF624AF7CD4\
FC7B98B887010000000000000017A914218A760E833A2E7253247A80A98773E755828A568701000\
0000000000017A9147C0001BDAAAC55A3C8E7D2532C1D03AA624C01B987010000000000000017A9\
141C09EFE4E4FDBD456EC4AB6AD07FED62A343855E87010000000000000017A914D6538B097499E\
DA80CF7A04245181D4A3CC74F8687010000000000000017A9144313CBE77DA0DEAFB28264CADD8F\
EAAC96FC85BD87010000000000000017A914CC5B2CF6614D49BBBF7767D03185A6D832CEB5D9870\
10000000000000017A91412928651FB3020BA311E8F7538DCF1DDCA76E686870100000000000000\
17A914E7BC1F020AB3F03C460F0B086D35E6FAE068CC0E87010000000000000017A914282504718\
8F2255DC8D3E2A8AE32F8096291CD0C87010000000000000017A914FC1C7773F89ADC895FEA79E5\
4E2743E533D00B5B87010000000000000017A914B9F1876D5EB5158848A9FA2137B50690D50D3A6\
B87010000000000000017A9149490504FC942C2B3B5BBF2521226B080A67754E287010000000000\
000017A9145200209167B6068B3D06DAD1FF8B3EAF8526F93B87010000000000000017A914D04CD\
2F7C4A34262F9F813FCFA7883B6DED58F0B87010000000000000017A9142ECAF6A37B1A745B9596\
1C5B2F4EB7817C4C3DB987010000000000000017A914E87EB6DEB35BF8014ECA9EDD692176149C2\
EF90187010000000000000017A914DA08630605B74870992258815286361FE0FCB6688701000000\
0000000017A91497ECC51DF5F02CF3D1C6FC1B60C4DB9F8E15096987010000000000000017A914E\
50AA0B5DDAE6FE9BD42E7BF92F54DA0BE881BB987010000000000000017A9141B95C0491E184C1C\
E85D02E3A87C0137CA8DE91287010000000000000017A9143FF4199FD9D9CAA47D7D96B80C16F69\
B730053CA87010000000000000017A914193C0B655BFEA9930715A1A5BDDB0BB893B0D8AA870100\
00000000000017A91450B1FF22FA9449967FED0DB1DE87FDFBD541D72487010000000000000017A\
914670C1529147501DBA2204FD836DA3F34FD8E0B7D87010000000000000017A914F73F0DDB6E5E\
60ACE0AF10F97856AA1A4E95C54787010000000000000017A914E541330E7EDEFFDC00AE7AB1294\
132995801E81587010000000000000017A91480FEC2774930F53EA33B8E66E97E8F84FFD0085587\
010000000000000017A9141F8947AAF10E8B8624593C222815DCF5AAEC3ADF87010000000000000\
017A914D85143458D87235920F399359A0C0417C224352A87010000000000000017A9144927D02B\
445F64E6FDF38D9A810295609DC4AC5187010000000000000017A9145B871913838261BEA672341\
FC99CA85085BF4AC587010000000000000017A914748FCEED7574502AEBF2C62690CBB923B64A7F\
4F87010000000000000017A91489316589502801B23D863D8BBEEC916D479751798701000000000\
0000017A91435070B6B83D7D1B1101ACBC37BCB09DEDDBD537E87010000000000000017A914634C\
C373C5B23E03CF14DF43B5851AD292262DA187010000000000000017A9140F259A3B12035C50CAC\
025E2D95E0A1ED9EE772987010000000000000017A914A71340C4997CE3B108B3D7418790DB09B9\
54AC2D87010000000000000017A914C8776C68444D211E85F5252CAEAF89087F0D3896870100000\
00000000017A9144E6F38FFB522BF41F80007A0092205BE2B51826087010000000000000017A914\
5EE46351B6E5674979CF431718516A5F37ED278887010000000000000017A91492C3F98E62C14BB\
6E19EAB7C3C9318CD8239C4B187010000000000000017A914DAC19B9747A528EB52937F82B5C16C\
B44CFEC0F287010000000000000017A9143FA897ECA27686CEC993001E321C1C07992DB80D87010\
000000000000017A9147030F14F26E3ADCCDFD7F4773F61231ED3B2B6FD87010000000000000017\
A91487D9BB24D2096CFA8C53084BB1BB0C37A0CEE07487010000000000000017A914AB95839BC17\
BE2E68EEB459BFE228F880F69D59287010000000000000017A914311D9E8F22392F73A82779D737\
740018D5E87F7587010000000000000017A9144FF538BD9B5D5A6BD9EBA04B737A611BB449A1398\
7010000000000000017A914E8096F86804803FD86C873A84E62906D76CC421A8701000000000000\
0017A9142FCB99F9040E61AB61D00C3EF6ADC70BBB2A85F487010000000000000017A9140838108\
F45C1C567F63D6E860FF05989D0AB891887010000000000000017A9148992FE8860AB7DF5A9331D\
EA12443585402163BB87010000000000000017A914B18C6869DFB7225313FCE3DED4F16D3DE48B4\
DF887010000000000000017A914BA61C69B1C7445C0D10157EC2814AF5BA41CC989870100000000\
00000017A9147E023654FA96B8D52C28351AE181799D91DBF68887010000000000000017A914B56\
6E0D96CF00A52078EEFA748EF0247544667C387010000000000000017A9142FD6E8B2F99C56BA94\
9E304C1B8F2540D234821187010000000000000017A9142EC446C664708DC8B7A8E7BE62B52F57D\
314672A87010000000000000017A914A83A6AD6BDAC0EC7955C4C43B60D24B24109F9FF87010000\
000000000017A914DE1DB69B25382829555CCC6AAA8AA63BD12CDDE187010000000000000017A91\
458F6CE7A217FC87FF6E142CF87CF90AA4FA2F10B87010000000000000017A914917C4B277895BB\
C78145DE7839DD91C6BE405F5887010000000000000017A914D7CFD1BF7448588830FE8A8158752\
A753178363A87010000000000000017A914A2CF47E9E404DBA5620927649D7DC8356274844A8701\
0000000000000017A9141C901BC7EAC73BE7327B229367289914C3F8C6A68701000000000000001\
7A9140520628432B715EB5C5E71C9226FB63B143AF52887010000000000000017A9148CDD0AFD7D\
101FCE14A91C52E1D1B48A5C0744A887010000000000000017A914D4C7CA7027F8B6F1CCD3C1251\
D4795B3DED8719D87010000000000000017A914535555ACFBECCDA7D075FED9EA11AC869604C0A4\
87010000000000000017A914895CE2C20F8F5329F58C9F476D688810DC001298870100000000000\
00017A914642523314A6CBBAC3E7F652A3C05F9CA01D2E97E87010000000000000017A914F888A0\
5F0CB4D47827EB4E8783029AB13F72505287010000000000000017A9149B98AE816C25D5B6F00E4\
1C6B5DAD784D5D2742587010000000000000017A9144AB6708FFDF718A0613CCAD583A8A41897FA\
32FB87010000000000000017A914A54ADB5EA9BF86B3448B87F9C0757B20FFAA5E5387010000000\
000000017A91450F9EE314091D0230B6DE95929987E36C394148587010000000000000017A91437\
B644FA3EC78F48A3C462BA5E7160B2CA87261B87010000000000000017A9142913C54EF75E67162\
1D9B6C5522BF9FFFC6CED3A87010000000000000017A914D518EB76E9BE6D71EEC8D385556A80F4\
3C05FB6287010000000000000017A91460145AFD72BFEC87556B873416CE4587E7BC7E998701000\
0000000000017A914E13D5A45E8B0893C19A49A15EC08193449AAAB4187010000000000000017A9\
14F275F3DB474D69AF9EAD3EC0567370CCBF3E746487010000000000000017A914838D4594A428A\
F7F7529B3602C45D1502A65454E87010000000000000017A914F07C7841846632721D95C02E017C\
9839BD0CF2F387010000000000000017A91440A2B63563BF89AFF8E8CEE676FD72C01AF501B4870\
10000000000000017A914265FD3F8B5C6881563349362A5DA9B7EBFDE92BC870100000000000000\
17A91498397417DD973AF18F5EBF92EBB8E51ADA61D73D87010000000000000017A914E02183BDF\
728238DB4DF8F6385E17F5146DE4B5787010000000000000017A914D3EF660C101063A96E05D775\
CA3D4262CFD074B187010000000000000017A914CA4F7904B03B6B96A77C8864049123E14B440E8\
A87010000000000000017A91495B0E42FB1FC34880E675574382C7275EA6CD01787010000000000\
000017A9145E4E3EA2ACDBF20BECA9A0B4E70F2E87FFDE0DC187010000000000000017A914581F3\
FE971ADB2F85FDCBE7EDA0B70A66AEFC4EF87010000000000000017A914FBDB6EF5E4F091D7AB9C\
292073DCBD42AF5506C787010000000000000017A914B2CD6EB4C5578D657845C52F5B878F18AED\
E038E87010000000000000017A9147229ABBC2728111359C072FEE42A915FB082F4A28701000000\
0000000017A914AD95FF86E48E4C96A01279320DB16B12166E83DC87010000000000000017A914D\
4E2A86ED4B6989A76C118F317B82EA6E0A9541D87010000000000000017A91428B92EC9EDFBE455\
2FC568C97AE0B42F917EA27087010000000000000017A914AB6B2A19D09C7F69B6E44177399326D\
5D8E8F25287010000000000000017A9147D46DC1788214DBE9637A351A3D988EDDDD93558870100\
00000000000017A914581213888AD70FDD5DB8305AA93F4504B25480E987010000000000000017A\
9149D753C76F7921F9531EC105CDDFFF1E66247D58387010000000000000017A9148B5AB097864C\
10014229173DEFD76F765219E25E87010000000000000017A91413C8BA037C351CBC041C16CD3EA\
3103A5384D46787010000000000000017A91405CDA4ECEED8A9A0FCA48B0B555961EEFF0B812087\
010000000000000017A91404E9D5B60243C77B5877FA0D9C308FF9536485E887010000000000000\
017A914AC7D72F11F5B184CF7818BADA58C6600B7864DBA87010000000000000017A9145E8F017F\
D9A211928ABCD93CADEF0D191ED03C0E87010000000000000017A914A25C1F8109CBB12CC03772B\
D86651467CA73A3CA87010000000000000017A9141BB30B4C77F88CBDDE2B2DD27075FECDEED940\
5D87010000000000000017A9142708F8686520B25C5A11B15DC42CEB863EB176788701000000000\
0000017A914419140CD5FED24372407743C1C103921152CB0D487010000000000000017A914AC06\
9F9F56973138A6A1101B861EB9CBD879956887010000000000000017A9149805E9F52EF6EF4D0D2\
8C5032F0B630CDD828A8D87010000000000000017A914270C754DC52F2F0E7E5D7DBBF1C2A408C4\
03573887010000000000000017A91483AA75604E59DB268B13DADABD472EF2338C28A7870100000\
00000000017A914A06FA249C7E9CC105737B9BA3573EC6460204E7687010000000000000017A914\
762C8D3EB9B2CF33E5BBFA45E0778ABEFE78678587010000000000000017A9149D900AB6C40A5B4\
6D3F68935952DC2AED29014C087010000000000000017A914E935F3E9FBE55BBCAD75DD60BEF3F9\
5E00F1622A87010000000000000017A9140C6D2F1DA93D7304FA9240736BC58D8840F188E687010\
000000000000017A914165C28B021E832BFF1B1A8C45386CBCBD706642187010000000000000017\
A9144AEB76BF356CE525AFDC2D07D2F66A69A167178987010000000000000017A914DECF7735663\
7EF721435F363FAE4DD9C45D89FA487010000000000000017A91410330D1FFC1E7A7AEF0C4C2EBA\
531F3F0D37FD7787010000000000000017A914CC5AF4BE892B7EEE8499022B5998664B8504ED318\
7010000000000000017A914BD2CACE8DB731C3483A6648B0203136FD72052A18701000000000000\
0017A914F113CF6FCB2C895D31CE830E16032ECDFBA6DAC187010000000000000017A91494F864D\
FC0D4B10AA6EE9AE2F58EC53CCD16340187010000000000000017A9147E21745F4C49BEE7F5D30E\
07E13AD8C3C8AB950287010000000000000017A9147E2102C20006A75E9D913BD49DC078E39BEDD\
39687010000000000000017A91411B30AD6F1A204E62F8E695370D27D10EE9155AA870100000000\
00000017A9144DE5EB7EBD52A96B5438B390A04FF62EA207CC8487010000000000000017A914D75\
EB7FE4A09459499F192D6896971BC95CE583587010000000000000017A914C9D6E6D1E73B7B4F00\
61D6BB3FF656846D96DA5287010000000000000017A91449B9144F7CCC4A7F074DF64CC74818AAA\
2B6C5FE87010000000000000017A9140E84334F6AA7C114DA10E97BE1EF53A812D1834287010000\
000000000017A9140BFBD65E2B36441BF47B30F14E9E6F0A6CF5E4DA87010000000000000017A91\
4425F5B52C0044FD27731F8ABEF1E2EE3E8CFFACE87010000000000000017A91466817593EEF0D7\
613746B695B46D184A23D8E31287010000000000000017A9146682538D1F6A4CCB7C71469232D95\
F790DBB4C0787010000000000000017A914426F543B496FD88A2CD896D26429A913856379608701\
0000000000000017A914B5485AE46CFC4842AAE02E7FACE0338413D6C3548701000000000000001\
7A914A4245E47B25EB66A18FF2108686F7E9BE08C618287010000000000000017A91470068C97C6\
3EAAB881CAC364C8E0F38A4604178687010000000000000017A914BF4BBF6DC95CA387A9EF8D531\
265D18777F60B0587010000000000000017A914DDE4B005C42AAA6B5F89A61470E9DBBB6F7E8523\
87010000000000000017A914E30EB89F8D6C7C9FD6DD1605C8372E829A1EF5DE870100000000000\
00017A914B678DFD785D0561C72914E4532F74E7DAF85607887010000000000000017A914DA3781\
495578BA74988C469768B03CD50C2C43DB87010000000000000017A91411A61C469AC90EEA146B0\
6C9F8FD980AE608787C87010000000000000017A91465E1F07D929993DC99AFD61BAE752053ECB5\
4FA987010000000000000017A91456A2C8FAFA39472C2BECCD8F38B03D5E91FED3D887010000000\
000000017A914B9CAC2E4D30B0FF4B0FEA49648944237CD46A71E87010000000000000017A91487\
60E52F67FF5963F865CD2A65C1C61892B257D987010000000000000017A914EA49A8C5E86777D42\
D262EDA5ACDABC465C3827087010000000000000017A91447F14F0110945EFD905EA7C6B785D78C\
7BF8C8D687010000000000000017A91436D1C380FA90DC30899506D44E5C6A7B1F18B57E8701000\
0000000000017A9148E1451BE1BB6000AB6A7315F8ECC865F863AF2A087010000000000000017A9\
14F9CE61FE0A6C55B467D8EE42790E3036B03BAA4987010000000000000017A9144CF66020E20E4\
FDA1F636C423FD569BC25913B8387010000000000000017A91407E4464354AE587FFA1C53B562A5\
99AD3BF908FB87010000000000000017A914933FC6041DABF1736BAAF78239D767C6BF88FCEA870\
10000000000000017A914867926889BD384E1A09346EA161232027BE14574870100000000000000\
17A914DD30A5077118821BC6FCA81C9D4A3D0FCA4C7DC287010000000000000017A914F35108013\
8A6DEA739D0B73C8D9DF1A01DF08EB787010000000000000017A9148089FDCFB3E9F4C321AF52BE\
EE44356392E84A9B87010000000000000017A91405865E04E90D1C72C0D253277FEF5F9BE2B5119\
287010000000000000017A91475EDC690D73822750E616D9ACCE38B8641D0D3CF87010000000000\
000017A914B7C6140FC4D835FEF114C9610DA07E1E493FB55B87010000000000000017A914584AF\
976AA48845ECEFAEAF67CDD5C9B8191531B87010000000000000017A9145425360053B819B35485\
C402DEE4556B8BF808CF87010000000000000017A9143254A83CCD778665A4F693FE91EE0BF565B\
BBCDA87010000000000000017A9148123956B71C8BB8E4ACB1288C9A99825091C4E738701000000\
0000000017A91406E542FEACF5DF53272C214AB33340D58B0589F087010000000000000017A9141\
92A3312D7940408FC7A9B7D8552EA23F4FA055F87010000000000000017A9145FADA5D3721D1060\
3A2BA5BA6DB315A1174EE70D87010000000000000017A914F8342C718E378447E6317958159F9C1\
9419BE25B87010000000000000017A9147D5755D78609E2D74DE5B938C0926BEB3A7A4126870100\
00000000000017A914253BCB6ED368FC88D4913FAD41615E1BA494C4BE87010000000000000017A\
9140E0C209CFB6A68DEA636ECF0C1091502009BC29A87010000000000000017A914FBB75EF7A0A2\
9354B4B12801B7124B10CAED38D587010000000000000017A914B35D446347C2316B0911CB3D0B7\
CDB0BFFBA937C87010000000000000017A9146EA21BD602B559321E1064E637DE059B8877A64D87\
010000000000000017A91461FF4ED47D632AD2CA987DB2DA654DABD1E5638887010000000000000\
017A91492AE41427B497753560AAB3064953419D67C208887010000000000000017A9145809FC01\
3492DAD43EE0D6A6320DB3A06E70FABB87010000000000000017A9141BAAE837B981C6B84F04D2F\
A1A7E07D105A9BCBF87010000000000000017A914670D95E98A2E5590D4018864AEADF0EA3432F8\
6A87010000000000000017A9142B77FD1655426B6EC7D4F842D99E2E059F59A29C8701000000000\
0000017A91440962121BBCE2F6002EF91532DE48CA1CB2F505187010000000000000017A914BFFD\
80A636717286D961E5E07DA8FF81D688A54F87010000000000000017A91485160BB7F5BAE220C29\
8BEC7B84A6BFB81F87AAB87010000000000000017A914C640B28233112FE5F81B47DDF72F0044DA\
D514F487010000000000000017A9143121B7F901A3987849709AE606CE07165EF71437870100000\
00000000017A91415B624C5D88B032797F9D1F34686B7C7AB7C02BF87010000000000000017A914\
510D00D8A4F94A2864BFC1922297471F2E5C667A87010000000000000017A914835E98CADF2EE03\
47C158BC36F2170990136EA2887010000000000000017A914E15DFAEEDCEB09F0A7718172477C27\
49C71DB47C87010000000000000017A9143A8936F17F344F86CBEB64CC25D863F5EBAFF7C587010\
000000000000017A9143FB864208D60F6F6C8533C91A38D71623A8A01D387010000000000000017\
A91470727B17ED0C6171FB62F09179DE918F176DADC587010000000000000017A9140935BBB9118\
86FD563C9FA9772D31FDEBC41B01787010000000000000017A9140E1BE5A5B2039502914490867F\
1D79C719C0B6F487010000000000000017A91401B9A3738438F6717DC6DF7BE88EF57CD59D34F78\
7010000000000000017A914E81A63930029630A37D67B63777B82D7163F54318701000000000000\
0017A9147596218AF86464CE82B9A9A88207E5443806A58387010000000000000017A914C105AFE\
A73060245931E4E7F2F69BE40A11D54C287010000000000000017A9145D9CF011B077B87A9C3ADC\
77CE066FDFC2609D3D87010000000000000017A914721A1986BD02B1C3D8B92E943378A99B9DEC2\
0C387010000000000000017A91488A37C52D221280DFF9190DA5F7BAFA1695B17CA870100000000\
00000017A914F3339B8C1A4902685D6E9C1BB32016BF3DBDE63187010000000000000017A914F49\
46DB87A481218049B58946C19CAAE5841676F87010000000000000017A914F80FDACE31D16C3DBA\
863274B237A86082BDD41D87010000000000000017A914BEC30695BCCF7552AA4758BDE2F60B8D4\
8BF1A0587010000000000000017A914BCD337D21DC54A07715319A26D6BCD64B2730F6987010000\
000000000017A91476DE3E9890B6B1C9E06253E6D1ED4526662CC20587010000000000000017A91\
46A8683D84E6BFB777D7F074ACBAE3E80F1E802B487010000000000000017A91440AA07A1B3587D\
5AADBDD46A767E30E66BD67FB387010000000000000017A9140E972742EA0B3CD7135B3C36510E3\
4B96A5648DA87010000000000000017A914676D8D4D6544B3E3E699C73AB49233FEDA3DD1B98701\
0000000000000017A914B09EE248B5FD49D30F8987A594D7926343B8C94B8701000000000000001\
7A914988E01725F197E6F872BDBC2BC02037ABAAF1A7387010000000000000017A914DF21961BE5\
EF077708AF8488D9DE14719A995A2B87010000000000000017A914ABA2C4479C1A2C119E69336C6\
786539D4558FE8287010000000000000017A914F072A223E4CBF74666B2510E1BC616489713C792\
87010000000000000017A914A18953952A78DFCC46EC0FF3FDFD43D78377B8D2870100000000000\
00017A914B3B81AB7D62FE1B8B789CB80BAFBF92D544B941D87010000000000000017A914D08180\
0100DB0FDECB04B0ADF545CB7A644DCA0487010000000000000017A9140073ECD12EBCE876EB672\
450DDA931F51937027C87010000000000000017A9148D64779ABFEF64112428E02A3EDCFA1ECCC0\
DF7087010000000000000017A914FBBE45402B6A9366CDDA937854D0A9629089BA1187010000000\
000000017A914CFB72B96AC56AA9F00163E16768DD29D1B11785387010000000000000017A91429\
CDA98D619335177D015A57FD9FFED52F590E5387010000000000000017A914FAF5219E71C4CCB0D\
CF93770178109FE0000C1C587010000000000000017A914465BFBF4E951771B7DAF21177137E126\
7688E19887010000000000000017A914CC3BF34FD7A84A5EF26406E96E3077FC6BBDE0358701000\
0000000000017A914A2C751ED05C2C283A1FC962AEF6B4C26FA93E05C87010000000000000017A9\
14F300D06971564E2405056A7150EA39A50B39802887010000000000000017A914FF9DD37C9E79F\
69A85DAB5F79D9904FDF76752C987010000000000000017A91403CC95F8A0A7A8D8ED7C1F97C441\
0ECFF69824BE87010000000000000017A914193BEDA5CADD0DE8E855667367ABC78638FEA167870\
10000000000000017A914D1D02C203A5A9CE08471A5124FA27A5AF76438D4870100000000000000\
17A91430EB790E5DAA0F0ED48393455FE8E0B15183EE8587010000000000000017A91428B3EC6B8\
75348342B14D5AEB2AAA0E8C542137D87010000000000000017A9144174C34F2357A64DEE8B4CE7\
47925C4241A5CEE687010000000000000017A91443AFA1D3E0F3E1B2E1CC18B832F8D27BB65256B\
187010000000000000017A914122F194C2E1661884337BEC2819F4D7644B1318287010000000000\
000017A9143D36B1085585A6F647DE0636FD4589E00EE655DA87010000000000000017A914D822D\
B79B4AA21F75B88A030C37CA5E7F094334887010000000000000017A914CCA8BD52A1B1C705914A\
A3D74770E97E2AB2A23987010000000000000017A914F7E7028BF7A2DACA99A30524FEDBDFA97BF\
7BA8087010000000000000017A914885E069C4002C0FA8220EF3B242B11EE947C3A2C8701000000\
0000000017A914A89F3E7379BF34C436AD066C72DA159DE352776B87010000000000000017A914C\
22491F670BC46F6424FC8C06575F98B2325B17087010000000000000017A91452C33793DFDFA9A6\
8457D009F86B3C3EF0360CB187010000000000000017A91471AA8CCB79EEBF2D0F2BF1B8248B44B\
96EBAD5D387010000000000000017A9144BB578612D9012F06FC64EC09AB2EEB081E64ED2870100\
00000000000017A914BE9AB690CAEB09E3788FD661CDB4D778239762C187010000000000000017A\
91489FE1EE34A5FC640BE9BE0BFDDB653AECEF4A8D387010000000000000017A914B2FD1311EFBA\
1F0865A3419138AE89260369374887010000000000000017A914886AA29DA4B13193C1E26BC793E\
1806FF184C58587010000000000000017A9145030EE56CE2F1CD8F412BA738D3A75D144DCD9B587\
010000000000000017A914D0A40C7A190A43F6112DE04C2FD0A1E65CAAE4E487010000000000000\
017A9140E68E6D203F1856838AE1E2E5FACCFC2B7F4AD6687010000000000000017A9142EF5C79A\
4FD98C29F4E710EFF0718090563F905887010000000000000017A914BC34AA7ECD76E8C2479DCE2\
B41DC38314B10F5A187010000000000000017A914F503450C5BFDCD4AFC4830B678D2857B7477E7\
7187010000000000000017A9145C72DD16E61067C1C3C68DD6529137034049F9EF8701000000000\
0000017A914AD1B044DAA5866D363997C45B3C8CCB65B7BC31987010000000000000017A9147A84\
4FD9E52BCD2DB4C9D56AF6643B4E948D206187010000000000000017A9148D4C97CF6B4B8B6C0F3\
7114BDBF77835871AFEEA87010000000000000017A914E4C6799857EAB7FD0A35B880F134D776E7\
DC9DC187010000000000000017A9142C536A911A8B81B286398A5E829FC4629084D57F870100000\
00000000017A914D9F83FE22BC0921C873CC3B73B1D1900B71766BD87010000000000000017A914\
B14DF1F7E5BEC2D285A5EA807E5499E3D7E0781287010000000000000017A914252FC7A2CD9BE8C\
0CE320E1B969556428E368D7F87010000000000000017A91491FA9E22C36C33B112CBED0EC4D867\
D907AA432287010000000000000017A914EE9F731C62A06AEB662D1B03E924CB7D37A76C8687010\
000000000000017A9143759F3043B5E7FBE145CD220C706B5D754718EC387010000000000000017\
A9149EE6D3FB3F5E1696E2E45798C350182E3DF3208C87010000000000000017A9141FA6C7EB877\
AEFE1B99CD83B0141FF549E92686287010000000000000017A91419E5F4E609B910AC5A05E6EE67\
019FD8472DF63A87010000000000000017A91496203F0699D211145F601489C393CB7BED0124238\
7010000000000000017A91424FE83A4D423967E5F0C6BEEE9D320A0E34AAB708701000000000000\
0017A9144FC7745AE3965A4D15AC6D47431CAD6F49755E4387010000000000000017A914E97FD6C\
ED2EA5C03194CA8A80D76FFC008B4396887010000000000000017A914BF1CC988CA395EA8232C16\
F179C16BE1C682A67887010000000000000017A9144E2E55B6B02FA914F3BA3BF78AC6EEDC867DC\
5B987010000000000000017A9149A69933478E6D44DD8719B2CB3DF9A6B3D6BE592870100000000\
00000017A914B6718DB69D1C238EB6924D15066EA01082DBA85687010000000000000017A914BC7\
DE737FE434ADEFEAE6A2D027C1BA07555D88B87010000000000000017A91403E40B1AFC4C3E0354\
F3501F0DC7898EE28DCAED87010000000000000017A9149AA03D684B994E94B6DB0C9F09B928C8C\
0A78BFE87010000000000000017A914600B5102BDB15C9E77FFF7DA2C89FC722E37E35487010000\
000000000017A914E0D17191B26B50F97B823FB8C07A383913DDA01387010000000000000017A91\
427A2143B86DECBDB7E6C15556C669EC087251E4A87010000000000000017A914D251B3994727C6\
A076D093F5FBE72228AD12F8EF87010000000000000017A9144BE333941300D67371F4451CE3E54\
FB78B6DD6FA87010000000000000017A9147300389B3156DF7DB0EA5D3FD62195554AE8851E8701\
0000000000000017A91486BA0AECF95D4B6006FA9F84931942992FD1642C8701000000000000001\
7A9140C77EE5C36B167AC7A5D0C398286B392AC217F1887010000000000000017A9142A0B405628\
9B08EA4039A8D72816378996925BFE87010000000000000017A914EC17633ACF5B2B7A19FAA178D\
8EA629182B3CDBC87010000000000000017A9148E1B205320245030CEDA27F41954D2922F79458F\
87010000000000000017A9149DFB959A2ACF805F362E552AB554AA6D7E4381B0870100000000000\
00017A91425DE21AB0437DEB00CE00CAD61BEE8C8E92C59FC87010000000000000017A914FF767C\
BF82D1451500886E4757213997AA94B9CB87010000000000000017A914AE89062101EE2739A55D0\
2467F7A37DBEB1294C087010000000000000017A914CBF79843A2B34BC3340DF85109ACE737D854\
1B9A87010000000000000017A914DEEE6AEEC380FA2B697351515710B558C3A3042187010000000\
000000017A9141526AD19F487778942BDC68DC3A0013171737E4D87010000000000000017A9147C\
D6787A3A93BF81CF2FBA230042E9A148E78A2A87010000000000000017A914270054CA0C55230A3\
421D32D74C75353A79F0D3487010000000000000017A914D4C7841BB907D8C015F6BCDC20316FEF\
B1573E0D87010000000000000017A914C3484A318C7ECBC50C30FA8F1D46B04C57728EC78701000\
0000000000017A91433D4119D2D6722108A24E537513B385B4EE6FF7A87010000000000000017A9\
142E2854782AB725B0724783754715D0F657D21BB087010000000000000017A914AE4615769BF46\
752DAD0435230BD2A29BFD7D37287010000000000000017A9146E8BC4BD4884B7A15F48817A8B53\
D7E27641454D87010000000000000017A91480DE3949C26FC553AB879818583EF7C49F42BA49870\
10000000000000017A914CE340005FEFB35DCC72BA56AEBDC53EA9AE8AA0F870100000000000000\
17A914469F788E989B4BA1FAC831E6B2060BD5DF0E565B87010000000000000017A91404395D69D\
F4A6FDE69E0F9F57B039E0B65D3C63387010000000000000017A9149C22A367BF4F94EEEB31F22F\
9C9CA25812F4065887010000000000000017A914CF0EC225CE90A841DA0ADB90CB421758E4029D5\
487010000000000000017A91497391AC7C937B5103057CB8CA661FA6DDA6A757787010000000000\
000017A914739F0DCA54CC07B50BE12BF752819610B690D7DA87010000000000000017A914B7301\
36BF80791683EF379E250D4876BA3584F0287010000000000000017A914E7C4BEA14DBAB969A042\
056A82969A7CA5A89F8687010000000000000017A914CF37CFE2BD2FB8A5D2C1A5F747268D08928\
CE1F887010000000000000017A914FE9186D53F4BA098D88A9E02298C9851B1BE16218701000000\
0000000017A9143FA4B41F8B05C1A8ED0136326F8C7D9718A0BA1D87010000000000000017A914F\
51CF2CF58EBAA594A866CD80C7AF2B521BEE48D87010000000000000017A9142ECC8A5DA870F867\
E16C60FBF36694418CE02F3C87010000000000000017A9147F806C494D5D3AEC54D8D26B9294E5F\
96BF5DD0E87010000000000000017A91458A793592700874FE0C0E920013277B31E95212D870100\
00000000000017A914175FF124DAE86F26CBD580276E02555C4743046E87010000000000000017A\
91408B7B4CA9F563223B2A1343F93EE47DE9AF353FF87010000000000000017A914274AC7377139\
CDA20F4E02A148D498C13ED909AD87010000000000000017A9140CFB8F57066A6F5B56F5B2F5894\
BD6133110EE4D87010000000000000017A9140FC934AB393D0A6619280047D9277160A883FEFE87\
010000000000000017A914A4B57CA9FC86EB437BF187D2E26CE27AFE609A3387010000000000000\
017A9147F66B2E18A54FB289C265779591D1408567ED7ED87010000000000000017A914DE1EB114\
848EA4CEA71346B4213D53EC188C341887010000000000000017A91481FE25344EAE5B08EA9075C\
E8ECCB72A7380849187010000000000000017A9141C80D8AA95A2354B761AC5699670A8383DA23F\
2187010000000000000017A9146BDB02F18655453D51F5939DB1411011358AC3578701000000000\
0000017A914D6AF4166E3583275938203ACF3A1D5FA61A6679F87010000000000000017A914F0C8\
F4090A002265741D7BDC9D7DC0A40A777F3087010000000000000017A91450EB41077583C81CC5D\
E2B683CAAAE2E0210887687010000000000000017A914238D05A394CFC8551A65A381FB163EBB48\
8AF07187010000000000000017A91407CA837ABC6A48F376FC0061A5243A771622E70F870100000\
00000000017A9143D75F21162AD8360FE2F445314A92A144BB285C087010000000000000017A914\
AF188973B2EE4EB8BE18AEE4CBBAB5E3AF3E046887010000000000000017A91459A4AD843BEC26E\
FB23A3D79ED4DC1FC3198932687010000000000000017A914A76BAA708BA4EE6C41FCF5F3B302B2\
418B48D88287010000000000000017A91404631BC00DE946332AA0583B176120ACFD445BE387010\
000000000000017A914D9E444ABB16FAEABF9D4AA1A0E391E03997AC54887010000000000000017\
A9142B49A5B04A5A14E5FCF8DF7AAB7BA5C9B53CD7EE87010000000000000017A9145B4D758DB15\
690E7B603E0C68B95CF853452458A87010000000000000017A9145C341F58322BD5A9643F0F3C6A\
08763A25781A6087010000000000000017A914571A9E1E0EE45833966ACB18B309E657DB8EE4088\
7010000000000000017A914103AB4F9AFC946382ADBD4E7CC5B3A7CBAF135CF8701000000000000\
0017A914C39132CF38AE124AACA27C4C02A4591AD63A7EDC87010000000000000017A914B598152\
AF624ACAE637DE23AB7851BD967E2037687010000000000000017A914C75D485B0E7AE95B01E3B1\
E21E94BC329398AE9D87010000000000000017A914DB5D962C19E67641D17D86879B1C390F0239D\
2AF87010000000000000017A91439DEA5CA47E1BEAF77CE8642DDD46C39DF09FF64870100000000\
00000017A914C75CD554F3E27A78BBEF732A5352425077F664C587010000000000000017A91442D\
1C44C1A1D54530BF70DB95A2015372B21B9AC87010000000000000017A914C60938B8C5E5F44169\
3E40433C4A6AABA0C9EE6787010000000000000017A914D9C135E9D10AB64AE36E97BC9E44CBFA1\
EF1495087010000000000000017A9147959F29BBD26D56BE8CD24B75778B9FA484047F287010000\
000000000017A9142A4529E64741331B32AB5394BB4AE196BEEAD76587010000000000000017A91\
47EE88D4DC5751005F23A9A0E98D41A2ABA5264BF87010000000000000017A9147FAFC1CF01E001\
4D512997A41DB7999BABE6DE6F87010000000000000017A91474CD0DF1488268B6E90207FAD3344\
F3C1E1380A087010000000000000017A91476FCB80FE5A1F075D88F919947F5E7CCDB42B57E8701\
0000000000000017A91418913B6C768F5BDE48BD27D80149C24C21D24B7E8701000000000000001\
7A914D136947F33E147B996577FA474C49949E8F7DE3A87010000000000000017A914D9D100AA2F\
2D5E2BB3B277D1F0BB0E5F8BD4AEC587010000000000000017A914AA59BEDEE44D5859FC5F09DB5\
243A1DEDC43792B87010000000000000017A91490FCF963AA19E4CC99D8448BAED36F047A3C7126\
87010000000000000017A914B879EF1296D4505DC28D8B8F3160BD59F11073DF870100000000000\
00017A9149390B6F5ED27D39D55B502B89339E840D32590CA87010000000000000017A914E7EECD\
963A14E15A17AA2517253F7FC75DD1E2D087010000000000000017A914ACD9FB6313C9E630D3B75\
0A22840DFB25609BEA587010000000000000017A914E5C4FAA21D5C8B2B95F2E9F0E726AB0C5A4A\
C1EF87010000000000000017A914673F44EEA31C0DC7262417EE84302452463299B087010000000\
000000017A914C12DE1CF8FA39DBBCF090F6E0FF6A1AD3573EC6087010000000000000017A91467\
6F3B57DCCA8230EAC8D7B49E48F057FFF02AD487010000000000000017A914E424D64919CF96492\
30007C315E144C8795AEE6787010000000000000017A9143FDC4D42F53B2784D90ADD947C4A7ACE\
652BDD5687010000000000000017A914FE40CAC6C2DBCB32D6FF298CE9A7CB5E0C3ABA5B8701000\
0000000000017A914E40816FF7C1AF60A662962E88F8189784823D0D287010000000000000017A9\
14FA8AC962EBC70D43341A7748F869B5E6AA62E45D87010000000000000017A914CAF29204C79E9\
9A8C91473DDD526E543A537824187010000000000000017A9142696A4E86CE306EF71A9422967A4\
77B6338E113187010000000000000017A9142090BD22A819BD9D0AEE95018FBB8CFD35F24B26870\
10000000000000017A9142E250CDECC2C9624E9FD256E3FB8245205BA2CD2870100000000000000\
17A9145C679FAC8899AECAFEC7E245FDAA0EA11DF2689F87010000000000000017A9147BC213A26\
202CDEE568F54B515D4AD99D0EC39B987010000000000000017A91474F09E6B09A6EEF744ED0176\
9A2FD671C169115A87010000000000000017A914F39870BC3EA92B72D15755557632F282C60390B\
587010000000000000017A9140A93BF0B71C05E36970282422FA0908627CE303B87010000000000\
000017A9142EB8B3FC9148B183A75CED0A6005D09012C732BA87010000000000000017A9147F75E\
1A4982E69F5C01714BB52DFC46FE4E98A2B87010000000000000017A914DE73D3EBECE62E151704\
320A289938844D6C9FC287010000000000000017A914B602BC62BDBD6058C1E38D9E7AEDEFEE47A\
7EF3287010000000000000017A914400A2629B9301AEF1583EB35F039A3531A6303988701000000\
0000000017A914B471A74509E7B8A13CC945A4C35AFEC2F3DA5ABE87010000000000000017A914F\
F74F39D2193234A89FC215395703CCE0E9BD30F87010000000000000017A9141B1BC02603A34DAA\
31614C3024A96367ED9461D087010000000000000017A9140245108EA8923C63DB7773234F24BEF\
2AA612ED387010000000000000017A9149F8C77E44A82A8A17932BB0E6FDB775D491ABDA6870100\
00000000000017A91421322E23320302FEC21A4D4B344F2E95D577D61987010000000000000017A\
9148EECB8489BE2355602DF87BF6A9E7D1181D9C0A887010000000000000017A914A48CAE84DD84\
65AA20C17EA9B460F0C2AFF2FAC487010000000000000017A914C46DD29658B3A58FE5F463F9EF6\
89765DC4C427B87010000000000000017A914A0BB21691A4062C685FA29BEB22A827A2F8685FF87\
010000000000000017A91474BDADE13FD6F7A1C144014BA12DCC4BD49F07A187010000000000000\
017A91412FFB1438C1007173AD4A8B808275C116793358587010000000000000017A9146F4442E2\
F64BDCADB1F068719C2529CDA9246C0F87010000000000000017A914AA2D17D6224FF5C35F51BB3\
3264C67107DCCEAE087010000000000000017A91461332279E2B06FCAA5CF8F402F4615B05C1FE2\
F987010000000000000017A914199CA9DB8418DA16530E395E88FF186D44F5E82B8701000000000\
0000017A9148DE9ACBE4C839041E7202D116CEDE188A01764E087010000000000000017A914E762\
8ED06694711651F9D5CC8DF59C993BE7D55E87010000000000000017A9141484C3AF2B36178BAFC\
1FFD07F52F2F068D84C7F87010000000000000017A914AC5AF93B877235F43FCA6B593206DBE0DF\
A4F4D787010000000000000017A91427A747A75A9520077E015DAFFD20E71D3428DD0A870100000\
00000000017A9143924FFF265A74AA669FE096DBE53FAE1562779A787010000000000000017A914\
BBA093D8BA7C54A3892D7018742CD318233E303C87010000000000000017A914C8CE1DDE5045BE3\
5722AB43301B3589D0B44403787010000000000000017A9144903A1FDA08E7FBBED4692731BFEF8\
20DD177FBC87010000000000000017A914AE3ADB61ED621BF1EA2EC9E60F686863756E7D6487010\
000000000000017A91416E6C6FCCDF444EC64FB618C473473580C2E625D87010000000000000017\
A914B2369C6C71BA1B9B32AC5D56E867BB70175ADA5387010000000000000017A9147F262FEF887\
23610E8B8F763562861B057C9B7EA87010000000000000017A914788472F65E16C3D436AAC9CF46\
287130DED5AAFA87010000000000000017A9141BC6A9D246836C1F535EE466577D08FCAB1C4FB18\
7010000000000000017A91452B3ECF6E9706C0DF31E7EB6B7A590A24498493A8701000000000000\
0017A914FE5B69A89542023B6CD415D5AA43F3AD09FA6C8D87010000000000000017A9144EEEEAC\
D9D42078EAA7182E5C2874D763B44A44B87010000000000000017A9147F187DF1D6C02223D7FE67\
6DFD52CF6C5EED318287010000000000000017A91498AF958723F72C5D40B8FB74B1DCB7D279141\
71787010000000000000017A91401F84130BF89902A2587CE9AEF41420E26429B99870100000000\
00000017A91458A486D08D2432D814BAD36D5D9C1FE3686EFAC387010000000000000017A9149C7\
173306E265C902A2EAB6829CA9FF48E9E9AA687010000000000000017A91459B9F40F6061E6FFCD\
ACC496245AE0F4BED1417787010000000000000017A9142204E2D73423B6A92C5ADE18B7A0A6592\
648299087010000000000000017A914E8BCF37B8C2673110E6D40FD73339EC7BBA0DF3E87010000\
000000000017A914E3BF404F5BD6A23B801F12B71C130FFE7A4DAF2987010000000000000017A91\
4268EC3826B434859D2C571779CA4C5B8891CA57687010000000000000017A914FBF121FF24BF88\
CD7DBE7D30D2495C7D81EEB92D87010000000000000017A91447297692C54A9B7B40AA380D959D9\
E4D9C2F9C8087010000000000000017A9148135789D8CA39CF9E7FA962E881039217116FCEC8701\
0000000000000017A914B9CE97F9B247218DA4EBF471D3A9BD78133E9FF18701000000000000001\
7A9148094D0EEDCDCBE620E498A125DE5017744417DCA87010000000000000017A914F1D4380667\
2AB44DF4D59DE20DA690BEFDC9D1C687010000000000000017A9141AB47EF15BADD8F61E0AF277E\
E82B187B2C065AE87010000000000000017A9145924AD08ABA1A049786B12E921C535B6B8CDACCF\
87010000000000000017A914061E52E9B229100E8C9735088309EF884BEA5DCF870100000000000\
00017A9149B10355F6BC52CFC6DB177B7192E0A18E86B83C087010000000000000017A914E63DD5\
FB2D03FE6F7DECC134E4BEF8F8BAB40F8C87010000000000000017A9140B565CFCB2BB894A95FF2\
E2A071D6669AF3DED8487010000000000000017A914EE626602F21998C76EC78D3624323D760090\
FC7F87010000000000000017A914AD7690F3DEABD58E47E154DF496292C32AC9E3B087010000000\
000000017A914F6E3F6848AB7E3386C53AB75C695A8F3302FD63F87010000000000000017A91415\
EEAD3CCBC6DAA8C2DB4BF86FBCE7A787729F7987010000000000000017A914C4C10BCD0317BF994\
C88F99AD0CC09DC66F8DF1987010000000000000017A914CD7B90629D6618309841775EC3F44DE6\
E0BDB1D387010000000000000017A9141FC481167EE6251AC19D01AD7165328B8C254E2C8701000\
0000000000017A914EFD7235F2ECFD182000350A46CDE4F2322373C2887010000000000000017A9\
141CF8EF05CB6E939870720C9F0BE8CE1D02A00C7187010000000000000017A91430918B626CBC8\
6E44F723EC19F7A808ADB921DAF87010000000000000017A9141CBF724AC35640E9ADF6E3147D77\
76854F10133887010000000000000017A91415DB4B36864AB5E6E473EB87A542D42392DC11BB870\
10000000000000017A914F66132836B4BA51F66CAD1FBEE63D4C70FA67E6F870100000000000000\
17A9142C20B9D3ACDB5338641CE7641F7151C2A6AF3CAB87010000000000000017A91440537FEAD\
74A78F8970E2E1365DF2727D4873F8387010000000000000017A914157B8B7197A6E81F241103B4\
4761F24E434D207787010000000000000017A914AC3214A4BDEDFDBF8CE32E9558D2BDE92387226\
D87010000000000000017A914464141BACEC11A212CBC6E37FC68794D2DADC29387010000000000\
000017A914E7BA02DE2BA1C9E1EE94BB28E24171E910B5006E87010000000000000017A91456C55\
DB9D9AFE74A47A45E1403A049BE81D752ED87010000000000000017A9146FC30C000BBE87E6A344\
736EEE6868084FC13DD187010000000000000017A9140C6D58D15A42378E28746B77DE05C62E148\
0E60D87010000000000000017A914B23FB61AFE915D8D7B3FD859328523D7D8D19B538701000000\
0000000017A914AEED3A549A9DA8820D49DA553B8024F6BF1AA39587010000000000000017A9142\
249CE3BF77BCA1EB89136CEBA29FB4AC73EF97087010000000000000017A914E1BA9C1FAFBBD35F\
9FC6679D86FC1ADA750300D087010000000000000017A9149CB8020195F36BDC1043E6BB3AF5BC9\
964A063DD87010000000000000017A9148B3820F786342AE9FD8E10E87325B9B59AD11814870100\
00000000000017A914730831E30994E4D3612AD1E59C323D6248A8366987010000000000000017A\
9148C36C5A959801E5B00EA1E6B01F976DC229F26BC87010000000000000017A914D3EEFC28480A\
AD414FB38CB46E06EADCAD66425587010000000000000017A91488DB2B043FAB4457712F613C083\
FF0E1E740DF4987010000000000000017A914DA782C118CECD4941B2C72B47E3F9B09CA6DB8B587\
010000000000000017A9140A2A6AC5DAF05AFAAEFBA9334D79812FBFA8A27487010000000000000\
017A914767541033ADEF2D7BFB41691B458A8F9B7AEAD8787010000000000000017A914F5491855\
03B57BB62B3B84ED9A34943C3D38019487010000000000000017A914ACD1FD793AFB65DB9A8FB6D\
D163BE2F58E3CBD2987010000000000000017A91428AF9DD86C8DE4FBC7BF4E909A7D8DEC14877B\
5287010000000000000017A914E94E559062506E6263E7431A211737FE9A0FC6AE8701000000000\
0000017A914E6855519B844B78C46C9A1BF2835805E02C32B2687010000000000000017A914C495\
50FB5DF79D4C4A76BF1B558A0580A90006FF87010000000000000017A914EF323ADD2C32EE8D678\
16701A7EE415953B54D9387010000000000000017A91444FA70AB2339E80E505A860D3EBFE766B2\
40FBB587010000000000000017A9144B9D64A3283232B40FC5C10DDB5B5D275F06D72F870100000\
00000000017A9144E4ABC391A934FCEC194F81201AFD3C129F346A787010000000000000017A914\
CD3E60D78DF813135AEBAC7166A318DB74E8EC4087010000000000000017A9140E514E23724BA65\
0656165BABA8057687F3F8A6787010000000000000017A914410DBA043DA9B4CAFC081854E94669\
1AC9F3B98987010000000000000017A914D08FBD215903E22239C0F5425309314B5A6A35FD87010\
000000000000017A914CC6C996738E37721A5FF88A2B2A879296944D86A87010000000000000017\
A914804476C98ADFEF6AD9C814FD92A9CCDF7AD836D187010000000000000017A9146FE7FCA43DA\
6EF32E954EA21E2AA56F1729806C987010000000000000017A914DAE1520E54A91DDA4D35273AB6\
D5F2CF5C7B440A87010000000000000017A91471D71DFDDDDE298E4CB39699F05735A2A12408AD8\
7010000000000000017A9143398D2A336A097A9365164FCB30236653DB27EB58701000000000000\
0017A9142EFCDC19B87A32BAEAF7E8E4C90CF50B375F614687010000000000000017A91405D61F3\
BBC3682BB75A46CCEE19045106FDDC6FD87010000000000000017A914F23AA2F849A06A60C82B90\
5B8F257D893E2BEC5087010000000000000017A91487AC418B1E04A8BC38A77E283E64E2AF23106\
0E487010000000000000017A9142220C8A391D35617340D4A06AACE8765FF1DC335870100000000\
00000017A91414184738932A43B2DDB18D3953BEA83B771AE91287010000000000000017A914BED\
5E7BC76EA89E798E94AFC246A2C02C27E1DF887010000000000000017A914E368EE479A77782ABE\
B6FA24CB83A8E770506C9C87010000000000000017A914208C4A9398227A458AAA9F19526CD1A7F\
5105A9B87010000000000000017A914D96D74A7139994922D3AF4AA21043FF4848DE59987010000\
000000000017A9143F391914868FD6A2CFAC228A9B815B7D76A6C3E987010000000000000017A91\
4E335B244AF2B1A03D8B48672B83F1AC996BA4F1987010000000000000017A9147D537CC73791F5\
F452C19C94AD835F15496A307587010000000000000017A914A201BBCE26A271F9803EE954BF530\
9DB1814FF0E87010000000000000017A914CC1B6B4F96590D46B311CFAB8DCAA43BFE7BA1C68701\
0000000000000017A914BDDCEDE9F888DEC90D0D923DF5F340BDE25A30148701000000000000001\
7A9143E9E77B92E9113A013F6BE02A8F2F27D3153843F87010000000000000017A9140275EACFE6\
32F4311E0A284AD4DD0091458F042E87010000000000000017A9144EB0089AE2B743776015DFA24\
8B9FEB1B3A1D51487010000000000000017A9140242CDBEEDC48FD079C32C4AD26EF09D18A9F5AC\
87010000000000000017A914DB19C183BF585D8D588E013017B4BFBC9566949C870100000000000\
00017A914FBF38B013DD3DC609B6D6ECC8BEA031F9243D71787010000000000000017A914D4653E\
F249958A9CE4225FD735BDDAE144CDAB9787010000000000000017A9147F04C9F24767A54EF0B58\
4EC23143C3DE52BAE1687010000000000000017A9141582D6932E1423BB2D6648738B9A725ED2D1\
A83D87010000000000000017A914C31C681A176D4385526A1DDB3FDBBC93F1B963C787010000000\
000000017A91454C13C1BF042E090D7954BD82DFA9CE0905831F487010000000000000017A914CE\
42B16D1A8D2300C78E200CAE9CC0D9CAB42BC787010000000000000017A91400F3D3DA97E023B49\
A83634DE4DA2E88EAD78B6787010000000000000017A914730B42FFCB74400C537C1F9DDE3272DE\
2ADDFEBF87010000000000000017A9141DD1CED16C2CF6EE5A1B4B573DA5C64B9F895B558701000\
0000000000017A914697808B033DAC9D4D3FC9A4696E58F2DF763E38287010000000000000017A9\
1485B894333C552F2BD9D137C80285F4E6BDC4A17287010000000000000017A914077C2A4908D0E\
4AB8F11D871D8925CF48934DAF187010000000000000017A914651DEFC112F1F50AAE9B57AB78BD\
97697D117A5587010000000000000017A9142FF8FAD2A6C0454F1E3A344F3A4EFB6E631895FF870\
10000000000000017A914A7B721BAB3320E92F42BCEAEF9D287DDF9AF9FA8870100000000000000\
17A914AD87D424D004CA2D8AD34C6E5220A5A8EC86E49187010000000000000017A91409B134C3E\
A12CD59E158FA9C3A6DE7458ED4025887010000000000000017A9147374595D979D90397DFEEDD2\
336E706B008A897287010000000000000017A9143EB72C2D69003036D4652470EC9BE19CA32B1E3\
B87010000000000000017A9144C7953760586A976FA8AA52F9A176C67BC7D2AFA87010000000000\
000017A9141B2D66C046258B72DF23A4E72E2A49F14EC1CA2387010000000000000017A914E30A6\
555896AE7F22E4567BD4E99313D70CED82987A0860100000000001976A914BFD9C318852CA57A56\
3786E67BB4D0A20B1D8F6788AC8F546900000000001976A914C43008A79FF433E7D126D3E9FA517\
B8FBC61F6D888AC00000000";

	testOneCryptoTransaction(hexStr6);

	const std::string hexStr7 = "\
01000000025EEA641A211620EABEA124D01661A467325B5497CCFFA017E6EEFD8B178B670F00000\
0006B483045022100DAF6E31624F2E27BED32E7A3524A21AEC9631C9D74A37EF4462BC4F2995A8D\
6C02204A73741EFC8D9093A403435B1E6411F5ADD80BFA1F72D9C87E7D7DF99A0063FF0121027B5\
4E1FBE67786DC3D19868EC7B1EB0C7D26F16EAFC1C12402520FFA636304FAFEFFFFFFDFDD448720\
9F31406336A32ADCDD622E8D217572442BE2CB3C056333F6CAEF19010000006A4730440220044C7\
6696461F1DC09E516F4A05AAC5FF3C9CB709D9E3B37E342704C7EA9AE4D02206C1BBE7BAFD987E8\
64E1DFC3C631E7D0E58F9D1310162F0ECDD37278182FA494012103966535450AA7E21B0770E9549\
3B8BFAD19F1D74E1C169798A40B7553DDFC1352FEFFFFFF023C5408000000000017A9140A4DFE51\
5ABD59B287E1E9ABA88A75A394E3488087B05C1100000000001976A914C57DC994D45A0F37F0E76\
74C56A5ACC05327DE3788AC83EF0600";

	calcTransactionTotOut(hexStr7, "1683692");



	const std::string hexStr8 = "\
01000000010000000000000000000000000000000000000000000000000000000000000000FFFFF\
FFF290397EF060004DDBFB0580429F0602408BA0492580004017E102F426974667572792F534547\
5749542FFFFFFFFF02BC94A74D000000001976A914AB0FCC2FB04EE80D29A00A80140B16323BED3\
D6E88AC0000000000000000266A24AA21A9ED9EFBA10530E625721F07899CF05BE9AFDB18132323\
A6EDD459A57C7C7ADEB3E000000000";

	calcTransactionTotOut(hexStr8, "1302828220");


	const std::string hexStr9 = "\
01000000010000000000000000000000000000000000000000000000000000000000000000FFFFF\
FFF0804FFFF001D021E01FFFFFFFF0100F2052A010000004341046C734EE6851FE4929B312386B5\
A772991E50BAFAC7837ABEC5D2C19444C8AFBEC3828688152B9ADC12E785CF21EE30BB3F8FC2223\
15A3F70B78F179D7F2DB572AC00000000";
	calcTransactionTotOut(hexStr9, "5000000000");


}



void testOneScript(const std::string& thisTestScript, const std::string& thisScriptString, const std::string& thisScriptType)
{
	std::cout << "Testing script" << std::endl;
	
	
	const BinaryScript script = BinaryScript(convertFromHex(thisTestScript));

	const Script myScript(Script::parse(script));
	const std::string scriptString = myScript.toString();
	
	std::cout << "Script: \"" << scriptString << "\"" << std::endl;
	
	if(scriptString != thisScriptString)
	{
		throw std::string("Script error");
	}
	
	const std::string scriptType = myScript.scriptTypeString();
	std::cout << "Script type: " << scriptType << std::endl;
	if(scriptType != thisScriptType)
	{
		std::cout << "Got type: " << scriptType << std::endl;		
		std::cout << "Should be: " << thisScriptType << std::endl;		
		throw std::string("Script type error");
	}
	
}




void testNumInputs(const std::string& transactionHex, const int numInputs)
{
	const std::string transaction = convertFromHex(transactionHex);
		
	std::list<SigPos> inputScriptPositions;
	const BinaryTransaction binaryTransaction(transaction);	
	const CryptoTransaction trans = CryptoTransaction::parseBinaryTransaction(binaryTransaction, inputScriptPositions);

	const TransAndInScriptPos transWithPos(binaryTransaction, inputScriptPositions);

	Amount totalIn;

	TransactionSet transSet;
	const std::list<TxHash> hashes = trans.getInputHashes();
	if(hashes.size() != numInputs)
	{
		throw std::string("Incorrect number hashes");
	}
	
	//const std::string transactionHex = "0100000001B9BF71C83545B79F445250089E1399BC3AE70DC4A2C6AAD10638D84ADAC06E07000000006A47304402204A79C665AC633F2867419860C18F98D1B24E9BC773C73E39469487C06B59184F02205639217D989367D8CAC50C0A0C1AC46C7674E48BCB1AB689A0439FB848C0ABD901210394D674B8C29FE57A1C2E2D4E2B0F6B3C0C93186F19DA387E7AE1CFD74CCC6ACFFFFFFFFF0100697626090000001976A9146CF76CC851B3F79531E6CFD55668A85509BBEE7A88AC00000000";

}




void testScript()
{
	const std::string testScript1 = "41046C734EE6851FE4929B312386B5A772991E50BAFAC7837ABEC5D2C19444C8AFBEC3828688152B9ADC12E785CF21EE30BB3F8FC222315A3F70B78F179D7F2DB572AC";
	const std::string scriptString1 = "DATA(046C734EE6851FE4929B312386B5A772991E50BAFAC7837ABEC5D2C19444C8AFBEC3828688152B9ADC12E785CF21EE30BB3F8FC222315A3F70B78F179D7F2DB572) OP_CHECKSIG ";
	testOneScript(testScript1, scriptString1, "P2PK output");
	
	const std::string testScript2 = "00473044022068103C2C80B1344E8C6472340BC51953CCB701170907791E0DDC8877A92B6C6E0220340BABB5CB17BD01F1344B568CD32B067D29D1BD02BB7BF1A0C5C17699DF6C9001473044022065552A86BA01CCF66393A1D5E3CF3D3336041179ED94DCEDC94D1F12D48F35CC02203A48B26BE4E3AEAEC00A48E7AB39ADDE25766B08CED08E819A36E9DE6F1F6E1F014C69522103DBBC7B49EA024A6A4047548ACFFE5CBA459396BD63E2FB4FB6B75EEA9B0ED4502103DB2C408C68EEB3A76D8BA5F21675319AA476414ADF2288A95F1397962D17673B2103EA685157EB8B90ED7FF4C31EA461F64D5DB77B3B9A485B7A765F37F75982083B53AE";
	const std::string scriptString2 = "OP_0 DATA(3044022068103C2C80B1344E8C6472340BC51953CCB701170907791E0DDC8877A92B6C6E0220340BABB5CB17BD01F1344B568CD32B067D29D1BD02BB7BF1A0C5C17699DF6C9001) DATA(3044022065552A86BA01CCF66393A1D5E3CF3D3336041179ED94DCEDC94D1F12D48F35CC02203A48B26BE4E3AEAEC00A48E7AB39ADDE25766B08CED08E819A36E9DE6F1F6E1F01) DATA(522103DBBC7B49EA024A6A4047548ACFFE5CBA459396BD63E2FB4FB6B75EEA9B0ED4502103DB2C408C68EEB3A76D8BA5F21675319AA476414ADF2288A95F1397962D17673B2103EA685157EB8B90ED7FF4C31EA461F64D5DB77B3B9A485B7A765F37F75982083B53AE) ";
	testOneScript(testScript2, scriptString2, "P2SH input signed");
	
	const std::string testScript3 = "A91441FD755E887052841A1884E3762E16052033586F87";
	const std::string scriptString3 = "OP_HASH160 DATA(41FD755E887052841A1884E3762E16052033586F) OP_EQUAL ";
	testOneScript(testScript3, scriptString3, "P2SH output");
	
	const std::string testScript4 = "00473044022021ECFD1AA59D5E0DAFD66FA7A8A723A436FE8328454F307A78CD3777B4704ACA022028D12A893E4C5B8539E2CC4317B110047AD9DE83A8FA96BF872D7416C9984A5901483045022100E2658409445A737E2B7D3C48A390ED9A23D1A760608DA863C6281D471D08F6C802204F7067C0723736949C9177BE584DD73D405A5E7A1C3BD18354A0F0DE2893277D014752210362119905CA9EB739F90EFBB5392E386F676BBACC8E28720C7064AAEC345B7B3421037762901989038E6372A8566F03AEE82F4F6CCE3047959D3279BE1E4680C477C052AE";
	const std::string scriptString4 = "OP_0 DATA(3044022021ECFD1AA59D5E0DAFD66FA7A8A723A436FE8328454F307A78CD3777B4704ACA022028D12A893E4C5B8539E2CC4317B110047AD9DE83A8FA96BF872D7416C9984A5901) DATA(3045022100E2658409445A737E2B7D3C48A390ED9A23D1A760608DA863C6281D471D08F6C802204F7067C0723736949C9177BE584DD73D405A5E7A1C3BD18354A0F0DE2893277D01) DATA(52210362119905CA9EB739F90EFBB5392E386F676BBACC8E28720C7064AAEC345B7B3421037762901989038E6372A8566F03AEE82F4F6CCE3047959D3279BE1E4680C477C052AE) ";
	testOneScript(testScript4, scriptString4, "P2SH input signed");

	const std::string testScript5 = "76A9147BF76B446DC6A0A1C73677C618FA75BB7289DDB788AC";
	const std::string scriptString5 = "OP_DUP OP_HASH160 DATA(7BF76B446DC6A0A1C73677C618FA75BB7289DDB7) OP_EQUALVERIFY OP_CHECKSIG ";
	testOneScript(testScript5, scriptString5, "P2PKH output");

	const std::string testScript6 = "A914E7F4B800F70C4F77751803D82309F479118E3BC287";
	const std::string scriptString6 = "OP_HASH160 DATA(E7F4B800F70C4F77751803D82309F479118E3BC2) OP_EQUAL ";
	testOneScript(testScript6, scriptString6, "P2SH output");

	
	const std::string transactionHex1 = "0100000001B9BF71C83545B79F445250089E1399BC3AE70DC4A2C6AAD10638D84ADAC06E07000000006A47304402204A79C665AC633F2867419860C18F98D1B24E9BC773C73E39469487C06B59184F02205639217D989367D8CAC50C0A0C1AC46C7674E48BCB1AB689A0439FB848C0ABD901210394D674B8C29FE57A1C2E2D4E2B0F6B3C0C93186F19DA387E7AE1CFD74CCC6ACFFFFFFFFF0100697626090000001976A9146CF76CC851B3F79531E6CFD55668A85509BBEE7A88AC00000000";
	testNumInputs(transactionHex1, 1);

	const std::string transactionHex2 = "\
010000000D841FC96D232C953BD0E6C94E74330E7925A1CC40726903192A904BF822702D0700000\
0008B483045022100E13B20979AE77BD8C5C5C51E73A9646D5E51AE5B6439215C7719B137C7CB06\
BF02203E18EBF4D60731015C6A320E7784C28B4E589A5BD62B8A304A18EDF52605FBA90141048F8\
5FFC306847AECCA51278A96C8A70C497CF7A9EBFA61F8BB4F325A3C452ADA100DA9B537217BC23A\
B6A54FF3B8A264482A3F81E3C99B9B2BA066798B541610FFFFFFFF58BBC932DEBC485682F365B05\
93C3A6B437C13ACD4F178DAEC324F24A250A515000000008B483045022100F8F1A1F4286B3534E3\
59962C196E7254622DDFF47CA33EDBCCCA311C2A60343702205F7DE471A0FDC1825BAA648ACD74A\
628CA043C9001A3851D1C9AEC7DD03A40DE014104A4A5C6F1BAFFDD80EA61AE6DFCB7FDAD14B0BE\
FFF8BB7803157E34DBC1FFDF47C3DDEA9FEFCDD6ECBBD3EE124044FFF2C3AE719F26A30C08E011A\
DB6FAD30143FFFFFFFF37941D22BE69566BCCC8BB78381D545A10204772CB759B389B7FCC8D487D\
5B1A000000008A473044022021955C137629A25C8272D2828B741C9AE01D451FF680372B5B35489\
A0BC9702902205F5F4AD1D476D0A1380CDC4BEEFC71C3C075F55CDB7D736197560D5170C56C9701\
41045D4979B0C7A3B44F0571B6EC6F7A9BA8F9224B88910CE17FB7F289272CFB27FEF03FA7EC59D\
8B2E956A43F777A5E33827C1B24347BBEB383ED75EF73362D555DFFFFFFFF2C04F29CBE90FD0664\
2C2D6BD90C60C8567BDB29BDCF5D996DC1480ACDC97A32010000008A47304402205ED545C1DAB86\
BD6F30A803AA502D9A32B7DEB3FF1439F9661D5B293C120F81D0220636302B06F261F6ABA14C0DB\
9959DD23DFB6C3B9599B621618F8DD7C9331AA77014104586E383D1F4B8E91603A3EE27C2D3980F\
E603997B8501139690515961AE4CAC54DD40156BFBCFA416D2E8AB6849753EF56A26D9180CF8FEC\
7E4F08898955BB6FFFFFFFFF92625AC74E6601AB1E1518258F5846CE06577CB7F1AA7CC42E27873\
FC64B3B41000000008B483045022100D9390E15A2F4941261CCB2E595C3473CF17805F39223C739\
084A6D45E8B39770022070C75980E1F696AA7F079495F29C06976D654454DF50B9DDEAA35F65BD1\
69FC3014104F8F917F0F97FB71F99647CEE5886D7351030C438EE5040017B939585D0CB13A57D5F\
60AA8FA64825850C39564F56B44F2D38B6B20C7D45B971398B4EFA528D7AFFFFFFFF5E63C6B79A5\
B98FE39A4EC16D7E5B4FEE5E2CCCA8A411FBBC3799E438C9E4070010000008B483045022100EE9B\
1EC5F1F7D632FDA66E29BE70086734B64F9D3EDF5B4DBA5363124440176D02203E30764704520A2\
E18C581217726AB356DE1087FB99F57DD4736241C8B16F538014104E4E0CC93E47B64ABCEC26D65\
0BECF06CB3FC016323DB917B7F24031F82784871A195FC98749AB47D8FD0D571BB5A5BE46EFC179\
020B026C1BE88C869B716DA1DFFFFFFFFCA0A6C8B77409DC867AF4B03C90238A9F25E5DAB1818A5\
B784CCF8174364CA78000000008A47304402202DDB1C1FE8AAA677281FABD51077CA7AF9843B65B\
5982AEC12117360E4C160C502203948C8E43F799B7567B735E5C62DC069E53E629B5B25E3D541CC\
A9480439A3E4014104BBE52ED8B29BAFF8441B95B308C49A1A91B91AB0F4E843A5BC9BF2E4AE231\
7363F8C1E1621A649F5D6D1CB7D7098438BB17016AD877438F357BFCB078264419FFFFFFFFFF622\
895C493FF42A65C3220931F8EA8BC91B8741528AF475F21E0168B235EC80000000008A473044022\
052F9C83B72001041102E299C9B22D1AB8C42725330C8BD25954A0FBAF710E19002202DB5199464\
08430D3C118F6B8BEA5CFB43537933536E4150AEC344F33BDA51E8014104264FEEB3C6A73F5078C\
2A848F3A95E80403DEBBF06B2F968DA915EABA674B0A91C42944C0AEA7F54CCCB71AD7304E14641\
E88E0E3BC0362CE7D05117846AC707FFFFFFFF1637995F5E6A0E48077585AD8EF87E1BA4AB6708F\
49E54ADB887C10328B5F28B000000008A473044022064B2B22C521539A3CAE6A80B884C407D9BBC\
D888A99DA9E9B354BE70925C301702200B8F7B57C79E709B175124C4896692E161B518A87E1950D\
2F78BFDAEF4583F4B01410494A168C6DA1EC199CD05594A1401BAC939F0C3B5C642F075247BC24D\
3809F4DCDE1734B76C0347DE937B5A79FEFB487A64A2A007C2644EEA66BB5363AFF1B3E6FFFFFFF\
F3B196D09FF770224A88595BDE72BA71B66EECB9975544390A62496866FDC13D1010000008B4830\
450221008E4A4AE41D4706EC3B675DFDA2A6AEB3D17A024BF09B7BED52F38C2FC2C61BFA022051D\
7AF4C0FD0680E3FB18D2E493EF92341B9C87A98A353ACC36861374BAC87020141048BA4CA846D84\
DFCD412AFD6FD10A3338181E17D9D0F8DFE28DDB00797C3305C2E795CF9FF9FDFE41451CE86401E\
BE652F3F56ECB2A9D669C866D0138F316AA9EFFFFFFFF1684B027C919E8B8DF20B7979823FAB624\
5666AEC69B8B33B880DCE31096FBDD000000008A47304402201B52AFD537A8CB096A73BA2D12D4F\
4FA8E2A0A610C272A839268B955FA548D85022046749AF470E73ED3F5A8342DB236C01DFF9C70BF\
C1C907A01BB54D007865A935014104D7B0CC727C58D7ED3847AE6C5F44CF6D8915FCE767DE89E02\
28F2FC09197BF21571B04F356AF87C0BF8E74EFB97160C355350F5630B475FDCD4E5D9419A0B37C\
FFFFFFFFA706DA0BBD95E78802F3432EBD1F27B75CA6AA7FA3D9AE215C612DCF7AD696EB0000000\
08A47304402205960FB98F5EB692DE024B3F7081A27B42740E3BB70AAAE07F428D98E4D7BA3E102\
202A7EF924FCF7D31948346E0EB1B08C7469DB243D441EE733A42DF507569960F10141043E4C9BD\
CAF2A460E01B1157EF8E9F40442A841119642850E576CCE05DB02DD84AD303A55338A1455945BE1\
E725AC8E00683BAD5850A3E8F7E70C5ABE277D506FFFFFFFFF2B85309CE5136A1782436EB4890C1\
708D99F9B1EDA249C74F536F9AE060649F6000000008B483045022100A7F87B54F7A3B7310E7811\
B805F87D50045E453010373EF1729430D9486A2E9D02206C87A8B2B18F39D393D243203A22BD3B7\
F1AEDB7BDC2823404909DD76E877FE0014104AC6B3699DD37CA5E7CC3D62EB43B76298379223D13\
5FA087159D7EDB2A91F90BC36B63595F1ADD8A0F3FDE04C3FF5043236769FF90AAD397F6B914661\
E1F052FFFFFFFFF02583E0F00000000001976A91425B4E99D14BC52151017DC3D2A770197008532\
F488AC00AB9041000000001976A914D9E866E7E16DFB482DB66475083156FB37D2784D88AC00000\
000";

	testNumInputs(transactionHex2, 13);



	const std::string transactionHex3 = "\
010000000F258521FF1E1ED4A3F1F82D3E257136890C335F0F7C77D0AC425EACA70196124600000\
0006A47304402202989CE07160B3C950D54AA3676F79FF6D17C3DA4D0BFFE9AA6F98491975BA32E\
02203D80413F82ACB576A6ADA07E2E8C5683B0DC74D49E6F89DD767478C96045ED56012103EC862\
EF96E9F052FB051E873CB1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFF020E52ACCD7F\
9B7647B24028F0B0E7EC6CB4D2B78B7945FBD4A7110B4EB58048000000006B4830450221008203D\
22686CFAF32DDE228425FA505A0246BF94A53B979F54EB5FC1DC6CC68F7022060D2433733D5C73C\
42900891CAD3CDD22C84BA2A388A457A86999BACD70A15F5012103EC862EF96E9F052FB051E873C\
B1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFFAF13CFF737F7397D0DB6CB37A670429C\
CF46CCB9B9CD48D6519AC36D223B6649000000006A4730440220066A5C0F3CF19AB5AB3D6294112\
531FA2558F1AB91A1DD7D0FDC938355F724ED02203B2740007020E7787197F7515061C52347CD0D\
FB9A0DB76D1A6E5384F31752B3012103EC862EF96E9F052FB051E873CB1B3CB5971F274FA319D43\
D6D1DACAAB2A689D5FFFFFFFF230D4863357294292B19EEDAF7F762D17CCA14DF58DA8301580B48\
7E2B90B850000000006B483045022100CF9B7712D207387F16DE0132A3D18AC1B0B7938D49AF9BF\
B76238512DDE73CB802200096B03DA4845A1C485B12FA1FD8B251420C73600784FBF8E8C62E3317\
D6BDCF012103EC862EF96E9F052FB051E873CB1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFF\
FFFFF83A0B21FAD27DE0FA1B717F70A5CE5E913FF2DB968B1D219D24146560D554459000000006B\
483045022100A4C452A29AA12F8C2562FC45A470331BA47D582B0AD46BFC04AF39CD6BD80CC7022\
03FFF656A1910892C05646EBC0ACED3C41572588C59CB5DD42C8828BF8943E2ED012103EC862EF9\
6E9F052FB051E873CB1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFF37F5ADE640DA025\
79176ADC0987CDCD93D7F8F9AA696F4E6214E2C94F49D495F000000006B483045022100897B6A7F\
43D45230E1257D02F157E376B06CF8B2237ADE35FB56F91DF8478EDF02202B0CE65BDA5DC7AEE96\
5620EB8C2970605AB7A28D944C69CA73D4B6D0745DFBF012103EC862EF96E9F052FB051E873CB1B\
3CB5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFF23FB927855C4B56A8362CD3CE2448B490AE\
6515BC32AEB22B647381CB0D53688000000006B483045022100D36CB30C6C0A5F3C21F5E726860D\
923CA85FB947835465DF282FE1A9E9790F2B02204CA5FD7E607D486880458B3B42060E6FA36E25F\
5265D063DEDA3CA251D62CD59012103EC862EF96E9F052FB051E873CB1B3CB5971F274FA319D43D\
6D1DACAAB2A689D5FFFFFFFF155C2DDF365E1687BC65757ADCFE6BD8F39727A4CAD1A6ECB56FF75\
86DDFD697000000006B483045022100A871F6DE1F69423EF4DE96DCC406582FC6C7A6CC8B0F8B7D\
ED917A767073034902202CA9A08EA3ECE89679E9E1F02DD8C93AAD790C7031D717BAE956D8CA7F4\
43831012103EC862EF96E9F052FB051E873CB1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFFF\
FFFFD3E8FA9128C22EC2EA6DBCBDD4438CEC7826A803771EF097102B61A0CC21DC9E000000006A4\
7304402207BAF556A4F90AF12FE3398B6DA80D7B37369127211F00C284F31C36C607EEB4902203D\
AEA7DFF4D847C2E55CBFBBEFD18B65DFB807FA2056853B34EEA04086D0A3A6012103EC862EF96E9\
F052FB051E873CB1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFFA06D5687F03ACBCDE3\
471D93D23562E83DEBC2388AD4E375529181975A8D13AB000000006B483045022100A799B494C8B\
E47F1F1B482E454444058007C8F463199260409F1864CB247941002205AC344BED9E1BF86410EA0\
0AD1469AFF7A11ECE5EB364863C83AC24F2177C292012103EC862EF96E9F052FB051E873CB1B3CB\
5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFF81616D8D75791046224A5CFBAFF2FA1F45D7A4\
60673FE609F05D2E7CCA675BC9000000006B483045022100F493F73DB255FC741C2C6E8B0453A8A\
3916E15C73434924F049559812123EAC6022037D296ED6CD60C99FCE4BE08CABC3160E9B91E6157\
9516DAE6A96F28BDDFA2E5012103EC862EF96E9F052FB051E873CB1B3CB5971F274FA319D43D6D1\
DACAAB2A689D5FFFFFFFF428851EF82828D486E77E593A1C3724627569D5B447330B5D8478CD80C\
E1C1CB000000006A473044022001DBEFE7B286ABFAFB839E8020EE2C237481231C8C71BBF77F5D8\
42871BC225B02206BC61AD87308D2FD85BA0D3231573F372D9190307D2C7B5C2B59C556E31FA9EC\
012103EC862EF96E9F052FB051E873CB1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFFD\
A3E4160EBA0F7EEDDA92B7A13FD41464ECD18DDC36408132AC0BA5927492CCD000000006B483045\
022100EF1E6FB05B21C0F4703C283F03FD1DE4E70399EFFD3200DE7E72158292F251FD022050074\
8CFF6376B9F34F9BFBEC835F90733CFAE017EFF059C5BEFBA70AF2BA044012103EC862EF96E9F05\
2FB051E873CB1B3CB5971F274FA319D43D6D1DACAAB2A689D5FFFFFFFFA0DC77996B7E0CE2E4310\
2930E5787140326849C68E4962802F9EDCFAE4E98D2000000006A47304402206B6956984437D8C8\
0AE79C9BBBAB6CD6B86CB00F6578D92C81C552125851EF8802203E225E4A97B41994EA988954C64\
F9B4964103F504EB0E509510138995BEC9C9101210330780EF5609437145EE3BDE7229A22300C5A\
8867F0B223A046F052B66CB7188EFFFFFFFFA0DC77996B7E0CE2E43102930E5787140326849C68E\
4962802F9EDCFAE4E98D2020000006A4730440220177574DE960C85884AE606A64864914DA1552C\
125AD66B1FF81C6CABE7EA60800220536DDCF37C7AB339613874FCA4F3AE62A321869A5EA79B6ED\
4B9E26273406C84012103F09B0D926E504EE15C0004787246048AFDD20E3E0266A04CEABBCC59EB\
B07296FFFFFFFF01A107B52C000000001976A9140DAA7C035FECF4512064A0E0FA24FF34E44E046\
588AC00000000";

	testNumInputs(transactionHex3, 15);
	
}



void testMultisigAddress(const std::string& scriptHex, const std::string& addressStr)
{
	const BinaryScript script1 = BinaryScript(convertFromHex(scriptHex));
	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(script1);
	std::cout << "Address: " << pubAddress.toString() << std::endl;
	
	if(pubAddress.toString() != addressStr)
	{
		throw std::string("Incorrect multisig address");
	}
	
}




void testMultisigTransaction(const int numReq, std::list<Pkif>& pkifList, const std::string& resultHex)
{
	//const int numReq = 2;
	std::list<PubKey> pubkeys;
	
	for(std::list<Pkif>::const_iterator it = pkifList.begin() ;
		it !=  pkifList.end() ;
		++it)
	{
		const Pkif& pkif = (*it);
		const PubKey pubKey = pkif.getPubKey();	
		pubkeys.push_back(pubKey);
	}
	
	
	const BinaryScript script = ScriptCreator::createMultiSigScript(numReq, pubkeys);	
	std::cout << "Script: " << hexString(script.raw()) << std::endl;
	
	if(hexString(script.raw()) != resultHex)
	{
		throw std::string("Multisigt script error");
	}
}


void testSeveralMultisigTransactions()
{
	std::list<Pkif> pubkeys;
	const Pkif pkifA("7XXcgwbE72BmbrtoDS4vgDp6UAvWjMMTGAL3ZiDV5BcfrRY86Z2");
	pubkeys.push_back(pkifA);
	
	const Pkif pkifB("7WgX5fryy9sHieUxnvuHACEuLsBbXiRq2bACnnCMibUWuRnM15Z");
	pubkeys.push_back(pkifB);
		
	testMultisigTransaction(2, pubkeys, "522102BAA3CE0023DB3AB9F114670CA8AC8F9E30531DE4AEDE50C57B3E2634C79CB41C21024B28E9C7ED29D1883BECCDC4331CAC24A7EA53ADC2D274FD5293F6DD7DEBA32A52AE");
	

	std::list<Pkif> pubkeys2;

	const Pkif pkif1("7ZN7SngWpLQ2jycRh1oab4MMQaAwt2u6Xj1gBLsuRbRc1VeoziN");
	pubkeys2.push_back(pkif1);

	const Pkif pkif2("7ZiZ4UfMrTbYEqLp87iZ8wL4HfG2EvRF7CXiGufPtn2FEhDTo2z");
	pubkeys2.push_back(pkif2);
	
	const Pkif pkif3("7YBvzb7udnEdgoKkcXkq8T4rkCfsPe6MksLqFEZ8xsqx8AAvCnt");
	pubkeys2.push_back(pkif3);
	
	const Pkif pkif4("7XDLVBYh5YtFdpjJ61RzPhawAErhgyJMYnc32cY9xmQo5aCRzw2");
	pubkeys2.push_back(pkif4);
	
	const Pkif pkif5("7WvPvqtdR8v4k4psoUPE5bhS4tchAdphNbTK6Vnv2sJ9gozMYnN");
	pubkeys2.push_back(pkif5);

	const Pkif pkif6("7XZ3SJK2FQACVCG9njDQDQJAMbjbiNxWceiZRH1qRrV1hbgYf2A");
	pubkeys2.push_back(pkif6);

	const Pkif pkif7("7X4m3UghCfqLoqWfoMHoZgez2EQXCshAe8HBdMoB362dpkDbk6d");
	pubkeys2.push_back(pkif7);

	const Pkif pkif8("7WZ3PYrkasBxhN6jysxPHJ3aaPTWx3MDmRZXSyGqWji1az74Uov");
	pubkeys2.push_back(pkif8);

	const Pkif pkif9("7XR83r5wxsiimUsKUB6gXeTJKuRdbGPU1R3zpG8juLEjsmxqndv");
	pubkeys2.push_back(pkif9);

	const Pkif pkif10("7WzTWCZriR3ErKB9j2VEnHLRdHamujcCKBpg3Z6peVR8gAemrs4");
	pubkeys2.push_back(pkif10);

	const Pkif pkif11("7Zm9yFKreYhPmQ9JbCTwGjb3cfkUFuk6ngSGb8RT68ZXZsBsYbB");
	pubkeys2.push_back(pkif11);

	const Pkif pkif12("7YtjT2bShU642FWu9dWPSNjBC5HHjDUkXTPR3g9y6xNkj9rdcjj");
	pubkeys2.push_back(pkif12);

	const Pkif pkif13("7WmqWtLWJWoVQMQZVKdEGZojUUwaPPpcU8q244qi4QYX8djJ9HJ");
	pubkeys2.push_back(pkif13);

	const Pkif pkif14("7ZBrxMxkAu2MFXChc8Dh4qdNdunZWFrQormEaiFPpQkxoANKiRZ");
	pubkeys2.push_back(pkif14);

	const Pkif pkif15("7WE3JzSSqVWZTPti8p9mkGaLpBcS29ut6zjPD9eEsNaeFnbVwub");
	pubkeys2.push_back(pkif15);

	const Pkif pkif16("7Y7nFepfwsGbFTWVE3jHbsV5Kvwmt9fU5qiCHPRfDuTn4CiT9a4");
	pubkeys2.push_back(pkif16);

	
	testMultisigTransaction(16, pubkeys2, "602103AC739B37BACE9B07AF72B6E04936847940673C720CF63CA048A81AB78FA5CA672103DADDE9370DB1C33729D3404F1036B515BE79455467BB283A2CCC7C0A5D68D0FF210311A3DD75F54F56F9129978FF6E71D23EF048CE86E24C2DBC5B43D72B59CCDD1D2102912264618D434BA9F762BB7D6D4EDFF7C13BDF3830549116561F095BEBD1A5A321026AAB24DBBD1FE4BEF14394DC55377ED9E8F27B6C6BCD6DA1A4504C509E3488172102BDE11AF215E41775CB353FD2844F95CF9149F0E390B49E4BC19EDE6E2C4A840D21027DA8FCF4136A7098283FC7E93709C31686B1298E9B29CFC62015AB678EAFC61121023A2E6E0827D06C8D13EB4A0B59C1EFC4DBA37F6E67883F30372786D18F1DFFED2102ABE519F592BE5326B2502666E603823B5153BF26911E3F615949CB5222A80300210273E4175A5F5712B3B3272DB326E28ED1F5DCA229F9A40C25E22E13ED8148E1ED2103E0C656829F554611C17C6EF664723BE11E862A02B349D9EB95363C9CAE79DE4A21036E489EF62441423A4CA2CA76A040C4FB3136BCD4D024C15BA19E9C85A973823F2102573C1057C3254D6806AA06385863E14B3D7AF49A0D2C5BA43D8AB8A367796AD82103952D97ABC5927558CE8EE32F92E8B80F02E1DA5B90A8A08881998C4A2E02E5C121020F0955C42AB705536E37AAB9D9DBBA6C317524196ABE80CDA505CBE800E02EF22103083726F936DF96C8B9A4ABE6705853A039FFC5316AB8F7F054632F6BA3876F8C60AE");
	
	
	std::list<Pkif> pubkeys3;
	const Pkif pkifFIrst("7XjtZ3i5tzQAMqvm9JXQNSK2B9YSrFTqhd1FmWdwaWDzmU44ap2");
	pubkeys3.push_back(pkifFIrst);
	
	testMultisigTransaction(1, pubkeys3, "512102D682253181C10F44110251D382AD8DEF3A1DB19BA7C6F30FFFD13B398E6C59BF51AE");



	
	testMultisigAddress("512102D682253181C10F44110251D382AD8DEF3A1DB19BA7C6F30FFFD13B398E6C59BF51AE", 
		"32D1iZ5vk2U9ETCWxBHizL413BSFWxjXZx");
	testMultisigAddress("522102BAA3CE0023DB3AB9F114670CA8AC8F9E30531DE4AEDE50C57B3E2634C79CB41C21024B28E9C7ED29D1883BECCDC4331CAC24A7EA53ADC2D274FD5293F6DD7DEBA32A52AE", 
		"3BdZbQAfBghkwZViPQvMNdG1ktyZjzKfMy");
	
	
}



void testOneMultisigSignature(const std::string& transactionStr, const std::string& wifStr, 
	const BigInt<256>& tempKey, const std::string& shouldBe)
{
	//const std::string transactionStr = "01000000019216109CA2156584714BA5CA5651D6A5F40DC757DF6E6B71A8F33224ECE0796200000000475121029BF38C24CA0A81C1DC1AD514FDA2C011ADEE5DDCF2B14409C497BADACD1032502102D75D81231F68AD2E9EA85C40EDE3F8C22ABA2A16407D52F808E691F7365F348652AEFFFFFFFF01009FDBEA080000001976A914C535F651CDA6992C42C46ADDFEC138E836DE22B688AC00000000";
	//const std::string wifStr = "5KKhSgwSSTLnwx93a5fB1YCUqG6HqDhyfLzWmNDHHSXvQhHK55x";
	//const BigInt<256> tempKey(64647374);
	
	
	const std::string transaction = convertFromHex(transactionStr);	
	const BinaryTransaction binaryTransaction(transaction);
	const Wif wif(wifStr);
	const PrivKey givenPriv = wif.getPriv();
		
	const RSVal rs = TransactionSigner::partlySignMultisigTransaction(binaryTransaction, givenPriv, tempKey);
	const std::string signature = Script::encodeSignature(rs);
	
	if(hexString(signature) != shouldBe)
	{
		std::cout << "Signature: " << hexString(signature) << std::endl;		
		throw std::string("Multisig signture error");
	}
	
}



void testMultisigSignature()
{
	const std::string transactionStr = "01000000019216109CA2156584714BA5CA5651D6A5F40DC757DF6E6B71A8F33224ECE0796200000000475121029BF38C24CA0A81C1DC1AD514FDA2C011ADEE5DDCF2B14409C497BADACD1032502102D75D81231F68AD2E9EA85C40EDE3F8C22ABA2A16407D52F808E691F7365F348652AEFFFFFFFF01009FDBEA080000001976A914C535F651CDA6992C42C46ADDFEC138E836DE22B688AC00000000";
	const std::string wifStr = "5KKhSgwSSTLnwx93a5fB1YCUqG6HqDhyfLzWmNDHHSXvQhHK55x";
	const BigInt<256> tempKey(64647374);
	const std::string shouldBe = "30440220062BA15515591CB3F20E6E253FAF3B15E47A3EC0B15DAF7DCD618F9BCE359F8402203F25D105D0C6BB08EC1CC315B55E46A5575030D99C85626D9BD327D39E01ACDF01";
	
	testOneMultisigSignature(transactionStr, wifStr, tempKey, shouldBe);
	
	
	const std::string transactionStr2 = "0100000001EBF7EAC9BC0C92533933699FE9664AA49C41ADCA9ACAD3ADC61C9F042B54632F0000000069522102488A582F7CCA153950976E954DEFE3DFE0A6D2BFB2A1A2A5EF4093BBE755BD6E21038580D9F7E52892DCC9EDCCFB1EBC0494B157793391F4B09A232E1374DF7807F02103CE846038A77F3D6644FC2298FBE79C1B1F65C720B4CA4D8FA8349C821D2A825A53AEFFFFFFFF0100DDEFDE080000001976A914D1A15DB8AC0626CFE881D3596C50364608DEE51088AC00000000";
	const std::string wifStr2 = "5JbvncmjnVozgntpeCmpGspe1N4RVTbppcCHwR3STotWv6aosd9";
	const BigInt<256> tempKey2(4324552);
	const std::string shouldBe2 = "3044022053D1F98D1999080D81484B7ACB297C9251B405AAF32BBB7C7E94F35C9000A6F2022052E499DFBE86FE678F362F29B8161FEB982779FC22B51455E63A0098E9375A8001";
	testOneMultisigSignature(transactionStr2, wifStr2, tempKey2, shouldBe2);

	const std::string transactionStr3 = "0100000001EBF7EAC9BC0C92533933699FE9664AA49C41ADCA9ACAD3ADC61C9F042B54632F0000000069522102488A582F7CCA153950976E954DEFE3DFE0A6D2BFB2A1A2A5EF4093BBE755BD6E21038580D9F7E52892DCC9EDCCFB1EBC0494B157793391F4B09A232E1374DF7807F02103CE846038A77F3D6644FC2298FBE79C1B1F65C720B4CA4D8FA8349C821D2A825A53AEFFFFFFFF0100DDEFDE080000001976A914D1A15DB8AC0626CFE881D3596C50364608DEE51088AC00000000";
	const std::string wifStr3 = "5JYzhyHXN1gRt9uakZvyHBnQ2dtFdJ2jHvoX2ZPx1iywuczeMwi";
	const BigInt<256> tempKey3(85885885);
	const std::string shouldBe3 = "3044022039C9680E7ED056D84BABB3080A3783F3FEE20D6F927CA05310C035512F90DDED02205138873BB0D547FF22D9BB6C34EB87877F77283AC9F96DE2D9433D9A3211AD9D01";
	testOneMultisigSignature(transactionStr3, wifStr3, tempKey3, shouldBe3);


}



void testTotSignMultisigSignature(const std::string& transactionStr, std::list<RSVal>& signatures,
	const std::string& transactionShouldBeHex)
{
	const std::string transaction = convertFromHex(transactionStr);
	const BinaryTransaction binaryTransaction(transaction);			
	const BinaryTransaction signedTransaction = TransactionSigner::insertSignaturesInMultisigTransaction(binaryTransaction, signatures);	
	if(hexString(signedTransaction.raw()) != transactionShouldBeHex)
	{
		std::cout << "SignedTrans: " << hexString(signedTransaction.raw()) << std::endl;
		throw std::string("Error for multisig tot sign");
	}
}


void testTotalSignOfMultisig()
{
	const std::string transactionStr = "0100000001EBF7EAC9BC0C92533933699FE9664AA49C41ADCA9ACAD3ADC61C9F042B54632F0000000069522102488A582F7CCA153950976E954DEFE3DFE0A6D2BFB2A1A2A5EF4093BBE755BD6E21038580D9F7E52892DCC9EDCCFB1EBC0494B157793391F4B09A232E1374DF7807F02103CE846038A77F3D6644FC2298FBE79C1B1F65C720B4CA4D8FA8349C821D2A825A53AEFFFFFFFF0100DDEFDE080000001976A914D1A15DB8AC0626CFE881D3596C50364608DEE51088AC00000000";
	std::list<RSVal> signatures;
	const std::string signature1 = convertFromHex("3045022100908A51FF6E41DDEF16CFD0883A6C9111A2C6B893B7B31D961A2EA6A6BF47E8FF022038433FCB6495038393E7838815D8DA91074F7985C0A681A779F417C9B50DD5C601");		
	ReadStream stream1(signature1);
	const RSVal rs1 = ScriptDecoder::decodeSignatureRS(stream1);
	signatures.push_back(rs1);

	const std::string signature2 = convertFromHex("3045022100DBD68483B9B6D281849EC62761140D9DEBDA33DA4E11D4319651E9A813CBEBBF02203E6A3E50B4392C784358852C6A1F0D88DE4304F8A4CF74642282F68E7B562CC201");		
	ReadStream stream2(signature2);
	const RSVal rs2 = ScriptDecoder::decodeSignatureRS(stream2);
	signatures.push_back(rs2);

	const std::string transactionShouldBeHex = "0100000001EBF7EAC9BC0C92533933699FE9664AA49C41ADCA9ACAD3ADC61C9F042B54632F00000000FDFE0000483045022100908A51FF6E41DDEF16CFD0883A6C9111A2C6B893B7B31D961A2EA6A6BF47E8FF022038433FCB6495038393E7838815D8DA91074F7985C0A681A779F417C9B50DD5C601483045022100DBD68483B9B6D281849EC62761140D9DEBDA33DA4E11D4319651E9A813CBEBBF02203E6A3E50B4392C784358852C6A1F0D88DE4304F8A4CF74642282F68E7B562CC2014C69522102488A582F7CCA153950976E954DEFE3DFE0A6D2BFB2A1A2A5EF4093BBE755BD6E21038580D9F7E52892DCC9EDCCFB1EBC0494B157793391F4B09A232E1374DF7807F02103CE846038A77F3D6644FC2298FBE79C1B1F65C720B4CA4D8FA8349C821D2A825A53AEFFFFFFFF0100DDEFDE080000001976A914D1A15DB8AC0626CFE881D3596C50364608DEE51088AC00000000";

	testTotSignMultisigSignature(transactionStr, signatures, transactionShouldBeHex);
	
	
	
	
	const std::string transactionStr2 = "0100000001EBF7EAC9BC0C92533933699FE9664AA49C41ADCA9ACAD3ADC61C9F042B54632F0000000069522102488A582F7CCA153950976E954DEFE3DFE0A6D2BFB2A1A2A5EF4093BBE755BD6E21038580D9F7E52892DCC9EDCCFB1EBC0494B157793391F4B09A232E1374DF7807F02103CE846038A77F3D6644FC2298FBE79C1B1F65C720B4CA4D8FA8349C821D2A825A53AEFFFFFFFF0100DDEFDE080000001976A914D1A15DB8AC0626CFE881D3596C50364608DEE51088AC00000000";
	std::list<RSVal> signatures2;
	const std::string signatureA1 = convertFromHex("3045022100908A51FF6E41DDEF16CFD0883A6C9111A2C6B893B7B31D961A2EA6A6BF47E8FF022038433FCB6495038393E7838815D8DA91074F7985C0A681A779F417C9B50DD5C601");		
	ReadStream streamA1(signatureA1);
	const RSVal rsA1 = ScriptDecoder::decodeSignatureRS(streamA1);
	signatures.push_back(rsA1);

	const std::string transactionShouldBeHex2 = "";

	try
	{
		testTotSignMultisigSignature(transactionStr2, signatures2, transactionShouldBeHex2);
		throw std::string("Multisig tot sign error");
	}
	catch(const std::string& err)
	{
		//OK, should fail
		std::cout << "OK, sign should fail" << std::endl;
	}
	
}



void testCreateEmptyTransaction()
{
	std::cout << "=============== Creating transaction with no inputs ===============" << std::endl;
	
	std::list<TxInput> inputs;
	std::list<TransactionOutput> recipients;
	int locktime(0);

	
	const TxData txData(inputs, recipients, locktime);
	//txData.printTransactionInformation();
	
	const std::list<BigInt<256> > emptyTempKeys;

	//const std::string
	const BinaryTransaction createdTransaction = txData.createSignedTransaction(emptyTempKeys);
	//std::cout << "createdTransaction: " << hexString(createdTransaction) << std::endl;
	
	if(createdTransaction.asHex() != "01000000000000000000")
	{
		throw std::string("Create transaction error");		
	}	
}





void createTransactionOneInput()
{
	std::cout << "=============== Creating transaction with one input ===============" << std::endl;
	std::list<TxInput> inputs;
	TxHash txHash("770e8efd35f6a0a83c0a6b800404f13bf869e2e58e9b18b077f140bc197b66cb");
	const int outputIndexNum(3);
	
	const BigInt<256> mypriv(1);
	PrivKey privkey(mypriv, false);	
	const BinaryScript script = ScriptCreator::createAnyoneCanSpendFundscript();
	//const BinaryScript binScript(script);
	const BigInt<128> prevAmountBigInt(123123123);
	const BigInt<64> sequence(0);
	
	const TransactionInput input(txHash, outputIndexNum, script, sequence);
	const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEY);
	
	inputs.push_back(txInput);
	
	
	std::list<TransactionOutput> recipients;
	int locktime(0);

	
	const TxData txData(inputs, recipients, locktime);
	txData.printTransactionInformation();
	
	const std::list<BigInt<256> > emptyTempKeys;

	const BinaryTransaction createdTransaction = txData.createSignedTransaction(emptyTempKeys);
	std::cout << "createdTransaction: " << createdTransaction.asHex() << std::endl;
	
	if(createdTransaction.asHex() != "0100000001CB667B19BC40F177B0189B8EE5E269F83BF10404806B0A3CA8A0F635FD8E0E770300000000FFFFFFFF0000000000")
	{
		throw std::string("Create transaction with one input error");		
	}	
}

void createTransactionOneInputOneOutput()
{
	std::cout << "=============== Creating transaction with one input one output ===============" << std::endl;
	std::list<TxInput> inputs;
	TxHash txHash("770e8efd35f6a0a83c0a6b800404f13bf869e2e58e9b18b077f140bc197b66cb");
	const int outputIndexNum(3);
	
	const BigInt<256> mypriv(1234);
	PrivKey privkey(mypriv, false);	
	const BinaryScript script = ScriptCreator::createAnyoneCanSpendFundscript();
	//const BinaryScript binScript(script);
	const BigInt<128> prevAmountBigInt(123123123);
	
	const BigInt<64> sequence(0);
	const TransactionInput input(txHash, outputIndexNum, script, sequence);	
	const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEY);
	
	inputs.push_back(txInput);
	
	
	std::list<TransactionOutput> recipients;
	
	
	const std::string decodedHash = convertFromHex("f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2");
	const std::string toStore = "DOCPROOF" + decodedHash;
	const BinaryScript outScript = ScriptCreator::createOpReturnScript(toStore);	
	//const BinaryScript binOutScript(outScript);
	const Amount zeroAmountVal;		
	const TransactionOutput scriptAndAmount(outScript, zeroAmountVal);
	recipients.push_back(scriptAndAmount);
	
	int locktime(0);

	
	const TxData txData(inputs, recipients, locktime);
	txData.printTransactionInformation();
	
	std::list<BigInt<256> > tempKeys;
	const BigInt<256> tempkey(983745987);
	tempKeys.push_back(tempkey);

	const BinaryTransaction createdTransaction = txData.createSignedTransaction(tempKeys);
	std::cout << "createdTransaction: " << createdTransaction.asHex() << std::endl;
	                                     
	if(createdTransaction.asHex() != "0100000001CB667B19BC40F177B0189B8EE5E269F83BF10404806B0A3CA8A0F635FD8E0E7703000000484730440220134D5827F5C1BDC7DA8920CF9033BBAADE2794F467F7964C351306B3DA606E850220572DD5A5BFDD01B33F2735FB14C75069BB11B2068CD9342CE6C240A425B75E9A01FFFFFFFF0100000000000000002A6A28444F4350524F4F46F2CA1BB6C7E907D06DAFE4687E579FCE76B37E4E93B7605022DA52E6CCC26FD200000000")
	{
		throw std::string("Create transaction with one input and one output error");		
	}	
}



void createTransactionTwoInputsTwoOutputs()
{
	std::cout << "=============== Creating transaction with two input two output ===============" << std::endl;
	std::list<TxInput> inputs;
	TxHash txHash("770e8efd35f6a03534534534534534534534e2e58e9b18b077f140bc197b66cb");
	const int outputIndexNum(3);
	
	const BigInt<256> mypriv(1234);
	PrivKey privkey(mypriv, false);	
	const BinaryScript script = ScriptCreator::createAnyoneCanSpendFundscript();
	//const BinaryScript binScript(script);
	const BigInt<128> prevAmountBigInt(123123123);
	const BigInt<64> sequence(0);

	const TransactionInput input(txHash, outputIndexNum, script, sequence);		
	const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEY);
	
	inputs.push_back(txInput);
	
	
	std::list<TransactionOutput> recipients;
	
	
	const std::string decodedHash = convertFromHex("f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2");
	const std::string toStore = "DOCPROOF" + decodedHash;
	const BinaryScript outScript = ScriptCreator::createOpReturnScript(toStore);
	//const BinaryScript binOutScript(outScript);
	const Amount zeroAmountVal;		
	const TransactionOutput scriptAndAmount(outScript, zeroAmountVal);
	recipients.push_back(scriptAndAmount);
	
	int locktime(0);

	
	const TxData txData(inputs, recipients, locktime);
	txData.printTransactionInformation();
	
	std::list<BigInt<256> > tempKeys;
	const BigInt<256> tempkey(983745987);
	tempKeys.push_back(tempkey);
	
	const BigInt<256> tempkey2(983753497);
	tempKeys.push_back(tempkey2);

	const BinaryTransaction createdTransaction = txData.createSignedTransaction(tempKeys);
	std::cout << "createdTransaction: " << createdTransaction.asHex() << std::endl;
	                                     
	if(createdTransaction.asHex() != "0100000001CB667B19BC40F177B0189B8EE5E23445533445533445533435A0F635FD8E0E7703000000484730440220134D5827F5C1BDC7DA8920CF9033BBAADE2794F467F7964C351306B3DA606E85022026D1E6B73F786969A24E1E477AECE2995F2A382C0CE28C23EBB5CE126A546A9D01FFFFFFFF0100000000000000002A6A28444F4350524F4F46F2CA1BB6C7E907D06DAFE4687E579FCE76B37E4E93B7605022DA52E6CCC26FD200000000")
	{
		throw std::string("Create transaction with two input and two output error");		
	}	
}

	


void createTransactionOneInputP2PKHOneOutput()
{
	std::cout << "=============== Creating transaction with one input one output ===============" << std::endl;
	std::list<TxInput> inputs;
	TxHash txHash("770e8ef827364287648237634872346bf869e2e58e9b18b077f140bc197b66cb");
	const int outputIndexNum(3);
	
	const BigInt<256> mypriv(666234);
	PrivKey privkey(mypriv, false);	
	const BigInt<128> prevAmountBigInt(123123123);
	
	const std::string scriptHashStr = ripemd160("xyz");
				
	const BinaryScript script = ScriptCreator::scriptPubkeyWithHash(scriptHashStr);
	//const BinaryScript binScript(script);
	const BigInt<64> sequence(0);

	const TransactionInput input(txHash, outputIndexNum, script, sequence);		
	const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEYHASH);
	
	inputs.push_back(txInput);
	
	
	std::list<TransactionOutput> recipients;
	
	
	const std::string decodedHash = convertFromHex("f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2");
	const std::string toStore = "DOCPROOF" + decodedHash;
	const BinaryScript outScript = ScriptCreator::createOpReturnScript(toStore);
	//const BinaryScript binOutScript(outScript);
	//const BinaryScript binScript(outScript);
	const Amount zeroAmountVal;		
	const TransactionOutput scriptAndAmount(outScript, zeroAmountVal);
	recipients.push_back(scriptAndAmount);
	
	int locktime(0);

	
	const TxData txData(inputs, recipients, locktime);
	txData.printTransactionInformation();
	
	std::list<BigInt<256> > tempKeys;
	const BigInt<256> tempkey(983745987);
	tempKeys.push_back(tempkey);

	const BinaryTransaction createdTransaction = txData.createSignedTransaction(tempKeys);
	std::cout << "createdTransaction: " << createdTransaction.asHex() << std::endl;
	                                     
	if(createdTransaction.asHex() != "0100000001CB667B19BC40F177B0189B8EE5E269F86B3472486337826487423627F88E0E77030000008A4730440220134D5827F5C1BDC7DA8920CF9033BBAADE2794F467F7964C351306B3DA606E85022024DE82BE948101E4236F0162332178400E10C411E7D759F88BD0958953B147F40141043AAB315C5BDD9915985C81913A53C4CB5E14C2DD45B17A1F7B3C332FAAABDAD63B33ABAF7A0791B3F6C647C6B369C12D4966D8036B866967F4BE4B67C26CC5E4FFFFFFFF0100000000000000002A6A28444F4350524F4F46F2CA1BB6C7E907D06DAFE4687E579FCE76B37E4E93B7605022DA52E6CCC26FD200000000")
	{
		throw std::string("Create transaction with one input P2PKH and one output error");		
	}	
}




void createTransactionOneInputP2PKOneOutput()
{
	std::cout << "=============== Creating transaction with one input one output ===============" << std::endl;
	std::list<TxInput> inputs;
	TxHash txHash("770e8ef827364287648237634872346bf869e2e58e9b18b077f140bc197b66cb");
	const int outputIndexNum(3);
	
	const BigInt<128> prevAmountBigInt(123123123);
	
	const Wif wif("KyVvmspJrJJpmMPkdMGpXgrEMWc3ipBVhhn84X2XffY87KoDP3jN");			
	const PrivKey privkey = wif.getPriv();

	const BitcoinEllipticCurve ec;
	const PubKey pubkey = privkey.getPubkey(ec);
				
			
	const BinaryScript script = ScriptCreator::createOldPubkeyFundscript(pubkey);
	//const BinaryScript binScript(script);
	const BigInt<64> sequence(0);

	const TransactionInput input(txHash, outputIndexNum, script, sequence);		
	const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEY);
	
	inputs.push_back(txInput);
	
	
	std::list<TransactionOutput> recipients;
	
	
	const std::string decodedHash = convertFromHex("f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2");
	const std::string toStore = "DOCPROOF" + decodedHash;
	const BinaryScript outScript = ScriptCreator::createOpReturnScript(toStore);
	//const BinaryScript binOutScript(outScript);
	const Amount zeroAmountVal;		
	const TransactionOutput scriptAndAmount(outScript, zeroAmountVal);
	recipients.push_back(scriptAndAmount);
	
	int locktime(0);

	
	const TxData txData(inputs, recipients, locktime);
	txData.printTransactionInformation();
	
	std::list<BigInt<256> > tempKeys;
	const BigInt<256> tempkey(983745987);
	tempKeys.push_back(tempkey);

	const BinaryTransaction createdTransaction = txData.createSignedTransaction(tempKeys);
	std::cout << "createdTransaction: " << createdTransaction.asHex() << std::endl;
	                                     
	if(createdTransaction.asHex() != "0100000001CB667B19BC40F177B0189B8EE5E269F86B3472486337826487423627F88E0E7703000000484730440220134D5827F5C1BDC7DA8920CF9033BBAADE2794F467F7964C351306B3DA606E8502203C5C533B9A3E00FAF668032C9B92D62749EF5137EC884447BD8065FC4C6CEE5D01FFFFFFFF0100000000000000002A6A28444F4350524F4F46F2CA1BB6C7E907D06DAFE4687E579FCE76B37E4E93B7605022DA52E6CCC26FD200000000")
	{
		throw std::string("Create transaction with one input P2PK and one output error");		
	}	
}


	
	
void createTransactionTwoInputsTwoOutputsTest2()
{
	std::cout << "=============== Creating transaction with two input two output ===============" << std::endl;
	std::list<TxInput> inputs;
	TxHash txHash("770e8efd35f6a03534534534534534534534e2e58e9b18b077f140bc197b66cb");
	const int outputIndexNum(3);
	
	const BigInt<256> mypriv(1234);
	PrivKey privkey(mypriv, false);	
	const BinaryScript script = ScriptCreator::createAnyoneCanSpendFundscript();
	//const BinaryScript binScript(script);
	const BigInt<128> prevAmountBigInt(123123123);
	const BigInt<64> sequence(0);

	const TransactionInput input(txHash, outputIndexNum, script, sequence);		
	const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEY);
	
	inputs.push_back(txInput);
	
	
	std::list<TransactionOutput> recipients;
	
	
	const std::string decodedHash = convertFromHex("f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2");
	const std::string toStore = "DOCPROOF" + decodedHash;
	const BinaryScript outScript = ScriptCreator::createOpReturnScript(toStore);	
	//const BinaryScript binOutScript(outScript);
	const Amount zeroAmountVal;		
	
	const TransactionOutput scriptAndAmount(outScript, zeroAmountVal);
	recipients.push_back(scriptAndAmount);

	const std::string decodedHash2 = convertFromHex("f2ca1bb6c7e907d062342342342342342342344e93b7605022da52e6ccc26fd2");
	const std::string toStore2 = "DOCPROOF" + decodedHash2;
	const BinaryScript outScript2 = ScriptCreator::createOpReturnScript(toStore2);
	//const BinaryScript binScript2(outScript2);
	const Amount amountVal(123123123);		
	
	const TransactionOutput scriptAndAmount2(outScript2, amountVal);
	recipients.push_back(scriptAndAmount2);

	
	int locktime(0);

	
	const TxData txData(inputs, recipients, locktime);
	txData.printTransactionInformation();
	
	std::list<BigInt<256> > tempKeys;
	const BigInt<256> tempkey(983745987);
	tempKeys.push_back(tempkey);
	
	const BigInt<256> tempkey2(983753497);
	tempKeys.push_back(tempkey2);

	const BinaryTransaction createdTransaction = txData.createSignedTransaction(tempKeys);
	std::cout << "createdTransaction: " << createdTransaction.asHex() << std::endl;
	                                     
	if(createdTransaction.asHex() != "0100000001CB667B19BC40F177B0189B8EE5E23445533445533445533435A0F635FD8E0E7703000000484730440220134D5827F5C1BDC7DA8920CF9033BBAADE2794F467F7964C351306B3DA606E85022003FD5013E66C31F5ABBB5DF019E343D381BE53353CB579A19F61521DD218F4D401FFFFFFFF0200000000000000002A6A28444F4350524F4F46F2CA1BB6C7E907D06DAFE4687E579FCE76B37E4E93B7605022DA52E6CCC26FD2B3B55607000000002A6A28444F4350524F4F46F2CA1BB6C7E907D062342342342342342342344E93B7605022DA52E6CCC26FD200000000")
	{
		throw std::string("Create transaction with two input and two output error 2");		
	}	
	
	//---------------------------------------
	const TxInput dummyInput;
	const std::list<BigInt<256> > dummyTempKeys;
	const BinaryTransaction unsignedTrans = txData.createTransactionInternal(false, dummyInput, dummyTempKeys);
	
	if(unsignedTrans.asHex() != "0100000001CB667B19BC40F177B0189B8EE5E23445533445533445533435A0F635FD8E0E770300000000FFFFFFFF0200000000000000002A6A28444F4350524F4F46F2CA1BB6C7E907D06DAFE4687E579FCE76B37E4E93B7605022DA52E6CCC26FD2B3B55607000000002A6A28444F4350524F4F46F2CA1BB6C7E907D062342342342342342342344E93B7605022DA52E6CCC26FD20000000001000000")
	{
		std::cout << "Unsigned trans: " << unsignedTrans.asHex() << std::endl;
		throw std::string("Create unsigned transaction error");		
	}	
	
}

	
	
void testCreateTransaction()
{
	testCreateEmptyTransaction();
	createTransactionOneInput();
	createTransactionOneInputOneOutput();
	createTransactionTwoInputsTwoOutputs();
	createTransactionOneInputP2PKHOneOutput();
	createTransactionOneInputP2PKOneOutput();
	createTransactionTwoInputsTwoOutputsTest2();
}	


void testDecodeScript1(const std::string& scriptHex, const std::string& decoded)
{
	//const std::string scriptHex = "76A91484EB725E264AADB07F9DB462436A1E3A0011DA5B88AC";
	//std::string script = getScript(hexStr);
	const BinaryScript script = BinaryScript(convertFromHex(scriptHex));
	
	
	const Script scr = Script::parse(script);
	
	const std::string decodedStr = scr.toString();
	std::cout << "Decoded: " << decodedStr << std::endl;

	if(decodedStr != "OP_DUP OP_HASH160 DATA(84EB725E264AADB07F9DB462436A1E3A0011DA5B) OP_EQUALVERIFY OP_CHECKSIG ")
	{
		std::cout << "Error for script: " << hexString(script.raw()) << std::endl;
		throw std::string("Script decode error");
	}

	const BinaryScript rawScr = scr.serialize();
	if(rawScr.raw() != script.raw())
	{
		std::cout << std::endl;
		std::cout << "Serialize error" << std::endl;
		
		std::cout << "Orig     : " << hexString(script.raw()) << std::endl;
		std::cout << "Serialzed: " << hexString(rawScr.raw()) << std::endl;
		
		throw std::string("Serialize error");
	}
	
}


void testDecodeScript()
{
	testDecodeScript1("76A91484EB725E264AADB07F9DB462436A1E3A0011DA5B88AC", 
		"OP_DUP OP_HASH160 DATA(84EB725E264AADB07F9DB462436A1E3A0011DA5B) OP_EQUALVERIFY OP_CHECKSIG ");
}







   
void testExtKey(const std::string& chainDescription, const std::string& xpriv, const std::string& xpub)
{
	std::cout << std::endl;
	std::cout << "=============================================" << std::endl;
	
	std::cout << "Testing chain " << chainDescription << std::endl;

	std::cout << "Orig extended: " << xpriv << std::endl;
	const ExtendedKey extendedKey = decodeHD(xpriv);
	if(!extendedKey.isPrivate)
	{
		std::cout << "This is not a private extended key" << std::endl;
		return;
	}

	const ExtendedPrivateKey getExtendedPrivateKey = decodePrivateHD(extendedKey);	
	const ExtendedPublicKey extendedPublicKey = getExtendedPrivateKey.getPub();

	std::cout << "Extended info:\n" << getExtendedPrivateKey.infoString() << std::endl;
	std::cout << "Extended priv: " << getExtendedPrivateKey.toString() << std::endl;	
	std::cout << std::endl;
	std::cout << "Extended pub : " << extendedPublicKey.toString() << std::endl;	
	std::cout << "Should be    : " << xpub << std::endl;
	if(extendedPublicKey.toString() != xpub)
	{
		throw std::string("Error in xpub");
	}


	std::cout << "Extended pub info:\n" << extendedPublicKey.infoString() << std::endl;
} 



void unitTestExtended()
{		
  	const std::string chainDescription8("m/0/2147483647 H");
	const std::string xpriv8("xprv9wSp6B7kry3Vj9m1zSnLvN3xH8RdsPP1Mh7fAaR7aRLcQMKTR2vidYEeEg2mUCTAwCd6vnxVrcjfy2kRgVsFawNzmjuHc2YmYRmagcEPdU9");
	const std::string xpub8("xpub6ASAVgeehLbnwdqV6UKMHVzgqAG8Gr6riv3Fxxpj8ksbH9ebxaEyBLZ85ySDhKiLDBrQSARLq1uNRts8RuJiHjaDMBU4Zn9h8LZNnBC5y4a");
	testExtKey(chainDescription8, xpriv8, xpub8);
		
		
	const std::string chainDescription7("m/0/2147483647H/1");
	const std::string xpriv7("xprv9zFnWC6h2cLgpmSA46vutJzBcfJ8yaJGg8cX1e5StJh45BBciYTRXSd25UEPVuesF9yog62tGAQtHjXajPPdbRCHuWS6T8XA2ECKADdw4Ef");	
	const std::string xpub7("xpub6DF8uhdarytz3FWdA8TvFSvvAh8dP3283MY7p2V4SeE2wyWmG5mg5EwVvmdMVCQcoNJxGoWaU9DCWh89LojfZ537wTfunKau47EL2dhHKon");
	testExtKey(chainDescription7, xpriv7, xpub7);
		
		
	const std::string chainDescription6("m/0H");
	const std::string xpriv6("xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7");	
	const std::string xpub6("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw");
	testExtKey(chainDescription6, xpriv6, xpub6);

		
	const std::string chainDescription5("m/0H/1");
	const std::string xpriv5("xprv9wTYmMFdV23N2TdNG573QoEsfRrWKQgWeibmLntzniatZvR9BmLnvSxqu53Kw1UmYPxLgboyZQaXwTCg8MSY3H2EU4pWcQDnRnrVA1xe8fs");	
	const std::string xpub5("xpub6ASuArnXKPbfEwhqN6e3mwBcDTgzisQN1wXN9BJcM47sSikHjJf3UFHKkNAWbWMiGj7Wf5uMash7SyYq527Hqck2AxYysAA7xmALppuCkwQ");
	testExtKey(chainDescription5, xpriv5, xpub5);
		
	const std::string chainDescription4("m/0H/1/2H");
	const std::string xpriv4("xprv9z4pot5VBttmtdRTWfWQmoH1taj2axGVzFqSb8C9xaxKymcFzXBDptWmT7FwuEzG3ryjH4ktypQSAewRiNMjANTtpgP4mLTj34bhnZX7UiM");	
	const std::string xpub4("xpub6D4BDPcP2GT577Vvch3R8wDkScZWzQzMMUm3PWbmWvVJrZwQY4VUNgqFJPMM3No2dFDFGTsxxpG5uJh7n7epu4trkrX7x7DogT5Uv6fcLW5");		
	testExtKey(chainDescription4, xpriv4, xpub4);
		
	const std::string chainDescription3("m/0H/1/2H/2");
	const std::string xpriv3("xprvA2JDeKCSNNZky6uBCviVfJSKyQ1mDYahRjijr5idH2WwLsEd4Hsb2Tyh8RfQMuPh7f7RtyzTtdrbdqqsunu5Mm3wDvUAKRHSC34sJ7in334");	
	const std::string xpub3("xpub6FHa3pjLCk84BayeJxFW2SP4XRrFd1JYnxeLeU8EqN3vDfZmbqBqaGJAyiLjTAwm6ZLRQUMv1ZACTj37sR62cfN7fe5JnJ7dh8zL4fiyLHV");
	testExtKey(chainDescription3, xpriv3, xpub3);

		
	const std::string chainDescription2("m/0H/1/2H/2/1000000000");
	const std::string xpriv2("xprvA41z7zogVVwxVSgdKUHDy1SKmdb533PjDz7J6N6mV6uS3ze1ai8FHa8kmHScGpWmj4WggLyQjgPie1rFSruoUihUZREPSL39UNdE3BBDu76");	
	const std::string xpub2("xpub6H1LXWLaKsWFhvm6RVpEL9P4KfRZSW7abD2ttkWP3SSQvnyA8FSVqNTEcYFgJS2UaFcxupHiYkro49S8yGasTvXEYBVPamhGW6cFJodrTHy");
	
	testExtKey(chainDescription2, xpriv2, xpub2);
	
		
	const std::string chainDescription1("m");
	const std::string xpriv1("xprv9s21ZrQH143K25QhxbucbDDuQ4naNntJRi4KUfWT7xo4EKsHt2QJDu7KXp1A3u7Bi1j8ph3EGsZ9Xvz9dGuVrtHHs7pXeTzjuxBrCmmhgC6");	
	const std::string xpub1("xpub661MyMwAqRbcEZVB4dScxMAdx6d4nFc9nvyvH3v4gJL378CSRZiYmhRoP7mBy6gSPSCYk6SzXPTf3ND1cZAceL7SfJ1Z3GC8vBgp2epUt13");

		
	testExtKey(chainDescription1, xpriv1, xpub1);
		
	const std::string chainDescription("m/0H");
	const std::string xpriv("xprv9uPDJpEQgRQfDcW7BkF7eTya6RPxXeJCqCJGHuCJ4GiRVLzkTXBAJMu2qaMWPrS7AANYqdq6vcBcBUdJCVVFceUvJFjaPdGZ2y9WACViL4L");	
	const std::string xpub("xpub68NZiKmJWnxxS6aaHmn81bvJeTESw724CRDs6HbuccFQN9Ku14VQrADWgqbhhTHBaohPX4CjNLf9fq9MYo6oDaPPLPxSb7gwQN3ih19Zm4Y");
		
	testExtKey(chainDescription, xpriv, xpub);
}




void testCalcHmacSha512(const std::string& K, const std::string& m, const std::string& expected)
{	
	std::cout << "Calculating HMAC SHA512.........." << std::flush;
		
	
	const std::string result = calcHmacSha512(K, m);
	if(hexString(result) != hexString(expected))
	{
		std::cout << std::endl;
		std::cout << "Got this HMAC: " << hexString(result) << std::endl;
		std::cout << "Should be    : " << hexString(expected) << std::endl;
		
		throw std::string("hmac error");
	}

	std::cout << "OK" << std::endl;	
}




void test1HmacSha512()
{	
	std::cout << "Calculating HMAC SHA512.........." << std::flush;
		
	const std::string K(convertFromHex("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
	const std::string m(convertFromHex("4869205468657265"));
	
	const std::string result = calcHmacSha512(K, m);
	if(hexString(result) != "87AA7CDEA5EF619D4FF0B4241A1D6CB02379F4E2CE4EC2787AD0B30545E17CDEDAA833B7D6B8A702038B274EAEA3F4E4BE9D914EEB61F1702E696C203A126854")
	{
		std::cout << "Got this HMAC: " << hexString(result) << std::endl;
		std::cout << "Should be    : " << "87AA7CDEA5EF619D4FF0B4241A1D6CB02379F4E2CE4EC2787AD0B30545E17CDEDAA833B7D6B8A702038B274EAEA3F4E4BE9D914EEB61F1702E696C203A126854" << std::endl;
		
		throw std::string("hmac error");
	}

	std::cout << "OK" << std::endl;	
}



void test2HmacSha512()
{	
	//const std::string sha512Empty = sha512("");
	//std::cout << "Sha512(empty): " << hexString(sha512Empty) << std::endl;
	
	std::cout << "Calculating HMAC SHA512.........." << std::flush;
		
	const std::string K(convertFromHex("4a656665"));
	const std::string m(convertFromHex("7768617420646f2079612077616e7420666f72206e6f7468696e673f"));
	
	//std::cout << "K: " << hexString(K) << std::endl;
	//std::cout << "m: " << hexString(m) << std::endl;
	
	//std::cout << std::endl;
	const std::string result = calcHmacSha512(K, m);
	if(hexString(result) != "164B7A7BFCF819E2E395FBE73B56E0A387BD64222E831FD610270CD7EA2505549758BF75C05A994A6D034F65F8F0E6FDCAEAB1A34D4A6B4B636E070A38BCE737")
	{
		std::cout << "Got this HMAC: " << hexString(result) << std::endl;
		std::cout << "Should be    : " << "164B7A7BFCF819E2E395FBE73B56E0A387BD64222E831FD610270CD7EA2505549758BF75C05A994A6D034F65F8F0E6FDCAEAB1A34D4A6B4B636E070A38BCE737" << std::endl;		
		throw std::string("hmac error");
	}

	std::cout << "OK" << std::endl;
	
	//std::cout << std::endl;
	//std::cout << std::endl;
}









void test3HmacSha512()
{	
	
	std::cout << "Calculating HMAC SHA512.........." << std::flush;
		
	const std::string K(convertFromHex("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
	const std::string m(convertFromHex("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"));
	
	
	const std::string result = calcHmacSha512(K, m);
	if(hexString(result) != "FA73B0089D56A284EFB0F0756C890BE9B1B5DBDD8EE81A3655F83E33B2279D39BF3E848279A722C806B485A47E67C807B946A337BEE8942674278859E13292FB")
	{
		std::cout << "Got this HMAC: " << hexString(result) << std::endl;
		std::cout << "Should be    : " << "fa73b0089d56a284efb0f0756c890be9b1b5dbdd8ee81a3655f83e33b2279d39bf3e848279a722c806b485a47e67c807b946a337bee8942674278859e13292fb" << std::endl;
		
		throw std::string("hmac error");
	}
	
	std::cout << "OK" << std::endl;
	
//	std::cout << std::endl;
//	std::cout << std::endl;
}





void testHmacSha512()
{	
	test1HmacSha512();
	test2HmacSha512();
	test3HmacSha512();	

	const std::string K(convertFromHex("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
	const std::string m(convertFromHex("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"));
	const std::string expected(convertFromHex("FA73B0089D56A284EFB0F0756C890BE9B1B5DBDD8EE81A3655F83E33B2279D39BF3E848279A722C806B485A47E67C807B946A337BEE8942674278859E13292FB"));
	
	testCalcHmacSha512(K, m, expected);


	const std::string K2(convertFromHex("abcdefabacbacbacbacbacb12312321312391299"));
	const std::string m2(convertFromHex("123123198712391823798789aaabcdef12837612837613876138761238761238718237618723618726318726318276318723"));
	const std::string expected2(convertFromHex("ED466A03F7E7951AB5DDBEAA9C7FF0045B4F361E9C01C2158BB095193A67F7B1F41B747BF754E38698251E5CA3528D0FB5C300A4B15B607ADCC58EA7720DEAE6"));
	
	testCalcHmacSha512(K2, m2, expected2);


	const std::string K3(convertFromHex("ab"));
	const std::string m3(convertFromHex("12312319123123191231231912312319123123191231231912312319123123191231231912312319123123191231231912312319123123198712391823798789aaabcdef12837612837613876138761238761238718237618723618726318726318276318723"));
	const std::string expected3(convertFromHex("2939700A752F7CF527A97E65124C48479086D61B2958F0406BF2360E0983C5FEC99FC2E5AC31229D3DFF83E4165451211386F5D7BA3E874D58B01D331CE8DB00"));
	
	testCalcHmacSha512(K3, m3, expected3);

	const std::string K4(convertFromHex("12312319123123191231231912312319123123191231231912312319123123191231231912312319123123191231231912312319123123198712391823798789aaabcdef12837612837613876138761238761238718237618723618726318726318276318723"));
	const std::string m4(convertFromHex("12"));
	const std::string expected4(convertFromHex("6600EC3BAD31C2194171F52CE167340A6092CD5A1B8876F565EE2475CFB1A22EAC0590B4522E18E59958298B8F1170964B1647010B0487BC336C0E1BD09F422E"));
	
	testCalcHmacSha512(K4, m4, expected4);

	const std::string K5(convertFromHex("1234"));
	const std::string m5(convertFromHex("5678"));
	const std::string expected5(convertFromHex("1F0429D5430FD0B84F2B53D0262E06CDC43AF9B8394C65868BBBA7785CC58689096DAA1B6CB1FDCF214C2A05848266A73D61DEE4491A276FB8C13201B8D5888E"));
	
	testCalcHmacSha512(K5, m5, expected5);
	
}




void testPbKdb2HmacSha512(const std::string& password, const std::string& salt, const int iterations, const int outSize,
							const std::string& expected)
{
	std::cout << "Testing PbKdb2HmacSha512......." << std::flush;
	
	const std::string result = PbKdb2HmacSha512(password, salt, iterations, outSize);
	
	if(hexString(result) != expected)
	{
		std::cout << std::endl;
		std::cout << std::endl;
		
		std::cout << "Result  : " << hexString(result) << std::endl;
		std::cout << "Expected: " << expected << std::endl;
		
		throw std::string("internal error in PbKdb2HmacSha512");
	}

	std::cout << "OK" << std::endl;
}




void testPbKdb2()
{
	std::cout << "Will calculate PbKdb2" << std::endl;


	testPbKdb2HmacSha512("password", "salt", 1, 64,
							"867F70CF1ADE02CFF3752599A3A53DC4AF34C7A669815AE5D513554E1C8CF252C02D470A285A0501BAD999BFE943C08F050235D7D68B1DA55E63F73B60A57FCE");

	testPbKdb2HmacSha512("password", "salt", 2, 64,
							"E1D9C16AA681708A45F5C7C4E215CEB66E011A2E9F0040713F18AEFDB866D53CF76CAB2868A39B9F7840EDCE4FEF5A82BE67335C77A6068E04112754F27CCF4E");

	testPbKdb2HmacSha512("passwordPASSWORDpassword", "saltSALTsaltSALTsaltSALTsaltSALTsalt", 4096, 64,
							"8C0511F4C6E597C6AC6315D8F0362E225F3C501495BA23B868C005174DC4EE71115B59F9E60CD9532FA33E0F75AEFE30225C583A186CD82BD4DAEA9724A3D3B8");

	testPbKdb2HmacSha512("passDATAb00AB7YxDTT", "saltKEYbcTcXHCBxtjD", 1, 64,
							"CBE6088AD4359AF42E603C2A33760EF9D4017A7B2AAD10AF46F992C660A0B461ECB0DC2A79C2570941BEA6A08D15D6887E79F32B132E1C134E9525EEDDD744FA");

	testPbKdb2HmacSha512("passDATAb00AB7YxDTTlRH2dqxDx19GDxDV1zFMz7E6QVqKIzwOtMnlxQLttpE57Un4u12D2YD7oOPpiEvCDYvntXEe4NNPLCnGGeJArbYDEu6xDoCfWH6kbuV6awi04Uz3ebEAhzZ4ve1A2wg5CnLXdZC5Y7gwfVgbEgZSTmoYQSzC5OW4dfrjqiwApTACO6xoOL1AjWj6X6f6qFfF8TVmOzU9RhOd1N4QtzWI4fP6FYttNz5FuLdtYVXWVXH2Tf7I9fieMeWCHTMkM4VcmQyQHpbcP8MEb5f1g6Ckg5xk3HQr3wMBvQcOHpCPy1K8HCM7a5wkPDhgVA0BVmwNpsRIbDQZRtHK6dT6bGyalp6gbFZBuBHwD86gTzkrFY7HkOVrgc0gJcGJZe65Ce8v4Jn5OzkuVsiU8efm2Pw2RnbpWSAr7SkVdCwXK2XSJDQ5fZ4HBEz9VTFYrG23ELuLjvx5njOLNgDAJuf5JB2tn4nMjjcnl1e8qcYVwZqFzEv2zhLyDWMkV4tzl4asLnvyAxTBkxPRZj2pRABWwb3kEofpsHYxMTAn38YSpZreoXipZWBnu6HDURaruXaIPYFPYHl9Ls9wsuD7rzaGfbOyfVgLIGK5rODphwRA7lm88bGKY8b7tWOtepyEvaLxMI7GZF5ScwpZTYeEDNUKPzvM2Im9zehIaznpguNdNXNMLWnwPu4H6zEvajkw3G3ucSiXKmh6XNe3hkdSANm3vnxzRXm4fcuzAx68IElXE2bkGFElluDLo6EsUDWZ4JIWBVaDwYdJx8uCXbQdoifzCs5kuuClaDaDqIhb5hJ2WR8mxiueFsS0aDGdIYmye5svmNmzQxFmdOkHoF7CfwuU1yy4uEEt9vPSP2wFp1dyaMvJW68vtB4kddLmI6gIgVVcT6ZX1Qm6WsusPrdisPLB2ScodXojCbL3DLj6PKG8QDVMWTrL1TpafT2wslRledWIhsTlv2mI3C066WMcTSwKLXdEDhVvFJ6ShiLKSN7gnRrlE0BnAw", 
		"saltKEYbcTcXHCBxtjD2PnBh44AIQ6XUOCESOhXpEp3HrcGMwbjzQKMSaf63IJemkURWoqHusIeVB8Il91NjiCGQacPUu9qTFaShLbKG0Yj4RCMV56WPj7E14EMpbxy6PlBdILBOkKUB6TGTPJXh1tpdOHTG6KuIvcbQp9qWjaf1uxAKgiTtYRIHhxjJI2viVa6fDZ67QOouOaf2RXQhpsWaTtAVnff6PIFcvJhdPDFGV5nvmZWoCZQodj6yXRDHPw9PyF0iLYm9uFtEunlAAxGB5qqea4X5tZvB1OfLVwymY3a3JPjdxTdvHxCHbqqE0zip61JNqdmeWxGtlRBC6CGoCiHO4XxHCntQBRJDcG0zW7joTdgtTBarsQQhlLXBGMNBSNmmTbDf3hFtawUBCJH18IAiRMwyeQJbJ2bERsY3MVRPuYCf4Au7gN72iGh1lRktSQtEFye7pO46kMXRrEjHQWXInMzzy7X2StXUzHVTFF2VdOoKn0WUqFNvB6PF7qIsOlYKj57bi1Psa34s85WxMSbTkhrd7VHdHZkTVaWdraohXYOePdeEvIwObCGEXkETUzqM5P2yzoBOJSdjpIYaa8zzdLD3yrb1TwCZuJVxsrq0XXY6vErU4QntsW0972XmGNyumFNJiPm4ONKh1RLvS1kddY3nm8276S4TUuZfrRQO8QxZRNuSaZI8JRZp5VojB5DktuMxAQkqoPjQ5Vtb6oXeOyY591CB1MEW1fLTCs0NrL321SaNRMqza1ETogAxpEiYwZ6pIgnMmSqNMRdZnCqA4gMWw1lIVATWK83OCeicNRUNOdfzS7A8vbLcmvKPtpOFvhNzwrrUdkvuKvaYJviQgeR7snGetO9JLCwIlHIj52gMCNU18d32SJl7Xomtl3wIe02SMvq1i1BcaX7lXioqWGmgVqBWU3fsUuGwHi6RUKCCQdEOBfNo2WdpFaCflcgnn0O6jVHCqkv8cQk81AqS00rAmHGCNTwyA6Tq5TXoLlDnC8gAQjDUsZp0z", 
		1, 65,
		"384BCD6914407E40C295D1037CF4F990E8F0E720AF43CB706683177016D36D1A14B3A7CF22B5DF8D5D7D44D69610B64251ADE2E7AB54A3813A89935592E391BF91");


	testPbKdb2HmacSha512("passDATAb00AB7YxDTT", "saltKEYbcTcXHCBxtjD", 100000, 65,
				"ACCDCD8798AE5CD85804739015EF2A11E32591B7B7D16F76819B30B0D49D80E1ABEA6C9822B80A1FDFE421E26F5603ECA8A47A64C9A004FB5AF8229F762FF41F7C");


	testPbKdb2HmacSha512("passDATAb00AB7YxDTTlRH2dqxDx19GDxDV1zFMz7E6QVqKIzwOtMnlxQLttpE57Un4u12D2YD7oOPpiEvCDYvntXEe4NNPLCnGGeJArbYDEu6xDoCfWH6kbuV6awi0", 
		"saltKEYbcTcXHCBxtjD2PnBh44AIQ6XUOCESOhXpEp3HrcGMwbjzQKMSaf63IJemkURWoqHusIeVB8Il91NjiCGQacPUu9qTFaShLbKG0Yj4RCMV56WPj7E14EMpbxy", 
		1, 65,
		"16226C85E4F8D604573008BFE61C10B6947B53990450612DD4A3077F7DEE2116229E68EFD1DF6D73BD3C6D07567790EEA1E8B2AE9A1B046BE593847D9441A1B766");


	testPbKdb2HmacSha512("passwordPASSWORDpassword", "saltSALTsaltSALTsaltSALTsaltSALTsalt", 4096, 64,
							"8C0511F4C6E597C6AC6315D8F0362E225F3C501495BA23B868C005174DC4EE71115B59F9E60CD9532FA33E0F75AEFE30225C583A186CD82BD4DAEA9724A3D3B8");

	testPbKdb2HmacSha512("passDATAb00AB7YxDTTlRH2dqxDx19GDxDV1zFMz7E6QVqKIzwOtMnlxQLttpE5", 
				"saltKEYbcTcXHCBxtjD2PnBh44AIQ6XUOCESOhXpEp3HrcGMwbjzQKMSaf63IJe",
				1, 65,
				"E2CCC7827F1DD7C33041A98906A8FD7BAE1920A55FCB8F831683F14F1C3979351CB868717E5AB342D9A11ACF0B12D3283931D609B06602DA33F8377D1F1F9902DA");


	testPbKdb2HmacSha512("MyLittlePasswordWhatIWillUse", "ThisIsTheSaltThatIsUsedNow", 1, 135,
							"C10D46C28A38D4F4121BD2E037E143CEB3AC245B89A9EB078476F0CF67789A12E13604E23F2C2E17FF8104C3B0223332E3C9E2F9E1ECA6131FA117E3F4CBA1FFB5511D802E5A0BA730A79EF9E33877749F0BCFCEEE05168D31AC618B5CDAE718BE539C787B56D86BC5A55D506FF82DFAE8E79FF493F9500C81BA9538CBBDAFB3185F96608C4C0B");

	testPbKdb2HmacSha512("MyLittlePasswordWhatIWillUse", 
				"ThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNowThisIsTheSaltThatIsUsedNow", 
				1, 4,"291F940F");

	testPbKdb2HmacSha512("MyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUseMyLittlePasswordWhatIWillUse", 
				"ThisIsTheSaltThatIsUsedNow", 
				1, 4,"12AA91FB");

	testPbKdb2HmacSha512("abc", "def", 1, 4,"A4E2216F");

	//std::cout << "PbKdb2HmacSha512 tests OK" << std::endl;
}



void testPrivateExtendedDerivation(const std::string& xpriv, const std::string& path, const std::string& shouldBeResult)
{	
	std::cout << std::endl;	
	std::cout << std::endl;
	std::cout << "================= Test ===================" << std::endl;

	ExtendedPrivateKey::getExtended(xpriv);
	std::cout << "Will test the xpriv" << std::endl;
	
	const ExtendedPrivateKey privKey(ExtendedPrivateKey::getExtended(xpriv));
	
	std::cout << "Will get derivation path: " << path << std::endl;
	DerivationPath mydp(path); 		//::getDerivationPath(path);
	std::cout << "OK, got here" << std::endl;
	const DerivationPath derivationPath(path); //(DerivationPath::getDerivationPath(path));
	std::cout << "Derivation path: " << derivationPath.toString() << std::endl;
	
	
	const ExtendedPrivateKey derived = privKey.derive(derivationPath);
	const std::string derivedExt = derived.toString();
	std::cout << "Result priv key: " << derivedExt << std::endl;
	if(derivedExt != shouldBeResult)
	{
		std::cout << "Got : " << derivedExt << std::endl;
		std::cout << "Need: " << shouldBeResult << std::endl;
		
		throw std::string("Internal error for ext format test");
	}
}




void testDerivationTestVectors()
{	
	std::cout << "Using given test vectors" << std::endl;
	
	const std::string xpriv("xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi");


	const std::string path1("m/0H");
	const std::string result1("xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7");
	testPrivateExtendedDerivation(xpriv, path1, result1);
	

	const std::string path2("m/0H/1");
	const std::string result2("xprv9wTYmMFdV23N2TdNG573QoEsfRrWKQgWeibmLntzniatZvR9BmLnvSxqu53Kw1UmYPxLgboyZQaXwTCg8MSY3H2EU4pWcQDnRnrVA1xe8fs");
	testPrivateExtendedDerivation(xpriv, path2, result2);




	const std::string path3("m/0H/1/2H");
	const std::string result3("xprv9z4pot5VBttmtdRTWfWQmoH1taj2axGVzFqSb8C9xaxKymcFzXBDptWmT7FwuEzG3ryjH4ktypQSAewRiNMjANTtpgP4mLTj34bhnZX7UiM");
	testPrivateExtendedDerivation(xpriv, path3, result3);




	const std::string path4("m/0H/1/2H/2");
	const std::string result4("xprvA2JDeKCSNNZky6uBCviVfJSKyQ1mDYahRjijr5idH2WwLsEd4Hsb2Tyh8RfQMuPh7f7RtyzTtdrbdqqsunu5Mm3wDvUAKRHSC34sJ7in334");
	testPrivateExtendedDerivation(xpriv, path4, result4);


	const std::string path5("m/0H/1/2H/2/1000000000");
	const std::string result5("xprvA41z7zogVVwxVSgdKUHDy1SKmdb533PjDz7J6N6mV6uS3ze1ai8FHa8kmHScGpWmj4WggLyQjgPie1rFSruoUihUZREPSL39UNdE3BBDu76");
	testPrivateExtendedDerivation(xpriv, path5, result5);

	//---------------

	const std::string myxpriv("xprv9s21ZrQH143K31xYSDQpPDxsXRTUcvj2iNHm5NUtrGiGG5e2DtALGdso3pGz6ssrdK4PFmM8NSpSBHNqPqm55Qn3LqFtT2emdEXVYsCzC2U");


	const std::string mypath1("m/0");	
	const std::string myresult1("xprv9vHkqa6EV4sPZHYqZznhT2NPtPCjKuDKGY38FBWLvgaDx45zo9WQRUT3dKYnjwih2yJD9mkrocEZXo1ex8G81dwSM1fwqWpWkeS3v86pgKt");
	testPrivateExtendedDerivation(myxpriv, mypath1, myresult1);


	const std::string mypath2("m/0/2147483647H");	
	const std::string myresult2("xprv9wSp6B7kry3Vj9m1zSnLvN3xH8RdsPP1Mh7fAaR7aRLcQMKTR2vidYEeEg2mUCTAwCd6vnxVrcjfy2kRgVsFawNzmjuHc2YmYRmagcEPdU9");	
	testPrivateExtendedDerivation(myxpriv, mypath2, myresult2);



	const std::string mypath3("m/0/2147483647H/1");	
	const std::string myresult3("xprv9zFnWC6h2cLgpmSA46vutJzBcfJ8yaJGg8cX1e5StJh45BBciYTRXSd25UEPVuesF9yog62tGAQtHjXajPPdbRCHuWS6T8XA2ECKADdw4Ef");
	testPrivateExtendedDerivation(myxpriv, mypath3, myresult3);



	const std::string mypath4("m/0/2147483647H/1/2147483646H");	
	const std::string myresult4("xprvA1RpRA33e1JQ7ifknakTFpgNXPmW2YvmhqLQYMmrj4xJXXWYpDPS3xz7iAxn8L39njGVyuoseXzU6rcxFLJ8HFsTjSyQbLYnMpCqE2VbFWc");
	testPrivateExtendedDerivation(myxpriv, mypath4, myresult4);


	const std::string mypath5("m/0/2147483647H/1/2147483646H/2");	
	const std::string myresult5("xprvA2nrNbFZABcdryreWet9Ea4LvTJcGsqrMzxHx98MMrotbir7yrKCEXw7nadnHM8Dq38EGfSh6dqA9QWTyefMLEcBYJUuekgW4BYPJcr9E7j");	
	testPrivateExtendedDerivation(myxpriv, mypath5, myresult5);


	//---------------


	const std::string thexpriv("xprv9s21ZrQH143K25QhxbucbDDuQ4naNntJRi4KUfWT7xo4EKsHt2QJDu7KXp1A3u7Bi1j8ph3EGsZ9Xvz9dGuVrtHHs7pXeTzjuxBrCmmhgC6");

	const std::string thepath1("m/0H");	
	const std::string theresult1("xprv9uPDJpEQgRQfDcW7BkF7eTya6RPxXeJCqCJGHuCJ4GiRVLzkTXBAJMu2qaMWPrS7AANYqdq6vcBcBUdJCVVFceUvJFjaPdGZ2y9WACViL4L");	
	testPrivateExtendedDerivation(thexpriv, thepath1, theresult1);
}






void testDerivation()
{	
	std::cout << std::endl;
	std::cout << "Testing extended address generation" << std::endl;


	const std::string xpriv("xprv9s21ZrQH143K3WqhR9vupHj96FYrzAQxdYwXG7KSrE2phFVgZbsB12zQrVsPj8tHBg2wSCSAisqPwh7y1dW5FqpydKVsTCyiY2Z3JszgNcQ");

	const ExtendedKey extendedKey = decodeHD(xpriv);
	if(!extendedKey.isPrivate)
	{
		throw std::string("Internal error, not extended private");
	}

	const ExtendedPrivateKey privKey = decodePrivateHD(extendedKey);
	std::cout << "Start priv key : " << privKey.toString() << std::endl;
	

	std::list<ChildNum> derivationList;
	
	const ChildNum childNum1(44, true);
	derivationList.push_back(childNum1);

	const ChildNum childNum2(0, true);
	derivationList.push_back(childNum2);

	const ChildNum childNum3(0, true);
	derivationList.push_back(childNum3);

	const ChildNum childNum4(0, false);
	derivationList.push_back(childNum4);

	const DerivationPath derivationPath(derivationList);

	std::cout << "Derivation path: " << derivationPath.toString() << std::endl;

	const ExtendedPrivateKey derived = privKey.derive(derivationPath);
	
	std::cout << "Result priv key: " << derived.toString() << std::endl;

	


	if(derived.toString() != "xprvA1Tf93Zzpe92pbEWMY9EvaDp8r8vQ8bgxutKRPKhnPqgMdfFdKTyAzEWNxRbqkuZxFCHBBd7yKqYVPBniLcEddHceN2ES195Z1CvmfcbnWn")
	{
		std::cout << "Got : " << derived.toString() << std::endl;
		
		throw std::string("Internal error for the ext format test");
		
	}



	const std::string xpriv2("xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi");
	const ExtendedKey extendedKey2 = decodeHD(xpriv2);
	if(!extendedKey2.isPrivate)
	{
		throw std::string("Internal error, not extended private");
	}

	const ExtendedPrivateKey privKey2 = decodePrivateHD(extendedKey2);
	
	const DerivationPath derivationPath2("m/0H"); //DerivationPath::getDerivationPath("m/0H"));
	std::cout << "Derivation path: " << derivationPath2.toString() << std::endl;
	const ExtendedPrivateKey derived2 = privKey2.derive(derivationPath2);	
	std::cout << "Result priv key: " << derived2.toString() << std::endl;
	if(derived2.toString() != "xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7")
	{
		throw std::string("Internal error for ext format test");
	}
}


void testGenerivPublicDerivation(const std::string& xpub, const std::string& path, const std::string& result)
{
	std::cout << std::endl;
	
	const ExtendedPublicKey pubKey(ExtendedPublicKey::getExtended(xpub));
	std::cout << "Start: " << pubKey.toString() << std::endl;
	
	const DerivationPath derivationPath(path); //DerivationPath::getDerivationPath(path));
	
	std::cout << "Derivation path: " << derivationPath.toString() << std::endl;
	
	
	const ExtendedPublicKey derived = pubKey.derive(derivationPath);
	const std::string derivedStr = derived.toString();
	

	std::cout << "Result: " << derivedStr << std::endl;
	if(derivedStr != result)
	{
		std::cout << std::endl;
		std::cout << "Result         : " << derivedStr << std::endl;
		std::cout << "Expected result: " << result << std::endl;
		throw std::string("Internal error for public derivation");
	}

	std::cout << "Test OK" << std::endl;
	
}


void testPublicDerivation(const std::string& xpub, const ChildNum& num, const std::string& result)
{	
	const ExtendedPublicKey pubKey(ExtendedPublicKey::getExtended(xpub));
	std::cout << "Start: " << pubKey.toString() << std::endl;
	
	//const ChildNum num(1, false);
	std::cout << "Child: " << num.toString() << std::endl;	
	
	const ExtendedPublicKey derived = CKDpub(pubKey, num);

	const std::string derivedStr = derived.toString();
	
	std::cout << "Result: " << derivedStr << std::endl;
	
	if(derivedStr != result)
	{
		throw std::string("Internal error for public derivation");
	}

	std::cout << "Test OK" << std::endl;
}



void testOnePublicDerivation()
{
	const ExtendedPublicKey pubKey(ExtendedPublicKey::getExtended("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw"));
	std::cout << "Start: " << pubKey.toString() << std::endl;
	
	const ChildNum num(1, false);
	std::cout << "Child: " << num.toString() << std::endl;	
	
	const ExtendedPublicKey derived = CKDpub(pubKey, num);

	const std::string derivedStr = derived.toString();
	
	std::cout << "Result: " << derivedStr << std::endl;
	
	if(derivedStr != "xpub6ASuArnXKPbfEwhqN6e3mwBcDTgzisQN1wXN9BJcM47sSikHjJf3UFHKkNAWbWMiGj7Wf5uMash7SyYq527Hqck2AxYysAA7xmALppuCkwQ")
	{
		throw std::string("Internal error for public derivation");
	}
}



void testPubDerivation()
{	
	std::cout << "Testing public derivation" << std::endl;
	
	testOnePublicDerivation();
	
	const std::string xpub("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw");
	const ChildNum num(1, false);
	const std::string result("xpub6ASuArnXKPbfEwhqN6e3mwBcDTgzisQN1wXN9BJcM47sSikHjJf3UFHKkNAWbWMiGj7Wf5uMash7SyYq527Hqck2AxYysAA7xmALppuCkwQ");
	
	testPublicDerivation(xpub, num, result);


	const std::string xpub2("xpub6D4BDPcP2GT577Vvch3R8wDkScZWzQzMMUm3PWbmWvVJrZwQY4VUNgqFJPMM3No2dFDFGTsxxpG5uJh7n7epu4trkrX7x7DogT5Uv6fcLW5");
	const ChildNum num2(2, false);
	const std::string result2("xpub6FHa3pjLCk84BayeJxFW2SP4XRrFd1JYnxeLeU8EqN3vDfZmbqBqaGJAyiLjTAwm6ZLRQUMv1ZACTj37sR62cfN7fe5JnJ7dh8zL4fiyLHV");

	testPublicDerivation(xpub2, num2, result2);


	const std::string xpub3("xpub6D4BDPcP2GT577Vvch3R8wDkScZWzQzMMUm3PWbmWvVJrZwQY4VUNgqFJPMM3No2dFDFGTsxxpG5uJh7n7epu4trkrX7x7DogT5Uv6fcLW5");
	const std::string path3("M/2");
	const std::string result3("xpub6FHa3pjLCk84BayeJxFW2SP4XRrFd1JYnxeLeU8EqN3vDfZmbqBqaGJAyiLjTAwm6ZLRQUMv1ZACTj37sR62cfN7fe5JnJ7dh8zL4fiyLHV");

	testGenerivPublicDerivation(xpub3, path3, result3);


	const std::string xpub4("xpub6FHa3pjLCk84BayeJxFW2SP4XRrFd1JYnxeLeU8EqN3vDfZmbqBqaGJAyiLjTAwm6ZLRQUMv1ZACTj37sR62cfN7fe5JnJ7dh8zL4fiyLHV");
	const std::string path4("M/1000000000");
	const std::string result4("xpub6H1LXWLaKsWFhvm6RVpEL9P4KfRZSW7abD2ttkWP3SSQvnyA8FSVqNTEcYFgJS2UaFcxupHiYkro49S8yGasTvXEYBVPamhGW6cFJodrTHy");

	testGenerivPublicDerivation(xpub4, path4, result4);






	const std::string xpub5("xpub661MyMwAqRbcFW31YEwpkMuc5THy2PSt5bDMsktWQcFF8syAmRUapSCGu8ED9W6oDMSgv6Zz8idoc4a6mr8BDzTJY47LJhkJ8UB7WEGuduB");
	const std::string path5("M/0");
	const std::string result5("xpub69H7F5d8KSRgmmdJg2KhpAK8SR3DjMwAdkxj3ZuxV27CprR9LgpeyGmXUbC6wb7ERfvrnKZjXoUmmDznezpbZb7ap6r1D3tgFxHmwMkQTPH");

	testGenerivPublicDerivation(xpub5, path5, result5);






	const std::string xpub6("xpub6ASAVgeehLbnwdqV6UKMHVzgqAG8Gr6riv3Fxxpj8ksbH9ebxaEyBLZ85ySDhKiLDBrQSARLq1uNRts8RuJiHjaDMBU4Zn9h8LZNnBC5y4a");
	const std::string path6("M/1");
	const std::string result6("xpub6DF8uhdarytz3FWdA8TvFSvvAh8dP3283MY7p2V4SeE2wyWmG5mg5EwVvmdMVCQcoNJxGoWaU9DCWh89LojfZ537wTfunKau47EL2dhHKon");

	testGenerivPublicDerivation(xpub6, path6, result6);






	const std::string xpub7("xpub6ERApfZwUNrhLCkDtcHTcxd75RbzS1ed54G1LkBUHQVHQKqhMkhgbmJbZRkrgZw4koxb5JaHWkY4ALHY2grBGRjaDMzQLcgJvLJuZZvRcEL");
	const std::string path7("M/2");
	const std::string result7("xpub6FnCn6nSzZAw5Tw7cgR9bi15UV96gLZhjDstkXXxvCLsUXBGXPdSnLFbdpq8p9HmGsApME5hQTZ3emM2rnY5agb9rXpVGyy3bdW6EEgAtqt");

	testGenerivPublicDerivation(xpub7, path7, result7);


	const std::string xpub8("xpub6ERApfZwUNrhLCkDtcHTcxd75RbzS1ed54G1LkBUHQVHQKqhMkhgbmJbZRkrgZw4koxb5JaHWkY4ALHY2grBGRjaDMzQLcgJvLJuZZvRcEL");
	const std::string path8("M/2234234");
	const std::string result8("xpub6FnCn6nT14Mhr21nYW3QjASeGKzK34CYwmtsSSEzeWYkQWhxsxo48nPajZ3kRLggazCa1vLmkBvVkYEY7xDez6gd6yShGwmENVgPWEE8BLc");

	testGenerivPublicDerivation(xpub8, path8, result8);


	const std::string xpub9("xpub6ERApfZwUNrhLCkDtcHTcxd75RbzS1ed54G1LkBUHQVHQKqhMkhgbmJbZRkrgZw4koxb5JaHWkY4ALHY2grBGRjaDMzQLcgJvLJuZZvRcEL");
	const std::string path9("M/2234234/234/12312/345/23440");
	const std::string result9("xpub6PAu3v4F5EZpbuH3t3d6wipA5ot68mTP1rKffmwpvMeWrK5xqR5AtzTXjFB65nAnWwvSJSRa4kCezhHyAakhPtnbuQ75iEa722D6NtXemGB");

	testGenerivPublicDerivation(xpub9, path9, result9);



	const std::string xpubRoot("xpub661MyMwAqRbcFr9sLa699ANa9HHVtkMVfibriHZKx4Cu1c1hbqhmNVdnkW1UTGM483smCxnwsDEJMukyZbSSC42mr7eLrjcSgPHisZdof4H");
	const std::string path10("M/89");
	const std::string result10("xpub68G7rbpNYGyxMNpUsDk73y2wHaHm9S3ucUVM5D1GtMAnMQYw7rnNTbvV1EMx76bdAPo3G7BpWxFLpJwWC6anRv6jYDpj6Tp8zaHxbesr8oV");

	testGenerivPublicDerivation(xpubRoot, path10, result10);



	const std::string path11("M/89/123/999999/768");
	const std::string result11("xpub6FKM9xxTUytMHTZiwBfx7ox9DNs7BgrNRabAsedNC6wUAiSnttj7urbb4atgbu7zh3Kfnm97VN5WM1QTFboPtKbq375Ly83cA7B5anisfTS");
	testGenerivPublicDerivation(xpubRoot, path11, result11);


	const std::string path12("M/89/123/999999/768/0");
	const std::string result12("xpub6HHK7EX5FLuaAguTEvLV7sNrsQmYuedxsVba2MxcmVfw7eDxuWSKVtyFe1VpcDEMwpJxmq5f8o1VyFjkVBhvV2DM1Dm1oxiTDYQQY7DWPkX");
	testGenerivPublicDerivation(xpubRoot, path12, result12);

	const std::string path13("M/0/0/0/0/0/123456789");
	const std::string result13("xpub6JCwV5qtSHUodDiRZW8TG3ka8LWrTDYPDRbkQeAmVYLr23zMBgCg4s92Mhy1pTx2RfUK2LADkRndB2dUet4KeXmN1SwJTfn4ycWYdMTFNCg");
	testGenerivPublicDerivation(xpubRoot, path13, result13);


	const std::string path14("M/0/1/2/3/4/5/6/7/8/9/10/11/12/13/14/15/16/17/18/19/20");
	const std::string result14("xpub6o9P3CWnajD6sbkQNb6nFsonWQidGiP4v2HAt2AdLpXdbSe7B6HkbsLXLJo1shBcVQWxj9Tri2646hh99TSwYVufTBwyNNbCyUdJ2gV1YAh");
	testGenerivPublicDerivation(xpubRoot, path14, result14);

	
	std::cout << "All tests OK" << std::endl;
}




	
void testMnemonicLen(const BigInt<264>& bigN, const int numWords, const int numBits)
{
	const BigInt<264> orig(bigN & numBits);
	const std::string mnemonicStr = Mnemonic::bigIntToMnemonic(orig, numWords);
	const BigInt<264> conv = Mnemonic::mnemonicToBigInt(mnemonicStr);

	std::cout << std::endl;
	std::cout << "orig: " << orig << std::endl;	
	std::cout << "conv: " << conv << std::endl;
	std::cout << "mneStr: " << mnemonicStr << std::endl;

	
	if(orig != conv)
	{
		std::cout << "orig: " << orig << std::endl;
		std::cout << "conv: " << conv << std::endl;
		
		throw std::string("Error, did not get same for mnemonic conversion length test");
	}

}





void testMnemonicClass(const BigInt<256>& myDataLong, const int numWords, const int numBits)
{
	const BigInt<256> myData1(myDataLong & numBits);
	
	const Mnemonic myMnemonicData(myData1, numWords);
	const BigInt<256> myData2 = myMnemonicData.toPrivKey();
	
	std::cout << std::endl;
	std::cout << "My data 1: " << myData1 << std::endl;
	std::cout << "My data 2: " << myData2 << std::endl;

	std::cout << "Mnemonic: " << myMnemonicData.toStr() << std::endl;
	
	if(myData1 != myData2)
	{
		throw std::string("Got mnemonic conversion error error");
	}

}



void testMnemonic()
{
	const BigInt<256> privKey(BigInt<256>::fromHex("5FFFA7115897AFCD111617617177188881199901110101012555161666162661"));
	
	const Mnemonic mnemonic(privKey);
	
	const BigInt<256> privKey2 = mnemonic.toPrivKey();

	std::cout << "Priv1: " << privKey << std::endl;
	std::cout << "Priv2: " << privKey2 << std::endl;
	
	if(privKey != privKey2)
	{
		throw std::string("Error, priv keys differ");
	}
	
	const std::string mnemonicStr = mnemonic.toStr();
	std::cout << "Mnemonic: " << mnemonicStr << std::endl;
	const std::string expectedMnemonic("garment wood session raise kit track dust seed gesture merry shiver baby bag smart affair cage absurd ankle fetch club grid arch office clever");
	
	if(mnemonicStr != expectedMnemonic)
	{
		std::cout << "Expected: " << expectedMnemonic << std::endl;
		throw std::string("Mnemonic error, incorrect value");
	}
	
	const Mnemonic mnemonic2(expectedMnemonic/*, false*/);
	
	if(mnemonic2.toStr() != mnemonicStr)
	{
		throw std::string("Error, different mnemonics");
	}
	
	const BigInt<256> privKey3 = mnemonic2.toPrivKey();	
	std::cout << "Priv3: " << privKey3 << std::endl;
	
	if(privKey2 != privKey3)
	{
		throw std::string("Error, the priv keys differ");		
	}
	
	
	std::cout << "Will test mnemonic from priv key" << std::endl;
	const BigInt<256> myprivkey1(BigInt<256>::fromHex("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"));
	std::cout << "myprivkey1: " << myprivkey1 << std::endl;
	
	const Mnemonic myMnemonic1(myprivkey1);
	const std::string myMnemonicStr1 = myMnemonic1.toStr();
	std::cout << "myMnemonicStr1: " << myMnemonicStr1 << std::endl;
	const Mnemonic myMnemonic2(myMnemonicStr1);
	const BigInt<256> myprivkey2 = myMnemonic2.toPrivKey();
	std::cout << "myprivkey2: " << myprivkey2 << std::endl;
	if(myprivkey1 != myprivkey2)
	{
		throw std::string("Error, private keys not equal");
	}
	
	if(myMnemonic1.numberWords() != 24)
	{
		throw std::string("Incorrect number of mnemonic words");
	}

	if(myMnemonic2.numberWords() != 24)
	{
		throw std::string("Incorrect number of mnemonic words");
	}

	std::cout << std::endl;
	std::cout << std::endl;
	
	const std::string mnemonic12wordsStr("uniform student offer lumber later rescue rabbit recall monster write ship kidney");
	
	const Mnemonic mnemonic12words(mnemonic12wordsStr);
	std::cout << "The 12 word mnemonic: " << mnemonic12words.toStr() << std::endl;
	
	const BigInt<256> privkey12words = mnemonic12words.toPrivKey();
	std::cout << "Privkey 12 words           : " << privkey12words << std::endl;	
	const std::string asNumber = mnemonic12words.getAsNumber();
	std::cout << "Mnemonic 12 words as number:  " << hexString(asNumber) << std::endl;
	
	if(hexString(asNumber) != "D7AEE65C277D56E6C0D9B8F5FD3183D3")
	{
		throw std::string("Mnemonic as number error");
	}
	
	const Mnemonic mnemonic12wordsAgain(privkey12words, 12);
	
	if(mnemonic12wordsAgain.toStr() != mnemonic12words.toStr())
	{
		std::cout << std::endl;
		std::cout << "First: " << mnemonic12words.toStr() << std::endl;
		std::cout << "Second: " << mnemonic12wordsAgain.toStr() << std::endl;
		
		throw std::string("Error, mnemonic with 12 words does not match");
	}
	
	std::cout << std::endl;
	std::cout << "=============================================" << std::endl;
	
	const BigInt<264> total("128736178263187263817263876123");
	const std::string myMnemonicStr = Mnemonic::bigIntToMnemonic(total, 24);
	const BigInt<264> total2 = Mnemonic::mnemonicToBigInt(myMnemonicStr);
	
	std::cout << "Mne: " << myMnemonicStr << std::endl;
	
	if(total != total2)
	{
		std::cout << "A: " << total << std::endl;
		std::cout << "B: " << total2 << std::endl;
		
		throw std::string("Error, did not get the same number");
	}
	
	std::cout << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	
	const BigInt<264> a("78912777474872837462387462387462387462783");
	std::cout << "a: " << a << std::endl;
	const std::string mneStr15 = Mnemonic::bigIntToMnemonic(a, 15);
	std::cout << "mneStr15: " << mneStr15 << std::endl;
	
	const BigInt<264> b = Mnemonic::mnemonicToBigInt(mneStr15);
	std::cout << "b: " << b << std::endl;

	if(mneStr15 != "detail duty upon elder drum patch trip castle demand material attend giraffe senior blush vacant")
	{
		throw std::string("Did not get correct mnemonic");		
	}
	
	if(a != b)
	{
		std::cout << "AA: " << a << std::endl;
		std::cout << "BB: " << b << std::endl;
		
		throw std::string("Error, did not get the same number after mnemonic conversion");
	}

	std::cout << std::endl;
	std::cout << "+++++++++++++++++++++++++++++++++" << std::endl;
	
	const BigInt<264> a1("38912777474872837462387462444355355387462387462783");
	std::cout << "a1: " << a1 << std::endl;
	const std::string mneStr18 = Mnemonic::bigIntToMnemonic(a1, 18);
	std::cout << "mneStr18: " << mneStr18 << std::endl;
	
	const BigInt<264> b1 = Mnemonic::mnemonicToBigInt(mneStr18);
	std::cout << "b1: " << b1 << std::endl;


	if(mneStr18 != "time ceiling jar demise bronze alone economy bring spice embrace drive predict female attend giraffe senior blush vacant")
	{
		throw std::string("Did not get correct mnemonic 18");		
	}
	
	if(a1 != b1)
	{
		std::cout << "AA1: " << a1 << std::endl;
		std::cout << "BB1: " << b1 << std::endl;
		
		throw std::string("Error, did not get same number after mnemonic conversion");
	}


	std::cout << std::endl;
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	
	const BigInt<264> a2("242387423423846287346278364763287462736437377377373773777");
	std::cout << "a2: " << a2 << std::endl;
	const std::string mneStr21 = Mnemonic::bigIntToMnemonic(a2, 21);
	std::cout << "mneStr21: " << mneStr21 << std::endl;
	
	const BigInt<264> b2 = Mnemonic::mnemonicToBigInt(mneStr21);
	std::cout << "b2: " << b2 << std::endl;


	
	if(a2 != b2)
	{
		std::cout << "AA2: " << a2 << std::endl;
		std::cout << "BB2: " << b2 << std::endl;
		
		throw std::string("Error, did not get same number after mnemonic 21 conversion");
	}

	if(mneStr21 != "afraid castle deliver minimum either economy drum spice excuse curve deposit skate trip cheese rare system unveil jaguar rifle system upon")
	{
		throw std::string("Did not get correct mnemonic 21");
	}


	std::cout << std::endl;
	std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
	
	const BigInt<264> a3init("623487263487236487263487623423423874233647662736437377377373773777");
	const BigInt<264> a3(a3init & 132);
	
	std::cout << "a3: " << a3 << std::endl;
	const std::string mneStr12 = Mnemonic::bigIntToMnemonic(a3, 12);
	std::cout << "mneStr12: " << mneStr12 << std::endl;
	
	const BigInt<264> b3 = Mnemonic::mnemonicToBigInt(mneStr12);
	std::cout << "b3: " << b3 << std::endl;


	
	if(a3 != b3)
	{
		std::cout << "AA3: " << a3 << std::endl;
		std::cout << "BB3: " << b3 << std::endl;
		
		throw std::string("Error, did not get same for mnemonic conversion");
	}

	if(mneStr12 != "mansion bag suit under cheese rare system unveil jaguar rifle system upon")
	{
		throw std::string("Did not get correct mnemonic 12");
	}



	std::cout << std::endl;
	std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
	
	const BigInt<264> bigN("478247234873773838838293842384787877771771716262626737363738377773");
	
	testMnemonicLen(bigN, 12, 132);
	testMnemonicLen(bigN, 15, 165);
	testMnemonicLen(bigN, 18, 198);
	testMnemonicLen(bigN, 21, 231);
	testMnemonicLen(bigN, 24, 264);


	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "()()()()()()()()()()()()()()()()()()()()()()()()()()()" << std::endl;

	const BigInt<256> rawData1("872872872872872872872872873388388388873287238278278738838838337");
	const Mnemonic mnemonicData(rawData1);
	const BigInt<256> rawData2 = mnemonicData.toPrivKey();
	
	std::cout << std::endl;
	std::cout << "Raw data 1: " << rawData1 << std::endl;
	std::cout << "Raw data 2: " << rawData2 << std::endl;

	std::cout << "Mnemonic: " << mnemonicData.toStr() << std::endl;
	
	if(rawData1 != rawData2)
	{
		throw std::string("Mnemonic conversion error error");
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}" << std::endl;

	const BigInt<256> myDataLong("872872872872872872872872873388388388873287238278278738838838337");
	const BigInt<256> myData1(myDataLong & 128);
	
	const Mnemonic myMnemonicData(myData1, 12);
	const BigInt<256> myData2 = myMnemonicData.toPrivKey();
	
	std::cout << std::endl;
	std::cout << "My data 1: " << myData1 << std::endl;
	std::cout << "My data 2: " << myData2 << std::endl;

	std::cout << "Mnemonic: " << myMnemonicData.toStr() << std::endl;
	
	if(myData1 != myData2)
	{
		throw std::string("Got mnemonic conversion error error");
	}

	if(myMnemonicData.toStr() != "market session broom extend castle because antique attack market series this hub")
	{
		throw std::string("Mneconic error");
	}
	
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "[][][][][][][][][][][][][][][][][][][][][]" << std::endl;

	const BigInt<256> dataLong("872872872872872872872872873388388388873287238278278738838838337");
	testMnemonicClass(dataLong, 12, 128);
	testMnemonicClass(dataLong, 15, 160);
	testMnemonicClass(dataLong, 18, 192);
	testMnemonicClass(dataLong, 21, 224);
	testMnemonicClass(dataLong, 24, 256);
	
	
	const BigInt<256> myHexData("234823764876ABBBCFFC242342342342");
	const Mnemonic hexMnemonic(myHexData, 12);

	const std::string hexMne = hexMnemonic.toStr();
	if(hexMne != "cash donate talk movie height target divide loyal cash balance angle lumber")
	{
		throw std::string("Mneconic error for hex test");		
	}


	const ExtendedPrivateKey pk(ExtendedPrivateKey::getExtended("xprv9s21ZrQH143K2R1J9Tz8t2uzCroXB5AkeG2BFVLmZTN153TxGjG2o5KpYyvcmDNQYyU9eoPrkZ3yi5gF4wuFdshA1jR7samayGDvHiMfnc7"));	
	std::cout << "PK: " << pk.toString() << std::endl;
	const ExtendedPublicKey puk = pk.getPub();
	const ExtendedPublicKey cleanedPub = puk.getWithoutDeptAndParent();
	std::cout << "PUBK: " << cleanedPub.toString() << std::endl;
	
	
	if(cleanedPub.toString() != "xpub661MyMwAqRbcEu5mFVX9FArikte1aXtc1Uwn3skP7ntywqo6pGaHLseJQG4aeW44HXAjt3eoxQdhxVwn5u8ZsepVv8hcYR6cVQxEjzndNvq")
	{
		throw std::string("Extended key unit test error");
	}
	
	const DerivationPath dp("m/1H/2H/3H");
	
	const ExtendedPrivateKey derPk = pk.derive(dp);
	std::cout << "Der: " << derPk.toString() << std::endl;
	
	if(derPk.toString() != "xprv9yHfW3qjYDUwCnhNtKqwXaRKE2eQj6S2uAmDsxZxAyCvTU1LMS6QRwbBJF7aT8vbHdZM1TFJZxqTdQm959vq4WRSwrkGqPB4igrJQwkUFN7")
	{
		throw std::string("Extended der unit test error");		
	}
	
	const ExtendedPublicKey myPub = derPk.getPub();
	std::cout << "MyPub: " << myPub.toString() << std::endl;
	
	if(myPub.toString() != "xpub6CH1uZNdNb3ERGmqzMNwtiN3n4Uu8Z9tGPgpgLyZjJjuLGLUtyQeyjuf9ZY8nc7Jpc4r52H9T14dRbwoy3mVHNCA4ub4LCuzpsipLm2Cded")
	{
		throw std::string("Extended derived publik extended key unit test error");		
	}
}




void unitTestsBitcoin()
{
	const bool compressed(true);
	
	std::cout << "Performing software tests" << std::endl;

	testMnemonic();

	//std::cout << "Ready" <<std::endl;
	//return;

	testDerivationTestVectors();

	
	testPubDerivation();	
	testDerivation();	
	testPbKdb2();	
	testHmacSha512();
	
	
	
	unitTestExtended();


	testDecodeScript();

	testCreateTransaction();
	
	testCryptoTransaction();
	testScript();
	
	testSeveralMultisigTransactions();
    testMultisigSignature();
    
    testTotalSignOfMultisig();
	//return;		//Todo: Remove

	unitTestsMultisig();


	const Pkif myLittlePkif("7TNNAM1DB7CWUm5zhm4ZddPwwrza6BiY7haU7croLo4LjxkMtwk");		
	
	const PubKey myLittlePubKey = myLittlePkif.getPubKey();
	const ECPoint myLittlePoint = myLittlePubKey.point;
	
	BitcoinEllipticCurve ec;
	const bool pointVerify = ec.verifyECPoint(myLittlePoint);
	if(!pointVerify)
	{
		std::cout << "Error, incorrect point" << std::endl;
		throw std::string("Error, incorrect point");;		
	}
	
	std::cout << "Will test raw non-compressed" << std::endl;
	
	const std::string rawNonCompressed = myLittlePoint.getRaw(false);
	std::cout << "rawNonCompressed: " << hexString(rawNonCompressed) << std::endl;
	
	if(hexString(rawNonCompressed) != "0496D9E4C0C992F2A7C19C8696C8C0350EFA21DC6F4974CCC97D5C1359DA18CA68DE86501F6FA118F2B32C7AA19B6E170E3A8212753867AA1CD65810D556C05418")
	{
		throw std::string("Raw format error");
	}
	
	std::cout << "Will test pubkey parse" << std::endl;
	
	const std::string strToParse = hexString(rawNonCompressed);
	std::cout << "Will parse: " << strToParse << std::endl;
	
	const PubKey myLittlePk2 = PubKey::fromHex(strToParse);	
	std::cout << "The point2      : " << hexString(myLittlePk2.point.getRaw(false)) << std::endl;

	
	bool comp(false);
	const ECPoint myLittlePoint3 = ECPoint::parsePointHex(hexString(rawNonCompressed), comp);
	std::cout << "The point3      : " << hexString(myLittlePoint3.getRaw(false)) << std::endl;
	

	const ECPoint myLittlePoint4 = ECPoint::parsePointBin(rawNonCompressed, comp);
	std::cout << "The point4      : " << hexString(myLittlePoint4.getRaw(false)) << std::endl;


	std::cout << "Will test compressed pkif" << std::endl;

	const Pkif compressedPkif("7ZZiYEx6f39yHA3KYB8KNkQXuopm3P3ttFyxgz2KtNF4f9JdMsy");
	const PubKey compressedPubKey = compressedPkif.getPubKey();
	const ECPoint compressedPoint = compressedPubKey.point;
	if(!ec.verifyECPoint(compressedPoint))
	{
		throw std::string("Error, incorrect point");;
	}


	std::cout << "Testing incorrect pkif" << std::endl;
	try
	{
		const Pkif errorPkif("7ZZiYEx6f39yHAgdggsggsgsgtNF4f9JdMsy");		
	}
	catch(const std::string& err)
	{
		
	}
	std::cout << "Ready testing incorrect pkif" << std::endl;
	
	const BigInt<1024> errX("54353453453534534534534354");
	const BigInt<1024> errY("87326427364364364736476347");
	
	try
	{
		const ECPoint errorPoint = ec.fromCoordinates(errX, errX);
	
		//const ECPoint errorPoint(errX, errX);
		if(ec.verifyECPoint(errorPoint))
		{
			throw std::string("Error, did not detect incorrect EC point");
		}

	}
	catch(const std::string& err)
	{
		std::cout << "OK, incorrect point" << std::endl;
	}

	const PubKey parsedPub = PubKey::fromHex("02E2D9949EA2B70CAD4AFB6E1F3C8E4FDA61CCA44A82B1E82D1B6BD770C0EB6928");

	std::cout << "Will parse incorrect pubkey" << std::endl;
	try
	{
		const PubKey parsedErrorPub = PubKey::fromHex("02E2D9949EA274747747777777777777777777777777B1E82D1B6BD770C0EB6928");
		throw "Error, no problem reported";
	}
	catch(const std::string& err)
	{
		
	}



	const BigInt<1024> myX(BigInt<1024>::fromHex("1111111111111111111111111111111111111111111111111111111111111111"));
	const BigInt<1024> myY(BigInt<1024>::fromHex("2222222222222222222222222222222222222222222222222222222222222222"));
	

	try
	{	
	const ECPoint myPoint = ec.fromCoordinates(myX, myX);
	
	
	//const ECPoint myPoint(myX, myX);
	const std::string myComprPoint = hexString(myPoint.getRaw(true));
	std::cout << "Compressed point: " << myComprPoint << std::endl;;
	if(myComprPoint != "031111111111111111111111111111111111111111111111111111111111111111")
	{
		throw std::string("Error with point");
	}

	}
	catch(const std::string& err)
	{
		std::cout << "OK, this is an incorrect point" << std::endl;
	}


	std::string bytePoint = hexStrToBytes("045B3EA0A716142474E91ADA25910934E948DE3ABC8DEAA8D1EB96DB392ED004BC0F1F02AA20F4FB979DD422863BA9FEE1C1B9AEB51D79D51BC67BCB611EF79ADE");	
	bool c(false);
	const ECPoint pp = ECPoint::parsePointBin(bytePoint, c);
	if(!ec.verifyECPoint(pp))
	{
		std::cout << "The point: " << pp << std::endl;
		throw std::string("Error, did not detect incorrect EC point");
	}



	std::string bytePoint2 = hexStrToBytes("045B24234234242343242342432424234348DE3ABC8DEAA8D1EB96DB392ED004BC0F1F02AA20F4FB979DD422863BA9FEE1C1B9AEB51D79D51BC67BCB611EF79ADE");	
	std::cout << "Will parse invalid point" << std::endl;	
	try
	{
		const ECPoint pp2 = ECPoint::parsePointBin(bytePoint2, c);
	}
	catch(const std::string& err)
	{
	}




	std::string hexPoint("045B3EA0A716142474E91ADA25910934E948DE3ABC8DEAA8D1EB96DB392ED004BC0F1F02AA20F4FB979DD422863BA9FEE1C1B9AEB51D79D51BC67BCB611EF79ADE");	
	const ECPoint pp3 = ECPoint::parsePointHex(hexPoint, c);
	if(!ec.verifyECPoint(pp3))
	{
		std::cout << "The point: " << pp3 << std::endl;
		throw std::string("Error, did not detect incorrect EC point");
	}




		
	const BigInt<256> startPriv1(BigInt<257>::fromHex("1234567890123456789012345678901234567890123456789012345678901234"));
	std::cout << "The priv1    : " << startPriv1 << std::endl;
	std::cout << "The priv1 hex: " << hexStr(startPriv1.getBinaryLong()) << std::endl;
	
	const PrivKey myPrivKey1(startPriv1, true);
	const std::string myAddress1("1P1Fcvrr4vDEHCbnqVgwD4Jyp1CgZefSbS");
	testPrivToAddress(myPrivKey1, myAddress1);



	const BigInt<257> startPriv2(BigInt<257>::fromHex("BEFB5713832D965651FA107A5F2502EAA6F42B72A1C94CEAA83498EC1CE645A1"));
	std::cout << "The priv2    : " << startPriv2 << std::endl;
	std::cout << "The priv2 hex: " << hexStr(startPriv2.getBinaryLong()) << std::endl;


	//const PrivKey myPrivKey2(startPriv2, true);
	//const std::string myAddress2("13BjsEULGYQkyLu93uRYspVXYtyVr83Ycy");
	//testPrivToAddress(myPrivKey2, myAddress2);

	const BigInt<256> startPriv3(BigInt<256>::fromHex("BEFB5713832D965651FA107A5F2502EAA6F42B72A1C94CEAA83498EC1CE645A1"));
	const PrivKey myPrivKey3(startPriv3, false);
	const std::string myAddress3("19vL3SJ4y8M5skJ2VzK9gdN5g2rq4k4QvF");
	testPrivToAddress(myPrivKey3, myAddress3);


	const BigInt<256> startPriv4(BigInt<256>::fromHex("BB5FA22F82FEFF433EE20E646285348CAAF69AE9E25DF02C00D72D9DA40A9C8E"));
	const PrivKey myPrivKey4(startPriv4, false);
	const Pkif myPkif4("7TNNAM1DB7CWUm5zhm4ZddPwwrza6BiY7haU7croLo4LjxkMtwk");
	testPrivToPkif(myPrivKey4, myPkif4);


		
	const std::string uncomp1("049B0E7A1454BDB2687DB0C3582AFC75F4A353D75BEE42C17C4992D9E5A48770C370365C5AECB2103FF31363CD8E8356190A33F291F0923C9296C182F89D55C944");
	const PubKey pk1 = PubKey::fromHex(uncomp1);
	//const ECPoint pubKey1 = PubKey::not_used_fromHexString(uncomp1);
	const std::string compStr1 = hexString(pk1.point.getRaw(true)); //getCompressedHex();	

	const PubKey pk2 = PubKey::fromHex(compStr1);
	//const ECPoint pubKey2 = PubKey::not_used_fromHexString(compStr1);	
	const std::string uncomp2 = hexString(pk2.point.getRaw(false)); //getNonCompressedHex();


	const PubKey pk3 = PubKey::fromHex(uncomp2);
	//const ECPoint pubKey3 = PubKey::not_used_fromHexString(uncomp2);
	const std::string compStr2 = hexString(pk3.point.getRaw(true)); //getCompressedHex();	

	std::cout << "Verbose: " << uncomp1 << std::endl;
	std::cout << "Compressed: " << compStr2 << std::endl;

	if(compStr1 != compStr2)
	{
		throw std::string("Error, compressed keys differ");
	}
	

	if(uncomp1 != uncomp2)
	{
		throw std::string("Error, uncompressed keys differ");
	}

	if(pk1.point != pk2.point)
	{
		throw std::string("Error, uncompressed points differ");
	}
		
	std::cout << "Will test mini private key" << std::endl;	
		
	const BitcoinKeyPair minikeypair = convertMiniPriv("S6c56bnXQiBjk9mqSYE7ykVQ7NzrRy", false);
	if(minikeypair.pubaddr.toString() /*.address*/ != "1CciesT23BNionJeXrbxmjc7ywfiyM4oLW")
	{
		std::cout << "Address: " << minikeypair.pubaddr.toString() /*.address*/ << std::endl;
		throw std::string("Mini priv test failed");			
	}
		
		
	//std::cout << "------------Will check mini private key result" << std::endl;	
		
	if(minikeypair.privaddr.toString() != "5JPy8Zg7z4P7RSLsiqcqyeAF1935zjNUdMxcDeVrtU1oarrgnB7")
	{
		std::cout << "Privkey: " << minikeypair.privaddr.toString() << std::endl;			
		throw std::string("Mini priv test failed");						
	}
		
	std::cout << "Testing mnemonics" << std::endl;

		
	const BigInt<256> firstTestVector(BigInt<256>::fromHex("39489384938478162378126313123123123222178236183617826772727727f2"));
	//const std::string testMnemonic = generateMnemonic(firstTestVector);
	const Mnemonic myMnemonicObj(firstTestVector); // = Mnemonic::generate(firstTestVector);
	const std::string testMnemonic = myMnemonicObj.toStr();
	
	
	std::cout << "The test mnemonic: " << testMnemonic << std::endl;
		
	const Mnemonic mnemonicObj2(testMnemonic/*, false*/);	
	const BigInt<256> secondTestVector = mnemonicObj2.toPrivKey();
		
	if(firstTestVector != secondTestVector)
	{	
		std::cout << "First test vector : " << firstTestVector << std::endl;
		std::cout << "Second test vector: " << secondTestVector << std::endl;
		
		throw std::string("Mnemonic test 1 failed");
	}
		

                                             
	const BigInt<256> thirdTestVector(BigInt<256>::fromHex("54534547774888382992999abbacbbacbabcbabbabbbbcbbbbb7a8a787a8887"));
	const Mnemonic mnemonicObj(thirdTestVector); //Mnemonic::generate(thirdTestVector));	

	const BigInt<256> fourthTestVector = mnemonicObj.toPrivKey();
		
	if(thirdTestVector != fourthTestVector)
	{						
		//std::cout << "Third: " << thirdTestVector << std::endl;
		//std::cout << "Fourth: " << fourthTestVector << std::endl;
			
		throw std::string("The Mnemonic test 2 failed");
	}



	const BigInt<256> testVector1(BigInt<256>::fromHex("84753847584573487587877777777a7a7a7a77c7c7c77b77b777b7b7b788f88f"));
	//const std::string testResult1 = generateMnemonic(testVector1);
	const Mnemonic myMnemonicObj1(testVector1); // = Mnemonic::generate(testVector1);
	const std::string testResult1 = myMnemonicObj1.toStr();
	
	 
	if(testResult1 != "love prefer balcony rack fire picnic stock valley jazz roof teach visual police desk business wedding kiwi sad target hunt kiwi settle timber vote")
	{
		std::cout << "Result: " << testResult1 << std::endl;
		throw std::string("Mnemonic test1 failed");
	}
        
	const BigInt<256> testVector2(BigInt<256>::fromHex("2389423894234234237489237489738738388383838472394723894789237487"));
	//const std::string testResult2 = generateMnemonic(testVector2);
	const Mnemonic myMnemonicObj2(testVector2); // = Mnemonic::generate(testVector2);
	const std::string testResult2 = myMnemonicObj2.toStr();
	
	
	std::cout << "Test result 2: " << std::endl << testResult2 << std::endl;		
	if(testResult2 != "castle energy mixed choose crouch borrow miss muffin casino picture friend attack logic dove asthma identify impulse sketch similar barely joy museum spirit topic")
	{
		throw std::string("Mnemonic test 3 failed");
	}


	const BigInt<256> testVector3(BigInt<256>::fromHex("ababababbabbabbabbabbababbbababbbbabbbababbbbabababababababbabba"));
	//const std::string testResult3 = generateMnemonic(testVector3);
	const Mnemonic myMnemonicObj3(testVector3); // = Mnemonic::generate(testVector3);
	const std::string testResult3 = myMnemonicObj3.toStr();
	
	if(testResult3 != "produce front turtle install rival tail upgrade jar rival tape push jazz rival tape push jazz rival still push install produce fruit puzzle final")
	{
		std::cout << "Result: " << testResult3 << std::endl;
			
		throw std::string("Mnemonic test3 failed");
	}

    const BigInt<256> testVector4(BigInt<256>::fromHex("3e141609b97933b66a060dcddc71fad1d91677db872031e85f4c015c5e7e8982"));
    //const std::string testResult4 = generateMnemonic(testVector4);
	const Mnemonic myMnemonicObj4(testVector4); // = Mnemonic::generate(testVector4);
	const std::string testResult4 = myMnemonicObj4.toStr();
    
    
    if(testResult4 != "dignity pass list indicate nasty swamp pool script soccer toe leaf photo multiply desk host tomato cradle drill spread actor shine dismiss champion exotic")
    {
		throw std::string("Mnemonic test4 failed");
	}


		
		
	const BigInt<256> testVector5(BigInt<256>::fromHex("0000000000000000000000000000000000000000000000000000000000000000"));
	//const std::string testResult5 = generateMnemonic(testVector5);
	const Mnemonic myMnemonicObj5(testVector5); // = Mnemonic::generate(testVector5);
	const std::string testResult5 = myMnemonicObj5.toStr();
	
	
	if(testResult5 != "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon art")
	{
		throw std::string("Mnemonic test 4 failed");
	}
		
 		
	//std::cout << "Should be2: " << "legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth title" << std::endl;

	
		
		
	const BigInt<256> privKeyR(BigInt<256>::fromHex("345345345252354234234234234234234"));
	PrivKey priv(privKeyR, compressed);
	//std::cout << "The root private key: " << priv << std::endl;
		
	const PrivKey myPrivKey(privKeyR, compressed);
		
	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKeyR, compressed);
		
	//Save the public root
	const ECPoint PublicKeyRoot = priv.point(ec);
	//std::cout << "The root public key: " << PublicKeyRoot << std::endl;


	testGenerateAddresses(privKeyR, PublicKeyRoot);
		
		
	//stSign();
		
	const BigInt<256> privKey1(BigInt<256>::fromHex("66473289ABCEF738374837847392392838330093939938848818191111FFAF31"));
	const std::string privVal1("5JbL8znGx6YAKyhMTC2mBgJiZp9vofMud2adz4wEo7e5VwG3ymx");
	const std::string pubVal1("1CvVWcQccwYbQSy2xoFR4fwAqCQ9BK6x5D");		
	testGeneration(privKey1, privVal1, pubVal1);

	const BigInt<256> privKey2(BigInt<256>::fromHex("6466373783738872372873282787AAAA7A7A77B77B7B777C77C7777D876FE123"));
	const std::string privVal2("5JaW9Sdn6aSzbeZ9JpRzmNK23x4J9UtwdM8hDQWRDEYrLi639Vh");
	const std::string pubVal2("1HwxQQ4KBNLF8C5NqFSCw6K8dTJHMK3BvH");
	testGeneration(privKey2, privVal2, pubVal2);

	const BigInt<256> privKey3(BigInt<256>::fromHex("748A88B8CC8EE8F8DD8E8110101239832299928388348374888F888F788888A8"));
	const std::string privVal3("5JhcU967vqV5F5xgaif5BEeh4Ef44Sv1MvBSvteQfZ48ZUfSGxM");
	const std::string pubVal3("1T6Bq5fVEShn4o429CCk1LBV96Qsg1wuJ");		
	testGeneration(privKey3, privVal3, pubVal3);

	const BigInt<256> privKey4(BigInt<256>::fromHex("5B27BE3AA5DEC37AB03C3A370790019966C642FDCB5B800F6DAD11F9990A9229"));
	const std::string privVal4("5JWS27KLkfoNkwrZyJBDURY349ANV8zvo3UZptJpf78z2xeXca4");
	const std::string pubVal4("1JfAc11XPLJoLXN5GMTLzoaeZDWeCGLGmM");		
	testGeneration(privKey4, privVal4, pubVal4);

	const BigInt<256> privKey5(BigInt<256>::fromHex("D8CB36B9628E1BBABA0F47A0E8A16189E03FA379BF2FB41EA8FFD53549D05B77"));
	const std::string privVal5("5KTmHfxNNNJ6KY1YTR1MsMWhAGwPiBubWGDufQBwemjAuAcM1pY");
	const std::string pubVal5("16pNo3yfxz4KzG1aCmso88MSLQb2Cb2nRd");		
	testGeneration(privKey5, privVal5, pubVal5);

	const BigInt<256> privKey6(BigInt<256>::fromHex("EFDD50ADCF5447B5C86DAFE49BCBE8CB4FE836C89321B8CBFEE8C3917BD68249"));
	const std::string privVal6("5KdvbfknT5CsdRc5xjiZuaaVcs2uUbc2WM1uYBM4RwoReskn7rr");
	const std::string pubVal6("12WqGoYkk9u1oQWCWMZZAvL93PBBACahjz");		
	testGeneration(privKey6, privVal6, pubVal6);

	const BigInt<256> privKey7(BigInt<256>::fromHex("2B6675725A9D247BC3C4C963521282B6F3DF2C40E9B5DEED588AFB6C4238C30E"));
	const std::string privVal7("5J9QBWvRTTpGhqMYA1Gf7u44BNLgNUtvC6HfVnbt9GsgEPe4ArQ");
	const std::string pubVal7("1KnUoYwPuLD5jPkrjGh1JMQeBbEZ98f8GT");		
	testGeneration(privKey7, privVal7, pubVal7);


	//std::cout << "Testing new address" << std::endl;
	const BigInt<256> privKey9(BigInt<256>::fromHex("B09BB32A217C403DD2789F624FA4D4B9553B1E4DEE712A9CB00B3C96F3AFB9D2"));
	const std::string privVal9("5KA4oQrWfKAdHVqbWXav8E56KGNKFY7tq4HRfDi2pyPM44bwckP");
	const std::string pubVal9("1AVmSw2XsyoLR8nXuvXJ9339FfaJnGvVke");		
	testGeneration(privKey9, privVal9, pubVal9);

	const BigInt<256> privKey10(BigInt<256>::fromHex("4B9C559339226D81446742681D722D73569B27236CF59A05547ACD328F75D0FA"));
	const std::string privVal10("5JPaxLS7iCVb2t8kyfpwi7NPGKvykaMHJWvmTWcqMdLE3EmnYDM");
	const std::string pubVal10("1MV8yCVPMD2iVreWESWuhc2xHQhE4Ywrws");		
	testGeneration(privKey10, privVal10, pubVal10);

	const BigInt<256> privKey11(BigInt<256>::fromHex("068970DA52B42718BDA97CD39C3DA05E8EF89EDD2D1E6096916075D59EF51D2B"));
	const std::string privVal11("5HsAZGykXbFp2o43METQbEXeprk8e8kwXkdK7mjMbyegPQ7fR3f");
	const std::string pubVal11("1LM72wDRT6CSKY3Z1E4mBSfmrFM9xMrXDV");		
	testGeneration(privKey11, privVal11, pubVal11);

	const BigInt<256> privKey12(BigInt<256>::fromHex("928B4B1C53EB2AFCD8EBAFECBD0DE74B2CF4A19895275CF6041D7D157F615AE6"));
	const std::string privVal12("5JvprVv8r82hgtcyxBEN3ne3YJhLHezLyK1YYchs3PKJctaw6qM");
	const std::string pubVal12("122LxttmgoFsF3Z6TizWGgaJqU8721Jpnu");		
	testGeneration(privKey12, privVal12, pubVal12);

	const BigInt<256> privKey13(BigInt<256>::fromHex("3E9E8446EBAD18D5BEF6ECB1FEB832224BDC8F281A2DFD675B6E9C282035EC28"));
	const std::string privVal13("5JHs73jysuaUAZ33tQ3spv44aqzmJ8F7Z79qbuifALkJU7JmcWT");
	const std::string pubVal13("1NQ8Bw1CCDqQMNAfzJKC9XpyyratsZRg7V");
	testGeneration(privKey13, privVal13, pubVal13);

	const BigInt<256> privKey14(BigInt<256>::fromHex("F3873BEBAE390A703DAE7C03CECA8F099EEE1CBD60743B1FE49329D38DAB95B9"));
	const std::string privVal14("5KfYBdGciaL8ZcvuTm6BnW3e7hWfbbCpdw6Wj7CNBLSxjGScTPA");
	const std::string pubVal14("1GtZKkRNyVWayNvgMutLjnJyJiyRQa2xLo");
	testGeneration(privKey14, privVal14, pubVal14);


	const BigInt<256> privKey15(BigInt<256>::fromHex("11FC7A447FED94CC8FDA4AA1396248980D80D268274C19AD0BD5649C7014758E"));
	const std::string privVal15("5HxD1shLjQKQxPgzfyy5WK87EMMgpXXS34KjVAdvjTEFbpMdfKr");
	const std::string pubVal15("1Af5oVHriPGSSbJtVxV8bK58ABoQ9owtAi");
	testGeneration(privKey15, privVal15, pubVal15);

	const BigInt<256> privKey16(BigInt<256>::fromHex("D12BD3B87FAF4F52EA7471DF9C27513B07286B99B0E8301FB1EA482B7EFBDBB2"));
	const std::string privVal16("5KQQaYD4a6LtXj9DPRkUv7xkyguq7KP3o5g8RoLDkaWSfdEmfiR");
	const std::string pubVal16("1PYx69pGo74e5bqL8JEBDwTjFi2UaUSGby");
	testGeneration(privKey16, privVal16, pubVal16);

	const BigInt<256> privKey17(BigInt<256>::fromHex("59F659A44E6182867D96D88C8CFD86C35E4BB53E06EE555BB9276FD4BD51ED74"));
	const std::string privVal17("5JVuYjF475wiYVN8Ara2oPiMuLZuUog8irDUCyepg1izYH7dwKE");
	const std::string pubVal17("1JB5z6fTgKCNjPaU46i2T97riMVN3imfsw");
	testGeneration(privKey17, privVal17, pubVal17);




	std::cout << "Will test compressed keys" << std::endl;
	const BigInt<256> privKey8(BigInt<256>::fromHex("AF0281E12C7EAD2B1CC0735E779D55B3F5FCFE75EBFC8E892D22E7E2D61F1B33"));
	const std::string privVal8("L35uZaQkgG9L3etwPxQrNtKTfNKPyVhbcPnG78bpr668wjchaBpS");
	const std::string pubVal8("158ijex2ttr27y8QJovghHHJX1PLLUNwNz");		
	testGeneration(privKey8, privVal8, pubVal8);




	testSign(Wif(privVal1));
	testSign(Wif(privVal2));
	testSign(Wif(privVal3));
	testSign(Wif(privVal4));
	testSign(Wif(privVal5));
	testSign(Wif(privVal6));
	testSign(Wif(privVal7));


	BigInt<256> val(123456789);
	std::cout << "First: " << val << std::endl;

	const std::string encoded = Base58Encode(val);
	std::cout << "Encoded: " << encoded << std::endl;
		
	const BigInt<256> decoded = Base58Decode<256>(encoded);
	std::cout << "Decoded: " << decoded << std::endl;
	
	if(val == decoded)
	{
		std::cout << "Values equal" << std::endl;			
	}
	else
	{
		std::cout << "ERROR, values NOT equal" << std::endl;			
	}

	testAddressGen();
}




