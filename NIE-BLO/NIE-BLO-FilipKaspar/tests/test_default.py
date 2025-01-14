"""
Wake docs: https://ackee.xyz/wake/docs/latest/
"""
from wake.testing import *

# Print failing tx call trace
def revert_handler(e: TransactionRevertedError):
    if e.tx is not None:
        print(e.tx.call_trace)
        print(e.tx.console_logs)


@default_chain.connect()
@on_revert(revert_handler)
def test_add_subject():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]

    # Try empty string
    try:
        Contract.addSubject("")
        assert False, "Should have reverted due to string being empty!"
    except TransactionRevertedError as e:
        assert e == Contract.EmptyStringNotAllowedForSubjectName()

    # Correct name and any EOA
    subject_name = "Test Subject 1"

    tx_result = Contract.addSubject(subject_name, from_=user1)
    
    # Verify event emitted
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.SubjectAdded(user1.address, subject_name)]

    # Try to add another subject
    try:
        Contract.addSubject("Try to make second subject", from_=user1)
        assert False, "Should have reverted due to attempting to add a second subject!"
    except TransactionRevertedError as e:
        assert e == Contract.SubjectAlreadyExistsForAddress(subject_name)

    # Test after voting started
    Contract.addVoter(owner.address)
    Contract.startVoting()
    
    assert Contract.bool_voting_started() == True
    
    try:
        Contract.addSubject(subject_name)
        assert False, "Should have reverted due to trying to add subject after voting started!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingAlreadyStarted()
    
    assert len(Contract.getSubjects()) == 1 # Only the user1

@default_chain.connect()
@on_revert(revert_handler)
def test_get_subjects():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    user1 = chain.accounts[1]

    # Check empty at the beginning
    assert len(Contract.getSubjects()) == 0

    # Add first subject
    subject_name1 = "Test Subject 1"
    Contract.addSubject(subject_name1)
    subjects = Contract.getSubjects()
    assert len(subjects) == 1
    assert Contract._subjects(subjects[0]).name == subject_name1

    # Add second subject
    subject_name2 = "Test Subject 2"
    Contract.addSubject(subject_name2, from_=user1)
    subjects = Contract.getSubjects()
    assert len(subjects) == 2
    assert Contract._subjects(subjects[0]).name == subject_name1
    assert Contract._subjects(subjects[1]).name == subject_name2


@default_chain.connect()
@on_revert(revert_handler)
def test_get_subject():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]

    # Should pass since everything is initialized
    subject = Contract.getSubject(owner)
    assert subject.name == ""

    # Now add/define owner
    subject_name = "Test Subject 1"
    Contract.addSubject(subject_name)
    subject = Contract.getSubject(owner)
    assert subject.name == subject_name


@default_chain.connect()
@on_revert(revert_handler)
def test_add_voter():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]
    user2 = chain.accounts[2]

    # Try to add a voter as non-owner
    try:
        Contract.addVoter(user1.address, from_=user1)
        assert False, "Should have reverted due to attempting to add voter as non-owner!"
    except TransactionRevertedError as e:
        assert e == Contract.NotPerformedByOwner()

    # Correct name
    tx_result = Contract.addVoter(owner.address)
    
    # Verify event emitted
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.VoterAdded(owner.address)]

    Contract.addVoter(user1.address)

    # Try to add another voter under the same address
    try:
        Contract.addVoter(owner.address)
        assert False, "Should have reverted due to attempting to add a second voter for the same address!"
    except TransactionRevertedError as e:
        assert e == Contract.VoterAlreadyRegistered(owner.address)

    # Test after voting started
    Contract.addSubject("Test Subject 1")
    Contract.startVoting()
    
    assert Contract.bool_voting_started() == True
    
    try:
        Contract.addVoter(user2.address)
        assert False, "Should have reverted due to trying to add voter after voting started!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingAlreadyStarted()
    
    assert len(Contract.getVoters()) == 2 # Only the owner and user1

@default_chain.connect()
@on_revert(revert_handler)
def test_start_voting():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]

    # Try to start the voting as non-owner
    try:
        Contract.startVoting(from_=user1)
        assert False, "Should have reverted due to trying to start voting as non-owner!"
    except TransactionRevertedError as e:
        assert e == Contract.NotPerformedByOwner()

    # Try to start voting with no voters
    Contract.addSubject("Subject Test 1")
    try:
        Contract.startVoting()
        assert False, "Should have reverted due to trying to start voting with no voters!"
    except TransactionRevertedError as e:
        assert e == Contract.NoVotersRegistered()

    # Try to start voting with no subjects
    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]

    Contract.addVoter(owner.address)
    try:
        Contract.startVoting()
        assert False, "Should have reverted due to trying to start voting with no subjects!"
    except TransactionRevertedError as e:
        assert e == Contract.NoSubjectsRegistered()

    # Start the voting
    Contract.addSubject("Subject Test 1")
    tx_result = Contract.startVoting()
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.VotingStarted()]
    assert Contract.bool_voting_started() == True
    assert Contract.voting_deadline() > 0



@default_chain.connect()
@on_revert(revert_handler)
def test_vote_positive():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]
    user2 = chain.accounts[2]
    user3 = chain.accounts[3]

    subject_name0 = "Test Subject 0"
    subject_name1 = "Test Subject 1"
    subject_name2 = "Test Subject 2"
    subject_name3 = "Test Subject 3"

    Contract.addVoter(owner.address)
    Contract.addVoter(user1.address)

    Contract.addSubject(subject_name0)
    Contract.addSubject(subject_name1, from_=user1)
    Contract.addSubject(subject_name2, from_=user2)
    Contract.addSubject(subject_name3, from_=user3)

    # Voting hasn't started
    try:
        Contract.votePositive(owner.address)
        assert False, "Should have reverted due to trying to vote before voting starts!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingHasntStarted()

    Contract.startVoting()

    # Vote for owner subject
    tx_result = Contract.votePositive(owner.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.PositiveVoted(owner.address, owner.address)]

    # Try to vote second time for the same subject
    try:
        Contract.votePositive(owner.address)
        assert False, "Should have reverted due to trying to vote second time for the same subject!"
    except TransactionRevertedError as e:
        assert e == Contract.VoterVotedThisSubjectAlready(owner.address)

    # Vote for second time
    tx_result = Contract.votePositive(user1.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.PositiveVoted(owner.address, user1.address)]

    # Vote for third time
    tx_result = Contract.votePositive(user2.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.PositiveVoted(owner.address, user2.address)]

    # Try to vote for the fourth time
    try:
        Contract.votePositive(user3.address)
        assert False, "Should have reverted due to trying to vote for a forth time with positive vote. Only 3 positive votes are allowed!"
    except TransactionRevertedError as e:
        assert e == Contract.NoMorePositiveVotes()
    
    # Vote for fourth time as negative
    tx_result = Contract.voteNegative(user3.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.NegativeVoted(owner.address, user3.address)]

    Contract.votePositive(user2.address, from_=user1.address)
    Contract.votePositive(user3.address, from_=user1.address)

    assert Contract._subjects(owner.address).votes == 1
    assert Contract._subjects(user1.address).votes == 1
    assert Contract._subjects(user2.address).votes == 2
    assert Contract._subjects(user3.address).votes == 0

    # Voting has ended
    Contract = D21.deploy()
    Contract.addSubject("Test Subject 1")
    Contract.addVoter(owner.address)
    Contract.startVoting()
    default_chain.mine(lambda t: t + 4 * 60 * 60 * 24)

    try:
        Contract.votePositive(owner.address)
        assert False, "Should have reverted due to trying to vote after the election period ended!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingEnded(Contract.voting_deadline())


@default_chain.connect()
@on_revert(revert_handler)
def test_vote_negative():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]
    user2 = chain.accounts[2]
    user3 = chain.accounts[3]

    subject_name0 = "Test Subject 0"
    subject_name1 = "Test Subject 1"
    subject_name2 = "Test Subject 2"
    subject_name3 = "Test Subject 3"

    Contract.addVoter(owner.address)

    Contract.addSubject(subject_name0)
    Contract.addSubject(subject_name1, from_=user1)
    Contract.addSubject(subject_name2, from_=user2)
    Contract.addSubject(subject_name3, from_=user3)

    # Voting hasn't started
    try:
        Contract.voteNegative(owner.address)
        assert False, "Should have reverted due to trying to vote before voting starts!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingHasntStarted()

    Contract.startVoting()

    # Try to vote negative as first vote
    try:
        Contract.voteNegative(owner.address)
        assert False, "Should have reverted due to trying to vote negative as first vote!"
    except TransactionRevertedError as e:
        assert e == Contract.NotEnoughPositiveVotesUsed(0)

    # Vote first time
    tx_result = Contract.votePositive(owner.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.PositiveVoted(owner.address, owner.address)]

    # Try to vote negative as second vote
    try:
        Contract.voteNegative(user1.address)
        assert False, "Should have reverted due to trying to vote negative as first vote!"
    except TransactionRevertedError as e:
        assert e == Contract.NotEnoughPositiveVotesUsed(1)

    # Vote second time
    tx_result = Contract.votePositive(user1.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.PositiveVoted(owner.address, user1.address)]

    # Give third vote as negative 
    tx_result = Contract.voteNegative(user2.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.NegativeVoted(owner.address, user2.address)]

    # Try to give fourth vote as negative 
    try:
        Contract.voteNegative(user3.address)
        assert False, "Should have reverted due to trying to vote negative for a second time!"
    except TransactionRevertedError as e:
        assert e == Contract.NegativeVoteHasBeenAlreadyUsed()

    # Try to give fourth vote as positive to a subject that has been given a negative already
    try:
        Contract.votePositive(user2.address)
        assert False, "Should have reverted due to trying to vote positive to a subject that has been previously given negative vote!"
    except TransactionRevertedError as e:
        assert e == Contract.VoterVotedThisSubjectAlready(user2.address)

    # Vote postive fourth time
    tx_result = Contract.votePositive(user3.address)
    assert len(tx_result.events) == 1
    assert tx_result.events == [D21.PositiveVoted(owner.address, user3.address)]

    assert Contract._subjects(owner.address).votes == 1
    assert Contract._subjects(user1.address).votes == 1
    assert Contract._subjects(user2.address).votes == -1
    assert Contract._subjects(user3.address).votes == 1

    # Voting has ended
    Contract = D21.deploy()
    Contract.addSubject("Test Subject 1")
    Contract.addVoter(owner.address)
    Contract.startVoting()
    default_chain.mine(lambda t: t + 4 * 60 * 60 * 24)

    try:
        Contract.voteNegative(owner.address)
        assert False, "Should have reverted due to trying to vote after the election period ended!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingEnded(Contract.voting_deadline())

@default_chain.connect()
@on_revert(revert_handler)
def test_vote_batch():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]
    user2 = chain.accounts[2]
    user3 = chain.accounts[3]
    user4 = chain.accounts[4]


    subject_name0 = "Test Subject 0"
    subject_name1 = "Test Subject 1"
    subject_name2 = "Test Subject 2"
    subject_name3 = "Test Subject 3"
    subject_name4 = "Test Subject 4"

    Contract.addVoter(owner.address)
    Contract.addVoter(user1.address)

    Contract.addSubject(subject_name0)
    Contract.addSubject(subject_name1, from_=user1)
    Contract.addSubject(subject_name2, from_=user2)
    Contract.addSubject(subject_name3, from_=user3)
    Contract.addSubject(subject_name4, from_=user4)

    # Voting hasn't started
    try:
        Contract.voteBatch([owner.address], [True])
        assert False, "Should have reverted due to trying to vote before voting starts!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingHasntStarted()

    Contract.startVoting()

    # Empty subjects
    try:
        Contract.voteBatch([], [True])
        assert False, "Should have reverted due to trying to vote for no subjects specified!"
    except TransactionRevertedError as e:
        assert e == Contract.EmptySubjectsNotAllowed()

    # Empty subjects
    try:
        Contract.voteBatch([owner.address], [])
        assert False, "Should have reverted due to trying to vote for subjects without specifying the vote type!"
    except TransactionRevertedError as e:
        assert e == Contract.SubjectsVotesNotSameLength(1, 0)

    # Different length in subjects and votes
    try:
        Contract.voteBatch([owner.address, user1.address, user2.address], [True, False, True, False])
        assert False, "Should have reverted due to trying to vote for subjects with different amount of votes than subjects!"
    except TransactionRevertedError as e:
        assert e == Contract.SubjectsVotesNotSameLength(3, 4)

    # Try to exceed the amount of left votes
    try:
        Contract.voteBatch([owner.address, user1.address, user2.address, user3.address, user4.address], [True, False, True, False, False])
        assert False, "Should have reverted due to trying to vote for subjects with different amount of votes than subjects!"
    except TransactionRevertedError as e:
        assert e == Contract.SentMoreThanRemainingVotes(4)

    # Try to vote first negative in a batch
    try:
        Contract.voteBatch([owner.address, user1.address, user2.address, user3.address], [False, True, True, True])
        assert False, "Should have reverted due to trying to vote for subjects with different amount of votes than subjects!"
    except TransactionRevertedError as e:
        assert e == Contract.NotEnoughPositiveVotesUsed(0)

    # Try to vote second negative in a batch
    try:
        Contract.voteBatch([owner.address, user1.address, user2.address, user3.address], [True, False, True, True])
        assert False, "Should have reverted due to trying to vote for subjects with different amount of votes than subjects!"
    except TransactionRevertedError as e:
        assert e == Contract.NotEnoughPositiveVotesUsed(1)

    # Try to vote four positive in a batch
    try:
        Contract.voteBatch([owner.address, user1.address, user2.address, user3.address], [True, True, True, True])
        assert False, "Should have reverted due to trying to vote for subjects with different amount of votes than subjects!"
    except TransactionRevertedError as e:
        assert e == Contract.NoMorePositiveVotes()

    # Try to vote four positive in a batch
    try:
        Contract.voteBatch([owner.address, user1.address, user2.address, user3.address], [True, True, True, True])
        assert False, "Should have reverted due to trying to vote for subjects with different amount of votes than subjects!"
    except TransactionRevertedError as e:
        assert e == Contract.NoMorePositiveVotes()

    # Vote negative as third vote
    tx_result = Contract.voteBatch([owner.address, user1.address, user2.address, user3.address], [True, True, False, True])
    assert len(tx_result.events) == 4
    assert tx_result.events == [
        D21.PositiveVoted(owner.address, owner.address),
        D21.PositiveVoted(owner.address, user1.address),
        D21.NegativeVoted(owner.address, user2.address),
        D21.PositiveVoted(owner.address, user3.address),
        ]

    # Vote negative as fourth vote
    tx_result = Contract.voteBatch([owner.address, user1.address, user2.address, user3.address], [True, True, True, False], from_=user1.address)
    assert len(tx_result.events) == 4
    assert tx_result.events == [
        D21.PositiveVoted(user1.address, owner.address),
        D21.PositiveVoted(user1.address, user1.address),
        D21.PositiveVoted(user1.address, user2.address),
        D21.NegativeVoted(user1.address, user3.address),
        ]

    assert Contract._subjects(owner.address).votes == 2
    assert Contract._subjects(user1.address).votes == 2
    assert Contract._subjects(user2.address).votes == 0
    assert Contract._subjects(user3.address).votes == 0

    # Voting has ended
    Contract = D21.deploy()
    Contract.addSubject("Test Subject 1")
    Contract.addVoter(owner.address)
    Contract.startVoting()
    default_chain.mine(lambda t: t + 4 * 60 * 60 * 24)

    try:
        Contract.voteBatch([owner.address], [True])
        assert False, "Should have reverted due to trying to vote after the election period ended!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingEnded(Contract.voting_deadline())


@default_chain.connect()
@on_revert(revert_handler)
def test_get_remaining_time():
    from pytypes.contracts.D21 import D21 as D21

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    subject_name0 = "Test Subject 0"
    Contract.addVoter(owner.address)
    Contract.addSubject(subject_name0)

    # Get time before election starts
    assert Contract.getRemainingTime() == 0

    # Get time during voting period
    Contract.startVoting()
    four_days = 4 * 60 * 60 * 24
    assert Contract.getRemainingTime() == four_days

    default_chain.mine(lambda t: t + 1 * 60 * 60 * 24 + 60)
    assert Contract.getRemainingTime() == four_days - (1 * 60 * 60 * 24 + 60)

    default_chain.mine(lambda t: t + 1 * 60 * 60 * 24 + 60)
    assert Contract.getRemainingTime() == four_days - (2 * 60 * 60 * 24 + 120)

    default_chain.mine(lambda t: t + 5 * 60 * 60)
    assert Contract.getRemainingTime() == four_days - (2 * 60 * 60 * 24 + 18120)

    default_chain.mine(lambda t: t + 12 * 60)
    assert Contract.getRemainingTime() == four_days - (2 * 60 * 60 * 24 + 18840)

    # Try to get time after the voting period has ended
    default_chain.mine(lambda t: t + 2 * 60 * 60 * 24)
    try:
        Contract.getRemainingTime()
        assert False, "Should have reverted due to trying to get remaining time after the election ended!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingEnded(Contract.voting_deadline())


@default_chain.connect()
@on_revert(revert_handler)
def test_get_results():
    from pytypes.contracts.D21 import D21, Subject 

    Contract = D21.deploy()
    chain.default_tx_account = chain.accounts[0]
    
    owner = chain.accounts[0]
    user1 = chain.accounts[1]
    user2 = chain.accounts[2]
    user3 = chain.accounts[3]
    user4 = chain.accounts[4]


    subject_name0 = "Test Subject 0"
    subject_name1 = "Test Subject 1"
    subject_name2 = "Test Subject 2"
    subject_name3 = "Test Subject 3"
    subject_name4 = "Test Subject 4"

    Contract.addVoter(owner.address)
    Contract.addVoter(user1.address)
    Contract.addVoter(user2.address)
    Contract.addVoter(user3.address)
    Contract.addVoter(user4.address)

    Contract.addSubject(subject_name0)
    Contract.addSubject(subject_name1, from_=user1)
    Contract.addSubject(subject_name2, from_=user2)
    Contract.addSubject(subject_name3, from_=user3)
    Contract.addSubject(subject_name4, from_=user4)

    # Try to get results before voting started
    try:
        Contract.getResults()
        assert False, "Should have reverted due to trying to get results before the voting starts!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingHasntStarted()

    Contract.startVoting()

    # Voting hasnt ended
    try:
        Contract.getResults()
        assert False, "Should have reverted due to trying to get results before the voting ends!"
    except TransactionRevertedError as e:
        assert e == Contract.VotingHasntEndedYet(Contract.voting_deadline())
    
    # Test correct voting and results
    Contract.voteBatch([user1.address, user2.address, user4.address, user3.address], [True, True, True, False])
    Contract.voteBatch([owner.address, user3.address, user2.address], [True, True, True], from_=user1.address)
    Contract.voteBatch([user3.address, user2.address, user4.address, user1.address], [True, True, False, True], from_=user2.address)
    Contract.voteBatch([user2.address, user1.address, user4.address], [True, True, False], from_=user3.address)
    Contract.voteBatch([user2.address], [True], from_=user4.address)

    default_chain.mine(lambda t: t + 4 * 60 * 60 * 24 + 1)

    assert Contract.getResults() == [
        Subject(subject_name2, 5),
        Subject(subject_name1, 3),
        Subject(subject_name0, 1),
        Subject(subject_name3, 1),
        Subject(subject_name4, -1),
    ]

    Contract = D21.deploy()
    Contract.addVoter(owner.address)
    Contract.addVoter(user1.address)
    Contract.addVoter(user2.address)
    Contract.addVoter(user3.address)
    Contract.addVoter(user4.address)

    Contract.addSubject(subject_name0)
    Contract.addSubject(subject_name1, from_=user1)
    Contract.addSubject(subject_name2, from_=user2)
    Contract.addSubject(subject_name3, from_=user3)
    Contract.addSubject(subject_name4, from_=user4)
    Contract.startVoting()

    Contract.voteBatch([user3.address, user2.address, user4.address], [True, True, False])
    Contract.voteBatch([owner.address, user3.address, user2.address], [True, True, False], from_=user1.address)
    Contract.voteBatch([user3.address, user2.address, user4.address], [True, True, False], from_=user2.address)
    Contract.voteBatch([user3.address, user1.address, user4.address], [True, True, False], from_=user3.address)
    Contract.voteBatch([user3.address], [True], from_=user4.address)

    default_chain.mine(lambda t: t + 4 * 60 * 60 * 24 + 1)

    assert Contract.getResults() == [
        Subject(subject_name3, 5),
        Subject(subject_name0, 1),
        Subject(subject_name2, 1),
        Subject(subject_name1, 1),
        Subject(subject_name4, -3),
    ]