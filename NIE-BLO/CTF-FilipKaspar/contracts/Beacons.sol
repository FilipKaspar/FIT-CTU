// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

import "./helpers.sol";

/**
 * @title Network Beacons
 * @notice The first line of defense in MegaCorp's surveillance network.
 * Three ancient beacons stand as silent guardians, each requiring a unique
 * cryptographic handshake to activate. Legend speaks of backdoors left by
 * the original architects - a fatal flaw in MegaCorp's perfect system.
 *
 * Your mission begins here: Light all three beacons to destabilize the
 * network's outer perimeter.
 */

interface IBeacon {
    function reward() external pure returns (uint256);
    function lightBeacon(uint256 pin) external returns (bool);
}

contract Beacon1 is IBeacon {
    uint256 public constant reward = 100;
    function lightBeacon(uint256 pin) external pure returns (bool) {
        unchecked {
            return pin == 1234;
        }
    }
}

contract Beacon2 is IBeacon {
    uint256 public constant reward = 100;
    uint16 a = 1234;
    uint16 b = 5678;

    function lightBeacon(uint256 pin) external view returns (bool) {
        unchecked {
            return uint256(uint16(a + b + pin) - 100) ^ 1 == 9;
        }
    }
}

contract Beacon3 is IBeacon, Initializable, Ownable {
    uint256 public constant reward = 100;
    uint256 public requestedPin = 0;

    function initialize(address _owner) external {
        owner = _owner;
    }

    function lightBeacon(uint256 pin) external view returns (bool) {
        unchecked {
            if (pin == 0) {
                return false;
        }
            return pin == requestedPin;
        }
    }

    function setRequestedPin(uint256 _requestedPin) external onlyOwner {
        requestedPin = _requestedPin;
    }
}