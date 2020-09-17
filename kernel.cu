#include <iostream>
#include <math.h>
// Kernel function to add the elements of two arrays

#define ITTERATIONS 10000

__global__ void run(float* array, int max, double zoom, double x_target, double y_target) {
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

	zoom = zoom * zoom;

	double norm_x = ((double)x / (max)) * (4 / zoom) - 2 / zoom + x_target;
	double norm_y = ((double)y / (max)) * (4 / zoom) - 2 / zoom + y_target;

	double x1 = 0, y1 = 0;

	bool diverging = false;

	for (int i = 0; i < ITTERATIONS; i++) {
		if (sqrtf(x1 * x1 + y1 * y1) >= 2.f) {
			array[x + y * max] = (i / (float)ITTERATIONS) * ((float)ITTERATIONS / 100);
			diverging = true;
			break;
		}

		double xx = (x1 * x1) - (y1 * y1) + norm_x;
		y1 = 2 * x1 * y1 + norm_y;
		x1 = xx;
	}
	if (!diverging)
		array[x + y * max] = 0.f;
}

float* arr;

void generate_image(int max, double zoom, double x_target, double y_target) {
	cudaMallocManaged(&arr, max * max * sizeof(float));

	run<<<dim3 (128, 128), dim3 (max / 128, max / 128)>>>(arr, max, zoom, x_target, y_target);
	cudaDeviceSynchronize();
}

float get_mandelbrot_at_point(int x, int y, int max) {
	return arr[x + y * max];
}