// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "HexCoding.h"
#include "LTO/Address.h"
#include "LTO/Transaction.h"
#include "proto/LTO.pb.h"
#include "uint256.h"
#include <iostream>

#include <TrustWalletCore/TWLTOAddress.h>
#include <TrustWalletCore/TWLTOSigner.h>
#include <PrivateKey.h>
#include <gtest/gtest.h>

using namespace TW;

TEST(TWLTOSigner, Sign) {
    auto input = LTO::Proto::SigningInput();

    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("AfD98A6sx2D1zQBwVmzjSvsdTkvfkmr6CW1YDvFh6ah5"));

    const auto publicKeyED25519 = privateKey.getPublicKey(TWPublicKeyTypeED25519);

    input.set_amount(int64_t(100000000));
    input.set_fee(int64_t(100000));
    input.set_to("3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH");
    input.set_attachment("hello");
    input.set_timestamp(int64_t(1559146613));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_public_key(publicKeyED25519.bytes.data(), publicKeyED25519.bytes.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWLTOSignerSign(inputTWData);
    auto output = LTO::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto transaction = LTO::Transaction(
        /* amount */ input.amount(),
        /* fee */ input.fee(),
        /* to */ LTO::Address(input.to()),
        /* attachment */ Data(input.attachment().begin(), input.attachment().end()),
        /* timestamp */ input.timestamp(),
        /* pub_key */ Data(input.public_key().begin(), input.public_key().end()));

    ASSERT_EQ(hex(transaction.serializeToSign()), "0402da29c40054a2f437f25b10dc52b7d7c4ccefe31e1302431094a6d8e6d409738f000000005ceeb0750000000005f5e10000000000000186a0014c2d5134ba47a58e0482ea5b40efc55c71b70947a0d5dbd7dc000568656c6c6f");
    ASSERT_EQ(hex(output.signature()), "26db0b9bd81faa85807b8e16647630c803180b9aedb194dad131bad2e28474936ae2bab6794e130036a0c0b7b17fe5664c8a0a8f98de4861b2b6c41fe76b5801");

    ASSERT_TRUE(publicKeyED25519.verify(
        Data(output.signature().begin(), output.signature().end()), transaction.serializeToSign()));
}
