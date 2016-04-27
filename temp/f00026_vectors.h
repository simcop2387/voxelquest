


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

int iSign(int num) {
	if (num < 0) {
		return -1;
	}
	else {
		return 1;
	}
}

float fSign(float num) {
	if (num < 0.0f) {
		return -1.0f;
	}
	else {
		return 1.0f;
	}
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


float roundVal(float val) {
	//return floor(val);
	if (val < 0.0) {
		return -floor(abs(val)+0.5f);
	}
	else {
		return floor(val+0.5f);
	}
}


float signedFloor(float val) {
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

	btVector3 getBTV() {
		return btVector3(fv4.x,fv4.y,fv4.z);
	}
	
	void setVec3(vec3 v) {
		
		fv4.x = v.x;
		fv4.y = v.y;
		fv4.z = v.z;
		
		iv4.x = fv4.x;
		iv4.y = fv4.y;
		iv4.z = fv4.z;
		
	}
	
	vec3 getVec3() {
		return vec3(fv4.x,fv4.y,fv4.z);
	}
	ivec3 getIVec3() {
		return ivec3(iv4.x,iv4.y,iv4.z);
	}
	
	void setBTV(btVector3 myBTV) {
		setFXYZ(myBTV.getX(), myBTV.getY(), myBTV.getZ());
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

	void setFXYZRef(FIVector4 *v1, float multiplier = 1.0f) {
		fv4.x = v1->getFX()*multiplier;
		fv4.y = v1->getFY()*multiplier;
		fv4.z = v1->getFZ()*multiplier;
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
	
	void addXYZW(float scalarX, float scalarY, float scalarZ, float scalarW, float multiplier = 1.0f) {
		fv4.x += scalarX * multiplier;
		fv4.y += scalarY * multiplier;
		fv4.z += scalarZ * multiplier;
		fv4.w += scalarW * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
		iv4.w = (int)fv4.w;
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
	
	void lerpXYZW(FIVector4 *v2, float amount) {
		float iamount = 1.0f-amount;
		fv4.x = fv4.x*iamount + v2->getFX()*amount;
		fv4.y = fv4.y*iamount + v2->getFY()*amount;
		fv4.z = fv4.z*iamount + v2->getFZ()*amount;
		fv4.w = fv4.w*iamount + v2->getFW()*amount;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
		iv4.w = (int)fv4.w;
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
	
	void minXYZ(FIVector4 *v1) {
		fv4.x = std::min(v1->getFX(), fv4.x);
		fv4.y = std::min(v1->getFY(), fv4.y);
		fv4.z = std::min(v1->getFZ(), fv4.z);

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void maxXYZ(FIVector4 *v1) {
		fv4.x = std::max(v1->getFX(), fv4.x);
		fv4.y = std::max(v1->getFY(), fv4.y);
		fv4.z = std::max(v1->getFZ(), fv4.z);

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
	
	bool anyXYZ() {
		return (
			(fv4.x != 0.0f) ||
			(fv4.y != 0.0f) ||
			(fv4.z != 0.0f)
		);
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








void safeNorm(btVector3 &normRef) {
	if (normRef.fuzzyZero()) {
		
	}
	else {
		normRef.normalize();
	}
}

float getShortestAngle(float begInRad, float endInRad, float amount) {
	int begInDeg = begInRad*180/M_PI;
	int endInDeg = endInRad*180/M_PI;
	
	float shortest_angle = ((((endInDeg - begInDeg) % 360) + 540) % 360) - 180;
	
	return shortest_angle * amount * M_PI / 180.0f;
}

btVector3 roundBTV(btVector3 v) {
	return btVector3(
		roundVal(v.getX()),
		roundVal(v.getY()),
		roundVal(v.getZ())	
	);
}
btVector3 floorBTV(btVector3 v) {
	return btVector3(
		floor(v.getX()),
		floor(v.getY()),
		floor(v.getZ())	
	);
}

btVector3 multByOtherRot( btVector3 imp, btMatrix3x3 otherRot) {
	// Vector3 myRHS = Vector3(imp.getX(),imp.getY(),imp.getZ());
	// Vector3 res = otherRot*myRHS;
	
	// return btVector3(res.x,res.y,res.z);
	
	return otherRot*imp;
}

btVector3 rotBTV2D(btVector3 source, float ang) {
	float baseAng = atan2(source.getY(),source.getX());
	baseAng += ang + M_PI/2.0f;
	
	

	return -btVector3(cos(baseAng),sin(baseAng),0.0f);
}

struct SphereStruct {
	FIVector4 position;
	float maxRad;
	float curRad;
	float radVel;
	float radAcc;
};

typedef int BaseObjType;




struct SkillCard {
	
	// whenever x, do y
	
	
	
	// condition
	// subject
	// action
	// subject
	
	// gain / lose
	
	
	std::vector<int> triggers;
};

struct StatSheet {
	std::vector<int> availableSkills;
	std::vector<int> activeSkills;
	std::vector<int> statusList;
	
	int baseStats[E_CS_LENGTH];
	int unapplyedStats[E_CS_LENGTH];
	
	int curStatus[E_STATUS_LENGTH];
	int maxStatus[E_STATUS_LENGTH];
	
	int availPoints;
	
	
};




class BaseObj
{
private:
	FIVector4 centerPoint;
	FIVector4 linVelocity;
	
	bool actionStates[E_ACT_LENGTH*RLBN_LENGTH];
	
public:
	
	PathInfo targPath;
	StatSheet statSheet;
	
	int objectType;
	int maxFrames;
	
	PoseKey defaultPose;
	
	BaseObjType uid;
	BaseObjType parentUID;
	vector<BaseObjType> children;
	btVector3 startPoint;
	btVector3 skelOffset;
	
	std::vector<int> targWeaponStack;
	std::vector<BodyStruct> bodies;
	
	int actorId;
	int orgId;
	
	int contactCount;
	int isGrabbedById;
	int isGrabbedByHand;
	int entType;
	int subType;
	bool isHidden;
	bool isOpen;
	bool isEquipped;
	bool zeroZ;
	
	float bounciness;
	float friction;
	float windResistance;
	
	// skeleton
	btVector3 aabbMinSkel;
	btVector3 aabbMaxSkel;
	
	// visual objects
	btVector3 aabbMinVis;
	btVector3 aabbMaxVis;
	
	
	
	
	//////////////////
	// NPC SPECIFIC //
	//////////////////
	
	int tbDir;
	btVector3 tbPos;
	
	int swingType[4];
	int isGrabbingId[4];
	
	int hitCooldown;
	int jumpCooldown;
	
	
	float airCount;	
	float bindingPower;
	float swingCount;
	float blockCount;
	float lastBlockDis;
	
	btVector3 behaviorTarget;
	btVector3 npcRepel;
	
	
	
	//////////////////
	// END SPECIFIC //
	//////////////////
	
	bool hasAtLeast(int status, int val) {
		return (statSheet.curStatus[status] >= val);
	}
	
	void modifyStatus(int status, int modVal) {
		statSheet.curStatus[status] += modVal;
		
		
		if (statSheet.curStatus[status] < 0) {
			statSheet.curStatus[status] = 0;
		}
		if (statSheet.curStatus[status] > statSheet.maxStatus[status]) {
			statSheet.curStatus[status] = statSheet.maxStatus[status];
		}
		
	}
	
	btVector3 getUnitBounds(bool getMax) {
		
		if (bodies.size() < 1) {
			cout << "ERROR: getUnitBounds() with no bodies\n";
		}
		
		btVector3 cp = getCenterPoint( E_BDG_CENTER );
		
		float diamXY = 2.0f;
		float diamZ = 4.0f;
		
		btVector3 newRad = btVector3(diamXY*0.5f,diamXY*0.5f,diamZ*0.5f);
		
		
		switch (entType) {
			case E_ENTTYPE_NPC:
				
			break;
			default:
			
			break;
		}
		
		cp = btVector3(floor(cp.getX()),floor(cp.getY()),cp.getZ());
		
		if (getMax) {
			cp += btVector3(1.0f,1.0f,newRad.getZ());
		}
		else {
			cp -= btVector3(0.0f,0.0f,newRad.getZ());
		}
		
		return cp;
		
	}
	
	
	
	
	bool holdingWeapon(int handNum) {
		
		if (handNum == -1) {
			return (
				( isGrabbingId[RLBN_LEFT] > -1 ) ||
				( isGrabbingId[RLBN_RIGT] > -1 )
			);
		}
		else {
			return ( isGrabbingId[handNum] > -1 );
		}
		
		
	}
	
	
	void setDamping(float linear, float angular) {
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			bodies[i].body->setDamping(linear,angular);
		}
	}
	
	void clearAABB(btVector3* aabbMin, btVector3* aabbMax) {
		*aabbMin = btVector3(FLT_MAX,FLT_MAX,FLT_MAX);
		*aabbMax = btVector3(FLT_MIN,FLT_MIN,FLT_MIN);	
	}
	
	void addAABBPoint(btVector3* aabbMin, btVector3* aabbMax, btVector3 newPoint) {
		btVector3 tempv;
		
		tempv = btVector3(
			max(newPoint.getX(), aabbMax->getX()),
			max(newPoint.getY(), aabbMax->getY()),
			max(newPoint.getZ(), aabbMax->getZ())
		);
		
		*aabbMax = tempv;
		
		tempv = btVector3(
			min(newPoint.getX(), aabbMin->getX()),
			min(newPoint.getY(), aabbMin->getY()),
			min(newPoint.getZ(), aabbMin->getZ())
		);
		
		*aabbMin = tempv;
	}
	
	
	bool hasBodies() {
		return (bodies.size() > 0);
	}
	
	void multVel(int i, btVector3 velMod) {
		
		bodies[i].body->setLinearVelocity(
			bodies[i].body->getLinearVelocity() * velMod
		);
	}
	
	void multVelAng(int i, btVector3 velMod) {
		
		bodies[i].body->setAngularVelocity(
			bodies[i].body->getAngularVelocity() * velMod
		);
	}
	
	void addVel(int i, btVector3 velMod) {
		
		bodies[i].body->setLinearVelocity(
			bodies[i].body->getLinearVelocity() + velMod
		);
	}
	
	FIVector4* getVel(int i) {
		
		if (i < bodies.size()) {
			
			linVelocity.setBTV( bodies[i].body->getLinearVelocity() );
		}
		
		
		return &linVelocity;
	}
	
	BodyStruct* getBodyByBoneId(int id) {
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			if (bodies[i].boneId == id) {
				return &(bodies[i]);
			}
		}
		
		return NULL;
	}
	
	float getTotalMass() {
		int i;
		
		float tot = 0.0f;
		
		for (i = 0; i < bodies.size(); i++) {
			tot += bodies[i].mass;
		}
		
		return tot;
	}
	
	float getMarkerMass() {
		return bodies[E_BDG_CENTER].mass;
	}
	
	
	
	void wakeAll() {
		// int i;
		
		// for (i = 0; i < bodies.size(); i++) {
		// 	bodies[i].body->setActivationState(ACTIVE_TAG);
		// }
	}
	
	
	
	bool allFalling() {
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			if (bodies[i].hasContact) {
				return false;
			}
		}
		
		return true;
	}
	
	bool baseContact() {
		return bodies[E_BDG_CENTER].hasContact;
	}
	
	float getPlanarVel() {
		
		if (bodies.size() < 1) {
			return 0.0f;
		}
		
		btVector3 sourceVel = bodies[E_BDG_CENTER].body->getLinearVelocity();
		
		return sqrt( sourceVel.getX()*sourceVel.getX() + sourceVel.getY()*sourceVel.getY() );
	}
	
	void setLinVel(btVector3 newVel, int i) {
		if (i < bodies.size()) {
			bodies[i].body->setLinearVelocity(newVel);
			bodies[i].body->setActivationState(ACTIVE_TAG);
		}
	}
	
	void applyImpulses(float timeDelta, int i) {
		
		if (i < bodies.size()) {
			if (bodies[i].totAV.isZero()&&bodies[i].totLV.isZero()) {
				
			}
			else {
				
				// if (zeroZ) {
				// 	bodies[i].body->setLinearVelocity(btVector3(
				// 		bodies[i].body->getLinearVelocity().getX(),
				// 		bodies[i].body->getLinearVelocity().getY(),
				// 		0.0f	
				// 	));
				// }
				
				bodies[i].body->setAngularVelocity(bodies[i].body->getAngularVelocity() + bodies[i].totAV*timeDelta);
				bodies[i].body->applyCentralImpulse(bodies[i].totLV*timeDelta);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			
			}
		}
		
	}
	
	
	
	// void begSwing() {
	// 	targWeaponStack.push_back(E_WEAPON_STATE_BEG);
	// }
	// void endSwing() {
	// 	targWeaponStack.push_back(E_WEAPON_STATE_END);
	// 	targWeaponStack.push_back(E_WEAPON_STATE_IDLE);
	// 	//targWeaponStack.push_back(E_WEAPON_POS_RELAXED);
	// }
	
	// int getStackElem(int n) {
	// 	std::list<int>::iterator ptr;
	// 	int i;

	// 	for( i = 0 , ptr = targWeaponStack.begin() ; i < n && ptr != targWeaponStack.end() ; i++ , ptr++ );

	// 	if( ptr == targWeaponStack.end() ) {
	// 	    // list too short 
	// 	    return -1;
	// 	}
	// 	else {
	// 	    // 'ptr' points to n-th element of list
	// 	    return *ptr;
	// 	}
	// }
	
	
	btVector3 getWeaponPos(int curPos) {
		
		float newLR = 0.0f;
		float newUD = 0.0f;
		
		switch (curPos) {
			case E_WEAPON_POS_RELAXED:
				newLR = 0.5f;
				newUD = 0.25f;
			break;
			case E_WEAPON_POS_LEFT:
				newLR = 0.0f;
				newUD = 0.5f;
			break;
			case E_WEAPON_POS_RIGHT:
				newLR = 1.0f;
				newUD = 0.5f;
			break;
			case E_WEAPON_POS_UP:
				newLR = 0.5f;
				newUD = 0.0f;
			break;
			case E_WEAPON_POS_DOWN:
				newLR = 0.5f;
				newUD = 1.0f;
			break;
			
			case E_WEAPON_POS_UP_LEFT:
				newLR = 0.0f;
				newUD = 0.25f;
			break;
			case E_WEAPON_POS_UP_RIGHT:
				newLR = 1.0f;
				newUD = 0.25f;
			break;
			case E_WEAPON_POS_DOWN_LEFT:
				newLR = 0.0f;
				newUD = 0.75f;
			break;
			case E_WEAPON_POS_DOWN_RIGHT:
				newLR = 1.0f;
				newUD = 0.75f;
			break;
			case E_WEAPON_POS_FORWARD:
				newLR = 0.5f;
				newUD = 0.5f;
			break;
			
		}
		
		return btVector3(newLR,newUD,0.0f);
	}
	
	void setGrabbedBy(int newId, int handNum) {
		int i;
		
		isGrabbedByHand = handNum;
		isGrabbedById = newId;
		
		int heldUID = newId;
		
		if (heldUID < 0) {
			heldUID = -3;
		}
		
		for (i = 0; i < bodies.size(); i++) {
			bodies[i].body->heldByUID = heldUID;
		}
	}
	
	// void updateWeaponTargs(double curStepTime) {
		
		
	// 	totWeaponTime += curStepTime;
		
		
	// 	if (totWeaponTime >= 1.0) {
	// 		totWeaponTime = 1.0;
			
	// 		if (targWeaponStack.size() > 1) {
	// 			//targWeaponStack.pop_front();
	// 			targWeaponStack.erase(targWeaponStack.begin() + 0);
	// 			totWeaponTime = 0.0f;
	// 		}
	// 	}
		
	// 	int curStep = E_WEAPON_STATE_IDLE;
	// 	int nextStep = E_WEAPON_STATE_IDLE;
		
	// 	if (targWeaponStack.size() > 0) {
	// 		curStep = targWeaponStack[0];
	// 	}
		
	// 	if (targWeaponStack.size() > 1) {
	// 		nextStep = targWeaponStack[1];
	// 	}
	// 	else {
	// 		nextStep = curStep;
	// 	}
		
		
		
		
		
		
	// 	float lerpTime = totWeaponTime;
		
	// 	btVector3 res0;
	// 	btVector3 res1;
		
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_IDLE) &&
	// 		(nextStep == E_WEAPON_STATE_IDLE)	
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 		res1 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 	}
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_BEG) &&
	// 		(nextStep == E_WEAPON_STATE_BEG)
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 		res1 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 	}
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_IDLE) &&
	// 		(nextStep == E_WEAPON_STATE_BEG)	
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 		res1 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 	}
		
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_BEG) &&
	// 		(nextStep == E_WEAPON_STATE_END)	
	// 	) {
	// 		if (totWeaponTime < 0.5f) {
	// 			res0 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 			res1 = getWeaponPos(E_WEAPON_POS_FORWARD);
	// 			lerpTime = totWeaponTime*2.0f;
	// 		}
	// 		else {
	// 			res0 = getWeaponPos(E_WEAPON_POS_FORWARD);
	// 			res1 = getWeaponPos(E_WEAPON_POS_RIGHT);
	// 			lerpTime = (totWeaponTime-0.5f)*2.0f;
	// 		}
	// 	}
	
	// 	if (
	// 		(curStep == E_WEAPON_STATE_END) &&
	// 		(nextStep == E_WEAPON_STATE_IDLE)	
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 		res1 = getWeaponPos(E_WEAPON_POS_RIGHT);
	// 	}
		
		
		
	// 	lrBounds = mixf(res0.getX(), res1.getX(), lerpTime);
	// 	udBounds = mixf(res0.getY(), res1.getY(), lerpTime);
		
		
	// }
	
	
	// void updateWeapon(
	// 	int handNum,
	// 	btVector3 weaponBeg,
	// 	btVector3 weaponEnd,
	// 	double curStepTime,
	// 	// float lrBounds,
	// 	// float udBounds,
	// 	float weaponLen
	// ) {
		
	// 	totTime += curStepTime;
		
	// 	//updateWeaponTargs(curStepTime);
		
		
		
		
	// 	float myMat[16];
	// 	Matrix4 myMatrix4;
	// 	Vector3 myVector0;
	// 	Vector3 myVector1;
	// 	Vector3 normVec;
	// 	Vector4 resVector0;
	// 	Vector4 resVector1;
		
	// 	Vector4 vf0;
	// 	Vector4 vf1;
		
	// 	btVector3 basePos;
	// 	float rad0 = 1.0f;
	// 	float rad1 = rad0 + weaponLen;
		
	// 	//float lrBounds = sin(totTime/4.0);
	// 	//float udBounds = sin(totTime);
	// 	//float udBounds2 = udBounds;//sin(totTime/8.0);
		
	// 	if (bodies.size() < 1) {
	// 		return;
	// 	}
		
		
	// 	// float weaponTheta = M_PI_2 + lrBounds*M_PI_8;
	// 	// float weaponPhi = M_PI_4 + udBounds*M_PI_4;
		
	// 	// float weaponTheta2 = (1.0f - lrBounds)*M_PI + cos(totTime/2.0f)*0.1f;
	// 	// float weaponPhi2 = 0 + udBounds*M_PI_2*1.5f + sin(totTime/3.0f)*0.1f;
		
		
	// 	bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
	// 	myMatrix4 = Matrix4(myMat);
		
	// 	// myVector0 = Vector3(
	// 	// 	cos(weaponTheta)*sin(weaponPhi)*rad0,
	// 	// 	sin(weaponTheta)*sin(weaponPhi)*rad0 + 0.5f,
	// 	// 	cos(weaponPhi)*rad0 + (1.0f-udBounds2)*0.75f
	// 	// );
	// 	// myVector1 = Vector3(
	// 	// 	cos(weaponTheta2)*sin(weaponPhi2)*rad1,
	// 	// 	sin(weaponTheta2)*sin(weaponPhi2)*rad1 + 0.5f,
	// 	// 	cos(weaponPhi2)*rad1
	// 	// );
		
	// 	// myVector0.x -= (myVector0.x*0.5f + myVector1.x*0.5f)*0.25f;
	// 	// myVector0.y -= (myVector0.y*0.5f + myVector0.y*0.5f)*0.25f;
		
	// 	// myVector0 *= 0.75f;
		
	// 	// myVector0.y += 0.25f;
		
	// 	// //if (myVector1.x > 0.0f) {
	// 	// 	myVector0.x += myVector1.x*0.25f;
	// 	// //}
		
	// 	// myVector1.y += 1.0f-abs(cos(weaponPhi2));
		
	// 	BodyStruct* handBody;
		
	// 	if (handNum == RLBN_LEFT) {
	// 		handBody = getBodyByBoneId(getCorrectedName(E_BONE_L_METACARPALS));
	// 	}
	// 	else {
	// 		handBody = getBodyByBoneId(getCorrectedName(E_BONE_R_METACARPALS));
	// 	}
		
		
		
	// 	btVector3 handCenter = handBody->body->getCenterOfMassPosition();
		
	// 	myVector0 = Vector3(weaponBeg.getX(), weaponBeg.getY(),weaponBeg.getZ());
	// 	myVector1 = Vector3(weaponEnd.getX(), weaponEnd.getY(),weaponEnd.getZ());
		
		
	// 	normVec = myVector1 - myVector0;
	// 	normVec.normalize();
	// 	normVec = normVec*(rad1-rad0);
	// 	myVector1 = myVector0 + normVec;
		
	// 	rightHandTop = true;//(myVector0.x < 0.0f);
		
		
		
	// 	vf0 = Vector4(myVector0.x, myVector0.y, myVector0.z, 1.0f);
	// 	vf1 = Vector4(myVector1.x, myVector1.y, myVector1.z, 1.0f);
		
	// 	resVector0 = myMatrix4*vf0;
	// 	resVector1 = myMatrix4*vf1;
		
	// 	weaponVec0[handNum] = btVector3(resVector0.x,resVector0.y,resVector0.z);
	// 	weaponVec1[handNum] = btVector3(resVector1.x,resVector1.y,resVector1.z);
		
	// 	btVector3 weapDif = handCenter-weaponVec0[handNum];
		
	// 	weaponVec0[handNum] += weapDif;
	// 	weaponVec1[handNum] += weapDif;
		
		
	// 	vf0 = Vector4( 1.0f,0.0f,0.0f,1.0f);
	// 	vf1 = Vector4(-1.0f,0.0f,0.0f,1.0f);
		
	// 	resVector0 = myMatrix4*vf0;
	// 	resVector1 = myMatrix4*vf1;
		
	// 	rightVec = btVector3(resVector0.x,resVector0.y,resVector0.z);
	// 	leftVec = btVector3(resVector1.x,resVector1.y,resVector1.z);
		
		
		
		
		
		
	// }
	
	
	void flushImpulses() {
		
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			bodies[i].totAV = btVector3(0.0f,0.0f,0.0f);
			bodies[i].totLV = btVector3(0.0f,0.0f,0.0f);
		}
		
		
	}
	
	float turnTowardsTargAng( float targAng ) {
		btVector3 curVec = bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()*btVector3(0.0f,1.0f,0.0f);
		curVec.setZ(0.0f);
		curVec.normalize();
		
		float curAng = atan2(curVec.getY(),curVec.getX());
		
		return getShortestAngle(curAng,targAng,1.0f);
	}
	
	float turnTowardsPointDelta(btVector3 targPoint) {
		btVector3 centerPoint = getCenterPoint(E_BDG_CENTER);
		
		btVector3 targVec = targPoint-centerPoint;
		targVec.setZ(0.0f);
		targVec.normalize();
		
		float targAng = atan2(targVec.getY(),targVec.getX());
		
		return turnTowardsTargAng(targAng);
		
		
		//return targAng-curAng;
		
		// btQuaternion quat;
		
		// if ( abs(vectorA.dot(vectorB)) == 1.0f ) {
		// 	// todo: handle (anti)parallel case
		// 	//curJoint->pivotAxis = btVector3(0.0f,0.0f,0.0f);
		// 	//curJoint->quat = btQuaternion(btVector3(0.0f,0.0f,1.0f), 0.0f);
		// }
		// else {
		// 	axis = (vectorA.cross(vectorB)).normalized();
		// 	angle = btAcos(vectorA.dot(vectorB)) / (vectorA.length() * vectorB.length());
		// 	quat = btQuaternion(axis, angle);
		// 	transform.setRotation(curJoint->quat);
		// 	//curJoint->pivotAxis = axis;
		// }
	}
	
	void applyAngularImpulse(btVector3 newAV, bool delayed, int i) {
		
		
		if (i < bodies.size()) {
			if (delayed) {
				bodies[i].totAV += newAV;
			}
			else {
				
				bodies[i].body->setAngularVelocity(bodies[i].body->getAngularVelocity() + newAV);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			}

		}
		
	}
	
	void applyImpulse(btVector3 imp, bool delayed, int i) {
		if (i < bodies.size()) {
			
			if (delayed) {
				bodies[i].totLV += imp;
			}
			else {
			
			
				bodies[i].body->applyCentralImpulse(imp);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			}
		}
		
	}
	
	void applyImpulseOtherRot(btVector3 imp, btMatrix3x3 otherRot, bool delayed, int i) {
		
		//Vector3 myRHS = Vector3(imp.getX(),imp.getY(),imp.getZ());
		//Vector3 res = otherRot*myRHS;
		btVector3 newImp = otherRot*imp;//btVector3(res.x,res.y,res.z);
		
		if (i < bodies.size()) {
			if (delayed) {
				bodies[i].totLV += newImp;
			}
			else {
				
				bodies[i].body->applyCentralImpulse(newImp);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			}
		}
		
		
	}
	
	bool isHumanoid() {
		return (
			(entType == E_ENTTYPE_NPC)
		);
	}
	
	// void makeWalk(btVector3 imp, btMatrix3x3 otherRot) {
		
	// 	if (isHumanoid()) {
			
	// 	}
	// 	else {
	// 		return;
	// 	}
		
		
	// 	bool lfDown = bodies[E_BDG_LFOOT].hasContact && 
	// 		(bodies[E_BDG_LFOOT].body->getLinearVelocity().length() < 0.1f);
	// 	bool rfDown = bodies[E_BDG_RFOOT].hasContact && 
	// 		(bodies[E_BDG_RFOOT].body->getLinearVelocity().length() < 0.1f);
		
	// 	int resInd = -1;
		
	// 	if (lfDown && rfDown) {
	// 		leftActive = !leftActive;
	// 		//resInd = E_BDG_LFOOT;
			
	// 		if (leftActive) {
	// 			resInd = E_BDG_LFOOT;
	// 		}
	// 		else {
	// 			resInd = E_BDG_RFOOT;
	// 		}
			
	// 	}
	// 	// else {
	// 	// 	if (lfDown) {
	// 	// 		resInd = E_BDG_LFOOT;
	// 	// 	}
	// 	// 	else {
	// 	// 		if (rfDown) {
	// 	// 			resInd = E_BDG_RFOOT;
	// 	// 		}
	// 	// 	}
	// 	// }
		
	// 	if (resInd == -1) {
	// 		return;
	// 	}
		
		
	// 	btVector3 newImp = otherRot*imp*bodies[resInd].mass;
		
	// 	bodies[resInd].body->applyCentralImpulse(newImp);
	// 	bodies[resInd].body->setActivationState(ACTIVE_TAG);
		
		
	// }
	
	
	
	void moveOffset(btVector3 offset, int ind) {
		btTransform trans;
		
		
		if (ind < bodies.size()) {
			
			trans.setIdentity();
			trans.setOrigin(
				bodies[ind].body->getCenterOfMassPosition() + offset
			);
			bodies[ind].body->setActivationState(ACTIVE_TAG);
			bodies[ind].body->setCenterOfMassTransform(
				trans
			);
			
		}
	}
	
	void moveToPoint(btVector3 newPoint, int ind) {
		btTransform trans;
		
		
		if (ind < bodies.size()) {
			
			trans.setIdentity();
			trans.setOrigin(newPoint);
			bodies[ind].body->setActivationState(ACTIVE_TAG);
			bodies[ind].body->setCenterOfMassTransform(
				trans
			);
			
		}
	}
	
	// void setCenterPoint(FIVector4* newPos) {
		
	// 	centerPoint.copyFrom(newPos);
				
	// }
	
	btVector3 getCenterPoint(int ind) {
		if (
			ind < bodies.size()
		) {
			return bodies[ind].body->getCenterOfMassPosition();
		}
		else {
			return btVector3(0.0f,0.0f,0.0f);
		}
	}
	
	FIVector4* getCenterPointFIV(int ind) {
		if (
			ind < bodies.size()
		) {
			centerPoint.setBTV( bodies[ind].body->getCenterOfMassPosition() );
		}
		else {
			centerPoint.setFXYZ(0.0f,0.0f,0.0f);
		}
		
		return &centerPoint;
	}
	
	
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
	
	
	float healthPerc() {
		return ((float)statSheet.curStatus[E_STATUS_HEALTH])/((float)statSheet.maxStatus[E_STATUS_HEALTH]);
	}
	
	bool isDead() {
		return (statSheet.curStatus[E_STATUS_HEALTH] <= 0);
	}
	
	bool isAlive() {
		return (statSheet.curStatus[E_STATUS_HEALTH] > 0);
	}
	
	bool getActionState(int action, int handNum) {
		return actionStates[action*RLBN_LENGTH + handNum];
	}
	
	void setActionState(int action, int handNum, bool newVal) {
		actionStates[action*RLBN_LENGTH + handNum] = newVal;
	}
	
	void clearActionStates() {
		int i;
		
		for (i = 0; i < E_ACT_LENGTH*RLBN_LENGTH; i++) {
			actionStates[i] = false;
		}
		
	}
	
	
	void init(
		BaseObjType _uid,
		BaseObjType _parentUID,
		int _objectType,
		int _entType,
		int _subType,
		FIVector4* cellPos
	) {
		
		int i;
		
		contactCount = 0;
		
		//mass = 10.0f;
		orgId = -1;
		actorId = -1;
		isHidden = false;
		
		maxFrames = 0;
		objectType = _objectType;
		entType = _entType;
		subType = _subType;
		
		behaviorTarget = btVector3(0.0f,0.0f,0.0f);
		npcRepel = btVector3(0.0f,0.0f,0.0f);
		
		isGrabbedByHand = -1;
		isGrabbedById = -1;
		
		lastBlockDis = 0.0f;
		blockCount = 0.0f;
		swingCount = 0.0f;
		bindingPower = 1.0f;
		airCount = 0.0f;
		
		tbDir = 0;
		
		for (i = 0; i < RLBN_LENGTH; i++) {
			isGrabbingId[i] = -1;
			swingType[i] = E_PG_SLSH;
		}
		
		for (i = 0; i < E_CS_LENGTH; i++) {
			statSheet.baseStats[i] = 5;
			statSheet.unapplyedStats[i] = 5;	
		}
		statSheet.availPoints = 10;
		
		for (i = 0; i < E_STATUS_LENGTH; i++) {
			statSheet.curStatus[i] = 10;
			statSheet.maxStatus[i] = 10;
		}
		
		
		zeroZ = false;
		jumpCooldown = 0;
		hitCooldown = 0;
		
		
		clearActionStates();
		
		
		isOpen = false;
		isEquipped = false;
		parentUID = _parentUID;
		uid = _uid;
		
		skelOffset = btVector3(0.0f,0.0f,0.0f);
		startPoint = cellPos->getBTV();
		
		bounciness = 0.0f;
		friction = 0.9;
		windResistance = 0.9;
		
		
		//tbPos = getUnitBounds(false);
		
		targPath.points[0] = btVector3(0.0f,0.0f,0.0f);
		targPath.points[1] = btVector3(0.0f,0.0f,0.0f);
		targPath.searchedForPath = false;
		targPath.didFindPath = false;
		targPath.finalPoints.clear();
		targPath.nextInd = -1;
		
		
	}
	
};

typedef map<BaseObjType, BaseObj>::iterator itBaseObj;


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



inline float qSign(float x) {return (x >= 0.0f) ? +1.0f : -1.0f;}
inline float qNorm(float a, float b, float c, float d) {return sqrt(a * a + b * b + c * c + d * d);}


btQuaternion matToQuat(
	float r11, float r12, float r13,
	float r21, float r22, float r23,
	float r31, float r32, float r33 	
) {
	float q0 = ( r11 + r22 + r33 + 1.0f) / 4.0f;
	float q1 = ( r11 - r22 - r33 + 1.0f) / 4.0f;
	float q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
	float q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
	if(q0 < 0.0f) q0 = 0.0f;
	if(q1 < 0.0f) q1 = 0.0f;
	if(q2 < 0.0f) q2 = 0.0f;
	if(q3 < 0.0f) q3 = 0.0f;
	q0 = sqrt(q0);
	q1 = sqrt(q1);
	q2 = sqrt(q2);
	q3 = sqrt(q3);
	if(q0 >= q1 && q0 >= q2 && q0 >= q3) {
	    q0 *= +1.0f;
	    q1 *= qSign(r32 - r23);
	    q2 *= qSign(r13 - r31);
	    q3 *= qSign(r21 - r12);
	} else if(q1 >= q0 && q1 >= q2 && q1 >= q3) {
	    q0 *= qSign(r32 - r23);
	    q1 *= +1.0f;
	    q2 *= qSign(r21 + r12);
	    q3 *= qSign(r13 + r31);
	} else if(q2 >= q0 && q2 >= q1 && q2 >= q3) {
	    q0 *= qSign(r13 - r31);
	    q1 *= qSign(r21 + r12);
	    q2 *= +1.0f;
	    q3 *= qSign(r32 + r23);
	} else if(q3 >= q0 && q3 >= q1 && q3 >= q2) {
	    q0 *= qSign(r21 - r12);
	    q1 *= qSign(r31 + r13);
	    q2 *= qSign(r32 + r23);
	    q3 *= +1.0f;
	} else {
	    cout << "Quaternion error\n";
	}
	float r = qNorm(q0, q1, q2, q3);
	q0 /= r;
	q1 /= r;
	q2 /= r;
	q3 /= r;
	
	return btQuaternion(q0,q1,q2,q3);
}

btVector3 quatToEulerXYZ(const btQuaternion &quat) {
	btVector3 euler;
	float w=quat.getW();   float x=quat.getX();   float y=quat.getY();   float z=quat.getZ();
	double sqw = w*w; double sqx = x*x; double sqy = y*y; double sqz = z*z; 
	euler.setZ((atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))));
	euler.setX((atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))));
	euler.setY((asin(-2.0 * (x*z - y*w))));
	
	return euler;
}


void moveToOrientation(
	btRigidBody* myBody,
	btVector3 tanAxis,
	btVector3 bitAxis,
	btVector3 norAxis,
	float kv = 0.5f
) {
	btQuaternion targetOrientation = matToQuat(
		tanAxis.getX(),	bitAxis.getX(), norAxis.getX(),
		tanAxis.getX(),	bitAxis.getX(), norAxis.getX(),
		tanAxis.getX(),	bitAxis.getX(), norAxis.getX()
	);
	btQuaternion currentOrientation = myBody->getOrientation();
	btQuaternion deltaOrientation = targetOrientation * currentOrientation.inverse();
	btVector3 deltaEuler = quatToEulerXYZ(deltaOrientation);
	btVector3 torqueToApply = (-1.0f*kv)*deltaEuler;
	myBody->applyTorque(torqueToApply);
	
}


class VBOWrapper {
private:
	
	
	void updateBase(
		GLfloat* _vertexData,
		int _sizeOfVD,
		GLuint* _indexData,
		int _sizeOfID
	) {
		
		sizeOfVD = _sizeOfVD;
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*_sizeOfVD, _vertexData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		
		if (_indexData != NULL) {
			sizeOfID = _sizeOfID;
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*_sizeOfID, _indexData);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		
		
	}
	
	void updateNewBase(
		GLfloat* _vertexData,
		int _sizeOfVD,
		GLuint* _indexData,
		int _sizeOfID
	) {
		
		sizeOfVD = _sizeOfVD;
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_sizeOfVD, _vertexData, drawEnum);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*_sizeOfVD, _vertexData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		
		if (_indexData != NULL) {
			sizeOfID = _sizeOfID;
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*_sizeOfID, _indexData, drawEnum);
			//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*_sizeOfID, _indexData);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	
	
	
	void initBase(
		GLfloat* _vertexData,
		int _sizeOfVD,
		int _maxSizeOfVD,
		GLuint* _indexData,
		int _sizeOfID,
		int _maxSizeOfID
	) {
		
		if (numVecs == -1) {
			cout << "DID NOT INIT numVecs!\n";
			return;
		}
		
		
		
		sizeOfID = _sizeOfID;
		maxSizeOfID = _maxSizeOfID;
		
		sizeOfVD = _sizeOfVD;
		maxSizeOfVD = _maxSizeOfVD;
		
		// vertexData = _vertexData;
		// indexData = _indexData;
		
		
		int i;
		
		attSize = numVecs*4;
		
		// a value of zero means that its tightly packed
		int strideVal = attSize*sizeof(GLfloat);
		
		
		// vao and vbo handle
		
		
		// generate and bind the vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		// generate and bind the vertex buffer object
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
		
		if (_vertexData != NULL) {
			// fill with data
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*maxSizeOfVD, _vertexData, drawEnum);
		}
		
		
		
		for (i = 0; i < numVecs; i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, strideVal, (char*)0 + i*4*sizeof(GLfloat));
		}
		
		
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// generate and bind the index buffer object
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		
		if (_indexData != NULL) {
			
			
			// fill with data
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*maxSizeOfID, _indexData, drawEnum);
			
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		
		
		// "unbind" vao
		glBindVertexArray(0);
		
		hasInit = true;
	
	}
	
	
	
	
public:
	GLuint vao, vbo, ibo;

	float lastVMUsage;
	int drawEnum;
	int sizeOfID;
	int maxSizeOfID;
	int sizeOfVD;
	int maxSizeOfVD;
	int numVecs;
	int attSize;
	bool hasInit;
	
	int procCount;

	VIStruct* vi;

	bool viIsShared;

	VBOWrapper() {
		vi = new VIStruct();
		viIsShared = false;
		lastVMUsage = 0.0f;
		ibo = 0;
		vbo = 0;
		numVecs = -1;
		hasInit = false;
		procCount = 0;
	}
	
	
	void remVI() {
		if (vi == NULL) {
			
		}
		else {
			
			if (viIsShared) {
				// manually delete
			}
			else {
				delete vi;
				vi = NULL;
			}
			
		}
	}
	
	~VBOWrapper() {
		remVI();
	}
	
	void setVI(VIStruct* _vi, bool _viIsShared) {
		remVI();
		viIsShared = _viIsShared;
		vi = _vi;
	}
	
	int getNumVerts() {
		return (vi->vertexVec.size()/(numVecs*4));
	}
	
	void init(
		int _numVecs, // number of 4 component vecs
		int _drawEnum //GL_DYNAMIC_DRAW GL_STATIC_DRAW	
	) {
		
		numVecs = _numVecs;
		drawEnum = _drawEnum;
		
	}
	
	void deallocVBO() {
		
		if (ibo != 0) {
			glDeleteBuffers(1,&ibo);
		}
		
		if (vbo != 0) {
			glDeleteBuffers(1,&vbo);
		}
		
		ibo = 0;
		vbo = 0;
		numVecs = -1;
		
		hasInit = false;
	}
	
	void clearVecs(bool shrinkToFit = true) {
				
		vi->vertexVec.clear();
		vi->indexVec.clear();
		
		if (shrinkToFit) {
			vi->vertexVec.shrink_to_fit();
			vi->indexVec.shrink_to_fit();	
		}
		
	}
	
	void checkInit() {
		if (hasInit) {
			
		}
		else {
			if (
				(vi->vertexVec.size() > 0) &&
				(vi->indexVec.size() > 0)	
			) {
				initBase(
					&(vi->vertexVec[0]),
					vi->vertexVec.size(),
					vi->vertexVec.size(),
					&(vi->indexVec[0]),
					vi->indexVec.size(),
					vi->indexVec.size()
				);
			}
			else {
				if (vi->vertexVec.size() > 0) {
					initBase(
						&(vi->vertexVec[0]),
						vi->vertexVec.size(),
						vi->vertexVec.size(),
						NULL,
						0,
						0
					);
				}
				else {
					initBase(
						NULL,
						0,
						0,
						NULL,
						0,
						0
					);
				}
			}
		}
	}
	
	void update() {
		
		checkInit();
		
		GLfloat* vertexPtr = NULL;
		GLuint* indexPtr = NULL;
		if (vi->vertexVec.size() > 0) {
			vertexPtr = &(vi->vertexVec[0]);
		}
		if (vi->indexVec.size() > 0) {
			indexPtr = &(vi->indexVec[0]);
		}
		
		updateBase(
			vertexPtr,
			vi->vertexVec.size(),
			indexPtr,
			vi->indexVec.size()
		);
	}
	void updateNew() {
		
		checkInit();
		
		GLfloat* vertexPtr = NULL;
		GLuint* indexPtr = NULL;
		if (vi->vertexVec.size() > 0) {
			vertexPtr = &(vi->vertexVec[0]);
		}
		if (vi->indexVec.size() > 0) {
			indexPtr = &(vi->indexVec[0]);
		}
		
		updateNewBase(
			vertexPtr,
			vi->vertexVec.size(),
			indexPtr,
			vi->indexVec.size()
		);
	}
	
	void beginFill() {
		procCount = 0;
		vi->vertexVec.clear();
		vi->indexVec.clear();
		
	}
	void endFill() {
		
		
		
		if (vi->vertexVec.size() > 0) {
			updateNew();
		}
		else {
			// todo: handle case where vertex buffer has gone to zero
		}
		
		VERTEX_MEM_USAGE -= lastVMUsage;
		float vertMem = (vi->vertexVec.size()+vi->indexVec.size())*4;
		VERTEX_MEM_USAGE += vertMem/(1024.0f*1024.0f);
		lastVMUsage = vertMem/(1024.0f*1024.0f);
		
	}
	
	
	void draw() {
		
		if (!hasInit) {
			cout << "NOT INIT!\n";
			return;
		}
		
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, sizeOfID, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	
	void drawCubes(int numCubes) {
		
		if (!hasInit) {
			cout << "NOT INIT!\n";
			return;
		}
		
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, numCubes*24, GL_UNSIGNED_INT, 0); //sizeOfID
		glBindVertexArray(0);
	}
	
	void drawPoints() {
		
		if (!hasInit) {
			cout << "NOT INIT!\n";
			return;
		}
		
		// glBindVertexArray(vao);
		// glDrawArrays(GL_POINTS, 0, sizeOfVD/8);
		// glBindVertexArray(0);
		
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, sizeOfVD/attSize);
		glBindVertexArray(0);
		
	}
	
	
	
	
	
	// inline void getIndVal(int procCount) {
	// 	vi->indexVec.push_back(0+procCount*4);
	// 	vi->indexVec.push_back(1+procCount*4);
	// 	vi->indexVec.push_back(2+procCount*4);
	// 	vi->indexVec.push_back(2+procCount*4);
	// 	vi->indexVec.push_back(1+procCount*4);
	// 	vi->indexVec.push_back(3+procCount*4);
	// }
	
	
	
	// inline void getIndVal2(int procCount) {
	// 	vi->indexVec.push_back(2+procCount*4);
	// 	vi->indexVec.push_back(1+procCount*4);
	// 	vi->indexVec.push_back(0+procCount*4);
	// 	vi->indexVec.push_back(3+procCount*4);
	// 	vi->indexVec.push_back(1+procCount*4);
	// 	vi->indexVec.push_back(2+procCount*4);
	// }
	
	inline void getPixVal(
		//FBOWrapper* fbow0,
		//FBOWrapper* fbow1,
		//int ind,
		float xb, float yb, float zb,
		float xm, float ym, float zm,
		float* data,
		int dataLen
	) {
		//int maskInd = xm + ym*2 + zm*4;
		
		// vi->vertexVec.push_back(xb+xm+NET_MASKS[mv[maskInd]].getX());
		// vi->vertexVec.push_back(yb+ym+NET_MASKS[mv[maskInd]].getY());
		// vi->vertexVec.push_back(zb+zm+NET_MASKS[mv[maskInd]].getZ());
		// vi->vertexVec.push_back(1.0f);
		
		vi->vertexVec.push_back(xb+xm);
		vi->vertexVec.push_back(yb+ym);
		vi->vertexVec.push_back(zb+zm);
		vi->vertexVec.push_back(1.0f);
		
		
		// vi->vertexVec.push_back(xb+xm);
		// vi->vertexVec.push_back(yb+ym);
		// vi->vertexVec.push_back(zb+zm);
		
		int i;
		
		for (i = 0; i < dataLen; i++) {
			vi->vertexVec.push_back(data[i]);
		}
		
		
		
		
		
		// glMultiTexCoord3f(
		// 	GL_TEXTURE0,
		// 	xb+xm,
		// 	yb+ym,
		// 	zb+zm
		// );
		// glMultiTexCoord4f(
		// 	GL_TEXTURE1,
		// 	fbow1->getPixelAtIndex(ind,R_CHANNEL)/255.0f,
		// 	fbow1->getPixelAtIndex(ind,G_CHANNEL)/255.0f,
		// 	fbow1->getPixelAtIndex(ind,B_CHANNEL)/255.0f,
			
		// 	fbow0->getPixelAtIndex(ind,B_CHANNEL) +
		// 	fbow0->getPixelAtIndex(ind,A_CHANNEL)*256
			
		// );
		
		//glVertex3f(xb+xm,yb+ym,zb+zm);
	}
	
	void vboBox(
		float bpX,
		float bpY,
		float bpZ,
		
		float iv0,
		float iv1,
		
		uint procFlag,
		
		float* data,
		int dataLen
	) {
		
		
		// counter clockwise == front face
		
		//         z  
		//  4 5     . x
		//  6 7     y
		            
		//    0 1
		//    2 3

		getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0,data,dataLen); // 0
		getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0,data,dataLen); // 1
		getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0,data,dataLen); // 2
		getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0,data,dataLen); // 3
		
		getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1,data,dataLen); // 4
		getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1,data,dataLen); // 5
		getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1,data,dataLen); // 6
		getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1,data,dataLen); // 7
		
		
		
		if (procFlags[0]&procFlag) { // x+
			
			vi->indexVec.push_back(5+procCount);
			vi->indexVec.push_back(1+procCount);
			vi->indexVec.push_back(3+procCount);
			vi->indexVec.push_back(3+procCount);
			vi->indexVec.push_back(7+procCount);
			vi->indexVec.push_back(5+procCount);
			
			
			
		}
		if (procFlags[1]&procFlag) { // x-
			
			vi->indexVec.push_back(6+procCount);
			vi->indexVec.push_back(2+procCount);
			vi->indexVec.push_back(0+procCount);
			vi->indexVec.push_back(0+procCount);
			vi->indexVec.push_back(4+procCount);
			vi->indexVec.push_back(6+procCount);
			
			
		}
		

		
		
		if (procFlags[2]&procFlag) { // y+
			
			vi->indexVec.push_back(7+procCount);
			vi->indexVec.push_back(3+procCount);
			vi->indexVec.push_back(2+procCount);
			vi->indexVec.push_back(2+procCount);
			vi->indexVec.push_back(6+procCount);
			vi->indexVec.push_back(7+procCount);
			
			
		}
		if (procFlags[3]&procFlag) { // y-
			
			vi->indexVec.push_back(4+procCount);
			vi->indexVec.push_back(0+procCount);
			vi->indexVec.push_back(1+procCount);
			vi->indexVec.push_back(1+procCount);
			vi->indexVec.push_back(5+procCount);
			vi->indexVec.push_back(4+procCount);
			
		}
		if (procFlags[4]&procFlag) { // z+
			
			vi->indexVec.push_back(4+procCount);
			vi->indexVec.push_back(5+procCount);
			vi->indexVec.push_back(7+procCount);
			vi->indexVec.push_back(7+procCount);
			vi->indexVec.push_back(6+procCount);
			vi->indexVec.push_back(4+procCount);
			
		}
		if (procFlags[5]&procFlag) { // z-
			
			vi->indexVec.push_back(3+procCount);
			vi->indexVec.push_back(1+procCount);
			vi->indexVec.push_back(0+procCount);
			vi->indexVec.push_back(0+procCount);
			vi->indexVec.push_back(2+procCount);
			vi->indexVec.push_back(3+procCount);
			
		}
		
		
		procCount += 8;
		
		
		
		// if (procFlags[0]&procFlag) { // x+
			
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0,data,dataLen);
			
		// 	getIndVal(procCount);
		// 	procCount++;
			
			
		// }
		// if (procFlags[1]&procFlag) { // x-
			
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0,data,dataLen);
			
		// 	getIndVal2(procCount);
		// 	procCount++;
			
		// }
		// if (procFlags[2]&procFlag) { // y+
			
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0,data,dataLen);
			
		// 	getIndVal2(procCount);
		// 	procCount++;
			
		// }
		// if (procFlags[3]&procFlag) { // y-
			
			
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0,data,dataLen);
			
		// 	getIndVal(procCount);
		// 	procCount++;
		// }
		// if (procFlags[4]&procFlag) { // z+
			
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1,data,dataLen);
			
		// 	getIndVal(procCount);
		// 	procCount++;
		// }
		// if (procFlags[5]&procFlag) { // z-
			
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0,data,dataLen);
		// 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0,data,dataLen);
			
		// 	getIndVal2(procCount);
		// 	procCount++;
		// }
	}

	
	
	
	
	
	
	
	
	
	
	
	
};


class VBOGrid {
public:

	int xpitch;
	int ypitch;
	
	int totVerts;
	int totInd;

	VBOWrapper vboWrapper;



	VBOGrid() {
		
	}
	
	void init(
		int _xpitch,
		int _ypitch
	) {
		
		xpitch = _xpitch+1;
		ypitch = _ypitch+1;
		
		totVerts = xpitch*ypitch;
		totInd = 6*totVerts;
		
		int i;
		int j;
		
		float fxp = xpitch-2;
		float fyp = ypitch;
		
		float fi;
		float fj;
		
		
		
		for (j = 0; j < ypitch; j++) {
			fj = j;
			for (i = 0; i < xpitch; i++) {
				fi = i;
				vboWrapper.vi->vertexVec.push_back(i);
				vboWrapper.vi->vertexVec.push_back(j);
				vboWrapper.vi->vertexVec.push_back(1.0f);
				vboWrapper.vi->vertexVec.push_back(0.0f);
				
				
				
				vboWrapper.vi->vertexVec.push_back(fi/fxp);
				vboWrapper.vi->vertexVec.push_back(fj/fyp);
				vboWrapper.vi->vertexVec.push_back(0.0f);
				vboWrapper.vi->vertexVec.push_back(0.0f);
				
			}
		}
		
		int ind0;
		int ind1;
		int ind2;
		int ind3;
		
		int ip1;
		int jp1;
		
		for (j = 0; j < (ypitch-1); j++) {
			
			for (i = 0; i < (xpitch-1); i++) {
				
				ip1 = i+1;
				jp1 = j+1;
				
				// if (ip1 == xpitch) {
				// 	ip1 = 0;
				// }
				// if (jp1 == ypitch) {
				// 	jp1 = 0;
				// }
				
				ind0 = (i) + (j)*xpitch;
				ind1 = (ip1) + (j)*xpitch;
				ind2 = (i) + (jp1)*xpitch;
				ind3 = (ip1) + (jp1)*xpitch;
				
				// 0 1
				// 2 3
				
				vboWrapper.vi->indexVec.push_back(ind0);
				vboWrapper.vi->indexVec.push_back(ind1);
				vboWrapper.vi->indexVec.push_back(ind3);
				
				vboWrapper.vi->indexVec.push_back(ind3);
				vboWrapper.vi->indexVec.push_back(ind2);
				vboWrapper.vi->indexVec.push_back(ind0);
				
				
			}
		}
		
		
		vboWrapper.init(
			2,
			GL_STATIC_DRAW
		);
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
	
	bool isFloat;
	
	
	TBOWrapper() {
		
	}
	
	void init(bool _isFloat, float* tbo_data, uint* tbo_data2, int _dataSize) {
		
		isFloat = _isFloat;
		
		dataSize = _dataSize;

		glGenBuffers(1, &tbo_buf);
		glBindBuffer(GL_TEXTURE_BUFFER, tbo_buf);
		
		if (isFloat) {
			glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tbo_buf);
			glBufferData(GL_TEXTURE_BUFFER, dataSize, tbo_data, GL_DYNAMIC_DRAW); // todo: dynamic draw? //GL_STATIC_DRAW
			
		}
		else {
			glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, tbo_buf);		
			glBufferData(GL_TEXTURE_BUFFER, dataSize, tbo_data2, GL_DYNAMIC_DRAW); // todo: dynamic draw? //GL_STATIC_DRAW
			
		}
		
		glGenTextures(1, &tbo_tex);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}
	
	void update(float* tbo_data, uint* tbo_data2, int newDataSize) {
		
		int tempDataSize;
		
		if (newDataSize < 0) {
			tempDataSize = dataSize;
		}
		else {
			tempDataSize = newDataSize;
		}
		
		
		tempDataSize = min(tempDataSize,dataSize);
		
		
		
		glBindBuffer(GL_TEXTURE_BUFFER, tbo_buf);
		if (isFloat) {
			glBufferSubData(GL_TEXTURE_BUFFER, 0, tempDataSize, tbo_data);
		}
		else {
			glBufferSubData(GL_TEXTURE_BUFFER, 0, tempDataSize, tbo_data2);
		}
		
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
	string threadDataString[THREAD_DATA_COUNT];
	
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







uint E_OCT_XP = 1;
uint E_OCT_XM = 2;
uint E_OCT_YP = 4;
uint E_OCT_YM = 8;
uint E_OCT_ZP = 16;
uint E_OCT_ZM = 32;
uint E_OCT_VISITED = 64;
uint E_OCT_SOLID = 128;
uint E_OCT_SURFACE = 256;
uint E_OCT_NOTNEW = 512;


struct OctNode {
	int parent;
	int children[8];
	uint flags;
	
	int x;
	int y;
	int z;	
	
	OctNode() {
		
	}
	
	void init(int _parent) {
		int i;
		
		parent = _parent;
		flags = 0;
		
		for (i = 0; i < 8; i++) {
			children[i] = -1;
		}
	}
	
};

const static uint CUBE_DATA_INVALID = 0xCCCCCCCC;
const static int CUBE_DATA_SIZE = CELLS_PER_HOLDER*CELLS_PER_HOLDER*CELLS_PER_HOLDER;
const static int CUBE_WRAP_ENTRIES = 4;
//const static int CUBE_WRAP_SIZE = VOXELS_PER_CELL*VOXELS_PER_CELL*3*CUBE_WRAP_ENTRIES;
const static uint CUBE_WRAP_INVALID = 1024;

// slice 0: yz
// slice 1: xz
// slice 2: xy

// struct CubeWrap {
	
// 	// data layout:
// 	// yz plane - x up
// 	// xz plane - y up
// 	// xy plane - z up
	
// 	// offset 0: min depth
// 	// offset 1: max depth
	
// 	uint data[CUBE_WRAP_SIZE];
	
// 	void insertValue(ivec3* loc, uint flags) { //, vec3* val) {
// 		int indYZ = (loc->y + loc->z*VOXELS_PER_CELL + 0*VOXELS_PER_CELL*VOXELS_PER_CELL)*CUBE_WRAP_ENTRIES;
// 		int indXZ = (loc->x + loc->z*VOXELS_PER_CELL + 1*VOXELS_PER_CELL*VOXELS_PER_CELL)*CUBE_WRAP_ENTRIES;
// 		int indXY = (loc->x + loc->y*VOXELS_PER_CELL + 2*VOXELS_PER_CELL*VOXELS_PER_CELL)*CUBE_WRAP_ENTRIES;
		
// 		int voxMax = VOXELS_PER_CELL-1;
// 		int voxMin = 0;
		
// 		if ((flags&E_OCT_XP) > 0) { // air x plus
// 			if (
// 				(loc->x < data[indYZ+1])
// 				// || (data[indYZ+1] == CUBE_WRAP_INVALID)
// 			) { // max
// 				data[indYZ+1] = loc->x;
// 			}
			
// 		}
// 		if ((flags&E_OCT_XM) > 0) { // air x minus
// 			if (
// 				(loc->x > data[indYZ+0])
// 				// || (data[indYZ+0] == CUBE_WRAP_INVALID)
// 			) { // min
// 				data[indYZ+0] = loc->x;
// 			}
// 		}
		
		
// 		if ((flags&E_OCT_YP) > 0) { // air y plus
// 			if (
// 				(loc->y < data[indXZ+1])
// 				// || (data[indXZ+1] == CUBE_WRAP_INVALID)
// 			) { // max
// 				data[indXZ+1] = loc->y;
// 			}
// 		}
// 		if ((flags&E_OCT_YM) > 0) { // air y minus
// 			if (
// 				(loc->y > data[indXZ+0])
// 				// || (data[indXZ+0] == CUBE_WRAP_INVALID)
// 			) { // min
// 				data[indXZ+0] = loc->y;
// 			}
// 		}
		
		
// 		if ((flags&E_OCT_ZP) > 0) { // air z plus
// 			if (
// 				(loc->z < data[indXY+1])
// 				// || (data[indXY+1] == CUBE_WRAP_INVALID)
// 			) { // max
// 				data[indXY+1] = loc->z;
// 			}
// 		}
// 		if ((flags&E_OCT_ZM) > 0) { // air z minus
// 			if (
// 				(loc->z > data[indXY+0])
// 				// || (data[indXY+0] == CUBE_WRAP_INVALID)
// 			) { // min
// 				data[indXY+0] = loc->z;
// 			}
// 		}
		
		
		
		
		
		
		
		
// 		// if ((flags&E_OCT_XP) > 0) { // air x plus
// 		// 	if (loc->x > data[indYZ+0]) { // min
// 		// 		data[indYZ+0] = loc->x;
// 		// 	}
// 		// 	if (loc->x < data[indYZ+1]) { // max
// 		// 		data[indYZ+1] = loc->x;
// 		// 	}
// 		// }
// 		// if ((flags&E_OCT_XM) > 0) { // air x minus
// 		// 	if (loc->x > data[indYZ+0]) { // min
// 		// 		data[indYZ+0] = loc->x;
// 		// 	}
// 		// 	if (loc->x < data[indYZ+1]) { // max
// 		// 		data[indYZ+1] = loc->x;
// 		// 	}
// 		// }
		
		
// 		// if ((flags&E_OCT_YP) > 0) { // air y plus
// 		// 	if (loc->y > data[indXZ+0]) { // min
// 		// 		data[indXZ+0] = loc->y;
// 		// 	}
// 		// 	if (loc->y < data[indXZ+1]) { // max
// 		// 		data[indXZ+1] = loc->y;
// 		// 	}
// 		// }
// 		// if ((flags&E_OCT_YM) > 0) { // air y minus
// 		// 	if (loc->y > data[indXZ+0]) { // min
// 		// 		data[indXZ+0] = loc->y;
// 		// 	}
// 		// 	if (loc->y < data[indXZ+1]) { // max
// 		// 		data[indXZ+1] = loc->y;
// 		// 	}
// 		// }
		
		
// 		// if ((flags&E_OCT_ZP) > 0) { // air z plus
// 		// 	if (loc->z > data[indXY+0]) { // min
// 		// 		data[indXY+0] = loc->z;
// 		// 	}
// 		// 	if (loc->z < data[indXY+1]) { // max
// 		// 		data[indXY+1] = loc->z;
// 		// 	}
// 		// }
// 		// if ((flags&E_OCT_ZM) > 0) { // air z minus
// 		// 	if (loc->z > data[indXY+0]) { // min
// 		// 		data[indXY+0] = loc->z;
// 		// 	}
// 		// 	if (loc->z < data[indXY+1]) { // max
// 		// 		data[indXY+1] = loc->z;
// 		// 	}
// 		// }
		
		
		
		
		
		
		
		
// 	}
	
// 	void init() {
// 		int i;
// 		int j;
// 		int k;
		
// 		int ind;
		
// 		for (k = 0; k < 3; k++) {
			
// 			for (j = 0; j < VOXELS_PER_CELL; j++) {
// 				for (i = 0; i < VOXELS_PER_CELL; i++) {
					
// 					ind = (k*VOXELS_PER_CELL*VOXELS_PER_CELL + j*VOXELS_PER_CELL + i)*CUBE_WRAP_ENTRIES;
					
// 					data[ind+0] = 0;
// 					data[ind+1] = VOXELS_PER_CELL-1;
// 					data[ind+2] = 0;
// 					data[ind+3] = 0;
					
// 				}
// 			}
// 		}
// 	}
// };

// typedef CubeWrap* CubeWrapPtr;

struct PaddedDataEntry {
	float terVal;
	int cellVal;
	bool visited;
};

struct VoxelBufferEntry {
	uint flags;
	int vbeIndex;
};

struct VoxelInfo {
	int viIndex;
	uint normId;
	uint matId;
	vec3 normal;
	vec3 pos;
};

struct VoxelCell {
	int curSize;
	int* indexArr;
};

struct VoxelMip {
	bool* mipArr;
	//vector<int> mipList;
};

struct VoxelBuffer {
	VoxelBufferEntry* data;
	VoxelCell* cellLists;
	vector<VoxelInfo> voxelList;
	
	VoxelMip mipMaps[NUM_MIP_LEVELS];
	
	int vcSize;
	int vcPitch;
	
	int vbSize;
	int vbPitch;
	
	
	
	int addIndex(int val) {
		voxelList.push_back(VoxelInfo());
		voxelList.back().viIndex = val;
		voxelList.back().normId = 0;
		voxelList.back().matId = TEX_NULL;
		
		int VLInd = (voxelList.size()-1);
		
		data[val].vbeIndex = VLInd;
		
		return VLInd;
	}
	
	// VoxelInfo* getVoxelInfo(int index) {
	// 	if (index < 0) {
	// 		return NULL;
	// 	}
	// 	else {
	// 		return &(voxelList[index]);
	// 	}
	// }
	
	uint getFlags(int flagPtr) {
		return data[flagPtr].flags;
	}
	
	bool getFlag(int flagPtr, uint flagVal) {
		return (
			(data[flagPtr].flags & flagVal) > 0
		);
	}
	void setFlag(int flagPtr, uint flagVal) {
		data[flagPtr].flags = data[flagPtr].flags | flagVal;
	}
	void clearFlag(int flagPtr, uint flagVal) {
		data[flagPtr].flags = data[flagPtr].flags & (~flagVal);
	}
	
	int addNode(VectorI3* pos, bool &wasNew) {
		int ind = pos->x + pos->y*vbPitch + pos->z*vbPitch*vbPitch;
		
		wasNew = !(getFlag(ind,E_OCT_NOTNEW));
		
		return ind;
	}
	
	uint getFlagsAtNode(int ind) {
		return data[ind].flags;
	}
	uint getIndAtNode(int ind) {
		return data[ind].vbeIndex;
	}
	
	void clearAllNodes() {
		int i;
		int j;
		
		int mySize = voxelList.size();
		int curInd;
		
		for (i = 0; i < mySize; i++) {
			curInd = voxelList[i].viIndex;
			data[curInd].flags = 0;
			data[curInd].vbeIndex = -1;
		}
		
		for (j = 0; j < vcSize; j++) {
			cellLists[j].curSize = 0;
		}
		
		voxelList.clear();
		
		int mipSize = vbPitch/2;
		int mipVol;
		
		for (i = 0; i < NUM_MIP_LEVELS; i++) {
			mipVol = mipSize*mipSize*mipSize;
			for (j = 0; j < mipVol; j++) {
				mipMaps[i].mipArr[j] = false;
			}
			//mipMaps[i].mipList.clear();
			mipSize /= 2;
		}
		
	}
	
};


struct PushModStruct
{
	int actionType;
	FIVector4 data[4];
};

struct ModUnitStruct {
	FIVector4 basePos;
	int brushAction;
	int modType;
	int radius;
};

// enum E_OBJECT_STRUCT_DATA {
// 	// E_OSD_CENTER,
// 	// E_OSD_RADIUS,
// 	E_OSD_OFFSET,
// 	E_OSD_BOUNDSMIN,
// 	E_OSD_BOUNDSMAX,
// 	E_OSD_VISMIN,
// 	E_OSD_VISMAX,
// 	E_OSD_CORNERDISTHICKNESS,
// 	E_OSD_POWERVALS,//THICKNESS,
// 	E_OSD_MATPARAMS,
// 	E_OSD_LENGTH
// };

struct ObjectStruct {
	int64_t globalId;
	// int objType;
	// int addType;
	int templateId;
	int orientation;
	vec3 offset;
	
	
	
	//vec4 data[E_PRIMTEMP_LENGTH];
	
};

struct PaddedData {
	PaddedDataEntry* data;
	vector<VectorI3> fillStack;
	VoxelBuffer voxelBuffer;
	
	
	
	intTrip boundToHolder;
	bool isFree;
};


struct TBOEntry {
	TBOWrapper tbo;
	VBOWrapper vbo;
	uint* data;
	uint writeIndex;
	
	int sizeInBytes;
	int sizeInEntries;
	bool locked;
	
	
	void init(int _sizeInBytes) {
		locked = false;
		writeIndex = 0;
		
		sizeInBytes = _sizeInBytes;
		sizeInEntries = sizeInBytes/4;
		
		data = new uint[sizeInEntries];
		
		tbo.init(false,NULL,data,sizeInBytes);
		
		vbo.init(
			4,
			GL_STATIC_DRAW
		);
		
		
	}
	
	void writeData(uint val) {
		data[writeIndex] = val;
		writeIndex++;
	}
	
	bool cantWrite(int len) {
		return (
			(writeIndex+len) >= sizeInEntries
		);
	}
	
	void writeDataArr(uint* vals, int len) {
		int i;
		
		for (i = 0; i < len; i++) {
			data[writeIndex] = vals[i];
			writeIndex++;
		}
		
	}
	
	
	void lock() {
		locked = true;
		writeIndex = 0;
		
		vbo.beginFill();
	}
	
	void unlock() {
		tbo.update(NULL, data, -1);
		vbo.endFill();
		glFlush();
		glFinish();
		locked = false;
	}
	
};



vec3 VORO_OFFSETS[27];

vec3 BASE_NORMALS[64];



// const static int VORO_PITCH = 16;
// const static float VORO_VARIANCE = 0.25f;



// class Q3Rend : public q3Render {
// public:
// 	void SetPenColor( f32 r, f32 g, f32 b, f32 a = 1.0f ) override
// 	{
// 		// Q3_UNUSED( a );

// 		// glColor3f( (float)r, (float)g, (float)b );
// 	}

// 	void SetPenPosition( f32 x, f32 y, f32 z ) override
// 	{
// 		x_ = x, y_ = y, z_ = z;
// 	}

// 	void SetScale( f32 sx, f32 sy, f32 sz ) override
// 	{
// 		// glPointSize( (float)sx );
// 		// sx_ = sx, sy_ = sy, sz_ = sz;
// 	}

// 	void Line( f32 x, f32 y, f32 z ) override
// 	{
// 		// glBegin( GL_LINES );
// 		// glVertex3f( (float)x_, (float)y_, (float)z_ );
// 		// glVertex3f( (float)x, (float)y, (float)z );
// 		// SetPenPosition( x, y, z );
// 		// glEnd( );
// 	}

// 	void Triangle(
// 		f32 x1, f32 y1, f32 z1,
// 		f32 x2, f32 y2, f32 z2,
// 		f32 x3, f32 y3, f32 z3
// 		) override
// 	{
		
// 		//glEnable( GL_LIGHTING );
// 		//glBegin( GL_TRIANGLES );
// 		//glColor4f( 0.2f, 0.4f, 0.7f, 0.5f );
// 		glVertex3f( (float)x1, (float)y1, (float)z1 );
// 		glVertex3f( (float)x2, (float)y2, (float)z2 );
// 		glVertex3f( (float)x3, (float)y3, (float)z3 );
// 		//glEnd( );
// 		//glDisable( GL_LIGHTING );
// 	}

// 	void SetTriNormal( f32 x, f32 y, f32 z ) override
// 	{
// 		glNormal3f( (float)x, (float)y, (float)z );
// 	}

// 	void Point( ) override
// 	{
// 		// glBegin( GL_POINTS );
// 		// glVertex3f( (float)x_, (float)y_, (float)z_ );
// 		// glEnd( );
// 	};

// private:
// 	f32 x_, y_, z_;
// 	f32 sx_, sy_, sz_;
// };

// Q3Rend q3Rend;





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




enum E_PRIMTEMP {
	E_PRIMTEMP_VISMIN,
	E_PRIMTEMP_VISMAX,
	E_PRIMTEMP_BOUNDSMIN,
	E_PRIMTEMP_BOUNDSMAX,
	E_PRIMTEMP_CORNERDIS, //x: cornerDis, y: wallThickness, z: powerXY, w: powerZ
	E_PRIMTEMP_MATPARAMS, //x: material
	E_PRIMTEMP_LENGTH
};

const static int FLOATS_PER_TEMPLATE = ((int)E_PRIMTEMP_LENGTH)*4;
const static float defaultTemplate[FLOATS_PER_TEMPLATE] = {
	-2.0,-2.0,-2.0, 0.0,
	 2.0, 2.0, 2.0, 0.0,
	-2.0,-2.0,-2.0, 0.0,
	 2.0, 2.0, 2.0, 0.0,
	 1.0, 1.0, 1.0, 1.0,
	 0.0, 0.0, 0.0, 0.0
};

string primTempStrings[] = {
	"visMin",
	"visMax",
	"boundsMin",
	"boundsMax",
	"cornerDis",
	"matParams"
};



inline float opI( float d1, float d2 ) {
		return max(d1,d2);
}

inline float opS( float d1, float d2 ) {
		return max(-d2,d1);
}

inline float sdBox( vec3 _p, vec3 b ) {
	vec3 p = _p;
	p.doAbs();
	vec3 d = p - b;
	
	float res = min(max(d.x,max(d.y,d.z)),0.0f);
	d.doMax(vec3(0.0f));
	res += d.length();
	
	return res;
}

float getBrick( vec3 uvwCoords) {
		
		vec3 temp3 = uvwCoords*0.5f;
		temp3.doFract();
		temp3 *= 2.0f;
		
		float mv1 = 0.0f;
		float mv2 = 0.0f;
		
		if (temp3.y < 1.0f) {
			mv1 = 1.0f;
		}
		if (temp3.z < 1.0f) {
			mv2 = 1.0f;
		}
		
		//float mv1 = float(mod(uvwCoords.y,2.0f) < 1.0f);
		//float mv2 = float(mod(uvwCoords.z,2.0f) < 1.0f);
		
		
		vec3 res = uvwCoords + vec3(0.5f*(mv1+mv2),0.0f,0.0f);
		res.doFract();
		
		res = (res-0.5f);
		res.doAbs();
		res *= 2.0f;
		
		return max(max(res.x,res.y),res.z);
}


inline void getBoundsCenRad(
	ObjectStruct* curObj,
	FIVector4* baseGeom,
	vec3 &visCen,
	vec3 &visRad
) {
	
	
	vec3 vmin = baseGeom[E_PRIMTEMP_BOUNDSMIN].getVec3();
	vec3 vmax = baseGeom[E_PRIMTEMP_BOUNDSMAX].getVec3();
	
	visCen = (vmax+vmin)*0.5f;
	visRad = (vmax-vmin)*0.5f;
	
	visCen += curObj->offset;
	
}

void getVisCenRad(
	ObjectStruct* curObj,
	FIVector4* baseGeom,
	vec3 &visCen,
	vec3 &visRad
) {
	
	
	vec3 vmin = baseGeom[E_PRIMTEMP_VISMIN].getVec3();
	vec3 vmax = baseGeom[E_PRIMTEMP_VISMAX].getVec3();
	
	visCen = (vmax+vmin)*0.5f;
	visRad = (vmax-vmin)*0.5f;
	
	visCen += curObj->offset;
	
}

inline vec3 getUVW(
		vec3 _pos,
		ObjectStruct* curObj,
		FIVector4* baseGeom,
		
		vec3 uvwScale,
		float angModBase,
		bool mirrored
) {
		
		float globIntersect = 999.0f;
		
		vec3 centerPos;
		vec3 centerRad;
		getBoundsCenRad(curObj,baseGeom,centerPos,centerRad);
		
		
		float cornerRad = baseGeom[E_PRIMTEMP_CORNERDIS].getFX();
		vec3 centerOffset = _pos - centerPos;
		vec3 innerBoxSize = centerRad-cornerRad;
		
		float wallThickness	= baseGeom[E_PRIMTEMP_CORNERDIS].getFY();
		vec3 centerOffsetAbs = centerOffset;
		centerOffsetAbs.doAbs();
		
		
		vec3 newOffset = centerOffsetAbs-innerBoxSize;
		newOffset.doMax(vec3(0.0));
		
		// vec3 newOffsetAbs = newOffset;
		// newOffsetAbs.doAbs();
		
		
		
		vec3 minCorner = centerPos-(innerBoxSize+cornerRad);
		vec3 pos = (_pos-minCorner)*uvwScale;
		

		
		vec3 uvwCoords = pos;
						
		vec3 newNorm1 = newOffset;
		newNorm1.z = 0.0f;
		newNorm1.normalize();
		
		if (centerOffset.x < 0.0f) {
			newNorm1.x *= -1.0f;
		}
		if (centerOffset.y < 0.0f) {
			newNorm1.y *= -1.0f;
		}
		
		vec3 newNorm2;
		newNorm2 = vec3(
			newNorm1.length(),
			newOffset.z,
			0.0f
		);
		
		newNorm2.normalize();
		
		
		vec3 centerOffsetNorm = centerOffset/innerBoxSize;
		centerOffsetNorm.doAbs();
		
		// y side
		if (newNorm1.x == 0.0f) {
				uvwCoords.x = pos.x;
				uvwCoords.y = pos.z;
				
		}
		
		// x side
		if (newNorm1.y == 0.0f) {
				uvwCoords.x = pos.y;
				uvwCoords.y = pos.z;
				
		}
		
		float curPhi = atan2(newNorm1.y, newNorm1.x);
		float curThe = atan2(newNorm2.y, newNorm2.x);
		
		float angMod = 
				(angModBase*2.0f/M_PI) *
				(max(floor(sqrt(cornerRad*cornerRad*2.0f)),1.0f));
		
		
		// side corner
		if (newNorm1.x*newNorm1.y != 0.0f) {
				uvwCoords.x = curPhi*angMod;
				uvwCoords.y = pos.z;
		}
		
		// top corner
		if (newNorm2.x*newNorm2.y != 0.0f) {
				uvwCoords.y = curThe*angMod;
		}
		
		// top
		if (newNorm2.x == 0.0f) {
				if (centerOffsetNorm.x > centerOffsetNorm.y) {
						uvwCoords.x = pos.y;
						uvwCoords.y = pos.x;
						
						if (centerOffset.x > 0.0f) {
								uvwCoords.y *= -1.0f;
								
								uvwCoords.y += 0.5f;
						}
				}
				else {
						uvwCoords.x = pos.x;
						uvwCoords.y = pos.y;
						
						if (centerOffset.y > 0.0f) {
								uvwCoords.y *= -1.0f;
								
								uvwCoords.y += 0.5f;
								
						}
				}
				
				globIntersect = abs(centerOffsetNorm.x - centerOffsetNorm.y);
				
		}
		
		
		uvwCoords.z = 0.0;
		
		return uvwCoords;//vec4(uvwCoords,newNorm2.x);
}





inline float primDis(
	vec3 pos,
	ObjectStruct* curObj,
	FIVector4* baseGeom
) {
	
	vec3 centerPos;
	vec3 centerRad;
	vec2 box_power;
	
	getBoundsCenRad(curObj,baseGeom,centerPos,centerRad);
	
	
	float cornerRad = baseGeom[E_PRIMTEMP_CORNERDIS].getFX();
	float wallThickness = baseGeom[E_PRIMTEMP_CORNERDIS].getFY();
	box_power.x = baseGeom[E_PRIMTEMP_CORNERDIS].getFZ();
	box_power.y = baseGeom[E_PRIMTEMP_CORNERDIS].getFW();
	
	vec3 innerBoxSize = centerRad-cornerRad;
	
	vec3 vectorFromCenter = pos - centerPos;
	
	
	
	vec3 absVecFromCenter = vectorFromCenter;
	absVecFromCenter.doAbs();
	
	
	
	vec3 newP = absVecFromCenter-innerBoxSize;
	newP.doMax(vec3(0.0));
	newP.doAbs();
	
	
	
	newP.x = pow(newP.x, box_power.x );
	newP.y = pow(newP.y, box_power.x );
	newP.x = pow( newP.x + newP.y, 1.0/box_power.x );
	
	newP.x = pow(newP.x, box_power.y );
	newP.z = pow(newP.z, box_power.y );
	newP.x = pow( newP.x + newP.z, 1.0/box_power.y );
	
	float finalRes = (newP.x-cornerRad);
	
	vec3 newBoxDim;
	
	if (
		(absVecFromCenter.x <= innerBoxSize.x) &&	
		(absVecFromCenter.y <= innerBoxSize.y) &&
		(absVecFromCenter.z <= innerBoxSize.z)
	) {
		
		newBoxDim = innerBoxSize-absVecFromCenter;
		
		finalRes -= min(min(newBoxDim.x,newBoxDim.y),newBoxDim.z);
	}
	
	finalRes = abs(finalRes+wallThickness*0.5f)-wallThickness*0.5f;
	
	
	vec3 boxRad;
	vec3 boxCen;
	
	getVisCenRad(curObj,baseGeom,boxCen,boxRad);
	
	
	return (
		opI(
			sdBox(pos-boxCen,boxRad),
			finalRes
		)
	);
	
	// return vec3(
	// 		(newP.x-cornerRad),
	// 		( (cornerRad-wallThickness)-newP.x ),
	// 		(newP.x-(cornerRad-wallThickness))
	// );
	
	
}




 
