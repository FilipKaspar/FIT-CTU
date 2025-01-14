// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

import "./Megacorp.sol";
import "./helpers.sol";

/**
 * @title The Sentinel Force
 * @notice MegaCorp's autonomous enforcement system. These AI-driven
 * units maintain order through constant surveillance of Neo Tokyo.
 *
 * The Sentinels are linked in a complex chain of command, each unit
 * monitoring the others. Their greatest strength - their interconnected
 * nature - may also be their greatest weakness.
 */

contract Sentinels is ISubtitlable {
    address private constant $ENTINEL = address(0x1);
    uint256 public sentinelCount;
    mapping(address => address) private sentinels;
    Megacorp public megacorp;

    modifier onlyMegacorp() {
        require(msg.sender == address(megacorp), "Only megacorp can call this function");
        _;
    }

    constructor() {
        megacorp = Megacorp(payable(msg.sender));
        sentinels[$ENTINEL] = $ENTINEL;
    }

    function addSentinel(address target) external{
        require(sentinelCount < 20, "Sentinel count limit reached");
        require(target != $ENTINEL, "Target cannot be sentinel");
        require(sentinels[$ENTINEL] != target, "Target already a sentinel");
        require(target != address(0), "Target cannot be zero address");
        sentinels[target] = sentinels[$ENTINEL];
        sentinels[$ENTINEL] = target;
        sentinelCount++;
    }

    function removeSentinel(address target) external {
        require(sentinelCount > 0, "No sentinels to remove");
        require(target != $ENTINEL, "Target cannot be sentinel");
        require(sentinels[target] != address(0), "Target is not a sentinel");
        address prev = $ENTINEL;
        address current = sentinels[$ENTINEL];

        while (current != $ENTINEL) {
            if (current == target) {
                sentinels[prev] = sentinels[current];
                sentinelCount--;
                return;
            }
            prev = current;
            current = sentinels[current];
        }
        revert("Sentinel not found");
    }

    function getSentinelAt(uint8 index) external view returns (address) {
        address current = sentinels[$ENTINEL];
        for (uint8 i = 0; i < index; i++) {
            if (current == $ENTINEL) {
                break;
            }
            current = sentinels[current];
        }
        if (current == $ENTINEL) {
            revert("Index out of bounds");
        }
        return current;
    }

    function getSentinel(address target) external view returns (address) {
        return sentinels[target];
    }

    function fullSurveillance() view external returns (uint256, uint256) {
        address current = sentinels[$ENTINEL];
        uint256 sentinelId = 0;
        uint256 failedCount = 0;

        while (current != $ENTINEL) {
            uint256 size;
            assembly {
                size := extcodesize(current)
            }
            if (size == 0) {
                failedCount++;
                if (failedCount > 20) {
                    revert("Too many failed sentinels");
                }
            }

            sentinelId++;

            current = sentinels[current];
        }

        return (sentinelId, failedCount);
    }

    function shallowSurveillance(uint256 sentinelId) external onlyMegacorp {
        uint256 i = 0;
        address current = sentinels[$ENTINEL];
        while (i < sentinelId && current != $ENTINEL) {
            i++;
            current = sentinels[current];
        }
        if (current == $ENTINEL) {
            revert("Sentinel not found");
        }
        (bool success,) = current.staticcall("");
        if (!success) {
            _fine(current);
        }
    }

    function _fine(address target) private {
        megacorp.transfer(target, 50);
    }
}


string constant ZERO_ADDRESS_STRING = "0x0000000000000000000000000000000000000000";
string constant ENTINEL_ADDRESS_STRING = "0x1";