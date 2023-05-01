#include "light.hpp"
#include "solver_api/basekissat.hpp"
#include "solver_api/baselstech.hpp"
#include "sharer.hpp"
#include "paras.hpp"
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include <mutex>
#include "mpi.h"

auto clk_st = std::chrono::high_resolution_clock::now();
char* worker_sign = "";

std::atomic<int> terminated;
int result = 0;
int winner_conf;

void * read_worker(void *arg) {
    basesolver * sq = (basesolver *)arg;
    sq->parse_from_MEM(sq->controller->instance);
    return NULL;
}

void * solve_worker(void *arg) {
   basesolver * sq = (basesolver *)arg;
   while (!terminated) {
        int res = sq->solve();
       
        if (res && !terminated) {
            //printf("c result: %d, winner is %d, winner run %d confs\n", res, sq->id, sq->get_conflicts());
            terminated = 1;
            sq->controller->terminate_workers();
            result = res;
            sq->controller->update_winner(sq->id, 0);
            winner_conf = sq->get_conflicts();
            if (res == 10) sq->get_model(sq->model);
        }
        //printf("get result %d with res %d\n", sq->id, res);

   }
   return NULL;
}

void light::init_workers() {
    terminated = 0;
    for (int i = 0; i < OPT(threads); i++) {
        if (solver_type == KISSAT) {
            basekissat* kissat = new basekissat(i, this);
            workers.push(kissat);
        }
        else if (solver_type == LSTECH) {
            baselstech* lstech = new baselstech(i, this);
            workers.push(lstech);    
        }
    }
}

void light::diversity_workers() {
    printf("c num %d\n", num_procs);
    for (int i = 0; i < OPT(threads); i++) {
        if(worker_type == SAT) {
            if (OPT(shuffle)) {
                workers[i]->configure("worker_index", i);
                workers[i]->configure("worker_number", OPT(threads));

                if(rank == 1) {
                    workers[i]->configure("worker_seed", 0);
                } else {
                    workers[i]->configure("worker_seed", rank);
                }
            }
            if (solver_type == KISSAT) {
                workers[i]->configure("stable", 1);
                workers[i]->configure("target", 2);
                workers[i]->configure("phase", 1);

                if (i == 2)
                    workers[i]->configure("stable", 0); 
                else if (i == 6)
                    workers[i]->configure("stable", 2); 

                if (i == 7)
                    workers[i]->configure("target", 0);
                else if (i == 0 || i == 2 || i == 3 || i == 6)
                    workers[i]->configure("target", 1);
                            
                if (i == 3 || i == 11 || i == 12 || i == 13 || i == 15)
                    workers[i]->configure("phase", 0);
            }
                
        } else if(worker_type == UNSAT) {
            OPT(share_lits) = 3000;
            workers[i]->configure("chrono", 1);
            workers[i]->configure("stable", 0);
            workers[i]->configure("target", 1);
            if (solver_type == KISSAT) {
                if (i == 0  || i == 1  || i == 7  || i == 8  || i == 11 || i == 15)
                if (i == 3  || i == 6  || i == 8  || i == 11 || i == 12 || i == 13)
                    workers[i]->configure("chrono", 0);
                if (i == 2)
                    workers[i]->configure("stable", 1); 
                if (i == 9)
                    workers[i]->configure("target", 0);
                else if (i == 14)
                    workers[i]->configure("target", 2);
            }
        } else {
            if (OPT(shuffle)) {
                workers[i]->configure("worker_index", i);
                workers[i]->configure("worker_number", OPT(threads));
                if(rank == num_procs - 1) {
                    workers[i]->configure("worker_seed", 0);
                } else {
                    workers[i]->configure("worker_seed", rank);
                }
            }
            if (solver_type == KISSAT) {   
                if (i == 13 || i == 14 || i == 9)
                    workers[i]->configure("tier1", 3);
                else
                    workers[i]->configure("tier1", 2);
                if (i == 3 || i == 6 || i == 8 || i == 11 || i == 12 || i == 13 || i == 14 || i == 15)
                    workers[i]->configure("chrono", 0);
                else
                    workers[i]->configure("chrono", 1);    

                if (i == 2 || i == 15)
                    workers[i]->configure("stable", 0);
                else if (i == 6 || i == 14)
                    workers[i]->configure("stable", 2);
                else
                    workers[i]->configure("stable", 1);

                if (i == 10)
                    workers[i]->configure("walkinitially", 1);
                else
                    workers[i]->configure("walkinitially", 0);

                if (i == 7 || i == 8 || i == 9 || i == 11)
                    workers[i]->configure("target", 0);
                else if (i == 0 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 10)
                    workers[i]->configure("target", 1);
                else
                    workers[i]->configure("target", 2);
                    
                if (i == 4 || i == 5 || i == 8 || i == 9 || i == 12 || i == 13 || i == 15)
                    workers[i]->configure("phase", 0);
                else
                    workers[i]->configure("phase", 1);
            }
        }
    }
}

void light::terminate_workers() {
    // printf("c controller reach limit\n");
    for (int i = 0; i < OPT(threads); i++) {
        workers[i]->terminate();
    }
}

void light::parse_input() {

    pthread_t *ptr = new pthread_t[OPT(threads)];
    for (int i = 0; i < OPT(threads); i++) {
        pthread_create(&ptr[i], NULL, read_worker, workers[i]);
    }   
    for (int i = 0; i < OPT(threads); i++) {
        pthread_join(ptr[i], NULL);
    }
    delete []ptr;

}

int light::solve() {
    //printf("c -----------------solve start----------------------\n");
    
    pthread_t *ptr = new pthread_t[OPT(threads)];
    for (int i = 0; i < OPT(threads); i++) {
      pthread_create(&ptr[i], NULL, solve_worker, workers[i]);
    }

    thread_inf unimprove[OPT(threads)];
    auto clk_sol_st = std::chrono::high_resolution_clock::now();
    int pre_time = std::chrono::duration_cast<std::chrono::seconds>(clk_sol_st - clk_st).count();
    int sol_thd = 0, intv_time = OPT(reset_time);// 初始化共享子句类
    
    if(OPT(share)) {
        s = new sharer(this);
        for (int j = 0; j < OPT(threads); j++) {
            s->producers.push(workers[j]);
            s->consumers.push(workers[j]);
        }
        s->clause_sharing_init(sharing_groups);
    }

    while (!terminated) {

        usleep(500000);

        if(OPT(share)) s->do_clause_sharing();

        int flag;
        // when getting terminate signal
        if(MPI_Test(&terminal_request, &flag, MPI_STATUS_IGNORE) == MPI_SUCCESS && flag == 1) {
            terminated = 1;
            terminate_workers();
        }

        auto clk_now = std::chrono::high_resolution_clock::now();
        int solve_time = std::chrono::duration_cast<std::chrono::seconds>(clk_now - clk_st).count();
        if (solve_time >= OPT(times)) {
            terminated = 1;
            terminate_workers();
        }
    }

    if(OPT(share)) s->clause_sharing_end();

    // printf("ending solve\n");
    // terminate_workers(); //important, need combine nps/dps !!!!!!!!!!!!!!!!

    for (int i = 0; i < OPT(threads); i++) {
        pthread_join(ptr[i], NULL);
    }
    
    // printf("ending join\n");
    if (result == 10)
        workers[winner_id]->model.copyTo(model);
    auto clk_now = std::chrono::high_resolution_clock::now();
    double solve_time = std::chrono::duration_cast<std::chrono::milliseconds>(clk_now - clk_sol_st).count();
    solve_time = 0.001 * solve_time;
    // printf("c solve time: %.2lf\nwinner is %d, period is %d\n", solve_time, winner_id, winner_period);
    // for (int i = 0; i < OPT(threads); i++) {
    //     printf("c thread %d waiting time: %.2lf\n", i, workers[i]->get_waiting_time());
    // }   
    delete []ptr;
    return result;
}

int light::run() {

    seperate_groups();

    init_workers();

    if(OPT(pakis)) diversity_workers();

    parse_input();

    int res = solve();
    
    return res;
}

void light::seperate_groups() {
    solver_type = light::KISSAT;
    // split distribute nodes to groups(SAT MODE、UNSAT MODE、DEFAULT MODE)
    int worker_procs = num_procs - 1;

    if(worker_procs >= 8) {
        int sat_procs = worker_procs / 8;
        int unsat_procs = worker_procs / 4;
        int default_procs = worker_procs - sat_procs - unsat_procs;

        std::vector<int> tmp;
        // [1, sat_procs] for sat
        if(rank >= 1 && rank <= sat_procs) {
            worker_type = light::SAT;
        }
        for(int i=1; i<=sat_procs; i++) {
            tmp.push_back(i);
        }
        sharing_groups.push_back(tmp);
        
        // [sat_procs+1, sat_procs+unsat_procs] for unsat
        if(rank >= sat_procs+1 && rank <= sat_procs+unsat_procs) {
            worker_type = light::UNSAT;
        }

        tmp.clear();
        for(int i=sat_procs+1; i<=sat_procs+unsat_procs; i++) {
            tmp.push_back(i);
        }
        sharing_groups.push_back(tmp);

        // [sat_procs+unsat_procs+1, worker_procs]
        if(rank >= sat_procs+unsat_procs+1 && rank <= worker_procs) {
            worker_type = light::DEFAULT;
        }

        tmp.clear();
        for(int i=sat_procs+unsat_procs+1; i<=worker_procs; i++) {
            tmp.push_back(i);
        }
        sharing_groups.push_back(tmp);

    } else {
        // 总线程太小就跑默认策略
        worker_type = light::DEFAULT;
        std::vector<int> tmp;
        for(int i=1; i<=worker_procs; i++) {
            tmp.push_back(i);
        }
        sharing_groups.push_back(tmp);
    }
}


void print_model(vec<int> &model) {
    printf("v");
    for (int i = 0; i < model.size(); i++) {
        printf(" %d", model[i]);
    }
    puts(" 0");
}