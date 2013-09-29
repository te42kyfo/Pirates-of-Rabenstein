enum cell_type {
	FLUID = 0,
	NO_SLIP = 1,
	SRC = 2
};

enum direction {
	NW = 0,
	N = 1,
	NE = 2,
	W = 3,
	C = 4,
	E = 5,
	SE = 6,
	S = 7,
	SW = 8
};

kernel void simulationStep(int width, int height,
						   global float* srcNW,
						   global float* srcN,
						   global float* srcNE,
						   global float* srcW,
						   global float* srcC,
						   global float* srcE,
						   global float* srcSW,
						   global float* srcS,
						   global float* srcSE,
						   global float* destNW,
						   global float* destN,
						   global float* destNE,
						   global float* destW,
						   global float* destC,
						   global float* destE,
						   global float* destSW,
						   global float* destS,
						   global float* destSE,
						   global int* flag_field) {


	global float* dest[9];
	global float* src[9];
	dest[NW] = destNW;
	dest[N] = destN;
	dest[NE] = destNE;
	dest[W] = destW;
	dest[C] = destC;
	dest[E] = destE;
	dest[SW] = destSW;
	dest[S] = destS;
	dest[SE] = destSE;

	src[NW] = srcNW;
	src[N] = srcN;
	src[NE] = srcNE;
	src[W] = srcW;
	src[C] = srcC;
	src[E] = srcE;
	src[SW] = srcSW;
	src[S] = srcS;
	src[SE] = srcSE;

	

    const int globalx = get_global_id(0);
    const int globaly = get_global_id(1);

	const int index = globaly*width + globalx;

	if( globalx < 0 || globalx >= width || 
		globaly < 0 || globaly >= height ) return;

	


	int dir_indices[9];
	dir_indices[NW] = index - width - 1;
	dir_indices[N]  = index - width;
	dir_indices[NE] = index - width + 1;
	dir_indices[W]  = index         - 1;
	dir_indices[C]  = index; 
	dir_indices[E]  = index         + 1;
	dir_indices[SW] = index + width - 1;
	dir_indices[S]  = index + width;
	dir_indices[SE] = index + width + 1;

	if( flag_field[index] == NO_SLIP) return;

	const float dirs[9][2] = { { -1,-1},
							   { 0, -1},
							   { 1, -1},
							   { -1, 0},
							   { 0,  0},
							   { 1,  0},
							   { -1, 1},
							   { 0,  1},
							   { 1,  1} };
	

	int opposite[9];
	opposite[NW] = SE;
	opposite[N] = S;
	opposite[NE] = SW;
	opposite[W] = E;
	opposite[C] = C;
	opposite[E] = W;
	opposite[SW] = NE;
	opposite[S] = N;
	opposite[SE] = NW;

	float weights[9];
	weights[NW] = 1.0f/36.0f;
	weights[N] =  1.0f/9.0f;
	weights[NE] = 1.0f/36.0f;
	weights[W] =  1.0f/9.0f;
	weights[C] =  4.0f/9.0f;
	weights[E] =  1.0f/9.0f;
	weights[SW] = 1.0f/36.0f;
	weights[S] =  1.0f/9.0f;
	weights[SE] = 1.0f/36.0f;

	float ftemp[9];
	

	if( flag_field[index] == FLUID) {
		//Collide
		float rho = 0;
		
		for( size_t i = 0; i < 9; i++) {
			rho += src[i][index];
		}

		float ux = ( src[NE][index] - src[NW][index] + 
					 src[E][index] - src[W][index] + 
					 src[SE][index] - src[SW][index] );
		
		float uy = ( src[SW][index] - src[NW][index] + 
					 src[S][index] - src[N][index] + 
					 src[SE][index] - src[NE][index] );

		ux /= rho;
		uy /= rho;
		
		float usquare = ux*ux+uy*uy;
	
		float f1 = 3.0f;
		float f2 = 9.0f/2.0f;
		float f3 = 3.0f/2.0f;
		float diag = 1.0f/36.0f;
		float axis = 1.0f/9.0f;
		float center = 4.0f/9.0f;
		
		float eq[9];

		eq[NW] = 
			diag * rho * (1.0f + f1*(-ux-uy) + f2*(ux+uy)*(ux+uy) - f3* usquare);
		eq[N] = 
			axis * rho * (1.0f + f1*(-uy) + f2*uy*uy - f3* usquare);
		eq[NE] = 
			diag * rho * (1.0f + f1*(ux-uy) + f2*(ux-uy)*(ux-uy) - f3* usquare);
		eq[W] = 
			axis * rho * (1.0f + f1*(-ux) + f2*ux*ux - f3*usquare);
		eq[C] = 
			center * rho * (1.0f - f3*usquare);
		eq[E] = 
			axis * rho * (1.0f + f1*(+ux) + f2*ux*ux - f3*usquare);
		eq[SW] = 
			diag * rho * (1.0f + f1*(-ux+uy) + f2*(-ux+uy)*(-ux+uy) - f3* usquare);
		eq[S] = 
			axis * rho * (1.0f + f1*(uy) + f2*uy*uy - f3* usquare);
		eq[SE] = 
			diag * rho * (1.0f + f1*(ux+uy) + f2*(ux+uy)*(ux+uy) - f3* usquare);

		for( size_t i = 0; i < 9;i++) {
			ftemp[i] = src[i][index] - (src[i][index]-eq[i]) / 0.55;
		}
			
	} else if( flag_field[index] == SRC) { 
		for( size_t i = 0; i < 9; i++) {
			ftemp[i] = src[i][index];
		}
	}
	/*
	if( globalx == 2 && globaly == 10) {
	
		for( size_t i = 0; i < 3; i++) {
			printf("%f %f %f\n", 
				   src[i*3][index], src[i*3+1][index], src[i*3+2][index]);
		}
		printf("\n");
		for( size_t i = 0; i < 3; i++) {
			printf("%f %f %f\n", ftemp[i*3], ftemp[i*3+1], ftemp[i*3+2]);
		}
		printf("\n\n");
	
	}*/
	
	if( flag_field[index] != NO_SLIP) {

		for( size_t i = 0; i < 9; i++) {
			if( flag_field[dir_indices[i]] == FLUID) {
				dest[i][dir_indices[i]] = ftemp[i];
			} else if( flag_field[dir_indices[i]] == NO_SLIP) {
				dest[opposite[i]][index] = ftemp[i];
			}
		}
	}

	return;
}


