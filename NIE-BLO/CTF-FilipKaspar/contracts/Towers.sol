// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

/**
 * @title The Twin Towers
 * @notice Rising above Neo Tokyo's skyline, the Twin Towers form the backbone
 * of MegaCorp's control system:
 *
 * - The Beacon Tower: An ancient structure that channels the power of the
 *   Network Beacons, requiring a precise activation sequence.
 *
 * - The Proxy Tower: A modern marvel of security architecture, using
 *   delegated calls and defensive shields to protect its core.
 *
 * Both must fall for the surveillance system to be compromised.
 */

contract BeaconTower {
    bool public activated;
    address public megacorp;

    event TowerActivated();

    constructor() {
        megacorp = msg.sender;
    }

    modifier onlyMegacorp() {
        require(msg.sender == megacorp, "Only megacorp can call this function");
        _;
    }

    function activate() public onlyMegacorp {
        activated = true;
        emit TowerActivated();
    }
}

contract BeaconTowerActivator {
    function uint2str(uint256 _i) internal pure returns (string memory) {
        if (_i == 0) return "0";

        uint256 temp = _i;
        uint256 digits;

        while (temp != 0) {
            digits++;
            temp /= 10;
        }

        bytes memory buffer = new bytes(digits);
        while (_i != 0) {
            digits -= 1;
            buffer[digits] = bytes1(uint8(48 + (_i % 10)));
            _i /= 10;
        }

        return string(buffer);
    }

    function checkTowerActivationProcedure(uint8 litBeaconsCount, string memory activationCode) public pure returns (string memory activationMessage) {
        string memory litBeaconsCountStr = uint2str(uint256(litBeaconsCount));
        string memory combined = string(abi.encodePacked(litBeaconsCountStr, activationCode));
        require(keccak256(abi.encodePacked(combined)) == keccak256(abi.encodePacked("55")), "Tower activation check 1 failed");
        require(keccak256(abi.encodePacked(litBeaconsCount)) == keccak256(abi.encodePacked(uint8(0))), "Tower activation check 2 failed");
        return "success";
    }
}

/*
* Proxy Tower
*/

contract ProxyTower {
    address public megacorp;
    address public implementation;
    bool public activated;

    modifier onlyMegacorp() {
        require(tx.origin == megacorp, "Only Megacorp can initiate this function");
        _;
    }

    constructor() {
        megacorp = msg.sender;
        implementation = address(new ProxyTowerController(megacorp));

    }

    fallback() external {
        address _impl = implementation;
        assembly {
            calldatacopy(0, 0, calldatasize())
            let result := delegatecall(gas(), _impl, 0, calldatasize(), 0, 0)
            returndatacopy(0, 0, returndatasize())
            switch result
            case 0 { revert(0, returndatasize()) }
            default { return(0, returndatasize()) }
        }
    }

    function activate() external onlyMegacorp {
        activated = true;
    }

    receive() external payable {}
}

contract ProxyTowerController {
    address public defender;
    address public megacorp;
    bool public activated;
    uint256 public shield;

    modifier onlyDefender() {
        require(msg.sender == defender, "Only defender can call this function");
        _;
    }

    modifier onlyWithActiveShield() {
        require(shield > 0, "Shield is not active");
        _;
    }

    constructor(address _megacorp) {
        megacorp = _megacorp;
    }

    function setDefender(address _defender) external {
        defender = _defender;
    }

    function setShield(uint256 _shield) external {
        shield = _shield;
    }

    function activate() external onlyWithActiveShield {
        activated = true;
    }
}