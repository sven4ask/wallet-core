// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "LTO/Address.h"

#include <Coin.h>
#include <HDWallet.h>
#include <gtest/gtest.h>
#include "map"

using namespace std;
using namespace TW;
using namespace TW::LTO;

TEST(LTOAddress, SecureHash) {
    const auto secureHash =
        hex(Address::secureHash(parse_hex("0157c7fefc0c6acc54e9e4354a81ac1f038e01745731")));

    ASSERT_EQ(secureHash, "0ad97158bc2ff70a7cce3593735d83628dd0168d6d371d85ee5369069f1a7f95");
}

TEST(LTOAddress, FromPrivateKey) {
    const auto privateKey =
        PrivateKey(parse_hex("09afd2f1cc195abaf7ff2eecc70610ea7f402e65e8dc242bb7232ba29169a17c"));
    const auto publicKeyEd25519 = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(hex(Data(publicKeyEd25519.bytes.begin(), publicKeyEd25519.bytes.end())),
              "7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d");
    const auto address = Address(publicKeyEd25519);

    ASSERT_EQ(address.string(), "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH");
}

TEST(LTOAddress, FromPrivateKey2) {
    const auto privateKey =
        PrivateKey(parse_hex("cda3e8209efe9a51af02f73b89b57b8cae7f5f2eb82d66e0f455d372d8041054"));
    const auto publicKeyEd25519 = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    const auto address = Address(publicKeyEd25519);

    ASSERT_EQ(address.string(), "3Jz5cQaYMXnEhv72WvuGp7RWXQo7RCKY71f");
}

TEST(LTOAddress, FromPublicKey) {
    const auto publicKey =
        PublicKey(parse_hex("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d"),
                  TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH");
}

TEST(LTOAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(std::string("abc")));
    ASSERT_FALSE(Address::isValid(std::string("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")));
    ASSERT_FALSE(Address::isValid(std::string("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuI")));
    ASSERT_FALSE(Address::isValid(std::string("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuJ")));
}

TEST(LTOAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH")));
}

TEST(LTOAddress, InitWithString) {
    const auto address = Address("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH");
    ASSERT_EQ(address.string(), "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH");
}

TEST(LTOAddress, Derive) {
    const auto mnemonic =
        "water process satisfy repeat flag avoid town badge sketch surge split between cabin sugar "
        "ill special axis adjust pull useful craft peace flee physical";
    const auto wallet = HDWallet(mnemonic, "");
    const auto address1 = TW::deriveAddress(
        TWCoinTypeLTO, wallet.getKey(DerivationPath("m/44'/353'/0'/0'/0'")));
    const auto address2 = TW::deriveAddress(
        TWCoinTypeLTO, wallet.getKey(DerivationPath("m/44'/353'/0'/0'/1'")));

    ASSERT_EQ(address1, "3JvLhJfCbXNEdZKkxBscerYkePdCbPAM9o1");
    ASSERT_EQ(address2, "3JtjeoLeWTLm7WB1j8BFTRTWWGf1iGPBpx7");
}