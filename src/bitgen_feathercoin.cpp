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




void testFtcAddressGen()
{
	testAddressGeneration("5nP65L6Lk4rQaNXy6VnQhN96oyKFu1fD118RVG6byb5yz4FAC8n", "6yvFR1FeWgUXT4YXc1tQUqri2Rn52KiN1w");
	testAddressGeneration("5muDxyFNSxS8nELGgcG9CSb4QxcoyuDQt1PubcHcD7E9x95mn5F", "6vPeZRSJ9wZ5PCDNRNiNBPKmDwBBH1qWAd");
    testAddressGeneration("N3Nj2k1Y9wiHFrSvcGQbJMEnkY1J6YQZbiiYNmeHbYc1NGWbd3fe", "71ipaGCo1VHf7zs26uybm4yF5h5FYam3dB");
    testAddressGeneration("N4p32DUqUGjaUy8rL4BcAYguNjAZkSqfrP6aVEEzLkXD4AEWizB4", "6kviDtLB9zdjT5u4pipUNSCZbSkoQS3pVP");
    testAddressGeneration("N7ZyUW5yXf3fgPqQXc1xta7ra7s82PuPVofyXJ58EzAgqzrWHp4C", "72pArrdP66yXUd3q7Kc5yRz2WGrjNmDbXc");
    testAddressGeneration("N4gsLJMqpqSFJEdrU3t48W4ZDNPFVJtkJbUjwRfkD8tZ3DNPhYmA", "6oyv29VkZg8S86yAuefErnwFryXNJpnoMB");
    testAddressGeneration("N45FLv1HYCYtpkwnL8fBQZ1WeLDF55TGVzPC1tBKU9zCoJWtFUDA", "6p7tKA355NZAVcd4gLHzcfJJjPJDJKzS3d");
    testAddressGeneration("N7iXx3XrdWXjTvoEM9c8Vw1HSDv6y5XEJCWQf1QW38o27btU5aXE", "6nEmQKr5QgkCavZZ9R3Rv7TzoBV4HH7v7V");
    testAddressGeneration("N7oVttmBPthbH3pNss4zY3zxgZ16tdms2t3YHDKS5c4RvACRjWLP", "6keLqnfWMQGbAApsJa1G9nXNjuLk51troR");
    testAddressGeneration("N7qKRcyGFWUH583fkborjKwv9ZxrufR79XS7sUknRzaWctCE96Ux", "6rkenpdeFELY7Gv8CaNhtMmuRkDXVkuEKR");
    testAddressGeneration("N8hobFXqECNBPjLZEDUjvfPGjgX2XFR1HiUVesbtNYM8EEAKjfFD", "6qWFNbyQQyHXzHiTwBNdTGAciuV2bTs4Cs");
    testAddressGeneration("N5JCRRXPFzpvzMnvJsDq7QFTNhRYEwbVwfGKSzZ1gQiFfZvPMe4m", "6zYPYX8ZrLsdjRTrKnYjaLzet6LD2tXxu1");
    testAddressGeneration("N8aHrKaKmdKLBG6tELp2MC4ktTGjkSY1gaj53jw93JaaUbtb6fKX", "72Z6voHbt7Nmx8KqKNqXJjbw1WBieTiTLF");
    testAddressGeneration("N5bBJV96LZ6DvX23QcYqvSbEXRnZj15whGHzmrL1bqajhKUvTBoP", "71pZbaQk6yqheAZcay6MQR3j6SUg7YHErM");
    testAddressGeneration("N7atUhU5ZuyYd5RPXNTuxhewg8p8JTncAVbeY9CQxe38tgiFRx2Y", "6n1kmsDrgaibGiprjDmqQYvRbEHmCvd573");
    testAddressGeneration("N1YFXAR1HQ4VQAfwQKEATnCFnn7yXUzafe1iGGFstadZoscVHgeu", "6tHZzXAoxbwCYJWL7Yq2oFYc8W2aF3S4zK");
    testAddressGeneration("N5Hcp2XFv1nvWnV78DawsacFXRmjuFKnYHdcMsKhPKAbYS3RjyoQ", "6w8HRmN5KdcCFfG7krmJbkP87CprQf4Bdz");
    testAddressGeneration("N4TfYxYbNAiBJFGmvcVwYcHHoVa9yZ91AcMXYVam3Seyuo1oTkrh", "71HELb61XZUqfNEZmcWiYGrAqanRzczD3E");
    testAddressGeneration("N8wPyELco8kVGdsS65JjYiZAxsi1uj8LXuCsYYQ6N9GxfZW5UGr7", "6et7a11bcnob27GHDKR9284VQBg7Bk9MiS");
    testAddressGeneration("N6mVwGNpjbWkAH7Swg28gzMtGnXF2mERYXY6utnhq8e43FMegKQL", "6gu7gWXmdrhHzLG2KjBMA3mrr87Lb5vT6C");
    testAddressGeneration("N4FiHa83fwv79DbMRGNQBaxWmqihaHJyo2SKnmSDuXUChTgj2RMV", "6wT2TtNdZyfVyieJqEQb1Q5FE7RTJMHzEA");
    testAddressGeneration("N3z8HL36hDLsapWw117FGvz7gQwnVWLiBW6KZiG5tkuwEdqjvU94", "6zQR1bLckQYa8kVJLUUth4xbqrhwqHbvzV");
    testAddressGeneration("N5qKJ1cDLwueFZR3YRETEskmshgdBrpd5apaqNb9amUs3kYypVZV", "6rCA3waU5fjbW9ybWe2Uwb7RNidfdFuDL5");
    testAddressGeneration("N3M7gRHqXozj4ChQ2yN1APGjApFRhUmF29YuzQt9v26CKeYMBa2k", "6nUKjB8sZkBFxDfhJukMz9ZcX6SU3go6WJ");
    testAddressGeneration("N51BUkdpyMhUuW6zypqE8GmF7b1RJwZz8D5PpRR9P5b33pihNNnH", "6mVeWSi5KxQPxPR61YhUmWGjuzEitiXNuk");
    testAddressGeneration("N4q7wk3fc4grd88SrEkLw1dEudTp6agHhwi3v35VkUvQUUMVh8TQ", "6mrKUhBsnrBtFbysmziV7dWrDYV6T1kYEz");
    testAddressGeneration("N2BVSpZ1XMWRXteosNgY3yoThmBti469TVVdD6Go2B63nhc8vMne", "6p2dqkAyvGwjTHpAxGkvvUx4KAA1A1ypkm");
    testAddressGeneration("N3iTAtBeAjBJtgL9KaZTDAb6YqUiy1WXo4Zq8zHXot9KBZZwt9Ux", "72ymqLoZhkAg5zSkaEXK1RzXWey28Fef3s");
    testAddressGeneration("N4tUShF3B1pWMrhgwYENTDuCJUjNGp9A9AejULTeMqo4My1wbRhs", "6kYcdJ58JiWQF3zuS4HHFSVYZjC4SkRjDB");
    testAddressGeneration("N26297fzkqgNHnNPUuFoimT3C9UfSHmTWSTFKS5D7w42j3GcBFK7", "6yK9jXrepVBF8uBon9Lk1LzFLeTXPjQmdE");
    testAddressGeneration("N3nAeFQ68MHjxqtd3Cctn84K1Vj7EQ3nmJK6Ahp9e42iKU32dnhE", "717LVe7jDaB7PyvezQyWE5sCQzi3wQzTZp");
    testAddressGeneration("N2aeBYTf7ScCZP2gueSH3wD7AmhyWuLvWsCzcbwHRhNqRS4utbMP", "6rWED4yFs2FWhT1cty6JxptEuKah5v8VGt");
    testAddressGeneration("N9dCKLNkCF5qS9CXMgHvkrReD6LBa9eAnsvMLarfL3Q4PFHrDHfg", "6evc8JXLdeVHVuVWywsfBLp8waEESRfiwM");
    testAddressGeneration("N2iZj7dSdQ2zjeJ2ko3qwqFL1yapjVg1yT75zSzPGCm9yKmXUzSg", "6jN4LzD8S8z7Uufynhp36DjkX6WdfdzgAj");
    testAddressGeneration("N2bTmZeSy36KaA42KAxuEm3zYR4T5TK6SbPNHBmqZkL7Q9RyCCVF", "6u1Ecyww3jiVm6Pufu9jiKjedeHWMD5GRf");
    testAddressGeneration("N39N443nFf7hJKJSy1VGhaS1hc2hBBPLAYXZGrBcPZKbXGi54d3g", "6teiYX6knyqvE9JQaPxpQ2wjwCuKpaSZC1");
    testAddressGeneration("N5QD7UHkH9iZ4US6CdsAbpVuc1U3VdNUKGMZuKq7MRneQ7aLKPxD", "6x3kNykWuEV2T9sEAddPKk2DwoVZAiLPTn");
    testAddressGeneration("N1RgrPWB3hF4TYLARm1hQVZAZxkRKtqTqFyCvdfY65HCqYcnCwpn", "6mPtnLXypDkpTDSs4Su5LzgPL77y78sSnD");
    testAddressGeneration("N5kA4shoWcw1rVcGNqqQ8Vsr1sS7ctrFK42v3UDaPCNiNz3jsCiX", "71fTJoT45BzWrk7oowgptYYtwd1rko3GPr");
    testAddressGeneration("N2WJfFzoxpAoBWws41wiX56ZDBerX6hELyHQ5ymANXEr6VimPyAV", "6rZdivYYzby5adjtHN8tyT4xMiJdmbNn25");
    testAddressGeneration("N3e24eGNkEX8qmSMXm3hM67pZmuHTyKwpeBSjGdzdZqZeqWJGVig", "6pYESjZBGx6d9oTBRZFtvxU7jm1vMGcjTE");
    testAddressGeneration("N28HoB7FttkgXdY8z9pYk8g7qrXydayiy5usYSEFwDm2uARiaCCN", "6htoHzG2bmTeyWWNoNLVVoyQK7FsYLaBn7");
    testAddressGeneration("N75M9f9hGRJ9RHN85ZazTjkiGceVLRqzFskpfLANTCS2jLqh3xGh", "6vZ5A3rzGsLnezkq2EjufHPCKA1LVdmDUf");
    testAddressGeneration("N5NgiLPJJr3pU3JU9YtuAdXYATPyoQk6JhDkhZgsFLK11wUTFXYU", "6rkMBir2ZH7qkJRvWBQSFmaW7C2vyg44jv");
    testAddressGeneration("N6GdDqpazivDcavMwYWjGbtf4U6qARUDtWYjWTmSaXVEPC3wULox", "71A6GPghrNgYyr5zVectWLAEGCWCCEvVbL");
    testAddressGeneration("N94QYxJrWxN4LmWtapoWZZ7ETmyeSDk2g2tSXUjaTKihsPasg2SY", "6puhLfzDobXzoRMdmnyy4Qh6R3Yc6i6suo");
    testAddressGeneration("N3ANU5ZMAacgZ35GN9rpehmpiKif8WJD5NmLTL4zenXqiwxg8obN", "6zh9w8p1F9kiQjDnNasGGJciFhJ1H3D3fj");
    testAddressGeneration("N6YuoF9AjVV97rQpWpqCKaU1ABG4g44i3zUREpz1dq7Ax5ihSS2P", "6yo6qReAeB9pLD7cRF6Wo4Hk9R8NB7dkSW");
    testAddressGeneration("N7BrWDytCtuPEUsCXv1WmHNJJ47dAn6qrqbagzEty3cw9Dvw3Zx4", "6m59tJhuKmsbSRYhZUhPNKeS8bZ6tG4vTW");
    testAddressGeneration("N3pMUNGo97FQ3wZuE9rrp9D21XS4ab6BWwx3jYthZNKAQfoScCh7", "6zfnvujFYeAnv9UNdgejLuyNNBkyksNHGk");
    testAddressGeneration("N59db5TmLJ6FNu1gNf3iLmBpK8qgzcQSkcqjfbgTMP9VYD3TBsCU", "6zW5nmnbXcbQbyRbD1sUsfkkRsZ9baCpRn");
    testAddressGeneration("N8sb5dVtjn8RpAnq1UH1cGkZLz7kpxcUrnyU6pgQhJwWVXQg7H3G", "6wxzAsK6TT1ATxi8a6kkRCGMmMVaLmv4yG");
    testAddressGeneration("N9MzyJoxZNGaPownvguHekDYsogAnwh9Nf5vP7YL6qkUnHRKbeo4", "71UQ2SLngKLVD1MseannYWob7bg6syiuA1");
    testAddressGeneration("N5zeLMMb3Gdr2Sq5mhxLUMs9UHkMh3SLkCHJLgjU7zT8hg6HnTEA", "72UDYKV7zkp4r9Jvn62JWMvDRxD3rhMHPW");
    testAddressGeneration("N5tXMjDnCjrS5aCuYV8DF8nAsx4HBYb9pSPokDdLeHc1wDNoNdmx", "6kxqNDKirFSEDnonFcvL6Yw6XqjLPsJTdR");
    testAddressGeneration("N44riZswxMxM67zqaf64sRnQif5HLBoLDHgBQpmHM8PuuSacmAxF", "71XAExCooHBvUbdY6466VMKkrcaT4YJ8j9");
    testAddressGeneration("N62r3Jh4xD3vVkdRJN8APhELzEdQBGbCVPwsDvU37wSdgTrYjxH6", "6ngrM7RaaDGQiG3dAgTLNjCuhANBKrS6ar");
    testAddressGeneration("N3xRQBxzcagKWWg42Nd5iGBJW87WjeE5GMyuPRF39QEN4wkr5EBM", "6gFmicNLY2z8pEMHEW1tce3n38Tya6FyX8");
    testAddressGeneration("N4Ef1WjypmnvkxNv57X85AexxY21ZfjFotmQgUFVyR6qE35Gi47K", "6kJkjRJKJB35YxU2Qdmceb5FMxBfTaj6Sv");
    testAddressGeneration("N2LFABTfVFLR1YDsi8jFtEfJ8RtpgurYYJz9FemnwCuAV2vULUvT", "6s3feckcBxD7u9Wq5p6WcaxmwRfheQpyvo");
    testAddressGeneration("N1o4M9YV9PKGAMSxXnRRr84MPExnV38y8VApWSYPwipxq8jUzgnM", "6hFLkEznt6ToqyuWcu9H8YuUpP4rw4wR66");
    testAddressGeneration("N9VjYtLguKt5vF7hivajbXefSbdCokqDjspitJEgEkTdkvoMYJak", "6zZp55Xx139BfbRDmyPDQNzLWKS8jM8Vxk");
    testAddressGeneration("N4jjiX7iGGk91SRCYCPLjwf1FtVvmtWnYXV5NApksZfuD2xzmRJn", "6wDrsueFK3Dh8wr4H8sRbMoFqXYBt2w2Hc");
    testAddressGeneration("N6xK5CVZE5oXLwt58U1rytjpjXcWSDgg8ayK7omxPSvMBvK5dV7Q", "6kVcpy4oEDjGt4HcSoNzpxTchgt8Sg4syo");
    testAddressGeneration("N98K5VVQj9Fx9w3TPEVKwTCM6FoxNg8mHwKt4pw2gENaRuVvSuWc", "71tMya8bVi7AmYMX8eGFuh2XCxAoj5cRKr");
    testAddressGeneration("N5f7d7i6VVH1rxQSAwrnr1nPeGbVNfVW7ebnVd98CGmwrdgKGCzz", "6iqMwMbx55sCHtpW7Nef3Ym5EfBijiQXvC");
    testAddressGeneration("N8LzuNdZ2VnK1SwFHVQPEe2ZusNH8ffDiCke1yvpKZYQU2K6ChsL", "6gbxsZ6xsfxBUCPhL4TKGKdGKuyo8gQRUf");
    testAddressGeneration("N7JFtcbWTSfcB6FkGq966d8rcDUsVbqpxTZM2VhTqPmRWiMywSdL", "6q9RAfCLEoaGc1vXdvCh6tL8EEE4PxGDie");
    testAddressGeneration("N68Le8hFZHto2KHBUiibfW9wbZymZJ92cvHq4j3A8x4nHTHd61S2", "6gFkJNxbBgjdXxRr7SNBMcnHVfUizGGSaB");
    testAddressGeneration("N6hoQDESKwSPNb6T5NTAWBRLJWFex1GyPhTBFvfZrvpbfDEanC23", "6w3XqAheApLdpnBWvASGU8ZnuT4CzqvGgC");
    testAddressGeneration("N3ZZMHsR7Qvafvmdh15xMivNxbDjQ9dYuzj9eLAHsF1JSvirvd1X", "6fG36A8MB7odYu1Z3pwKY7KkEX8umZ8y9t");
    testAddressGeneration("N1XTz5vX7HGTzs9isb46bnAqaoJe1cGgmhZr1dquLtLJ2HQdkV5t", "6x9T14a6DN31uryRfDV1B721rzYvvFJdKB");
    testAddressGeneration("N4kj5aHrjrankTcT9pUVqNpii99WuDzHmYBW5LZbLStJayxk8NRc", "6tzd7yBouE23vfv1pe1PDc52P6kZvmdqPm");
    testAddressGeneration("N5m1JMRFyvLoRpSSFcKAmW3hi8MxV3hxxuJnNfgJfVaUprEAhKLV", "6r4NntCxLM2tNv1WtPEX4qET99TzZxbBif");
    testAddressGeneration("N3WDHp5ab7bjquD5R6wsm4yBmvnxWsAfz5oJjNFfaBPia8346AYf", "6yyL8J2tMNvpt5bAhSDoWDV587zn9YZPJy");
    testAddressGeneration("N3UAu378DxcW2bV7Ff6tGDkQoMnGoEKp3v2KhkpH7rtSRrzseXA5", "6zQou2AFp7bUseqxEBVRy5zH6HkteCjvJM");
    testAddressGeneration("N2BbTmBga7XZnGGLfuWPZRp6BwEcsD1UWo5KxQH4iJkxS5VMM1Zi", "6w2AFpMfM5iTPBKv8E1ZA7WN4X3aNLwuQW");
    testAddressGeneration("N6ex5NmETDThpst2FPnKfX2ei1owGMMZvjmLnzwxM5FuiZiUA1gp", "6sGu8yr6qkuXxMcC3e2kVbqQdVaRLRUo8j");
    testAddressGeneration("N6bQbA8eXA2m3PzR7XK9tu64Z7bew9tth59kuFpB9eevGEevi19P", "6s7YbsG8WQurAsGmYraMBxiDHt7hJ3d12g");
    testAddressGeneration("N7vjncCmuNSmSy6LZF6WZmi9tGrexqtGeRH2JGFnY2KKnaJbjmsE", "71CiBKdUGWyMfeoLmXwUNrTcWk83TMjnxM");
    testAddressGeneration("N4BpJsdRHUTrkZ95CgrBmr87KsVjxsq8keBP57uLjtSGcfzWpW28", "6pjMUh8PErKBmJDbrRStqgEDskcbqmzqCJ");
    testAddressGeneration("N3VrRxo9DSTAyryLuRhuoEto2FHBqfb8dhK3xovgEw6VdCByKoHG", "6mSwao2kuZQtuFU7KHrXh4tYK4uke8KoXZ");
    testAddressGeneration("N9sD2fx7maVMxDANb5njSUYGGsnvMa9pTm1ESNsjdPkD3sTE1fLZ", "6sJHoxw6H7Ukdh3RZFJGvaRjeuUyJNGk1C");
    testAddressGeneration("N5EVPkacR4B1kgsgDtmURAPYB5iLviBboMgDULvLhNfs2TSLzq9S", "6qTcTj6JQitfGzTHo7GXX6no7XZGxnxcp8");
    testAddressGeneration("N6EqDJeZohaLp3W3b9LM5euz2Gy48s6e168CD9qBXri94b6CHmJz", "6fGFGnG2qQKS6VEQgEGFM1YfJMTpVoSrA6");
    testAddressGeneration("N43ygnMo9QWnQzQpirXQFpWHso7VFSvmGHDKuv8vSLT7dnKa7Bzt", "6qXQRe3TLpLFBUMoJqGfgt5RAHS8464p6G");
    testAddressGeneration("N9d8p6yCgrLowAeSmar4ZJTfZgsT2Q8LooE548wThVw5cHLTC8mi", "6iFF4D699B8xPcXGwTkGrwDzKtkrkwwtvv");
    testAddressGeneration("N9hor4hyuHrFysDe1NUjX8vxa2xPWkvHTCQFJrmv5dPm7mc1ymx5", "6z1Tcp4vcF1CXmb8Ej8SNy519rYSXAbLa1");
    testAddressGeneration("N5G9qcP8LShXgdT8uv8zT6KFdYDbZT8qXfYdALJW4U41VG3b5wF6", "71cuUqUajvWrDxDSzZmTXEw1pqBvQCWhDW");
    testAddressGeneration("N5G8F67tE2JMokJFAQ6ehBJSeYDEGQyKkFYUcBFLu7EuuqSXEASL", "6iNjkGuGj9BeQKAXaSZho6ByfJ7L42U2y6");
    testAddressGeneration("N6sDEhoppy9YHXbhpApfybVkNnyZPXRjKqo9J5b9yTc2ZcpLrHC4", "72dJ1E682HUf2YK4rXFfN9ad7jdTm2yBH8");
    testAddressGeneration("N9JcxfUyWSdzV8XTZGC6gmpu41QqVNEGjgQN4B9pTLni52F2gmdv", "6g64AVgKVPvs3hFCCmBvkXYPB1rvJeVShY");
    testAddressGeneration("N5sHRBQ964XhsvCwdYSGkbPWDLSNaq9hKaRBxurQL6NkTNC6kQLA", "6vYvqX8Q3i3MxzA654PQZnVfrwZPfAttfs");
    testAddressGeneration("N2iKvD9jTzfsB5g9qwoTL7b8theZhp4n1fr8hkd1WZrhp8D169rc", "6o9a9mW5t2RNa1eTR4csgDZL2BCeBt8iin");
    testAddressGeneration("N7eXsMeitjBSDtY5g3BVa1TvdYeWMD3EUVUBMFvWxBBZuRqj5s2A", "6qBaQvG7Gqvg4AdXVp8ytjfXfrneWCY6oY");
    testAddressGeneration("N8An1KuukWU2iHcnKkDf3Lc6CP8B4jdFcukEc7qFTKMWymgz41G7", "6sWpVm5n4SKar4GxzHq4sqGXsLsWRhsrcq");
    testAddressGeneration("N3o85PxVn1tbujBccF29JdB8XiCaKkyUGYAMDzfXUci9hzHgk1Wk", "6e7rKBMcKJFHsDGW6ZyAGeiDrC5L9LUy56");
    testAddressGeneration("N5yNPTz7HjeWKwgLZZytYk4QS32NdrRkFWHgewpprPTjSGUhQmi8", "6t9XRS1ypG14HzZYyFKxDf5c1kexw9PECk");
    testAddressGeneration("N6yYKakiU49y7TFyMW9afQzmpXLhG4iLtuAKk2ZdDjtWvqY4x93W", "6hzgMiuSPaTNNSwcA9b1wRgm5SVhPZEpJQ");
    testAddressGeneration("N6ypHTcAdG2brbCCcuzGEXgs2YcxYbq1ic4jVm4TWfrJPvaetpM4", "72zuw32HZZvrbqSQhVZHzi7dwZWZBaceZQ");
    testAddressGeneration("N99ob3Ejq6adGRHKk23TNeGEcj8qbtrKxmUF4faGa43wvsXpEVQp", "72NJhQn36m5mRXx9fcER9QWvAHhgPtnRz2");
    testAddressGeneration("N8KTUzMp7R3Yvoc45nJ5tvqUqKVJ4Q97x9V2UdCXYALr3LBNrbbh", "6mXxnbhvqzVfoc5GgJj5BZ7ekiaxK9p45z");	
}



void testFtcMultisig()
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



void unitTestsFtc()
{
	std::cout << "Unit tests for bitgen-ftc" << std::endl;
	
	testFtcMultisig();	
	testFtcAddressGen();
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
			unitTestsFtc();
		
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


	
