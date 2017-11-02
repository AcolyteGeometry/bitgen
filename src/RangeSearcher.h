#ifndef RANGE_SEARCHER_H
#define RANGE_SEARCHER_H

/**
 * RangeSearcher.h
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

#include <vector>
#include "bigint.h"





class Range
{
public:
	Range(const BigInt<256>& inMin, const BigInt<256>& inMax) : 
		min(inMin), max(inMax) {}
		
		
	BigInt<256> min;		
	BigInt<256> max;	
};


class RangeSearcher
{
public:

	void add(const Range& range)
	{
		ranges.push_back(range);
	}
	
	bool isInRange(const BigInt<256>& val) const;
	
	bool empty() const
	{
		return ranges.empty();
	}
	
	int size() const { return ranges.size(); }
	
	std::string toString() const;
	
	BigInt<256> rangeSum() const;
	
	static std::vector<std::string> permute(const std::string& str);
	
	
private:	
	std::vector<Range> ranges;
};




#endif
