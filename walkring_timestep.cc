// 
// walkring_timestep.cc
//
// Time stepping module for 1d random walk on a ring
//
#include "walkring_timestep.h"
#include <omp.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <stdlib.h>
// Perform a single time step for the random walkers
//
// parameters:
//
//  walkerpositions: the positions of a number of walkers (note that
//                   the number of walker Z can be found from
//                   Z=walkerpositions.size())
//
//  N:               the number of possible positions. All positions
//                   in the walkerpositions array should remain
//                   between 0 and N-1
//
//  prob:            the probability to jump to the left. Also the
//                   probability to just right.  (the probability to
//                   stay on the same spot is thus 1-2p.)
//
// output:
//
//  the content of the walkerpositions arrays should have changed to
//  reflect the random movement of all walker (i.e., they will each
//  have been given a chance to move on position to the left or two
//  the right).
//
void walkring_timestep(rarray<int,1>& walkerpositions, int N, double prob) {
    int nthreads = omp_get_num_threads();
    int Z = walkerpositions.size();
    #pragma omp parallel default(none) shared(Z, walkerpositions, prob, N, std::cout) 
    {
        static thread_local std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count()); // using clock to seed
        static thread_local std::uniform_real_distribution<double> dist(0.0000, 1.0000); //[0,1)
    #pragma omp for
    for (int i = 0; i < Z; i++) {
        double r = dist(mt); // drawing a random number
        if (r < prob) {
            // move to the right, respecting periodic boundaries
            walkerpositions[i]++;
            if (walkerpositions[i] == N)
                walkerpositions[i] = 0;
        } else if (r < 2*prob) {
            // move to the left, respecting periodic boundaries
            if (walkerpositions[i] == 0)
                walkerpositions[i] = N-1;
            else
                walkerpositions[i]--;
        } else {
            // walkerposition remains unchanged
        }
    
    }
    }
}

