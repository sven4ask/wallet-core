// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class LTOTests: XCTestCase {

    func testAddressFromPrivateKey() {
        let key = PrivateKey(data: Data(hexString: "09afd2f1cc195abaf7ff2eecc70610ea7f402e65e8dc242bb7232ba29169a17c")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = LTOAddress(publicKey: pubkey)

        XCTAssertEqual(pubkey.data.hexString, "7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d".lowercased())
        XCTAssertEqual(address.description, "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH")
    }

    func testAddressFromPublicKey() {
        let pubkey = PublicKey(data: Data(hexString: "7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d")!, type: .ed25519)!
        let address = LTOAddress(publicKey: pubkey)

        XCTAssertEqual(address.description, "3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH")
    }
}