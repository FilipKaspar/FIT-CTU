// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

import "./Beacons.sol";
import './Towers.sol';
import './PowerGrid.sol';
import './Sentinels.sol';

/**
 * @title MegaCorp Central Control
 * @notice The nerve center of Neo Tokyo's surveillance state. All systems
 * converge here: Beacons, Towers, Power Grid, and Sentinels.
 *
 * "The perfect system cannot be broken... but it must be activated first."
 *  - Anonymous MegaCorp Engineer, 2084
 *
 * Your ultimate goal: Turn MegaCorp's own systems against itself and
 * trigger a cascade failure that will free Neo Tokyo.
 */

contract Megacorp is Ownable, ISubtitlable {
    /* Beacons */
    address[] private _beacons;
    mapping(address => bool) private _litBeacons;
    uint8 _litBeaconsCount = 0;

    /* Beacon Tower */
    BeaconTower public immutable beaconTower;
    BeaconTowerActivator public immutable beaconTowerActivator;

    /* Proxy Tower */
    ProxyTower public immutable proxyTower;

    /* The Gate */
    PowerGrid public immutable powerGrid;

    /* Sentinels */
    Sentinels public sentinels;

    /* Megacorp state */
    enum SurveillanceStatus {
        DEACTIVATED,
        ACTIVATED,
        SYSTEM_FAILURE
    }
    SurveillanceStatus public surveillanceStatus;

    /* Task execution */
    mapping(bytes4 => bool) public executedTasks;

    /*
    * Modifiers
    */

    // modifier onlyOnce() {
    //     require(!executedTasks[msg.sig], "Task already executed");
    //     executedTasks[msg.sig] = true;
    //     _;
    // }

    modifier onlyWhenBeaconsLit() {
        require(_litBeaconsCount >= 3, "Beacons are not lit");
        _;
    }

    modifier onSystemFailure() {
        require(surveillanceStatus == SurveillanceStatus.SYSTEM_FAILURE, "System failure");
        _;
    }

    modifier noSystemFailure() {
        require(surveillanceStatus != SurveillanceStatus.SYSTEM_FAILURE, "System failure");
        _;
    }

    modifier onlyOnce() {
        require(!executedTasks[msg.sig], "Task already executed");
        executedTasks[msg.sig] = true;
        _;
    }

    /*
    * Constructor
    */

    constructor() {
        // mint some for myself
        _mint(address(this), 2222);

        // setup beacons
        setupBeacon(address(new Beacon1()));
        setupBeacon(address(new Beacon2()));
        setupBeacon(address(new Beacon3()));

        // setup towers
        beaconTower = new BeaconTower();
        beaconTowerActivator = new BeaconTowerActivator();
        proxyTower = new ProxyTower();

        // initialize surveillance status
        surveillanceStatus = SurveillanceStatus.DEACTIVATED;

        // setup power grid
        powerGrid = new PowerGrid();
        _mint(address(powerGrid), 1000);

        // setup sentinels
        sentinels = new Sentinels();
        sentinels.addSentinel(address(this));
        _mint(address(sentinels), 3000);

        // remove owner
        owner = address(this);
    }

    /*
    * Tasks
    */

    function lightBeacon(uint256 index, uint256 pin) external {
        IBeacon beacon = getBeacon(index);
        require(_litBeacons[address(beacon)] == false, "Beacon already lit");
        bool success = beacon.lightBeacon(pin);
        if (!success) {
            revert("Beacon not lit");
        }
        _litBeacons[address(beacon)] = true;
        _litBeaconsCount++;
        uint256 reward = beacon.reward();
        require(reward <= 1000, "Beacons are not that valuable!");
        _giveTokens(reward, msg.sender, "Beacon lit, take some reward!");
    }

    function activateBeaconTower(string memory activationCode) external onlyOnce onlyWhenBeaconsLit {
        (, bytes memory data) = address(beaconTowerActivator).call(abi.encodeWithSignature("checkTowerActivationProcedure(uint8, string)", _litBeaconsCount, activationCode));
        beaconTower.activate();
        _giveTokens(2000, msg.sender, "Beacon tower activated! 2000 tokens for you!");
    }

    function activateProxyTower() onlyOnce external {
        proxyTower.activate();
        _giveTokens(4000, msg.sender, "Proxy tower activated! 4000 tokens for you!");
    }

    function energizeGrid() external {
        powerGrid.energize();
        _giveTokens(powerGrid.storedEnergy(), msg.sender, "Energized and converted some energy to tokens!");
    }

    function enableSurveilance() noSystemFailure external {
        (bool success, bytes memory data) = address(powerGrid).call(abi.encodeWithSignature("checkSystems()"));
        bool systemsWorking = success ? abi.decode(data, (bool)) : false;
        if (!success || (surveillanceStatus == SurveillanceStatus.ACTIVATED && !systemsWorking)) {
            surveillanceStatus = SurveillanceStatus.SYSTEM_FAILURE;
            _giveTokens(2500, msg.sender, "SYSTEM_FAILURE means you can't trust the system anymore! 2500 tokens for you!");
            return;
        }
        if (systemsWorking) {
            surveillanceStatus = SurveillanceStatus.ACTIVATED;
        }
    }

    function boom() onSystemFailure onlyOnce external {
        require(address(sentinels) == address(0), "Sentinels are still working");
        _giveTokens(10000, msg.sender, "Did you just destroy the system??? 10000 tokens for you!");
    }

    function shallowSurveil(uint256 sentinelId) external {
        sentinels.shallowSurveillance(sentinelId);
    }

    function fullSurveil() onlyOnce external {
        require(surveillanceStatus == SurveillanceStatus.ACTIVATED, "Surveillance not activated");

        (bool success, bytes memory data) = address(sentinels).call{gas: 1000000}(abi.encodeWithSignature("fullSurveillance()"));
        (uint256 sentinelId, uint256 failedCount) = abi.decode(data, (uint256, uint256));
        if (!success) {
            _giveTokens(3000, msg.sender, "You destroyed my sentinels! 3000 tokens for you!");
            sentinels = Sentinels(address(0));
        }
    }

    function tokensForOwner(address target) onlyOwner external {
        emit Subtitles("How did you get here?!");
        transfer(target, balanceOf(address(this)));
    }

    /*
    * Helper Functions
    */

    function _giveTokens(uint256 value, address to, string memory subtitles) private {
        emit Subtitles(subtitles);
        _mint(to, value);
    }

    function isBeaconTowerActivated() public returns (bool activated) {
        return beaconTower.activated();
    }

    function setupBeacon(address beacon) public {
        require(_beacons.length <= 3, "Too many beacons");
        _beacons.push(beacon);
    }

    function getBeacon(uint256 index) public view returns (IBeacon) {
        return IBeacon(_beacons[index]);
    }

    /*
    * ERC20 part
    */
    uint256 public totalSupply;
    mapping(address => uint256) public balances;
    mapping(address => mapping(address => uint256)) public allowed;

    function balanceOf(address _owner) public view returns (uint256 balance) {
        return balances[_owner];
    }

    function transfer(address _to, uint256 _value) public returns (bool success) {
        require(balances[msg.sender] >= _value, "Insufficient balance");
        balances[msg.sender] -= _value;
        balances[_to] += _value;
        return true;
    }

    function transferFrom(address _from, address _to, uint256 _value) public returns (bool success) {
        require(balances[_from] >= _value && allowed[_from][msg.sender] >= _value);
        balances[_to] += _value;
        balances[_from] -= _value;
        allowed[_from][msg.sender] -= _value;
        return true;
    }

    function approve(address _spender, uint256 _value) public returns (bool success) {
        allowed[msg.sender][_spender] = _value;
        return true;
    }

    function allowance(address _owner, address _spender) public view returns (uint256 remaining) {
        return allowed[_owner][_spender];
    }

    function _mint(address _to, uint256 _amount) private {
        totalSupply += _amount;
        balances[_to] += _amount;
    }

    function _burn(address _from, uint256 _amount) private {
        require(balances[_from] >= _amount);
        totalSupply -= _amount;
        balances[_from] -= _amount;
    }

    receive() external payable {
        revert("You can't bribe the megacorp!");
    }

    fallback() external {
        revert("Fall back stranger.");
    }

    /*
    * Multicall for multiple calls in one transaction
    */

    function multicall(bytes[] calldata data) external {
        for (uint256 i = 0; i < data.length; i++) {
            (bool success, ) = address(this).call(data[i]);
            if (!success) {
                revert("Multicall failed");
            }
        }
    }

    /*
    Evaluation part
    */

    function evaluate(address hackeer) external view returns (uint256) {
        return balances[hackeer];
    }
}
