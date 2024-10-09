#ifdef _OPENMP
#include <omp.h>
#endif
void main() {
    #pragma omp parallel
    {
        int np, iam = 0;
        #ifdef _OPENMP
        np = omp_get_num_threads();
        iam = omp_get_thread_num();
        #endif
        printf(“Hello from thread %d (total %d)\n”,iam, np);
    }
}