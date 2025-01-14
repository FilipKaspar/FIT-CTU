# Security Audit | D21-bcbr720

by **Filip Kašpar**

## Contents

1. [Executive summary](#1-executive-summary)
2. [System Overview](#2-system-overview)
3. [Trust Model](#3-trust-model)
4. [Audit Methodology](#4-audit-methodology)
5. [Static analysis](#5-static-analysis)
6. [Manual Code Review](#6-manual-code-review)
7. [Local Deployment](#7-local-deployment)
8. [Unit and Fuzz Tests](#8-unit-and-fuzz-tests)
9. [Findings](#9-findings)

## 1. Executive Summary

The D21 Voting System is a decentralized smart contract implementation of the "Janeček Method D21," designed to provide a transparent, fair, and modern voting mechanism. I have been asked to perform a security audit for this smart contract.

### **Revision 1.0**

The smart contract has been reviewed in the dates between 14 December and 16 December 2024.

The audit has been performed on the latest commit, `08e0010`.

The scope for this task was pretty small; only **D21** smart contract has been audited.

First I used the automatic analysis tool **Wake** to perform static analysis on the contract. Afterwards, a manual code review has been performed on the contract, where I looked for potential vulnerabilities and coding bad habits. After the manual code review, the contract has been locally deployed, and different techniques were used to try to hack the contract. For the last step, unit and fuzz tests were implemented and run on the **D21** contract.

During the review, I mainly focused on those points:

1. Use cases for the **D21** voting system should be correctly implemented
2. Reentrancy attacks
3. Best practices for the code

After performing all the above-mentioned steps, **no** errors or vulnerabilities have been found in the contract.

Although no vulnerabilities have been found in the contract, it is still recommended to address all of the recommendations mentioned in this security audit.

<div style="page-break-after: always;"></div>

## 2. System Overview

This section contains an outline of the audited contract.

### **D21**

Since the code has only one contract, all the functionalities are implemented in this contract.

The contract should implement 11 use cases:

1. UC1 - Everyone can register a subject (e.g. political party)
2. UC2 - Everyone can list registered subjects
3. UC3 - Everyone can see the subject’s result
4. UC4 - Only the voting owner can add eligible voters
5. UC5 - Only the owner can start the voting period
6. UC6 - Voting ends after 4 days from the voting start
7. UC7 - Subjects can’t be registered after the voting has started
8. UC8 - Every voter has 3 positive and 1 negative vote
9. UC9 - Voter can not give more than 1 vote to the same subject
10. UC10 - Negative vote can be used only after 2 positive votes
11. UC11 - Voters should be able to vote in one transaction

## 3. Trust Model

### Key Participants and Their Trust Assumptions

1. **Contract Owner**
    - Role: The contract owner is responsible for managing the voting process, including adding eligible voters and subjects, and starting the voting period.
    - Trust Assumption: The owner is trusted to act honestly and in the best interest of the system. Misuse of this role (e.g. adding unauthorized voters) can compromise the fairness of the voting process.
2. **Voters**

    - Role: Voters are participants eligible to cast votes according to the rules defined by the contract.
    - Trust Assumption: Voters are not trusted to follow the rules without enforcement by the contract. The smart contract has to define those rules for voters.

3. **Subjects**

    - Role: Subjects are political parties that voters can vote for.
    - Trust Assumption: Subjects are not trusted to follow the rules without enforcement by the contract. The smart contract has to define those rules for subjects.

<div style="page-break-after: always;"></div>

## 4. Audit Methodology

1. **Automatic tool-based analysis** - Using tools like Wake the code is checked. For example, the static analysis is performed in this step.

2. **Manual code review** - In this step the contract and the whole code are checked for potential vulnerabilities. The code is checked for best and bad practices as well, such as code duplication and code inconsistency.

3. **Local deployment of the contract and hacking** - This step focuses on locally deploying the contract and trying to manually break it.

4. **Unit and fuzz testing** - As the final phase, unit and fuzz tests are written and run on the contract using the Wake framework. This step ensures that the contract works as expected.


## 5. Static analysis

Static analysis is a critical step in ensuring the security and correctness of smart contracts. For this security audit the **Wake** framework has been used.

The **D21** contract has been checked with Wake static analyzer and no errors or warnings has been found.


## 6. Manual Code Review

The code has been checked line by line, and **no** vulnerabilities have been found. The code looks good from a functioning point of view and doesn't show any signs of potential weaknesses.

For the best practices part, there could have been some adjustments. Namely:

1. The contract doesn't use the newest Solidity approach when reverting in a function. The current approach in the contract is to check using if and then revert. The newest method is to use the **require** function with an error code inside as an argument.

2. Some checks (e.g., ownerOnly, votingStarted, ...) are duplicated throughout the contract, making it easier to make a mistake. The preferred approach for this is to make a solidity **modifier** that can be reused for different functions. That makes the code more efficient for gas since less code is written, and also there is less room for mistakes since the code is defined in one place.

3. Function **getResults()** in the contract calculates the results every time the function is called; therefore, if a lot of voters were to vote on the smart contract function getResults(), it would take a long time to finish. It is recommended to calculate the results somewhere else and not directly in the **getResults()** function. For example, in the **votePositive** and **voteNegative** functions.


There have also been some well-done, implemented best practices. Namely:

1. Making votePostive and votePositive private and calling them from different functions
2. Consistent and self-explanatory variable naming.

<div style="page-break-after: always;"></div>

## 7. Local Deployment

Testing on a local deployment is crucial since we can test how the contract would behave on an actual chain.

The compilation of the contract didn't raise any errors or warnings; therefore, the compilation was successful.

The same goes for deploying the contract; again, no errors or warnings have been raised; therefore, the deployment of the contract is successful as well.

After playing around with the contract, **no** noticeable errors or weaknesses have been found. The contract worked as intended for the desired use cases defined in the system overview chapter. If anything has been missed, unit and fuzz testing should reveal it.

It wasn't in the official requirements for this project, but an additional proxy contract is recommended to be created for the project. A simple proxy contract that points to the address of the actual **D21** contract. The advantage of this approach is that when the **D21** contract would have to be modified, it can happen so the address of the contract stays the same for the users of the contract.

## 8. Unit and Fuzz Tests

Unit and fuzz tests have been implemented for this audit.

### **Unit Tests**

Code with unit tests can be found in the `tests/test_default.py` file.

The file has been run, and no weaknesses of the functionality of the contract have been discovered.

During writing the unit tests, some observations have been made. Here is one recommendation:

1. When testing voting after the voting period is over, the **eVotingNotAllowed** error is raised. A more precise message is recommended, something like **eVotingHasEnded**. The same goes for when the voting hasn't started yet, something like **eVotingHasNotStarted**.


### **Fuzz tests**

For the audit, one fuzz test has been made. It can be found in the `tests/test_fuzz_voting.py` file.

The fuzz test does 2 main things:
1. Checks if the votebatch correctly adds votes to subjects
2. Checks if getResults returns subjects in correct order

The fuzz test has been run over **20 000 times** and didn't return any errors when testing the contract.


After running unit and fuzz tests, still no vulnerabilities have been discovered.

<div style="page-break-after: always;"></div>

## 9. Findings

After carefully implementing all the steps described in the methodology chapter, no findings have been discovered. No weaknesses have been found when the contract was inspected manually. Similarly, no weaknesses have been found when deploying and trying to hack the contract, as well as when running custom unit and fuzz tests.

This contract is just simply unhackable 😁.

Of course I could have missed some possible attacks; therefore, another audit from a different company is recommended for extra check.