import { expect } from 'chai';
import 'mocha';

import { bufToHex, fromHexString } from '../../Utils';
import { PrivateKey, LTOAddress, PublicKey, PublicKeyType, HDWallet } from '../../../lib';

describe('LTOAddress', () => {

    it('test address from PrivateKey', () => {
        const key = PrivateKey.createWithData(fromHexString('09afd2f1cc195abaf7ff2eecc70610ea7f402e65e8dc242bb7232ba29169a17c'));
        const pubkey = key.getPublicKeyEd25519();
        const address = LTOAddress.createWithPublicKey(pubkey);

        expect(bufToHex(pubkey.data())).to.equal('0x7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d'.toLowerCase());
        expect(address.description()).to.equal('3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH');
    });

    it('test address from PublicKey', () => {
        const pubkey = PublicKey.createWithData(fromHexString('7fd8a5e9576d419cf4bc0f2dc07b67ac6d1cf8b8dcc2e81387bf705c78766c7d'), PublicKeyType.ED25519);
        const address = LTOAddress.createWithPublicKey(pubkey);

        expect(address.description()).to.equal('3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH');
    });
});
