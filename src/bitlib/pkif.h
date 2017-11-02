#ifndef PKIF_FORMAT_H
#define PKIF_FORMAT_H

/**
 * pkif.h
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

#include "bitcoinEC.h"
#include "pubAddress.h"



class PubKey;

class Pkif
{
public:
	Pkif(const std::string& str) : pkif(str) 
	{
		//if(!isPkif(pkif))
		//{
		//	throw std::string("Error, not a valid pkif");
		//}
	}
	
	PubKey getPubKey() const;

	static bool isPkif(const std::string& pkif);
	
	std::string toString() const
	{
		return pkif;
	}
	
	//Todo: Remove/rename
	static const ECPoint parsePkif(const std::string& pkif, bool& compressed, bool& onlyIndicatedUsage, bool& signVerify);

	friend bool operator==(const Pkif& a, const Pkif& b);

private:	
	const std::string pkif;
};

bool operator==(const Pkif& a, const Pkif& b);



#endif

