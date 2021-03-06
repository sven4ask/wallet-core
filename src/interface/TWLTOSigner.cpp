// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWLTOSigner.h>

#include "../LTO/Signer.h"
#include "../proto/LTO.pb.h"

using namespace TW;
using namespace TW::LTO;

TW_LTO_Proto_SigningOutput TWLTOSignerSign(TW_LTO_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    auto transaction = Transaction(
        /* amount */ input.amount(),
        /* fee */ input.fee(),
        /* to */ Address(input.to()),
        /* attachment */ Data(input.attachment().begin(), input.attachment().end()),
        /* timestamp */ input.timestamp(),
        /* pub_key */ publicKey.bytes);

    Data signature = Signer::sign(privateKey, transaction);

    Proto::SigningOutput protoOutput = Proto::SigningOutput();
    protoOutput.set_signature(reinterpret_cast<const char *>(signature.data()), signature.size());
    protoOutput.set_json(transaction.buildJson(signature).dump());
    std::string serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
