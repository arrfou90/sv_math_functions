#include <pthread.h>
#include <stdio.h>

const int N = 1 << 20;
/*
__global__ void kernel(float *x, int n)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    for (int i = tid; i < n; i += blockDim.x * gridDim.x) {
        x[i] = sqrt(pow(3.14159,i));
    }
}

void *launch_kernel(void *dummy)
{
    float *data;
    cudaMalloc(&data, N * sizeof(float));

    kernel<<<1, 64>>>(data, N);

    cudaStreamSynchronize(0);

    return NULL;
}

int main()
{ 

    const int num_threads = 8;

    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, launch_kernel, 0)) {
            fprintf(stderr, "Error creating threadn");
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        if(pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error joining threadn");
            return 2;
        }
    }

    cudaDeviceReset();

    return 0;
}
*/
__global__ void kernel(float *x, int n)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    for (int i = tid; i < n; i += blockDim.x * gridDim.x) {
        x[i] = sqrt(pow(3.14159,i));
    }
}

int main()
{
    const int num_streams = 8;

    cudaStream_t streams[num_streams];
    float *data[num_streams];

    for (int i = 0; i < num_streams; i++) {
        cudaStreamCreateWithFlags (&streams[i],cudaStreamNonBlocking);

        cudaMalloc(&data[i], N * sizeof(float));

        // launch one worker kernel per stream
        kernel<<<5, 64, 0, streams[i]>>>(data[i], N);

        // launch a dummy kernel on the default stream
        kernel<<<1, 1>>>(0, 0);
    }

    cudaDeviceReset();

    return 0;
}
