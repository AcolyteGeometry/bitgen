#ifndef ENCRYPTED_RESULT_H
#define ENCRYPTED_RESULT_H

#include "bitcoinEC.h"

class EncryptedResult
{
public:
	EncryptedResult(const ECPoint inPublicPoint, const ECPoint inSecretPoint) :
		publicPoint(inPublicPoint), secretPoint(inSecretPoint) {}

	static EncryptedResult encrypt(const BigInt<1024>& random, const ECPoint& publicPoint);
	static ECPoint decypt(const ECPoint& R, const BigInt<1024>& privKey);

		
	const ECPoint publicPoint;
	const ECPoint secretPoint;
};



#endif
