// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

/**
 * @title Helpers
 * @notice A collection of utility functions and modifiers for the game.
 *
 * todo i was tired, ask michal to verify this file
 */

contract Ownable {
    address public owner;
    constructor() {
        owner = msg.sender;
    }
    modifier onlyOwner() {
        require(msg.sender == owner, "Only the owner can call this function");
        _;
    }
}

contract Initializable {
    bool public initialized;
    modifier initializer() {
        require(!initialized, "Already initialized");
        initialized = true;
        _;
    }
}

contract ISubtitlable {
    event Subtitles(string subtitles);
}

abstract contract Tokenable {
    address public token;

    constructor(address _token) {
        token = _token;
    }

    function _balanceOf(address _account) internal view returns (uint256) {
        (bool success, bytes memory data) = token.staticcall(abi.encodeWithSelector(0x70a08231, _account));
        require(success, "Balance query failed");
        return abi.decode(data, (uint256));
    }

    function _approve(address _spender, uint256 _amount) internal {
        (bool success, ) = token.call(abi.encodeWithSelector(0x095ea7b3, _spender, _amount));
        require(success, "Approval failed");
    }

    function _transferFrom(address _from, address _to, uint256 _amount) internal {
        (bool success, ) = token.call(abi.encodeWithSelector(0x23b872dd, _from, _to, _amount));
        require(success, "Transfer failed");
    }

    function _transfer(address _to, uint256 _amount) public {
        (bool success, ) = token.call(abi.encodeWithSelector(0xa9059cbb, _to, _amount));
        require(success, "Transfer failed");
    }

    function _burn(uint256 _amount) internal {
        // send my balance to zero address of _amount
        (bool success, ) = token.call(abi.encodeWithSelector(0x70a08231, address(this), _amount));
        require(success, "Burn failed");
    }
}