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

#include <gtest/gtest.h>

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