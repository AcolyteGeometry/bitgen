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
 

#include "version.h"
#include "currencySpecific.h"
#include "util.h"


#include "commandLine.h"
#include "bitgencli.h"
#include "unittest.h"

#include <iostream>




void testLtcAddressGen()
{
	testAddressGeneration("6v9EFJr2og8VNUpP7y7cXtzGPFEyGnufqRofcUUXGo4ygw3wWFT", 
						"LYqQLA3c8hTNnB8Lnxnc52Hy5wCyo8EtN3");
	testAddressGeneration("6ujyaGbZpyaX7LzX8Y3DZcwmPEWAjzH9Tj5vTKhyiZm579VyuBW", 
						"LbrykpPkLyLtxxW5jLXbCoMEAN9o2EHDyt");
	testAddressGeneration("6vUnAUH2UAwHG7P11tTm3qb3PykR6vS1oUyf2j8Z6JkFAZxX12f", 
						"LKsPmk4RXJviERYw8b59zLJrqN9TvK7kKz");
	testAddressGeneration("6uU99jmhehKi3GMcwtRrHbShxKn4PFs6mQ6p4fzuXABLz349WdE", 
						"LN1igYFxpe3hV6axaxS5ved3Twa9p69VqG");
	testAddressGeneration("6vAadd5bPtSGsfJUtvPCAS83yT1W42XwnBXYBk5jD37j2R72FMU", 
						"LKUHGpXbwPF6RiN3ZSj5zuA5A8bsRdmbb6");
	testAddressGeneration("6uqKNcW9YsYs9dxtSoj4NE8mj8oVMzhY3eJy3DzquVh6gRhLRtA", 
						"LPcWDfJuqN1H3wSieq6pqSGcJh9gqm3ikb");
	testAddressGeneration("6unEn7XAVpHimJKKV3e8FK1oPTVUVPZnVxY3YnsSbKQV2aEaYPf", 
						"LSuhg5QocbuvqHBb4WVwegYYiAaHS42UXz");
	testAddressGeneration("6w2rLRxjA7st45BH7gdS3RFUAKgsj97HYTchexUj86Czc9NohRz", 
						"LSwRjFxLcLBLBiLnq1ZQ1grkZJSTA5YhJ1");
	testAddressGeneration("6vbA2FQWKzSq2zcvvxVdR5jWfH8x86z38WyZPsiKEikZawSpepG", 
						"LVmSStQXacu4WkqUuaXbH5ja5JZx5yxvh6");
	testAddressGeneration("6uC3vnB24V4iYQpmkyBn2Fgyj7egvWHggFm3xqJhYvjawUZSk47", 
						"LUpv5c8UYtdxu91hxCejDEQGBhscZpA47V");				
	testAddressGeneration("6uzydSURd7QAJHMkV4i9h3MhGEj7w5M5kteAxqWiLbXGqiA3U31", 
						"LRZDZ4mfY836dy6A5461Jri9cCDFdaRock");
	testAddressGeneration("6uaU1LNB92dj1WBH9iPtLKXYyL7p6vPViX2t7quTRYp5nCVDWtD", 
						"LfU8KquySNKxbCmopSYnK6AH4uXDqDVgrM");
	testAddressGeneration("6vriep4xEEpbCtEz95suQY7aChyv4pLdaNiSHW8SXhfdNEzbvuM", 
						"LPWV8UhHrwknMoNxJw2CBjyeGaUWoSPn7E");
	testAddressGeneration("6v89DaFLmA1j5egg7EoK1E4TuNWmphKXWTNgssRXituUBZaySKU",
						"LKzGEhshpivptfj1YdEvk5f92HjrLdSoeZ");
	testAddressGeneration("6uFqeDpHfDvLxi8i9LzsJP6yzbupKY6a9RJcTfaWab6v6Nz4nvJ",
						"LNB4Hw7vF4UEb2nXM1s5YSt2k3hdYPcVZ6");
	testAddressGeneration("6uxJJ4nLV4kZ17L1vuMYhdUaPQ2Lcv1svHE8sfb7V5PvBCsp8fU",
						"LU1UZic6UqB33tPfjyKpcdD8SgedauTDSA");
	testAddressGeneration("6vkHrk6v1VX4jNfdYS49E2AvL86YLz3bk2e3hnJ3S7af29S1NHF",
						"LKr8s3UpLoyKPAs1r9oEKzs8vZQWSjN35V");
	testAddressGeneration("6uqNSF18Vh39syZb7om1dQWF2qbpxoTydbC6DoZN23S7UkWc1KM",
						"LZphUPwh8D1MstDn59EvKAHhPzwcZXsjvB");
	testAddressGeneration("6u9ZWX5bUajJDsM8suWFDL9jWUVBuK2VFx8EQ4vADbuqy51vLVx",
						"LhPmH2px4vHypxzivRwe8aLfVcpavQQy1j");
	testAddressGeneration("6uhJ5rYXdMyQsy22ZxJdh4wRfJkNAdSMgMBNZJZpsxvBAi1tJHB",
						"LNMQxa1NM9CoihmjfB41yQapDUgLhimpDX");
	testAddressGeneration("6vAbvfNFdw1pq6PPnnKgH9dCATprv5WJAeyqLhwNsG3DnCNpy2o",
						"LRNYc4L1noknAkPcBsrHRVQiezrrKXBPAA");
	testAddressGeneration("6uNFVkHK9U5HS8DGmxiiPqZgW2Gu5q2482A9SYtjtLXHRt7fivc",
						"LPogYamc4nWhrGQ8CXNzQdsPjjrHPYbuDN");
	testAddressGeneration("6uFEG58gEq7C2Rb9ddfwpfzrCQDBcFUegpos9DLphjAtXPHRSuy",
						"LXPb2bNEgJL1b58HFaA4N5WtefVFkgLUbx");
	testAddressGeneration("6v5fLVsmiMqeN5KLJAgissRwvWQqm2b75vMPkCJAqWyu1tn3gfA",
						"LUvbxcf4RheE1bDGN3DFFkoyxLp5F8AYMc");
	testAddressGeneration("6usQFnhZA45ppyUQbuvLe8RLpLsrcWyuQLTYsscD7NyTjKrVFUZ",
						"LQmU3YLxB75TAtvKdoF37GcEE6E3fF8285");
	testAddressGeneration("6upBsXVZjugnLC7pqsnv3j9kVzrP6iE4m3uAwYv5xWX4WKzjkQ3",
						"LbiMEBVkSJjYkX52gerkqwSGkBQd1jntG9");
	testAddressGeneration("6vPB89XV5YAmiexHMEYsNpknsYrCy1eX3m4mKFKjCNNuhjqdeRy",
						"LLSomEDLSTdDZwBcXx3j7aSCAxrPdpzxFo");
	testAddressGeneration("6uP46af1Av8evVrXMEkg17yFLvTYgYiU435H8XY1srZBvYnWswG",
						"LNUdBFBTKDTbpvrS5JVEo3mrpd2XUu7H4U");
	testAddressGeneration("6uJmPEV7QpLmZvYWn2RKvqKfyNWAnnbk6Fau948umFh4fcyAbib",
						"LefNBFb7ywB93ipc3w49tWG2JJ6z2vRDTn");
	testAddressGeneration("6v2miVYBYUN51JNrUEZSdH7BX53uQ9Y9TkF3CQZcHrtcT9vMz5y",
						"LPea1R4fiicFknKspGqcXrgmhhssyBV1qb");
	testAddressGeneration("6vGJFsjhMnQx3RSknJL4NbpvvKXcwXFuiCXJc7nUDrE7ur6MDMf",
						"LWS4vPqMhTMTwUuFjCLqLdnC79MH3GEBW3");
	testAddressGeneration("6ug4MGzxsXT92ubtkR1wNYyqV1CFEuat89SzQfGwQm6asZwgxFA",
						"LgDmt3NJxhEDp4yeVDJJkvqqhX3Fj3t4cV");
	testAddressGeneration("6us3RoZ4B6Wg6GrWzG7pxpaKLPcSyArrNCEeKr51YH9hQVii2p3",
						"LXhYHridYpnPqwP3fKJ2byosJVskaj5i6C");
	testAddressGeneration("6uNyVNHZpqBtyWMqZG9NLuXyKm3A1EqAcZj3ZRFfSHG3oohRUDR",
						"LfRZ5BK6v5VYjX6dX1GDBhbNz6ncGpgz2o");
	testAddressGeneration("6vJU54L8kepGdXftkzWCS6nKbjiWXkwhaFZx97nNs1D1TaMyq23",
						"LNRM12YcNxgMGMmqiqPDiSQedKegbL1EtE");
	testAddressGeneration("6vgfpfcqtFxXternUqz7P8T1ogZcGNYaBHv6M37KunrSnxUqrkq",
						"LeV2SAWvDFGgxTYvkkB1sZe562KsqNtBxE");
	testAddressGeneration("6vn9nLuUZQmrskGujL12sz9hx91xzdKQaqu3Jn6tVscjwARWS1q",
						"LaHZsaEVWaFoGyY8fbayM3KorStvPTFZqN");
	testAddressGeneration("6uw4Je5NawSfDHdxsktcS1fagyvF8HB5mJC1rDPCkTdYdtFehmn",
						"LRyRyudf6LF4EA6e2ZPo2ytEpdDzvq7JGP");
	testAddressGeneration("6uoXr2zaK1xLhakQ4X91iYGwVf6M4X1Ag9b8x7dyADTbPsTL2MT",
						"LQVjmx8HneZHe6ZbLZ8QsijB7AuGZkDqGy");
	testAddressGeneration("6uwG5TS8kxhpPeU1ggPuiCGkXXPhtLHzBA7so8fkdYWmS9WMUiK",
						"LVfZn5ZnjQcSomK4eaecXhrBDNHM2fL2sF");
	testAddressGeneration("6vxBjC3YqmuJN55sjUs1DSnLYWyzvH7gVFTNz1P8oEi96vWRYJ9",
						"LN8vgED6vf2wUemFmZKCWw7mqQJd29nuHZ");
	testAddressGeneration("6uVQwStTsXyg5EHrnzPjHwqQEyWJGdC5N9MkF8WRohp3FYV4KQx",
						"Le2K6wJWt2JhojXkVLhxTYUign3z1b2Xn9");
	testAddressGeneration("6vVgx6GBeHude2MVfbVvMyHQuQonArPWNQxSicrgyZnqu3UtRcU",
						"LYBp2rJMX5wg477aunVLAbwLqXfssteRGu");
	testAddressGeneration("6uLRGh5zzS3wtV82NHTKtc72F2nRFcVAd7KnFNxH4aKv4B6VpUj",
						"LSTTtwR4hWLWDRUDf7vwP63GUVp3bMdMFB");
	testAddressGeneration("6uQiwJdBnz5m71mis1FcWgVqa1oyNRhKWucbxUFxbNLHitBRU5p",
						"LgiioVRgHYfWxQE4TwhTcTbcFKu8Bcvj7A");
	testAddressGeneration("6vw3CbaJauTvCfAoRZaWSpgwEfvSKt4jEXUYHz4upHUdcCPbkse",
						"LVpGTfNjgo1CARW4U9iP7E8RnJBkWFwsgh");
	testAddressGeneration("6v28t73NKDqzuZAPPnRC9Jo9N52FeKtMuKqwr3aX66R2S55vMMx",
						"LQQp7uxnbwfmVATWcePMMRbbvTqMcZV5Zs");
	testAddressGeneration("6ugh9FcNTbCKBNWJKio3NPosCs9BvdFCK2BXEgejKHTm5s5usKN",
						"Lf1cnSt4ahwTJb9Mc6Ux4pTwrqLGTY2BX7");
	testAddressGeneration("6vPSkSmhUCpMDFQKb6v7ADCdUZysXCUK2gX8CVzi9eSZVgWcJqU",
						"LKKxoYEH78fUgHbLDXAiUe9GuWD9B1AjV5");
	testAddressGeneration("6vckgxCjxpa8uiwPY5uo26aVFxUjbJSW2yAaE6TdvEX8hWMpPCC",
						"LepUoKJ9K2EF9B4aB33rfvsj3MwJYa3gfQ");
	testAddressGeneration("6uvr6aeiVfB7H5XSGRNX1Y2ivrvS7Jyq5yx1WALYCqyf63sonHd",
						"LQRsXu1YZM4i7NQzKqkGrvHnPjXvoEyxsZ");
	testAddressGeneration("6w4PMNesfi6FNdtQfedaQjuNFJ6uognNjt2mmTP5CjTLBWVsHBF",
						"LYngBP1DvaLFqKWm1SKoZZJ6byFj6ytxDX");
	testAddressGeneration("6vhEdcGMBXykHSPLcm9PxhZySSxjimdYPsL6q8DkjnpapxD5GVM",
						"LYd6bNP1A8FreeEs3GaGiGs28ctcmrsXCW");
	testAddressGeneration("6u9GfkvfkEbrXdZZyK6uCCxXcaNk4KhBu9SCLZbaCYnP5tbw7Ny",
						"LdpLvKqHUeJjuRECVykyLMSVJMF5i5dc9L");
	testAddressGeneration("6uQJ6SNWWqtX1ViktyUvrUnvnQCZ96a73HM88or9A4kkgGrC43H",
						"LYaDHfqcHFGUCHtNBh1SXu4HzFfRCgfeUz");
	testAddressGeneration("6urdBp8cRdf9qddQBwR4PfDtYUZ9uFCBhjNsdkDKdXYg7kWipJo",
						"LKV49p8mKzfGcQZ2pNSgU3ufBaf2iqLt6B");
	testAddressGeneration("6uMU7hGDkHMcXxuUUPNA6H82QyCZFC9exT1hwoK4SucgMhrGVhm",
						"LKrKXpvT185abe8tine94swKckuNZ3xTMW");
	testAddressGeneration("6v7n4bdGfN31LyTosNhie8f9EXL9oQtTLwpdFneaUA5yM1mQci1",
						"LKL3v1EZFwn3DTB3sZjzKqtYg3UJoLh19W");
	testAddressGeneration("6w2Bscds8czhq615JCNsdMb6uJVCYEKr2nFGvGyFMsLWXSu26RF",
						"LMzfoV9KY17qmC9oQU8LJCwa4seDF21isN");
	testAddressGeneration("6uoRMW7WrcvoEADK4zvdwhdq89dXzWf61rL7cVcNrdhc3SQeNqB",
						"LSMpPf8DBQrWyPYNSmy5bTraPDnsAWXbHw");
	testAddressGeneration("6vVLt9PooCBrzUkP3uVAEMMdvWxHFxHduDeMCXxxbCJjV5g7HLe",
						"LZTfMVTWH29WRqCx7dR8QDLnNnQMvLw1Hq");
	testAddressGeneration("6vofKfAmtiqjuVx7FLPdjro62Nh3oFHZJN6ebvY7NtztaZJE2Am",
						"LQwGEdR8WDaLo4oJHt9saqH3tpmYbmnVy4");
	testAddressGeneration("6vgp7FFMJxnhWxJtxxfW6ZkmJBAftj1AGJm2pQLEqkaYwCUaSdg",
						"LWmEG2TkhmuUMEiuggiLUy5Wx7K9qCRNpz");
	testAddressGeneration("6vg97rnJMBuBqF4Q244h6SaGgjgbKEojFzzvgPUZrjngAxGRTbi",
						"LfW1HbeTvVa54qj3zdP9YPvDiuDPGAnDL5");
	testAddressGeneration("6vSYL4NQvsKQyDU9kiA1UJgMxT2VnF1PdhmRHXJ6htM9CWkLKk5",
						"LgBjyr4uPGw8wn77whQYTLrcCFM8MwowsH");
	testAddressGeneration("6uJD2fuq8ZkWLncFq8Hhq2PV6SLtYJZ8NHtbKZtob4Ca8qfkEQp",
						"LTgqL2Esojajd4gV46hhxFq9oEknG1wYsY");
	testAddressGeneration("6vwqwQNHm62N87MrnLmgHUjGG3gRmrtk7M5uXAczVBWX2TPVCoD",
						"LdfxtqLBXd2FHyNYsxo7tDxz5y3akSV1rn");
	testAddressGeneration("6uTt2VSuBjfVnkcA4TowmXL4JdZmbGoLEKWmkAdBwKMFFpyd5VA",
						"LLwS9ubLpZG5i2Hh5FMig7LV6shceBt3mj");
	testAddressGeneration("6vqKCvDLwMMZTjWYfup3kzTngTxjECQa86JfuPXtB25paPN8gEn",
						"La2CrViq4vjikNBgRq3LXNh9pgQH5u5nzB");
				
}



void testLtcMultisig()
{
	std::list<std::string> pubkeyList1;
	pubkeyList1.push_back("024c0eaf4c884eb22191ab91b688a27b03ab2150ea33a7dec88bd6707d409abd79");
	pubkeyList1.push_back("03b13004940d81f0f3c6de5961ecd4bc3fbe5d238068a8fb76f0b384e88bd4bfb8");
	
	testMultisigPubKeyGeneric(pubkeyList1, 2, "M8GFfyYCQpRFiS4FQBAbtP9aqy8w3wEKW4"); //"3247N68EThZpuvnMJJBG4juBXGYV6Db4bm");
	

	std::list<std::string> pubkeyList2;

	pubkeyList2.push_back("025233b9e7720fec6d02f95099f95ef97a5ca7d032fda7a2f68529ab61ce48e5c4");
	pubkeyList2.push_back("02576f5616691cb77fe7ba078264667de14311a39ff051074b68d039f2cc126e11");
	testMultisigPubKeyGeneric(pubkeyList2, 2, "MSK9FhTd7VNMLcW4eS4jrLXrcHLEtUcxQv"); //"3L6zwp3fANWvY7EAYZ5Q2hHTHajnvDpgMw");


	std::list<std::string> pubkeyList3;

	pubkeyList3.push_back("020426c93c79f3e09b7baf6a004f20bb838c722a87af69e1ce90482e890963fd31");
	pubkeyList3.push_back("029d9bf5689d00554d3b1a660167bc66aadc38178d09f04b2ccf0b40876d667b64");
	
	testMultisigPubKeyGeneric(pubkeyList3, 2, "MSELnQXZg7Q5Jir5wsRH1SZX2pWdUCdGA9"); //"3L2CUX7bizYeWDaBqzRwBoK7i7vBXgdkTp");


	std::list<std::string> pkifList1;

	pkifList1.push_back("7YQyu4HhRDqwyoKisgntL5PGKnbR1EVnJQHhnszzPYEZf1vGVgW");
	pkifList1.push_back("7Wv6PANhVUbxdG9qr83DQ2EorsQ2Qpd2qMYPp5opWr9yHqNCZ2G");
	pkifList1.push_back("7Xmh8DLjzX39Y7rbxxzpJG9EPjcZ9BjgEKXxA6CByqRhGXr67Pa");

	testMultisigGeneric(pkifList1, 3, "MMiSNwVWY1sHwgLPijVPtKpkWHqAa4rBsh"); //"3FWJ545Yau1s9B4VcrW44gaMBbEiY5XDZ2");


	std::list<std::string> pkifList2;

	pkifList2.push_back("7YFe22Bd1CPRmuvAzVP7rX1M5wuuJLNFDrdRcZTKyzEb1GyzarS");
	pkifList2.push_back("7YKDDCEndDsqDB2ZuWKK4JBHnW9nGNGvKDMJU2g3V8Sdsb5z8fv");
	pkifList2.push_back("7ZdLeDGuVemtQKRiJJBwVBAbVU8PQnRAoAYX5SX3SxszRiMjGAt");
	pkifList2.push_back("7WbQnYxgHseyxp9r9vLFMHVzDwfzZAhvULYZvLfsrTdnZ4K3v6n");
	pkifList2.push_back("7ZWSZCxHneypsY76gBEPDZNzm75h1MNwqB4N91riJUvdqsopGf2");
	pkifList2.push_back("7ZNjp1n1o2QLx4nRMGPygYtFRzDaM6c23gb3tEeZ3SzwZ22Qi9N");
	pkifList2.push_back("7WSLRUeGWY8qbvCfsqdbeL1YYkPuEZRtzSWufnZesffdXxujUpN");
	pkifList2.push_back("7YtxF7QyywQrM9bsANARgomYnyfYqUF6UpmFR8QnStwqephUTuh");
	pkifList2.push_back("7Y6fMiXaAZkc52sfuL5V7zZgaRJzNEgixzqvkGKxgeYjnkjPQe6");
	pkifList2.push_back("7ZWqzSs1TpDGtPsha7H9tGBGVC248hV6mmhJEQ8eVTgSDd3L1ie");
	pkifList2.push_back("7Y9tXiAaQBbMGT7ikG7h1fFGDZ6H3d5LrrdriouwisQQGpdgdXF");
	pkifList2.push_back("7Y5poZumcEXQkMhZusA9kG8S4udRjbeFywfwazth9JkKh1ADxUh");
	pkifList2.push_back("7XKijjqaucBdahLAMvK9k3bUsUCak3nNJAnAjuMag8DuLAKddb6");
	pkifList2.push_back("7ZwqMXjZWn7nyunfU6YLmMu5mtrfCAyrtzSziQLowZX4HYKczBt");
	pkifList2.push_back("7ZKB7hBFi2AouV1jqqaBVRZG8VvjpihbrrC35sUR2j453kYxMir");

	testMultisigGeneric(pkifList2, 10, "MESdzPXtqJmG8zdyjJPMXWzCyZsFMiz8jG"); //"38EVgW7vtBuqLVN5dRQ1hsjoesGoK4Ws1x");


}



void unitTestsLtc()
{
	std::cout << "Unit tests for bitgen-ltc" << std::endl;
	
	testLtcMultisig();	
	testLtcAddressGen();
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
			unitTestsLtc();
		
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


	
