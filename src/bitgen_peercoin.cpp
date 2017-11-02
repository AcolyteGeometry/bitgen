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





void testPpcAddressGen()
{
	testAddressGeneration("7AXPMtC7dtqU71jASbBmxXdWdeHr6ybQiCwm1SBJDWHKT7cFHwy", 
						"PB3BNLA9kkxd7UifRBJpCSh3T5ekEUkEux");
						
	testAddressGeneration("7AboXS3m79EEFGRBZo6p9nk5XTUTvKjtTdVicLB73Vt5qjR6f2T", 
						"PS2zVXCC34dU2B8Z8L7HQGyFtfJq9JTKeH");

	testAddressGeneration("79WgJbUgtrwjanyQxVHmzXwayWAiJW6f2LZ5AkozFVZtHUbzsMe", 
						"PHpohGBccxVF9uPoa9rVoWiXjqigPaxjMU");

	testAddressGeneration("79tnZWAE57tQhSyHRpS67mhwN7Cs4tNXatmGkFHsT9ycen7FNNS", 
						"PUbTaHbaDG17zHRfNPfDwELDYzer3q6jQB");

	testAddressGeneration("7979RGrTHUCrGAGyUGNohp5yBo5y1asaFW9yv7ikNwk8o6Svmfd", 
						"PBAMtpv4WVKSZsSyfbV9LjZb7Jc6FwCR26");
				
	testAddressGeneration("79QD1QjagRx939T7C6rbeuHHQiKju5BSft4eZVEWQ6rLbnjkWGg",
						"PPVbw4n79i3rJJpaWTj8s96aV2qbsEBYhL");
						
	testAddressGeneration("795v8oHTuvBbpzNgLDTPaWu3VJbWrPTwAoxXHRnRGsnt1b5f9EP",
						"PPU5s6EsxoQHSmsLQyPnt2kiW5dnGZpz9t");
						
	testAddressGeneration("79Pt9cpgPRZVVxGtrHC5Usr9SfSaUkbicEwZx8gvAN5oDHAbZip",
						"PHbMLkncB9NzGWaDP9KB6GJW12kxwFf8f6");
						
	testAddressGeneration("79kKgPFpERJPw4TxuYcCB1WGn1hAYuZ4jqY1QHnF5BEr9bQM3pE",
						"PAV5qPtHePricBLNimnaEGDYRzFztrK1HB");
	testAddressGeneration("79iZbra3y4cAe4B2kUHqBTuNc9pag7tTxz8mECwQWa84cLUZaft",
						"PMcozKjQTeT4wGNWTy8GbicqNkgxuRgVBj");
	testAddressGeneration("78uPmMTKuZzccJJ15jSJYZjk7Vyx2S6VgfYvy6ALBbMEfqzNUNe",
						"PHjSxAxR4Z6FnHEemPURGrmVnrhA14MufG");
	testAddressGeneration("78p58UAaoNmJ3hR9ggYTEoMBbFBNNQaZRvysywbWiDB7tjEGgaZ",
						"PTJZov6iNFBN6SwPNow6M6xAvgxau7ehZf");
	testAddressGeneration("7ASckc3HMKJEjGLjPqXFw3npRvtw1uhDAfWMLtzTAVhZL3Hk1Ah",
						"PBM1dLPkz68oHFG5wAVU83Lu3y7RCgNFc5");
	testAddressGeneration("798DeTKhvjzD5qaB7i5KTBkkyHCxi4WZmvokgNLm5cS1jUxjQcA",
						"PVETaBGuhubKp2sNZaizqxPQFYgLWKSe28");
	testAddressGeneration("79RPN1KqLfp25oY3Vkmop5FRTiAQCL7ueaxc5Rg4Bqg3jXW7wuF",
						"PN1hLJ6vPvoZZZqqThSQvKJmEWRvvsDpTx");
	testAddressGeneration("78uFciEjYEqn9UfXNcPY6VwWr4RG2oX7tYr3qB8hBbA5bhJiTCN",
						"PXWH4A2GxpkssfrmtvkF9AkHYDzw2o5QEV");
	testAddressGeneration("79AjvgFXE9YBuWeL9UeMgumJwRnFFYQcdpAiRSBz28pEzYVuEVN",
						"PPR7yCtYDpoJSRs3rzhYc6onHjnShNLXiR");
	testAddressGeneration("79WXzsrVkw42AyHNxCtTb5tCQd1AxVBp31r1iMT2PJTuwi1xjtK",
						"PQ5drNLTCVuoanh1cRuSA7LmnCBcfef2mQ");
	testAddressGeneration("7Aa15WX5kutsochb3iTWEmHQVvGvBCme92WeEXaAf9dKjAcXmwK",
						"PQhv6pYVYjo8SNijjq3skWbGr17gWjEPRv");
	testAddressGeneration("7APZFUmveFYmeSaHgqoXBH6dSkKRd8i8wph1fnAuCpctZDUVnF6",
						"PAcMtK5uJyc1PAJAxhCaTtXN3prBrdmrdR");
	testAddressGeneration("79XfUE4yByrB7Jgq3mxLS9rjH7U3Pvb3gK4uCh385ygLktqxkQn",
						"PAZN7Fv2zxqQH7yQ3Fa8gSiBHuBjWPj89A");
	testAddressGeneration("7AfEyneMHWBmaqBr5CEg83MqpYv413jJ6dzRK1wn6kmx3vE3HhB",
						"PEeKWmDd2tojZFfvvadCCQddg1x3DVLjia");
	testAddressGeneration("79BUb2vX27n3fjBjBdz1NDbVyai8b75s78dGMPnS37nEsc6Ec2a",
						"PShTE7PtKDwYRcri5MwpwRHopeMVgXHhiQ");
	testAddressGeneration("78t26fqCZzUDQNxSryxYsYuXAUN6jbNX5TMuukffj37ZwkZWoXw",
						"PHHEmFexGJ4MBnYufpxK1FijyRFKZPxYLC");
	testAddressGeneration("7AVNfKPiqNrY4dYPhcEaDwi7S1kq3aK2kw4FmAUeP7Bnm8g9Xt3",
						"PVpG5apBHSbZ6jbKz4z7Ph8SHRRGq59qKQ");
	testAddressGeneration("7ABjBxGkFwiAQz5b6m7bBzp8E7qTGbELzSxvZCg9urGoZpVrnfs",
						"PSfegThtJbecDhi7fugnpWUkKjeExfdFTn");
	testAddressGeneration("79Sqc1ckERVq3bGP7zVX1bgaszeGA9rKTnZNyMC8jSpachcYWPY",
						"PJXX2RxrhEDpiQbNqW7FToc3A5x5hgNr5M");
	testAddressGeneration("79LGsmMPuKyeFnPPSnzBs3UYXVXAnxHdZKaq2gqQbaPSppjbHEb",
						"PDxjVyR7K5A9pGRwEMiSE8vkGjKxoWaCJU");
	testAddressGeneration("791kK2QUcUFvDgbLt9n7YbLwGy3TZZTkFEEaZyKAYSJ3TeVqZdc",
						"PHD19W4p7JCPsnkBciBVHUrCQDdKk2DnQi");
	testAddressGeneration("79QRg4ASSaCfJd81uoKgwkk3uiKxTa3c7MoE6hES94ViL7rZVg6",
						"PKKu2B97DSA43pYcFZPUn9MEbGfrXz8U2q");
	testAddressGeneration("7AJ9QP1dbgPqAL3m4BBHmxvjXYWStMnUAoAJKKw9DMqca7xNbVY",
						"PKo8Z3D4P2KiCTVQkXtdSnnvn5nLgDU2x8");
	testAddressGeneration("79iuHa5vYVLiio9EdwjuzEgdKrqb2NXv4AFGEQbL3V6A8h7zK6L",
						"PSJcerr8LkhnN4unaCQpYkQbLQNV7ayrNh");
	testAddressGeneration("797bjpP4SojFX3xL6NvDqhHbzCR8nTihc7LfmGvSCTXidZdRYJw",
						"PGkmhExFng1jLUsH1dh9Lvzq9NQvdbWSZi");
	testAddressGeneration("79NtTLtnpKAwad8o9bGYWf5SxDXedwq3Xcro8cxcfihNbDz9qPu",
						"PVSRnK4vW6j16F5MRC4uqtpvB9wMtoXWy9");
	testAddressGeneration("79KUXmPCwxaDhZgt2rrsMo6wqh626Gu7WgE2qVMr2Ys6apDvyDe",
						"PHjPhiA38qvbDN8yUbcE9d1uzC1xr9Lea3");
	testAddressGeneration("7A9Mqvbx8EE4PRsbdQaHPDE3QB1Wi9azmPhS5zHaJaQDAEgZvB3",
						"PGKLsLCuRrwCspLKVFzgebRktmoU2m3HXY");
	testAddressGeneration("79qyATtXjtgX9CJTgspyigsdvFYjgaZrEzUYUJEUyJGVheE6UHB",
						"PMH4WLLxeTW1EtemmCepLTz3kassKnMrdk");
	testAddressGeneration("79RCQd8t1HWCkAha9cCndNJ3dcrVvotgEUAc5y7TdECa24o7BwZ",
						"PMTbD3cp7GpxHvc3Xk5R9EoyvGNYP7R6yL");
	testAddressGeneration("79kiKRyyNdeDn5SiHZ6fPGfmZMwJ3TLG2tmfXoL7oTiCBcW1c5A",
						"PWT1oDNMMuyLCLbqfgYjDbN9QWykgRKT45");
	testAddressGeneration("78npdoKS73KnfWysETKjuc9mUDR39Y7v64uroyLoJqFhvEycVrS",
						"PRt67nFsUr5uNDVQvbvaLkEfUsqcsMy76S");
	testAddressGeneration("797vrrEi2QFdYMkyVUUMBwLdBvmsVWw23FMyjYnGJXMvqrivuzi",
						"PB3mQpyTQCxdG7DH5jyaWyQ7oKnZF14QCq");
	testAddressGeneration("7AaZxqJvqRJPmcYNM3SD7XsjhduckUDsRCmw5JNFZiCaHox8TjF",
						"PFrFCHSJgtnPFLHXsy7aiSJ29N2FGM5Tzt");
	testAddressGeneration("798yECXhmwfTHLmgjtvCGGUQJpA3UrWRPfW9Qu9iQYku8sMzDys",
						"PLYibMeeDwLTwjNAhB69HaTZCL5oSy5fJ3");
	testAddressGeneration("7A2RZruk9cT4rZqx5PBWPpwiTNHCPAFnv5zKvCcCLUEYT1ihsUK",
						"PTVqzDDbCphYQQi2nwK3mKGXQLSEZPZ2rD");
	testAddressGeneration("7A2S6qU95NsCDTSEDj9kGjKv5PRD8Wp8VvcDNYFgah1fsq5okpy",
						"PPHP3qzdFbDGXtc94GNhS7frE6dptc9PAZ");
	testAddressGeneration("7AVt5Fe84ZX3pXGyP1xth8UTdFtLbQFhhckx1Qji5QZPxuGiQLg",
						"PLeJ4bxrib1nGHU9VwTdj5Fj58J3bWPQt3");
	testAddressGeneration("79xadLQUdGnKVwtptW2pWPEFj3rA9999MJurQSDKk1zFEZXk2Ra",
						"PKXfPYM62e791dPmFR6G2uGv5v1f6ZWdpM");
	testAddressGeneration("7AS6K4CxXvMxLAdBqMkexHiacZDDwGJAG8RNRmsQua5Fv1jE4J4",
						"PKYheoffD2bzdbykBdGkZEQBX4CebdqxH6");
	testAddressGeneration("7AV8Esbpbrn1FRjEUUC1xQnc1nzBTSExDmWh38x2Miz6DFZk7ci",
						"PPjiy5RwkwKYaXAw8rufby4hVXY6s85qv9");
	testAddressGeneration("7AWta8zgSS6XKt136pDj15qHVgdcqrHuGxdauy66eWWY1tH9pzA",
						"PHvZEqJ1FQmfr46yQfCeettBMSKa8YBwiT");
	testAddressGeneration("79BDVBzEQCN8S2tF9x4Q3Cz7GGKrtYDkLC71U8TmcECnBca6hYr",
						"PCv8SmGTeenBTBModJM5bQWAf7DFqLyBwZ");
	testAddressGeneration("7AaXxW3HqmFfsVsM8uPKXA6PxKRYWQ3rvKje63X9AHvvmubXx1W",
						"PRdmx1ndYqM4dbzT1pNj8LdhAXj7CW4mDA");
	testAddressGeneration("79XgzXaLD3UE1MMKbuUJBxB7fTTunWGSqMXXucWZ4bRMAaWeuoT",
						"PBF6T5yN5iMDkaQza9G4ux9hGoPtKRQtq4");
	testAddressGeneration("79YNo3tTfAVf5mxbY1WZ1wfQ1vn5QGaM9cwvu7V7ASMQYKzhEMd",
						"PU3gpQqM2FJprqRKxmXQUHLm7mevro6zJu");
	testAddressGeneration("79VyRVAaJhc9V7NnUmKRvRZ8yxzVnJztmBBE54e8WhjRWV9SHVs",
						"PHsA5vvcEDQADiDBPyRbfH8vD2XAZqKaYH");
	testAddressGeneration("7A7cAzqj27YWKfVJfaAGNahshRZhERWNS457MiuduY6KsT8pDyA",
						"P8vsJBkY5V3EqDMNJ33qPKrkwPCVSPVxY4");
	testAddressGeneration("796XGV5ZzcbAqWm6PhpgaK5FK1KzcZCycd7MdFwgNNxymZHTwzW",
						"PJ9TiWcmE4oCHRTYjEa2CYXdZLCJwJJnHj");
	testAddressGeneration("79gw98gZnTm5kSTjoZySmxEjjC8yoLv1WC3yCQXuqSvwg8wK7nP",
						"PTB4xRzA3U2P7aeRVUy8eFCs5MAeFCBjDk");
						
}


void unitTestsPpc()
{
	std::cout << "Unit tests for bitgen-ppc" << std::endl;
	testPpcAddressGen();
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
			unitTestsPpc();
		
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


	
