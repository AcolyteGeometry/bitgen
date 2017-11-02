/**
 * timelock.cpp
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



#include "timelock.h"
#include "Script.h"


/*
std::string scriptInteger(const int blockHeight)
{
	std::string script;
	

	BigInt<256> num(blockHeight);
	const std::string intBytes = num.getBinaryShort();

	const unsigned char byteValSize(intBytes.size());
	script += byteValSize;

	//Take bytes in reverse
	for(int i = (intBytes.size() - 1) ; i >= 0 ; i--)
	{
		const unsigned char mybyte = intBytes[i];
		script += mybyte;
	}
	
	
	return script;
}
*/



BinaryScript createTimelockScript(const int blockHeight, const PubKey& pubKey)
{
	std::list<ScriptItem> items;

	ScriptItem num(ScriptItem::NUMBER, blockHeight);
	items.push_back(num);
	
	const ScriptItem op_checkclocktimeverify(ScriptItem::OP_CHECKLOCKTIMEVERIFY);
	items.push_back(op_checkclocktimeverify);

	const ScriptItem op_drop(ScriptItem::OP_DROP);
	items.push_back(op_drop);
	
	const std::string pubkeyStr = pubKey.getRaw();
	const ScriptItem opData(ScriptItem::DATA, pubkeyStr);
	items.push_back(opData);
	
	const ScriptItem op_checksig(ScriptItem::OP_CHECKSIG);
	items.push_back(op_checksig);
	
	const Script thisScript(items);
	const BinaryScript thisString = thisScript.serialize();
	return thisString; //BinaryScript(thisString);

/*
	
	std::string script;
		
	script += scriptInteger(blockHeight);
		
	const unsigned char byteValCheckClocktime(177);	//OP_CHECKLOCKTIMEVERIFY
	script += byteValCheckClocktime;
				
	const unsigned char byteValOpDrop(117);	//OP_DROP
	script += byteValOpDrop;
			
	const std::string raw = pubKey.getRaw();
	
	const unsigned char pubkeySize(raw.size());
	script += pubkeySize;
	
	script += raw;
		
	const unsigned char byteValOpChecksig(172);	//OP_CHECKSIG
	script += byteValOpChecksig;
	
	if(script != thisString)
	{
		std::cout << "Script 1: " << hexString(script) << std::endl;
		std::cout << "Script 2:       " << hexString(thisString) << std::endl;
		
		throw std::string("Internal error for timelock");
	}
	
	return BinaryScript(script);
	*/
}

