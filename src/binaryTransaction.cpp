


#include "binaryTransaction.h"

#include "hash.h"
#include "transaction.h"


BinaryTransaction BinaryTransaction::fromHex(const std::string& hexStr)
{
	const std::string trans = convertFromHex(hexStr);
	return BinaryTransaction(trans);
}




std::string BinaryTransaction::hashStr() const
{
	const std::string hashSigned = doubleSha256(transaction);			
	return hexString(Transaction::getReversedStr(hashSigned));
}


bool operator==(const BinaryTransaction& a, const BinaryTransaction& b)
{
	return (a.transaction == b.transaction);
}

bool operator!=(const BinaryTransaction& a, const BinaryTransaction& b)
{
	return !(a == b);
}
