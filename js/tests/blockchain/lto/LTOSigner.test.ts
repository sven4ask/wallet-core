import { expect } from 'chai';
import 'mocha';

import * as Long from 'long';

import { fromHexString, bufToHex } from '../../Utils';
import { TW, PrivateKey, LTOSigner } from '../../../lib';

describe('LTOSigner', () => {

    it('test LTO transaction signing', () => {
        const signingInput = TW.LTO.Proto.SigningInput.create({
                amount: Long.fromString('100000000'),
                attachment: fromHexString('68656c6c6f206c746f'),
                fee: Long.fromString('100000'),
                privateKey: fromHexString('8f82fada11bb9b821263d7e0b98ce0e095d4865627f8eb6696c5eabc2c79bb00'),
                publicKey: fromHexString('da29c40054a2f437f25b10dc52b7d7c4ccefe31e1302431094a6d8e6d409738f'),
                timestamp: Long.fromString('1559146613'),
                to: '3JfLsayRvWbJhZMuP6bmG447DqtHgGWnkuH',
        });
        const sign: TW.LTO.Proto.SigningOutput = LTOSigner.sign(signingInput);
        const signBytes = bufToHex(sign.signature);

        expect(signBytes).to.equal('0x1da78a3509867789b845de0dcfe94eeb44245cf2fb535ede4f25aee874f85de5815811dda17bd313066c4431679a3439fe9b5b8e1ed2a9604e7baf4b6ead0705');
    });
});
