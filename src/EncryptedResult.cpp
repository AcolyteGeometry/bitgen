

#include "EncryptedResult.h"



EncryptedResult EncryptedResult::encrypt(const BigInt<1024>& r, const ECPoint& Q)
{	
	BitcoinEllipticCurve ec;
	
	const ECPoint R = ec.multiplyGenPoint(r);
	//std::cout << "R to publish: " << R << std::endl;
	
	const ECPoint S = ec.multiply(Q, r);	
	//std::cout << "S: " << S << std::endl;
	
	EncryptedResult enc(R, S);
	return enc;	
}


//decrypts the encrypted EC point
ECPoint EncryptedResult::decypt(const ECPoint& R, const BigInt<1024>& privKey)
{
	const BitcoinEllipticCurve ec;
	
	const ECPoint otherS =  ec.multiply(R, privKey);
	return otherS;	
}
