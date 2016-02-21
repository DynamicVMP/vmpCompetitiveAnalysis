date

./imavmp experiments/12x50.vmp
echo "100 generations" >> results/pareto_set
echo "100 generations" >> results/pareto_front
./imavmp experiments/12x50.vmp
echo "200 generations" >> results/pareto_set
echo "200 generations" >> results/pareto_front
./imavmp experiments/12x50.vmp
echo "300 generations" >> results/pareto_set
echo "300 generations" >> results/pareto_front

date

cp -fr results results.12x50
make init

