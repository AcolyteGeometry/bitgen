/**
 * RangeSearcher.cpp
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

#include "RangeSearcher.h"




bool isChar(const char c)
{
	return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}


char toUpper(const char c)
{
	return (c + 'A' - 'a');
}


std::string RangeSearcher::toString() const
{
	std::string ret;
	for(std::vector<Range>::const_iterator it = ranges.begin() ;
		it != ranges.end();
		++it)
	{
		const Range& range = (*it);
		ret += range.min.strHex();
		ret += " to\n";
		ret += range.max.strHex();
		ret += "\n";		
	}
	return ret;
}


BigInt<256> RangeSearcher::rangeSum() const
{
	BigInt<256> sum;
	
	for(std::vector<Range>::const_iterator it = ranges.begin() ;
		it != ranges.end();
		++it)
	{
		const Range& range = (*it);
		const BigInt<256> diff = range.max - range.min;
		sum += diff;
	}
	
	return sum;
}



bool RangeSearcher::isInRange(const BigInt<256>& val) const
{
	//std::cout << val.strHex() << std::endl;
	for(std::vector<Range>::const_iterator it = ranges.begin() ;
		it != ranges.end();
		++it)
	{
		const Range& range = (*it);
		if(val > range.min && val < range.max)
		{
			return true;
		}	
	}
		
	return false;
}





std::vector<std::string> RangeSearcher::permute(const std::string& str)
{
	//std::cout << "permute(" << str << ")" << std::endl;
	std::vector<std::string> ret;
	
	if(str.empty())
	{
		ret.push_back("");
		return ret;
	}
		
		
		
	const char first = str[0];
	const std::string rest = str.substr(1);
	
	
	const std::vector<std::string> restPermute = permute(rest);
	
	
	
	for(std::vector<std::string>::const_iterator it = restPermute.begin();
		it != restPermute.end();
		++it)
	{
		const std::string& str = (*it);
		if(isChar(first))
		{
			const char upperChar = toUpper(first);
			const std::string lowercharStr = first + str;
			ret.push_back(lowercharStr);

			const std::string uppercharStr = upperChar + str;
			ret.push_back(uppercharStr);				
		}
		else
		{
			const std::string noncharStr = first + str;
			ret.push_back(noncharStr);
		}
	}
	
	//std::cout << "Return size: " << ret.size() << std::endl;
	
	return ret;
}





