cmake_minimum_required( VERSION 2.8 )

project( Q_pmtn_Cmax_solver )
add_executable( Q_pmtn_Cmax_solver "main.cpp")

add_library( library "Q_pmtn_Cmax_solver.cpp" )
target_link_libraries( Q_pmtn_Cmax_solver library)

