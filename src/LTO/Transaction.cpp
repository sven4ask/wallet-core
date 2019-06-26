// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "BinaryCoding.h"
#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::LTO;

Data Transaction::serializeToSign() const {
    auto data = Data();
    if (attachment.size() > 140) {
        throw std::invalid_argument("Maximum attachment size is 140 bytes");
    }
    if (pub_key.empty()) {
        throw std::invalid_argument("Public key can't be empty");
    }
    data.resize(2);
    data[0] = static_cast<byte>(TransactionType::transfer);
    data[1] = static_cast<byte>(TransactionVersion::V2);
    append(data, pub_key);

    encode64BE(timestamp, data);
    encode64BE(amount, data);
    encode64BE(fee, data);
    append(data, Data(std::begin(to.bytes), std::end(to.bytes)));
    encodeDynamicLengthBytes(attachment, data);
    return data;
}