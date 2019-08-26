contract A {
}

contract B {
    function f() public {
    }
}

contract C is A, B {
    function g() public {
        super.f();
    }
}
