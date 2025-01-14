// SPDX-License-Identifier: MIT
pragma solidity 0.8.28;

struct Subject {
    string name;
    int256 votes;
}

struct Voter {
    bool negativeVoteLeft;
    address[] voted_subjects;
}

interface IVoteD21 {
    event VotingStarted();
    event VoterAdded(address indexed voter);
    event SubjectAdded(address indexed addr, string name);
    event PositiveVoted(address indexed voter, address indexed subject);
    event NegativeVoted(address indexed voter, address indexed subject);

    /**
     * @notice UC1. Add a new subject to the voting.
     *         UC7. Subjects can't be registered after the voting has started.
     * @dev Emits the SubjectAdded event. One EOA cannot deploy more than one subject.
     * @param name_ Subject name. Non-empty string.
     */
    function addSubject(string memory name_) external;

    /**
     * @notice UC2. List all registered subjects.
     * @return List of subject addresses.
     */
    function getSubjects() external view returns (address[] memory);

    /**
     * @notice UC3. Get subject details and results.
     * @param addr_ Subject address.
     * @return Subject details.
     */
    function getSubject(address addr_) external view returns (Subject memory);

    /**
     * @notice UC4. Add a new voter to the voting.
     * @dev Voters can only be added by the owner. Emits the VoterAdded event.
     * @param voter_ Voter address.
     */
    function addVoter(address voter_) external;

    /**
     * @notice UC5. Start the voting. Voting ends after 4 days from the voting start.
     * @dev Only the owner can start the voting. Emits the VotingStarted event.
     */
    function startVoting() external;

    /**
     * @notice UC8. Vote positive for a subject. A voter has three positive votes.
     *         UC9. Cannot vote for the same subject twice.
     *         New votes are not accepted after the voting ends.
     *         Votes are not accepted before the election starts.
     * @dev Emits the PositiveVoted event.
     * @param subject_ Subject address.
     */
    function votePositive(address subject_) external;

    /**
     * @notice UC8. Vote positive for a subject. A voter has one negative vote.
     *         UC9. Cannot vote for the same subject twice.
     *         UC10. Negative vote can be used only after 2 positive votes.
     *         New votes are not accepted after the voting ends.
     *         Votes are not accepted before the election starts.
     * @dev Emits the NegativeVoted event.
     * @param subject_ Subject address.
     */
    function voteNegative(address subject_) external;

    /**
     * @notice UC11. Voters should be able to vote in one transaction.
     * @param subjects_ List of subject addresses for voting.
     * @param votes_ List of votes. True for a positive vote, false for a negative vote.
     * @dev Votes are processed in the order of the subjects list.
     *      The lists must have the same length.
     *      The lenght may not exceed the number of allowed votes left for the voter.
     *      Every vote emits the PositiveVoted or NegativeVoted event.
     *      In case of a failure, the function reverts.
     */
    function voteBatch(
        address[] calldata subjects_,
        bool[] calldata votes_
    ) external;

    /**
     * @notice Get the remaining time for the voting to end.
     * @dev If the voting is not started, return 0.
     * @return Remaining time in seconds.
     */
    function getRemainingTime() external view returns (uint256);

    /**
     * @notice Get the voting results.
     * @dev Results are sorted by the number of votes in descending order.
     * @return List of subjects with their votes.
     */
    function getResults() external view returns (Subject[] memory);
}
