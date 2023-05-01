#include "utils/System.h"
#include "core/Dimacs.h"
#include "simp/SimpSolver.h"

#include "baselstech.hpp"

// using namespace Minisat;

// Macros for minisat literal representation conversion
#define MINI_LIT(lit) lit > 0 ? Minisat::mkLit(lit - 1, false) : Minisat::mkLit((-lit) - 1, true)

#define INT_LIT(lit) sign(lit) ? -(var(lit) + 1) : (var(lit) + 1)

void baselstech::add(int l) {
   puts("wrong");
}

int baselstech::configure(const char* name, int val) {
   if (strcmp(name, "worker_index") == 0) solver->worker_index = val;
   if (strcmp(name, "worker_seed") == 0) solver->worker_seed = val;
   if (strcmp(name, "worker_number") == 0) solver->worker_number = val;
   return 1;
}

int baselstech::solve() {
   Minisat::vec<Minisat::Lit> miniAssumptions;
   Minisat::lbool res = solver->solveLimited(miniAssumptions);
   if (res == (Minisat::lbool((uint8_t)0))) return 10;
   if (res == (Minisat::lbool((uint8_t)1))) return 20;
   return 0;
}

int baselstech::val(int l) {
   if (solver->model[l] != (Minisat::lbool((uint8_t)2))) {
      int lit = solver->model[l] == (Minisat::lbool((uint8_t)0)) ? l + 1 : -(l + 1);
      return lit;
   }
   return l;
}

void baselstech::terminate() {
   solver->interrupt();
}

void cbkLstechExportClause(void * issuer, int lbd, Minisat::vec<Minisat::Lit> &cls) {
	baselstech* mp = (baselstech*)issuer;
	if (lbd > mp->good_clause_lbd) return;

   shared_ptr<clause_store> ncls = std::make_shared<clause_store>(cls.size());  
	for (int i = 0; i < cls.size(); i++) {
		ncls->data[i] = INT_LIT(cls[i]);
   }

   ncls->lbd  = lbd;
   mp->export_clause.push(ncls);
}

bool cbkLstechImportClause(void * issuer, int * lbd, Minisat::vec<Minisat::Lit> & mcls) {
   baselstech* mp = (baselstech*)issuer;
   shared_ptr<clause_store> cls = NULL;
   if (mp->import_clause.pop(cls) == false) return false;
   *lbd = cls->lbd;
   mcls.clear();
   for (size_t i = 0; i < cls->size; i++) {
      Minisat::Lit lit = MINI_LIT(cls->data[i]);
      mcls.push(lit);
   }
   cls->free_clause();
   return true;
}

baselstech::baselstech(int id, light* light) : basesolver(id, light) {
   printf("c id is %d\n", id);
   solver = new Minisat::SimpSolver();
	solver->issuer          = this;
   solver->cbkExportClause = NULL;
   solver->cbkImportClause = NULL;
   if (OPT(share)) {
      solver->cbkExportClause = cbkLstechExportClause;
      solver->cbkImportClause = cbkLstechImportClause;
   }
}

baselstech::~baselstech()
{
	delete solver;
}

void baselstech::parse_from_MEM(char* instance) {
    int vars, clauses;
    vec<vec<int>> clause;
    readinstance(instance, &vars, &clauses, clause);
    maxvar = vars;
    solver->varGrowTo(vars);
    Minisat::vec<Minisat::Lit> lits;
    for (int i = 1; i <= clauses; i++) {
      int l = clause[i].size();
      lits.clear();
      for (int j = 0; j < l; j++) 
         lits.push(MINI_LIT(clause[i][j]));
      solver->addClause_(lits);
    }
}
int baselstech::get_conflicts() {
    return 0;
}