/*
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 406: Project 3: Do The Wave.
 */	

/*
 * f_i(r+lamda*c_i,t) - f_i(r,t) = Omega_i*f(r,t)
 */
__kernel void update(__global float8* from, __global float8* to,
	__global int* dir, __global float8* omega){

	unsigned int x = get_global_id(0);
	unsigned int y = get_global_id(1);

	// For each row in Omega...
	for(int i = 0; i < 5; i++){
		// Start by setting to f_i(r,t)
		int new_z = dir[store(x,y,i)]/(WIDTH*DIRECTIONS);
		int new_x = dir[store(x,y,i)]%WIDTH;

		float8 new = from[store(x,y,i)];
		// and add the dot product of Omega_i and f(r,t)
		for(int j = 0; j < 5; j++){
			new += from[store(x,y,j)] * omega[i*DIRECTIONS + j];			
		}

		// Store to the out buffer.
		to[store(new_z,new_x,i)] = new;
	}
}

__kernel void heights(__global float* rbuffer, __global float8* from){

	unsigned int z = get_global_id(0);
	unsigned int x = get_global_id(1);

	float height = 0.0;

	// For each wave direction...
	for(int i = 0; i < 5; i++){
		// Sum the heights of the eight wave numbers.
		float8 f_i = from[store(z,x,i)];
		height += f_i.s0+f_i.s1+f_i.s2+f_i.s3+f_i.s4+f_i.s5+f_i.s6+f_i.s7;
	}

	// Take the average of the wave numbers.
	rbuffer[(2*x+2*WIDTH*z)*3 + 1] = height/5;
	// Don't try to double store with the first row of pixles.
	if (z != 0)rbuffer[(2*x+2*WIDTH*(z-1))*3 + 4] = height/5;
}
/*
 * N = {(y_2-y_1),2S/W,L/W(y_4-y_3)}
 */
__kernel void normals(__global float* rbuffer, __global float4* nbuffer){
	float4 normal;

	unsigned int z = get_global_id(0);
	unsigned int x = get_global_id(1);

	float y1, y2, y3, y4;

	// Deal with the edges.
	if (x > 0) y2 = rbuffer[(2*(x-1)+2*WIDTH*z)*3 + 1];
	else y2 = 0;
	if (x < WIDTH-1) y1 = rbuffer[(2*(x+1)+2*WIDTH*z)*3 + 1];
	else y1 = 0;

	if (z > 0) y4 = rbuffer[(2*x+2*WIDTH*(z-1))*3 + 1];
	else y4 = 0;
	if (z < LENGTH-1) y3 = rbuffer[(2*x+2*WIDTH*(z+1))*3 + 1];
	else y3 = 0;

	normal.x = y2-y1;
	normal.y = 2*SCALE/WIDTH;
	normal.z = LENGTH/WIDTH * (y4-y3);
	normal.w = 0.0;

	// Make the normal unit length.
	normal = normalize(normal);

	nbuffer[(z+WIDTH*x)] = normal;
}

/*
 * Ring Of Fire.
 */
__kernel void colors(__global float* rbuffer, float4 light_dir,
	__global float4* eye, __global float4* nbuffer){

	unsigned int z = get_global_id(0);
	unsigned int x = get_global_id(1);

	float4 normal = nbuffer[(z+WIDTH*x)];
	float4 wave_nec;

	// Get the wave normal in the eye coordinate system.
	wave_nec.x = dot(eye[0],normal);
	wave_nec.y = dot(eye[1],normal);
	wave_nec.z = dot(-eye[2],normal);
	wave_nec.w = 0.0;

	wave_nec = normalize(wave_nec);

	float lightness = max(0.0,dot(-wave_nec,eye[2]));

	float4 darkwater = (float4)(1.0,0.0,0.0,0.0);
	float4 lightwater = (float4)(1.0,0.5,0.0,0.0);
	float4 skycolor = (float4)(0.45,0.45,0.45,0.0);

	float4 finalcolor = (dot(wave_nec,light_dir))
		*(mix(darkwater,lightwater,lightness) + dot((float4)(1.0,1.0,1.0,0.0), skycolor)
		*pow(1-lightness,5));

	// Deal with the double storing the values in the render buffer.
	if (z < LENGTH-1) rbuffer[(2*x+2*WIDTH*z + VCOUNT)*3 + 0] = finalcolor.x;
	if (z < LENGTH-1) rbuffer[(2*x+2*WIDTH*z + VCOUNT)*3 + 1] = finalcolor.y;
	if (z < LENGTH-1) rbuffer[(2*x+2*WIDTH*z + VCOUNT)*3 + 2] = finalcolor.z;
	rbuffer[(2*x+2*WIDTH*(z-1) + VCOUNT)*3 + 3] = finalcolor.x;
	rbuffer[(2*x+2*WIDTH*(z-1) + VCOUNT)*3 + 4] = finalcolor.y;
	rbuffer[(2*x+2*WIDTH*(z-1) + VCOUNT)*3 + 5] = finalcolor.z;
}