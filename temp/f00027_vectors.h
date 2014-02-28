
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

int getPackedColor(uint r, uint g, uint b) {
    return (
        (1 << 15) |
        ((b >> 3)<<10) | 
        ((g >> 3)<<5) | 
        ((r >> 3))
    );
}

int clamp(int val, int min, int max) {
    if (val > max) {
        val = max;
    }
    if (val < min) {
        val = min;
    }
    return val;
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

inline float clampfZO(float val) {
    return clampf(val,0.0f,1.0f);
}

inline float fGenRand() {
    
    return (float)rand()/(float)RAND_MAX;
    //return ((float)(rand()%100000))/100000.0f;
}


inline int iGenRand(int nMin, int nMax)
{
    return nMin + (int)((double)rand() / (RAND_MAX+1) * (nMax-nMin+1));
}

// inline int iGenRand(int val) {
    
//     return abs(rand()%val);
//     //return rand()%(val+1) - val/2;
// }

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

    void copyIntDiv(FIVector4* cf, int val) {
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

    void copyIntMult(FIVector4* cf, int val) {
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

    static void normalizeBounds(FIVector4* minBounds, FIVector4* maxBounds) {
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

    static bool intersect(FIVector4* aMin, FIVector4* aMax, FIVector4* bMin, FIVector4* bMax) {
        
        float aWidth = aMax->getFX() - aMin->getFX();
        float aHeight = aMax->getFY() - aMin->getFY();
        float aDepth = aMax->getFZ() - aMin->getFZ();

        float bWidth = bMax->getFX() - bMin->getFX();
        float bHeight = bMax->getFY() - bMin->getFY();
        float bDepth = bMax->getFZ() - bMin->getFZ();

        float aWidthC = (aMax->getFX() + aMin->getFX())/2.0f;
        float aHeightC = (aMax->getFY() + aMin->getFY())/2.0f;
        float aDepthC = (aMax->getFZ() + aMin->getFZ())/2.0f;

        float bWidthC = (bMax->getFX() + bMin->getFX())/2.0f;
        float bHeightC = (bMax->getFY() + bMin->getFY())/2.0f;
        float bDepthC = (bMax->getFZ() + bMin->getFZ())/2.0f;



        return  (abs(aWidthC - bWidthC) * 2.0f <= (aWidth + bWidth)) &&
                (abs(aHeightC - bHeightC) * 2.0f <= (aHeight + bHeight)) &&
                (abs(aDepthC - bDepthC) * 2.0f <= (aDepth + bDepth));


        // return (
        //     aMin->inBoundsIsEqualXYZ(bMin,bMax) ||
        //     aMax->inBoundsIsEqualXYZ(bMin,bMax) ||
        //     bMin->inBoundsIsEqualXYZ(aMin,aMax) ||
        //     bMax->inBoundsIsEqualXYZ(aMin,aMax)
        // );
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

    void rotate90(int ind) {
        switch (ind) {
            case 0: // 0 deg

            break;

            case 1: // 90 deg
                setFXY(-fv4.y,fv4.x);
            break;

            case 2: // 180 deg
                setFXY(-fv4.x,-fv4.y);
            break;

            case 3: // 270 deg
                setFXY(fv4.y,-fv4.x);
            break;
        }
    }

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

    float dot(FIVector4 *otherVec) {

        return fv4.x * otherVec->getFX() +
        fv4.y * otherVec->getFY() +
        fv4.z * otherVec->getFZ();
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

private:
    // passed to GPU, transformed

    FIVector4 boundsMinInPixels;
    FIVector4 boundsMaxInPixels;
    FIVector4 visMinInPixels;
    FIVector4 visMaxInPixels;

public:

    // passed to GPU

    FIVector4 geomParams[E_GP_LENGTH];

    // enum E_GEOM_PARAMS {
    // E_GP_BOUNDSMININPIXELST,
    // E_GP_BOUNDSMAXINPIXELST,
    // E_GP_VISMININPIXELST,
    // E_GP_VISMAXINPIXELST,
    //     E_GP_CORNERDISINPIXELS,
    //     E_GP_POWERVALS,
    //     E_GP_POWERVALS2,
    //     E_GP_THICKVALS,
    //     E_GP_MATPARAMS,
    //     E_GP_LENGTH
    // }

    // geomParams[E_GP_BOUNDSMININPIXELST]
    // geomParams[E_GP_BOUNDSMAXINPIXELST]
    // geomParams[E_GP_VISMININPIXELST]
    // geomParams[E_GP_VISMAXINPIXELST]
    // geomParams[E_GP_CORNERDISINPIXELS]
    // geomParams[E_GP_POWERVALS]
    // geomParams[E_GP_POWERVALS2]
    // geomParams[E_GP_THICKVALS]
    // geomParams[E_GP_MATPARAMS]

    // FIVector4 boundsMinInPixelsT;
    // FIVector4 boundsMaxInPixelsT;
    // FIVector4 visMinInPixelsT;
    // FIVector4 visMaxInPixelsT;
    // FIVector4 cornerDisInPixels;
    // FIVector4 powerVals;
    // FIVector4 powerVals2;
    // FIVector4 thickVals;
    // FIVector4 matParams;

    // internal use

    FIVector4 anchorPointInPixels;
    FIVector4 moveMinInPixels;
    FIVector4 moveMaxInPixels;



    bool visible;
    bool hasAnchor;

    //   1
    // 2   0
    //   3


    int rotDir;

    int minRot;
    int maxRot;
    int curRot;
    int buildingType;

    int id;
    int globalID;
    //static const int paramsPerEntry = 27;

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

    /*
    FIVector4 boundsMinInPixels;
    FIVector4 boundsMaxInPixels;
    FIVector4 visMinInPixels;
    FIVector4 visMaxInPixels;

    FIVector4 boundsMinInPixelsT;
    FIVector4 boundsMaxInPixelsT;
    FIVector4 visMinInPixelsT;
    FIVector4 visMaxInPixelsT;
    */

    FIVector4* getBoundsMinInPixels() {
        return &boundsMinInPixels;
    }
    FIVector4* getBoundsMaxInPixels() {
        return &boundsMaxInPixels;
    }
    FIVector4* getVisMinInPixels() {
        return &visMinInPixels;
    }
    FIVector4* getVisMaxInPixels() {
        return &visMaxInPixels;
    }

    FIVector4* getBoundsMinInPixelsT() {
        return &geomParams[E_GP_BOUNDSMININPIXELST];
    }
    FIVector4* getBoundsMaxInPixelsT() {
        return &geomParams[E_GP_BOUNDSMAXINPIXELST];
    }
    FIVector4* getVisMinInPixelsT() {
        return &geomParams[E_GP_VISMININPIXELST];
    }
    FIVector4* getVisMaxInPixelsT() {
        return &geomParams[E_GP_VISMAXINPIXELST];
    }

    int getClampedRot() {
        int tempRot = curRot;
        while (tempRot < 0) {
            tempRot += 4;
        }
        while (tempRot > 3) {
            tempRot -= 4;
        }

        return tempRot;
    }

    void rotate(int mod, bool ignoreConstraints) {

        if (hasAnchor) {
            curRot += mod;


            if (ignoreConstraints) {
                if (curRot > 3) {
                    curRot = 0;
                }
                if (curRot < 0) {
                    curRot = 3;
                }
            }
            else {
                if (curRot > maxRot) {
                    curRot = maxRot-1;
                    rotDir *= -1;
                }
                if (curRot < minRot) {
                    curRot = minRot+1;
                    rotDir *= -1;

                }

            }

            
        }
        else {
            cout << "Attemped to rotate without anchor.\n";
        }

        
    }

    void initBounds(
        int _buildingType,
        int _id,
        int _globalID,
        int alignBottomMiddleTop,

        float _zOffset,
        
        FIVector4* p1,
        FIVector4* p2,
        FIVector4* rad,
        FIVector4* _cornerDisInPixels,
        FIVector4* _visInsetFromMin,
        FIVector4* _visInsetFromMax,
        FIVector4* _powerVals,
        FIVector4* _powerVals2,
        FIVector4* _thickVals,
        FIVector4* _matParams

        
        
    ) {
        buildingType = _buildingType;
        id = _id;
        globalID = _globalID;
        float temp;
        float zOffset = _zOffset;

        curRot = 0;
        rotDir = 1;
        visible = true;
        hasAnchor = false;

        anchorPointInPixels.setFXYZ(0.0f,0.0f,0.0f);


        boundsMinInPixels.setFXYZRef(p1);
        boundsMaxInPixels.setFXYZRef(p2);

        FIVector4::normalizeBounds(&boundsMinInPixels,&boundsMaxInPixels);

        


        boundsMinInPixels.addXYZRef(rad,-1.0f);
        boundsMaxInPixels.addXYZRef(rad,1.0f);

        switch (alignBottomMiddleTop) {
            
            case 0: // bottom _@_
                zOffset += (rad->getFZ()-_visInsetFromMin->getFZ());
            break;
            case 1: // middle -@-
                zOffset += 0.0f;
            break;
                    //     ___
            case 2: // top  @
                zOffset += -(rad->getFZ()-_visInsetFromMax->getFZ());
            break;
            

        }

        boundsMinInPixels.addXYZ(0.0f,0.0f,zOffset);
        boundsMaxInPixels.addXYZ(0.0f,0.0f,zOffset);


        visMinInPixels.setFXYZRef(&boundsMinInPixels);
        visMaxInPixels.setFXYZRef(&boundsMaxInPixels);

        visMinInPixels.addXYZRef(_visInsetFromMin, 1.0f);
        visMaxInPixels.addXYZRef(_visInsetFromMax, -1.0f);

        geomParams[E_GP_CORNERDISINPIXELS].setFXYZRef(_cornerDisInPixels);
        geomParams[E_GP_POWERVALS].setFXYZRef(_powerVals);
        geomParams[E_GP_POWERVALS2].setFXYZRef(_powerVals2);
        geomParams[E_GP_THICKVALS].setFXYZRef(_thickVals);
        geomParams[E_GP_MATPARAMS].setFXYZRef(_matParams);


        moveMinInPixels.setFXYZRef(&boundsMinInPixels);
        moveMaxInPixels.setFXYZRef(&boundsMaxInPixels);

        geomParams[E_GP_BOUNDSMININPIXELST].setFXYZRef(&boundsMinInPixels);
        geomParams[E_GP_BOUNDSMAXINPIXELST].setFXYZRef(&boundsMaxInPixels);
        geomParams[E_GP_VISMININPIXELST].setFXYZRef(&visMinInPixels);
        geomParams[E_GP_VISMAXINPIXELST].setFXYZRef(&visMaxInPixels);


    }


    void initTree(
        int _buildingType,
        int _id,
        int _globalID,
        int alignBottomMiddleTop,

        float _zOffset,
        
        FIVector4* p0,
        FIVector4* p1,

        float radP0,
        float radP1,

        // FIVector4* rad,
        // FIVector4* _cornerDisInPixels,
        FIVector4* _visInsetFromMin,
        FIVector4* _visInsetFromMax,
        // FIVector4* _powerVals,
        // FIVector4* _powerVals2,
        // FIVector4* _thickVals,
        FIVector4* _matParams

        
        
    ) {
        buildingType = _buildingType;
        id = _id;
        globalID = _globalID;
        float temp;
        float zOffset = _zOffset;

        float radMax = max(radP0, radP1);

        curRot = 0;
        rotDir = 1;
        visible = true;
        hasAnchor = false;

        anchorPointInPixels.setFXYZ(0.0f,0.0f,0.0f);


        boundsMinInPixels.setFXYZRef(p0);
        boundsMaxInPixels.setFXYZRef(p1);

        boundsMinInPixels.addXYZ(-radMax);
        boundsMaxInPixels.addXYZ(radMax);

        FIVector4::normalizeBounds(&boundsMinInPixels,&boundsMaxInPixels);


        

        // switch (alignBottomMiddleTop) {
            
        //     case 0: // bottom _@_
        //         zOffset += (radMax-_visInsetFromMin->getFZ());
        //     break;
        //     case 1: // middle -@-
        //         zOffset += 0.0f;
        //     break;
        //             //     ___
        //     case 2: // top  @
        //         zOffset += -(radMax-_visInsetFromMax->getFZ());
        //     break;

        // }

        // boundsMinInPixels.addXYZ(0.0f,0.0f,zOffset);
        // boundsMaxInPixels.addXYZ(0.0f,0.0f,zOffset);


        visMinInPixels.setFXYZRef(&boundsMinInPixels);
        visMaxInPixels.setFXYZRef(&boundsMaxInPixels);

        visMinInPixels.addXYZRef(_visInsetFromMin, 1.0f);
        visMaxInPixels.addXYZRef(_visInsetFromMax, -1.0f);

        // geomParams[E_GP_CORNERDISINPIXELS].setFXYZRef(_cornerDisInPixels);
        // geomParams[E_GP_POWERVALS].setFXYZRef(_powerVals);
        // geomParams[E_GP_POWERVALS2].setFXYZRef(_powerVals2);
        

        geomParams[E_TP_P0].setFXYZRef(p0);
        geomParams[E_TP_P1].setFXYZRef(p1);
        geomParams[E_TP_THICKVALS].setFXYZ(radP0, radP1, 0.0f);


        geomParams[E_TP_MATPARAMS].setFXYZRef(_matParams);


        moveMinInPixels.setFXYZRef(&boundsMinInPixels);
        moveMaxInPixels.setFXYZRef(&boundsMaxInPixels);

        // geomParams[E_GP_BOUNDSMININPIXELST].setFXYZRef(&boundsMinInPixels);
        // geomParams[E_GP_BOUNDSMAXINPIXELST].setFXYZRef(&boundsMaxInPixels);
        geomParams[E_TP_VISMININPIXELST].setFXYZRef(&visMinInPixels);
        geomParams[E_TP_VISMAXINPIXELST].setFXYZRef(&visMaxInPixels);


    }



    void applyTransform(int rotMod, bool ignoreConstraints) {

        rotate(rotMod,ignoreConstraints);

        geomParams[E_GP_BOUNDSMININPIXELST].setFXYZRef(&boundsMinInPixels);
        geomParams[E_GP_BOUNDSMAXINPIXELST].setFXYZRef(&boundsMaxInPixels);
        geomParams[E_GP_VISMININPIXELST].setFXYZRef(&visMinInPixels);
        geomParams[E_GP_VISMAXINPIXELST].setFXYZRef(&visMaxInPixels);

        geomParams[E_GP_BOUNDSMININPIXELST].addXYZRef(&anchorPointInPixels,-1.0f);
        geomParams[E_GP_BOUNDSMAXINPIXELST].addXYZRef(&anchorPointInPixels,-1.0f);
        geomParams[E_GP_VISMININPIXELST].addXYZRef(&anchorPointInPixels,-1.0f);
        geomParams[E_GP_VISMAXINPIXELST].addXYZRef(&anchorPointInPixels,-1.0f);

        geomParams[E_GP_BOUNDSMININPIXELST].rotate90(getClampedRot());
        geomParams[E_GP_BOUNDSMAXINPIXELST].rotate90(getClampedRot());
        geomParams[E_GP_VISMININPIXELST].rotate90(getClampedRot());
        geomParams[E_GP_VISMAXINPIXELST].rotate90(getClampedRot());

        geomParams[E_GP_BOUNDSMININPIXELST].addXYZRef(&anchorPointInPixels,1.0f);
        geomParams[E_GP_BOUNDSMAXINPIXELST].addXYZRef(&anchorPointInPixels,1.0f);
        geomParams[E_GP_VISMININPIXELST].addXYZRef(&anchorPointInPixels,1.0f);
        geomParams[E_GP_VISMAXINPIXELST].addXYZRef(&anchorPointInPixels,1.0f);

        FIVector4::normalizeBounds(&geomParams[E_GP_BOUNDSMININPIXELST],&geomParams[E_GP_BOUNDSMAXINPIXELST]);
        FIVector4::normalizeBounds(&geomParams[E_GP_VISMININPIXELST],&geomParams[E_GP_VISMAXINPIXELST]);

        FIVector4::growBoundary(&moveMinInPixels, &moveMaxInPixels, &geomParams[E_GP_VISMININPIXELST], &geomParams[E_GP_VISMAXINPIXELST]);
    }

    void initAnchorPoint(FIVector4* _anchorPointInPixels, int _minRot, int _maxRot) {

        int i;


        hasAnchor = true;
        anchorPointInPixels.setFXYZRef(_anchorPointInPixels);        
        minRot = _minRot;
        maxRot = _maxRot;

        for (i = 0; i < 4; i++) {
            applyTransform(1,true);
        }

    }

    

};


class DynObject {

public:

    FIVector4 pos;
    FIVector4 posRel;

    int r;
    int g;
    int b;

    int colPacked;
    bool isRelative;
    bool doRender;


    DynObject() {
        
    }

    void init(int _x, int _y, int _z, int _r, int _g, int _b, bool _doRender, bool _isRelative, FIVector4* _cameraPos) {
        isRelative = _isRelative;
        doRender = _doRender;

        if (isRelative) {
            posRel.setIXYZ(_x,_y,_z);
            pos.setFXYZRef(_cameraPos);
            pos.addXYZRef(&posRel);
        }
        else {
            pos.setIXYZ(_x,_y,_z);
            posRel.setIXYZ(0,0,0);
        }

        r = _r;
        g = _g;
        b = _b;

        colPacked = getPackedColor(r,g,b);
    }


};

 
