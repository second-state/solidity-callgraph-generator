pragma solidity >=0.4.16 <0.6.0;

contract C {
    function f() public pure {
        g([uint(1), 2, 3]);
    }

    function g(uint[3] memory) public pure {
    }
}
