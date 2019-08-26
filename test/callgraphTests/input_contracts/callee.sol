contract Callee {
    function f1() public {
    }
    function f2() public {
        f1();
    }
}
