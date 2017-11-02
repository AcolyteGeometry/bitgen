#ifndef BIG_INT_H
#define BIG_INT_H

/**
 * bigint - Big integer class
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

#include <iostream>


class BigIntInternalTypes
{
public:
#if __x86_64__ || __ppc64__
	typedef unsigned long long intType;
	typedef long long intTypeSigned;
	typedef __uint128_t longType;
#else
	typedef unsigned int intType;
	typedef int intTypeSigned;
	typedef unsigned long long longType;
#endif	
};


template<int NUM_BITS>
class BigInt
{
public:

//Constants
	//typedef unsigned int intType;
	
 	typedef BigIntInternalTypes::intType intType;
 	typedef BigIntInternalTypes::intTypeSigned intTypeSigned;
 	typedef BigIntInternalTypes::longType longType;

private:	
	
	const static int intTypeSize = sizeof(intType) * 8;


	const static int NUM_INTS = NUM_BITS / intTypeSize + ( (NUM_BITS % intTypeSize) > 0 ? 1 : 0);
	const static int NUM_BYTES = NUM_INTS * sizeof(intType);

	const static int NUM_BYTES_NEEDED = NUM_BITS / 8 + ( (NUM_BITS % 8) > 0 ? 1 : 0);


	const static int num_ints = NUM_INTS;

public:
	static const char* version() { return "0.7.4"; }

	const static int specifiedNumBits = NUM_BITS;
	static unsigned int numBits() { return NUM_BITS; }
	static unsigned int maxBits() { return NUM_INTS * intTypeSize; }

	int numberBitsNeeded() const;	//Total number bits USE FOR BIT STORAGE CALCULATION	

	bool tooLarge() const;


	static BigInt fromBytes(const std::string& str);
	static BigInt fromHex(const std::string& str);
	static BigInt fromDec(const std::string& str);
	static BigInt onePattern();					//Generates a number which in binary is only ones


	static BigInt maxPosVal()
	{
		BigInt<NUM_BITS> maxVal;
		for(int i = 0 ; i < (NUM_BITS -1) ; i++)
		{
			maxVal.setBit(i);		//Todo: Implement more efficiently!
		}
		
		return maxVal;
	}


	static BigInt minNegVal()
	{
		BigInt zero;
		BigInt one(1);
		
		BigInt val = zero - maxPosVal() - one;
		return val;
	}


	void setBit(int bitnum)
	{
		const int arrIndex = bitnum / intTypeSize;
		const int bitIndex = bitnum % intTypeSize;
		intType pattern(1);
		pattern <<= bitIndex;
		val[arrIndex] |= pattern;
	}
	
	/*
	void clearBit(int bitnum)
	{
		const int arrIndex = bitnum / intTypeSize;
		const int bitIndex = bitnum % intTypeSize;
		intType pattern(1);
		pattern <<= bitIndex;
		val[arrIndex] &= pattern;
	}
	*/
	
//Constructors

//Deprecated, use fromHex() and fromDec instead
	BigInt(const char* val);
//private:	

	//Treats the std::string as an arry of bytes for the bignum
	//Deprecated, use fromBytes() instead
	BigInt(const std::string& str, const char dummy);		//Todo: Make private
public:
	BigInt();	//Initialize to zero
	BigInt(const BigInt& other);	
	BigInt(int v);

private:
	BigInt(intType primearr[], int size);//This is machine dependent, do not use in client code

public:
	template <int OTHER_SIZE>
	static BigInt<NUM_BITS> asPositive(const BigInt<OTHER_SIZE>& other)
	{
		BigInt<NUM_BITS> ret;
		int min = (NUM_INTS < other.NUM_INTS) ? NUM_INTS : other.NUM_INTS;
		
		for(int i = 0 ; i < min ; i++)
		{
			ret.val[i] = other.val[i];
		}
		
		return ret;	
	}


	template <int OTHER_SIZE>
	explicit BigInt(const BigInt<OTHER_SIZE>& other)
	{	
		int min = (NUM_INTS < other.NUM_INTS) ? NUM_INTS : other.NUM_INTS;
		
		for(int i = 0 ; i < min ; i++)
		{
			val[i] = other.val[i];
		}


		if(other.isNegative())
		{
			intType ones(0);
			ones = ~ones;
			for(int i = min ; i < NUM_INTS ; i++)
			{
				val[i] = ones;		//Pad ones
			}
		}
		else
		{		
			for(int i = min ; i < NUM_INTS ; i++)
			{
				val[i] = 0;			//Pad zeroed
			}		
		}
	}


	//Todo: Move implementation out of class
	//Gets the bit at given position. 0 if the "rightmost" bit (least significant)
	bool operator[](unsigned int pos) const
	{
		const unsigned int byteNum = pos / intTypeSize;
		const unsigned int bit = pos % intTypeSize;
		
		const intType thisbyte = val[byteNum];
		
		return ((thisbyte >> bit) & 0x01);
	}

	

	std::string strHex() const;
	std::string strDec(const int minDigits = 1) const;
	
	
		
	//These functions seturns the number bytes in MSB to LSB order. Returns multiples of 8 bits
	
	
	//Returns the number with as few bytes as possible
	
	std::string getBinaryShortest() const;
	
	//Alias for getBinaryShort()
	std::string getBinary() const
	{
		return getBinaryShort();
	}
	
	std::string getBinaryShort() const
	{
		return getBinaryImplementationStr(false);
	}
	
	//Returns the number of bytes needed for the declared size
	std::string getBinaryLong() const
	{
		return getBinaryVal(true);
	}
	
	//Returns the number of bytes needed for the actual store size
	//This is machine dependent, do not use in portable code	
	std::string getBinaryFull() const
	{
		return getBinaryImplementationStr(true); 
	}
	
	

	bool odd() const 
	{
		return (*this)[0];
	}


	BigInt<NUM_BITS>& operator++();
	BigInt<NUM_BITS> operator++(int);
	
	BigInt<NUM_BITS>& operator--();
	BigInt<NUM_BITS> operator--(int);

	void operator+=(const BigInt& other);
	void operator-=(const BigInt& other);
	void operator*=(const BigInt& other);
	void operator/=(const BigInt& other);
	void operator%=(const BigInt& other);

	void operator+=(const unsigned int& other);
	void operator-=(const unsigned int& other);
	void operator*=(const unsigned int& other);
	void operator/=(const unsigned int& other);
	void operator%=(const unsigned int& other);

	void operator^=(const BigInt& other);
	void operator|=(const BigInt& other);



	void operator<<=(int totalBits);
	void operator>>=(int totalBits);
	
	BigInt<NUM_BITS> operator>>(int totalBits) const;
	BigInt<NUM_BITS> operator<<(int totalBits) const;
	
	
	//Todo: Move out of class
	bool fitInInt() const;	
	int intVal() const		//Trunkates large values
	{
		if(isNegative())
		{
			BigInt<NUM_BITS> zero;
			BigInt<NUM_BITS> posVal = zero - (*this);
//			unsigned int pos = posVal.val[0];
//			int neg = -1 * pos;
 			intType pos = posVal.val[0];
 			intTypeSigned neg = -1 * pos;
			
			return neg;
		}
		else
		{
			return val[0];
		}
	}

	bool isNegative() const
	{
//		const unsigned int myhighestPart = val[NUM_INTS - 1];
 		const intType myhighestPart = val[NUM_INTS - 1];
	
		intType highBitSet(1);
		highBitSet <<= (intTypeSize - 1);

	
		return (myhighestPart & highBitSet);
	}




	//Todo
	/*
	void operator~()
	{
		for(int i = 0 ; i < NUM_INTS ; i++)
		{
			val[i] = ~val[i];
		}
		
		//maskHighBits(numBitToKeep);
	}
*/
	BigInt<NUM_BITS> operator~()
	{
		BigInt<NUM_BITS> ret(*this);
		
		for(int i = 0 ; i < NUM_INTS ; i++)
		{
			ret.val[i] = ~ret.val[i];
		}
		
		//maskHighBits(numBitToKeep);
		return ret;
	}

	
	void operator&=(int numBitToKeep)
	{
		maskHighBits(numBitToKeep);
	}
	

	BigInt<NUM_BITS> operator&(int numBitToKeep) const
	{
		return returnMaskHighBits(numBitToKeep);
	}
	
	BigInt<NUM_BITS> returnMaskHighBits(int numBitToKeep) const;
	

	std::string getLastBytes(const int numBytes) const
{	
	const std::string privBytes = getBinaryLong();
	const int startPos = privBytes.size() - numBytes;
	const std::string priv = privBytes.substr(startPos);
	
	return priv;
}
	

private:
	void maskHighBits(int numBitToKeep);


	std::string getBinaryImplementationStr(bool fullLength = false) const;	//Full actual length
	std::string getBinaryVal(bool fullLength = false) const;				//Specified length


	void shiftRightOneBit();


	unsigned int highestSignificatPart() const;
	intType getVal(int num) const { return val[num]; }


	void fromHexString(const char* str, const int numChars);
	void fromDecimalString(const char* str, const int numChars);

	intType val[NUM_INTS];	//Stores the value!




template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator*(const BigInt<MYNUM_BITS>& me, const int other);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator+(const BigInt<MYNUM_BITS>& me, const int other);

template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator^(const BigInt<MYNUM_BITS>& a, const BigInt<MYNUM_BITS>& b);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator&(const BigInt<MYNUM_BITS>& a, const BigInt<MYNUM_BITS>& b);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator|(const BigInt<MYNUM_BITS>& a, const BigInt<MYNUM_BITS>& b);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator*(const int other, const BigInt<MYNUM_BITS>& me);

template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator%(BigInt<MYNUM_BITS> numerator, const BigInt<MYNUM_BITS> divisor);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator/(BigInt<MYNUM_BITS> numerator, const BigInt<MYNUM_BITS> divisor);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator*(const BigInt<MYNUM_BITS>& me, const BigInt<MYNUM_BITS>& other);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator+(const BigInt<MYNUM_BITS>& me, const BigInt<MYNUM_BITS>& other);
template<int MYNUM_BITS> friend BigInt<MYNUM_BITS> operator-(BigInt<MYNUM_BITS> me, const BigInt<MYNUM_BITS>& other);
template<int MYNUM_BITS> friend bool operator==(const BigInt<MYNUM_BITS>& me, const BigInt<MYNUM_BITS>& other);
template<int MYNUM_BITS> friend bool operator!=(const BigInt<MYNUM_BITS>& me, const BigInt<MYNUM_BITS>& other);
template<int MYNUM_BITS> friend bool operator>=(const BigInt<MYNUM_BITS>& me, const BigInt<MYNUM_BITS>& other);


template<int MYNUM_BITS> friend bool operator==(const BigInt<MYNUM_BITS>& me, const int& other);
template<int MYNUM_BITS> friend bool operator!=(const BigInt<MYNUM_BITS>& me, const int& other);



template<int MYNUM_INTS> friend bool operator>(const BigInt<MYNUM_INTS>& a, const BigInt<MYNUM_INTS>& b);
template<int MYNUM_INTS> friend bool operator<(const BigInt<MYNUM_INTS>& a, const BigInt<MYNUM_INTS>& b);

template<int MYNUM_INTS> friend BigInt<MYNUM_INTS> mulModulus(BigInt<MYNUM_INTS>& me, BigInt<MYNUM_INTS>& other, 
									const BigInt<MYNUM_INTS>& prime);

template<int MYNUM_INTS> friend class BigInt;
template<int MYNUM_INTS> friend class UnsignedBigInt;

template<int MYNUM_INTS> friend std::ostream& operator<<(std::ostream& str, const BigInt<MYNUM_INTS>& obj);

};




template<int NUM_BITS> BigInt<NUM_BITS> operator^(const BigInt<NUM_BITS>& a, const BigInt<NUM_BITS>& b)
{
	BigInt<NUM_BITS> ret;
	for(int i = 0 ; i < a.NUM_INTS ; i++)
	{
		ret.val[i] = a.val[i] ^ b.val[i];
	}

	//Todo: Zero for the highest (non-used) bits?

	return ret;
}



template<int NUM_BITS> BigInt<NUM_BITS> operator&(const BigInt<NUM_BITS>& a, const BigInt<NUM_BITS>& b)
{
	BigInt<NUM_BITS> ret;
	for(int i = 0 ; i < a.NUM_INTS ; i++)
	{
		ret.val[i] = a.val[i] & b.val[i];
	}

	//Todo: Zero for the highest (non-used) bits?

	return ret;
}

template<int NUM_BITS> BigInt<NUM_BITS> operator|(const BigInt<NUM_BITS>& a, const BigInt<NUM_BITS>& b)
{
	BigInt<NUM_BITS> ret;
	for(int i = 0 ; i < a.NUM_INTS ; i++)
	{
		ret.val[i] = a.val[i] | b.val[i];
	}

	//Todo: Zero for the highest (non-used) bits?

	return ret;
}





template<int NUM_BITS>
bool BigInt<NUM_BITS>::fitInInt() const
{
		//Fits if all ints except the last is  all 0 for positive
		//or all 1 for negative
		if(isNegative())
		{
			intType ones(0);
			ones = ~ones;

			for(int i = 1 ; i < NUM_INTS ; i++)
			{
				if(val[i] != ones)
				{
					return false;
				}
			}
			
			return true;

		}
		else
		{
			for(int i = 1 ; i < NUM_INTS ; i++)
			{
				if(val[i] != 0)
				{
					return false;
				}
			}
			
			return true;
			
		}		
}




//Returns the index of the first integer that in non-zero
template<int NUM_BITS>
void BigInt<NUM_BITS>::operator|=(const BigInt<NUM_BITS>& other)
{
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		val[i] = val[i] | other.val[i];
	}	
}




//Returns the index of the first integer that in non-zero
template<int NUM_BITS>
	unsigned int BigInt<NUM_BITS>::highestSignificatPart() const
	{
		//int firstNonZero(0);
		for(int i = NUM_INTS - 1 ; i >= 0 ; i--)
		{
			if(val[i] != 0)
			{
				return i;
			}
		}
	
		return 0;
	}



template<int NUM_BITS>
	int BigInt<NUM_BITS>::numberBitsNeeded() const
	{
		const int highestPart = highestSignificatPart();
		
		intType highPart = val[highestPart];
		int i = 0 ;
		for(; i < intTypeSize ; i++)
		{
			if(highPart == 0)
			{
				break;
			}
			highPart >>= 1;			
		}
		
		const int numBits = highestPart * intTypeSize + i;
		return numBits;
		
	}


/*
template<int NUM_BITS>
	int BigInt<NUM_BITS>::highestBitsNum() const
	{
		for(int i = (NUM_INTS * 8 * sizeof(intType)) - 1 ; i >= 0 ; i--)
		{
			if((*this)[i] == true)
			{
				return i;		
			}
		}
		
		return 0;
	}
*/





template<int NUM_BITS>
	BigInt<NUM_BITS>::BigInt()	//Initialize to zero
	{
		for(int i = 0 ; i < NUM_INTS ; i++)
		{
			val[i] = 0;
		}
	}





template<int NUM_BITS>
	BigInt<NUM_BITS>::BigInt(const BigInt& other) 
	{
		for(int i = 0 ; i < NUM_INTS ; i++)
		{
			val[i] = other.val[i];
		}
	}





template<int NUM_BITS>
	BigInt<NUM_BITS>::BigInt(int v) 
	{		
		if(v >= 0)
		{
			for(int i = 0 ; i < NUM_INTS ; i++)
			{
				val[i] = 0;
			}
		
			val[0] = v;
		}
		else
		{
			unsigned int positive = v * -1;
			BigInt<NUM_BITS> posVal(positive);
			BigInt<NUM_BITS> zero;
			BigInt<NUM_BITS> negVal = zero - posVal;
			(*this) = negVal;
		}
		
		if(tooLarge())
		{
			std::string errMsg("Too large integer found in ctor");			
		}
	}





template<int NUM_BITS>
	BigInt<NUM_BITS> BigInt<NUM_BITS>::returnMaskHighBits(int numBitToKeep) const
	{
		//std::cout << "VAL0: " << (*this) << std::endl;
		
		BigInt<NUM_BITS> retval(*this);
		const int numPartsToKeep = (numBitToKeep / intTypeSize);
		
		if(numPartsToKeep == NUM_INTS)
		{
			return retval;
		}

		//std::cout << "VAL1: " << retval << std::endl;
		

		//std::cout << "Will set zero: " << std::endl;
		//std::cout << "The intsize: " << intTypeSize << std::endl;
		
		for(int i = (numPartsToKeep + 1) ; i < NUM_INTS ; i++)
		{
			//std::cout << "Setting zero: " << i << std::endl;
			retval.val[i] = 0;
		}
		
		//std::cout << "VAL2: " << retval << std::endl;
		
		const int numBitsToKeepInLastInt = numBitToKeep % intTypeSize;
		if(numBitsToKeepInLastInt == 0)
		{
			//std::cout << "Setting this zero: " << numPartsToKeep << std::endl;
			
			retval.val[numPartsToKeep] = 0;
		}
		else
		{
			intType ones(0);
			ones = ~ones;
			//std::cout << "ones1: " << ones << std::endl;
			//std::cout << "ones2: " << 0xffffffff << std::endl;
			
			const int numBitsToRemove = intTypeSize - numBitsToKeepInLastInt;
			//std::cout << "Changing: " << numPartsToKeep << std::endl;
			
			retval.val[numPartsToKeep] &= (ones >> numBitsToRemove);
		}
		
		//std::cout << "VAL3: " << retval << std::endl;
		
		return retval;
	}




template<int NUM_BITS>
	void BigInt<NUM_BITS>::maskHighBits(int numBitToKeep)
	{		
		const int numPartsToKeep = (numBitToKeep / intTypeSize);

		if(numPartsToKeep == NUM_INTS)
		{
			return;
		}
		
		
		for(int i = (numPartsToKeep + 1) ; i < NUM_INTS ; i++)
		{
			val[i] = 0;
		}
		
		
		const int numBitsToKeepInLastInt = numBitToKeep % intTypeSize;		
		if(numBitsToKeepInLastInt == 0)
		{
			val[numPartsToKeep] = 0;
		}
		else
		{
			const int numBitsToRemove = intTypeSize - numBitsToKeepInLastInt;
			
			//Todo: Do not use constant 0xfffffff
			intType ones(0);
			ones = ~ones;

			
			val[numPartsToKeep] &= (ones >> numBitsToRemove);
		}		
	}












template<int NUM_BITS>
//std::string hexStr(unsigned int val, bool& isZero)
std::string hexStr(BigIntInternalTypes::intType val, bool& isZero)
{
	std::string ret;
	//int numNonPrinted(0);

//	for(int i = sizeof(unsigned int) * 2 - 1 ; i >= 0 ; i--)
 	for(int i = sizeof(BigIntInternalTypes::intType) * 2 - 1 ; i >= 0 ; i--)
	{
		unsigned int shifted = (val >>  i * 4) & 0x0f;
		
		if(shifted != 0)
		{
			isZero = false;
		}
		
		char c;
		if(shifted >= 0 && shifted <= 9)
		{
			c = ('0' + shifted);
		}
		else
		{
			c = ('A' + shifted - 10);
		}
		
		if(!isZero)
		{
			ret = ret + c;
		}
	}


		
	return ret;
}





template <int NUM_BITS>
std::string BigInt<NUM_BITS>::getBinaryShortest() const
{
	std::string ret;
	BigInt<NUM_BITS> val = (*this);
	const BigInt<NUM_BITS> zero;
	
	if(val == zero)
	{
		ret += '\0';
		return ret;		//Always return at least one byte
	}
	
	
	while(val != zero)
	{		
		const unsigned int intVal = val.val[0] & 0x0f;
		const unsigned char v1 = intVal;		
		val >>= 4;
		
		const unsigned int intVal2 = val.val[0] & 0x0f;
		const unsigned char v2 = intVal2;		
		val >>= 4;
		
		const unsigned char res = v1 | (v2 << 4) ;
		std::string rr(1, res);
		
		ret = rr + ret;
		
	}
		
	return ret;	
}




template <int NUM_BITS>
std::string BigInt<NUM_BITS>::getBinaryImplementationStr(bool fullLength) const
{
	std::string ret;
	BigInt<NUM_BITS> val = (*this);
	const BigInt<NUM_BITS> zero;
	
	if(val == zero)
	{
		ret += '\0';
		return ret;		//Always return at least one byte
	}
	
	for(int i = 0 ; i < NUM_BYTES ; i++) 
	{
		if(!fullLength && val == zero)
		{
			break;
		}
		
		const unsigned int intVal = val.val[0] & 0x0f;
		const unsigned char v1 = intVal;		
		val >>= 4;
		
		const unsigned int intVal2 = val.val[0] & 0x0f;
		const unsigned char v2 = intVal2;		
		val >>= 4;
		
		const unsigned char res = v1 | (v2 << 4) ;
		std::string rr(1, res);
		
		ret = rr + ret;
		
	}
		
	return ret;	
}



template <int NUM_BITS>
std::string BigInt<NUM_BITS>::getBinaryVal(bool fullLength) const
{
	std::string ret;
	BigInt<NUM_BITS> val = (*this);
	const BigInt<NUM_BITS> zero;
		
	for(int i = 0 ; i < NUM_BYTES_NEEDED ; i++) 
	{
		if(!fullLength && val == zero)
		{
			break;
		}
		
		const unsigned int intVal = val.val[0] & 0x0f;
		const unsigned char v1 = intVal;		
		val >>= 4;
		
		const unsigned int intVal2 = val.val[0] & 0x0f;
		const unsigned char v2 = intVal2;		
		val >>= 4;
		
		const unsigned char res = v1 | (v2 << 4) ;
		std::string rr(1, res);
		
		ret = rr + ret;
		
	}
		
	return ret;	
}



template <int NUM_BITS>
bool BigInt<NUM_BITS>::tooLarge() const
{
	if(numberBitsNeeded() > numBits())
	{
		return true;
	}
	
	return false;
}






template <int NUM_BITS>
std::string BigInt<NUM_BITS>::strDec(const int minDigits) const
{	
	std::string ret;
	
	const BigInt<NUM_BITS>& self = (*this);
	const BigInt<NUM_BITS> zero;


	//if(self == zero)
	//{
	//	return "0";
	//}	
	//else 
	
	if(self.isNegative())
	{
		ret += "-";
		const BigInt<NUM_BITS> zero;
		const BigInt<NUM_BITS> pos = zero - self;
		
		ret += pos.strDec();
		return ret;
	}


		
	
	
	
	BigInt<NUM_BITS> val(*this);
	

	
	
	const BigInt<NUM_BITS> ten(10);
	
	
	while(val != zero)
	{
		//std::cout << "a" << std::endl;		
		const BigInt<NUM_BITS> remainder = val % ten;
		//std::cout << "b" << std::endl;		
		const int rem = remainder.intVal();
		//std::cout << "c" << std::endl;
		
		const char c = ('0' + rem);
		ret = c + ret;
		//std::cout << "d" << std::endl;
		
		val /= ten;
		//std::cout << "e" << std::endl;
	}
	
	while(ret.size() < minDigits)
	{
		ret = '0' + ret;		
	}
	
	return ret;	
}




/*
//Todo: Print only NUM_BYTES bytes
template <int NUM_BITS>
std::string BigInt<NUM_BITS>::strHex() const
{
	std::string ret;
	bool isZero(false);		//Get ALL digits
	for(int i = NUM_INTS - 1 ; i >= 0 ; i--)
	{
		ret += hexStr<NUM_BITS>(val[i], isZero);
	}
	
	return ret;	
}
*/


template <int NUM_BITS>
std::string BigInt<NUM_BITS>::strHex() const
{
	std::string ret;
	const BigInt<NUM_BITS>& self = (*this);
	
	if(self.isNegative())
	{
		ret += "-";
		const BigInt<NUM_BITS> zero;
		const BigInt<NUM_BITS> pos = zero - self;
		
		ret += pos.strHex();
		return ret;
	}
	
	
	//std::cout << "Num ints: " << obj.NUM_INTS << std::endl;
	std::string s;
	bool isZero(true);
	for(int i = self.NUM_INTS - 1 ; i >= 0 ; i--)
	{
		//std::cout << "Iterating: " << i << std::endl;
		s += hexStr<NUM_BITS>(self.val[i], isZero);
	}
	
	if(s.empty())
	{
		ret += "0";
	}
	else
	{
		ret += s;
	}
	
	return ret;
}






template<int NUM_BITS>
std::ostream& operator<<(std::ostream& str, const BigInt<NUM_BITS>& obj)
{
	if(obj.isNegative())
	{
		str << "-";
		const BigInt<NUM_BITS> zero;
		const BigInt<NUM_BITS> pos = zero - obj;
		
		str << pos;
		return str;
	}
	
	if(obj.numberBitsNeeded() > obj.numBits())
	{
		str << "TooLarge";
		return str;
	}
	
	//std::cout << "Num ints: " << obj.NUM_INTS << std::endl;
	std::string s;
	bool isZero(true);
	for(int i = obj.NUM_INTS - 1 ; i >= 0 ; i--)
	{
		//std::cout << "Iterating: " << i << std::endl;
		s += hexStr<NUM_BITS>(obj.val[i], isZero);
	}
	
	if(s.empty())
	{
		str << "0";
	}
	else
	{
		str << s;
	}
	
	return str;
}





//Todo: This is machine dependent, do not use in client code
template<int NUM_BITS>
BigInt<NUM_BITS>::BigInt(intType primearr[], int size)
{
	//const int size = sizeof(primearr);
	if(size > NUM_INTS)
	{
		throw std::string("Error, size out of bounds");
	}
		
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		if(i < size)
		{
			val[i] = primearr[size - i - 1];
		}
		else
		{
			val[i] = 0;
		}
	}
}




template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::fromBytes(const std::string& str)
{
	BigInt<NUM_BITS> ret;
	
	//BigInt<NUM_BITS> twofivesix(256);
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		ret <<= 8;
		
		const unsigned char val = str[i];	
		BigInt<NUM_BITS> v(val);
		ret |= v;
	}
	
	return ret;
}



template <int NUM_BITS>
BigInt<NUM_BITS>::BigInt(const std::string& str, const char dummy)
{
	//std::cout << "Using " << std::endl;
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		val[i] = 0;		
	}
	
	BigInt<NUM_BITS> twofivesix(256);	//Todo: Not needed
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		const unsigned char val = str[i];
		
		BigInt<NUM_BITS> v(val);
		//(*this) <<= 8;
		(*this) *= twofivesix;	//Todo: Use shift instead		
		(*this) += v;			//Todo: Use bitwis or instead		
	}
	
}




template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::onePattern()
{
	BigInt ret;

	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		
		ret.val[i] = ~0; //Set all bits to one   //0xffffffff;	//Todo: Do not use constant
	}

	ret &= NUM_BITS;
	
	return ret;
}




template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::fromHex(const std::string& str)
{
	if(str.empty())
	{
		throw std::string("Error, empty hex number");
	}
	
	const char firstChar = str[0];
	if(firstChar == '-')
	{
		const std::string rest = str.substr(1);
		const BigInt<NUM_BITS> zero;
		const BigInt<NUM_BITS> posVal = fromHex(rest);
		const BigInt<NUM_BITS> negVal = zero - posVal;
		return negVal;
	}
	
	
	BigInt<NUM_BITS * 2> ret;
	unsigned int digitNum(0);
	for(int i = (str.size() - 1) ; i >= 0 ; i--)
	{
		const unsigned char c = str[i];
		if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
		{
			std::string errMsg("Error, digit out of range: ");
			errMsg += c;
			errMsg += " in ";
			errMsg += str;
			
			//std::cout << "Error, digit out of range: " << c << std::endl;
			throw errMsg; //std::string("Out of range");
		}
		unsigned int d = (c >= '0' && c <= '9') ? (c - '0') : ((c >= 'a' && c <= 'f') ? (10 + c - 'a') : (10 + c - 'A'));
		
		//std::cout << "Adding: " << d << std::endl;
		BigInt<NUM_BITS * 2> valuetoadd(d);
		//std::cout << "First val: " << valuetoadd << std::endl;
			
		valuetoadd <<= (4 * digitNum);
		//std::cout << "Now we are adding: " << valuetoadd << " with digitnum: " << digitNum << std::endl;
		
		ret += valuetoadd;

		digitNum++;
		
		if(ret.numberBitsNeeded() > NUM_BITS)
		{
			std::string errMsg("Hex string too large: ");
			errMsg += str;
			throw errMsg; //std::string("Hex string too large");
		}
	}

	BigInt ret2(ret);


	return ret2;
}



template <int NUM_BITS>
void BigInt<NUM_BITS>::fromHexString(const char* str, const int numChars)
{
		unsigned int digitNum(0);
		for(int i = (numChars - 1) ; i >= 0 ; i--)
		{
			const unsigned char c = str[i];
			if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
			{
				std::cout << "Error, digit out of range: " << c << std::endl;
				throw std::string("Out of range");
			}
			unsigned int d = (c >= '0' && c <= '9') ? (c - '0') : ((c >= 'a' && c <= 'f') ? (10 + c - 'a') : (10 + c - 'A'));
		
			//std::cout << "Adding: " << d << std::endl;
			BigInt valuetoadd(d);
			//std::cout << "First val: " << valuetoadd << std::endl;
			
			valuetoadd <<= (4 * digitNum);
			//std::cout << "Now we are adding: " << valuetoadd << " with digitnum: " << digitNum << std::endl;
	
	
			(*this) = (*this) + valuetoadd;

			digitNum++;
		}
}



template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::fromDec(const std::string& str)
{
	BigInt<NUM_BITS> val;
	val.fromDecimalString(str.c_str(), str.size());
	return val;
}


template <int NUM_BITS>
void BigInt<NUM_BITS>::fromDecimalString(const char* str, const int numChars)
{
	if(numChars == 0)
	{
		throw std::string("Decimal string error");
	}
	
	
	const char firstChar = str[0];
	if(firstChar == '-')
	{
		BigInt<NUM_BITS> zero;
		BigInt<NUM_BITS> posNum;
		posNum.fromDecimalString(str + 1, numChars - 1);
		
		BigInt<NUM_BITS> negNum = zero - posNum;
		(*this) = negNum;
		return;
	}

	
	//4 extra bits to detect overflow
	const int bitsForConversion = NUM_BITS + 4;
	BigInt<bitsForConversion> ten(10);
	BigInt<bitsForConversion> exponentOfTen(1);

	BigInt<bitsForConversion> res;

	BigInt<NUM_BITS> maxValA(BigInt<NUM_BITS>::maxPosVal());	
	BigInt<bitsForConversion> maxVal(maxValA);
	
	//std::cout << "The maxval: " << maxVal << std::endl;
	
	for(int i = (numChars - 1) ; i >= 0 ; i--)
	{
		const char c = str[i];
		//std::cout << "Parsing digit: " << c << std::endl;
		if(c > '9' || c < '0')
		{
			throw std::string("Error, non-digit");
		}
		unsigned int d = c - '0';
		
		//std::cout << "Creating bigint" << std::endl;
		BigInt<bitsForConversion> digit(d);
		//std::cout << "Multiplying" << std::endl;
	
		const BigInt<bitsForConversion> valuetoadd = digit * exponentOfTen;
		//std::cout << "Adding" << std::endl;
	
		res += valuetoadd;
		//(*this) += addVal; //valuetoadd;
		//std::cout << "Mult2" << std::endl;
		if(res > maxVal)
		{
			throw std::string("constructed value too large");
		}
				
		exponentOfTen = exponentOfTen * ten;	
	}	
	
	const BigInt<NUM_BITS> result(res);
	(*this) = result;
}




//Parses for example:
//-123ABC
//ACA
//-145
//3788
template <int NUM_BITS>
BigInt<NUM_BITS>::BigInt(const char* str)
{
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		val[i] = 0;		
	}
				
	(*this) = fromHex(str);
} 







template <int NUM_BITS>
BigInt<NUM_BITS>& BigInt<NUM_BITS>::operator++()
{
	(*this) = (*this) + BigInt<NUM_BITS>(1);	//Todo: Improve!!!
	return (*this);
}


template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::operator++(int)
{
	BigInt<NUM_BITS> temp = (*this);
	(*this) = (*this) + BigInt<NUM_BITS>(1);
	return temp;
}



template <int NUM_BITS>
BigInt<NUM_BITS>& BigInt<NUM_BITS>::operator--()
{
	(*this) = (*this) - BigInt<NUM_BITS>(1);
	return (*this);
}

template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::operator--(int)
{
	BigInt<NUM_BITS> temp = (*this);
	(*this) = (*this) - BigInt<NUM_BITS>(1);
	return temp;
}







template <int NUM_BITS>
void BigInt<NUM_BITS>::operator+=(const unsigned int& val)
{
	BigInt<NUM_BITS> other(val);
	(*this) = (*this) + other;
}

template <int NUM_BITS>
void BigInt<NUM_BITS>::operator-=(const unsigned int& val)
{
	BigInt<NUM_BITS> other(val);	
	(*this) = (*this) - other;
}


template <int NUM_BITS>
void BigInt<NUM_BITS>::operator*=(const unsigned int& val)
{
	BigInt<NUM_BITS> other(val);		
	(*this) = (*this) * other;
}


template <int NUM_BITS>
void BigInt<NUM_BITS>::operator/=(const unsigned int& val)
{
	BigInt<NUM_BITS> other(val);		
	(*this) = (*this) / other;
}

template <int NUM_BITS>
void BigInt<NUM_BITS>::operator%=(const unsigned int& val)
{
	BigInt<NUM_BITS> other(val);		
	(*this) = (*this) % other;
}



template <int NUM_BITS>
void BigInt<NUM_BITS>::operator^=(const BigInt& other)
{
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		val[i] =  val[i] ^ other.val[i];		
	}	
}



template <int NUM_BITS>
void BigInt<NUM_BITS>::operator+=(const BigInt& other)
{
	(*this) = (*this) + other;
}

template <int NUM_BITS>
void BigInt<NUM_BITS>::operator-=(const BigInt& other)
{
	(*this) = (*this) - other;
}


template <int NUM_BITS>
void BigInt<NUM_BITS>::operator*=(const BigInt& other)
{
	(*this) = (*this) * other;
}


template <int NUM_BITS>
void BigInt<NUM_BITS>::operator/=(const BigInt& other)
{
	(*this) = (*this) / other;
}

template <int NUM_BITS>
void BigInt<NUM_BITS>::operator%=(const BigInt& other)
{
	(*this) = (*this) % other;
}







template<int NUM_BITS>
class UnsignedBigInt : BigInt<NUM_BITS>
{
public:
	UnsignedBigInt(BigInt<NUM_BITS>& bi) : BigInt<NUM_BITS>(bi) {}

	//template<int MYNUM_INTS> friend OutStream& operator<<(OutStream& out, const UnsignedBigInt<MYNUM_INTS>& v);

};








template <int NUM_BITS> 
std::ostream& operator<<(std::ostream& out, const UnsignedBigInt<NUM_BITS>& v)
{	
	const int firstNonZero = v.highestSignificatPart();	

	for(int i = 0 ; i <= firstNonZero ; i++)
	{
		out << HexInt(v[firstNonZero - i], 8) << " ";
	}

	return out;
}



template<int NUM_BITS> 
bool operator==(const BigInt<NUM_BITS>& me, const BigInt<NUM_BITS>& other)
{
	for(int i = 0 ; i < me.NUM_INTS ; i++)
	{
		if(me.val[i] != other.val[i])
		{
			return false;
		}
	}


	return true;
}



template<int NUM_BITS> 
bool operator==(const BigInt<NUM_BITS>& me, const int& otherInt)
{
	const BigInt<NUM_BITS> other(otherInt);
	return me == other;	
}



template<int NUM_BITS> bool operator!=(const BigInt<NUM_BITS>& me, const BigInt<NUM_BITS>& other)
{
	for(int i = 0 ; i < me.NUM_INTS ; i++)
	{
		//std::cout << "Comparing" << std::endl;
		if(me.val[i] != other.val[i])
		{
			//std::cout << "Val: " << me.val[i] << " != " << other.val[i] << std::endl;
			return true;
		}
	}


	return false;
}


template<int NUM_BITS> bool operator!=(const BigInt<NUM_BITS>& me, const int& otherInt)
{
	const BigInt<NUM_BITS> other(otherInt);
	return me != other;
}



template<int NUM_BITS> bool operator>=(const BigInt<NUM_BITS>& me, const BigInt<NUM_BITS>& other)
{
	for(int i = me.NUM_INTS - 1 ; i >= 0 ; i--)
	{
		//std::cout << "Comparing" << std::endl;
		if(me.val[i] < other.val[i])
		{
			//std::cout << "Val: " << me.val[i] << " NOT >= " << other.val[i] << std::endl;
			return false;
		}
	}


	return true;
}













//Todo: remove??? use operator>>=
//   val >>= 1;
template <int NUM_BITS>
void BigInt<NUM_BITS>::shiftRightOneBit()
{
	intType highBitSet(1);
	highBitSet <<= (intTypeSize - 1);
	
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		val[i] >>= 1;
		if(i < (NUM_INTS-1) && val[i+1] % 2 == 1)	//bit is "1" in last position
		{
			val[i] |= highBitSet; //0x80000000;	//Todo!!!!!!!
		}
	}
}




template <int NUM_BITS>
void BigInt<NUM_BITS>::operator>>=(int totalBits)
{
	if(totalBits == 1)	//Not really needed
	{
		shiftRightOneBit();
		return;
	}
	
	(*this) = (*this) >> totalBits;
}








template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::operator>>(int totalBits) const
{

	if(totalBits == 0)		//Compiler bug???
	{
		return (*this);
	}

	BigInt<NUM_BITS> ret(*this);


	//*out << "totalBits: " << totalBits << "\n";

	
	const int numInts = totalBits / intTypeSize;
	//*out << "numInts: " << numInts << "\n";
	const int numBits = totalBits % intTypeSize;
	//*out << "numBits: " << numBits << "\n";


	if(numInts != 0)
	{
		//First shift full ints
		for(int i = 0 ; i < NUM_INTS - numInts ; i++)
		{
			ret.val[i] = ret.val[i + numInts]; 
		}
	}
		
	//zero in the left end
	for(int i = NUM_INTS - numInts ; i < NUM_INTS ; i++)
	{
		ret.val[i] = 0; 
	}


	
	//Now shift the remaining bits
	
	if(numBits != 0)
	{
	intType storage(0);
	
	intType ones(0);
	ones = ~ones;

	
	for(int i = NUM_INTS - 1  ; i >= 0 ; i--)
	{
		const unsigned int bitsToShiftRight = intTypeSize - numBits;
		//Save old value
		intType tempStorage = (ret.val[i]  << bitsToShiftRight) & (ones << bitsToShiftRight);
		
		//Now update this cell
		ret.val[i] >>= numBits;
		ret.val[i] |= storage;
		storage = tempStorage;
	}
	}


	return ret;
}




template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::operator<<(int totalBits) const
{
	if(totalBits == 0)		//Compiler bug???
	{
		return (*this);
	}

	BigInt<NUM_BITS> ret(*this);

	
	const int numInts = totalBits / intTypeSize;
	const int numBits = totalBits % intTypeSize;


	if(numInts != 0)
	{
		//First shift full ints
		for(int i = NUM_INTS - 1 ; i >= numInts ; i--)
		{
			ret.val[i] = ret.val[i - numInts]; 
		}
	}
		
	//zero in the end
	for(int i = numInts - 1 ; i >= 0 ; i--)
	{
		ret.val[i] = 0; 
	}


	
	//Now shift the remaining bits
	
	if(numBits != 0)
	{
	intType storage(0);
	
	intType ones(0);
	ones = ~ones;
	
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		const unsigned int bitsToShiftRight = intTypeSize - numBits;
		intType tempStorage = (ret.val[i] >> bitsToShiftRight) & (ones >> bitsToShiftRight);
		ret.val[i] <<= numBits;
		ret.val[i] |= storage;
		storage = tempStorage;
	}
	}
	
	return ret;
}








template <int NUM_BITS>
void BigInt<NUM_BITS>::operator<<=(int totalBits)
{
	if(totalBits == 0)		//Compiler bug???
	{
		return;
	}

	
	const int numInts = totalBits / intTypeSize;
	const int numBits = totalBits % intTypeSize;


	if(numInts != 0)
	{
		//First shift full ints
		for(int i = NUM_INTS - 1 ; i >= numInts ; i--)
		{
			val[i] = val[i - numInts]; 
		}
	}
		
	//zero in the end
	for(int i = numInts - 1 ; i >= 0 ; i--)
	{
		val[i] = 0; 
	}


	
	//Now shift the remaining bits
	if(numBits != 0)
	{
	intType storage(0);
	
	intType ones(0);
	ones = ~ones;
	
	for(int i = 0 ; i < NUM_INTS ; i++)
	{
		const unsigned int bitsToShiftRight = intTypeSize - numBits;
		intType tempStorage = (val[i] >> bitsToShiftRight) & (ones /*0xffffffff*/ >> bitsToShiftRight);		//Todo!!!!!!!!!
		val[i] <<= numBits;
		val[i] |= storage;
		storage = tempStorage;
	}
	}
}









template <int NUM_BITS>
BigInt<NUM_BITS> modNegative(BigInt<NUM_BITS> numerator, const BigInt<NUM_BITS> divisor)
{
	const int divisorBits = divisor.numberBitsNeeded();
	
	
	const BigInt<NUM_BITS> zero;
	while(numerator < zero)
	{
		//std::cout << "WORKING" << std::endl;
		BigInt<NUM_BITS> numeratorPos = zero - numerator;
		
		const int firstOneBit = numeratorPos.numberBitsNeeded();
		//std::cout << "numeratorBits: " << firstOneBit << std::endl;
		
		const int divisorShift = (firstOneBit - divisorBits);			
		//std::cout << "divisorShift: " << divisorShift << std::endl;

		
		BigInt<NUM_BITS> divisorMultiple(divisor);
		
		if(divisorShift > 0)
		{
			//No need to shift
		divisorMultiple <<= divisorShift;
		
		if(divisorMultiple > numeratorPos)
		{
			divisorMultiple >>= 1; //.shiftRightOneBit();
			
			if(divisorMultiple > numeratorPos)
			{
				std::cout << "Still larger!!!!!!!!: " << divisorMultiple << "\n";
				
				//for(;;)
				//{
				//}
			}
		}
		}

		//Todo: operator-=
		numerator = numerator + divisorMultiple;

	}		
	
	return numerator;
}






template <int NUM_BITS>
BigInt<NUM_BITS> modPositive(BigInt<NUM_BITS> numerator, const BigInt<NUM_BITS> divisor)
{
	const int divisorBits = divisor.numberBitsNeeded();
	
	while(numerator > divisor || (numerator == divisor) )
	{		
		//std::cout << "Working" << std::endl;
		const int firstOneBit = numerator.numberBitsNeeded();
		
		const int divisorShift = (firstOneBit - divisorBits);			
		
		BigInt<NUM_BITS> divisorMultiple(divisor);
		
		if(divisorShift > 0)
		{

		divisorMultiple <<= divisorShift;
		
		if(divisorMultiple > numerator)
		{
			divisorMultiple >>= 1;
			
			if(divisorMultiple > numerator)
			{
				throw std::string("Internal error in modulus");
				//std::cout << "Still larger!!!!!!!!: " << divisorMultiple << "\n";				
			}
		}
		}

		//Todo: operator-=
		numerator -= /*numerator -*/ divisorMultiple;

	}		
		
	return numerator;
}







//The number divisor must be a positive number!!!!!!
template <int NUM_BITS>
BigInt<NUM_BITS> operator%(BigInt<NUM_BITS> numerator, const BigInt<NUM_BITS> divisor)
{
	if(divisor.isNegative())
	{
		std::cerr << "Error, modulos can not divide by negative numbers" << std::endl;
		throw std::string("Error, modulos can not divide by negative numbers");
	}
			
	if(numerator.isNegative())
	{
		return modNegative(numerator, divisor);	
	}
	else
	{
		return modPositive(numerator, divisor);		
	}
}








template <int NUM_BITS>
BigInt<NUM_BITS> operator/(BigInt<NUM_BITS> numerator, const BigInt<NUM_BITS> divisor)
{
	const int divisorBits = divisor.numberBitsNeeded();
	
	BigInt<NUM_BITS> result;
	
	
	while(numerator > divisor || (numerator == divisor) )
	{
		const int firstOneBit = numerator.numberBitsNeeded();
		
		int divisorShift = (firstOneBit - divisorBits);
				
		BigInt<NUM_BITS> divisorCopy(divisor);
		
		divisorCopy <<= divisorShift;
		
		if(divisorCopy > numerator)
		{
			divisorCopy >>= 1;
			divisorShift--;

			
			if(divisorCopy > numerator)
			{
				std::cout << "Still larger!!!!!!!!: " << std::endl;
				throw std::string("Error, still marger");
			}
		}
					
		//BigInt<NUM_BITS> tworaisedToShift(1);
		//tworaisedToShift <<= divisorShift;

		//BigInt<NUM_BITS> tworaisedToShift;
		//tworaisedToShift.setBit(divisorShift);
								
		//result += /*result +*/ tworaisedToShift;

		result.setBit(divisorShift);

		numerator -= /*numerator -*/ divisorCopy;				
	}		


	return result;
}

























template <int NUM_BITS>
BigInt<NUM_BITS> operator*(const BigInt<NUM_BITS>& me, const BigInt<NUM_BITS>& other)
{		
 	//BigIntInternalTypes::intType ones(0);
 	//ones = ~ones;
	
	const int themaxBits = BigInt<NUM_BITS>::NUM_INTS * BigInt<NUM_BITS>::intTypeSize * 2;

	
	BigInt<themaxBits> result;		//Note : Double number of ints
	//std::cout << "RESULT INTS: " << result.NUM_INTS << std::endl;
	
	const int resNumInts = (result.NUM_INTS);
	//std::cout << "resNumInts: " << resNumInts << std::endl;


	const unsigned int meHighestIndex = me.highestSignificatPart();	
	const unsigned int otherHighestIndex = other.highestSignificatPart();

	for(unsigned int i = 0 ; i <= meHighestIndex ; i++)
	{
		//unsigned int upmemory(0);
 		BigIntInternalTypes::intType upmemory(0);
		
		for(unsigned int j = 0 ; j <= otherHighestIndex ; j++)
		{	
			//const unsigned long long a = me.val[i];
			const BigIntInternalTypes::longType a = me.val[i];
			
			//const unsigned long long b = other.val[j];
			const BigIntInternalTypes::longType b = other.val[j];
						
			//const unsigned long long product = a * b + upmemory;			
			const BigIntInternalTypes::longType product = a * b + upmemory;
		
			const int pos = j + i;
			
			//const unsigned long long down_temp = product + (unsigned long long)result.val[pos];
			const BigIntInternalTypes::longType down_temp = product + (unsigned long long)result.val[pos];
			
			//const unsigned int putback = down_temp & 0xffffffff;
			const BigIntInternalTypes::intType putback = down_temp;// & ones;
			
			result.val[pos] = putback;
			
			upmemory = (down_temp >> BigInt<NUM_BITS>::intTypeSize);			
		}
		
		result.val[i + otherHighestIndex + 1] = upmemory;
	}
	
	const BigInt<NUM_BITS> r(result);
	return r;
}





template<int NUM_BITS> BigInt<NUM_BITS> operator*(const int integer, const BigInt<NUM_BITS>& me)
{
	const BigInt<NUM_BITS> theInteger(integer);

	return me * theInteger;
}



template<int NUM_BITS> BigInt<NUM_BITS> operator*(const BigInt<NUM_BITS>& me, const int integer)
{
	const BigInt<NUM_BITS> theInteger(integer);

	return me * theInteger;
}


template<int NUM_BITS> BigInt<NUM_BITS> operator+(const BigInt<NUM_BITS>& me, const int integer)
{
	const BigInt<NUM_BITS> theInteger(integer);

	return me + theInteger;
}





template <int NUM_BITS>
BigInt<NUM_BITS> operator+(const BigInt<NUM_BITS>& me, const BigInt<NUM_BITS>& other)
{
 	BigIntInternalTypes::intType ones(0);
 	ones = ~ones;
	
	BigInt<NUM_BITS> result;

	//unsigned int memory(0);
	BigIntInternalTypes::intType memory(0);

	for(int i = 0 ; i < me.NUM_INTS ; i++)
	{
//		unsigned long long a = me.val[i];
//		unsigned long long b = other.val[i];
 		BigIntInternalTypes::longType a = me.val[i];
 		BigIntInternalTypes::longType b = other.val[i];
	
	
//		unsigned long long sum = a + b + memory;
//		unsigned int low = sum & 0xffffffff; 
 		BigIntInternalTypes::longType sum = a + b + memory;
 		BigIntInternalTypes::intType low = sum & ones; 
	
		result.val[i] = low;
	
		memory = sum >> BigInt<NUM_BITS>::intTypeSize;	
	}
		
	return result;
}







template <int NUM_BITS>
BigInt<NUM_BITS> operator-(BigInt<NUM_BITS> me, const BigInt<NUM_BITS>& other)
{
	BigInt<NUM_BITS> result;

//	unsigned long long borrowArray[me.NUM_INTS];
 	BigIntInternalTypes::longType borrowArray[me.NUM_INTS];
	
	for(int i = 0 ; i < me.NUM_INTS ; i++)
	{
		borrowArray[i] = 0;
	}	

 	BigIntInternalTypes::intType ones(0);
 	ones = ~ones;

	for(int i = 0 ; i < me.NUM_INTS ; i++)
	{	
		
//		unsigned long long borrow = borrowArray[i];
//		unsigned long long positive = me.val[i];
//		unsigned long long negative = other.val[i];
 		BigIntInternalTypes::longType borrow = borrowArray[i];
 		BigIntInternalTypes::longType positive = me.val[i];
 		BigIntInternalTypes::longType negative = other.val[i];
		
		
		if((borrow + positive) < negative)
		{
			//Borrow "the last bit" from next value			
			//bool doBorrow(true);
			
//			borrowArray[i] += 0xffffffff; //Compiler bug????
 			borrowArray[i] += ones; //Compiler bug????
						
			borrowArray[i] += 0x00000001; 
			
			for(int j = i + 1 ; j < (me.NUM_INTS)  ; j++)
			{
			
				if(me.val[j] == 0)				
				{
					//borrowArray[j] += 0xffffffff;
 					borrowArray[j] += ones;											
					//Continue, borrow from next position
				}
				else
				{
					me.val[j]--;
					//doBorrow = false;
					break;
				}				
			
			
			}
		}


//		unsigned long long diff = (borrowArray[i] + me.val[i]) - other.val[i];			
 		BigIntInternalTypes::longType diff = (borrowArray[i] + me.val[i]) - other.val[i];	

//		unsigned int low = 0xffffffff & diff; 
 		BigIntInternalTypes::intType low = ones & diff; 
	
		result.val[i] = low;
	
//		unsigned long long high = diff >> BigInt<NUM_BITS>::intTypeSize;
 		BigIntInternalTypes::longType high = diff >> BigInt<NUM_BITS>::intTypeSize;		
 		
		if(high != 0)
		{
			std::cout << "Error, high not zero: " << int(high) << " i: " << i << "\n";
			throw std::string("Error, high not zero");
		}
	}
		
	return result;
}





template <int NUM_BITS>
bool operator>(const BigInt<NUM_BITS>& a, const BigInt<NUM_BITS>& b)
{
	const bool aNeg = a.isNegative();
	const bool bNeg = b.isNegative();
	
	if(!aNeg && bNeg)
	{
		return true;	//Always
	}
	else if(aNeg && !bNeg)
	{
		return false;	//Always
	}
	else if(aNeg && bNeg)
	{
		//Two negative values
		const BigInt<NUM_BITS> zero(0);
		const BigInt<NUM_BITS> aPositive = zero - a;
		const BigInt<NUM_BITS> bPositive = zero - b;
		return bPositive > aPositive;
	}
	

	//Both positive
	for(int i = a.NUM_INTS - 1 ; i >= 0 ; i--)
	{
		if(a.val[i] > b.val[i])
		{
			return true;
		}
		else if(a.val[i] < b.val[i])
		{
			return false;
		}
	}
	
	return false;
}





template <int NUM_BITS>
bool operator<(const BigInt<NUM_BITS>& a, const BigInt<NUM_BITS>& b)
{
	return b > a;	
}

















#endif


