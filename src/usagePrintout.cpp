/**
 * GenerationParameters.cpp - Bitcoin address generator
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
 

#include "usagePrintout.h"
#include "currencySpecific.h"


#include <iostream>

void printUsage()
{
	std::cout << std::endl;		
	std::cout << getCurrencyName() << " address generator" << std::endl;
	std::cout << "Usage: bitgen-" << getCurrencyShortName() << " command [options]" << std::endl;
		
	std::cout << "Key generation: " << std::endl;
	std::cout << "brainwallet | bulk | dice | hash | hex | mnemonic | random | wif" << std::endl;
	std::cout << std::endl;

	std::cout << "Entity information: " << std::endl;	
	std::cout << "info" << std::endl;
	std::cout << std::endl;

		
	//std::cout << "Key validation: " << std::endl;	
	//std::cout << " infoAddress" << std::endl;
	//std::cout << std::endl;


	std::cout << "Public address postscript and invoice generation: " << std::endl;
	std::cout << "pub | invoice" /* pubAddr | reqPayment */ << std::endl;
	std::cout << std::endl;
	
	std::cout << "Vanity address generation: " << std::endl;		
	std::cout << "vanity" << std::endl;
	std::cout << std::endl;

	std::cout << "Mini private key handling: " << std::endl;		
	std::cout << "mini | miniToWif" << std::endl;
	std::cout << std::endl;
	
	std::cout << "Hierarchial random generation: " << std::endl;
	std::cout << "genRoot | genPub | genPriv" << std::endl;
	std::cout << std::endl;

	std::cout << "Extended address: " << std::endl;
	std::cout << "extended" << std::endl;
	std::cout << std::endl;

	std::cout << "Electrum  address: " << std::endl;
	std::cout << "electrum" << std::endl;
	std::cout << std::endl;


	std::cout << "Display bitgen version:" << std::endl;		
	std::cout << "-v" << std::endl;		
	std::cout << std::endl;

	std::cout << "Display license:" << std::endl;		
	std::cout << "license" << std::endl;		
	std::cout << std::endl;
	
	std::cout << "Example: bitgen-" << getCurrencyShortName() << " random" << std::endl;		
	std::cout << std::endl;


		
	std::cout << "Project homepage: http://bitgen.org/" << std::endl;
	std::cout << "Email: bitgen@openmailbox.org" << std::endl;
	std::cout << std::endl;
}



/*
void printBitgenUsage()
{
	std::cout << std::endl;						
	std::cout << "Usage: bitgen-" << getCurrencyShortName() <<  " command [options]" << std::endl;
		
	std::cout << "Key generation: " << std::endl;
	std::cout << "brainwallet | bulk | dice | hash | hex | mnemonic | random" << std::endl;
	std::cout << std::endl;

		
	std::cout << "Entity information: " << std::endl;	
	std::cout << "info" << std::endl;
	std::cout << std::endl;

	std::cout << "Example: bitgen-" << getCurrencyShortName() << " random" << std::endl;		
	std::cout << std::endl;

	std::cout << "Display bitgen version:" << std::endl;		
	std::cout << "-v" << std::endl;		
	std::cout << std::endl;
		
	std::cout << "Project homepage: http://bitgen.org/" << std::endl;
	std::cout << "Email: bitgen@openmailbox.org" << std::endl;
	std::cout << std::endl;
}
*/
