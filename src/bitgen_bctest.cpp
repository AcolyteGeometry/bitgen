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




void testBitcoinTestnetAddressGen()
{
	testAddressGeneration("92ESVyQ8hJZVvLxRgitCiuiQG7r17wwDTbhP9TCC2jDBGRYjGcm", 
					"mrTgmcwMqV39W7N9F69Hv3M2L7nEocwYKS");						
	testAddressGeneration("92Vu8xndqmgqMqawLhPrvi2VA7DVeyRL3oN64CefqJp4Zkse8k2",
					"mt2ahtQDqqwQCRB1tzwjDC1yx35nCzFkvc");
	testAddressGeneration("92SFBvrBPG1H6jPDBChxpmuhVCyRcYm5eAvUt91fUjzfPddVYRn",
					"mzQ4D5U8eLhQQ9SJ67mjTjKm5VrkEdwRwV");
	testAddressGeneration("92juJHrZ7kUirJ6B1AacjpA3nz5V8Fo1m77gg5aS2tFwD8cHFhN",
					"mtF6HvfMGcBAJ2ogCuqFurcrvAoeBMtTs9");
	testAddressGeneration("92FKRK8DPkG4EPD6TSCYnhR6g7g2n2rCEDPtk1PwZBEKPmNYLff",
					"mjbZ2CJpSvFnBnarJkot7jdbfgYoyqCHzm");
	testAddressGeneration("93Nx489iY4An81MSYbvn2HXGLbTZDiTm8Y5oBxVd1YAcqVBKHNX",
					"miHP8pvgRVrVR4QEzxeaGLgvX19CpkiqYg");
	testAddressGeneration("92uqHsZPk4V4gpvU71S97BApPGm2iKwHK4YBuKsBqvsWw2FsgyH",
					"mtqR3y6bmHUEFjpWYhBHYe1Pux4iwMkGFq");
	testAddressGeneration("92i4mFd39rd7JBExcsKSD7wLLEqeJCU7CK4cgG3TFNLvxdjKufT",
					"mmwmFL7NQA89BhmYHpUZeX3ijAjEDdF9jY");
	testAddressGeneration("92NcF9r53wn1TRy6DQ18hdFZoLKUx4RxU12UFkbUHyYEKTcT9PR",
					"mjLMs74mDkBcUrqfi2Z4C7mg3XiYaxDv3B");
	testAddressGeneration("91msTgshecQp2dzeT8gziegcCz5W453zH3UmuJjA92rV5Te3YVh",
					"mkD4B715q6bgj9otj3R9oAQLSsT5bgT5Mt");
	testAddressGeneration("92uhrkG6sE526SFRpyNU3v9DWmbsASzQ5mfF1sEGgTPknML19Jb",
					"myzcwM4fMozaa4sZYQ8po2WAfw8KTNfVzA");
	testAddressGeneration("938BW9QSAAi8Fa7JriDMUpkku8kfAcBFTGNETxMpvWCZGRzRECW",
					"mi6car6FJ531h6yACprpaEptD9dXhCy9xQ");
	testAddressGeneration("92VvVc2gJ6THKr6gy9NdXfQ6htkbDtgrBprv15wiKjpVPXM5zZx",
					"moQfc2XE1QttzczgfPWm3fftdCum8dhLsK");
	testAddressGeneration("92cEQsAfGT2WRFcj9Bm2LrXDhDwwhKki6Ug7EPrpGjoXnBSFJJE",
					"mvCtUe1ZEusmNPLxgLPDZ2jaGAxbawH8a8");
	testAddressGeneration("928Rstx8RursU4LCYFb8g4BzTR8ubBAySXmgCTG8FKyAJDy6FUW",
					"n1uoq663WcMuit1WMeTymZfcKDBGDWyZrq");
	testAddressGeneration("92aaGTtKg6bDNVGsgR1PjrCFavy9WSWUEua1fY8EPFg9pE5fB7T",
					"msHi9YyyME9HvyvN5DoCQHvGr28j9Mns18");
	testAddressGeneration("935Rc1DMt3yar1evmsUD8gpN4w827JBK4hUSpQgVgs8FA9T5AAd",
					"n3kNaDkrSNmsTRtqn6pExHGbGuVvq5LUJy");
	testAddressGeneration("93HpEJX9nMLHthEutKRkCn6E2ewjn7CRVcKCuyBHiCDeVvCahvo",
					"mnLgSeYkgWFJhzXCQtNYzGwckC7QryR1Gb");
	testAddressGeneration("92YiNAhRLQTpX7HgzV1viPBimafDoe9FJA3Yv3rZAWPzfYk1DxX",
					"mtjxsYkYts6Ki2eUQekZoo6jSv2zJtU736");
	testAddressGeneration("92PWrueg5UDLQ6rS5kHBVbR8vAwxmF2i7EFY4K9kUdP8Bky6XzF",
					"mpv1LFNtk1dE5naVYXT8aQxgTnsXiykrLK");
	testAddressGeneration("925ApmSv1D2qZEEx6mtJamb8hrKqWP1pgnHUX4X2i78fCnk6un2",
					"mvZ6a2vuTfkAM24v3eFH3zt5tNw372LE2R");
	testAddressGeneration("93NpuS9cwJhsHtWKcuT9UALNpmUWuHSGVxZw2MF4VwGhnzcdBBQ",
					"mzhSVK6ZP2jnP93rKQ4B24UA9txbyJiCGS");
	testAddressGeneration("924cKejFrW5kzyrMcLKvK6ko7cDkEuZRFCC9Dedis1vXYZANqhd",
					"mrzR8y2pPugsduHxkxdXPXrts2nmrSsAA8");
	testAddressGeneration("93VvawbyaYq12dpyqwu2m1dBegSEmeYTXbShnv9xUpmuhHvDSEB",
					"modLuC7uqWShi3WDBrhcd7V2d6rwdftfZB");
	testAddressGeneration("93HFoZNQgwdQJJKHyx4WbA7zK3VP4JYZ4GiAsMaHM3KAAr2NeHZ",
					"mmWo6pv6BXA5fkj7pycqgcLhVdR55vFM36");
	testAddressGeneration("93VpNuEtg7nkFzidH8FacFTQNy5f652irSkPsF5BAF2McPZQ1Xs",
					"miBgshDJL8CfGKKF5Hkr4yKkTgwrMm8QTa");
	testAddressGeneration("91iURzCRhvxAcgrwYFvtLDvn7TLdHd6h93yAabW2DB8LJCsTxBT",
					"miMa8ZeCVfeoCeFDbaWRqq5HGJ9sjNqNyB");
	testAddressGeneration("92gRCADNCYUF3CmKqAnFTNKrsZYUsLdJT2NKVzncLeqEvdGNpk8",
					"mpNZnBE2NzWhQN8ETnrx2adNFis2D6cRKm");
	testAddressGeneration("91t1wKmSVR5ME4FPq23cMnh9AV2bspubSwo5xjk2dxWmQmCCRtK",
					"msGL46NrQMZEk5oc5CgLeURtk7iiXPyUtd");
	testAddressGeneration("92UKy5ZZHuEXE7qCzVAoDmRqif7bK7DQ9fR7EQWoaVEMWR9Zmgo",
					"muQSatt4XhBHqXm47dgD76jcXb3LjujWHY");
	testAddressGeneration("93VEP7ASEJhxSyzCsQkJhC4imHbg3Nr1UcjWVpJ2QNvJ81DBStQ",
					"n1H1hnfvHmp8C6fnhGTraxXD4EeGDLkqu7");
	testAddressGeneration("92gfnDyjZpHYhbbV7hiX7e5Ser4soaZoFjea1Ci5LTHiA4c4v74",
					"mhKmf1vgk6HsmJAKqe1PtB1hBryBKcfGdg");
	testAddressGeneration("92ZDnyFUXL5TaRaC9wXGdmLU4PAuhBrLFLU6MpPBkwzVVMfebHu",
					"mufdoED7Nu7N3UEdUAxEyHPCFoa7SMeNko");
	testAddressGeneration("93Mg2vmerqTKsPBLSsMbvr37QavVabZvFZSnP6W5RW7bwWj3m5D",
					"mgpncspPQje6C38HbtdQo5dpeen55zdJLP");
	testAddressGeneration("92PqaZqU6PKcFueyf8nWzjwuAkkJ8FLpyHvUkPgcnt8tTNRP6Z5",
					"moHyq8iwuL3F5vHaZLFuap3MmxGLgGxupZ");
	testAddressGeneration("927Axi3ESWiMSQHxarM6Xmqb3n3gwB5JEZ1EJ2m1oqgBbTyr6b6",
					"myPkGaMSaCQfnf6REE897Y84cmvuTxEXsh");
	testAddressGeneration("933LyKqCpnfKAWP9APwpmu9dzoa6sfQK3jtNK5rCVUjPWMuAkxi",
					"mnFSuxMtSDxTrUhmwMBaT4EVfuQ8LHG3Vu");
	testAddressGeneration("92HKPxs6QuNQ3egAFnrWvA1AabNXPZShfqw6SHE127oMm19nPvq",
					"myXDp7TpFDz6cYTE8pgmYfQ7MRHPabDHJ8");
	testAddressGeneration("926jd8xWh87WbGgt5kHEbvYi5Cp9BDgwoE16eeB1gDk2qdWtZkM",
					"n4ahBZjVKUaLnaQxRCMemMtfSvupTExwRG");
	testAddressGeneration("91nSLnGN4KLbA6URywdKsCyUWv7jZ8no6m9xuSTj2YnQRfYcGJM",
					"my7yMLccJg8YjK8VMDHyiUafEdbRpurdkk");
	testAddressGeneration("938T7AReoaGy6B5g96WMvmu7Bu61kaGJt3zVUjfk1BBwkexf5FL",
					"mhoTeW2mpkWY677VhEcfQL4KqLvq7NkPcL");
	testAddressGeneration("939Fb2N7xNnhrDkCNptfmFGb8Xiyi6schPiHoMJMWs1w1cP93yh",
					"mzFFw8rDPEsaj6nz9xwSbAhDdHjjrsZiLa");
	testAddressGeneration("91b74tG5VauxnXJPUGp6mAat4VY6NdGas6Xb1nsnRa8eMX8CJ5x",
					"mjXBkSKP15e77Cca9FdHbamtUKmT4evA4a");
	testAddressGeneration("92n1PTkKLCqgH4HB6JJV4TGvdWkr8ikG17Rqwpmwy3utKtGqkFx",
					"mp1PJpQcEynJcHUeaQ1SHfn8PcVaK3W8Me");
	testAddressGeneration("91bE4jghzB6oS5i1cTAuBLs8JMrgkSaTtLgS2PHkcNWfEasFysZ",
					"moGvT5FED3ASCTxofonr52VqTGFRbzvQnd");
	testAddressGeneration("91b3AFPhdv7sbMhRso8vnWkhHyVfmrsCqmPuefyv4QrMkvoN9xY",
					"muJBfgiUyznwM7PQRjcH9xGwcp6nUDognG");
	testAddressGeneration("92YSs1GkyEchxB2qJiL3PVqPrDtY1b9x8YzH6vPfPqpWFreicHK",
					"n4TKUbkn3mh7A87Qf86i4DShgg647wVMgy");
	testAddressGeneration("93GES2uxP2yxVfwp7AZAs1JGYSHpFJY1FLrdeAKadVA2WAYMr1D",
					"n2u26jW8mYZ46SwMGxjdMeYgnUnRFTjhqy");
	testAddressGeneration("92r8dZDyhQU6H9ntUy1P7aqgi6WJ3CDj6of2HZ52JpZz4pHse5y",
					"mwTyLqcnbxQVQ6HP7zLMsLTVMfJLtNFRzS");
	testAddressGeneration("92FoTWL77aWLY8Zt7eotYckCfB1822zUEPzcs3Nc4QAV5SVtLeg",
					"mvcEQ8E6b7M7aLyXuk4k5551j4sJRFdjdi");
	testAddressGeneration("92MzTfagzz7dD8fEBGu4AjkamnyeftM1WES43sDVTJFGV8bXJbv",
					"mw3BxUawQ74cmmwNerVkhcqcJzwYYNdsvR");
	testAddressGeneration("92TFEmSpn1xk5beKvB4kFFnKwBoqAnrMARv7rNFmNqUkANLHYSc",
					"mr57tWGh8HoX149BVxRThHtRLGjZ2HLeWU");
	testAddressGeneration("92zKKVNVDBXNW17VvpPWnLQaP4yrGtZQMRB3XhxArHoVuy5Qxrs",
					"mjiPTRQZsFEzFoTdK6uXrdufpxZ7mV2BAw");
	testAddressGeneration("92Br45V5FEgaBZWrK87bj6g29XHZuG7fy1Fyeptc5k7mdLgcfo8",
					"mhxCcwdFqA1dNBkJuhf6WfbyJtu4M2Br1P");
	testAddressGeneration("92YXP6UPVuZWXZ2jo8fnmmGxHv2mZ97P4JpSumamqjcWUC9ogZW",
					"mpLEzikSk6CG7q3mwbxu4J9VCEDhoW7gSU");
	testAddressGeneration("93MDMJmCUJjtDTEWXM3QgMFSWk8R3ZRx29jwVkHyfQo9spSWQHm",
					"mmxZzQU3G5K3N1cngLKJyauZhp3Eghd9sc");
	testAddressGeneration("93Skqnzt4h3ZbQwGVPnWNBytfhUWTLnye7GmghxeSbJqgiKUntP",
					"mjd7fka8TpJteY5CcrZXT83GZaD1DBJuJ5");
	testAddressGeneration("91oGe4FQtSzTnMhGpYZZbfy3hXBAWHF5Qqs9JDh5jdSAL2p8eMg",
					"n3kxgy5JvwCsKUFBejbUH2VMWiR8ZGfEQu");
	testAddressGeneration("92JnBBjKdCXGdrU4afTPs9ATzKj1hpBuZmxvRw73YhYQ9B71Lrs",
					"mjKSZmeC8gGuFVRej4javnpcwKcmnvSKBq");
	testAddressGeneration("92i8TV8pvbV6MQGUKLHwCUjY98xJP4VDa1eo3DyWuv3rTi9geE4",
					"mtKtLBw2fnH6t3ni8B6xfMLqa4PJ87iUHG");
	testAddressGeneration("92uJKLhJxHDuEuTNP3LXhcGcthMggseZ7H9GM2ps34SYUWsmR7P",
					"mouYknys55sn3PwvGLWWhmAVmfo7yvoJgz");
	testAddressGeneration("91uyCtTc3sqZvqojqyJADN2jw1TQpL8Nf1p7M7ypAfb8JbF1W3z",
					"mv6Qu9FByqjQw61bHWo1V1J9P5MPVfpynb");
	testAddressGeneration("92EMkQxyJkSnbY2MnDLsv1Zy5N9rGamGhV4Mzv7tD6GxduufHDk",
					"miZMDUUVaBjTZ3MpoiPWDUJVGqPaJLmEZ2");
	testAddressGeneration("92iTtoUFaGZWVBK7JNkN1QJpwuxsLtXjfkrhokVY5uqcbxnbgUy",
					"mnqNbseNNWhCAZr66UWMmwr2gJjncrrMzn");
	testAddressGeneration("93HDuj7MECyYnfqWXaxi8WyyR6nMVECGMJ555nnNSRrD1FgCs3Y",
					"mxAx4Ewwuf8uaXbZj5wqeW5EuCsjFmWWcV");
	testAddressGeneration("927NZQmACzTU4tWbaAFoWrrHKjcPjcEZU5F8cRjnnCXYv5dBKX6",
					"mqUPaB53VH84vdF45ckC1MeMPri4bk5GcX");
	testAddressGeneration("921WYPMCFK8YfjU4we7ByQ2KCzpuW96AfQUG1pfrTEoHyXdJcZv",
					"mjkiF6s3bDsvmDAvS1F5XtHVseejTA4Ngs");
	testAddressGeneration("92aPvarMQTNuo1gEDUfkwjYaRqRfdMPdKFY13MRGfLKDiH2shR8",
					"msZS6ZxZ2fzvYtbEfsxzU3iMGdxLyA51GE");
	testAddressGeneration("92Nk9Hoiz8nSHoxPuhNJFkArtFujkdMArR3CPuXvKQzDCdfdQoj",
					"mm2RusbBxBnc3e2qsaS8o7w5xMAXYjB3j2");
	testAddressGeneration("93Pf1ZaDe9uHJKHVx9nCyV6tBZxFmZSNCWsJR7UGSpGWjDep6zy",
					"n46zyxDm8NntyJXrjZqdCW2PVMh9EmBqmy");
	testAddressGeneration("93MoxnjwcXp7UPCE6SshZ3vXEX1VLwGKRjuZD2qMtkSH38jSXGA",
					"mj1N52FGboH4XZPnov263jSXKeHt4FBqYC");
	testAddressGeneration("92mh77PTgXjnbyRkdP1t9ynCMCKtUi3ZZmbVERNAjeszhMP3uBv",
					"mfkqJuuUgzYk8qb44kotHzbjrisCqaSLVS");
	testAddressGeneration("91vjjzuoqHAbUBwWGvujCbis4J4pgyq49WmHmGLAbpxGnDceJsJ",
					"mjs4DbZccisSg9Br47WtJpMMQ7KgvDmywT");
	testAddressGeneration("92gWSe2QM2LK99T8DLtKgteLqhxYtcszW75ipJNsvVbXNfut9gf",
					"mgum8Fh7UM73cRwTqtH8JqzHBMRFFGg4Lb");
	testAddressGeneration("93LsZBL1upjRqHMX6u7iXE29P5hEhpnMJ2P88SL3aP1kbCqSZ6T",
					"mkPERCq3TYSUACAqkJvfpmRgbBtkcVMQ2m");
	testAddressGeneration("93KxXNvAQB7Je3NBmLUCYFKgVL9SU8GC6Up2XcGvEJZRonrBsA3",
					"n2rsFUqumy1x99XYyroDq6i8yz58bYDiAa");
	testAddressGeneration("932pnDnrxW76QjgvdadkRCXfvgd9rYbhaiTBLQtoSMNFHb9QBmo",
					"n3KSku6MEtU8MnMhcGg2cuZTc2PbAPKFvi");
	testAddressGeneration("92RiVGHNksmZVS2SCRvbZv5ZDPd7C3Hc33rzFYBEEynFuSSXg6M",
					"mrfJfwLN9ta6qeUmWFHGYV8HLzQMtM3wyf");
	testAddressGeneration("931eF4p699aX7i21SuDEJUw4dxtty7AAEoqEq8en94GHvL6bzbH",
					"mpniWUax9yorGh5YUmceG3CNLjjuffxJtm");
	testAddressGeneration("93QnjiLWKkKKuwCtasUbte6SgzcKpjXjh5sMuGXoR3hX5NcQd42",
					"msVHqe9eyDAa1G2i5HSoW3WsNgCwrrFVba");
	testAddressGeneration("91crm2XdGfKhfqGihjs6YaNZhXUNB1JsHi89B3Kgw5GprNokvDn",
					"mvvqrP5Ly5mmKCwTFpRoR44574eJJJTcjz");
	testAddressGeneration("93PkLXyzqQZMifQoXYwuyRutSyzX6pbFK3t2LEutXTDJeu8iWqx",
					"mj1TxBhXKNuSk3r3RLg3npxfSxVgXa8xG5");
	testAddressGeneration("93JsJm1D96SvQvutk1VFrfc39oMNPaXCGtA5LPC5EJXmkp2FFcg",
					"mrv2pk5THVhArxJYuHruRrvN7eMEbm5Pkh");
	testAddressGeneration("92qbS6cgVmjab3nD9T2SGADvsNyj43pUB3NFWqMoK98wWwwAmZ1",
					"myyk4nTqWpy9AFqd2Jo2N31eMvWkKBnUDE");
	testAddressGeneration("933NvNfP59rifwxrUG2sVajY62mko1BsSCDUo5h6UQA6QBac5Tj",
					"mzdm2YyjvwnxLPwq3GvDm2c9JUdVBgSf2G");
	testAddressGeneration("92jStXGgE6zdiSscdHgXGgbLGt8S3uWdvMX98Vhiu7qWTef8GNu",
					"n1ckjRP8XzWkZ8kNiXw8m5qwG7vTDD6q4M");
	testAddressGeneration("92PSkgPqEMKoNSX9zevFvQxVvwxPVKmbKjpaCwBU8MaBtEcxZki",
					"mpkGjBD2yPpRkoEjh2TX9vzpaLvb4UDRGG");
	testAddressGeneration("92UJRnsmejyRhYXTY4AgSVRUg6YJasG6twcRT1EWqeWHTtx3fCt",
					"mySTosaewqdrPjE3aLjTp52HRCqn93Bo9c");
	testAddressGeneration("92WpEoztUrxQHmgmtjw5kc7BVjFvzzh7KeHo7GNA61LGuBQosJA",
					"mtUbLUZ2mDaEyGX5Kee3BdXsLMUpgDr5kY");
	testAddressGeneration("93CMVyCYLgF2kutgherXJBAShnmQMzGc1WDpyTN1cjRZ3fHUtwk",
					"mxWXT8uCWnZ6n3XGXuLRBTWabTcycykJMo");
	testAddressGeneration("934j1A7bSTA3FEFbinf63UBKW9gXygMS7AuAE4fPSrPWdxY4wEf",
					"mpzXorHPqVJqsSDUq8MoJHfJ1xgrWs1qLT");
	testAddressGeneration("92vg8bhojrXVXKLW3SYCFVE2EmYgFAZmvz2vtjmYPaSxqSVDuVH",
					"mfzAPjsoVAeMDSEefq6YShr9TkdgaM44SJ");
	testAddressGeneration("91czVdjiEu26uTeG23NUaweZUDAeTs8jgGT43yB8qXBSWB6yGRh",
					"mojdBNahCDRmwEXfCHkMhwqoAfsmMQwxKs");
	testAddressGeneration("925diTFtR9rKNSbL5GPqD5Lfb8y56ftw5ywJNQWoz4GsPNkbhYE",
					"mqVcegBsZcVm7qVnWQpCeJ89DizLXP9mqs");
	testAddressGeneration("92qB8j8bGJU3oq9Y6yjJeXY3Htbxjgcd1YM3rZ3qDQu4WFS3R3G",
					"mipTe7VeQsJF5bfvm7huYadi1wRgLk2qwT");
	testAddressGeneration("939Q8ZR4u2fmffznS3axgM2U1MJh4t73W8rVG1vkBi12d8bTCLh",
					"mjhs8emHcHq9wwpZgoWCzRunhwQ1827QU4");
	testAddressGeneration("92aPcwCdTVivt4i9hBLM5Hrjv1PhgYtLVHveQE1U4ae7Rh9TFx5",
					"mvukbi2pNLKU88UxZAwfdBoxvN5gE3xvvZ");
	testAddressGeneration("92zEVVhsxYJTG2VRTfKfY9SAdhc9B6uxeGLgJZDT2QszttbbXXM",
					"my2zYqdxA611x7hZeUUMzd87RmmvQiyZjG");
	testAddressGeneration("93Re7WF5NW5kis2GgGogvuaMYmgMiUUixjhs5UpZbBjzRUYF9Sb",
					"myRqmcx7MWZdBryoYZiWCjZYTityUgnih6");
	testAddressGeneration("92BdCjXghra4XD1vWPDh6e8kpmazkcJqxLpCqdyDVLEtezRZj54",
					"mi9mxtkbW1A4PWSjCLLj88xBQ9ftrr2UmE");
	testAddressGeneration("93Pc9pnYycnfzswwKdbV4GU7msAhoLUZxKVnMbsbG3VNtYDdnhf",
					"mpUaaZj5bUJvvDmVYJWCRnehBxxXWMb14H");
	testAddressGeneration("91fPVq1xDotB9RNi45AXLUggXAaBgpg3go5vbBMpvPwyQTen8vg",
					"miutdwdsN5fML7eNbQFwsyDRYUqG2HYfBX");
	testAddressGeneration("93RrxNF5VBvVGEyvdtRxfFGuP4bh6HBRYf3SNVw7XH7vEsPcKcQ",
					"mku2AbVtzBrD3eN8wByQSuc595WKddiEcj");
	testAddressGeneration("93Wp9SNKKq3NH2KFGbjJuGFuq2G52wY8HtXLkpe1VHHBwsfLWuP",
					"mypvxSx33yzeB4bA2QGEKWh4z4hUfNjqZ3");
	testAddressGeneration("93J8BLWmxFy4mpkSWYdcW1yLH5LprjvLWVqD6VsG6cP4wmMCiqc",
					"mzpxoE9wZK6yTULqHVqtFxLMMeVL7QQ9wt");
	testAddressGeneration("92hg2nhtNRth1H7ibEhXcVAz53jajYZt9kU2zMxRFDnwxL7gJai",
					"mg278Knr23UddKgfwpVsuq8PaWw42fyxX3");
	testAddressGeneration("92wVW5h7oDQv1KXjJCpyCTogKwQkkzA6rkjgtKeoeJg4HCAtvyW",
					"mqPFXVJmmGPKKc635Yzb65Jjh2Bai8afzN");
	testAddressGeneration("91vKp2rz1nuE6ezR45owtCB37g27CEnZibeWX5cnJxaG3yWsCs9",
					"n3KuSHYWM7kBbvxUXJBAa7qr4EXn4xWHnp");
	testAddressGeneration("93P4tqboxUhky2jMfyZsYRJbPwwumnSvfnnJcCm1e8E1RmNzo3a",
					"mpHxtDNYijHbcVYtCanNEfjgWn1S2edRnr");
	testAddressGeneration("93AqMkmnAr81tnWgMrmJifEMLGBZao8oYy4nB296by2RFaU6vLZ",
					"mmQByYGv3HqbKMZ6i5gWDPQYMmxkmGS3G6");
	testAddressGeneration("92Cnp87kQgcpAv5Cn9x1ZcGymGNJLytFtzkj2V3Xrw82Cb3iDDK",
					"mqPDZ6j6RpHewWa1Yh5EjAaT6oYrGYjpS8");
	testAddressGeneration("92kbQL1KdZmytVaKCw46gsZqwBXJGNXSpbAzg2gJVYcH2BBd3fr",
					"mm2G91jziNZdKnxaomuqghdECoWwzL2o7G");
}


void unitTestsBitcoinTestnet()
{
	std::cout << "Unit tests for bitgen-testnet" << std::endl;
	testBitcoinTestnetAddressGen();
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
			unitTestsBitcoinTestnet();
		
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


	
