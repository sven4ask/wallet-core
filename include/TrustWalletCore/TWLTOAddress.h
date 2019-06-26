// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents an LTO address.
TW_EXPORT_CLASS
struct TWLTOAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWLTOAddressEqual(struct TWLTOAddress *_Nonnull lhs, struct TWLTOAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWLTOAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWLTOAddress *_Nullable TWLTOAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWLTOAddress *_Nonnull TWLTOAddressCreateWithPublicKey(
    struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWLTOAddressDelete(struct TWLTOAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWLTOAddressDescription(struct TWLTOAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWLTOAddressKeyHash(struct TWLTOAddress *_Nonnull address);

TW_EXTERN_C_END