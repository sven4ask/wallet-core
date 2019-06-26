// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../LTO/Address.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWLTOAddress.h>

#include <memory>

using namespace TW;
using namespace TW::LTO;

bool TWLTOAddressEqual(struct TWLTOAddress *_Nonnull lhs, struct TWLTOAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWLTOAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWLTOAddress *_Nullable TWLTOAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    try {
        const auto address = Address(*s);
        return new TWLTOAddress{std::move(address)};
    } catch (...) {
        return nullptr;
    }
}

struct TWLTOAddress *_Nonnull TWLTOAddressCreateWithPublicKey(
    struct TWPublicKey *_Nonnull publicKey) {
    return new TWLTOAddress{Address(publicKey->impl)};
}

void TWLTOAddressDelete(struct TWLTOAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWLTOAddressDescription(struct TWLTOAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWLTOAddressKeyHash(struct TWLTOAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
}
