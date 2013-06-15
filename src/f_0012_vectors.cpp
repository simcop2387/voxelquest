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

inline istream &operator>>(istream &is, Vector3 &t) {
   is >> t[0] >> t[1] >> t[2];
   return is;
}

inline ostream &operator<<(ostream &os, const Vector3 &t) {
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
