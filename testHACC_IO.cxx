#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "mpi.h"
#include <iostream>

#include "RestartIO_GLEAN.h"

using namespace std;

int main (int argc, char * argv[]) 
{
	char* fname = 0;	
	char* buf = 0;
	int numtasks, myrank, status;
	MPI_File fh;
	int runs = 1;

	status = MPI_Init(&argc, &argv);
    if ( MPI_SUCCESS != status)
    {
        printf(" Error Starting the MPI Program \n");
        MPI_Abort(MPI_COMM_WORLD, status);
    }

    // volatile int di = 0;
    // char hostname[256];
    // gethostname(hostname, sizeof(hostname));
    // printf("PID %d on %s ready for attach\n", getpid(), hostname);
    // fflush(stdout);
    // while (0 == di)
    //     sleep(5);



    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	if (argc != 3)
    {
        printf (" USAGE <exec> <particles/rank>  < Full file path>  ");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    
	int64_t num_particles =  atoi(argv[1]);
	

	fname = (char*)malloc(strlen(argv[2]) +1);
	strncpy (fname, argv[2], strlen(argv[2]));
	fname[strlen(argv[2])] = '\0';
    
    // Let's Populate Some Dummy Data
	float *xx, *yy, *zz, *vx, *vy, *vz, *phi;
	int64_t* pid;
	uint16_t* mask;

	    // Let's Read Restart File Now
	float *xx_r, *yy_r, *zz_r, *vx_r, *vy_r, *vz_r, *phi_r;
	int64_t* pid_r;
	uint16_t* mask_r;
	int64_t my_particles;
    
	xx = new float[num_particles];
    yy = new float[num_particles];
    zz = new float[num_particles];
    vx = new float[num_particles];
    vy = new float[num_particles];
    vz = new float[num_particles];
    phi = new float[num_particles];
    pid = new int64_t[num_particles];
    mask = new uint16_t[num_particles];
    
    

	RestartIO_GLEAN* rst = new RestartIO_GLEAN();
	

	
	//? set interface for I/O (read & write)
	//? ******************************************
	//rst->Set_POSIX_IO_Interface(1);
	// rst->Set_POSIX_IO_Interface();

	rst->Set_Sync_MPI_IO_Interface();	
	//rst->Set_Sync_MPI_IO_Header(); //default
	//rst->Set_Async_MPI_IO_Interface();
	//rst->Set_Async_MPI_IO_Header();

	//overwrite specific read or write mode
	//rst->Set_Read_Sync_MPI_IO_Interface();


	//? set File mode
	//? ******************************************
	rst->SetMPIIOSharedFilePointer();
	//rst->SetMPIOIndepFilePointer();
	rst->SetFileDistribution (GLEAN_SINGLE_FILE);
        //rst->SetFileDistribution(GLEAN_FILE_PER_RANK);	



	rst->Initialize(MPI_COMM_WORLD);
	rst->PrintIOCoordInfo();


	for (int loop = 1; loop <= runs; loop++)
	{
	if (myrank == 0)
	cout << "\n rank 0 @ run: "<< loop << "/" << runs << endl;
	
	rst->CreateCheckpoint (fname, num_particles);


	for (uint64_t i = 0; i < num_particles; i++)
	{
		xx[i] = (float)i;
		yy[i] = (float)i;
		zz[i] = (float)i;
		vx[i] = (float)i;
		vy[i] = (float)i;
		vz[i] = (float)i;
		phi[i] = (float)i;
		pid[i] =  (int64_t)i;
		mask[i] = (uint16_t)myrank;
	}
	
	rst -> Write ( xx, yy, zz, vx, vy, vz, phi, pid, mask);
	
	rst->Close();
	
        
	my_particles  = rst->OpenRestart (fname);
    
	if (my_particles != num_particles)
	{
		cout << " Particles Counts Do NOT MATCH " <<  endl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}
    
    rst->Read( xx_r, yy_r, zz_r, vx_r, vy_r, vz_r, phi_r, pid_r, mask_r);

	rst->Close();

    // Verify The contents
	for (uint64_t i = 0; i < num_particles; i++)
	{
		if ((xx[i] != xx_r[i]) || (yy[i] != yy_r[i]) || (zz[i] != zz_r[i])
			|| (vx[i] != vx_r[i]) || (vy[i] != vy_r[i]) || (vz[i] != vz_r[i])
			|| (phi[i] != phi_r[i])|| (pid[i] != pid_r[i]) || (mask[i] != mask_r[i]))
		{
			cout << " Values Don't Match Index:" << i <<  endl;
            cout << "XX " << xx[i] << " " << xx_r[i] << " YY " << yy[i]  << " " << yy_r[i] << endl;
            cout << "ZZ " << zz[i] << " " << zz_r[i] << " VX " << vx[i]  << " " << vx_r[i] << endl;
            cout << "VY " << vy[i] << " " << vy_r[i] << " VZ " << vz[i]  << " " << vz_r[i] << endl;
            cout << "PHI " << phi[i] << " " << phi_r[i] << " PID " << pid[i]  << " " << pid_r[i] << endl;
            cout << "Mask: " << mask[i] << " " << mask_r[i] << endl;
            
			MPI_Abort (MPI_COMM_WORLD, -1);
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
    
	if (0 == myrank)
		cout << " CONTENTS VERIFIED... Success " << endl;
    

	}

	rst->Finalize();
	delete rst;
	rst = 0;
    
    // Delete the Arrays
    delete []xx;
	delete []xx_r;
	delete []yy;
	delete []yy_r;
	delete []zz;
	delete []zz_r;
	delete []vx;
	delete []vx_r;
	delete []vy;
	delete []vy_r;
	delete []vz;
	delete []vz_r;
	
	delete []phi;
	delete []phi_r;
    
	delete [] pid;
	delete [] pid_r;
    
	delete [] mask;
	delete [] mask_r;
    
	
	
	MPI_Finalize();

	return 0;
}

