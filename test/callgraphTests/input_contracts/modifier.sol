contract A {
    constructor(uint _x, uint _y) public {}
}

contract B is A(7, 4) {
    function f(A a) public {
    }
}

contract C {
    modifier m() {
        f1();
        _;
        // This is type conversion, so it should not be included in the callgraph.
        A(8);
    }

    B b = new B();
    // This does not call constructor of B, so it should not be in the callgraph either.
    B[] r = new B[](3);

    function f1() public {
        A a = new A(8, 9);
    }

    function f2() m public {
    }
}
