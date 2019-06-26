// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PublicKey.h"
#include "LTO/Signer.h"
#include "LTO/Transaction.h"

#include <TrezorCrypto/sodium/keypair.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::LTO;

TEST(LTOSigner, SignTransaction) {
    const auto privateKey =
        PrivateKey(parse_hex("09afd2f1cc195abaf7ff2eecc70610ea7f402e65e8dc242bb7232ba29169a17c"));
    const auto publicKeyED25519 = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(hex(Data(publicKeyED25519.bytes.begin(), publicKeyED25519.bytes.end())),
              "7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d");
    // 3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH
    const auto address = Address(publicKeyED25519);

    auto tx1 = Transaction(
        /* amount */ 100000000,
        /* fee */ 100000000,
        /* to */ address,
        /* attachment */ parse_hex("66616c6166656c"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d"));

    auto signature = Signer::sign(privateKey, tx1);

    EXPECT_EQ(hex(tx1.serializeToSign()),
                                "04027fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d00"
                                "00016372e852120000000005f5e1000000000005f5e100014c2d5134ba47a58e0482ea"
                                "5b40efc55c71b70947a0d5dbd7dc000766616c6166656c");
    EXPECT_EQ(hex(signature), "96bc4fd81a613b1a8bfa2c0db9e68b5fe33f77a555a75ec5c76c98210b70f1bee53c140e5ef5eb273b6e33f37a61269fab96a0fbc59ed265b197d210bb19d503");

    ASSERT_TRUE(publicKeyED25519.verify(signature, tx1.serializeToSign()));
}