// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "LTO/Address.h"
#include "LTO/Transaction.h"
#include <LTO/Signer.h>

#include <gtest/gtest.h>

using json = nlohmann::json;

using namespace std;
using namespace TW;
using namespace TW::LTO;

TEST(LTOTransaction, serialize) {
    auto tx1 = Transaction(
        /* amount */ 100000000,
        /* fee */ 100000000,
        /* to */ Address("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH"),
        /* attachment */ parse_hex("66616c6166656c"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "04027fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d00"
                                "00016372e852120000000005f5e1000000000005f5e100014c2d5134ba47a58e0482ea"
                                "5b40efc55c71b70947a0d5dbd7dc000766616c6166656c");

    auto tx2 = Transaction(
        /* amount */ 1,
        /* fee */ 1,
        /* to */ Address("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH"),
        /* attachment */ Data(),
        /* timestamp */ 1,
        /* pub_key */
        parse_hex("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d"));
    auto serialized2 = tx2.serializeToSign();
    ASSERT_EQ(hex(serialized2),
              "04027fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d000000000000000100000"
              "000000000010000000000000001014c2d5134ba47a58e0482ea5b40efc55c71b70947a0d5dbd7dc0000");

}

TEST(LTOTransaction, failedSerialize) {
    // 141 bytes attachment
    auto tx1 = Transaction(
        /* amount */ 100000000,
        /* fee */ 100000000,
        /* to */ Address("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH"),
        /* attachment */
        parse_hex("66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166"
                  "656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c"
                  "6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66"
                  "616c6166656c66616c6166656c66616c6166656c01"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d"));
    EXPECT_THROW(tx1.serializeToSign(), invalid_argument);
}

TEST(LTOTransaction, jsonSerialize) {
    const auto privateKey =
        PrivateKey(parse_hex("09afd2f1cc195abaf7ff2eecc70610ea7f402e65e8dc242bb7232ba29169a17c"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(hex(Data(publicKey.bytes.begin(), publicKey.bytes.end())),
              "7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d");
    
    const auto address = Address(publicKey);

    auto tx1 = Transaction(
        /* amount */ 10000000,
        /* fee */ 100000000,
        /* to */ address,
        /* attachment */ parse_hex("66616c6166656c"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d"));

    auto signature = Signer::sign(privateKey, tx1);

    auto json = tx1.buildJson(signature);

    ASSERT_EQ(json["type"], TransactionType::transfer);
    ASSERT_EQ(json["version"], TransactionVersion::V2);
    ASSERT_EQ(json["fee"], 100000000);
    ASSERT_EQ(json["senderPublicKey"], "9c4PB1eQMAz5PYcE7D5o3NQESCZaQ1ieMj2SLTfYxrnY");
    ASSERT_EQ(json["timestamp"], 1526641218066);
    ASSERT_EQ(json["proofs"].dump(), "[\"2ubHQ9b25mDZvyRmmurQz6q6LFyqBobYDQ5FqaEWy5iAaBXhqc7bvhM9McjtUknmBkvxtRujyzie97McTKGmbJ48\"]");
    ASSERT_EQ(json["recipient"], "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH");
    ASSERT_EQ(json["amount"], 10000000);
    ASSERT_EQ(json["attachment"], "4t2Xazb2SX");
    ASSERT_EQ(json.dump(), "{\"amount\":10000000,\"attachment\":\"4t2Xazb2SX\",\"fee\":100000000,\"proofs\":[\"2ubHQ9b25mDZvyRmmurQz6q6LFyqBobYDQ5FqaEWy5iAaBXhqc7bvhM9McjtUknmBkvxtRujyzie97McTKGmbJ48\"],\"recipient\":\"3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH\",\"senderPublicKey\":\"9c4PB1eQMAz5PYcE7D5o3NQESCZaQ1ieMj2SLTfYxrnY\",\"timestamp\":1526641218066,\"type\":4,\"version\":2}");
}