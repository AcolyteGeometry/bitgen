#ifndef TXT_GENERATION_H
#define TXT_GENERATION_H
/**
 * txtGeneration.h - Bitcoin address generator
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


#include <string>
#include <list>

#include "extraInfo.h"




class FilePair
{
public:
	FilePair(const std::string& publicStr, const std::string& privateStr) :
		publicFile(publicStr), privateFile(privateStr) {}
	
	const std::string publicFile;
	const std::string privateFile;
};



class TxtInfoFile
{
public:
	TxtInfoFile(const std::string& inPublic, const std::string& inPrivate) :
		publicTxt(inPublic), privateTxt(inPrivate)  {}
	
	
	const std::string publicTxt;
	const std::string privateTxt;
};


void saveExtendedTxtFiles(FilePair& files, const std::string& fileId, const bool currencySpecific = false);


FilePair generateMnemonicFile(
					const std::string& genMethod, const std::list<ExtraInfo>& generationExtra,
					const std::string& thetime, const std::string& accountName,
					const std::string& mnemonic,
					const std::string& versionNumber, const std::string& applicationName,
					const bool currencySpecific,
					const std::string& xpriv, 
					const std::string& xpub,
					const std::string& derivedXpub,
					const bool includedExtendedAddress,
					const int numWords);

FilePair generateExtendedTxtFiles(const std::string& pub, const std::string priv, 
					const std::string& genMethod, 
					const std::string& thetime, const std::string& accountName,
					const std::string& versionNumber, const std::string& applicationName,
					const std::list<ExtraInfo>& generationExtra,
					const std::string& derivationPath,
					const bool currencySpecific);


TxtInfoFile getTxtFiles(const std::string& pubkey, const std::string privateb58, 
					const std::string& genMethod, const std::list<std::string>& generationExtra,
					const std::string& thetime, const std::string& accountName,
					const std::string& mnemonic, const bool compressed,
					const std::string& versionNumber, const std::string& applicationName);
					
					
void saveTxtFiles(const std::string& pubkey, const std::string privateb58, 
					const std::string& genMethod, const std::list<ExtraInfo>& generationExtra,
					const std::string& thetime, const std::string& accountName,
					const std::string& mnemonic, const bool compressed, const std::string pubPath, const std::string privPath,
					const std::string& versionNumber, const std::string& applicationName);
					


#endif
