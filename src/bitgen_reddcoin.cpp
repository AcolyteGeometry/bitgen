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




void testRddAddressGen()
{
	testAddressGeneration("V2wmLXsqgmaz8swoD5drM4Hh8Rm3CRTo4dARbWnPkx2H55szJ6Qp", "Rek1aQZh4uJ8vk2UxW3tfD6k2389DguR2G");
	testAddressGeneration("7M2CBGkFs4KSMCqg3K21KtKCdFAdS4PW4ts4JoZBSStuCQaQhNc", "RfMVy6TenBagNc58vuFJBZxaBmFskZFhJF");
	testAddressGeneration("7MKaGQnDbvaVz7cWpExMRsvAtTzxJoWefapqsPJ5yMNraXiMUEB", "Rq6VxowSepMxCADoUXgQFRV5eLUA3L4N2t");
    testAddressGeneration("UyA52XJBA5G5swtbMy5NPFQhkwxAhn3pU53inctovR16K8FeJJYw", "RrKxusAM4F9e1PzpwXGdpgHWFZGgGuKcQj");
    testAddressGeneration("UyvYxECwzeiMsQMtxqeLojBoZ9Xif9hnrPmvbDmrHk89ACYK88FP", "RqZquDdwz9TBsrGH3aX66igWEs6jesLC5R");
    testAddressGeneration("Uzqxa1wdx3Zx7rt5PFfWoGvVj1rm6i27UfrS86M1D2DUnfQEQ7Rr", "RgnxF1S5WQ913Y2NsoGemYU9Rw9WG1xbR1");
    testAddressGeneration("V4cG6y3r67kqFaA8PTd4TPq2w6or64EvztQ9osr1G6iFJ6LgMtnw", "RfXDBLUG6iikm9mVwPJdbQaecR1c3Vnx5z");
    testAddressGeneration("V3vC2zpfn116nTJ5wnGvmUWvcRTgKscGVKj633gPfCmddxh9aRgb", "Rpygbo8h4BnmxAueZguX5BdV5QVGmWW2RV");
    testAddressGeneration("V12XYYQea6bcHC6PBGYpF7V3Z3hFbBFkoYRfMgpxxKZFGfkQQFta", "RqoGVb6CYL3MXKz8WqK7nXhtKfzs6USL4G");
    testAddressGeneration("V5KgAGahmpkkpBT3jDpDgf9SdKVJg55wvRJiKVrB1BQN8Kis9Q4J", "RYkmmh1mzC7b3Siqzjb4tBT5gDfsHbyK3T");
    testAddressGeneration("V1h4bXgwmaUaRz9pTrnayuAM6idPtbYwsunFZHRiprPvJAxkcTNJ", "RiuJTZfJn7jiDz2SSnrpAnbDhG1DGRvegj");
    testAddressGeneration("V2yJ7BBpuHyX2SCnv5E3Hpo7B8HejoNaykWHnKwZcWzDJdELwCQB", "Revn2iy8F2zvy2Ks2DX7WNp8Z6Yh7Lu41V");
    testAddressGeneration("V5ZSiaUqEzJQ24sphvyBD1aD1gvgTxJV3iqPPDDdoXNKUFQ1v5SZ", "RYg2yjDrr3xr918zKpqjaFGxAjLu8MZS5E");
    testAddressGeneration("UzaSQLym8ykD4wmiUm5oSeRSzYrsSpf66mAcfQxLPqHMLU86r6Qe", "RnBRiVw8rKAWZJFcRnWTBVckAjLnJqTDxM");
    testAddressGeneration("V6YvsNoaYG6UWR8xthsBMTZ3nqg6bqQhpe62zaH9QEBku5yNbwpf", "RrRhj4S5hrccPDXesx9WvKh9w3PFLYoend");
    testAddressGeneration("UyDbjAmEfAoUWrUe7LKE2HWrYd6LyxQqmpen1Gcix3T2hNMG9LvX", "RazQBkXFRqEQPu3wY82q7wkPBLUFh2vy8h");
    testAddressGeneration("V2HM5ZUfAgHNsV5xFRmEJDNSNk6np5CtkeqMMf5PJafe5NXMAZxn", "RhnSntjzGkMnzjiLUFqtfaApzouemrvHS1");
    testAddressGeneration("V3QQVpFDsj2pt32KmECsRfpxyMj3chuVgcTJbE9B5uoQE4zn9ekC", "RbmWSMdRcvLw6fHa86pLxPXn8cDifLLgu7");
    testAddressGeneration("Uzof7mv4yuyGTKbh18QLCcoSeZYZA8yYgWWcv9jB7h1oQaudM1WS", "RgKZ2iHn3cUceCkJNbwcSr3z5dcpEjhZuh");
    testAddressGeneration("V1kLyjggcqbBDCUWCJaascZiGN5JbHcvzkJCXJ5iEKuEfpbdkgCq", "Ri3btUj238Mkgo94yf9YCpPceuuoARmjYy");
    testAddressGeneration("V1RiMtqxSPmVqR9pfqpzoT7vAMkC9QvB6nxWMwyEj1hvwGjqYESk", "RpRGfQF54SFk4wiSxRuZY8Vo1Uw5m9R56k");
    testAddressGeneration("V6abkafemr99BTssdDuyA2Kx1cq7ohmYaaQspNx1CE4NMBWWp5bf", "Rcem1d9yBXmjazwF9bYHuhwEgofVsDQ72x");
    testAddressGeneration("Uz2WpzHEzsW8YboiwgmSVja5kEJyf6Bqiiinpizdfzd2A5B8WWGN", "RYufSriwzjeWdXKZQb2SFyjhBG7LH8iUJF");
    testAddressGeneration("V18UQdb56qEVfnGCociu1tQLpJTRB3ppNxBT4YKq8dXQg3aVaPwZ", "RhqJXrZVxCNiJuHs6PFszPgUa2yRgtwTUQ");
    testAddressGeneration("V77JY7SF47xAeZEK8muymZXR2SZaf6EqkFPdNWN7ZCVAruha9tzh", "RrLm9d7YvhyQvsFUvR3Nwh5GtKWp8RL4qQ");
    testAddressGeneration("V1e3vtH6wudQsPXYDHgMjaa2V9Uyjw8DPDHqGfYU3HG6EX4Tii1P", "ReQCNoQX6cfPyBSPFw2qHN6YNmPijL29aR");
    testAddressGeneration("V1spQn4kCdiqPFNAyA4aQQDd66ctcFogZSGtYXzz3m1myKSAnSQh", "RjnRNyT1sJw7jL1k6itQYPvhMY45m9cGss");
    testAddressGeneration("UyUENjMqB3gzF9kSUFRSELskKBwX3Qpbuej2W1rP7YNuAgwhKaAY", "Rr1vyFDeMFjcPCjQ8KH6GkAkC4Ko3JnAtc");
    testAddressGeneration("Uya8WJaiAN23HB8znvHBkxPHupikdS3j3UZppj8AnPUxxYDmN7zt", "RuxC3abCVGD2Vn5AtaFwuGUejANTTEPijM");
    testAddressGeneration("V6evDeJ9qPocsu3j78RhbPQEco358KkhByxkaD8VmEa9TinYpcLx", "Re2nweD5FrjWpPGxJKQZNXDgBMGQcKpySD");
    testAddressGeneration("V1x2VGENUNtRdjJtYGMeT1EoUeXn4mNfwpGF5Lz1aCqttRNXVSZz", "RgeJ75W7Z159oSsRq92pE6D3jCZHUq64ss");
    testAddressGeneration("V1NruaG4noFBkPymAZNx6dJCHudSWdpSrbLv916A52A4DmYJdcKF", "RaHePtVjFLMK9ivX6LTw8dSRhaAo7Apuqv");
    testAddressGeneration("V4m2FEMAFckgBK47ccCfDZ9JXQjH8sKHjRWjKksU3arpkEnmULoJ", "RhZWnZZBh6Hg6qdeiF5MDs8sSxt2vvRwcg");
    testAddressGeneration("V25U8TnB14gwGCLdomwRfvHNLdhF5xTziHE1NFrrgSxmYqXzQHVZ", "RnxRsB2EvVGwfyW6xvnyyaFU3df2dUfYMb");
    testAddressGeneration("V5izPUkdfizemv4MPHvr5pe4TrfdvTHrAY7Uy2UWpSmxz6kYwWBJ", "RwAHDS5XaMaw1qyRre7adgxHeY8JioYTUF");
    testAddressGeneration("V4iQYMj2Epc9NShb8s8zNo53KzoBCdY8DY1gy5VjLwbUZGsmRWrH", "Rg2uPrGm9qqpDCowWupPosuD3C2Sz5yTfv");
    testAddressGeneration("Uy1pKQKVrFmabcBXDLo8McuaTzEGBJdwposenfvhoewdNJeiNQyn", "RqbUgqoM2fSV5sjU3XTDYMG4Ekg89wyPqi");
    testAddressGeneration("V1aqx64QHpqD6caJzc9obiqBbtKvMpcfXSjeWria2ZTrXfc6MVoj", "RbaNuNZxTX4hTfi9YpEWyUsaLDib6o1jvh");
    testAddressGeneration("V53Wj3iMjWgTy9FtNAYydXHBZCi8gbbLx5GNCkgMYzE9gH2ewsuQ", "RdH2sNzPBJ1nXF9nffCFuHfgFs53pmGYsK");
    testAddressGeneration("V39UMvBkP77NAonN4EAzBbDBu47GRzPWZrjvgnTCEcEETSP1d8Ja", "Rv4gJqnNE72Uf7Uiuxvm61tXNYiwAM5j7H");
    testAddressGeneration("V6dES7snxd8swsKWog2BeV2TraM5Lf4QVXdYS12YtXF6eBkUf92g", "Rpi6yTXgrWqoio2px557PkLvPwTmycxKuQ");
    testAddressGeneration("V3DjjENQV3bNqpsqPdQfSrdv4qJajNQsajDCFboZneZsFFtimt3i", "RutAzNQAisL1ofczBXUqJKQcDDaKeUxTVi");
    testAddressGeneration("V3vmczDgCGhsNLaL51pyGaonWaMSqrCBG882uxx4Y81h3K8d5qbQ", "RfjqpNKDwUzSjNurj8ShPWjriQujeZPYF5");
    testAddressGeneration("V4T2TDHefxNJ8GT877SMvN1NdDoZ23DfcMpQNEXyUYqWuuRe39xW", "Rgt22YwtAiHLeHy3SrPN2CmRHz5Si7rBaq");
    testAddressGeneration("V32wn5dUBEUZtnggVETBwGemcWqKdf8X3vkmTQ87UfNjEMHGTm2o", "Rek6KXkYDhyUvjTTVUjdiSAqGpkqVQhF66");
    testAddressGeneration("V6MkrL3s3oDt7XfQrHgNJRmQSq3s9VWbVGByiTsRuRZiVcs5qXTX", "RoFJo9SF1QBmxwy23f6jcFedEY77C7SEqq");
    testAddressGeneration("V1wgb8UUiEWRnaicDWULNCru31AHgwCakVkvzv19bFaJ53tuvYNz", "Ri5P8Z1ccMrKyEqVeGe7TgHqhg7FXP83cV");
    testAddressGeneration("Uzs7Lrgm4wWxe8c4tznvxgtmgSxpbiU73WEUbTqVaTLcU5cU7Xx7", "RphA8QQGDoFGzwTuR4RDL5FXaYsnpLDmaP");
    testAddressGeneration("V1JrfLqmyLg27wd3JCKBksiW8bRMpwJwAJWaYdJHG7nNXaL8VPDk", "RvqWodEJ7tBTXFvaQjAsvgAVD6VAktD8cY");
    testAddressGeneration("V5ABsUZ5d5jpk2Yb5rPVug2P62YhTU7zuzLf3A9Gk2xtWKvWEbvR", "Rf3oKwBYXbEwhHLKZPcTxnV7Y8v66W8cnH");
    testAddressGeneration("V4L76LGCvCsSLVMRRFoNihMu7CYiUc4xjxti4nwbpA63HMPpc8Qe", "Rg1Bti4yhZYfYoRCsTeXY7PK2MF92nmkn5");
    testAddressGeneration("V1vTgEJjh55unnLV9iuQ7vSizF2kV6EB2aUt668AEpQ6q2xKUGCf", "RikwxPij8SsPRPq7qq3sB5PNk6Xtikpp8j");
    testAddressGeneration("V43rzN3fRfk9gUvAhX4RqekYfiUAdXfgw4nKgei4M55nybGbgX7T", "Rk3UxmaBJgjTkA5pEHhkb7SNHiKCXLmJwp");
    testAddressGeneration("V714QE42o8eCLUWSXQKd7fBPpogb4QKiBG7SGQsbB2d7U73VapfX", "RiMJxJuzAhpt5hLqaaoAbmKG288UK2VC5d");
    testAddressGeneration("V6jbGaRdFtN7SQeR8Z3c7vFNbPuy8YXsnuvf9HBMs97YmuuKZyZ1", "Rb9a1C8ZxfCyCxN2Yh2C6fiL1W3yp63iYJ");
    testAddressGeneration("UyEsSy3fjBvRSFvmLiUZ59sbTjy1Nwz2dhw6MbAGan8jPwiCdBbE", "RoeLXuQZzpZBvuua3NGQTeuEJbYZSiN1Da");
    testAddressGeneration("V28Gi4cPD8vqAXKpzhmwMxJ5k5QUJkLCetHQN3NLGu2X8barBh11", "RcNzBQMj8BjWsKaDj7DYavHMYcPSAy4QAW");
    testAddressGeneration("V4Bm7LiNTkwcwnCGn9GfJJsXVgeQRhErT9ZVBH5bgXzTTUzEV8Rm", "RsyKqR4WFfFnNpGQGU3ZJgZuqXC9q37fgU");
    testAddressGeneration("V6Ty47C14kD17sg33Na11eH3CAUPu2CMTes5ng8iAeTuBNLS4zkH", "Rk4pLPxUb94RLohFDKCjVbg3tovHeRfHyY");
    testAddressGeneration("V3v1KsLSnUFuEvVkUpzsueNbv1p1FuEHztDALim7FjQiJxw8qSAy", "RuySh5PuuHqL2WLh3JDyzsiRFk2w1TaS4d");
    testAddressGeneration("UzCmWL6gJqHUJRtip11agxhvh3dqb5HJ5GwqYDRFXRRG2mXxyVjj", "Rqv1DGP3NFdGNZD7vrTaDPPLvMZX4hqqK9");
    testAddressGeneration("V6NUBi4c4TuPqqMK6wW2Zh3fUhrsPmS7uycwtTcFjYRsakHjEqS2", "RvV18TZDv9mTbEuF79EFZry6XZZf5EMGzr");
    testAddressGeneration("V78XBPN31HDubHwX8sWr1tFuUgiNnCxrviYX9sPDvmX6Mi3D2xwM", "RhR94oDiQ4kiUaBXSxMjzhPkDNamyrM5ns");
    testAddressGeneration("V5YDkoL3683HHZbnMAXZwvxKh4mw3EqaD1sSsdGptJpByDAM2k5H", "RiwbpKdL69FKtksQo1Go7xjf2HA7zk7q8s");
    testAddressGeneration("V351Ui38znVZ1pFXbPWEkHPzHhNv6CZ1gnEcE23x3rkz9PZNuHmN", "RucHBbv8r6eBriuTZrV8sTPnm15WLYBP6Q");
    testAddressGeneration("V5sUCtbG5qZ3PHwLKPBeTxNCVxQbFZYTNGYYYmPhzatgHZ1xY9a6", "Rh5cpi8Cb7WF59EHxpFoPghnuXgA98hxPC");
    testAddressGeneration("UzTVUtr16VScoGBq9mRH2kxvX99Jt6fKRBm3eJ8VZNtJK7zyzScH", "RdQocFgK8rqV82iBRG7jNiNSfXQwTYzCKv");
    testAddressGeneration("V6x32Ajf3sVf2Ti5JRQgY4todCdixNHQ1LoonLZXv4istRnzRfKZ", "Rwud37oPkoE5Dbe37MXKJ6BWPxVCXo2Brd");
    testAddressGeneration("V1btfdznRFdWBMFSq8qLRTKwCSPmJ1ZGwJL2th6kg919W7ywfujk", "RqXEUgboU3W7R2zrnjrrxdqCWsYz9FCnGe");
    testAddressGeneration("UyQXRubWuVrLBUnDYuMtNr5T2Vw2qX3pX6R9EbES2yjMBRg9PSNv", "RvNHdPhvtF4ShsJHzwtM4FdDxejwBS7Zuh");
    testAddressGeneration("UzZE15BWQnwfNBZEWXyhRUXTVU5jSxopAofmzTg5M16T6gZ2SXph", "Rrw2UZ3NfCXY2QzFgLhGJ9P8PN61FJaBbW");
    testAddressGeneration("V2MCzv2Fxs9tq7rnWCS2ns3ZF5p5J4nJqJsPN5WqhFV1KbGzGeHW", "RcGLVGpkKzhZBDtpaNnLPiLxVWeyikrnFZ");
    testAddressGeneration("V6AW8kZQLiMfLGhkTcy7kixJ4Mnkh5bfFvYbjvz22ULmnMquH7m3", "RowV9Ub6wGG4JTKTHPgNSo4tf3thk3xhka");
    testAddressGeneration("V6HquB2se1qU7YHYsZVSyeGp7QnG6K8BeB5RrLrEgjLy9xh3nbj4", "Rk9q9v35WrooHDnj2o8fSeUuEWrpmNrFT4");
    testAddressGeneration("V2Es6Lb8Bj9BmhU95PGrheX7PxNxyqwHycTEj7nodrhFNgcVfq3b", "RwLgM8FDjrHz2BXKtUU2S5LDUcde6hkYzj");
    testAddressGeneration("V5LR3nXQb2hmew83W8cseGsJdxW9hQNbk9y9YHt7ynvDQFNcrN7T", "RZJw8UFJ9YUAypUFoVsTLKEuLx4RbnAFFR");
    testAddressGeneration("UxqzgCS4kNaG2bkes3JKAkLop1siKYZwLZ6utmzZ5ePYe6wH97ea", "RrdY63DxnWjKcersS8JVq5LmmYa2UagJBg");
    testAddressGeneration("UyhXycxUCJW1JCDhqKxEbgfVB3BznuN1PV5CBJCR33YgGNHoW3pb", "RdX2VHwzrj28mkvG4nBSDE2p2JvLu5x6R4");
    testAddressGeneration("V3ACiwmx97diMH9L2XNGytwsGh3RBRwFXeSH8Sv72oY7pGUCaaJZ", "Rjx9ySUkDm2v89yif1S5Ej7VPdJhbc3xtY");
    testAddressGeneration("Uxwtemdq2MRzL1saSWa6kbpkxpGixYHpCzhL2QZcBLvzhxATbTcX", "RnTv4xWnKYD8cH1X5amKaixkFd8WfbpZGN");
    testAddressGeneration("V4u4tSEqZQzyYcBqd3DokUWuxdE2y7VBtjPSRzLCk4eUBmzEtzr2", "RddKLUX6sp1AYvU4Mm2YGtGEWFb4MH8Spi");
    testAddressGeneration("V4neq4VgtJFVZ2QTYaJZCqc2Bm8piZzKt2xDc3xkYMUm1PbfygX9", "Rb6ib59xGqfXjgZKMBLe7U4FA1veJVkL3o");
    testAddressGeneration("V5TMeaYbWsWqcorx4qMjPMCAwEcR2YCYQ3A7SBRRxjkptJzfAd7J", "RwhA9aDVhDdY46nuCMbCP7HXD65rYBvdL2");
    testAddressGeneration("V4TZwArny5ECmytub7qLyggLDiySWu8qeX7fJFXwwEnbsxvp4XrG", "Rpj8xMzReq6diw68wz2Hn5deUmXyaeLC5e");
    testAddressGeneration("UzJWkZ6vDvRtyu7j85HYWvUaCydbTWARbb3e5KqusNwP1b8wxDph", "RkaPntHupeQjDhepurG2Ps6dawWt9mLGgf");
    testAddressGeneration("UyeZTp4DWTSKrJAfMF1Yr3djuhFrKkusacTE6BnebHYUTiD24dd3", "RiVWa5xcmUKYhuZqbbgknUL2RSbvG7KfqT");
    testAddressGeneration("V35wmwdQA6bpEtHuXYqPb19exD8EuL72b8MAxdb3G28AuVudLzAy", "RbgznqNYRCvkRXAWyNrQ212HLNGRzTp7Y4");
    testAddressGeneration("V29iMyFcNNzu8aE8t15WZGVT8KzCnZ4hFV1zpe7rGi7CSgZLd62M", "Rp6iVLVUKxAWBQygim4QmHaXpWJk4GCQvN");
    testAddressGeneration("V5TQ11ruHhMXAzgoftDdvW7M434iBLGSpYfrDkcYhpUR6GfyFxZc", "RvGr7MGgRMYvFC9sDKr7XV957gXrUUuj4W");
    testAddressGeneration("V3v3LfCqdEgKibiqSXnW2DzEmHS5x1FiLJXAzMRSV5hqJND1oYDz", "RuPTTsKicJPdsfav5Buf4sHtwL6KaRLhvC");
    testAddressGeneration("UzL1abVCc2t8uXt2vNTsGViFw8FeiQCnRfZXrbfyeTDZ9tdFESWk", "RaXNzv2yYdokVd4Sk2FFajdNXSCmWvN5hE");
    testAddressGeneration("V3PnfC79DwnxAwaq9ej4rXwCsTwQyo9fpfw364m6T4hrSgxRavV1", "Ro9KTNMraDWbeTVvv6dB9jHtzuJ1nB4uUy");
    testAddressGeneration("V3nNEZwA8bjXTaH5SXRKC3QPA96Sm169MUfUz5JwDEByg9KEr5Qh", "RuhSpo4SR6UMDoywtMsTCWrKbpEpptacrx");
    testAddressGeneration("V1Kkn5hrDSTLRpYLV38jYJvmDGNjXbsnFydqfPe2PMFx6fAA7QUW", "RcW6KLe3UnHa4kQhgDwFLf3NDfxH4emrTh");
    testAddressGeneration("UygeUr39UryGd7GLLkw5THPKppymg3FP1PJLqXfVWMLxt6Y45kDK", "Rs9C3eqvjuHC6huykRqfcnGvB9DcPDPZzD");
    testAddressGeneration("V1v8iZZvf4XQuj7KmKqSg5heFSMdJfBdeC2we1daRuhAvZ1WnBSm", "RdZuXGVByBav44DYSMDL8WAXJMtnpf2VSz");
    testAddressGeneration("Uzz1SP1ei6Qy9GDUoW3i37DRWWVhj3MHbVqR1UN9ZWCrgWbuouWh", "RbGC9KL49hRV7UUH3LiZ25BbSg3EEJjwZD");
    testAddressGeneration("V6mvp3wH9L8ibFZ15BYJV8eNkYmSfY6YV1jcfR183nY2cUo7w6Wy", "RmzKgb9QGT4LS98ZgAMKrqybHy752MYZvM");
    testAddressGeneration("V6i9Ey31JCoHTqiaMioNEGxRWT7YmQgvZPXXKawwYg9gdZHJt4SP", "Rt7UEBpStsesqz5mqj2bp34hXubooeKsfK");
    testAddressGeneration("UyNS6uyLoGVjzj2HRKR14oYRWb6dwz1cWLmnuJoMPHuTpubp161U", "RbtLxBquueferCHmhrf2NFzhtj7L7h9T7e");
    testAddressGeneration("V4efzH4BQugjM81fHzAiDLqJmBJnkeC86rSVH43KfSMGDb24vaAX", "Rf1DBGRdZR8GL8jStR8NZn4DH3XwkqrwUM");
    testAddressGeneration("V5BmQCttyqNvgqS5xat21rkcYYC8Y4B3Mk1LocsDYz4WKRA86wGn", "Rir6ZmwR2GDFz2aeEw6UG6vndhUBMcCZT3");
    testAddressGeneration("V5hHdWtBXhXVEC5Zs7EyG5JZpQipGXy9KDpHki7EPBbzPwwrCDfz", "RtoPpijeSSb7xv1bE9dMfc5WJZwxvc7dB3");	
}



void testRddMultisig()
{
	/*
	std::list<std::string> pubkeyList1;
	pubkeyList1.push_back("024c0eaf4c884eb22191ab91b688a27b03ab2150ea33a7dec88bd6707d409abd79");
	pubkeyList1.push_back("03b13004940d81f0f3c6de5961ecd4bc3fbe5d238068a8fb76f0b384e88bd4bfb8");
	
	testMultisigPubKeyGeneric(pubkeyList1, 2, "3247N68EThZpuvnMJJBG4juBXGYV6Db4bm");
	

	std::list<std::string> pubkeyList2;

	pubkeyList2.push_back("025233b9e7720fec6d02f95099f95ef97a5ca7d032fda7a2f68529ab61ce48e5c4");
	pubkeyList2.push_back("02576f5616691cb77fe7ba078264667de14311a39ff051074b68d039f2cc126e11");
	testMultisigPubKeyGeneric(pubkeyList2, 2, "3L6zwp3fANWvY7EAYZ5Q2hHTHajnvDpgMw");


	std::list<std::string> pubkeyList3;

	pubkeyList3.push_back("020426c93c79f3e09b7baf6a004f20bb838c722a87af69e1ce90482e890963fd31");
	pubkeyList3.push_back("029d9bf5689d00554d3b1a660167bc66aadc38178d09f04b2ccf0b40876d667b64");
	
	testMultisigPubKeyGeneric(pubkeyList3, 2, "3L2CUX7bizYeWDaBqzRwBoK7i7vBXgdkTp");


	std::list<std::string> pkifList1;

	pkifList1.push_back("7YQyu4HhRDqwyoKisgntL5PGKnbR1EVnJQHhnszzPYEZf1vGVgW");
	pkifList1.push_back("7Wv6PANhVUbxdG9qr83DQ2EorsQ2Qpd2qMYPp5opWr9yHqNCZ2G");
	pkifList1.push_back("7Xmh8DLjzX39Y7rbxxzpJG9EPjcZ9BjgEKXxA6CByqRhGXr67Pa");

	testMultisigGeneric(pkifList1, 3, "3FWJ545Yau1s9B4VcrW44gaMBbEiY5XDZ2");


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

	testMultisigGeneric(pkifList2, 10, "38EVgW7vtBuqLVN5dRQ1hsjoesGoK4Ws1x");
*/

}



void unitTestsRdd()
{
	std::cout << "Unit tests for bitgen-rdd" << std::endl;
	
	testRddMultisig();	
	testRddAddressGen();
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
			unitTestsRdd();
		
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


	
