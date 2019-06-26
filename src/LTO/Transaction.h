// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"
#include "../proto/LTO.pb.h"

namespace TW::LTO {

enum class TransactionType { transfer = 4 };
enum class TransactionVersion { V1 = 1, V2 = 2 };

class Transaction {
    /// We only support Transfer V1 transaction.
    /// See
    /// https://docs.wavesplatform.com/en/blockchain/waves-protocol/data-structures.html#section-8555a9aaf83a8d01f18a2c38d19484fe
  public:

    int64_t amount;
    int64_t fee;
    Address to;
    Data attachment;
    int64_t timestamp;
    Data pub_key;

    Transaction(int64_t amount, int64_t fee,  Address to, Data attachment, int64_t timestamp, Data pub_key)
        : amount(std::move(amount))
        , fee(std::move(fee))
        , to(std::move(to))
        , attachment(std::move(attachment))
        , timestamp(std::move(timestamp))
        , pub_key(std::move(pub_key)) {
    }

  public:
    Data serializeToSign() const;
};

} // namespace TW::LTO
