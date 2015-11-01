//##

const int numWaves = 8;

const float PI2 = 6.28318;

float speed[8] = float[](
	WAVE_SPEED*5.0,
	WAVE_SPEED*7.0,
	WAVE_SPEED*3.0,
	WAVE_SPEED*5.0,
	
	WAVE_SPEED*7.0,
	WAVE_SPEED*11.0,
	WAVE_SPEED*13.0,
	WAVE_SPEED*17.0
);
const vec2 direction[8] = vec2[](
		vec2(cos(0.000*PI2),sin(0.000*PI2)),
		vec2(cos(0.125*PI2),sin(0.125*PI2)),
		vec2(cos(0.250*PI2),sin(0.250*PI2)),
		vec2(cos(0.375*PI2),sin(0.375*PI2)),
		vec2(cos(0.500*PI2),sin(0.500*PI2)),
		vec2(cos(0.625*PI2),sin(0.625*PI2)),
		vec2(cos(0.750*PI2),sin(0.750*PI2)),
		vec2(cos(0.825*PI2),sin(0.825*PI2))
);
//##

float wave(int i, float x, float y, float speedMod) {
		float frequency = 2.0*M_PI/(wavelength[i]);
		float phase = speedMod * speed[i] * frequency;
		float theta = dot(direction[i], vec2(x, y));
		return amplitude[i] * sin(theta * frequency + curTime*timeScale * phase);
}


float waveHeight3(vec2 param, float speedMod) {

		float x = param.x;
		float y = param.y;

		float height = 0.0;
		for (int i = 0; i < numWaves; ++i) {
			height += wave(i, x, y, speedMod);
		}
		return clamp((height+1.0)*0.5,0.0,1.0);
}

float waveHeight2(vec2 param) {

		float x = param.x;
		float y = param.y;

		float height = 0.0;
		float fi;
		float fiMax = float(4);
		for (int i = 0; i < 4; ++i) {
			fi = float(i)/fiMax;
			height += wave(i, x, y, 1.0);
		}
		return height;
}

float waveHeight(vec2 param, float dv) {

		float x = param.x;
		float y = param.y;

		float height = 0.0;
		float fi;
		float fiMax = float(numWaves);
		for (int i = 0; i < numWaves; ++i) {
			fi = float(i)/fiMax;
			height += mix(wave(i, x, y, 1.0),0.0,(1.0-fi)*dv);
		}
		return height;
}

float dWavedx(int i, float x, float y) {
		float frequency = 2.0*M_PI/wavelength[i];
		float phase = speed[i] * frequency;
		float theta = dot(direction[i], vec2(x, y));
		float A = amplitude[i] * direction[i].x * frequency;
		float res = A * cos(theta * frequency + curTime*timeScale * phase);
		return res*2.0;
}

float dWavedy(int i, float x, float y) {
		float frequency = 2.0*M_PI/wavelength[i];
		float phase = speed[i] * frequency;
		float theta = dot(direction[i], vec2(x, y));
		float A = amplitude[i] * direction[i].y * frequency;
		float res =  A * cos(theta * frequency + curTime*timeScale * phase);
		return res*2.0;
}

vec3 waveNormal(vec2 param) {
		float x = param.x;
		float y = param.y;
		float dx = 0.0;
		float dy = 0.0;
		for (int i = 0; i < numWaves; ++i) {
				dx += dWavedx(i, x, y);
				dy += dWavedy(i, x, y);
		}
		vec3 n = vec3(-dx, -dy, 1.0);
		return normalize(n);
}
