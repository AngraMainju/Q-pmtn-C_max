#include "Q_pmtn_Cmax_solver.h"
using namespace std;

bool AreSame(double a, double b)
{
    return fabs(a - b) < 1e-5;
}

bool comparator(const value_index &l, const value_index &r)
{
    return l.first > r.first;
}

Q_pmtn_Cmax_solver::Q_pmtn_Cmax_solver(string file_name, bool deto)
{
    detailed_output_ = deto;
    file_not_ok_ = ReadFromFile(file_name);
    if (!file_not_ok_)
    {
        CreateGroups();
    }
}

bool Q_pmtn_Cmax_solver::ReadFromFile(string file_name)
{
    ifstream myfile("./../testfiles/" + file_name);
    if (!myfile.is_open())
    {
        return 1;
    }
    else
    {
        istream &mystream = myfile;

        string line;
        int i = 0;
        machine_numb_ = 0;

        getline(mystream, line);
        {
            istringstream is(line);
            int temp;
            while (is >> temp)
            {
                machine_speed_.push_back(value_index(temp, i));
                i++;
                machine_numb_++;
            }
        }

        i = 0;
        jobs_numb_ = 0;
        getline(mystream, line);
        {

            istringstream is(line);
            int temp;
            while (is >> temp)
            {
                job_proc_time_.push_back(value_index(temp, i));
                i++;
                jobs_numb_++;
            }
        }

        sort(machine_speed_.begin(), machine_speed_.end(), comparator);
        sort(job_proc_time_.begin(), job_proc_time_.end(), comparator);
        first_free_machine_ = 0;

        return 0;
    }
}

void Q_pmtn_Cmax_solver::CreateGroupWithOneJob(int jobindex)
{
    groups_.push_back(Job_group());
    groups_[groups_.size() - 1].first_job_index_ = jobindex;
    groups_[groups_.size() - 1].last_job_index_ = jobindex;
    if (first_free_machine_ < machine_numb_)
    {
        groups_[groups_.size() - 1].first_machine_index_ = first_free_machine_;
        groups_[groups_.size() - 1].last_machine_index_ = first_free_machine_;
        groups_[groups_.size() - 1].machine_power_ = machine_speed_[first_free_machine_].first;
        first_free_machine_++;
    }
    else
    {
        groups_[groups_.size() - 1].first_machine_index_ = -1;
        groups_[groups_.size() - 1].last_machine_index_ = -1;
        groups_[groups_.size() - 1].machine_power_ = 0;
    }
    groups_[groups_.size() - 1].remaining_proc_time_ = job_proc_time_[jobindex].first;
    groups_[groups_.size() - 1].group_speed_ = groups_[groups_.size() - 1].machine_power_ / (groups_[groups_.size() - 1].last_job_index_ - groups_[groups_.size() - 1].first_job_index_ + 1);
}

void Q_pmtn_Cmax_solver::AddJobToLastGroup()
{
    groups_[groups_.size() - 1].last_job_index_++;
    if (first_free_machine_ < machine_numb_)
    {
        groups_[groups_.size() - 1].last_machine_index_++;
        groups_[groups_.size() - 1].machine_power_ += machine_speed_[first_free_machine_].first;
        first_free_machine_++;
    }
    groups_[groups_.size() - 1].group_speed_ = groups_[groups_.size() - 1].machine_power_ / (groups_[groups_.size() - 1].last_job_index_ - groups_[groups_.size() - 1].first_job_index_ + 1);
}

void Q_pmtn_Cmax_solver::CreateGroups()
{
    CreateGroupWithOneJob(0);
    for (int i = 1; i < jobs_numb_; ++i)
    {
        if (AreSame(job_proc_time_[i].first, job_proc_time_[i - 1].first))
        {
            AddJobToLastGroup();
        }
        else
        {
            CreateGroupWithOneJob(i);
        }
    }
}

void Q_pmtn_Cmax_solver::CalculatePhaseLength()
{
    phase_length_ = std::numeric_limits<double>::max();
    for (int i = 0; i < groups_.size() - 1; ++i)
    {
        double speeddifference = groups_[i].group_speed_ - groups_[i + 1].group_speed_;
        if (speeddifference > 0)
        {
            double temp = (groups_[i].remaining_proc_time_ - groups_[i + 1].remaining_proc_time_) / speeddifference;
            if (temp < phase_length_)
            {
                phase_length_ = temp;
            }
        }
    }
    for (int i = 0; i < groups_.size(); ++i)
    {

        double temp = groups_[i].remaining_proc_time_ / groups_[i].group_speed_;
        if (temp < phase_length_)
        {
            phase_length_ = temp;
        }
    }
}

void Q_pmtn_Cmax_solver::SimulatePhase()
{
    cout << "---  " << phase_start_ << " - " << phase_start_ + phase_length_ << "  ---  " << endl;
    for (int i = 0; i < groups_.size(); i++)
    {
        // write phase infos
        if (groups_[i].first_machine_index_ >= 0)
        {
            cout << "Group"
                 << " " << i + 1 << ": (";
            for (int j = groups_[i].first_machine_index_; j < groups_[i].last_machine_index_; j++)
            {
                cout << "M" << machine_speed_[j].second + 1 << ", ";
            }
            cout << "M" << machine_speed_[groups_[i].last_machine_index_].second + 1 << ") - (";
            for (int j = groups_[i].first_job_index_; j < groups_[i].last_job_index_; j++)
            {
                cout << "J" << job_proc_time_[j].second + 1 << ", ";
            }
            cout << "J" << job_proc_time_[groups_[i].last_job_index_].second + 1 << ")" << endl;

            if (detailed_output_)
            {
                int mnum = groups_[i].last_machine_index_ - groups_[i].first_machine_index_ + 1;
                int jnum = groups_[i].last_job_index_ - groups_[i].first_machine_index_ + 1;
                for (int k = 0; k < jnum; k++)
                {
                    cout << "     " << phase_start_ + k * phase_length_ / jnum << " - " << phase_start_ + (k + 1) * phase_length_ / jnum << ": ";
                    for (int l = 0; l < mnum; l++)
                    {
                        cout << "M" << machine_speed_[groups_[i].first_machine_index_ + ((k + l) % mnum)].second + 1 << " - ";
                        cout << "J" << job_proc_time_[groups_[i].first_job_index_ + ((k + l) % jnum)].second + 1 << "   ";
                    }
                    cout << endl;
                }
            }

            // Update remaining proc time
            groups_[i].remaining_proc_time_ -= groups_[i].group_speed_ * phase_length_;
            if (!AreSame(groups_[i].remaining_proc_time_, 0))
            {
                cout << "     rem. proc. time: " << groups_[i].remaining_proc_time_ << endl;
            }
            else
            {
                cout << "     Group finished." << endl;
            }
        }
    }
    cout << endl;
}

void Q_pmtn_Cmax_solver::CreateNewGroups()
{
    for (int i = 0; i < groups_.size(); i++)
    {
        if (AreSame(groups_[i].remaining_proc_time_, 0))
        {
            groups_.erase(groups_.begin() + i);
        }
    }
    for (int i = 0; i < groups_.size() - 1; i++)
    {

        if (AreSame(groups_[i].remaining_proc_time_, groups_[i + 1].remaining_proc_time_))
        {
            groups_[i].last_job_index_ = groups_[i + 1].last_job_index_;
            groups_[i].machine_power_ += groups_[i + 1].machine_power_;
            if (groups_[i + 1].last_machine_index_ >= 0)
            {
                groups_[i].last_machine_index_ = groups_[i + 1].last_machine_index_;
            }
            groups_[i].group_speed_ = groups_[i].machine_power_ / (groups_[i].last_job_index_ - groups_[i].first_job_index_ + 1);
            groups_.erase(groups_.begin() + i + 1);
            i--;
        }
    }
    phase_start_ += phase_length_;
}

void Q_pmtn_Cmax_solver::SolveProblem()
{
    while (groups_.size() > 1)
    {
        CalculatePhaseLength();
        SimulatePhase();
        CreateNewGroups();
    }
    if (groups_.size() <= 1)
    {
        if (groups_.size() == 1 && !AreSame(groups_[0].remaining_proc_time_, 0))
        {
            phase_length_ = groups_[0].remaining_proc_time_ / groups_[0].group_speed_;
            SimulatePhase();
        }
    }
}