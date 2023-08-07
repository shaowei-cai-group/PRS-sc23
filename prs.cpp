#include "prs.h"
#include "light.hpp"

static vec<int> tmp;

light* prs_init(int num_threads) {
    assert(num_threads >= 1 && num_threads <= 128);
    light* S = new light();
    S->opt->nThreads = num_threads;
    return S;
}
void prs_add(light* S, int lit) {
    
    if(lit == 0) {
        S->clauses.push();
        tmp.copyTo(S->clauses[S->clauses.size()-1]);
        tmp.clear();
    } else tmp.push(lit);
    S->num_vars = std::max(S->num_vars, abs(lit));
}

int prs_solve(light* S) {
    assert(tmp.size() == 0);
    int res = S->run();
    // printf("========== solve ============\n");
    // printf("result: %d\n", res);
    return res;
}

int prs_value(light* S, int lit) {

    assert(abs(lit) == abs(S->model[abs(lit)-1]));

    if(lit > 0) {
        return S->model[abs(lit)-1];
    } else {
        return -S->model[abs(lit)-1];
    }
}


void prs_release(light* S) {
    delete S;
}