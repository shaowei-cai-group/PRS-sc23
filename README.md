

### Intro

This is an api version of PRS-sc23.

### How to build

```bash
cd PRS-sc32
./build.sh
```

You can run `build.sh` to build `libprs.a`

### How to use

```cpp

#include "PRS-sc23/prs.h"

int main() {

    // 8 threads
    light* S = prs_init(8);

    prs_add(S, 1);
    prs_add(S, -2);
    prs_add(S, 0);

    prs_add(S, -1);
    prs_add(S, 2);
    prs_add(S, 0);

    int res = prs_solve(sat_solver);

    if(res == 20) {
        printf("UNSAT\n");
    } else {
        printf("SAT\n");
        for(int i=1; i<=2; i++) {
            printf("%d", prs_value(S, i));
        }
    }

    prs_release(S);

    return 0;
}

```

### How to Change Parameters

modify utils/paras.hpp and use `build.sh` to rebuild.

```
instance: input CNF 

nThreads: the number of workers in PRS

cutoff: the wall time for SAT solving

clause_sharing: whether use clause sharing (1: enable; 0: disable) 

preprocessor: whether use preprocessing (1: enable; 0: disable)
```