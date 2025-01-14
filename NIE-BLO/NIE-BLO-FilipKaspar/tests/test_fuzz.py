from wake.testing import *
from wake.testing.fuzzing import *
from typing import Dict, List
from pytypes.contracts.D21 import D21 as D21

# Print failing tx call trace
def revert_handler(e: TransactionRevertedError):
    if e.tx is not None:
        print(e.tx.call_trace)
        print(e.tx.console_logs)

class BatchVoteFuzzTest(FuzzTest):
    voters_subjects : Dict[Address, int] = {}
    votes : List = []
    def pre_sequence(self) -> None:
        self.owner = chain.accounts[0]
        chain.default_tx_account = chain.accounts[0]

    def pre_flow(self, flow_vote_batch):
        self.Contract = D21.deploy(from_=self.owner)

        self.voters_subjects.clear()
        num_voters_subjects = random_int(4, 9)
        self.votes = [0] * (num_voters_subjects)
        for i in range(0, num_voters_subjects):
            self.Contract.addSubject(f"Test {i}", from_=chain.accounts[i])
            self.Contract.addVoter(chain.accounts[i])
            self.voters_subjects[chain.accounts[i]] = random_int(0, num_voters_subjects)
        
        print("-----------------------Start new flow-----------------------")
        print(self.voters_subjects)

        self.Contract.startVoting()

    @flow()
    def flow_vote_batch(self):
        print(len(self.voters_subjects.items()))
        for voter_addr, num_subjects in self.voters_subjects.items():
            start_subj = random_int(0, num_subjects)
            num_positive = 0
            negative_voted = False
            for cnt, i in enumerate(range(start_subj, random_int(start_subj, num_subjects))):
                vote = random_bool()
                vote = True if num_positive < 2 else vote
                vote = False if num_positive >= 3 else vote
                vote = True if negative_voted else vote

                self.Contract.voteBatch([list(self.voters_subjects.keys())[i]], [vote], from_=voter_addr)

                print(f"Acc {voter_addr}: {i} + {vote}")
                self.votes[i] += 1 if vote else -1
                num_positive += 1 if vote else 0
                negative_voted = False if vote else True
                
                if cnt >= 3: break

    @invariant()
    def invariant_balances(self):
        default_chain.mine(lambda t: t + 4 * 60 * 60 * 24 + 1)
        assert sorted(self.votes, reverse=True) == [obj.votes for obj in self.Contract.getResults()]
        tx = self.Contract.getResults(request_type="estimate")
        print(tx)
        breakpoint()


@default_chain.connect()
@on_revert(revert_handler)
def test_batch_votes():
    BatchVoteFuzzTest().run(1, 200)