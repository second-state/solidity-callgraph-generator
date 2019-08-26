pragma experimental ABIEncoderV2;

contract test {
	struct messageDetails{
		address receiver;
		address postman;
		uint timestamp;
	}

	struct nonceDetails{
		bytes encryptedNonce;
		bytes32 decryptor;
		uint timestamp;
	}

    function f1() public {
    }

	function setGoldenHash(address receiver, bytes32 hash, messageDetails memory mDetails, nonceDetails memory nDetails) public returns(bool success){
        f1();
    }
}
