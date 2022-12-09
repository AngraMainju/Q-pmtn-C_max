#ifndef Q_PMTN_H_
#define Q_PMTN_H_

#include <vector>
#include <iostream>
#include <limits>
#include <cmath>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;

typedef std::pair<double, int> value_index;
bool comparator(const value_index &l, const value_index &r);

struct Job_group
{
    int first_job_index_;
    int last_job_index_;
    int first_machine_index_;
    int last_machine_index_;

    double remaining_proc_time_;
    double machine_power_;
    double group_speed_;
};

class Q_pmtn_Cmax_solver
{
public:
    Q_pmtn_Cmax_solver(string file_name, bool deto);
    void SolveProblem();

    bool file_not_ok_;
    bool detailed_output_;

private:
    int jobs_numb_;
    vector<value_index> job_proc_time_;
    int machine_numb_;
    vector<value_index> machine_speed_;
    int first_free_machine_;

    double phase_start_ = 0;
    double phase_length_;

    vector<Job_group> groups_;

    // Reading file and initailizing variables
    bool ReadFromFile(string file_name);
    void CreateGroupWithOneJob(int jobindex);
    void AddJobToLastGroup();
    void CreateGroups();

    // Simulation of one phase
    void CalculatePhaseLength();
    void SimulatePhase();
    void CreateNewGroups();
};

#endif