#!/bin/bash

export SCOREP_EXPERIMENT_DIRECTORY=ScoreP_Result
export SCOREP_ENABLE_PROFILING=1
export SCOREP_ENABLE_TRACING=1

#! perf
#! *********
#export SCOREP_METRIC_PERF=instructions:page-faults,cycles,cache-misses
#export SCOREP_METRIC_PERF=


#!  papi in scoreP
#! *****************
export SCOREP_METRIC_PAPI=
#? papi preset
#export SCOREP_METRIC_PAPI=${SCOREP_METRIC_PAPI},PAPI_FP_OPS,
#export SCOREP_METRIC_PAPI=PAPI_TOT_CYC,PAPI_FP_OPS
#?papi native
#* io
#export SCOREP_METRIC_PAPI=${SCOREP_METRIC_PAPI},io:::rchar,io:::wchar,io:::syscr,io:::syscw,io:::read_bytes,io:::write_bytes,io:::cancelled_write_bytes
#* appio
#export SCOREP_METRIC_PAPI=${SCOREP_METRIC_PAPI},appio:::READ_BYTES,appio:::READ_CALLS,appio:::READ_ERR,appio:::READ_INTERRUPTED,appio:::READ_WOULD_BLOCK,appio:::READ_SHORT,appio:::READ_EOF,appio:::READ_BLOCK_SIZE,appio:::READ_USEC,appio:::WRITE_BYTES,appio:::WRITE_CALLS,appio:::WRITE_ERR,appio:::WRITE_SHORT,appio:::WRITE_INTERRUPTED,appio:::WRITE_WOULD_BLOCK,appio:::WRITE_BLOCK_SIZE,appio:::WRITE_USEC,appio:::OPEN_CALLS,appio:::OPEN_ERR
#export SCOREP_METRIC_PAPI=${SCOREP_METRIC_PAPI},appio:::OPEN_FDS,appio:::SELECT_USEC,appio:::RECV_BYTES,appio:::RECV_CALLS,appio:::RECV_ERR,appio:::RECV_INTERRUPTED,appio:::RECV_WOULD_BLOCK,appio:::RECV_SHORT,appio:::RECV_EOF,appio:::RECV_BLOCK_SIZE,appio:::RECV_USEC,appio:::SOCK_READ_BYTES,appio:::SOCK_READ_CALLS,appio:::SOCK_READ_ERR,appio:::SOCK_READ_SHORT,appio:::SOCK_READ_WOULD_BLOCK,appio:::SOCK_READ_USEC,appio:::SOCK_WRITE_BYTES,appio:::SOCK_WRITE_CALLS
#export SCOREP_METRIC_PAPI=${SCOREP_METRIC_PAPI},appio:::SOCK_WRITE_ERR,appio:::SOCK_WRITE_SHORT,appio:::SOCK_WRITE_WOULD_BLOCK,appio:::SOCK_WRITE_USEC,appio:::SEEK_CALLS,appio:::SEEK_ABS_STRIDE_SIZE,appio:::SEEK_USEC
#* stealtime
#export SCOREP_METRIC_PAPI=${SCOREP_METRIC_PAPI},stealtime:::TOTAL,stealtime:::CPU1,stealtime:::CPU2,stealtime:::CPU3,stealtime:::CPU4,stealtime:::CPU5,stealtime:::CPU6,stealtime:::CPU7,stealtime:::CPU8,stealtime:::CPU9,stealtime:::CPU10,stealtime:::CPU11,stealtime:::CPU12,stealtime:::CPU13,stealtime:::CPU14,stealtime:::CPU15,stealtime:::CPU16,stealtime:::CPU17,stealtime:::CPU18
#* perf in papi (only few)
#export SCOREP_METRIC_PAPI=${SCOREP_METRIC_PAPI},perf::CYCLES,perf::PERF_COUNT_HW_CPU_CYCLES,perf::CACHE-MISSES,perf::BRANCHES,perf::PERF_COUNT_SW_PAGE_FAULTS,perf::L1-DCACHE-LOADS 



#! resources
#! *************
export SCOREP_METRIC_RUSAGE=all

# tell you which functions have been issuing communications. With unwinding enabled
export SCOREP_ENABLE_UNWINDING=true


