kernel void getDensity(global float* NW,
						global float* N,
						global float* NE,
						global float* W,
						global float* C,
						global float* E,
						global float* SW,
						global float* S,
						global float* SE,
						global float* density,
						int width, int height ) {

    const int globalx = get_global_id(0);
    const int globaly = get_global_id(1);

	const int index = globaly*width + globalx;

	if( globalx < 0 || globalx >= width || 
		globaly < 0 || globaly >= height) return;


	density[index] = ( NW[index] + N[index] + NE[index] +
					  W[index] + C[index] + E[index] + 
					  SW[index] + S[index] + SE[index] );


}

