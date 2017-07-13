#include <stdio.h>
#include <mpi.h>
#include <sys/utsname.h>

int main(int argc, char* argv[])
{
	int rank, size, ret;
	struct utsname unametest;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	ret = uname(&unametest);

	printf("rank = %d, ", rank);
	printf("size = %d\n", size);

	printf("rank = %d, ", rank);
	printf("sysname = %s, ", unametest.sysname);
	printf("nodename = %s, ", unametest.nodename);
	printf("release = %s, ", unametest.release);
	printf("version = %s, ", unametest.version);
	printf("machine = %s\n", unametest.machine);

	MPI_Finalize();
}
