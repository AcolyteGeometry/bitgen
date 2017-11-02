
#include "RSVal.h"
#include "bitcoinEC.h"


bool RSVal::lowS() const
{
	BitcoinEllipticCurve ec;

	const BigInt<1024> nHalf = (ec.n >> 1);
	return !(s > nHalf);		
}

