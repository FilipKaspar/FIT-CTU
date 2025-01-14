#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"

using namespace std;
#endif /* __PROGTEST__ */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCompanyHandler {
public:
    explicit CCompanyHandler(ACompany &id) {
        company_id = id;
        to_send_empty = make_shared<condition_variable>();
    }

//private:
    shared_ptr<condition_variable> to_send_empty;
    ACompany company_id;

    thread wait_thread;
    thread solved_thread;
    unsigned long int last_solved = 0;

    vector<pair<unsigned long int *, AProblemPack>> problems;
    // Long int - indicates how many problems from pack has been loaded to solver if = problempack.size + 1. Then whole pack is finished
};

class COptimizer {
public:
    static bool usingProgtestSolver(void) {
        return true;
    }

    static void checkAlgorithm(AProblem problem) {
        // dummy implementation if usingProgtestSolver() returns true
    }

    void start(int threadCount);

    void stop();

    void addCompany(ACompany company);

    void waitThreads(CCompanyHandler &company);

    void solveThreads(CCompanyHandler &company);

    void workingThreads(AProgtestSolver &solver);

//private:
    condition_variable to_solve_empty;
    mutex wrk1;
    AProgtestSolver solver;

    int thread_count;

    vector<CCompanyHandler> companies;

    vector<thread> work_threads;
    deque<pair<unsigned long int, CCompanyHandler *>> all_problems;
    vector<pair<unsigned long int *, CCompanyHandler *>> problems_to_set;
};

// TODO: COptimizer implementation goes here
void COptimizer::addCompany(ACompany company) {
    CCompanyHandler comp(company);
    companies.push_back(move(comp));
}

void COptimizer::start(int threadCount) {
    thread_count = threadCount;

    solver = createProgtestSolver();
    for (int i = 0; i < threadCount; i++) {
        work_threads.emplace_back(&COptimizer::workingThreads, this, ref(solver));
    }

    for (auto &company: companies) {
        company.wait_thread = thread(&COptimizer::waitThreads, this, ref(company));
        company.solved_thread = thread(&COptimizer::solveThreads, this, ref(company));
    }
}

void COptimizer::waitThreads(CCompanyHandler &company) {
    AProblemPack cur_problem_pack = company.company_id->waitForPack();
    unique_lock<mutex> lock(wrk1);
    lock.unlock();

    unsigned long int cnt = 0;
    while (cur_problem_pack != nullptr) {
        lock.lock();
        company.problems.emplace_back(new unsigned long(0), cur_problem_pack);

        all_problems.emplace_back(cnt, &company);
        lock.unlock();

        to_solve_empty.notify_one();
        cur_problem_pack = company.company_id->waitForPack();
        cnt++;
    }
    lock.lock();
    company.problems.emplace_back(nullptr, nullptr);
    lock.unlock();
    company.to_send_empty->notify_one();
    //cout << "WaitThread skončil" << endl;
}

void COptimizer::solveThreads(CCompanyHandler &company) {
    while (true) {
        unique_lock<mutex> lock(wrk1);
        company.to_send_empty->wait(lock, [&company]() {
            if (company.problems.empty() || company.last_solved >= company.problems.size()) return false;
            if (!company.problems[company.last_solved].second) return true;
            return *company.problems[company.last_solved].first ==
                   company.problems[company.last_solved].second->m_Problems.size() + 1;
        });

        //cout << "SolveThread vybuzněn" << endl;

        if(company.last_solved >= company.problems.size()) continue;
        auto problem = company.problems[company.last_solved];
        //lock.unlock();
        if (!problem.first || !problem.second) break;

        while (*problem.first == problem.second->m_Problems.size() + 1) {
            company.company_id->solvedPack(problem.second);
            //lock.lock();

            delete company.problems[company.last_solved].first;
            company.problems[company.last_solved].second = nullptr;

            company.last_solved++;
            if(company.last_solved >= company.problems.size()) break;
            problem = company.problems[company.last_solved];
            //lock.unlock();

            if (!problem.first || !problem.second) return;
        }
    }
}


void COptimizer::workingThreads(AProgtestSolver & solver) {
    while (true) {
        unique_lock<mutex> lock(wrk1);
        to_solve_empty.wait(lock, [this]() { return !all_problems.empty(); });

        //cout << "WorkingThread začátek" << endl;

        auto cur_company = all_problems.front();
        all_problems.pop_front();
        if (!cur_company.second) break;
        auto cur_problem = cur_company.second->problems[cur_company.first];
        lock.unlock();

        for (auto problem = cur_problem.second->m_Problems.begin() + int(*cur_problem.first);
             problem != cur_problem.second->m_Problems.end(); problem++) {
            lock.lock();
            if (!solver->addProblem(*problem)) {
                lock.unlock();
                break;
            }
            lock.unlock();
            (*(cur_problem.first))++;
        }

        lock.lock();
        if (cur_problem.second->m_Problems.size() == *cur_problem.first) {
            problems_to_set.emplace_back(cur_problem.first, cur_company.second);
        } else {
            all_problems.push_front(cur_company);
            to_solve_empty.notify_one();
        }
        //If we used all the problems, set to counter to the size of the problem array

        // Checking here one more time, in case the last problem has been loaded, therefore the for loop is gonna end and it would have to
        // wait for another thread call to call the solve method
        if (!solver->hasFreeCapacity()) { // Segfault here??
            auto copy_solver = solver;
            solver = createProgtestSolver();
            auto copy = problems_to_set;
            problems_to_set.clear();
            lock.unlock();
            copy_solver->solve();
            //cout << "Solver activated" << endl;
            lock.lock();
            for (auto &problem: copy) {
                (*problem.first)++;
                problem.second->to_send_empty->notify_one();
            }
        }
        //cout << "WorkingThread konec" << endl;
    }
}

void COptimizer::stop() {
    unique_lock<mutex> lock(wrk1);
    lock.unlock();
    for (auto &company: companies) {
        company.wait_thread.join();
    }
    lock.lock();

    for (int i = 0; i < thread_count; i++) all_problems.emplace_back(420, nullptr);
    to_solve_empty.notify_all();
    lock.unlock();

    for (auto &work_thread: work_threads) {
        work_thread.join();
    }

    lock.lock();
    if (!problems_to_set.empty()) {
        solver->solve();
        for (auto &problem: problems_to_set) {
            (*problem.first)++;
            problem.second->to_send_empty->notify_one();
        }
        problems_to_set.clear();
    }
    lock.unlock();

    for (auto &company: companies) {
        company.solved_thread.join();
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__

int main(void) {
    for(int i = 0; i < 1; i++) {
        COptimizer optimizer;
        ACompanyTest company = std::make_shared<CCompanyTest>();
        ACompanyTest companyb = std::make_shared<CCompanyTest>();
        optimizer.addCompany(company);
        optimizer.addCompany(companyb);
        optimizer.start(50);
        optimizer.stop();
        if (!company->allProcessed())
            throw std::logic_error("(some) problems were not correctly processsed");
        if (!companyb->allProcessed())
            throw std::logic_error("(some) problems were not correctly processsed");
        //cout << i << endl;
    }
    return 0;
}

#endif /* __PROGTEST__ */
