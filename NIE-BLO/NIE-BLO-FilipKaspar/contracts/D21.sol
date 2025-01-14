// SPDX-License-Identifier: MIT
pragma solidity 0.8.28;

import "./IVoteD21.sol";

contract D21 is IVoteD21 {
    // Custom storage stuff

    mapping(address => Subject) public _subjects; // Make everything private at the end
    address[] private subject_addrs;
    mapping(address => uint256) subject_addrs_index;

    mapping(address => Voter) private _voters;
    address[] private voters_addrs;

    address private owner;
    bool public bool_voting_started = false;
    uint256 public voting_deadline;

    Subject[] private r;

    uint8 constant ALLOWED_VOTES = 4;

    // Custom errors

    error EmptyStringNotAllowedForSubjectName();
    error SubjectAlreadyExistsForAddress(string set_name);
    error VoterAlreadyRegistered(address set_address);
    error VotingAlreadyStarted(); // change to time since start
    error VotingHasntStarted();
    error NotPerformedByOwner();
    error NoVotersRegistered();
    error NoSubjectsRegistered();
    error EmptySubjectsNotAllowed();
    error SubjectsVotesNotSameLength(uint256 subjects_length, uint256 votes_length);
    error SentMoreThanRemainingVotes(uint8 remaining_votes);
    error VoterVotedThisSubjectAlready(address subject_addr);
    error NoMoreVotesAvailable();
    error VotingEnded(uint256 ended);
    error VotingHasntEndedYet(uint256 voting_end_time);
    error NegativeVoteHasBeenAlreadyUsed();
    error NotEnoughPositiveVotesUsed(uint8 total_votes_used);
    error NoMorePositiveVotes();

    // Functions

    function switchAddrs(uint256 position_i, address subject_) private {
        uint256 subject_index = subject_addrs_index[subject_];
        subject_addrs_index[subject_] = position_i;
        subject_addrs_index[subject_addrs[position_i]] = subject_index;

        address subject_tmp_addr = subject_addrs[subject_index];
        subject_addrs[subject_index] = subject_addrs[position_i];
        subject_addrs[position_i] = subject_tmp_addr;
    }

    function recalculateVotes(address subject_, bool vote) private {
        uint256 subject_index = subject_addrs_index[subject_];
        uint256 i;
        if(vote) { // Positive vote
            while(subject_index > 0){
                i = subject_index - 1;
                if(_subjects[subject_addrs[i]].votes < _subjects[subject_].votes){
                    switchAddrs(i, subject_);
                }
                subject_index = i;
            }
        } else {
            while(subject_index < subject_addrs.length - 1){
                i = subject_index + 1;
                if(_subjects[subject_addrs[i]].votes > _subjects[subject_].votes){
                    switchAddrs(i, subject_);
                }
                subject_index = i;
            }
        }

        delete r;
        for(uint256 k = 0; k < subject_addrs.length; k++){
            r.push(_subjects[subject_addrs[k]]);
        }
    }

    function privateVotePositive(address subject_) votingStarted(true) votingEnded checkAmountVotesLeft checkVotedSubjects(subject_) private{
        uint8 maxVotes = _voters[msg.sender].negativeVoteLeft ? 3 : 4;
        require(_voters[msg.sender].voted_subjects.length < maxVotes, NoMorePositiveVotes());
        _voters[msg.sender].voted_subjects.push(subject_);

        _subjects[subject_].votes++; // if subject hasn't been registered, we don't care, some random address votes gonna increment
        recalculateVotes(subject_, true);
        emit PositiveVoted(msg.sender, subject_);
    }

    function privateVoteNegative(address subject_) votingStarted(true) votingEnded checkAmountVotesLeft checkVotedSubjects(subject_) private{
        require(_voters[msg.sender].negativeVoteLeft, NegativeVoteHasBeenAlreadyUsed());
        require(_voters[msg.sender].voted_subjects.length >= 2, NotEnoughPositiveVotesUsed(uint8(_voters[msg.sender].voted_subjects.length)));

        _voters[msg.sender].negativeVoteLeft = false;
        _voters[msg.sender].voted_subjects.push(subject_);

        _subjects[subject_].votes--;
        recalculateVotes(subject_, false);
        emit NegativeVoted(msg.sender, subject_);
    }

    constructor() {
        owner = msg.sender;
    }

    modifier onlyOwner() {
        require(msg.sender == owner, NotPerformedByOwner());
        _;
    }

    modifier votingStarted(bool started) {
        if(started){
            require(bool_voting_started == true, VotingHasntStarted());    
        } else {
            require(bool_voting_started == false, VotingAlreadyStarted());
        }
        _;
    }

    modifier votingEnded(){
        require(bool_voting_started && block.timestamp < voting_deadline, VotingEnded(voting_deadline));
        _;
    }

    modifier checkAmountVotesLeft(){
        require(ALLOWED_VOTES - uint8(_voters[msg.sender].voted_subjects.length) > 0, NoMoreVotesAvailable());
        _;
    }

    modifier checkVotedSubjects(address subject_){
        for(uint8 i = 0; i < _voters[msg.sender].voted_subjects.length; i++){
            if(_voters[msg.sender].voted_subjects[i] == subject_) revert VoterVotedThisSubjectAlready(subject_);
        }
        _;
    }

    /// @inheritdoc IVoteD21
    function addSubject(string memory name_) votingStarted(false) external {
        require(bytes(name_).length > 0, EmptyStringNotAllowedForSubjectName());
        require(bytes(_subjects[msg.sender].name).length == 0, SubjectAlreadyExistsForAddress(_subjects[msg.sender].name));

        Subject memory new_subject = Subject(name_, 0);
        _subjects[msg.sender] = new_subject;
        subject_addrs_index[msg.sender] = subject_addrs.length;
        subject_addrs.push(msg.sender);
        r.push(new_subject);

        emit SubjectAdded(msg.sender, name_);
    }

    /// @inheritdoc IVoteD21
    function getSubjects() external view returns (address[] memory) {
        address[] memory subjects = new address[](subject_addrs.length);

        for(uint256 i = 0; i < subject_addrs.length; i++){
            subjects[i] = subject_addrs[i];
        }

        return subjects;
    }

    /// @inheritdoc IVoteD21
    function getSubject(address addr_) external view returns (Subject memory) {
        return _subjects[addr_];
    }

    function getVoters() external view returns (address[] memory) {
        address[] memory voters = new address[](voters_addrs.length);

        for(uint256 i = 0; i < voters_addrs.length; i++){
            voters[i] = voters_addrs[i];
        }

        return voters;
    }

    /// @inheritdoc IVoteD21
    function addVoter(address voter_) onlyOwner votingStarted(false) external{
        require(!_voters[voter_].negativeVoteLeft, VoterAlreadyRegistered(voter_));

        _voters[voter_] = Voter(true, new address[](0));
        voters_addrs.push(msg.sender);
        emit VoterAdded(voter_);
    }

    /// @inheritdoc IVoteD21
    function startVoting() onlyOwner votingStarted(false) external {
        require(voters_addrs.length != 0, NoVotersRegistered());
        require(subject_addrs.length != 0, NoSubjectsRegistered());

        voting_deadline = block.timestamp + 4 days;
        bool_voting_started = true;
        emit VotingStarted();
    }

    /// @inheritdoc IVoteD21
    function votePositive(address subject_) external {
        privateVotePositive(subject_);
    }

    /// @inheritdoc IVoteD21
    function voteNegative(address subject_) external {
        privateVoteNegative(subject_);
    }

    /// @inheritdoc IVoteD21
    function voteBatch(
        address[] calldata subjects_,
        bool[] calldata votes_
    ) external {
        require(subjects_.length > 0, EmptySubjectsNotAllowed());
        uint8 remaining_votes = ALLOWED_VOTES - uint8(_voters[msg.sender].voted_subjects.length);
        require(votes_.length <= remaining_votes, SentMoreThanRemainingVotes(remaining_votes));
        require(subjects_.length == votes_.length, SubjectsVotesNotSameLength(subjects_.length, votes_.length));

        for(uint8 i = 0; i < votes_.length; i++) {
            if(votes_[i]){ // Positive vote
                privateVotePositive(subjects_[i]);
            } else {
                privateVoteNegative(subjects_[i]);
            }
        }
    }

    /// @inheritdoc IVoteD21
    function getRemainingTime() external view returns (uint256) {
        if(bool_voting_started == false) return 0;
        require(block.timestamp < voting_deadline, VotingEnded(voting_deadline));
        
        return voting_deadline - block.timestamp;
    }

    /// @inheritdoc IVoteD21
    function getResults() votingStarted(true) external view returns (Subject[] memory) {
        require(block.timestamp > voting_deadline, VotingHasntEndedYet(voting_deadline));
        return r;
    }
}
