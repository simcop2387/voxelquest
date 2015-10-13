


int boolToInt(bool val) {
	if (val) {
		return 1;
	}
	else {
		return 0;
	}
}

uint* toUintPtr(char* baseAdr) {
	void* voidPtr = baseAdr;
	uint* uintPtr = (uint*)voidPtr;
	return uintPtr;
}

int* toIntPtr(char* baseAdr) {
	void* voidPtr = baseAdr;
	int* intPtr = (int*)voidPtr;
	return intPtr;
}

float* toFloatPtr(char* baseAdr) {
	void* voidPtr = baseAdr;
	float* floatPtr = (float*)voidPtr;
	return floatPtr;
}

float fract(float val) {
	return (val - floor(val));
}

int intDiv(int v, int s) {
	float fv = v;
	float fs = s;

	if (v < 0) {
		return -ceil(-fv / fs);
	}
	else {
		return v / s;
	}
}

void pack16(float num, float &outR, float &outG) {

    int iz = int(num);
    int ir = iz%256;
    int ig = iz/256;

    outR = float(ir)/255.0;
    outG = float(ig)/255.0;

}

uint zipBits(bool* boolArr, int len) {
	uint i;
	uint result = 0;
	uint oneVal = 1;
	
	for (i = 0; i < len; i++) {
		if (boolArr[i]) {
			result = result | (oneVal<<i);
		}
		
	}
	
	return result;
}

void unzipBits(uint zipped, bool* boolArr, int len) {
	uint i;
	uint zeroVal = 0;
	uint oneVal = 1;
	
	
	
	for (i = 0; i < len; i++) {
		boolArr[i] = (
			(zipped & (oneVal<<i)) > zeroVal
		);
	}
	
}


// inline uint clampChar(int baseVal, int bitShift) {
// 	int val = baseVal;
	
// 	if (val > 255) {
// 		val = 255;
// 	}
// 	if (val < 0) {
// 		val = 0;
// 	}
	
// 	uint retVal = val;
	
// 	return (retVal << bitShift);
	
// }

int clamp(int val, int min, int max) {
	if (val > max) {
		val = max;
	}
	if (val < min) {
		val = min;
	}
	return val;
}

float mixf(float v1, float v2, float lerpVal) {
	return v1*(1.0f-lerpVal) + v2*lerpVal;
}

float mixb(float v1, float v2, bool lerpVal) {
	if (lerpVal) {
		return v2;
	}
	else {
		return v1;
	}
}


float roundf(float val) {
	//return floor(val);
	if (val < 0.0) {
		return -floor(abs(val));
	}
	else {
		return floor(val);
	}
}

float clampf(float val, float min, float max) {
	if (val > max) {
		val = max;
	}
	if (val < min) {
		val = min;
	}
	return val;
}

float smoothstep(float edge0, float edge1, float x) {
    // Scale, bias and saturate x to 0..1 range
    x = clampf((x - edge0)/(edge1 - edge0), 0.0f, 1.0f); 
    // Evaluate polynomial
    return x*x*(3.0f - 2.0f*x);
}

inline float clampfZO(float val) {
	return clampf(val, 0.0f, 1.0f);
}


// these random generators are used for things that DO NOT effect networking

inline float fGenRand() {
	float intPart;
	float res = abs ( modf(sin(RAND_COUNTER*433.2351267) * 43758.8563f, &intPart) );
	RAND_COUNTER += 1.0f;
	if (RAND_COUNTER >= 8000000.0f) {
		RAND_COUNTER = 0.0f;
	}
	
	return res;
}

inline int iGenRand(int nMin, int nMax)
{
	return nMin + (int)( (fGenRand()) * (nMax + 1 - nMin));
}

// these random generators are used for things that DO effect networking

inline float fGenRand2() {
	float intPart;
	float res = abs ( modf(sin(RAND_COUNTER2*433.2351267) * 43758.8563f, &intPart) );
	RAND_COUNTER2 += 1.0f;
	if (RAND_COUNTER2 >= 8000000.0f) {
		RAND_COUNTER2 = 0.0f;
	}
	
	return res;
}

inline int iGenRand2(int nMin, int nMax)
{
	return nMin + (int)( (fGenRand2()) * (nMax + 1 - nMin));
}




float fSeedRand2(float x, float y) {
	float intPart;
	return modf(sin(x * 12.9898 + y * 78.233) * 43758.5453, &intPart);
}

int iSeedRand2(float x, float y, int minV, int maxV) {
	float res = fSeedRand2(x, y);
	return minV + max(int((maxV + 1 - minV) * res), 0);
}




unsigned int intLogB2 (unsigned int val) {
	unsigned int ret = -1;
	while (val != 0) {
		val >>= 1;
		ret++;
	}
	return ret;
}

int intPow(int x, int p) {
	int i = 1;
	for (int j = 1; j <= p; j++) {
		i *= x;
	}
	return i;
}

int wrapCoord(int val, int max) {
	while (val < 0) {
		val += max;
	}
	while (val >= max) {
		val -= max;
	}

	return val;
}



class FIVector4 {
private:
	iVector4 iv4;
	fVector4 fv4;

	iVector3 iv3;
	fVector3 fv3;

	iVector2 iv2;
	fVector2 fv2;

public:

	FIVector4() {
		iv4.x = 0;
		iv4.y = 0;
		iv4.z = 0;
		iv4.w = 0;
		fv4.x = 0.0;
		fv4.y = 0.0;
		fv4.z = 0.0;
		fv4.w = 0.0;
	}

	float operator[] (int ind) { //float&
		
		switch (ind) {
			case 0:
				return fv4.x;
			break;
			case 1:
				return fv4.y;
			break;
			case 2:
				return fv4.z;
			break;
			case 3:
				return fv4.w;
			break;
		}
		
		cout << "invalid vector index";
		return -1.0f;
	}

	void setIndex(int ind, float val) {
		
		switch (ind) {
			case 0:
				fv4.x = val;
				iv4.x = val;
			break;
			case 1:
				fv4.y = val;
				iv4.y = val;
			break;
			case 2:
				fv4.z = val;
				iv4.z = val;
			break;
			case 3:
				fv4.w = val;
				iv4.w = val;
			break;
		}
	}
	
	float getIndex(int ind) {
		
		switch (ind) {
			case 0:
				return fv4.x;
			break;
			case 1:
				return fv4.y;
			break;
			case 2:
				return fv4.z;
			break;
			case 3:
				return fv4.w;
			break;
		}
		
		return 0.0f;
	}


	void setIXYZW(int x, int y, int z, int w) {
		iv4.x = x;
		iv4.y = y;
		iv4.z = z;
		iv4.w = w;
		fv4.x = (float)x;
		fv4.y = (float)y;
		fv4.z = (float)z;
		fv4.w = (float)w;
	}
	void setIXYZ(int x, int y, int z) {
		iv4.x = x;
		iv4.y = y;
		iv4.z = z;
		fv4.x = (float)x;
		fv4.y = (float)y;
		fv4.z = (float)z;
	}
	void setIXY(int x, int y) {
		iv4.x = x;
		iv4.y = y;
		fv4.x = (float)x;
		fv4.y = (float)y;
	}
	void setIX(int x) {
		iv4.x = x;
		fv4.x = (float)x;
	}
	void setIY(int y) {
		iv4.y = y;
		fv4.y = (float)y;
	}
	void setIZ(int z) {
		iv4.z = z;
		fv4.z = (float)z;
	}
	void setIW(int w) {
		iv4.w = w;
		fv4.w = (float)w;
	}

	void copyFrom(FIVector4 *cf) {
		iv4.x = cf->getIX();
		iv4.y = cf->getIY();
		iv4.z = cf->getIZ();
		iv4.w = cf->getIW();

		fv4.x = cf->getFX();
		fv4.y = cf->getFY();
		fv4.z = cf->getFZ();
		fv4.w = cf->getFW();
	}

	void copyIntDiv(FIVector4 *cf, int val) {
		iv4.x = cf->getIX();
		iv4.y = cf->getIY();
		iv4.z = cf->getIZ();
		iv4.w = cf->getIW();

		fv4.x = cf->getFX();
		fv4.y = cf->getFY();
		fv4.z = cf->getFZ();
		fv4.w = cf->getFW();

		intDivXYZ(val);
	}

	void copyIntMult(FIVector4 *cf, int val) {
		iv4.x = cf->getIX();
		iv4.y = cf->getIY();
		iv4.z = cf->getIZ();
		iv4.w = cf->getIW();

		fv4.x = cf->getFX();
		fv4.y = cf->getFY();
		fv4.z = cf->getFZ();
		fv4.w = cf->getFW();

		multXYZ(val);
	}

	void setFloatArr(float* vals) {
		fv4.x = vals[0];
		fv4.y = vals[1];
		fv4.z = vals[2];
		iv4.x = fv4.x;
		iv4.y = fv4.y;
		iv4.z = fv4.z;
	}

	void setFXYZW(float x, float y, float z, float w) {
		fv4.x = x;
		fv4.y = y;
		fv4.z = z;
		fv4.w = w;
		iv4.x = (int)x;
		iv4.y = (int)y;
		iv4.z = (int)z;
		iv4.w = (int)w;
	}
	void setFXYZ(float x, float y, float z) {
		fv4.x = x;
		fv4.y = y;
		fv4.z = z;
		iv4.x = (int)x;
		iv4.y = (int)y;
		iv4.z = (int)z;
	}
	
	void setFXYZ(float scalar) {
		fv4.x = scalar;
		fv4.y = scalar;
		fv4.z = scalar;
		iv4.x = (int)scalar;
		iv4.y = (int)scalar;
		iv4.z = (int)scalar;
	}

	void setFXYZRef(FIVector4 *v1) {
		fv4.x = v1->getFX();
		fv4.y = v1->getFY();
		fv4.z = v1->getFZ();
		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void setFXY(float x, float y) {
		fv4.x = x;
		fv4.y = y;
		iv4.x = (int)x;
		iv4.y = (int)y;
	}
	void setFX(float x) {
		fv4.x = x;
		iv4.x = (int)x;
	}
	void setFY(float y) {
		fv4.y = y;
		iv4.y = (int)y;
	}
	void setFZ(float z) {
		fv4.z = z;
		iv4.z = (int)z;
	}
	void setFW(float w) {
		fv4.w = w;
		iv4.w = (int)w;
	}
	
	
	
	
	void fixForRot() {
		iv4.x = -iv4.x;
		iv4.y = -iv4.y;
		iv4.z = -iv4.z;
		
		iv4.x = max(iv4.x, 0);
		iv4.y = max(iv4.y, 0);
		iv4.z = max(iv4.z, 0);
		
		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;
	}
	
	void setRandNoSeed() {

		fv4.x = fGenRand();
		fv4.y = fGenRand();
		fv4.z = fGenRand();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void setRand(FIVector4 *seedPos, float addVal, float multVal) {

		// default output is -1 to 1

		FIVector4 r0;
		FIVector4 r1;
		FIVector4 r2;

		FIVector4 l0;
		FIVector4 l1;
		FIVector4 l2;

		float intPart;

		r0.setFXYZ(12.989f, 78.233f, 98.422f);
		r1.setFXYZ(93.989f, 67.345f, 54.256f);
		r2.setFXYZ(43.332f, 93.532f, 43.734f);

		l0.setFXYZ(
			seedPos->getFX(),
			seedPos->getFY(),
			seedPos->getFZ()
		);
		l1.setFXYZ(
			seedPos->getFZ(),
			seedPos->getFY(),
			seedPos->getFX()
		);
		l2.setFXYZ(
			seedPos->getFY(),
			seedPos->getFX(),
			seedPos->getFZ()
		);

		fv4.x = modf(sin(l0.dot(&r0)) * 43758.8563f, &intPart);
		fv4.y = modf(sin(l1.dot(&r1)) * 24634.6345f, &intPart);
		fv4.z = modf(sin(l2.dot(&r2)) * 56445.2345f, &intPart);
		
		
		fv4.x += addVal;
		fv4.y += addVal;
		fv4.z += addVal;
		
		fv4.x *= multVal;
		fv4.y *= multVal;
		fv4.z *= multVal;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void addXYZ(float scalar, float multiplier = 1.0f) {
		fv4.x += scalar * multiplier;
		fv4.y += scalar * multiplier;
		fv4.z += scalar * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void addW(float scalar) {
		fv4.w += scalar;
		iv4.w = (int)fv4.w;
	}
	
	void addXYZ(float scalarX, float scalarY, float scalarZ, float multiplier = 1.0f) {
		fv4.x += scalarX * multiplier;
		fv4.y += scalarY * multiplier;
		fv4.z += scalarZ * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	void addXYZRef(FIVector4 *scalar, float multiplier = 1.0f) {
		fv4.x += scalar->getFX() * multiplier;
		fv4.y += scalar->getFY() * multiplier;
		fv4.z += scalar->getFZ() * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void multXYZRef(FIVector4 *scalar) {
		fv4.x *= scalar->getFX();
		fv4.y *= scalar->getFY();
		fv4.z *= scalar->getFZ();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void lerpXYZ(FIVector4 *v2, float amount) {
		float iamount = 1.0f-amount;
		fv4.x = fv4.x*iamount + v2->getFX()*amount;
		fv4.y = fv4.y*iamount + v2->getFY()*amount;
		fv4.z = fv4.z*iamount + v2->getFZ()*amount;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void lerpXYZ(FIVector4 *v2, FIVector4* av) {
		
		float xa;
		float ya;
		float za;
		
		if (av == NULL) {
			xa = 1.0f;
			ya = 1.0f;
			za = 1.0f;
		}
		else {
			xa = (*av)[0];
			ya = (*av)[1];
			za = (*av)[2];
		}
		
		
		float ixa = 1.0f-xa;
		float iya = 1.0f-ya;
		float iza = 1.0f-za;
		fv4.x = fv4.x*ixa + v2->getFX()*xa;
		fv4.y = fv4.y*iya + v2->getFY()*ya;
		fv4.z = fv4.z*iza + v2->getFZ()*za;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void multXYZ(float scalar) {
		fv4.x *= scalar;
		fv4.y *= scalar;
		fv4.z *= scalar;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	void multXYZ(float scalarX, float scalarY, float scalarZ) {
		fv4.x *= scalarX;
		fv4.y *= scalarY;
		fv4.z *= scalarZ;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void multXYZ(FIVector4 *scalar) {
		fv4.x *= scalar->getFX();
		fv4.y *= scalar->getFY();
		fv4.z *= scalar->getFZ();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void divXYZ(FIVector4 *scalar) {
		fv4.x /= scalar->getFX();
		fv4.y /= scalar->getFY();
		fv4.z /= scalar->getFZ();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void floorXYZ() {
		fv4.x = floor(fv4.x);
		fv4.y = floor(fv4.y);
		fv4.z = floor(fv4.z);

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void intDivXYZ(int scalar) {

		iv4.x = intDiv(iv4.x, scalar);
		iv4.y = intDiv(iv4.y, scalar);
		iv4.z = intDiv(iv4.z, scalar);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}
	
	void intDivXYZ(int scalarX, int scalarY, int scalarZ) {


		iv4.x = intDiv(iv4.x, scalarX);
		iv4.y = intDiv(iv4.y, scalarY);
		iv4.z = intDiv(iv4.z, scalarZ);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}

	void modXYZ(int scalar) {
		iv4.x = iv4.x % scalar;
		iv4.y = iv4.y % scalar;
		iv4.z = iv4.z % scalar;

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}

	

	void wrapXYZ(int scalar) {
		iv4.x = wrapCoord(iv4.x, scalar);
		iv4.y = wrapCoord(iv4.y, scalar);
		iv4.z = wrapCoord(iv4.z, scalar);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}

	void wrapXY(int scalar) {
		iv4.x = wrapCoord(iv4.x, scalar);
		iv4.y = wrapCoord(iv4.y, scalar);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;

	}

	void powXYZ(float p1, float p2, float p3) {
		fv4.x = pow(fv4.x,p1);
		fv4.y = pow(fv4.y,p2);
		fv4.z = pow(fv4.z,p3);

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void absXYZ() {
		fv4.x = abs(fv4.x);
		fv4.y = abs(fv4.y);
		fv4.z = abs(fv4.z);

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void averageXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = (v1->getFX() + v2->getFX())*0.5f;
		fv4.y = (v1->getFY() + v2->getFY())*0.5f;
		fv4.z = (v1->getFZ() + v2->getFZ())*0.5f;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void averageNegXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = (v1->getFX() - v2->getFX())*0.5f;
		fv4.y = (v1->getFY() - v2->getFY())*0.5f;
		fv4.z = (v1->getFZ() - v2->getFZ())*0.5f;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void minXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = std::min(v1->getFX(), v2->getFX());
		fv4.y = std::min(v1->getFY(), v2->getFY());
		fv4.z = std::min(v1->getFZ(), v2->getFZ());

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void maxXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = std::max(v1->getFX(), v2->getFX());
		fv4.y = std::max(v1->getFY(), v2->getFY());
		fv4.z = std::max(v1->getFZ(), v2->getFZ());

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void clampXYZS(float minV, float maxV) {
		if (fv4.x < minV) {
			fv4.x = minV;
		}
		if (fv4.y < minV) {
			fv4.y = minV;
		}
		if (fv4.z < minV) {
			fv4.z = minV;
		}
		if (fv4.x > maxV) {
			fv4.x = maxV;
		}
		if (fv4.y > maxV) {
			fv4.y = maxV;
		}
		if (fv4.z > maxV) {
			fv4.z = maxV;
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void clampXYZ(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			fv4.x = minV->getFX();
		}
		if (fv4.y < minV->getFY()) {
			fv4.y = minV->getFY();
		}
		if (fv4.z < minV->getFZ()) {
			fv4.z = minV->getFZ();
		}
		if (fv4.x > maxV->getFX()) {
			fv4.x = maxV->getFX();
		}
		if (fv4.y > maxV->getFY()) {
			fv4.y = maxV->getFY();
		}
		if (fv4.z > maxV->getFZ()) {
			fv4.z = maxV->getFZ();
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void clampX(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			fv4.x = minV->getFX();
		}
		if (fv4.x > maxV->getFX()) {
			fv4.x = maxV->getFX();
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	void clampY(FIVector4 *minV, FIVector4 *maxV) {

		if (fv4.y < minV->getFY()) {
			fv4.y = minV->getFY();
		}
		if (fv4.y > maxV->getFY()) {
			fv4.y = maxV->getFY();
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	void clampZ(float minV, float maxV) {

		if (fv4.z < minV) {
			fv4.z = minV;
		}
		if (fv4.z > maxV) {
			fv4.z = maxV;
		}

		iv4.z = (int)fv4.z;
	}



	bool inBoundsIsEqualXYZ(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			return false;
		}
		if (fv4.y < minV->getFY()) {
			return false;
		}
		if (fv4.z < minV->getFZ()) {
			return false;
		}
		if (fv4.x > maxV->getFX()) {
			return false;
		}
		if (fv4.y > maxV->getFY()) {
			return false;
		}
		if (fv4.z > maxV->getFZ()) {
			return false;
		}

		return true;
	}

	static void normalizeBounds(FIVector4 *minBounds, FIVector4 *maxBounds) {
		float temp;

		if (minBounds->getFX() > maxBounds->getFX()) {
			temp = maxBounds->getFX();
			maxBounds->setFX(minBounds->getFX());
			minBounds->setFX(temp);
		}
		if (minBounds->getFY() > maxBounds->getFY()) {
			temp = maxBounds->getFY();
			maxBounds->setFY(minBounds->getFY());
			minBounds->setFY(temp);
		}
		if (minBounds->getFZ() > maxBounds->getFZ()) {
			temp = maxBounds->getFZ();
			maxBounds->setFZ(minBounds->getFZ());
			minBounds->setFZ(temp);
		}
	}


	
	static bool intersectInt(FIVector4 *aMin, FIVector4 *aMax, FIVector4 *bMin, FIVector4 *bMax) {
		
		if (aMax->getFX() <= bMin->getFX()) return false;
		if (aMin->getFX() >= bMax->getFX()) return false;
		if (aMax->getFY() <= bMin->getFY()) return false;
		if (aMin->getFY() >= bMax->getFY()) return false;
		if (aMax->getFZ() <= bMin->getFZ()) return false;
		if (aMin->getFZ() >= bMax->getFZ()) return false;
		
		return true;
		
	}
	
	

	static bool intersect(FIVector4 *aMin, FIVector4 *aMax, FIVector4 *bMin, FIVector4 *bMax) {

		float aWidth = aMax->getFX() - aMin->getFX();
		float aHeight = aMax->getFY() - aMin->getFY();
		float aDepth = aMax->getFZ() - aMin->getFZ();

		float bWidth = bMax->getFX() - bMin->getFX();
		float bHeight = bMax->getFY() - bMin->getFY();
		float bDepth = bMax->getFZ() - bMin->getFZ();

		float aWidthC = (aMax->getFX() + aMin->getFX()) / 2.0f;
		float aHeightC = (aMax->getFY() + aMin->getFY()) / 2.0f;
		float aDepthC = (aMax->getFZ() + aMin->getFZ()) / 2.0f;

		float bWidthC = (bMax->getFX() + bMin->getFX()) / 2.0f;
		float bHeightC = (bMax->getFY() + bMin->getFY()) / 2.0f;
		float bDepthC = (bMax->getFZ() + bMin->getFZ()) / 2.0f;



		return  (abs(aWidthC - bWidthC) * 2.0f <= (aWidth + bWidth)) &&
						(abs(aHeightC - bHeightC) * 2.0f <= (aHeight + bHeight)) &&
						(abs(aDepthC - bDepthC) * 2.0f <= (aDepth + bDepth));
	}

	static void growBoundary(FIVector4 *minB, FIVector4 *maxB, FIVector4 *minV, FIVector4 *maxV) {
		if (minB->getFX() > minV->getFX()) {
			minB->setFX(minV->getFX());
		}
		if (minB->getFY() > minV->getFY()) {
			minB->setFY(minV->getFY());
		}
		if (minB->getFZ() > minV->getFZ()) {
			minB->setFZ(minV->getFZ());
		}
		if (maxB->getFX() < maxV->getFX()) {
			maxB->setFX(maxV->getFX());
		}
		if (maxB->getFY() < maxV->getFY()) {
			maxB->setFY(maxV->getFY());
		}
		if (maxB->getFZ() < maxV->getFZ()) {
			maxB->setFZ(maxV->getFZ());
		}

	}
	
	bool any() {
		return (
			(fv4.x != 0.0f) ||
			(fv4.y != 0.0f) ||
			(fv4.z != 0.0f) ||
			(fv4.w != 0.0f)
		);
	}
	bool all() {
		return (
			(fv4.x != 0.0f) &&
			(fv4.y != 0.0f) &&
			(fv4.z != 0.0f) &&
			(fv4.w != 0.0f)
		);
	}


	bool inBoundsXYZ(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			return false;
		}
		if (fv4.y < minV->getFY()) {
			return false;
		}
		if (fv4.z < minV->getFZ()) {
			return false;
		}
		if (fv4.x >= maxV->getFX()) {
			return false;
		}
		if (fv4.y >= maxV->getFY()) {
			return false;
		}
		if (fv4.z >= maxV->getFZ()) {
			return false;
		}

		return true;
	}
	
	bool inBoundsXYZSlack(FIVector4 *minV, FIVector4 *maxV, float slack) {
		if (fv4.x < minV->getFX()-slack) {
			return false;
		}
		if (fv4.y < minV->getFY()-slack) {
			return false;
		}
		if (fv4.z < minV->getFZ()-slack) {
			return false;
		}
		if (fv4.x >= maxV->getFX()+slack) {
			return false;
		}
		if (fv4.y >= maxV->getFY()+slack) {
			return false;
		}
		if (fv4.z >= maxV->getFZ()+slack) {
			return false;
		}

		return true;
	}





	bool inBoundsXY(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			return false;
		}
		if (fv4.y < minV->getFY()) {
			return false;
		}
		if (fv4.x > maxV->getFX()) {
			return false;
		}
		if (fv4.y > maxV->getFY()) {
			return false;
		}

		return true;
	}

	bool iNotEqual(FIVector4 *otherVec) {
		return (
			(iv4.x != otherVec->getIX()) ||
			(iv4.y != otherVec->getIY()) ||
			(iv4.z != otherVec->getIZ()) ||
			(iv4.w != otherVec->getIW())
		);
	}
	
	bool fNotEqual4(FIVector4 *otherVec) {
		return (
			(fv4.x != otherVec->getFX()) ||
			(fv4.y != otherVec->getFY()) ||
			(fv4.z != otherVec->getFZ()) ||
			(fv4.w != otherVec->getFW())
		);
	}



	float wrapDistance(FIVector4 *otherVec, int maxPitch, bool doSet = true) {

		int i;
		int j;

		int bestI;
		int bestJ;

		float shortestDis = FLT_MAX;
		float curDis;

		for (i = -1; i <= 1; i++) {
			for (j = -1; j <= 1; j++) {

				otherVec->addXYZ(i * maxPitch, j * maxPitch, 0, 1.0f);
				curDis = distance(otherVec);
				otherVec->addXYZ(i * maxPitch, j * maxPitch, 0, -1.0f);

				if (curDis < shortestDis) {
					shortestDis = curDis;
					bestI = i;
					bestJ = j;
				}

			}
		}

		if (doSet) {
			otherVec->addXYZ(bestI * maxPitch, bestJ * maxPitch, 0);
		}


		return shortestDis;

	}

	/*

	var raw_dx = Math.abs(x2 - x1);
	var raw_dy = Math.abs(y2 - y1);

	var dx = (raw_dx < (xmax / 2)) ? raw_dx : xmax - raw_dx;
	var dy = (raw_dy < (ymax / 2)) ? raw_dy : ymax - raw_dy;

	var l2dist = Math.sqrt((dx * dx) + (dy * dy));

	*/

	void rotate(float a, int plane) {

		float xp = fv4.x;
		float yp = fv4.y;
		float zp = fv4.z;

		switch (plane) {
		case E_PLANE_XY:
			xp = fv4.x * cos(a) - fv4.y * sin(a);
			yp = fv4.y * cos(a) + fv4.x * sin(a);
			break;

		case E_PLANE_YZ:
			zp = fv4.z * cos(a) - fv4.y * sin(a);
			yp = fv4.y * cos(a) + fv4.z * sin(a);
			break;

		case E_PLANE_XZ:
			xp = fv4.x * cos(a) - fv4.z * sin(a);
			zp = fv4.z * cos(a) + fv4.x * sin(a);
			break;
		}

		fv4.x = xp;
		fv4.y = yp;
		fv4.z = zp;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;

	}

	void rotate90(int ind) {
		switch (ind) {
		case 0: // 0 deg

			break;

		case 1: // 90 deg
			setFXY(-fv4.y, fv4.x);
			break;

		case 2: // 180 deg
			setFXY(-fv4.x, -fv4.y);
			break;

		case 3: // 270 deg
			setFXY(fv4.y, -fv4.x);
			break;
		}
	}
	
	float distanceXY(FIVector4 *otherVec) {

		float dx = fv4.x - otherVec->getFX();
		float dy = fv4.y - otherVec->getFY();

		return sqrt(dx * dx + dy * dy);
	}

	float distance(FIVector4 *otherVec) {

		float dx = fv4.x - otherVec->getFX();
		float dy = fv4.y - otherVec->getFY();
		float dz = fv4.z - otherVec->getFZ();

		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	float length() {
		return sqrt(fv4.x * fv4.x + fv4.y * fv4.y + fv4.z * fv4.z);
	}
	float lengthXY() {
		return sqrt(fv4.x * fv4.x + fv4.y * fv4.y);
	}

	float manhattanDis(FIVector4 *otherVec) {

		float dx = abs(fv4.x - otherVec->getFX());
		float dy = abs(fv4.y - otherVec->getFY());
		float dz = abs(fv4.z - otherVec->getFZ());

		return max(max(dx, dy), dz);
	}

	void normalize() {
		float len = sqrt(fv4.x * fv4.x + fv4.y * fv4.y + fv4.z * fv4.z);

		fv4.x = fv4.x / len;
		fv4.y = fv4.y / len;
		fv4.z = fv4.z / len;

	}
	
	void normalizeXY() {
		float len = sqrt(fv4.x * fv4.x + fv4.y * fv4.y);
		
		fv4.x = fv4.x / len;
		fv4.y = fv4.y / len;
	}
	

	float dot(FIVector4 *otherVec) {

		return fv4.x * otherVec->getFX() +
					 fv4.y * otherVec->getFY() +
					 fv4.z * otherVec->getFZ();
	}

	static void cross(FIVector4 *outVec, FIVector4 *v1, FIVector4 *v2) {


		float x1 = v1->getFX();
		float y1 = v1->getFY();
		float z1 = v1->getFZ();

		float x2 = v2->getFX();
		float y2 = v2->getFY();
		float z2 = v2->getFZ();

		outVec->setFXYZ(
			(y1 * z2) - (y2 * z1),
			(z1 * x2) - (z2 * x1),
			(x1 * y2) - (x2 * y1)
		);

	}




	iVector4 *getIXYZW() {
		return &iv4;
	}
	fVector4 *getFXYZW() {
		return &fv4;
	}

	iVector3 *getIXYZ() {
		iv3.x = iv4.x;
		iv3.y = iv4.y;
		iv3.z = iv4.z;
		return &iv3;
	}
	fVector3 *getFXYZ() {
		fv3.x = fv4.x;
		fv3.y = fv4.y;
		fv3.z = fv4.z;
		return &fv3;
	}

	iVector2 *getIXY() {
		iv2.x = iv4.x;
		iv2.y = iv4.y;
		return &iv2;
	}
	fVector2 *getFXY() {
		fv2.x = fv4.x;
		fv2.y = fv4.y;
		return &fv2;
	}

	int getIX() {
		return iv4.x;
	}
	int getIY() {
		return iv4.y;
	}
	int getIZ() {
		return iv4.z;
	}
	int getIW() {
		return iv4.w;
	}

	float getFX() {
		return fv4.x;
	}
	float getFY() {
		return fv4.y;
	}
	float getFZ() {
		return fv4.z;
	}
	float getFW() {
		return fv4.w;
	}


};




void hsv2rgb(materialNode* matNode) {
	
	
	static FIVector4 K_HSV;
	static FIVector4 P_HSV;
	static FIVector4 C_HSV;
	static FIVector4 R_HSV;
	
	K_HSV.setFXYZW(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
	
	C_HSV.setFXYZ(matNode->h,matNode->s,matNode->l);
	
	
	P_HSV.setFXYZ(
		abs(fract(C_HSV[0] + K_HSV[0]) * 6.0f - K_HSV[3]),
		abs(fract(C_HSV[0] + K_HSV[1]) * 6.0f - K_HSV[3]),
		abs(fract(C_HSV[0] + K_HSV[2]) * 6.0f - K_HSV[3])
	);
	R_HSV.setFXYZ(
		C_HSV[2] * mixf(K_HSV[0], clampf(P_HSV[0] - K_HSV[0], 0.0f, 1.0f), C_HSV[1]),
		C_HSV[2] * mixf(K_HSV[0], clampf(P_HSV[1] - K_HSV[0], 0.0f, 1.0f), C_HSV[1]),
		C_HSV[2] * mixf(K_HSV[0], clampf(P_HSV[2] - K_HSV[0], 0.0f, 1.0f), C_HSV[1])	
	);
	
	matNode->r = R_HSV[0];
	matNode->g = R_HSV[1];
	matNode->b = R_HSV[2];
	
}


struct RotationInfo {
	float rotMatrix[16];
	FIVector4 basePoint;
	FIVector4 axisAngle;
};

struct ModUnitStruct {
	FIVector4 basePos;
	int brushAction;
	int modType;
	int radius;
};



float getRandSeededPos(float xv, float yv, float zv) {
	float intPart;		
	return abs ( modf(sin(xv*433.2 + yv*522.9 + zv*839.4) * 43758.8563f, &intPart) );
}


float getRandSeeded(FIVector4 *seedPos, FIVector4 *seedVals) {
	float intPart;		
	return abs ( modf(sin(seedPos->dot(seedVals)) * 43758.8563f, &intPart) );
}

int iGetRandSeeded(
	FIVector4 *seedPos,
	FIVector4 *seedVals,
	int minV,
	int maxV
) {
	float intPart = 0.0f;		
	int res = abs( modf(sin(seedPos->dot(seedVals)) * 43758.8563f, &intPart) )*1000000.0f;
	res = (res % (maxV + 1 - minV)) + minV;
	
	return res;
}




class AxisRotation {

public:

	float rotationMatrix[16];
	float inputMatrix[4];
	float outputMatrix[4];

	FIVector4 tempRes1;
	FIVector4 tempRes2;
	FIVector4 tempRes3;

	void doRotationOr(FIVector4 *output, FIVector4 *input, int orientationOffset)
	{
		int i;
		int j;
		int k;

		outputMatrix[0] = 0.0f;
		outputMatrix[1] = 0.0f;
		outputMatrix[2] = 0.0f;
		outputMatrix[3] = 0.0f;

		inputMatrix[0] = input->getFX();
		inputMatrix[1] = input->getFY();
		inputMatrix[2] = input->getFZ();
		inputMatrix[3] = 1.0;


		for (i = 0; i < 4; i++ ) {
			for (j = 0; j < 1; j++) {
				outputMatrix[i] = 0;
				for (k = 0; k < 4; k++) {
					outputMatrix[i] += ALL_ROT[orientationOffset+i*4+k] * inputMatrix[k];
				}
			}
		}

		output->setFXYZW(
			outputMatrix[0],
			outputMatrix[1],
			outputMatrix[2],
			outputMatrix[3]
		);

	}

	void doRotation(FIVector4 *output, FIVector4 *input, FIVector4 *axis, float angle)
	{
		int i;
		int j;
		int k;

		float u = axis->getFX();
		float v = axis->getFY();
		float w = axis->getFZ();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		rotationMatrix[0] = (u2 + (v2 + w2) * ca) / L;
		rotationMatrix[1] = (u * v * (1 - ca) - w * sqrtL * sa) / L;
		rotationMatrix[2] = (u * w * (1 - ca) + v * sqrtL * sa) / L;
		rotationMatrix[3] = 0.0f;

		rotationMatrix[4] = (u * v * (1 - ca) + w * sqrtL * sa) / L;
		rotationMatrix[5] = (v2 + (u2 + w2) * ca) / L;
		rotationMatrix[6] = (v * w * (1 - ca) - u * sqrtL * sa) / L;
		rotationMatrix[7] = 0.0f;

		rotationMatrix[8] = (u * w * (1 - ca) - v * sqrtL * sa) / L;
		rotationMatrix[9] = (v * w * (1 - ca) + u * sqrtL * sa) / L;
		rotationMatrix[10] = (w2 + (u2 + v2) * ca) / L;
		rotationMatrix[11] = 0.0f;

		rotationMatrix[12] = 0.0f;
		rotationMatrix[13] = 0.0f;
		rotationMatrix[14] = 0.0f;
		rotationMatrix[15] = 1.0f;



		outputMatrix[0] = 0.0f;
		outputMatrix[1] = 0.0f;
		outputMatrix[2] = 0.0f;
		outputMatrix[3] = 0.0f;

		inputMatrix[0] = input->getFX();
		inputMatrix[1] = input->getFY();
		inputMatrix[2] = input->getFZ();
		inputMatrix[3] = 1.0;


		for (i = 0; i < 4; i++ ) {
			for (j = 0; j < 1; j++) {
				outputMatrix[i] = 0;
				for (k = 0; k < 4; k++) {
					outputMatrix[i] += rotationMatrix[i*4+k] * inputMatrix[k];
				}
			}
		}

		output->setFXYZW(
			outputMatrix[0],
			outputMatrix[1],
			outputMatrix[2],
			outputMatrix[3]
		);

	}
	
	
	
	void buildRotMatrix(
		RotationInfo* rotInfo
	) {
		
		float u = rotInfo->axisAngle.getFX();
		float v = rotInfo->axisAngle.getFY();
		float w = rotInfo->axisAngle.getFZ();
		float angle = rotInfo->axisAngle.getFW();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		rotInfo->rotMatrix[0] = (u2 + (v2 + w2) * ca) / L;
		rotInfo->rotMatrix[1] = (u * v * (1 - ca) - w * sqrtL * sa) / L;
		rotInfo->rotMatrix[2] = (u * w * (1 - ca) + v * sqrtL * sa) / L;
		rotInfo->rotMatrix[3] = 0.0f;

		rotInfo->rotMatrix[4] = (u * v * (1 - ca) + w * sqrtL * sa) / L;
		rotInfo->rotMatrix[5] = (v2 + (u2 + w2) * ca) / L;
		rotInfo->rotMatrix[6] = (v * w * (1 - ca) - u * sqrtL * sa) / L;
		rotInfo->rotMatrix[7] = 0.0f;

		rotInfo->rotMatrix[8] = (u * w * (1 - ca) - v * sqrtL * sa) / L;
		rotInfo->rotMatrix[9] = (v * w * (1 - ca) + u * sqrtL * sa) / L;
		rotInfo->rotMatrix[10] = (w2 + (u2 + v2) * ca) / L;
		rotInfo->rotMatrix[11] = 0.0f;

		rotInfo->rotMatrix[12] = 0.0f;
		rotInfo->rotMatrix[13] = 0.0f;
		rotInfo->rotMatrix[14] = 0.0f;
		rotInfo->rotMatrix[15] = 1.0f;

	}
	
	
	void applyRotation(
		FIVector4 *output,
		FIVector4 *input,
		RotationInfo* rotInfo
	) {
		int i;
		int j;
		int k;
		int m;
		
		//tempRes3.copyFrom(baseOffset);
		//tempRes3.addXYZRef(parentOffset,-1.0f);

		float u = rotInfo->axisAngle.getFX();
		float v = rotInfo->axisAngle.getFY();
		float w = rotInfo->axisAngle.getFZ();
		float angle = rotInfo->axisAngle.getFW();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		


		for (m = 0; m < 3; m++) {
			outputMatrix[0] = 0.0f;
			outputMatrix[1] = 0.0f;
			outputMatrix[2] = 0.0f;
			outputMatrix[3] = 0.0f;

			inputMatrix[0] = input[m].getFX();// - (rotInfo->basePoint[0]);
			inputMatrix[1] = input[m].getFY();// - (rotInfo->basePoint[1]);
			inputMatrix[2] = input[m].getFZ();// - (rotInfo->basePoint[2]);
			inputMatrix[3] = 1.0;


			for (i = 0; i < 4; i++ ) {
				for (j = 0; j < 1; j++) {
					outputMatrix[i] = 0;
					for (k = 0; k < 4; k++) {
						outputMatrix[i] += rotInfo->rotMatrix[i*4+k] * inputMatrix[k];
					}
				}
			}

			output[m].setFXYZW(
				outputMatrix[0],// + (rotInfo->basePoint[0]),
				outputMatrix[1],// + (rotInfo->basePoint[1]),
				outputMatrix[2],// + (rotInfo->basePoint[2]),
				outputMatrix[3]
			);
			//output[m].normalize();
		}

		

	}
	
	
	void doRotationTBN(
		FIVector4 *output,
		FIVector4 *input,
		FIVector4 *axisAngle,
		FIVector4 *parentOffset,
		FIVector4 *baseOffset
	) {
		int i;
		int j;
		int k;
		int m;
		
		tempRes3.copyFrom(baseOffset);
		tempRes3.addXYZRef(parentOffset,-1.0f);

		float u = axisAngle->getFX();
		float v = axisAngle->getFY();
		float w = axisAngle->getFZ();
		float angle = axisAngle->getFW();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		rotationMatrix[0] = (u2 + (v2 + w2) * ca) / L;
		rotationMatrix[1] = (u * v * (1 - ca) - w * sqrtL * sa) / L;
		rotationMatrix[2] = (u * w * (1 - ca) + v * sqrtL * sa) / L;
		rotationMatrix[3] = 0.0f;

		rotationMatrix[4] = (u * v * (1 - ca) + w * sqrtL * sa) / L;
		rotationMatrix[5] = (v2 + (u2 + w2) * ca) / L;
		rotationMatrix[6] = (v * w * (1 - ca) - u * sqrtL * sa) / L;
		rotationMatrix[7] = 0.0f;

		rotationMatrix[8] = (u * w * (1 - ca) - v * sqrtL * sa) / L;
		rotationMatrix[9] = (v * w * (1 - ca) + u * sqrtL * sa) / L;
		rotationMatrix[10] = (w2 + (u2 + v2) * ca) / L;
		rotationMatrix[11] = 0.0f;

		rotationMatrix[12] = 0.0f;
		rotationMatrix[13] = 0.0f;
		rotationMatrix[14] = 0.0f;
		rotationMatrix[15] = 1.0f;



		for (m = 0; m < 3; m++) {
			outputMatrix[0] = 0.0f;
			outputMatrix[1] = 0.0f;
			outputMatrix[2] = 0.0f;
			outputMatrix[3] = 0.0f;

			inputMatrix[0] = input[m].getFX() + (tempRes3[0]);
			inputMatrix[1] = input[m].getFY() + (tempRes3[1]);
			inputMatrix[2] = input[m].getFZ() + (tempRes3[2]);
			inputMatrix[3] = 1.0;


			for (i = 0; i < 4; i++ ) {
				for (j = 0; j < 1; j++) {
					outputMatrix[i] = 0;
					for (k = 0; k < 4; k++) {
						outputMatrix[i] += rotationMatrix[i*4+k] * inputMatrix[k];
					}
				}
			}

			output[m].setFXYZW(
				outputMatrix[0] - (tempRes3[0]),
				outputMatrix[1] - (tempRes3[1]),
				outputMatrix[2] - (tempRes3[2]),
				outputMatrix[3]
			);
			output[m].normalize();
		}

		

	}
	

};
AxisRotation axisRotationInstance;


struct SphereStruct {
	FIVector4 position;
	float maxRad;
	float curRad;
	float radVel;
	float radAcc;
};

typedef int BaseObjType;


// states:
// explore
// seek
// avoid
// return

class BaseObj
{
public:
	
	//   1
	// 2   0
	//   3
	// int rotDir;
	// int minRot;
	// int maxRot;
	// int curRot;
	
	int objectType;
	int maxFrames;
	
	BaseObjType uid;
	BaseObjType parentUID;
	//map<BaseObjType, bool> containsUID;
	vector<BaseObjType> children;
	
	int isGrabbingId;
	int isGrabbedById;
	int entType;
	bool isHidden;
	bool isFalling;
	bool isJumping;
	bool isOpen;
	bool inWater;
	bool isEquipped;
	
	float bounciness;
	float friction;
	float windResistance;
	
	
	FIVector4 positionInCells;
	FIVector4 diameterInCells;
	
	FIVector4 diameterInCellsRotated;
	FIVector4 boundsMinTransInCells;
	FIVector4 boundsMaxTransInCells;
	
	FIVector4 boundsMinTransInPixels;
	FIVector4 boundsMaxTransInPixels;
	FIVector4 centerPointInPixels;
	
	FIVector4 boundsMinTransInPixelsTarg;
	FIVector4 boundsMaxTransInPixelsTarg;
	FIVector4 centerPointInPixelsTarg;
	
	FIVector4 tempVec;
	
	//0, 2, 4 - default
	
	//x+,x-,y+,y-,z+,z-
	FIVector4 orientationXYZ;
	
	
	float ang;
	FIVector4 posOffsetInPixels;
	FIVector4 vel;
	FIVector4 acc;
	
	
	
	
	BaseObj() {
		
	}
	
	void removeChild(BaseObjType _uid) {
		int i;
		
		for (i = 0; i < children.size(); i++) {
			if (children[i] == _uid) {
				children.erase(children.begin() + i);
				return;
			}
		}
	}
	
	
	
	void updateOrientation(int ox, int oy, int oz) {
		
		int orOffset = (oz*NUM_ORIENTATIONS*NUM_ORIENTATIONS + oy*NUM_ORIENTATIONS + ox)*16;
		
		orientationXYZ.setIXYZW(
			ox,
			oy,
			oz,
			orOffset
		);
		
		axisRotationInstance.doRotationOr(
			&diameterInCellsRotated,
			&diameterInCells,
			orOffset
		);
	}
	
	void updateBounds() {
		boundsMinTransInCells.copyFrom(&positionInCells);
		boundsMaxTransInCells.copyFrom(&positionInCells);
		boundsMaxTransInCells.addXYZRef(&diameterInCellsRotated);
		
		tempVec.copyFrom(&diameterInCellsRotated);
		tempVec.fixForRot();
		boundsMinTransInCells.addXYZRef(&tempVec);
		boundsMaxTransInCells.addXYZRef(&tempVec);
		
		FIVector4::normalizeBounds(&boundsMinTransInCells,&boundsMaxTransInCells);
		
	}
	
	void updatePixelBounds() {
		boundsMinTransInPixelsTarg.copyFrom(&boundsMinTransInCells);
		boundsMaxTransInPixelsTarg.copyFrom(&boundsMaxTransInCells);
		
		centerPointInPixelsTarg.averageXYZ(
			&boundsMinTransInPixelsTarg,
			&boundsMaxTransInPixelsTarg
		);
		
		boundsMinTransInPixelsTarg.addXYZRef(&posOffsetInPixels);
		boundsMaxTransInPixelsTarg.addXYZRef(&posOffsetInPixels);
		centerPointInPixelsTarg.addXYZRef(&posOffsetInPixels);
	}
	
	void updateTargets(FIVector4* fv) {
		centerPointInPixels.lerpXYZ(&centerPointInPixelsTarg, fv);
		boundsMinTransInPixels.lerpXYZ(&boundsMinTransInPixelsTarg, fv);
		boundsMaxTransInPixels.lerpXYZ(&boundsMaxTransInPixelsTarg, fv);
	}
	
	void rotate(
		int mod, // 0:+, 1:-
		int axis // 0:x, 1:y, 2:z
	) {
		
		int offset = (axis*2+mod)*6;
		
		updateOrientation(
			ROT_MAP[orientationXYZ.getIX()+offset],
			ROT_MAP[orientationXYZ.getIY()+offset],
			ROT_MAP[orientationXYZ.getIZ()+offset]
		);
	}
	
	void toggleGrav(float gravMod) {
		acc.setFXYZ(0.0f,0.0f,gravMod*80.0f);
	}
	
	void init(
		BaseObjType _uid,
		BaseObjType _parentUID,
		int _objectType,
		int _entType,
		FIVector4* cellPos,
		int xs,
		int ys,
		int zs,
		int ox = 0,
		int oy = 2,
		int oz = 4
	) {
		
		isHidden = false;
		
		ang = 0.0f;
		maxFrames = 0;
		objectType = _objectType;
		entType = _entType;
		isFalling = false;
		isJumping = false;
		isGrabbedById = -1;
		isGrabbingId = -1;
		inWater = false;
		bounciness = 0.0f;
		friction = 0.9;
		windResistance = 0.9;
		isOpen = false;
		isEquipped = false;
		parentUID = _parentUID;
		uid = _uid;
		
		
		posOffsetInPixels.setFXYZ(0.0f,0.0f,0.0f);
		vel.setFXYZ(0.0f,0.0f,0.0f);
		toggleGrav(-1.0f);
		
		positionInCells.copyFrom(cellPos);
		diameterInCells.setIXYZ(xs,ys,zs);
		updateOrientation(ox,oy,oz);
		updateBounds();
		updatePixelBounds();
		updateTargets(NULL);
	}
	
};

typedef map<BaseObjType, BaseObj>::iterator itBaseObj;

class ObjDef {
public:
	
	string classId;
	
	ObjDef() {
		
	}
	
};

class VNode {
public:
	
	int tokenIndex;
	int ruleNumber;
	std::vector<VNode*> children;
	
	VNode(int _tokenIndex) {
		tokenIndex = _tokenIndex;
	}
	
	~VNode() {
		int i;
		
		for (i = 0; i < children.size(); i++) {
			delete children[i];
		}
		children.clear();
	}
	
};

struct AssignStruct {
	VNode* lastAssign;
	VNode* newAssign;
	int tokenIndex;
	int genIndex;
};





class VBOWrapper {
public:
	GLuint vao, vbo, ibo;

	int sizeOfID;
	int sizeOfVD;
	GLfloat* vertexData;
	GLuint* indexData;

	VBOWrapper() {
		
	}
	
	void init(GLfloat* _vertexData, int _sizeOfVD, GLuint* _indexData, int _sizeOfID) {
		
		
		sizeOfID = _sizeOfID;
		sizeOfVD = _sizeOfVD;
		vertexData = _vertexData;
		indexData = _indexData;
		
		
		
		// vao and vbo handle
		
		
		// generate and bind the vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		// generate and bind the vertex buffer object
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
		
		// fill with data
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*sizeOfVD, vertexData, GL_STATIC_DRAW);
		
		
		// set up generic attrib pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (char*)0 + 4*sizeof(GLfloat));
		
		
		// generate and bind the index buffer object
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		

		// fill with data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*sizeOfID, indexData, GL_STATIC_DRAW);
		
		// "unbind" vao
		glBindVertexArray(0);
		
	
	}
	
	void draw() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, sizeOfID, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	
	
};

// class PlaneGrid {
// public:

// 	PlaneGrid() {
		
// 	}
// 	void init() {
// 		int plane_width = 4; // amount of columns
// 		int plane_height = 2; // amount of rows

// 		int total_vertices = (plane_width + 1) * (plane_height + 1);
// 		planeVert = new CIwFVec2[total_vertices];
// 		memset(planeVert, 0, sizeof(CIwFVec2) * total_vertices);

// 		int numIndPerRow = plane_width * 2 + 2;
// 		int numIndDegensReq = (plane_height - 1) * 2;
// 		int total_indices = numIndPerRow * plane_height + numIndDegensReq;

// 		planeInd = new uint16[total_indices];

// 		make_plane(plane_width, plane_height, planeVert, planeInd);

// 		...

// 		void make_plane(int width, int height, CIwFVec2 *vertices, uint16 *indices) {
			
// 			width++;
// 			height++;

// 			int size = sizeof(CIwFVec2);
// 			// Set up vertices
// 			for(int y = 0; y < height; y++)
// 			{
// 			    int base = y * width;
// 			    for(int x = 0; x < width; x++)
// 			    {
// 			        int index = base + x;
// 			        CIwFVec2 *v = vertices + index;
// 			        v->x = (float) x;
// 			        v->y = (float) y;
// 			        Debug::PrintDebug("%d: %f, %f", index, v->x, v->y);
// 			    }
// 			}

// 			Debug::PrintDebug("-------------------------");

// 			// Set up indices
// 			int i = 0;
// 			height--;
// 			for(int y = 0; y < height; y++)
// 			{
// 			    int base = y * width;

// 			    //indices[i++] = (uint16)base;
// 			    for(int x = 0; x < width; x++)
// 			    {
// 			        indices[i++] = (uint16)(base + x);
// 			        indices[i++] = (uint16)(base + width + x);
// 			    }
// 			    // add a degenerate triangle (except in a last row)
// 			    if(y < height - 1)
// 			    {
// 			        indices[i++] = (uint16)((y + 1) * width + (width - 1));
// 			        indices[i++] = (uint16)((y + 1) * width);
// 			    }
// 			}

// 			for(int ind=0; ind < i; ind++)
// 			    Debug::PrintDebug("%d ", indices[ind]);
			
// 		}
// 	}
// };




class TBOWrapper {
public:
	GLuint tbo_buf;
	GLuint tbo_tex;
	
	int dataSize;
	
	TBOWrapper() {
		
	}
	
	void init(float* tbo_data, int _dataSize) {
		
		dataSize = _dataSize;

		glGenBuffers(1, &tbo_buf);
		glBindBuffer(GL_TEXTURE_BUFFER, tbo_buf);
		glBufferData(GL_TEXTURE_BUFFER, dataSize, tbo_data, GL_STATIC_DRAW);
		glGenTextures(1, &tbo_tex);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}
	
	void update(float* tbo_data) {
		glBindBuffer(GL_TEXTURE_BUFFER, tbo_buf);
		glBufferSubData(GL_TEXTURE_BUFFER, 0, dataSize, tbo_data);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}
	
	
};

class ThreadWrapper {
private:
	bool threadRunningEx; // thread is running (exclusive, must lock)
public:
	std::thread threadMain;
	std::mutex threadMutex;
	bool threadRunning; // thread is running (not exclusive)
	
	int threadDataInt[THREAD_DATA_COUNT];
	
	ThreadWrapper() {
		
	}
	
	void init() {
		threadRunning = false;
		threadRunningEx = false;
	}
	
	void setRunningLocked(bool val) {
		threadMutex.lock();
		threadRunningEx = val;
		threadMutex.unlock();
	}
	
	bool isReady() {
		bool doProc = false;
		
		if (threadRunning) {
			if (threadMutex.try_lock()) {
				if (threadRunningEx) {
					
				}
				else {
					doProc = true;
				}
				threadMutex.unlock();
			}
		}
		else {
			doProc = true;
		}
		
		return doProc;
	}
	
};



struct PushModStruct
{
	int actionType;
	FIVector4 data[4];
};





// struct vec2 {
// 	float x;
// 	float y;
// 	float z;	
// };

// struct vec3 {
// 	float x;
// 	float y;
// 	float z;	
// };

// struct vec4 {
// 	float x;
// 	float y;
// 	float z;
// 	float w;
// };

// class ShaderSimplex() {
// public:

// 	ShaderSimplex() {
		
// 	}
	
	
// 	//#############################


// 	//
// 	// Description : Array and textureless GLSL 2D/3D/4D simplex 
// 	//               noise functions.
// 	//      Author : Ian McEwan, Ashima Arts.
// 	//  Maintainer : ijm
// 	//     Lastmod : 20110822 (ijm)
// 	//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
// 	//               Distributed under the MIT License. See LICENSE file.
// 	//               https://github.com/ashima/webgl-noise
// 	// 

// 	vec3 mod289(vec3 q) {
// 		vec3 res;
// 		q - floor(q * (1.0 / 289.0)) * 289.0;
// 		return res;
// 	}

// 	vec4 mod289(vec4 q) {
// 		vec4 res;
// 		q - floor(q * (1.0 / 289.0)) * 289.0;
// 		return res;
// 	}

// 	vec4 permute(vec4 q) {
// 		vec4 res;
// 		mod289(((q*34.0)+1.0)*q);
// 		return res;
		
// 	}

// 	vec4 taylorInvSqrt(vec4 q) {
// 		vec4 res;
// 		1.79284291400159 - 0.85373472095314 * q;
// 		return res;
// 	}

// 	float snoise(vec3 v) { 
// 		const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
// 		const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// 		// First corner
// 		vec3 i  = floor(v + dot(v, C.yyy) );
// 		vec3 x0 =   v - i + dot(i, C.xxx) ;

// 		// Other corners
// 		vec3 g = step(x0.yzx, x0.xyz);
// 		vec3 l = 1.0 - g;
// 		vec3 i1 = min( g.xyz, l.zxy );
// 		vec3 i2 = max( g.xyz, l.zxy );

// 		//   x0 = x0 - 0.0 + 0.0 * C.xxx;
// 		//   x1 = x0 - i1  + 1.0 * C.xxx;
// 		//   x2 = x0 - i2  + 2.0 * C.xxx;
// 		//   x3 = x0 - 1.0 + 3.0 * C.xxx;
// 		vec3 x1 = x0 - i1 + C.xxx;
// 		vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
// 		vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// 		// Permutations
// 		i = mod289(i); 
// 		vec4 p = permute( permute( permute( 
// 		         i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
// 		       + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
// 		       + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// 		// Gradients: 7x7 points over a square, mapped onto an octahedron.
// 		// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
// 		float n_ = 0.142857142857; // 1.0/7.0
// 		vec3  ns = n_ * D.wyz - D.xzx;

// 		vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

// 		vec4 x_ = floor(j * ns.z);
// 		vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

// 		vec4 x = x_ * ns.x + ns.yyyy;
// 		vec4 y = y_ * ns.x + ns.yyyy;
// 		vec4 h = 1.0 - abs(x) - abs(y);

// 		vec4 b0 = vec4( x.xy, y.xy );
// 		vec4 b1 = vec4( x.zw, y.zw );

// 		//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
// 		//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
// 		vec4 s0 = floor(b0)*2.0 + 1.0;
// 		vec4 s1 = floor(b1)*2.0 + 1.0;
// 		vec4 sh = -step(h, vec4(0.0));

// 		vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
// 		vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

// 		vec3 p0 = vec3(a0.xy,h.x);
// 		vec3 p1 = vec3(a0.zw,h.y);
// 		vec3 p2 = vec3(a1.xy,h.z);
// 		vec3 p3 = vec3(a1.zw,h.w);

// 		//Normalise gradients
// 		vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
// 		p0 *= norm.x;
// 		p1 *= norm.y;
// 		p2 *= norm.z;
// 		p3 *= norm.w;

// 		// Mix final noise value
// 		vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
// 		m = m * m;
// 		return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
// 	}


// 	//#############################
	
	
// };



// void opSeamlessSimplexNoise(float oct, float pers, float freq) {
	
// 	currentState = S_OP_SEAMLESS_SIMPLEX_NOISE;

// 	int i;

// 	int ix;
// 	int iy;
// 	int iz;

// 	float fx;
// 	float fy;
// 	float fz;

// 	float testVal;

// 	//float oct = 4.0;
// 	//float pers = 0.5;
// 	//float freq =- 1.0/gfTEXTURE_SIZE;

// 	float ts = gfTEXTURE_SIZE;


// 	for (i = 0; i < iTotalUnits; i++) {
// 		ix = i%(iPageSize);
// 		iy = (i%(iPageSize*iPageSize))/iPageSize;
// 		iz = i/(iPageSize*iPageSize);

// 		fx = ((float)ix) + fOffsets.x;
// 		fy = ((float)iy) + fOffsets.y;
// 		fz = ((float)iz) + fOffsets.z;

// 		//float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
// 		testVal = 
// 			(
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz) * 		(ts-fx) *		(ts-fy) *	(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz) * 		(fx) *			(ts-fy) *	(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz) * 		(ts-fx) *		(fy) *		(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz) * 		(fx) *			(fy) *		(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz-ts) * 	(ts-fx) *		(ts-fy) *	(fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz-ts) * 	(fx) *			(ts-fy) *	(fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz-ts) * 	(ts-fx) *		(fy) *		(fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz-ts) * 	(fx) *			(fy) *		(fz)
// 			)/(ts*ts*ts);

// 		fDataArray[i] = testVal;

		
// 	}

// 	currentState = E_OP_SEAMLESS_SIMPLEX_NOISE;
// }


 
