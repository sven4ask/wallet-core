package com.trustwallet.core.app.blockchains.ltonetwork

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.LTOSigner
import wallet.core.jni.proto.LTO

class TestLTOTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testLTOTransactionSigning() {
        val signingInput = LTO.SigningInput.newBuilder()
        signingInput.apply {
            amount = 100_000_000
            fee = 100_000
            to = "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH"
            attachment = ByteString.copyFrom("68656c6c6f206c746f".toHexByteArray())
            timestamp = 1559146613
            publicKey = ByteString.copyFrom("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d".toHexByteArray())
            privateKey = ByteString.copyFrom("09afd2f1cc195abaf7ff2eecc70610ea7f402e65e8dc242bb7232ba29169a17c".toHexByteArray())
        }

        val sign: LTO.SigningOutput = LTOSigner.sign(signingInput.build())
        val signBytes = sign.signature
        assertEquals(signBytes.toByteArray().toHex(), "0x7efc239dd1a09c6d6ac077cf82e3fe8621654b9a6be10bf91d5de8fa4612992a7fcec1abf287d134cf4c189403f9c41f881c7bbeb5c146b99fd28441ff1eab07")
    }
}