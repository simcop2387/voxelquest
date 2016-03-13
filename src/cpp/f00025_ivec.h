





///////////////////////////////////////////////////////////////////////////////
// 3D vector
///////////////////////////////////////////////////////////////////////////////
struct VectorI3
{
    int x;
    int y;
    int z;

    // ctors
    VectorI3() : x(0), y(0), z(0) {};
    VectorI3(int x, int y, int z) : x(x), y(y), z(z) {};

    // utils functions
    void        set(int x, int y, int z);
    void        mod(int scale);
    void        doAbs();
    int       length() const;                         //
    int       distance(const VectorI3& vec) const;     // distance between two vectors
    VectorI3&    normalize();                            //
    int       dot(const VectorI3& vec) const;          // dot product
    VectorI3     cross(const VectorI3& vec) const;        // cross product
    bool        equal(const VectorI3& vec, int e) const; // compare with epsilon

    // operators
    VectorI3     operator-() const;                      // unary operator (negate)
    VectorI3     operator+(const VectorI3& rhs) const;    // add rhs
    VectorI3     operator-(const VectorI3& rhs) const;    // subtract rhs
    VectorI3&    operator+=(const VectorI3& rhs);         // add rhs and update this object
    VectorI3&    operator-=(const VectorI3& rhs);         // subtract rhs and update this object
    
    VectorI3     operator+(const int rhs) const;    // add rhs
    VectorI3     operator-(const int rhs) const;    // subtract rhs
    VectorI3&    operator+=(const int rhs);         // add rhs and update this object
    VectorI3&    operator-=(const int rhs);         // subtract rhs and update this object
    
    VectorI3     operator*(const int scale) const;     // scale
    VectorI3     operator*(const VectorI3& rhs) const;    // multiplay each element
    VectorI3&    operator*=(const int scale);          // scale and update this object
    VectorI3&    operator*=(const VectorI3& rhs);         // product each element and update this object
    VectorI3     operator/(const int scale) const;     // inverse scale
    VectorI3&    operator/=(const int scale);          // scale and update this object
    bool        operator==(const VectorI3& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const VectorI3& rhs) const;   // exact compare, no epsilon
    bool        operator<(const VectorI3& rhs) const;    // comparison for sort
    int       operator[](int index) const;            // subscript operator v[0], v[1]
    int&      operator[](int index);                  // subscript operator v[0], v[1]

    friend VectorI3 operator*(const int a, const VectorI3 vec);
    friend std::ostream& operator<<(std::ostream& os, const VectorI3& vec);
};

inline VectorI3 VectorI3::operator-() const {
    return VectorI3(-x, -y, -z);
}

inline VectorI3 VectorI3::operator+(const VectorI3& rhs) const {
    return VectorI3(x+rhs.x, y+rhs.y, z+rhs.z);
}

inline VectorI3 VectorI3::operator-(const VectorI3& rhs) const {
    return VectorI3(x-rhs.x, y-rhs.y, z-rhs.z);
}

inline VectorI3& VectorI3::operator+=(const VectorI3& rhs) {
    x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline VectorI3& VectorI3::operator-=(const VectorI3& rhs) {
    x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
}




inline VectorI3 VectorI3::operator+(const int rhs) const {
    return VectorI3(x+rhs, y+rhs, z+rhs);
}

inline VectorI3 VectorI3::operator-(const int rhs) const {
    return VectorI3(x-rhs, y-rhs, z-rhs);
}

inline VectorI3& VectorI3::operator+=(const int rhs) {
    x += rhs; y += rhs; z += rhs; return *this;
}

inline VectorI3& VectorI3::operator-=(const int rhs) {
    x -= rhs; y -= rhs; z -= rhs; return *this;
}







inline VectorI3 VectorI3::operator*(const int a) const {
    return VectorI3(x*a, y*a, z*a);
}

inline VectorI3 VectorI3::operator*(const VectorI3& rhs) const {
    return VectorI3(x*rhs.x, y*rhs.y, z*rhs.z);
}

inline VectorI3& VectorI3::operator*=(const int a) {
    x *= a; y *= a; z *= a; return *this;
}

inline VectorI3& VectorI3::operator*=(const VectorI3& rhs) {
    x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
}

inline VectorI3 VectorI3::operator/(const int a) const {
    return VectorI3(x/a, y/a, z/a);
}

inline VectorI3& VectorI3::operator/=(const int a) {
    x /= a; y /= a; z /= a; return *this;
}

inline bool VectorI3::operator==(const VectorI3& rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

inline bool VectorI3::operator!=(const VectorI3& rhs) const {
    return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

inline bool VectorI3::operator<(const VectorI3& rhs) const {
    if(x < rhs.x) return true;
    if(x > rhs.x) return false;
    if(y < rhs.y) return true;
    if(y > rhs.y) return false;
    if(z < rhs.z) return true;
    if(z > rhs.z) return false;
    return false;
}

inline int VectorI3::operator[](int index) const {
    return (&x)[index];
}

inline int& VectorI3::operator[](int index) {
    return (&x)[index];
}

inline void VectorI3::set(int x, int y, int z) {
    this->x = x; this->y = y; this->z = z;
}

inline void VectorI3::mod(int scale) {
    this->x = (this->x % scale);
    this->y = (this->y % scale);
    this->z = (this->z % scale);
}

inline void VectorI3::doAbs() {
    this->x = abs(this->x);
    this->y = abs(this->y);
    this->z = abs(this->z);
}

inline int VectorI3::length() const {
    return sqrtf(x*x + y*y + z*z);
}

inline int VectorI3::distance(const VectorI3& vec) const {
    return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y) + (vec.z-z)*(vec.z-z));
}

inline VectorI3& VectorI3::normalize() {
    int xxyyzz = x*x + y*y + z*z;
    //int invLength = invSqrt(xxyyzz);
    int invLength = 1.0f / sqrtf(xxyyzz);
    x *= invLength;
    y *= invLength;
    z *= invLength;
    return *this;
}

inline int VectorI3::dot(const VectorI3& rhs) const {
    return (x*rhs.x + y*rhs.y + z*rhs.z);
}

inline VectorI3 VectorI3::cross(const VectorI3& rhs) const {
    return VectorI3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

inline bool VectorI3::equal(const VectorI3& rhs, int epsilon) const {
    return (abs(x - rhs.x) <= epsilon) && (abs(y - rhs.y) <= epsilon) && (abs(z - rhs.z) <= epsilon);
}

inline VectorI3 operator*(const int a, const VectorI3 vec) {
    return VectorI3(a*vec.x, a*vec.y, a*vec.z);
}

inline std::ostream& operator<<(std::ostream& os, const VectorI3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

const static VectorI3 DIR_VECS_IV[NUM_ORIENTATIONS] = {
    VectorI3(1, 0, 0),
    VectorI3(-1, 0, 0),
    VectorI3(0, 1, 0),
    VectorI3(0, -1, 0),
    VectorI3(0, 0, 1),
    VectorI3(0, 0, -1)
};


