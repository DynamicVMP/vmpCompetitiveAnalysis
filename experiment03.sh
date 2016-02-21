./imavmp experiments/100x1000.vmp
echo "100 generations" >> results/pareto_set
echo "100 generations" >> results/pareto_front
./imavmp experiments/100x1000.vmp
echo "200 generations" >> results/pareto_set
echo "200 generations" >> results/pareto_front
./imavmp experiments/100x1000.vmp
echo "300 generations" >> results/pareto_set
echo "300 generations" >> results/pareto_front
./imavmp experiments/100x1000.vmp
echo "400 generations" >> results/pareto_set
echo "400 generations" >> results/pareto_front
./imavmp experiments/100x1000.vmp

cp -fr results results.100x1000
make init

