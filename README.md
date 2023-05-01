## Compile
```
make clean; make
```


## Run

instance: input cnf
threads: number of workers
times: time limit
share: enable clause sharing (1: enable, 0: disable)
DCE: enable delay clause exchange (1: enable, 0: disable)


### default

```
./PRS $instance --clause_sharing=1 --threads=$threads --times=$times
```

### dce(delay clause exchange)

```
./PRS $instance --clause_sharing=1 --DCE=1 --threads=$threads --times=$times
```


