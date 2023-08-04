
cd solvers/kissat-inc
make clean
./configure
make -j
cd ../..

cd preprocess/m4ri-20140914
make clean
./configure
make -j
cd ../..

make clean
make -j
