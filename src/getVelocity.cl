kernel void getVelocity(global float* NW,
						global float* N,
						global float* NE,
						global float* W,
						global float* C,
						global float* E,
						global float* SW,
						global float* S,
						global float* SE,
						global float* vel,
						int width, int height ) {

    const int globalx = get_global_id(0);
    const int globaly = get_global_id(1);

	const int index = globaly*width + globalx;

	if( globalx < 0 || globalx >= width || 
		globaly < 0 || globaly >= height) return;


	vel[index*2    ] =  ( NE[index] - NW[index] + 
						  E[index] - W[index] +
						  SE[index] - SW[index] );

	vel[index*2 + 1] =  ( SW[index] - NW[index] +
						  S[index] - N[index] +
						  SE[index] - NE[index] );
}
