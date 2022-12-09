#include "Q_pmtn_Cmax_solver.h"

int main(int argc, char **argv)
{
    bool detailedoutput = 0;
    if (argc == 1)
    {
        std::cout << "Error. Give an input file, please." << std::endl;
        return 1;
    }
    if (argc > 2)
    {
        std::string arg2(argv[2]);
        detailedoutput = (arg2 == "-d");
    }

    Q_pmtn_Cmax_solver MyProblem(argv[1], detailedoutput);
    if (MyProblem.file_not_ok_)
    {
        std::cout << "Error. File not found." << std::endl;
        return 1;
    }

    MyProblem.SolveProblem();

    return 0;
}