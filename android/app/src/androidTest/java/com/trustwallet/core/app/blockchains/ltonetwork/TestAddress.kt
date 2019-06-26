package com.trustwallet.core.app.blockchains.ltonetwork

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.LTOAddress

class TestAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPrivateKey() {
        val key = PrivateKey("09afd2f1cc195abaf7ff2eecc70610ea7f402e65e8dc242bb7232ba29169a17c".toHexByteArray())
        val pubkey = key.getPublicKeyEd25519()
        val address = LTOAddress(pubkey)

        assertEquals(pubkey.data().toHex(), "0x7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d".toLowerCase())
        assertEquals(address.description(), "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH")
    }

    @Test
    fun testAddressFromPublicKey() {
        val pubkey = PublicKey("7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d".toHexByteArray(), PublicKeyType.ED25519)
        val address = LTOAddress(pubkey)

        assertEquals(address.description(), "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH")
    }
}