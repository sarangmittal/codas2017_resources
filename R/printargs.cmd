#PBS -l nodes=1:ppn=1,walltime=0:60:00
#PBS -m abe
#PBS -M dmcr@Princeton.EDU

cat $PBS_NODEFILE

echo "About to call R"
cd $PBS_O_WORKDIR
R CMD BATCH --slave "--args test1 test2=no" printargs.R

