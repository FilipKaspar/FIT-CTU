// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

import "./helpers.sol";

/**
 * @title Neo Tokyo Power Grid
 * @notice The heart of MegaCorp's energy distribution network. The Grid
 * is powered by experimental fusion technology, requiring precise fuel
 * cell configurations to maintain stability.
 */

abstract contract PowerLimiter {
    function _checkMaxPower(uint256 _storedEnergy) internal pure {
        require(_storedEnergy <= 1010, "This is starting to be unsafe");
    }
}

contract PowerGrid is Ownable, PowerLimiter, Tokenable, ISubtitlable {
    FusionBattery public fusionBattery;
    uint256 public storedEnergy;
    bool public energized;

    modifier onlyWhenEnergyDepleted() {
        require(!energized, "There is already energy stored");
        _;
    }

    modifier onlyWhenCellFueled() {
        require(FusionBattery(fusionBattery).isFueled(), "Not enough fuel in the cell");
        _;
    }

    constructor() Tokenable(msg.sender) {
        fusionBattery = new FusionBattery();
    }


    function setFusionBattery(address _fusionBattery) external onlyOwner {
        fusionBattery = FusionBattery(_fusionBattery);
    }

    function energize() external onlyOwner onlyWhenCellFueled onlyWhenEnergyDepleted {
        require(!energized, "Already energized");

        storedEnergy += fusionBattery.fuelAmount();
        energized = true;

        _checkMaxPower(storedEnergy);
        require(storedEnergy > 0, "Somehow no energy got stored");
    }

    function checkSystems() external view returns (bool systemsWorking) {
        return energized && FusionBattery(fusionBattery).isFueled();
    }
}

interface IFuelCell {
    function isEmpty() external returns (bool);
    function isType(string memory _type) external returns (bool);
    function hasEnoughEnergy(uint16 percentage) external returns (bool);
    function fuelAmount() external returns (uint16);
}

contract FusionBattery {
    IFuelCell public fuelCell;

    modifier onlyWhenFuelCell() {
        require(address(fuelCell) != address(0), "No fuel cell installed");
        _;
    }

    function addFuelCell(uint16 _fuelAmount, address _fuelCellAddress) external {
        _checkFuelAmount(_fuelAmount);

        IFuelCell _fuelCell = IFuelCell(msg.sender);
        if(_fuelCell.hasEnoughEnergy(_fuelAmount)) {
            fuelCell = IFuelCell(_fuelCellAddress);
            require(!_fuelCell.hasEnoughEnergy(_fuelAmount), "What?");
        }
    }

    function fuelAmount() public onlyWhenFuelCell returns (uint16) {
        uint16 _fuelAmount = fuelCell.fuelAmount();
        _checkFuelAmount(_fuelAmount);
        return _fuelAmount;
    }

    function removeFuelCell() external {
        revert("You cannot remove the fuel cell once it is installed");
    }

    function isFueled() external view returns (bool) {
        return address(fuelCell) != address(0);
    }

    function _checkFuelAmount(uint16 _fuelAmount) internal pure {
        require(_fuelAmount <= 100, "This is not possible");
        require(_fuelAmount >= 90, "The grid is powerhungry, we need moar fuel");
    }
}
