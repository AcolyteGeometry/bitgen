#Generated makefile for Linux

ifeq ("$(MAKECMDGOALS)","withoutbw")
	COMPILE=g++ -I. -Isrc -Isrc/bitlib/bigintctl -Isrc/bitlib -Isrc/qrenc -Isrc/argon -Isrc/bitgen -c
else
	COMPILE=g++ -O2 -msse2 -I. -Isrc -Isrc/bitlib/bigintctl -Isrc/bitlib -Isrc/qrenc -Isrc/argon -Isrc/bitgen -c
endif

all: bitsig bitcry bitcalc btc bctest bitcoin bitcoincash dogecoin feathercoin litecoin namecoin peercoin vertcoin 

withoutbw: bitsig bitcry bitcalc btc bctest_nobw bitcoin_nobw bitcoincash_nobw dogecoin_nobw feathercoin_nobw litecoin_nobw namecoin_nobw peercoin_nobw vertcoin_nobw 

test:
	./bitgen test

bctest: obj/bitgen_bctest.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bctest_specific.o 
	g++ -o bitgen-bctest obj/bitgen_bctest.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bctest_specific.o 

bitcoin: obj/bitgen_bitcoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 
	g++ -o bitgen obj/bitgen_bitcoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 

btc: obj/bitgen_bitcoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 
	g++ -o bitgen-btc obj/bitgen_bitcoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 

bitcoincash: obj/bitgen_bitcoincash.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoincash_specific.o 
	g++ -o bitgen-bcc obj/bitgen_bitcoincash.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoincash_specific.o 

dogecoin: obj/bitgen_dogecoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_dogecoin_specific.o 
	g++ -o bitgen-doge obj/bitgen_dogecoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_dogecoin_specific.o 

feathercoin: obj/bitgen_feathercoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_feathercoin_specific.o 
	g++ -o bitgen-ftc obj/bitgen_feathercoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_feathercoin_specific.o 

litecoin: obj/bitgen_litecoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_litecoin_specific.o 
	g++ -o bitgen-ltc obj/bitgen_litecoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_litecoin_specific.o 

namecoin: obj/bitgen_namecoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_namecoin_specific.o 
	g++ -o bitgen-nmc obj/bitgen_namecoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_namecoin_specific.o 

peercoin: obj/bitgen_peercoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_peercoin_specific.o 
	g++ -o bitgen-ppc obj/bitgen_peercoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_peercoin_specific.o 

vertcoin: obj/bitgen_vertcoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_vertcoin_specific.o 
	g++ -o bitgen-vtc obj/bitgen_vertcoin.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_vertcoin_specific.o 

bitsig: obj/bitsig.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 
	g++ -o bitsig obj/bitsig.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 

bitcry: obj/bitcry.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 
	g++ -o bitcry obj/bitcry.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 

bitcalc: obj/bitcalc.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 
	g++ -o bitcalc obj/bitcalc.o obj/EncryptedResult.o obj/ExtendedKey.o obj/GenerationMethods.o obj/GenerationParameters.o obj/HierarchialRandom.o obj/RSVal.o obj/RangeSearcher.o obj/SaveFolderHandler.o obj/Script.o obj/ScriptItem.o obj/VanitySearcher.o obj/addressConversion.o obj/aes.o obj/amount.o obj/base64.o obj/binaryTransaction.o obj/bitcalccli.o obj/bitgenConfig.o obj/bitgencli.o obj/bitsigConfig.o obj/bitsigFileFormat.o obj/commandLine.o obj/encrypt.o obj/encryptionFileFormats.o obj/hmac.o obj/keychainFunc.o obj/license.o obj/miniPriv.o obj/predicates.o obj/scriptdecoder.o obj/sigFile.o obj/signature.o obj/signatureFileFormats.o obj/transaction.o obj/transactionSignatureChecker.o obj/transactiondecoder.o obj/unittest.o obj/usagePrintout.o obj/argon2d-ref.o obj/blake2b-ref.o obj/Directory.o obj/FileUtil.o obj/WinRandom.o obj/bitcoinKeyPair.o obj/electrumMnemonic.o obj/keyDerivationFunction.o obj/language.o obj/mnemonic.o obj/mnemonicBase.o obj/nonblockFile.o obj/postscriptGeneration.o obj/printPS.o obj/qrenc.o obj/random.o obj/randomUtils.o obj/simpleFile.o obj/txtGeneration.o obj/url.o obj/util.o obj/ECPoint.o obj/base58.o obj/bitcoinEC.o obj/hash.o obj/multiSig.o obj/pkif.o obj/privKey.o obj/pubAddress.o obj/pubKey.o obj/ripemd160.o obj/sha2.o obj/testutils.o obj/timelock.o obj/wif.o obj/bigintutil.o obj/bitstream.o obj/mask.o obj/mmask.o obj/mqrspec.o obj/qrencode.o obj/qrinput.o obj/qrspec.o obj/rscode.o obj/split.o obj/currencySpecific_bitcoin_specific.o 

obj/EncryptedResult.o: src/EncryptedResult.cpp
	$(COMPILE) -o obj/EncryptedResult.o src/EncryptedResult.cpp

obj/ExtendedKey.o: src/ExtendedKey.cpp
	$(COMPILE) -o obj/ExtendedKey.o src/ExtendedKey.cpp

obj/GenerationMethods.o: src/GenerationMethods.cpp
	$(COMPILE) -o obj/GenerationMethods.o src/GenerationMethods.cpp

obj/GenerationParameters.o: src/GenerationParameters.cpp
	$(COMPILE) -o obj/GenerationParameters.o src/GenerationParameters.cpp

obj/HierarchialRandom.o: src/HierarchialRandom.cpp
	$(COMPILE) -o obj/HierarchialRandom.o src/HierarchialRandom.cpp

obj/RSVal.o: src/RSVal.cpp
	$(COMPILE) -o obj/RSVal.o src/RSVal.cpp

obj/RangeSearcher.o: src/RangeSearcher.cpp
	$(COMPILE) -o obj/RangeSearcher.o src/RangeSearcher.cpp

obj/SaveFolderHandler.o: src/SaveFolderHandler.cpp
	$(COMPILE) -o obj/SaveFolderHandler.o src/SaveFolderHandler.cpp

obj/Script.o: src/Script.cpp
	$(COMPILE) -o obj/Script.o src/Script.cpp

obj/ScriptItem.o: src/ScriptItem.cpp
	$(COMPILE) -o obj/ScriptItem.o src/ScriptItem.cpp

obj/VanitySearcher.o: src/VanitySearcher.cpp
	$(COMPILE) -o obj/VanitySearcher.o src/VanitySearcher.cpp

obj/addressConversion.o: src/addressConversion.cpp
	$(COMPILE) -o obj/addressConversion.o src/addressConversion.cpp

obj/aes.o: src/aes.cpp
	$(COMPILE) -o obj/aes.o src/aes.cpp

obj/amount.o: src/amount.cpp
	$(COMPILE) -o obj/amount.o src/amount.cpp

obj/base64.o: src/base64.cpp
	$(COMPILE) -o obj/base64.o src/base64.cpp

obj/binaryTransaction.o: src/binaryTransaction.cpp
	$(COMPILE) -o obj/binaryTransaction.o src/binaryTransaction.cpp

obj/bitcalc.o: src/bitcalc.cpp
	$(COMPILE) -o obj/bitcalc.o src/bitcalc.cpp

obj/bitcalccli.o: src/bitcalccli.cpp
	$(COMPILE) -o obj/bitcalccli.o src/bitcalccli.cpp

obj/bitcry.o: src/bitcry.cpp
	$(COMPILE) -o obj/bitcry.o src/bitcry.cpp

obj/bitgenConfig.o: src/bitgenConfig.cpp
	$(COMPILE) -o obj/bitgenConfig.o src/bitgenConfig.cpp

obj/bitgen_bctest.o: src/bitgen_bctest.cpp
	$(COMPILE) -o obj/bitgen_bctest.o src/bitgen_bctest.cpp

obj/bitgen_bitcoin.o: src/bitgen_bitcoin.cpp
	$(COMPILE) -o obj/bitgen_bitcoin.o src/bitgen_bitcoin.cpp

obj/bitgen_bitcoincash.o: src/bitgen_bitcoincash.cpp
	$(COMPILE) -o obj/bitgen_bitcoincash.o src/bitgen_bitcoincash.cpp

obj/bitgen_dogecoin.o: src/bitgen_dogecoin.cpp
	$(COMPILE) -o obj/bitgen_dogecoin.o src/bitgen_dogecoin.cpp

obj/bitgen_feathercoin.o: src/bitgen_feathercoin.cpp
	$(COMPILE) -o obj/bitgen_feathercoin.o src/bitgen_feathercoin.cpp

obj/bitgen_litecoin.o: src/bitgen_litecoin.cpp
	$(COMPILE) -o obj/bitgen_litecoin.o src/bitgen_litecoin.cpp

obj/bitgen_namecoin.o: src/bitgen_namecoin.cpp
	$(COMPILE) -o obj/bitgen_namecoin.o src/bitgen_namecoin.cpp

obj/bitgen_peercoin.o: src/bitgen_peercoin.cpp
	$(COMPILE) -o obj/bitgen_peercoin.o src/bitgen_peercoin.cpp

obj/bitgen_vertcoin.o: src/bitgen_vertcoin.cpp
	$(COMPILE) -o obj/bitgen_vertcoin.o src/bitgen_vertcoin.cpp

obj/bitgencli.o: src/bitgencli.cpp
	$(COMPILE) -o obj/bitgencli.o src/bitgencli.cpp

obj/bitsig.o: src/bitsig.cpp
	$(COMPILE) -o obj/bitsig.o src/bitsig.cpp

obj/bitsigConfig.o: src/bitsigConfig.cpp
	$(COMPILE) -o obj/bitsigConfig.o src/bitsigConfig.cpp

obj/bitsigFileFormat.o: src/bitsigFileFormat.cpp
	$(COMPILE) -o obj/bitsigFileFormat.o src/bitsigFileFormat.cpp

obj/commandLine.o: src/commandLine.cpp
	$(COMPILE) -o obj/commandLine.o src/commandLine.cpp

obj/encrypt.o: src/encrypt.cpp
	$(COMPILE) -o obj/encrypt.o src/encrypt.cpp

obj/encryptionFileFormats.o: src/encryptionFileFormats.cpp
	$(COMPILE) -o obj/encryptionFileFormats.o src/encryptionFileFormats.cpp

obj/hmac.o: src/hmac.cpp
	$(COMPILE) -o obj/hmac.o src/hmac.cpp

obj/keychainFunc.o: src/keychainFunc.cpp
	$(COMPILE) -o obj/keychainFunc.o src/keychainFunc.cpp

obj/license.o: src/license.cpp
	$(COMPILE) -o obj/license.o src/license.cpp

obj/miniPriv.o: src/miniPriv.cpp
	$(COMPILE) -o obj/miniPriv.o src/miniPriv.cpp

obj/predicates.o: src/predicates.cpp
	$(COMPILE) -o obj/predicates.o src/predicates.cpp

obj/scriptdecoder.o: src/scriptdecoder.cpp
	$(COMPILE) -o obj/scriptdecoder.o src/scriptdecoder.cpp

obj/sigFile.o: src/sigFile.cpp
	$(COMPILE) -o obj/sigFile.o src/sigFile.cpp

obj/signature.o: src/signature.cpp
	$(COMPILE) -o obj/signature.o src/signature.cpp

obj/signatureFileFormats.o: src/signatureFileFormats.cpp
	$(COMPILE) -o obj/signatureFileFormats.o src/signatureFileFormats.cpp

obj/transaction.o: src/transaction.cpp
	$(COMPILE) -o obj/transaction.o src/transaction.cpp

obj/transactionSignatureChecker.o: src/transactionSignatureChecker.cpp
	$(COMPILE) -o obj/transactionSignatureChecker.o src/transactionSignatureChecker.cpp

obj/transactiondecoder.o: src/transactiondecoder.cpp
	$(COMPILE) -o obj/transactiondecoder.o src/transactiondecoder.cpp

obj/unittest.o: src/unittest.cpp
	$(COMPILE) -o obj/unittest.o src/unittest.cpp

obj/usagePrintout.o: src/usagePrintout.cpp
	$(COMPILE) -o obj/usagePrintout.o src/usagePrintout.cpp

obj/argon2d-ref.o: src/argon/argon2d-ref.cpp
	$(COMPILE) -o obj/argon2d-ref.o src/argon/argon2d-ref.cpp

obj/blake2b-ref.o: src/argon/blake2b-ref.cpp
	$(COMPILE) -o obj/blake2b-ref.o src/argon/blake2b-ref.cpp

obj/Directory.o: src/bitgen/Directory.cpp
	$(COMPILE) -o obj/Directory.o src/bitgen/Directory.cpp

obj/FileUtil.o: src/bitgen/FileUtil.cpp
	$(COMPILE) -o obj/FileUtil.o src/bitgen/FileUtil.cpp

obj/WinRandom.o: src/bitgen/WinRandom.cpp
	$(COMPILE) -o obj/WinRandom.o src/bitgen/WinRandom.cpp

obj/bitcoinKeyPair.o: src/bitgen/bitcoinKeyPair.cpp
	$(COMPILE) -o obj/bitcoinKeyPair.o src/bitgen/bitcoinKeyPair.cpp

obj/electrumMnemonic.o: src/bitgen/electrumMnemonic.cpp
	$(COMPILE) -o obj/electrumMnemonic.o src/bitgen/electrumMnemonic.cpp

obj/keyDerivationFunction.o: src/bitgen/keyDerivationFunction.cpp
	$(COMPILE) -o obj/keyDerivationFunction.o src/bitgen/keyDerivationFunction.cpp

obj/language.o: src/bitgen/language.cpp
	$(COMPILE) -o obj/language.o src/bitgen/language.cpp

obj/mnemonic.o: src/bitgen/mnemonic.cpp
	$(COMPILE) -o obj/mnemonic.o src/bitgen/mnemonic.cpp

obj/mnemonicBase.o: src/bitgen/mnemonicBase.cpp
	$(COMPILE) -o obj/mnemonicBase.o src/bitgen/mnemonicBase.cpp

obj/nonblockFile.o: src/bitgen/nonblockFile.cpp
	$(COMPILE) -o obj/nonblockFile.o src/bitgen/nonblockFile.cpp

obj/postscriptGeneration.o: src/bitgen/postscriptGeneration.cpp
	$(COMPILE) -o obj/postscriptGeneration.o src/bitgen/postscriptGeneration.cpp

obj/printPS.o: src/bitgen/printPS.cpp
	$(COMPILE) -o obj/printPS.o src/bitgen/printPS.cpp

obj/qrenc.o: src/bitgen/qrenc.cpp
	$(COMPILE) -o obj/qrenc.o src/bitgen/qrenc.cpp

obj/random.o: src/bitgen/random.cpp
	$(COMPILE) -o obj/random.o src/bitgen/random.cpp

obj/randomUtils.o: src/bitgen/randomUtils.cpp
	$(COMPILE) -o obj/randomUtils.o src/bitgen/randomUtils.cpp

obj/simpleFile.o: src/bitgen/simpleFile.cpp
	$(COMPILE) -o obj/simpleFile.o src/bitgen/simpleFile.cpp

obj/txtGeneration.o: src/bitgen/txtGeneration.cpp
	$(COMPILE) -o obj/txtGeneration.o src/bitgen/txtGeneration.cpp

obj/url.o: src/bitgen/url.cpp
	$(COMPILE) -o obj/url.o src/bitgen/url.cpp

obj/util.o: src/bitgen/util.cpp
	$(COMPILE) -o obj/util.o src/bitgen/util.cpp

obj/ECPoint.o: src/bitlib/ECPoint.cpp
	$(COMPILE) -o obj/ECPoint.o src/bitlib/ECPoint.cpp

obj/base58.o: src/bitlib/base58.cpp
	$(COMPILE) -o obj/base58.o src/bitlib/base58.cpp

obj/bitcoinEC.o: src/bitlib/bitcoinEC.cpp
	$(COMPILE) -o obj/bitcoinEC.o src/bitlib/bitcoinEC.cpp

obj/currencySpecific_bctest_specific.o: src/bitlib/currencySpecific_bctest_specific.cpp
	$(COMPILE) -o obj/currencySpecific_bctest_specific.o src/bitlib/currencySpecific_bctest_specific.cpp

obj/currencySpecific_bitcoin_specific.o: src/bitlib/currencySpecific_bitcoin_specific.cpp
	$(COMPILE) -o obj/currencySpecific_bitcoin_specific.o src/bitlib/currencySpecific_bitcoin_specific.cpp

obj/currencySpecific_bitcoincash_specific.o: src/bitlib/currencySpecific_bitcoincash_specific.cpp
	$(COMPILE) -o obj/currencySpecific_bitcoincash_specific.o src/bitlib/currencySpecific_bitcoincash_specific.cpp

obj/currencySpecific_dogecoin_specific.o: src/bitlib/currencySpecific_dogecoin_specific.cpp
	$(COMPILE) -o obj/currencySpecific_dogecoin_specific.o src/bitlib/currencySpecific_dogecoin_specific.cpp

obj/currencySpecific_feathercoin_specific.o: src/bitlib/currencySpecific_feathercoin_specific.cpp
	$(COMPILE) -o obj/currencySpecific_feathercoin_specific.o src/bitlib/currencySpecific_feathercoin_specific.cpp

obj/currencySpecific_litecoin_specific.o: src/bitlib/currencySpecific_litecoin_specific.cpp
	$(COMPILE) -o obj/currencySpecific_litecoin_specific.o src/bitlib/currencySpecific_litecoin_specific.cpp

obj/currencySpecific_namecoin_specific.o: src/bitlib/currencySpecific_namecoin_specific.cpp
	$(COMPILE) -o obj/currencySpecific_namecoin_specific.o src/bitlib/currencySpecific_namecoin_specific.cpp

obj/currencySpecific_peercoin_specific.o: src/bitlib/currencySpecific_peercoin_specific.cpp
	$(COMPILE) -o obj/currencySpecific_peercoin_specific.o src/bitlib/currencySpecific_peercoin_specific.cpp

obj/currencySpecific_vertcoin_specific.o: src/bitlib/currencySpecific_vertcoin_specific.cpp
	$(COMPILE) -o obj/currencySpecific_vertcoin_specific.o src/bitlib/currencySpecific_vertcoin_specific.cpp

obj/hash.o: src/bitlib/hash.cpp
	$(COMPILE) -o obj/hash.o src/bitlib/hash.cpp

obj/multiSig.o: src/bitlib/multiSig.cpp
	$(COMPILE) -o obj/multiSig.o src/bitlib/multiSig.cpp

obj/pkif.o: src/bitlib/pkif.cpp
	$(COMPILE) -o obj/pkif.o src/bitlib/pkif.cpp

obj/privKey.o: src/bitlib/privKey.cpp
	$(COMPILE) -o obj/privKey.o src/bitlib/privKey.cpp

obj/pubAddress.o: src/bitlib/pubAddress.cpp
	$(COMPILE) -o obj/pubAddress.o src/bitlib/pubAddress.cpp

obj/pubKey.o: src/bitlib/pubKey.cpp
	$(COMPILE) -o obj/pubKey.o src/bitlib/pubKey.cpp

obj/ripemd160.o: src/bitlib/ripemd160.cpp
	$(COMPILE) -o obj/ripemd160.o src/bitlib/ripemd160.cpp

obj/sha2.o: src/bitlib/sha2.cpp
	$(COMPILE) -o obj/sha2.o src/bitlib/sha2.cpp

obj/testutils.o: src/bitlib/testutils.cpp
	$(COMPILE) -o obj/testutils.o src/bitlib/testutils.cpp

obj/timelock.o: src/bitlib/timelock.cpp
	$(COMPILE) -o obj/timelock.o src/bitlib/timelock.cpp

obj/wif.o: src/bitlib/wif.cpp
	$(COMPILE) -o obj/wif.o src/bitlib/wif.cpp

obj/bigintutil.o: src/bitlib/bigintctl/bigintutil.cpp
	$(COMPILE) -o obj/bigintutil.o src/bitlib/bigintctl/bigintutil.cpp

obj/bitstream.o: src/qrenc/bitstream.c
	gcc -DHAVE_CONFIG_H -o obj/bitstream.o -c src/qrenc/bitstream.c

obj/mask.o: src/qrenc/mask.c
	gcc -DHAVE_CONFIG_H -o obj/mask.o -c src/qrenc/mask.c

obj/mmask.o: src/qrenc/mmask.c
	gcc -DHAVE_CONFIG_H -o obj/mmask.o -c src/qrenc/mmask.c

obj/mqrspec.o: src/qrenc/mqrspec.c
	gcc -DHAVE_CONFIG_H -o obj/mqrspec.o -c src/qrenc/mqrspec.c

obj/qrencode.o: src/qrenc/qrencode.c
	gcc -DHAVE_CONFIG_H -o obj/qrencode.o -c src/qrenc/qrencode.c

obj/qrinput.o: src/qrenc/qrinput.c
	gcc -DHAVE_CONFIG_H -o obj/qrinput.o -c src/qrenc/qrinput.c

obj/qrspec.o: src/qrenc/qrspec.c
	gcc -DHAVE_CONFIG_H -o obj/qrspec.o -c src/qrenc/qrspec.c

obj/rscode.o: src/qrenc/rscode.c
	gcc -DHAVE_CONFIG_H -o obj/rscode.o -c src/qrenc/rscode.c

obj/split.o: src/qrenc/split.c
	gcc -DHAVE_CONFIG_H -o obj/split.o -c src/qrenc/split.c



clean:
	rm -f *.o *.exe obj/*.o objwin/*.o bitgen bitsig bitcry bitcalc bitgen-btc bitgen-ltc bitgen-doge bitgen-testnet bitgen-nmc bitgen-ppc bitgen-vtc bitgen-rdd bitgen-ftc bitgen-bctest bitgen-bcc

