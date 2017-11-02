#ifndef KEYCHAIN_FUNC_H
#define KEYCHAIN_FUNC_H
/**
 * keychainFunc.h
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

#include <vector>
#include <string>

void generatePrivateFromRandom(const std::vector<std::string>& args);
void generatePrivateFromDice(int argc, char* argv[]);
void generatePrivateFromBrainwallet(int argc, char* argv[]);
void generatePrivateFromHash(int argc, char* argv[]);
void createAddressFromHex(int argc, char* argv[]);


void importPublicKey(int argc, char* argv[]);
void importPrivateKey(int argc, char* argv[]);
void listPublicAddresses(const std::vector<std::string>& args);
void listThePublicKeys(const std::vector<std::string>& args);
void listOwnKeys(const std::vector<std::string>& args);
void listKeys(int argc, char* argv[]);
void setDefaultPrivatekey(int argc, char* argv[]);
void deleteKey(int argc, char* argv[]);


#endif
