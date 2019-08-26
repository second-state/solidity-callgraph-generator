import "./callee.sol";

contract Caller {
  function f(Callee c) public {
    c.f1();
  }
}
