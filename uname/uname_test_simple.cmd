#PBS -l nodes=2:ppn=2,walltime=0:60:00
#PBS -m abe
#PBS -M dmcr@Princeton.EDU

module load openmpi
module list
cat $PBS_NODEFILE

echo "About to call mpiexec"
cd $PBS_O_WORKDIR
mpiexec --mca btl ^openib uname_test_simple
