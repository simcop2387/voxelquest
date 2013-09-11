
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
        iv4.x /= scalar;
        iv4.y /= scalar;
        iv4.z /= scalar;

        fv4.x = (float)iv4.x;
        fv4.y = (float)iv4.y;
        fv4.z = (float)iv4.z;

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

    float distance(FIVector4 *otherVec) {

        float dx = fv4.x - otherVec->getFX();
        float dy = fv4.y - otherVec->getFY();
        float dz = fv4.z - otherVec->getFZ();

        return sqrt(dx*dx + dy*dy + dz*dz);
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



class Vector3  {
public:
	
	float e[3];

    Vector3() { e[0] = 0; e[1] = 0; e[2] = 0;}
    Vector3(float e0, float e1, float e2) {e[0]=e0; e[1]=e1; e[2]=e2;}
    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }
    void setX(float a) { e[0] = a; }
    void setY(float a) { e[1] = a; }
    void setZ(float a) { e[2] = a; }
	void setXYZ(float a, float b, float c) {
		e[0] = a;
		e[1] = b;
		e[2] = c;
	}

    inline Vector3(const Vector3 &v) {
	 e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2];
    }

    const Vector3& operator+() const { return *this; }
    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    float& operator[](int i) {  return e[i]; }
    float operator[](int i) const { return e[i];}

    Vector3& operator+=(const Vector3 &v2);
    Vector3& operator-=(const Vector3 &v2);
    Vector3& operator*=(const float t);
    Vector3& operator/=(const float t);



    float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    float squaredLength() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }

    void makeUnitVector();




    float minComponent() const { return e[indexOfMinComponent()]; }
    float maxComponent() const { return e[indexOfMaxComponent()]; }
    float maxAbsComponent() const { return e[indexOfMaxAbsComponent()]; }
    float minAbsComponent() const { return e[indexOfMinAbsComponent()]; }
    int indexOfMinComponent() const {
	return (e[0]< e[1] && e[0]< e[2]) ? 0 : (e[1] < e[2] ? 1 : 2);
    }

    int indexOfMinAbsComponent() const {
	if (fabs(e[0]) < fabs(e[1]) && fabs(e[0]) < fabs(e[2]))
	    return 0;
	else if (fabs(e[1]) < fabs(e[2]))
	    return 1;
	else
	    return 2;
    }

    int indexOfMaxComponent() const {
	return (e[0]> e[1] && e[0]> e[2]) ? 0 : (e[1] > e[2] ? 1 : 2);
    }

    int indexOfMaxAbsComponent() const {
	if (fabs(e[0]) > fabs(e[1]) && fabs(e[0]) > fabs(e[2]))
	    return 0;
	else if (fabs(e[1]) > fabs(e[2]))
	    return 1;
	else
	    return 2;
    }

    
};



inline bool operator==(const Vector3 &t1, const Vector3 &t2) {
   return ((t1[0]==t2[0])&&(t1[1]==t2[1])&&(t1[2]==t2[2]));
}

inline bool operator!=(const Vector3 &t1, const Vector3 &t2) {
   return ((t1[0]!=t2[0])||(t1[1]!=t2[1])||(t1[2]!=t2[2]));
}

inline std::istream &operator>>(std::istream &is, Vector3 &t) {
   is >> t[0] >> t[1] >> t[2];
   return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vector3 &t) {
   os << t[0] << " " << t[1] << " " << t[2];
   return os;
}

inline Vector3 unitVector(const Vector3& v) {
    float k = 1.0f / sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
    return Vector3(v.e[0]*k, v.e[1]*k, v.e[2]*k);
}

inline void Vector3::makeUnitVector() {
    float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vector3 operator*(float t, const Vector3 &v) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, float t) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator/(const Vector3 &v, float t) {
    return Vector3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}


inline float dot(const Vector3 &v1, const Vector3 &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1]  + v1.e[2] *v2.e[2];
}

inline Vector3 cross(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
		      (v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2]),
		      (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}


inline Vector3& Vector3::operator+=(const Vector3 &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    return *this;
}

inline Vector3& Vector3::operator*=(const float t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    return *this;
}

inline Vector3& Vector3::operator/=(const float t) {
    e[0]  /= t;
    e[1]  /= t;
    e[2]  /= t;
    return *this;
}

inline Vector3 reflect(const Vector3& in, const Vector3& normal)
{
  // assumes unit length normal
  return in - normal * (2 * dot(in, normal));
}
 
