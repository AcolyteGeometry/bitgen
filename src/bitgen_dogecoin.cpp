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



//Todo: Rename
void testDogeAddressGen()
{
	testAddressGeneration("6Ksgf3XHGAW8YT8Z8DtRnFSwHjqfCtuRymNn6xUQQ6dMbwtoqTe", 
						"DEBNRq1fkoghQ6i3FSASrGCfuY5hNeKrmp");
	testAddressGeneration("6JHjtBip253MQP8oVqcy8TkTTauzwrdMrBnf9fXt5mPZbh3n9Xo", 
						"D6HxNncEwdxtS4WSgFmWmPwez2aGSZ2QyK");
	testAddressGeneration("6Jp7W3g1d5vuMt7R9Am3S5P9pKTneAeQnc2cG6iDv4ifF2oU18H", 
						"DNU1ot86f2j89bPNxomLCnkFzewn6dE2C7");
	testAddressGeneration("6KJqQhQ3MJhKGvcfDRBfPmyTJFCNm7KQcWcKMKnurFvVupnAN8A", 
						"D6p2sZduJZTxKpac8BHBzXBS1XnoUoDfH7");
	testAddressGeneration("6KXJXKut596gbVk6rMbZ4VnVLiGHWKJDbmpytpoEy4GCE6md1WH",
						"DNhiKxBuNcj9ES8M4CkWAYVyhsujmfjjoz");
	testAddressGeneration("6JnVw1ts5aJWhyVfy2P4HDnwtoVuvmwTMtASu7HuZjGns4dgYrp",
						"DHFmnh4kQcF9o5M8LnaAvi4jiqWCUXdFds");
	testAddressGeneration("6JB4ka5SKEwVxPS13vrC8Y8ghEdhT31zuoRztsv2VidLig9EpS1",
						"DSWXjHU8nVdG1cnGavidm4NZuF4X3mAKGn");
	testAddressGeneration("6JtXhFrrAMkbFqjzf4QoBebA3KbjyoRLN4ErvHnqQWYADy7YKyc",
						"DTHJxssZfET17e52twYDUduTXrY37KXkH2");
	testAddressGeneration("6JPxdoquax971QGZBCzqCVA39BJ4tsoci2Y1XJTFeyyTiqcPhax",
						"DGB4Espxa12jizpHB5sZMPWUXnd9nWsf4K");
	testAddressGeneration("6KMasAY3SLW6bn18uMcCAeRCPM1J2QCm2evJraTKDVUqPn6Jz8v",
						"DJWc5tqHbABzf7zrvyGnXr4wXGHsn6DQdH");
	testAddressGeneration("6KTpVWDRYT66BSAwdzqTufERcD8uUuDBJcYorSCG3d7Ng2RfYNq",
						"DPU4Arn7PNqgrSWdHx4nnT7vXLyEZDRSdp");
	testAddressGeneration("6KrAckyqWrpopCB1MQotNZ7GzsA49LrhX8Lt3czXRgNEpShDwcS",
						"DAFibZbiXLmvmKEDR1MkfUDQZfZ4JZRfbc");
	testAddressGeneration("6KqSevBweMiQxNVjeqYH3Dg3QKGi9MFkFqJDBCn3NQ6Qmx7snLN",
						"DHu5DG7dNM3UT2TsiCihDRgEL7xpf4kex6");
	testAddressGeneration("6KVaj3GU8MKeBL55fKQtEHkP1P3S7Tn5JXEMeXewDcMMwZcNLDL",
						"DB7ajgghJejKhfJb4tMkWfSaLbTwrkSPWn");
	testAddressGeneration("6Ja9gh4UuoUz9owDk4pmBhR8PMSwU5KJt55kyvuFfaoPL9eC1cy",
						"DU6JMRzV16T7N4ixfjaJFar28FUrbiR5Vu");
	testAddressGeneration("6KNrZF2zABPqWCf1bQWUuUjmdXGmc3GK9sGTLgq2XLRkSQ9yVuf",
						"DAkur8ZCABdkzzZEhQX2dD3ApHbaiyynbx");
	testAddressGeneration("6JF6XqL88XqLSGYyAEL7oSTUJAVtRoVFLGufejLVcVvWe9MePPd",
						"D8uS1zQ11m7zzHvVUCmv5igmE9bAjNhG7t");
	testAddressGeneration("6Ka6QHmPgge56HTdudZHBkPfoBSfmZf1DScVs28GjaK6ZppobVz",
						"DKmapHuNcfWQDqi7niTPBXVJjJ7ag4d2NG");
	testAddressGeneration("6KkShsz2TfLFY8NH5HudHJBzWtEDuk6SqQs6Re5QRGBckeBKCPo",
						"D9JCP8JeXHwyZxQHzQB6fX1QoYicowXpkM");
	testAddressGeneration("6JLDfiHAjecM4oy7xXNqEP5oR4sXakZgjLTEPytUeTq5De81ioK",
						"DREzfokM3f4RZDWkDPE4PmgvNTqsFcG4Cs");
	testAddressGeneration("6KGL9CJBpqjUAn9i3B746x6fyEg5Jz56RjD3NMJ8Ry38WMydr4A",
						"DJZYqtR2snTcb5uVtPrZEHDrpgy1e7RvSe");
	testAddressGeneration("6KqQFyxCbqZgw139cTNcjgugCKBx9rqLN1n1E5wXHXTTFeUooLq",
						"D92mRCR3edNaQZQSENabUf8AWdJBBQagVt");
	testAddressGeneration("6KUnTzib5pbduS9E4G3A7KZ75J32cR3KtHMK799trqYGnHXETTo",
						"DLCvN6ZwZYQqiSkh2DvTbd5rqoLydj4mBR");
	testAddressGeneration("6KUEwRuGD7wrp3UXhnMhjfRfvPRVZxWY1Gpw1WKZYicPMpdRfN7",
						"DBCZ8U7CAUnPFLpbouBhcGcQC3kFDv44LD");
	testAddressGeneration("6Jf8r2WYwcqe2afzitWoP3DFPCnHsbHVc2vrcPVkR4xEw3Jbjid",
						"DFmauvz3Yg9GYiWsMvKgv9txJCC1qkDuMZ");
	testAddressGeneration("6KYsEymB1Xq3LADkx1tZPCux588GZM6cmAASKdz9j2SArZqBRTo",
						"DB8ssuhZjDZpyBixWQQHQ3WUythNVNH1uf");
	testAddressGeneration("6K9ABXMvBPgoqsAgmL7eK25JiqZL4N3S5oDiP1ShScGGoHeXDqX",
						"DTHKVfheJfWXZxesqzkMhthiNf7P5mF8fd");
	testAddressGeneration("6JSsLPm1ugrYRqFmzHS8ZvR9i5zsgBa46JoKN3vhdL1v4GkRcDC",
						"DMU5uaMPpxXRhSRhs3tit4SdUGdTPCkGNB");
	testAddressGeneration("6JZCL3HVXmDMpKydv4F2Qw3RKHT5fbTHYTN3hZLFh6xyXofRNyU",
						"DJd9BG6oXgnXUk4iLd1UpkheP1EGAN1wY2");
	testAddressGeneration("6Kdh7p5A5LgZVB5oePBBY5HopbpHzBxWYW5hUWeNiC7DSv6b8fZ",
						"DSQDmBdPDHs4VDQrv2HBdtzvvRzqPUA4U2");
	testAddressGeneration("6Kjq8j8xFisH8entJhhLUkVXqksXJwJCE8r2dGVi63XF1P2oTCK",
						"DH6cuuEHNG3mdqj4VQjuWePjvczTFGAdz9");
	testAddressGeneration("6JhMNUHvFFqPDctmjXZ6bNqmVfgs5zfkX4RJMDtksJyN2xFtYaV",
						"DHRTVbPuGZw5HXCTm486VDwiFMSetw5rJo");
	testAddressGeneration("6KaPDzbr8oeWnhXnkrPc2LKmUh4K8cd9w2XoBpaRueTVWQkeUBT",
						"DSihkjdPJJir8FLWCCevp9t2D8VemGFarD");
	testAddressGeneration("6KpYmRLoByHuK3LDdCCAe72DqedawnrVTJFJi4BSLWyeTiyLjKn",
						"DGncWAEPomGcVagz7C7makZZkeh6MuhRFY");
	testAddressGeneration("6JhbzD9oeEZip4LPyB27pY4DkNzrKRysXjpp3Avntbc7xjRAKgH",
						"DCRFSYxhp7H4KkZq9xq34V7rFZfLrJmJW8");
	testAddressGeneration("6Kr58aFkZ6jGa4rSEZTfikj13VxFLLV9yiQ1TdWmYgAyGHgeAfp",
						"D5mpZu8cgNCNXQcxCHxkPSHo756EKYchaq");
	testAddressGeneration("6JCCmNCpbQJAuxtn8bv6g2AnZWfvJ7Hm5dErbJZHCXp9uPxrH9A",
						"D8ij9QVeXoD25FdCGNeE8d6LE1U9Fs9rE4");
	testAddressGeneration("6K5XJN9GWaxgGPzkp1ngHFVUDFu7LBZKvgaS21GxDRLSRqGVi8S",
						"DMD8Wd1gbARprLLg1zNusbqsdy6bMsbtBa");
	testAddressGeneration("6KeqkjpsgGrJSgHmxGeosL3M2qg7v5E6iBjqYwHANrJVMEQ6gKL",
						"DMu5Ws71GH116ShvQ48sStrc7m5oq2pxmL");
	testAddressGeneration("6JaiPRh6Yn8ttAJECxrEDNMxVZqUChcs4Kmp4QtTKyjjiNbqhVb",
						"D6KvZAq2wX7n5iARffPeeRMs87yYs61DmX");
	testAddressGeneration("6Jmb1wFEQjQRY2jwUk2CEytQh1DHP7kNCgZEzcCtcaL5E31dyHN",
						"DNpP6gZmPsZFn5GTWPQHE89DLCu57ovDeY");
	testAddressGeneration("6KNUQWUNsdBjtyJJQHvVnwubHAPju4KqoUEdw65jr94UpHfMR1o",
						"DKS5HfjRE5exwsrUgC7CknBKtWzJSxXE3q");
	testAddressGeneration("6Ju5i1Q7TcNKL9rqbHFwxH3J8Ls9WUxE8RPcXLBDBxZuHN3SPBG",
						"DFshJPjP3pBRofmQWguyFEjNHgYxAwsdLj");
	testAddressGeneration("6JA9xvfPHZ9v9LRrDmd2honxUCywLhrg1TssaAZWYKqAQpcATYN",
						"DLPZXbm9e99wPYqYoq3sijZwADGg8w2dgK");
	testAddressGeneration("6JXkRCo7Fw5YsMAT69V4H3iSzPvXnt7ufiwT3TrUqcGm3ssDvb3",
						"D76YEMWMkFq7xr4UqWA46Tff6xvx5FVEvq");
	testAddressGeneration("6JGhC9oe9mce5HnjT7fxAnAjiCnXPBv2t57cDzixdFMMB5ABrmM",
						"DDCoDrYXmJXwcrCCLpyg9guLVxSdnq76x8");
	testAddressGeneration("6JvbFPy5f1Bj3DWDXB7WSK12tAkQkuF5you3W74Z1g5iKaBCM4n",
						"D6SDDYMVPCeNAKDFqJ1Rj1EauPFEHQhJ9z");
	testAddressGeneration("6KshZzHNZNCHYdGrNu3GWaRPD87A6L9Z7cA1F44mGJyLrt4Ge1a",
						"DUDn11S1FUj7V7x8kck74AVuZWsMCKG6YM");
	testAddressGeneration("6KwMsScPoBiTXnruD9DohBxzcEyoD2pATZWZs1Y1PnXJybMWrtG",
						"D6RPtidStY5QXwdGjH4k6qf9zmEisCnB85");
	testAddressGeneration("6KVzg8LKbYXWqDqowTLLyhn7NjvAaMC2vdhvpYaSPjfPcoSoUeC",
						"DJjvJH3z2C4kz6a1vXufFjsHma3KucSMQd");
	testAddressGeneration("6JoNKtUmiHz3mKbYNdLJWZR8Ep7fMhgDf9c59j616s8HTTaGyJy",
						"DHTaZhPK2MW3cj5remN8gtN1r1SkxZEup6");
	testAddressGeneration("6KYcfXTvNKLuzJzZFHTcH4AdkUVrN9DvyXcJYCgsduNGgpQhvFq",
						"DHJxEuwGfLHoERhdvkwJtJhqdotydC5QaE");
	testAddressGeneration("6J8oCZm3uviig3cmB7D85JBnUZpjNRJvwwSX2XSF851Y5orjNui",
						"D7VKmmScSfbw1oNePMjcMkNrwY1aeZjBrB");
	testAddressGeneration("6KaMv4DzA9CmrDC7EiwHMLrrkKAoau5oahEHNmf1yMDZwtNtMCX",
						"DH2sE4rkY6cwgB8aFZkkbvD99roD4A2ZtZ");
	testAddressGeneration("6JerNn8mkmVT6f9bvhhPHRVTvYUkTBJDkunVzkxD1tYtJeko4XX",
						"DGKCjDfjCfVJewonvCKGvCWp7mVW8UkAhL");
	testAddressGeneration("6JwZkhoUrtuCTrENAoC4gqMU8CNDU5PT9wafwNdn4Sy8QPQXXzv",
						"D8Qo5L9nrGGNAWbjez3KxU9Qg5hJGcb2gX");
	testAddressGeneration("6JLXFLRpDdSH9fZfYneLCaSb6B2UYhhX7uvh27CEvkzkw4oLypa",
						"DN3rQFvLa4o2u3xJfKgFEez3bUiX6SxLaD");
	testAddressGeneration("6JBekPGwjAYSm8FEFqiFmZq7dxonpphXP9sZ9Vic3379ZVGdVM8",
						"D9g53WseDFoz5ZBadAYB2CttTrB5Fgkb53");
	testAddressGeneration("6KhQFxwuZSAaQrx2nzFptXFk63ZLubD4F3C3DPKLZvd3UNRXWCG",
						"DNaB54zh3sdFjw3WdfMv1bCSk3ivPAkxGM");
	testAddressGeneration("6KJGH2jGdecEkyxt6XqnotBTnf1HsoPhhsjcK1nqy9i1g964ZF1",
						"DL3Rmt7RXPFPEXLvEQsazdi3iJBhWzBLq3");
	testAddressGeneration("6Ke8MiGnyiqZGMhyknt3CqQvQexDNPW4PV2zaK9nsqurTuNpZw3",
						"DLTnRxvjU3bPUMKH4EioHSzi1XQJ7s88ed");
	testAddressGeneration("6J8yEGkB4QmUXUA5Uoa1wKkYY8DfQwRfN9CfwGG4yDpXKiANNfJ",
						"DLd5NG8LH2fj4PpSiiBjYTkBJazKz8Gd78");
	testAddressGeneration("6JAA6fWnaYGNQuSjcuGK1kUZfm69oTmuNkoqXpjLAz3be9q8ckW",
						"DEuyVyne6ph2bTsemD5nVKZvzP9MA1TNcZ");
	testAddressGeneration("6Kt1vtfPCQE5qDW5i6bpcVhGtuGBaxAJB4Q4aisvvEcpuVeat5s",
						"DC4ytM2pNiViv6nCZjeEUSPnKvPMBYndwW");
	testAddressGeneration("6KLg73Gkhw454zQz5TF1jwkY4MpqyYJFAtLXUrAg6YZeCt9H7GJ",
						"DKhNU1pFRBpvCp5NBDVqheEGdcmquRMdfz");
	testAddressGeneration("6KUym2csxdzQRAG96cwzaefyMCsE2x867LL9SDPqzd4BCtPJaTu",
						"DGqDqLUhw7Zc8sjyseQc581jYBwSUD2TUi");
	testAddressGeneration("6JWjm3yPreLmHbqfDiejEkRixB8vpzcmVvkfTu8XncNtehkp83A",
						"DAKgLEyspNpPuehJ7X5yf9tRTQi19x7Zz3");
	testAddressGeneration("6KCHT2pvZkxtyr7Zv5pwF7e2PRF89D58qLx76X3gGhgPvpfKLps",
						"DCeSgvHowDVeQ6Pb8EzNRKGXjX3oQv7hdR");
	testAddressGeneration("6JRvTZPApTPyJS2gnCYaChkoxcnmVraQug6poWLBdkUY3iMRVso",
						"DLXUV47KA6KK5me4XH7LXt8cjQJTtR7uAs");
	testAddressGeneration("6JqyEsMdZBY1jMgei8XckeC2bX4XGiSVGYFEcvF4Q9AkKsopvWh",
						"DBR1SqbgEXmSqHqFZzePPx6WtGwvPAwb5z");
	testAddressGeneration("6Jox4EMh49oxmVASVc5XzGNNegxNmMh1HCZUdQX6sAEQGh2WQEm",
						"D5UYtrmtrHFfp7WyJwi6DzUGUPpXEjTPoK");
	testAddressGeneration("6JyrEsoqXTZTcTSsetmdAnshWYjm3MhGQKQEWZFpX8Xg83yKrcy",
						"DKBg56gxmkyVtisJmo2umXxDSREuMC1nYz");
	testAddressGeneration("6K5riS4hp9d3dCLNWH6Y5m5JCViGXBRJ7LbBZhX59VwSPsURgTp",
						"DB4DL41fMrJYrxxrLAoUti7ZKHoFkZxwZp");
	testAddressGeneration("6Jd5sesUDXMkb1Wdq5q9hjdhxNoCLbbqqCawGSFELXdWswde2XK",
						"DB2fXvUfSV5eCPYotjgSgqy59ibopqKK2v");
	testAddressGeneration("6KoKbXyxEqizmkr499tQc5vdEwh4LiqNJLzyvLQxfrZPK1hht8N",
						"D5ZSmWHQn54Xjkeh9NJwB2M2tFGx2fibpk");
	testAddressGeneration("6Ka2V4zyAa2tXZ4dNZcEH4Vqh2nAk6sxyifpwKrM2LnyDryvxCN",
						"DLc9E21n3V1WP9rRfonxqC1zGYAgq6cQV2");
	testAddressGeneration("6Kw9EoqpvTx7ueHZK9BtJ9pyCTaKHXXJs356bRL5Ps3BNWnd4Z7",
						"DBL5n32QNCDhRAEXdyNWXbhHsbiDzXCiGo");
	testAddressGeneration("6KmgR2qysuXEyPCH7YQoFfBrU9sqZ13J6beJVfwaWXCQZumtiUB",
						"D6b6TWWQ35AEeoFe7aHsYuCsi9PwY6uRHC");
	testAddressGeneration("6JPQa64hiiuLmtqZdsYR3W8XT1VVoJr2pyxv4iMJQmHhwPPuwSE",
						"DJcavsYtJg6cnjjNuLLBN9ZqZTUY4SDH7p");
	testAddressGeneration("6L2dkm8iT541dVf9neeh7oCnvTBpLDrqaXydH21G2oApV7Gs7Lg",
						"DCqMiR8Fhurb7VqqLz5bRicxYXnqPxQL7t");
	testAddressGeneration("6JNVQ6fq5oUua5QZNyZ1qSupnSM3P864DTKZtnkHkBgpU7Lfen1",
						"DCkJtQYwQhHwQRtvgFhtPAcTTTAfbHcM5x");
	testAddressGeneration("6JmsbakKXDSgYjKQacRSU6EzX4kSKrzCRoij2bWfRFpsgscn113",
						"DMPfBfXNUFEPDTLBnU8E8KVMvXQ8pXfHmy");
	testAddressGeneration("6KHvitn4HcEbcRS8uD88DqSn6GY1ydL8aEGPpxkNVMcMd3YrYbp",
						"DFWF73yeP8fkCkBP9Jek36rJ1t6CkkzNnb");
	testAddressGeneration("6JxuX7DjH6BgjCnrJhuGa9Mga5QJZSCWcvgGwRWW3E4XxkSiPZa",
						"DGE784624fkEJWEvHFFKa5qb3MjVkmkK37");
	testAddressGeneration("6KkdcYnz1K2G629cfFaNxEwEqd249Nk5AYJXiDpXhQgpGFCB3Q8",
						"DNGYWWEusqGc3XjQ9pBhnaMuqoVU2fdPLg");
	testAddressGeneration("6KjmN7NUJ54ZL9aCuNT2Jbk2ibi3EskCo8XjVMGENJNko7MHpKr",
						"DAKxof8KMLTUtfFhHWrQgzefTaFpQ7P9AY");
	testAddressGeneration("6JSjtz3yDzaCk5mkSivMhcowYCQvTDXusTN68Xba35KuNXwo2B1",
						"DCxcco4GkhorxPSjErJmufY3buq87CnAox");
	testAddressGeneration("6JB6QDp8X8RH4hTM1fBH9HLRCBMxdxyQKEwf6jwGBcT731D1ytr",
						"DEfRJBvpPrVsLn5XYxiH9BXqGWrqT5Ectp");
	testAddressGeneration("6KWR8EjwQ7w6eRwQ6Rk24CtG9M6bkWvsA2EANaQxoBrjtEds6J7",
						"DE7V1aRBZ59Ht2FGtS2LRpFvY3qmBWMuzE");
	testAddressGeneration("6JN55rM9o8VsmjCusYuSiVYhJfA5zFzk6WTU1v8yaVjrbWjqdTn",
						"DJJjS3Fx6ToTrPBvCysqWAg4JWTzPkEgwR");
	testAddressGeneration("6JGMB4ZVSzXDrrDk55Gydhasf2rZtcnWFdza5irtvzmq3zqV5Jk",
						"DQ7gdKgBudyPGSatQdN4ba8epiBtXkuGEw");
	testAddressGeneration("6JXDSy3V286QK7nL6t1bpwVQqvPFnTyKxQ2xe7w7y8SVaWkhQj7",
						"D8MSjRbkZvi51jgELqzGyGZZYX3xpW4Y5i");
	testAddressGeneration("6KrYgSUc4nvCCb82gzo8MwhJkKChJXaKLEnykZpCmEiD6rmvC3o",
						"D5dipJX5LnYrSn2u99bddzuYDJw5asfYWX");
	testAddressGeneration("6KD8rg1nJ5z9AyisSKtzNa5Uiqev73BunNWCADytsabd1f2KFV1",
						"DGETrxaoFYSG15VMAonWqwaHkJK8GeY4cG");
	testAddressGeneration("6Ka747euLe6Vhya5W7zjhUNwic6brQiVYjdTeVW1JofNNNL6byr",
						"DDCG6qXsNK5MG8rJQTrtS7xoNCnjJ2hjJ2");
	testAddressGeneration("6Kz1JCPCV3fxnwwUBtfJBN4CxogENhHdc86KFdodA5Zt65aSnVa",
						"DJuzyoxgN4Q4ChczZw3LN8dmFxjhXFya1C");
	testAddressGeneration("6KaHMDm8QLSc713gj1FAiZGXuxzXagPrHQGUbqnzFPfB1h4Z3sf",
						"DRudkTbVgn7868dRa17g4Zuez8SEB51GTW");
	testAddressGeneration("6JWcwn2m1r3zrjgUBShotVFFJpAoY9XS1CXECRXWJRZTRmsqGzw",
						"DJwQKtrQ5WeyZaFf1Gyucv9NC1vuKCxMQb");
	testAddressGeneration("6JwLAVCxvufiveUcB9ors5KnubfDeYAj5GWLkiLTdmhPEimWHT9",
						"D5EWibeDxdyPxSCetkJWAeZHBd1Ds4yvU6");
	testAddressGeneration("6JMmP7oEyazwMgo75FHrCGf4XLZgLmQ8XvHceBGdxabH36kJYiA",
						"DCBgRcFqUGz3QYhWSPw8taEp3wuPW5JYov");
	testAddressGeneration("6JBfZoQpihqxzkxDEGRgQq8fKUP2AeujFZdYCJFsnzEjAZVdFLa",
						"DFbvhvKeC5FqSRSHyVuZaUiDGErZjMkYJH");
	testAddressGeneration("6K1JVV8nY7tWCK53jCA27vKQK5oA4tjha8KwBGfHzFK8sV22MPp",
						"D777oXZUAcKtw8Nm4x8jPr6ThfJVBjmPfP");
	testAddressGeneration("6JzR5w3zcdhpqCis4nRjjpnsDvZ7ksYsoeBiUwstH4FR9633asJ",
						"DMtzQzZXSUq2jTwsRbi8BtKgEs5QKsmkp9");
	testAddressGeneration("6KYowLHkHL478Qpdam4n1fzkDsUTCh34yUyYJT1HgLssRRoqJ93",
						"DL854B6NaNPkKvgUAXXPgrhsKsad3NwR37");
	testAddressGeneration("6JpHqKxR6G5Suit8EJ8zh4vVg2y9ZedzY6A9heEV348StFW9tkr",
						"DEP4JBYHaDitpCFcJaBdFBHvmBq8tZzVpU");
	testAddressGeneration("6K8SmYDjTmPGXWyEUXR4EMoCzLGK6yWsrwjJyQgX5DQXJUJBQGg",
						"DKSFp2Uz9uNYFu1qS6aNMk1Wib1Y4DiJsU");
	testAddressGeneration("6KuTxQw7hDBcWHvzJA35xQwt2QfDuR2Pn47V8RxzGF5foJPnjTU",
						"DJ8KgC51Ken1YnmB42pkMc6oJnD5kfahf4");
														
}



void testDogeMultisig()
{
	
	std::list<std::string> pubkeyList1;
	pubkeyList1.push_back("0339D4C31766C3B8F30090EB7775D703948E226C386282E55FCF1C264426EBACF0");
	pubkeyList1.push_back("035897B947207661209BFA1EAD2E0101D7E2C01DBA60EF68B5805932AFCA107891");
	
	testMultisigPubKeyGeneric(pubkeyList1, 2, "9rjP3TzjMh2SYQeXFdYHr9H1UJVzDPM264");
	

	std::list<std::string> pubkeyList2;
	
	pubkeyList2.push_back("02F589C3BCD2AE19A49B4AD8F72A1F2313FAA31F131D50E6209C0F19B82ECB36D7");
	pubkeyList2.push_back("0305289867FE648A2078B5D1BEDAB9532CFCA10AD3890625AC55C4528F312E5B2E");
	pubkeyList2.push_back("03101F6D7912909C19ECE521EF208E52F0B0E9337965B87FCE5BF5DC5F90F343C2");

	testMultisigPubKeyGeneric(pubkeyList2, 1, "A4EChg4in5f65wGrVtWLJeNaUfpHZZDksW");


	std::list<std::string> pkifList1;

	pkifList1.push_back("7YhHqUjj53N73hYeMyT9vzbZ3rmTQyAPugYFNPoWseiBWivBMYZ");
	pkifList1.push_back("7YLD1p2uKmRbA4g2dhaws8BJwYYLnrQPfVbEnVFpeZ2oGA3g66W");
	pkifList1.push_back("7XNJ2J7W1xxtmmFN2fZby1x3Zps3g3QVJP3YrkqjkJ4LawBmgj9");
	pkifList1.push_back("7XcCCCK76tN6aUErsupzJuFF8gfoHMTYWGqjGJrcfWqmcM4tCSa");
	pkifList1.push_back("7ZQiztUgNxJRN7MJBkRLsajcJy4fk896TubiNFgjr8Hagjtn5eX");

	testMultisigGeneric(pkifList1, 4, "A63aQmLgbdjgrZkLCqtJLN3HRbJWR88x8X");



}





void unitTestsDoge()
{
	std::cout << "Unit tests for bitgen-doge" << std::endl;
	testDogeMultisig();	
	
	testDogeAddressGen();
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
			unitTestsDoge();
		
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


	
