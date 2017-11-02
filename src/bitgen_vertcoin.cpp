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




void testVtcAddressGen()
{
	testAddressGeneration("WSRfwyJN1i7FjZWRPHY7UVpD5YdiLk7nY9A4ofMXxQXBYBm3HCyg", "Vtujn17eZ9Pvrn5EEsaWLTZJEPf6nCbiPE");
	testAddressGeneration("7fxuPYEh9r4AwVeTS1QYc9Xxc3Wfxjfc3VgASETGfPWjLofw17r", "VptxXhiiSxAXvWWwtSDZRrc3WW7WRkPofb");
	testAddressGeneration("7gHAu1y3TZAsaztbNbfZYjAHYFkUhhUbzLfy5UQRyeVTDJwHZ8W", "VcPZTvuLVGVEeuNrWktZATZGQE8pMWvYLp");
	testAddressGeneration("7hU77vgMjVu2YShNQLKM1YLifGF4JmNhKFpwoXRpj8J3hZGduoy", "Vp7ziunApEftvU675u2Tsnv2xfWxpacq5R");	
	testAddressGeneration("7hCnxduJKNHj4iSpMXkV9b5nauRiCFmpJZ9nZwhY3wuNtr8jB9i", "VpgSdAQYHJKtB6vyqXNZjV9TUkfeJjZRf1");
	testAddressGeneration("WSst8h2fA4cJGHtep54uFHh5MZaDwUa4yWSMFCkmibhAhReLnUZu", "VyKzdQxv5QhNXFLjjNFNPt9NP5vBUgmohh");
    testAddressGeneration("WTDqUijzj5BLcLrQ6MweRPB73JzXXo7rTDiYqV4j6aVAieSp36ry", "VgiZUBBh4EQfNyQhcP6pqdTDZqu5UfV3RY");
    testAddressGeneration("WSgNCd9iy5EsRX2K1C5KjfkjGXkWX8vSw35TE1DkZ6U2gAhj6haS", "VmuJZYpx7kT1WaYXAHZhHEMn2xfY5ZW8DV");
    testAddressGeneration("WV6prpFATdgB72kVkYn7uDFm5t6M1Bz8LNgtNDPk87h9Rsn3C1FU", "Vam7gSYPhJvFhzs8R6hseFTvEoMKde94zd");
    testAddressGeneration("WT9Rr4zsBwkTMWdezTJPkPeKU1hmGyzG5rHUQCy2zF1F8NWdPomL", "VavMcm4sZa52YZ1urSYGmcRkgRtXcmKUZE");
    testAddressGeneration("WUUb5o98gBJfNQQzRoUkmexZmyab29X4xA3Mv2tAAHQUdJt7rGpe", "VnWMANHiX66ZfZoxhmGpFNS1eSMuF7AuQ2");
    testAddressGeneration("WXDPBFQjSAiYK1QRAFB83BFK4gXcWYtAoG392BPhnhShBLBrpqvG", "VcP7GhnJYTePwv9vGHBG2iL5ZxcNFrGekx");
    testAddressGeneration("WVSp5C5PR4T3XcYbnRzJZhmWgT6G5vcEkfNuKKb83QvqcMq7puFH", "VtM4Z7nLHMG8H68jsTp45tJreyz4LW7RBf");
    testAddressGeneration("WWaWd6DH5Xr3zvHjn5Yfi16dgCUk17CPB1c2vjtUeC2ShsBAVeD7", "Vy6kWLp5yf3N7x58oxWHCXgGkx72k6eVb3");
    testAddressGeneration("WWcfJ8frNLbEfcS4mBKC2T8ky2HbEm71gCpGnrrd4mjGruNAbLEm", "VfDRxhjxkX1cTgoJs9FWUSyzjkevaGz4NS");
    testAddressGeneration("WU8cNJCAQuUwwSXsrAamhKBdLw6Rrd6unm8fR8ahx7sXPhHG5crk", "VcKiHtUWQi1u5BybCUGn9tY86m5qULRok4");
    testAddressGeneration("Wah4i6pavWQCSYQP6ei6MkX9v24o5UXWANUz8ryppisExj7pWj6g", "VnjJsxhguCa66Kj1o4JufrK4BYhg1bYZ8Z");
    testAddressGeneration("WYgKSn4E9MBA6gjkHNjnmrpaMx5Qipc8fk6woXdDT9C9zT5CnjZW", "VjwEpJVEaQBUfVGXHxXgtmS152F5bieTHn");
    testAddressGeneration("WTPxsJ2Rn59RCq11abXVr9HRXx3kiSnzRi7SQJu7TkDViRLqKVit", "VxKPmMSkmguebQeAcCu9h3VHJETpFDArg7");
    testAddressGeneration("WTLmyJubutSj4j7kDmsH8nQX6QCxACVwG6L6aT6XC3gXZzqtqgqk", "Vfy1uDcbno9U8BNn6YpzvsjHF5j2fMiZem");
    testAddressGeneration("WXHUUMmZRrSFUPQioReztrnWHUdo7usaB4ocskaT32qmdHhKdxUQ", "Vi1pXicN2ENFYiqWpgmJ7YRbwpt5cskfpB");
    testAddressGeneration("WU89Ngetjvr49bMy9oYzWthH3KdLaxxX2L3tKudmRV44RMGpWvM6", "Vt8LedvWqUiAW9DhvpZPSkkpcePQFP2p9L");
    testAddressGeneration("WU1XwyLxfck48HE7bJ2B6V2r6NkdLn1neuSKYTFoh85y3CjoPp4i", "VjunN2uDCMKRyENBK3JgaYjNPjmsRipYVq");
    testAddressGeneration("WUvJoMaPHq9UiTQFi1nk4H4p2gH6oNe4JpkJNS3VMw2aYeTDTpCP", "VcnEtyafMTspCMSyUrEouLzcA8XhW2cyAN");
    testAddressGeneration("WVrxFmpEQVBuYYu5PgjeFE4SjouQ22Fhc34Rq7EFqTQuRBT38ufM", "VgAd5XsTQF3XWVnm5545tJT1hjyf2pzjJp");
    testAddressGeneration("WWfcyGiXpx2jQ2U18h1F2hSkL2ztqtgUFnpYALvdGLRj6qyfuFVo", "VneDXn5YWM1xKPHdFqsfHYHkcjYckAacgZ");
    testAddressGeneration("WX7agGhcCyFS7UZSU9v3Paa2rry6R1oHpyESJF6VabijhMWo9vfx", "Vs1DcQF9qKGyi9NKvTm3gj5RzWovtKWQNz");
    testAddressGeneration("WY1BFWFsxjdwt5zKfpuiBeLaZ8f2QDE8zPSXovFGB61HQ4YYbz3e", "VbgbvsW2AcQZ4AMpfV72iu8PywxiJ9iujb");
    testAddressGeneration("Waeq1FzipoMqKrz3xELjjVxVPVyywyAMN6DMRuqRcG7t7QecR2Vj", "VeFghkwigPMYWotG548cQgeMSwBo7zZerQ");
    testAddressGeneration("WWrctrudhLpt3f5wxJZGxobsCk8MajTWmaw5HmVjFxxphoBEZy5N", "Vp8iZMka6VMyc7hTpX3tGUHKC5hjHJpM7C");
    testAddressGeneration("WZwvva7jfqVS6jZoW5b9DjB4uB6z53d6aiYqDoewbfdMRCxgEaB5", "VmHEn52uBvmYXYzVBpyUBrpJqCfoYqFLLe");
    testAddressGeneration("Wa97b8vZhHxNHirsYnsQeB1ypccuN6psDFLiA2YHJ1wXwAtdmgJM", "VebwE9kAY7Bwrs12NPnvTN8tsBstz4tYfx");
    testAddressGeneration("WYoNZVwmnD7NL848ECbT7kmy73JK6W78XzAkD63fuv2q9RkHaUjX", "VmGXNd1hD8tASrvAeuQYFDu8xzS6meJznd");
    testAddressGeneration("WWceeKUnzceckL5NiNtdsVrsGMysNGDDqW29FjWTEGLG72y1wmif", "VbTXDgVJPVY54eYC79HUPXFYUtKPxUVe4z");
    testAddressGeneration("WVfWgpMm4K9fxh2PWnXVzYzmoYRpQWwVUvyWHDykxuRuz2BavECz", "Vn7oNGfSFAcSZmbaEFE1CqBob4ooL3sAB2");
    testAddressGeneration("WajN7CKD6dWPWVEDWiL51414cDDhBPH6zFpDyh7aNfTeVM9JRoK3", "VgVeYX8aZ1gs22DNEk5B7co7nkmPsx9E24");
    testAddressGeneration("WXgtfXqUsEKjucfHYKCkf8T24qZBzX7bugArGtNMB1T6XCeyZUWj", "VvmjskdvNDgX5JJ5q9xqtBHndJVoKgKZ7E");
    testAddressGeneration("WXPg72KRp5e5tbCgAXRaTpyENwh9C95c4etMu7pzMS6tjaDfHxnt", "VeNtmSuoqGBCRitYAfrrDsizXitEa23jQ4");
    testAddressGeneration("WVKqjyVZLB3tGH2XXZwXqBVZQjw4ZBYvpxW1ZtuzdujAeQLJmN6k", "VmK7ZFp6x1DSHSCoia8NFMVeyMdQtKLVNh");
    testAddressGeneration("WUEV1LLfRfFXL57BE8jHkCsuRh2Mz5BGEeYJvzThwsSoKhTaFavN", "VmVVVv7M4qcLPxfw65kXtVtH4QhEdF3UHT");
    testAddressGeneration("WYfoqHfuy6jgC8b9jcfyn25eMqRhc8v2DEv3trVLGSZWJhv612rn", "VauuwKeKqYw5Ae4JNLxDqE75yMQEDtvLE4");
    testAddressGeneration("WX9zPy2A4zpRHLzkTHqBvMyemcDPaNuReUMaXXQM7jNf6X621ocG", "VdsGjaZ4JeA5bsAVYBwrp6L1aGHrfjaXHk");
    testAddressGeneration("WaXjfqZtSzVoTtkwbjnPba5C9wvVnNPoENxysnbj3kQNEFB5P7eJ", "VqEgkfmoCxzh5a5H8wks2AMAezePDyMKjY");
    testAddressGeneration("WSSQwjXuyfRHdx43A9CxhZ1Pwahyfhf1uNsidW48tZY9eVwpHYRx", "VosEMimfnWb2GwPrebiTJk1xJrXHQD1g6u");
    testAddressGeneration("WTVsCq8MCNrLv3YEuuKMi9N8HFSquttz2h2eMHqMWXwrcDJFaHU7", "VotareVFPCPifDBmUMNDfq4TvQxJo1VCe1");
    testAddressGeneration("WVjubz5Wso74ApNtE9Jq84EjMvom221BAwB5BLtKAtMFdyKerEK1", "VvsuxVxrYghFWgQytG5bCy14tSE6nucsg8");
    testAddressGeneration("WYZB2GPRVbi6NiRTvPgZwJeETSBYLZuhKQSqByDp6jwTTd1V8bvW", "Vpg4eCVVkG87S5qNfkyNAbmZ43Dr8jMPAj");
    testAddressGeneration("WTxwxK8hCro4Xpv6BmZw1R3hpyDqNJt2aea56usMvHna463ftGG5", "Vh2ziwhKfxP2nZ12zzPk4u5o9YnzGenhXM");
    testAddressGeneration("WZ2vHYttwL2RhaUSY9zADEokNUGV7kBTPX5AguR4WiQHoqSDfq9D", "VopnaF3JxXzAz1ZtgNKXyL7UCbSq3pCsUh");
    testAddressGeneration("WSXBuGfN5CjA7vufPyGowwutQACVH314YXUp6Jpsxghpwx6y1gkc", "VhUzJhKtvie95AzRzozJagxszkmiZhPrTs");
    testAddressGeneration("WaC65eRR8PUpq6eqvs3rGomrD5ba5FQxcEcvosBmmq4LFscrYYvE", "Vvyx7UdgiNKhFoEXkifnBW8D9E8xFFzPwC");
    testAddressGeneration("WYJeHHGDdkPBGDjcPCBgNEySiquuAGRSMJrCwuyN1F1uKbfpo78e", "VvB3uCcCfRykQorKnTEES4NVjRAhEGxWxx");
    testAddressGeneration("WYp6H9vuFqGrwMmPA9JmVNgHqGVUAYUx8KU84xfRB3kpQV3je7LP", "VqCdRBiGBTUUtvmzTmwBUrZrcBka9AeGg1");
    testAddressGeneration("WaDaCCkk6u9Zzfs13W5Q6TBfF4GVS8UFzJqyB5yiy7HegtuWqaBh", "VbEwsxouaEHujHAdVZshVQ8JYucJJREpgK");
    testAddressGeneration("WXxgHFqzBWLCBAxRAcxKcvvTCPPmBxzt22oyk8XUWw3GnkQ5qRtQ", "VwdGLb6zXn3GWRhsms89ppzVgaLa4NZcLC");
    testAddressGeneration("WXhQQRaHUyLDikW85hBoJ7p9b1tdYkNzjbcSyVW79jbtrLjnuaUe", "VgBiQj35YfQDTfP8osTeD8xG6W37GVjvj6");
    testAddressGeneration("WULvHFM1hCNiFtbS7BJJ2w5v6TuGy81YWvb5ZV4bDPyivkUprpwb", "VprkTDRcuqYUzTzDDijNRDtuioRzH43wNk");
    testAddressGeneration("WXGvdMMsgsiDYfwHPwRVwrEBoqZcwKQrdr8bGTie5B86wQP2xCUy", "Va3CbYk6jNhxYnUcAzeGSyhNZ62C8K39ML");
    testAddressGeneration("WYcMUBSAxD7GN9CGvzHFa6JJKbAWqWryqJpZx8HBFfTTjBqRMv9b", "Vq9Sjxc3dznBczPXwUEgbHxQEaxhayud3t");
    testAddressGeneration("WTjfVVNLsp1MTarWwBE27d64TEZyW7enysDykisPsJkRVcPUKdhw", "VdAseqtqj3Nnawz5NjAntYuZohfwxUL6MP");
    testAddressGeneration("WZj2V98bSLQoKLT3Wgp1cStjNftwuaHnbjqGZXMts8H1QWZxgvNG", "VqwtEe6kqGyX7Z51jBfgbkt8My4Ke5RVen");
    testAddressGeneration("WWrk489u6axE99657bouwNR7o2Lj9F9sdAmY3vGBpPtcyTwrsRmz", "VqRohAQi83bdo4RPDN6aYfiR2YdWtUvq3m");
    testAddressGeneration("WYWeCkbKjrKf1d1WTFKjSsY5JBcpaUDHDM3Lm2m9a1zm2KwHYu6c", "VpPjqGwtoz1X3xo7cdDkR3g2CK6uYd8SL7");
    testAddressGeneration("WXkuwyzhKG6JPp3YXYTa1Bc572XHja6h7ZyESseCkkNjQfrNdhHZ", "Vn38oaoMMWhCWTDkwCP4soAYsm6wc64PZ4");
    testAddressGeneration("WYAufPWkueGHfSyJiFfBQvuEh1tCujPtEX3g1tg1f7vN8HBZs96z", "VdH9VqRRupb23UCmprSr33M216Pz71asXr");
    testAddressGeneration("WaRxAnzdofb6UirHqGtXPoPvYuU3HfNgXhWchVt8BUhx3Xi8mPSW", "VoQf2FbQNj72r3oTRKTVQvBCZS248CAMwz");
    testAddressGeneration("WYJ19fnty46z1qhgWX8g5ohLa5CDawbNKqu8cUN9Z9rEa8B2wsoF", "Vf9us5PydYGAYaYVfztS4CVZgkjf8NYUvf");
    testAddressGeneration("WU75PEVeNJEEA6PRvFZkJvZY8iT42NVeYEqT7ncKTnzusctwvupg", "Vw5fM4aTZ2kK1JK8bu5JJRHtx2DPkGHYKj");
    testAddressGeneration("WScp6qfbn7rErHeiho2fzHVb9vCR165q5QAeGgivEvh6BFzS3REM", "Vq7tqfJbZH8QaiM4M4BM5Xdu2ZgRvgiMUE");
    testAddressGeneration("WTKSoGCtiScLg8zbcy8T37kNdJU8ccysKoQ6khMXVY8fNz9PVY4g", "VjySTiuULUiUHyHY6aSwE1kCs47muKCCae");
    testAddressGeneration("WSn4hTEGZ6WctPCfR4ZnpT9oCnCWAjLGRZEmNkatZJNrdedFpLZe", "VwtAjdT4wtxqvRKiY55DsGaKbDHyeRQfpX");
    testAddressGeneration("WTe5YJLqaKe5SxmUCfafTvQvdVqGh3v5Zj2oetJbXeQKxpWPgb3f", "VwtVzT5BBJDw2E37hZRjJD7pndoDo6JHSj");
    testAddressGeneration("WUy962v5BXjf8T6jjQfsDjLpiu1chhWaNHqEZnW9TxQoY4zgYaAP", "Vw2BY5swApS7PqhPbMxqzFhw4gCFsPn2Zk");
    testAddressGeneration("WTDBVAoQHfHTs46fTLnsoQYDBVmCLsNn2QfRMyWwLRRAY9VWcb1X", "Vsb1c8wi5ahHW1obCRrBtTRL7eQVRsyhfv");
    testAddressGeneration("WaY4CGgRkZRe2GsvoeZiaZ7eNyAUcfSRMHggo1sFmKUzLDRZKH3P", "Vnk2HTktHeDMDdQsyuW1uFj6QWWyLmTJ2M");
    testAddressGeneration("WVb2MRvdVo8TVYDLxm5WEucjoqTJ7hvp5qC2Jyao7ZENTUkogmez", "VcYXhcErzGQk8sMQr382ogwzHqqH65UKJd");
    testAddressGeneration("WWyLaKXBoDYXeGNDiNeCi9ydShuSitNYyCYb9KU2KRwYehbSsrsr", "Vt5w1t9bb1cMjV22LpW2ggw9rBYof8E1cT");
    testAddressGeneration("WTiMoo8qGsbUsUXSDjX5Rn3b2KHtCx9nceYw5cs2nMja1gkPj4w4", "Vbe8c6TcacXLDPENMgSdXLnhREWo8Rq5GW");
    testAddressGeneration("WVK1VknKRW2DSwiJsp6Q8eud4oQa6hCWnVsRsCijX5pmv8wuEhpa", "Vb8K6xizeVdDnhXi7rwKw2RGmaSoMq4QHC");
    testAddressGeneration("WTzrynysMenic1MitBArwChnfTeeVH977U8gfvRXfbuuZWAhwvWY", "Vr8y6CgQC2GNZQFhuqjpra25RjCLTTPXeB");
    testAddressGeneration("WY6YwG521yVY5NPcmBuFkDfRiWFTqi7EyJae8fv8Ka5PZogY6pfX", "VvpKZpiqJ1mf9zoeu7Zh4n4jcjvrWixmvG");
    testAddressGeneration("WTi6JRycvcbFpNLfFMSa2UrC3Yy8S6tUue9LkR8HKJCSecxKkaTy", "VidqVSW2YVgUg3mFK8exTPuWaMV6S361P8");
    testAddressGeneration("WY7Mo4ZpY5P6m2ZA2oUTuL2fpRgxxB6sYugwdatxjaFraMJF8XpH", "VvRuvCrEWhgjMQETFYKDjsuVND6kd2HYCH");
    testAddressGeneration("WTeCJEz62LkSwfeDJQGKSU9396zDx3TnoQoEBqWkBvUpWnsCEUkc", "Vw1uTUHHgkVq338P2Dms24N8zSWVGkLx7Z");
    testAddressGeneration("WaFfEF5ppv79cjMB7jwcVZfUrcgUjLTSbdXpu1pFnLVmSHdaXK1H", "VyCtG46zR3GeRV8i3VbH7EhE2EL8tqD26c");
    testAddressGeneration("WU1wDixuNP14Y3ELg66DDYedKVBrnq9hpEz55ViaVpRRXHHUESwK", "VaHzEXPG2R5nxPoB1uXtsExgwzLXGhd9Jb");
    testAddressGeneration("WYR2wzXFySRjbWpHszFKqgb1Fom35TUEtrg9pghFPXfKhva35b8U", "VefpUu5bWrYzRBrsxUBqLgKLBfJHEqy2A8");
    testAddressGeneration("WVV5QxrfaczZXDow3aJKYCNWsr5TkACirVVFmUhEr8uMzgTfGtSG", "Vtaj6MgSbULKcdTCx3LjMy6o9gWiEV2b3S");
    testAddressGeneration("WTDARTegWYHHhysmoR9oDyeLWshWiW7EJKo8N5egzoDRF2hU2Lxg", "VtqxP7De9eeVx34HJbjULonYHobfZdrcV4");
    testAddressGeneration("WWEjZ9cciKFJXRLkdb1UkSJVQL1kB1zsBrsqwJ7Z8KkZ758N3gEu", "Vd6yesxkbcsxfeq9gSk8tjm3drsZya4TH2");
    testAddressGeneration("WUhVc6WSgGErRvPgdWE5iK7JujggQJHQgFryEZ7uYr9tJGdqcELt", "VhkTYMSaKMdQjLnKdrLocLLV5qzu1G98GK");
    testAddressGeneration("WUsFUZ6nVyGNH8gGbnHXzs47z89mDUn2J8DUftp6s22n8QZb2j1U", "VkkK7fRbHQQdGe1SKdBenb5hSEtbJyp5kA");
    testAddressGeneration("WUuw5y34JJj9qCjgmCXyKzS348VzmWM9JPyjuQFXEZXXkYqm8Q49", "VfRXyXgjkoD8XsVR4KCC9gABkpYi6SZruC");
    testAddressGeneration("WW9yMGx8DEFkZEUiko4Ut2jkd8vhyh9xApVmf1RrP5dTKn4EeAny", "VdsS7tWpQewmfc8yx6XTKX95vnRDsB7qGP");
    testAddressGeneration("WZALYqjSDAmNEXZNeYu1ugC1mHjJrWo546MfVJLYanVdHXVcDtBP", "VhfqYDyjUjN54sAM6oJRktzJuj44HeyYqz");
    testAddressGeneration("WU4WWy6VBKKfReJXD8qqFXfyGkPYrWTnsG9zndhmJ5WRaNZidJtH", "VcUzjxFUo73ijGGWsqG9a8oGzbX8YMGxGS");
    testAddressGeneration("WXCMo72n1yDZzi5i6t9SWCmt4usfouYorA1ymuP5L9LekNt5RkBj", "VvyzQ9NaJTguA91N6MGGmrN2sFUpTmet8h");
    testAddressGeneration("WUmtbaWXYaQRvmixKwUWNmJh1h43PsaVC48JJGywgQ1RFG2whtFg", "VgyrTm2ptz3NjL1cFEnXvPLbd8qzZ8MQQ8");
    testAddressGeneration("WYo5PwSNNhAFw6hDcThnbe7SiEGuYsX4ccUwvMYrpw1RtWQCAMtZ", "VrUon2GeQXs1JEgvvFb3UMmyroTCxTtTXB");
    testAddressGeneration("WStzgeTnsDi3skHBHSJ3UBxatHNxKZbk42HRXk1xWWtq53f1cACv", "VdPnTzDJG7y6gQJQfr6tFGBYbwKqXpkCwR");
    testAddressGeneration("WXP9rCyj1fREKeRJVHkhMwyo8NFYFaSt42zjJYWbNV16JQPKCeqY", "Va4mRqXiyftyQoL2K1Jz3MwpCitk91mzG6");
    testAddressGeneration("WYPAPrkj8Tiw1Wi5VZxVi72WrjYuXGAkTq56k2X9qJ6vp4M6HKSS", "VhGqiwDmMiZLZ4Ups9DoWmjcEurpSeV9jv");
    testAddressGeneration("WXzEdD6W6gfWfL12wEW3MUArtWoLvdsBHTKv9D9i5WAF16bGB6y6", "Vh885gYaUVKoQ5WT4m9xBbHrANbFPiQRVH");
    testAddressGeneration("WZe1vcrTZJHeyCKWstUNNFYnKaGVLFEcMk5J5kzo1g6JhVHSWY6e", "VmRhWaha3uGuffyWzYb8vYinT8fpg28riA");
    testAddressGeneration("Wat7aK329e1Ca3KmZy2tfo4dzLC7bbWRXEbWNZZJeBepRtRxhjHY", "VmvPGdc5QBz4bnUNT6HYuC7LMxSTc2QCCi");
    testAddressGeneration("WXR4VyyDtrfycgQU2UGtuSLWFZhU5rz7bBcue2YFD3cEzqfjbyKy", "VezYE2UEuoo5i4oAA65p9A9qgZR8K2suEw");
    testAddressGeneration("WTTint96UxHuLv1TVMHDBExgtQw8Me1n7uuHwdepAJZhXfJNXA3Q", "Vdmfu12GRyqZAkNRP2osZ25efu6Wh3yNtG");
    testAddressGeneration("WV2oxv2dmBApc3rPeuTKaxctYWvq3rNZK8htZ2NoWnpEgU4RijLJ", "VoTZ1pWYDHQ5kyzRawtrZnMo7uXXyNf8Nq");
}



void testVtcMultisig()
{
	std::cout << "Todo: multisig tests" << std::endl;
	std::cout << std::endl;
	
	/*
	std::list<std::string> pubkeyList1;
	pubkeyList1.push_back("024c0eaf4c884eb22191ab91b688a27b03ab2150ea33a7dec88bd6707d409abd79");
	pubkeyList1.push_back("03b13004940d81f0f3c6de5961ecd4bc3fbe5d238068a8fb76f0b384e88bd4bfb8");
	
	testMultisigPubKeyGeneric(pubkeyList1, 2, "3247N68EThZpuvnMJJBG4juBXGYV6Db4bm");
*/

}



void unitTestsVtc()
{
	std::cout << "Unit tests for bitgen-vtc" << std::endl;
	
	testVtcMultisig();	
	testVtcAddressGen();
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
			unitTestsVtc();
		
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


	
