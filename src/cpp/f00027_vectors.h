
struct iVector4 {
	int x;
	int y;
	int z;
	int w;
};
struct fVector4 {
	float x;
	float y;
	float z;
	float w;
};

struct iVector3 {
	int x;
	int y;
	int z;
};
struct fVector3 {
	float x;
	float y;
	float z;
};


struct iVector2 {
	int x;
	int y;
};
struct fVector2 {
	float x;
	float y;
};

struct iBoundingBox {
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

struct fBoundingBox {
	float xMin;
	float yMin;
	float xMax;
	float yMax;
};

int intDiv(int v, int s) {
    float fv = v;
    float fs = s;

    if (v < 0) {
        return -ceil(-fv/fs);
    }
    else {
        return v/s;
    }
}

inline float clamp(float val) {
    float retval = val;
    if (retval < 0.0f) retval = 0.0f;
    if (retval > 1.0f) retval = 1.0f;
    return retval;
}

inline float fGenRand() {
    
    return (float)rand()/(float)RAND_MAX;
    //return ((float)(rand()%100000))/100000.0f;
}

inline int iGenRand(int val) {
    
    return abs(rand()%val);
    //return rand()%(val+1) - val/2;
}

unsigned int intLogB2 (unsigned int val) {
    unsigned int ret = -1;
    while (val != 0) {
        val >>= 1;
        ret++;
    }
    return ret;
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

    void copyFrom(FIVector4* cf) {
        iv4.x = cf->getIX();
        iv4.y = cf->getIY();
        iv4.z = cf->getIZ();
        iv4.w = cf->getIW();

        fv4.x = cf->getFX();
        fv4.y = cf->getFY();
        fv4.z = cf->getFZ();
        fv4.w = cf->getFW();
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

    void setFXYZRef(FIVector4 * v1) {
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

    void addXYZ(float scalar, float multiplier=1.0f) {
        fv4.x += scalar*multiplier;
        fv4.y += scalar*multiplier;
        fv4.z += scalar*multiplier;

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }
    void addXYZ(float scalarX, float scalarY, float scalarZ, float multiplier=1.0f) {
        fv4.x += scalarX*multiplier;
        fv4.y += scalarY*multiplier;
        fv4.z += scalarZ*multiplier;

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }
    void addXYZRef(FIVector4 *scalar, float multiplier=1.0f) {
        fv4.x += scalar->getFX()*multiplier;
        fv4.y += scalar->getFY()*multiplier;
        fv4.z += scalar->getFZ()*multiplier;

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

    


    void intDivXYZ(int scalar) {

        float fScalar = (float)scalar;

        iv4.x = intDiv(iv4.x,scalar);
        iv4.y = intDiv(iv4.y,scalar);
        iv4.z = intDiv(iv4.z,scalar);

        // if (iv4.x < 0) {
        //     iv4.x = -ciel(-fv4.x/fScalar);
        // }
        // else {
        //     iv4.x /= scalar;
        // }

        // if (iv4.y < 0) {
        //     iv4.y = -ciel(-fv4.y/fScalar);
        // }
        // else {
        //     iv4.y /= scalar;
        // }

        // if (iv4.z < 0) {
        //     iv4.z = -ciel(-fv4.z/fScalar);
        // }
        // else {
        //     iv4.z /= scalar;
        // }

        fv4.x = (float)iv4.x;
        fv4.y = (float)iv4.y;
        fv4.z = (float)iv4.z;

    }

    void modXYZ(int scalar) {
        iv4.x = iv4.x%scalar;
        iv4.y = iv4.y%scalar;
        iv4.z = iv4.z%scalar;

        fv4.x = (float)iv4.x;
        fv4.y = (float)iv4.y;
        fv4.z = (float)iv4.z;

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

    void wrapXYZ(int scalar) {
        iv4.x = wrapCoord(iv4.x,scalar);
        iv4.y = wrapCoord(iv4.y,scalar);
        iv4.z = wrapCoord(iv4.z,scalar);

        fv4.x = (float)iv4.x;
        fv4.y = (float)iv4.y;
        fv4.z = (float)iv4.z;

    }

    void wrapXY(int scalar) {
        iv4.x = wrapCoord(iv4.x,scalar);
        iv4.y = wrapCoord(iv4.y,scalar);

        fv4.x = (float)iv4.x;
        fv4.y = (float)iv4.y;

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
    void clampZ(FIVector4 *minV, FIVector4 *maxV) {

        if (fv4.z < minV->getFZ()) {
            fv4.z = minV->getFZ();
        }
        if (fv4.z > maxV->getFZ()) {
            fv4.z = maxV->getFZ();
        }

        iv4.x = (int)fv4.x;
        iv4.y = (int)fv4.y;
        iv4.z = (int)fv4.z;
    }





    bool inBoundsEqualXYZ(FIVector4 *minV, FIVector4 *maxV) {
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
        if (
            (iv4.x == otherVec->getIX()) &&
            (iv4.y == otherVec->getIY()) &&
            (iv4.z == otherVec->getIZ())
        ) {
            return false;
        }
        else {
            return true;
        }
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

                otherVec->addXYZ(i*maxPitch, j*maxPitch, 0, 1.0f);
                curDis = distance(otherVec);
                otherVec->addXYZ(i*maxPitch, j*maxPitch, 0, -1.0f);

                if (curDis < shortestDis) {
                    shortestDis = curDis;
                    bestI = i;
                    bestJ = j;
                }

            }   
        }

        if (doSet) {
            otherVec->addXYZ(bestI*maxPitch, bestJ*maxPitch, 0);
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

    float distance(FIVector4 *otherVec) {

        float dx = fv4.x - otherVec->getFX();
        float dy = fv4.y - otherVec->getFY();
        float dz = fv4.z - otherVec->getFZ();

        return sqrt(dx*dx + dy*dy + dz*dz);
    }

    float manhattanDis(FIVector4 *otherVec) {

        float dx = abs(fv4.x - otherVec->getFX());
        float dy = abs(fv4.y - otherVec->getFY());
        float dz = abs(fv4.z - otherVec->getFZ());

        return max(max(dx,dy),dz);
    }

    void normalize() {
        float len = sqrt(fv4.x*fv4.x + fv4.y*fv4.y + fv4.z*fv4.z);

        fv4.x = fv4.x/len;
        fv4.y = fv4.y/len;
        fv4.z = fv4.z/len;


    }




    iVector4* getIXYZW() {
        return &iv4;
    }
    fVector4* getFXYZW() {
        return &fv4;
    }

    iVector3* getIXYZ() {
        iv3.x = iv4.x;
        iv3.y = iv4.y;
        iv3.z = iv4.z;
        return &iv3;
    }
    fVector3* getFXYZ() {
        fv3.x = fv4.x;
        fv3.y = fv4.y;
        fv3.z = fv4.z;
        return &fv3;
    }

    iVector2* getIXY() {
        iv2.x = iv4.x;
        iv2.y = iv4.y;
        return &iv2;
    }
    fVector2* getFXY() {
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









class GameGeom {
public:

    FIVector4 boundsMinInPixels;
    FIVector4 boundsMaxInPixels;
    FIVector4 originInPixels;
    FIVector4 powerVals;
    FIVector4 coefficients;
    FIVector4 squareVals;
    FIVector4 minMaxMat;

    int id;

    //float minRad;
    //float maxRad;
    //float matId;

    GameGeom() {

    }

    void init(int _id) {
        id = _id;
    }

    float getRand() {
        return (fGenRand()+1.0f)/2.0f;
    }

    void initRand(int _id, float x, float y, float z) {
        id = _id;

        float rad = 256.0f;
        float diam = 2.0f*rad;
        float zh = 256.0f;

        if (fGenRand() > 0.5) {
            boundsMinInPixels.setFXYZ(x-rad*3.0, y-rad, z+zh);
            boundsMaxInPixels.addXYZ(x+rad*3.0, y+rad, z+diam+zh);
        }
        else {
            boundsMinInPixels.setFXYZ(x-rad, y-rad*3.0, z+zh);
            boundsMaxInPixels.addXYZ(x+rad, y+rad*3.0, z+diam+zh);
        }

        /*
        boundsMinInPixels.setFXYZ(x-rad, y-rad, z+zh);
        boundsMaxInPixels.addXYZ(x+rad, y+rad, z+diam+zh);
        */

        

        originInPixels.copyFrom(&boundsMinInPixels);
        originInPixels.addXYZRef(&boundsMaxInPixels);
        originInPixels.multXYZ(0.5f);

        powerVals.setFXYZ(2.0f,2.0f,2.0f);
        coefficients.setFXYZ(1.0,1.0,1.0);
        squareVals.setFXYZ(0.0,0.0,0.0);
        minMaxMat.setFXYZ(0.5f,1.0f,2.0f);

        //minRad = 0.75;
        //maxRad = 1.0;
        //matId = 1.0f;

        
        //powerPhi = fGenRand()*3.0f + 1.0f;
        //powerTheta = fGenRand()*3.0f + 1.0f;
        //thickness = 16.0f;
        //upAxis = 2.0f;


    }

};
