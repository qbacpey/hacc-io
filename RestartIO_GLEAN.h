#ifndef _RESTART_IO_SINGLE_FILE
#define _RESTART_IO_SINGLE_FILE

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

#include "mpi.h"

// Needed to get this working on OSX
#ifndef   pwrite64               
#define pwrite64  pwrite
#endif

#ifndef   pread64               
#define pread64 pread           
#endif


enum GLEAN_IO_MODE
{
    WRITE_CHECKPOINT,
    READ_RESTART,
    UNDEF_MODE
};


enum GLEAN_IO_INTERFACE
{
    USE_POSIX,
    USE_SYNC_MPIIO, // Default
    USE_ASYNC_MPIIO, // Async I/O
    UNDEF_INTERFACE
};

enum GLEAN_MPIIO_HEADER_INTERFACE
{
    USE_SYNC_MPIIO_FOR_HEADER,  // Default
    USE_ASYNC_MPIIO_FOR_HEADER, // Async I/O
};

enum GLEAN_POSIX_API
{
    USE_READ_WRITE =  0,// Default
    USE_PREAD_PWRITE = 1
};

enum GLEAN_FILE_DISTRIBUTION
{
    GLEAN_SINGLE_FILE,
    GLEAN_FILE_PER_RANK,
    GLEAN_BGQ_ION_PART
};

enum GLEAN_MPIIO_FILE_PTR
{
    INDEPENDENT_FILE_PTR,
    SHARED_FILE_PTR
};


static const int GLEAN_MAX_STRING_LEN           = 8192;

// Set this to 24 MB for now
static const int64_t FILE_HEADER_SIZE_MAX		= 25165824;

static const int HEADER_METAINFO_SIZE          = 16;

class RestartIO_GLEAN
{

public:
    
    RestartIO_GLEAN ();
		
    ~RestartIO_GLEAN ();
		
    int Initialize (MPI_Comm comm);

    int Finalize (void);

    int CreateCheckpoint (char* path_prefix, int64_t& num_particles);
    
    int64_t OpenRestart (char* pathname);
	
    int Close (void);
	
    int Write ( float* xx, float* yy, float* zz,
                float* vx, float* vy, float* vz,
                float* phi, int64_t* pid,
                uint16_t* mask);

    // Return the number of particles for the current rank
    int Read (  float*& xx, float*& yy, float*& zz,
                float*& vx, float*& vy, float*& vz,
                float*& phi, int64_t*& pid,
                uint16_t*& mask);

    // Need some Get and Set Attributes to Tweak Performance
    int SetMPIIOSharedFilePointer(void);
    
    int SetMPIOIndepFilePointer (void);
    
    int SetFileDistribution (GLEAN_FILE_DISTRIBUTION file_dist);
    
    void EnablePreAllocateFile(void);
    
    void DisablePreAllocateFile(void);
    
    // read and write 
    void Set_Sync_MPI_IO_Interface(void);
    void Set_Async_MPI_IO_Interface(void);
    void Set_POSIX_IO_Interface(int val = 0);
   
    // header write and read modes
    void Set_Async_MPI_IO_Header(void);
    void Set_Sync_MPI_IO_Header(void);

    // get methods
    GLEAN_IO_INTERFACE Get_IO_Interface(void);
    
    void PrintIOCoordInfo (void);

    //! new
    void test_read(void);

    void test_write(void);
	
private:
    
    int __duplicateCommunicator (MPI_Comm comm);
    
    int __initalizePartitionInfo (void);
    
    int __createPartitions (void);
    
    int __destroyPartitions (void);
    
    #ifdef __bgq__
    int __initalize_BGQ_PartitionInfo (void);
    #endif

    // Associated with Checkpoints
    
    int __POSIX_Create (void );
    
    int __MPIIO_Create (void );
    
    
    int __POSIX_Close_Checkpoint (void);
    
    int __MPIIO_Close_Checkpoint (void);
    
    
    // Associated with Restarts
    
    int __POSIX_Read_Header (void);
    
    int __MPIIO_Read_Header (void);
    
    int __POSIX_Open_Restart (void);
    
    int __MPIIO_Open_Restart (void);
    
    int __POSIX_Close_Restart (void);
    
    int __MPIIO_Close_Restart (void);
    
    
    
    int __POSIX_Write_Data (const char *buf, int64_t* nbytes,
                            off_t start_off);
    
    int64_t __POSIX_Read_Data (unsigned char *buf, int64_t& nbytes,
                            off_t start_off);
    

    void __HandleMPIIOError (int errcode, char *str);
    
    //! new
    void  __check_read_end(void);
    
    void  __check_write_end(void);

    void __check_valid_read(void); //for async header. verifies the content of the header

		
    MPI_Comm m_globalComm; //  Global Communicator

    int m_globalCommSize; // Size of Global Communicator
		
    int m_globalCommRank; // Rank of Global Communicator
        
    int m_totPartitions; // Divide into partitions and write a file out per partition
    
    int m_partitionID; // 1D Partition ID
	
    int m_idInPartition; // Rank ID in the given partition
		
    MPI_Comm m_partitionComm;

    int m_partitionSize;

    int m_partitionRank;
    
    char* m_basePathName; // Root Path Name
    
    int m_basePathNameLen;
    
    char* m_partFileName; // File of Partition
    
    int m_partFileNameLen;
    
    int64_t m_localParticles;
    
    
    int64_t m_totPartParticles;
    
    int64_t m_totGlobalParticles;
    
    GLEAN_IO_MODE m_mode; // Current Access Mode
    
    GLEAN_IO_INTERFACE m_interface; // Which IO Interface to Use

    GLEAN_MPIIO_HEADER_INTERFACE m_header_mode; // MPIIO header Interface (sync/async) to used for read and write
    
    GLEAN_POSIX_API m_posixAPI;
    
    MPI_File m_fileHandle; // MPI File Handle
    
    GLEAN_MPIIO_FILE_PTR m_filePtrMode;
    
    MPI_Comm m_fileComm; // Shared or independent Ptr for File
    
    int m_posixFD; // POSIX Descriptor
    
    int64_t m_partFileSize; // Valid only on the Part Comm Root
    
    int64_t* m_header;

    int64_t m_headerSize;

    // Performance Related Information
    
    double m_startTime;

    double m_endTime;
    
    // Performance Tweaking Parameters
    int m_preallocFile;
    
   GLEAN_FILE_DISTRIBUTION m_fileDist;

    //! new 

    int64_t m_localParticles_verify; //used for async read header verification
    int test;
    int test_xx;
    int test_yy;
    int test_zz;
    int test_vx;
    int test_vy;
    int test_vz;
    int test_phi;
    int test_pid;
    int test_mask;
    int test_header;


    // write request need for Async I/O
    MPI_Request requests_write[9] = {
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL};
    // MPI_Request request_write_xx     = MPI_REQUEST_NULL;
    // MPI_Request request_write_yy     = MPI_REQUEST_NULL;
    // MPI_Request request_write_zz     = MPI_REQUEST_NULL;
    // MPI_Request request_write_vx     = MPI_REQUEST_NULL;
    // MPI_Request request_write_vy     = MPI_REQUEST_NULL;
    // MPI_Request request_write_vz     = MPI_REQUEST_NULL;
    // MPI_Request request_write_phi    = MPI_REQUEST_NULL;
    // MPI_Request request_write_pid    = MPI_REQUEST_NULL;
    // MPI_Request request_write_mask   = MPI_REQUEST_NULL;
    MPI_Request request_write_header = MPI_REQUEST_NULL;

    MPI_Request requests_read[9] = {
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL,
        MPI_REQUEST_NULL};
    // read request need for Async I/O
    //  MPI_Request request_read_xx     = MPI_REQUEST_NULL;
    //  MPI_Request request_read_yy     = MPI_REQUEST_NULL;
    //  MPI_Request request_read_zz     = MPI_REQUEST_NULL;
    //  MPI_Request request_read_vx     = MPI_REQUEST_NULL;
    //  MPI_Request request_read_vy     = MPI_REQUEST_NULL;
    //  MPI_Request request_read_vz     = MPI_REQUEST_NULL;
    //  MPI_Request request_read_phi    = MPI_REQUEST_NULL;
    //  MPI_Request request_read_pid    = MPI_REQUEST_NULL;
    //  MPI_Request request_read_mask   = MPI_REQUEST_NULL;
     MPI_Request request_read_header = MPI_REQUEST_NULL;

};


#endif

