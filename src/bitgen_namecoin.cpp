/**
 * bitcoin.cpp - Bitcoin address generator
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
 



#include "commandLine.h"
#include "bitgencli.h"
#include "unittest.h"

#include <iostream>







void testNamecoinAddressGen()
{
	testAddressGeneration("5Kdah1RhZY99HSyCL91ehzrmPLwnRe5BjFMBduEgb8aA8toAvif", 
						"MzCxmWqvj9jSyNZvzeLjC5ncfrqTC6ccxg");
	testAddressGeneration("5Jn5MzHJHkRtyHqi4iRA7ENWHKdmQvJKJ3iaENr6TGw6mHM5axj", 
						"MyiGjCNU1bwYrjNpLaoCSoosNaY9nLkZiK");
	testAddressGeneration("5K8mZbSo7ZQbV7vMUStFVUoHFLHVRqWdiXgHiFATyEtd4wPyJAZ", 
						"NGiuJQWhTJzVAZYdfmFno1A322wX45XXSq");
	testAddressGeneration("5Jq1d6u2jj3t25SnxFvv3do8HKSdJXsczYK4qqkCNyzcjwU5fmY", 
						"NAbKQviCHLe6cnmMsfFiyaHGSYiQPb5ubB");
	testAddressGeneration("5JEKyuE2uG88MbgJe1EqbeEjrxcmFnmy28CkDxnBZDBeoLeM1qb", 
						"N9X6Gb84sArKmQsFBBXvPvF6Bv5iMkVwff");
	testAddressGeneration("5JE9ZjeSK47fyuuUmM8oVkhXeHeGqL8kXkfUKaD7vEdsuzUFc5n", 
						"NGPK1K78WptaH3xvRXNezgW6dZUWHN4gVP");
	testAddressGeneration("5HyZScuRL8wscFSGne3rFKpD9v2DW2Lk5SEgFDUAURjqkM7GrTs", 
						"NEG7x7txTeiUg7PTRbLV6UP4TTkDmBdBh8");
	testAddressGeneration("5J3fBEhKJnncC7NCuZNBrMC32UhR22ErV51CHbB4PcedDo2cMFn", 
						"N2A3PeLxZL5xdoAQLpBDJqay49YFPT8sqj");
	testAddressGeneration("5Kdah1RhZY99HSyCL91ehzrmPLwnRe5BjFMBduEgb8aA8toAvif", 
						"MzCxmWqvj9jSyNZvzeLjC5ncfrqTC6ccxg");
	testAddressGeneration("5JhvR1kj8aJPozNMmJ3YUoGyp64X8xN2CV8GvxZMCtVbgkcpwNZ", 
						"N2epADPfMuEi8KGFbK4nPkYBx83zkFyRBA");
	testAddressGeneration("5JNEbdibxGtg94icXz15MW3vMxF8gcqi1G4WkDxNY4Xb729qF1t",
						"NC27gZCUTrGgc7HutzAMVxCTy6efgU3fce");
	testAddressGeneration("5KCZW3cZxFGWFV8PT5pfZbzVBATffAJDcpqSsWQzLMWHrCTy6LV",
						"NHiLA8Di9nPNEek9szADqXSxW9uZaoY1wU");
	testAddressGeneration("5K82THRnv2ZRJ5BMf5ipwa3QSnfNHtr4BMpVaApK1KkhH7W3bbC",
						"N4Wsc3h5eWrzLpzehviiK4YfiSLsmT5A78");
	testAddressGeneration("5KVh5tuyAAXrwjXzxSnfhGs2oKFyTfwP4RjiVZsw3m9Ak38mrAT",
						"NKuTVFUom9aH3rWBDrkXz1gN6zLFComCnY");
	testAddressGeneration("5K9xqN23nJ9pUU62E28hyPPUCPomP78YZWbP5KLZuMLFVuP5WEK",
						"MzhV9zgNmsANiUtfiXwjZThBpeXTkoab4x");
	testAddressGeneration("5JpD4u6u8RfwUwfpDsazvFPcY5BVasaHbu4cEwpj9fueT9WHF2u",
						"N9Tidi3GasqDU8dVWPeQ86UVtD8u9mhZhY");
	testAddressGeneration("5JyMW1zjyQPUdvGPtzo3qpZwfFuFjk5syTB3CPS4QHKDi4yLsay",
						"MxkcxsfKnCGcFytYq17JBYjL9rVmzRYsn7");
	testAddressGeneration("5HxAmuVej2yDYQ9cEnxzr2yTT8oUjD4526psvhG2PqoJiUmL2mM",
						"N8BUQ9Nn642woShzhWoZHrf9V1cuaRiL3F");
	testAddressGeneration("5JfYweuFn8A67J7GAX7nU7uwS238nHzbTJt3KCzALiRRqMBneJG",
						"N4HDwJaMJ3ETjCaHTmbSBmpsL7MygWMc37");				
	testAddressGeneration("5JVpNMtGQtwcRrqFNsCFsji92shgp9TVrMjqkXdqqbLw2K4V8mm",
						"NFeHi5Fxo8qpoC95nYBCV7sQGXLZ5i3knK");
	testAddressGeneration("5Ka4h9JzwdqRWXK8XmMxRcMJRGDPm425PJBDNaphJP1zf1GoNyq",
						"N8xsezvyKMqwAuUDK446rUtHc5d2cQmXyW");
	testAddressGeneration("5HvH73uuCZV9EvjSbEP9V6NP45tboViNQARetbUGswv24VQ1uFx",
						"MxmK1AQfqNa75uP9EeiwLTwPjxUPSYgeiQ");
	testAddressGeneration("5J2Fam5Ay5WNK48oteCRBoz9aXSKE1JPjQ6eY9nesqyqnBVUJYC",
						"N8RbugCSvd17xX2ePjmJ16rrJ3saWGtkKz");
	testAddressGeneration("5HrwCGyHBKpQfqyW2FrveFWdGsYg4GgPjKEbtG98YvUoryJ2zJL",
						"MvxonntDLfqMVSWL2RZXeNGG2wYdesS9N5");
	testAddressGeneration("5KDedVPd7rBkRPu7MT895G1QiHKjr8PXtaKYovJEZbAeKYVY4WA",
						"Myo8Q7ZnHjaYZ7zqeLoezjgCgLvJLFnpNS");
	testAddressGeneration("5K9mjv3zxyRJU8jQu4Dy9eVmoSjpWJ9JqoWdiPMViSdEEW5QiJG",
						"MvkqqK5MAUAeyNgcmTCUoGjZR7CxYnNiPg");
	testAddressGeneration("5HrMtEmjD7EtYsnye3dMNuLjbGqJaqDaWiVw2Js7oqptPRhc743",
						"N1T4pT4v2iQNDeM8LYqKv9jBCrxTdMn3it");
	testAddressGeneration("5JSQmab8rtHfuZUQE5Pxosrjvad21NABJv1TxmpmyrZK29DE8fX",
						"NKpiL3kVft3FyEsDRcUy91z2KaPVkBy5ap");
	testAddressGeneration("5KZ7P6TLYTbTaew1oXowznXkqrVkMj6Pwd98HST6PpMTVVjzWDo",
						"Mz1HP6CLBzpFjVQTKUuNSBYj2mb95xxAkd");
	testAddressGeneration("5KZNMd9WCbMsgKYQn8kAmqewrMqwpFHiwshfri6qMJLxQ2Eji2Q",
						"N2ubAMuquY5aLNdU3yRrQQBKtq8YzxG8Ti");
	testAddressGeneration("5KUrRMiqiBotExKSX8uEF7AFx6L6d4p1oNFHBj6rPTm6YPCcUqC",
						"N1nFapBs8THNjgbPpU9sKcAewDyW9FQvFE");
	testAddressGeneration("5J9bfu7UiTruyCo8CM2615187zEakQfo2qSX3HF1MQkyoJEoPNZ",
						"N1AhUjuRUum5QJFe7opN1BkRbgCA2WsrLj");
	testAddressGeneration("5KWEYrj5Fu3yPvUXjPRh62yxuPs4SrwAHQfDLbvWrZ3dTA8UQuH",
						"N5nHZnZZgefJQYkiqa1aLB1Wxyo8EooPej");
	testAddressGeneration("5KfH11qRqvNN8rneusY1CjHUkFNigZfanJrhzW5keCs5hFiAHG8",
						"N4nYn75KwfwZhDe16T6VrQz6ALRgzJnrYV");
	testAddressGeneration("5JLu3gd3CX8QyihsJx1NRZHzG3a55VBytKhvFTmweJx4waeaeQ7",
						"NADYkbAJ1kB1yYd6bts2ZBbzBgEf9KZYiL");
	testAddressGeneration("5JjyiU2At3X3ZY72KHDgS4Dx8MLnciMspGU8MR6aqWYpcMRTgML",
						"N8hy6j9qi6thN1bPRTb6W3ypsM13Nh496w");
	testAddressGeneration("5Hsk6Kn5LQZ4omD1dGnt26LHSEaw93SbsSdQtrLMTwaWkaRJou1",
						"NHXBfeJhv9taWPySo3yasBNs4TjQ5vLKvR");
	testAddressGeneration("5KfZCakpshcin9XwqbEQSx23g649nv7rAUYQkn6m2sp2mhmw14g",
						"N2Jy6tZRwjup1V1JYivDmRPHiYgtmb7cLY");
	testAddressGeneration("5JBUKaH2SXyrfb6spjL4mreV9SRbjRTYnTty7yskGhQtuxjPAZ2",
						"NETzME1fESuxqprZUjkMP6ujeyUpVeYPjF");
	testAddressGeneration("5JNU1sPi91qea5tTehWvRHKCzXt7WaYwmy3c3jZJK9EfxwK1D6t",
						"NHDq6PGBChzyySDo87Wxo9Tnfk1Uw3w8BC");
	testAddressGeneration("5HuS5uYBKU18Hcmg5uEYivorMJTk3Vog4dtWeou3my4dg6pzhkj",
						"N2cLULFQaryZ9BK9HXgRqPbVLfJRpprFWp");
	testAddressGeneration("5Kc7DhULhg1yU2XPN2oCa6whZ2WMRnwisSpATDh3oBzKYfEsK19",
						"N8ELou7DLW9UCNfxFVxqhVNEoTeZ9oho6n");
	testAddressGeneration("5HpVxrM7LfSAAt2ZBD6sJFSqviWwNPg3hE7qfHfLgn9KZN3CMhn",
						"NJ8jsyWyiybgKV2rgg1vZNJE7BzK7eCPqr");
	testAddressGeneration("5HpRVcKKtJvdNUBVhfHS8m4jwc7tm6abbeigLZUfxqKVYVvwMS5",
						"N4oMhkZJRVWSWZBzh8rLFfUUfkHaUX4d41");
	testAddressGeneration("5JVPHFrDa7SPtsMEqCc21uMGyT5XWJHqsCKz1qijf5JxKXUd5vo",
						"MzP4HzNWaBhbXQM5oSGNhHj1ooM8jqsWMg");
	testAddressGeneration("5K59ahqyW3WooKGdL1RcxbexBzinf5tpVaRNSspzxdXsP5Agi2j",
						"NDbEh33pGTK5ssdA1G4rP2gag399qTDND7");
	testAddressGeneration("5KFanXRenEnMonT8u8yZgLhbrbrXLfiaNFpHgND2cUi8RhqJsQp",
						"NKVGCgcw26HiickSjQEnidXj5T5rHHfiBb");
	testAddressGeneration("5KNBFEXWikN6AvSFitCcDugHw21bbtguiB1wi2vVVwognyeoYCp",
						"NBHGbiw2oxrqaj2wVzMUru1FkqqX5f9yqD");
	testAddressGeneration("5JfxzmW3ZCHxBgnTmohc8YndShMbS8soxZtNS8G8G6q69KTgwkn",
						"ND6ydykgrc2eQKMJBFSssQ1As6qykLSWE4");
	testAddressGeneration("5KEwfpa6GtPBTZvt11419CG8ZmtuE6R5KG3MoQc2F71GLnZGegK",
						"MvxNWuC7cvQpVAaSm7G4zVPF2SG57ZVw25");
	testAddressGeneration("5JAa3r9W9UWPaDzct2UQ7GrpE2FHmtmrQzSr84hcgXoK39LnmTi",
						"NFLp4fZFf5bUCDCifPDT1m57LhxJMRJ5y9");
	testAddressGeneration("5Kgx1YAKQZHraST1kugYPor9L3oXiczeKqRLaMVfTP6Ro5se1Xc",
						"NKohPrpUuXCjpyih9LkuHP2Z6J4Mqu5DSa");
	testAddressGeneration("5KkspfFwvzXE4xoKd13iehT1ag6DpynrAQTZz7G8MAmCPYPGC3F",
						"N262Dihbtv9PSweerxHoEBxJGEvCvQerFd");
	testAddressGeneration("5JxdsM7ENEd7VeH2LgkGpASsGfqRB2D8gtXuXwvv4DG2Kv8etAp",
						"N9PBsEsM3wWHB16XYF7rBjykocBAYGsxRi");
	testAddressGeneration("5KKK5qnozQR18DX4rjeSGCSoKPayGaGXffEbfxyRoXwskkd1Hrk",
						"NHg1aXb62x5WNB2RH9G2BSSB2TgFyMA8bm");
	testAddressGeneration("5JMwWhqSADoqwJnQgLMX7EMH4vsXKxNaVujocoeN9zj31EAqRuX",
						"N5K8RKewHBefhRAB2BReBs1z58MpsoRpD5");
	testAddressGeneration("5JTqp9KwFEvoLSxxEDXNJXRYQHNrwg6KSVT1o3HurDKhMHJk2uh",
						"NHiSsJEJi4tHP2QwfUj6SsCj7kaGTDnK4a");
	testAddressGeneration("5K2gLeBUZdKDqBbe43MJpGCPKrcchWtRHFLJdN7YsUofhHvmcsK",
						"N2sXkxz1SX8DkKxsqzSWR7hsAna9tVvWPK");
	testAddressGeneration("5KGnQjYp75pZTLeEutTZRG1HPJc5sZX69hdQPsY44FUvGUVrC5t",
						"NDqnWDut2okH8s7g6gjPxBvNhWoYxFm1gB");
	testAddressGeneration("5JzdKshKeqwyVr3VMdwkayfJzUqUar8bCDZvbTnnsXvTnchfSk1",
						"N1fgprCPJxrgqog9Ni41FHDj2iMJsQwhPN");
	testAddressGeneration("5KV3s7ersZ3auLrhtA9pY6cvssWMYdMQ7MdaCyJTHScYMW3BT6H",
						"N1JqF8H8ZniAv3VQ6Suxr5CsizGtthyP2H");
	testAddressGeneration("5J5L6tACteKiDz2BVrAo1DGL5zbNtDE2YH3x499cEbat7LbYDiD",
						"NFMkqs2zHz3NNaihztJ5Bf8NV6ngcHaagZ");
	testAddressGeneration("5K1DkzdaxXmEawiDUJyGKuoB2HWUVWDN3N2EwBKwrLpfsXBDC5A",
						"MzJLvRFbRXjg37G4d1N88LTMX6P18KAA41");
	testAddressGeneration("5JP2D57u45bwxDNE2jUu5nA7BbZh23yUnVVpq45ZsJhBCFqQPKj",
						"N3p6bxR5KWKREQrr81AoxtsnrtnkSBpbgJ");
	testAddressGeneration("5JXv9XqzGXgCgXnKxGfL459wGyWwGcZtDoQ1qHSEGSHHgeK7L9z",
						"NGLfqoymZVrNYP9Xdwri4YVoEFRnQU5EfU");
	testAddressGeneration("5K2JKeKnh2AU2DbBS3LYbZcHJue6tAk1hJYLitUz4cdnczrZTBX",
						"N2m5oKKiiyjpJNUWgUMkvMRCKU1Uu2m4vs");
	testAddressGeneration("5JF7VwQ5Lv8UAyxXeQbXbYJBtZxEfdYP8UPhmmq4DPkWPpK3jRm",
						"NDQkgCnpEjeqJkyHVGroB8XVqA6YkY7stV");
	testAddressGeneration("5KeqUqLKmUn7yKHBcfR9V1VHypUQch5SzCPkqXHoHGtYoG79uiu",
						"N91CXUZtDkTkPkRsbsdeCGGrdLNccC7AVf");
	testAddressGeneration("5JBTFyP3289YKun9XARHyqDTKzGm3wxtqeKvMteqpGqdXqJSTRz",
						"MwbP57TuKCJAVniNRuQansNdJmdXsVzSEQ");				
}


void unitTestsNamecoin()
{
	std::cout << "Unit tests for bitgen-ltc" << std::endl;
	testNamecoinAddressGen();
}







int main(int argc, char* argv[])
{				
	try
	{
		if(argc > 1)
		{
		const std::string firstArg(argv[1]);
		if(firstArg == "test")
		{
			unitTestsNamecoin();
		
#ifndef NO_BRAINWALLET_GENERATION
		//brainwalletTests();
#endif	
			std::cout << std::endl;
			std::cout << "All tests performed successfully" << std::endl;	
			return 0;
		}	
		}
		
		doBitgenWork(argc, argv);
	}
	catch(const std::string& err)
	{
		std::cerr << "Error: " << err << std::endl;
		return 1;
	}
	
	return 0;
}


	
