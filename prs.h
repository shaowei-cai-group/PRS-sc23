#pragma once

class light;

light* prs_init();

void prs_add(light* S, int lit);

int prs_solve(light* S);

int prs_value(light* S, int lit);

void prs_release(light* S);