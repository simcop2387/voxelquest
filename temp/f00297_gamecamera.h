// f00297_gamecamera.h
//

#include "f00297_gamecamera.e"
#define LZZ_INLINE inline
GameCamera::GameCamera ()
                 {}
void GameCamera::focusOn (int entID)
                            {}
void GameCamera::testCollision (float oldX, float oldY, float oldZ, float newX, float newY, float newZ, bool skipTest)
                                                                                                              {
        float oldPos[3] = {oldX,oldY,oldZ};
        float testPos[3] = {newX,newY,newZ};
        int i;
        
        
        if (skipTest) {
            for (i = 0; i < 3; i++) {
                
                accel[i] += (testPos[i]-oldPos[i])/4.0;
                
                accel[i] = accel[i]/1.1;
                
                unitPos[i] += accel[i];
            }
        }
    }
void GameCamera::setUnitPosition (float x, float y, float z)
                                                    {
        
        unitPos[0] = x;
        unitPos[1] = y;
        unitPos[2] = z;
    }
void GameCamera::addUnitPosition (float x, float y, float z)
                                                    {
        
        float oldX = unitPos[0];
        float oldY = unitPos[1];
        float oldZ = unitPos[2];
        
        float newX = unitPos[0]+x;// *timeValSec;
        float newY = unitPos[1]+y;// *timeValSec;
        float newZ = unitPos[2]+z;// *timeValSec;
        
        testCollision(oldX, oldY, oldZ, newX, newY, newZ, true);
        
    }
void GameCamera::setRotation (float a, float b)
                                       {
        rotation[0] = a;
        rotation[1] = b;
        
        accelA = 0.0f;
        accelB = 0.0f;
        lastAccelA = 0.0f;
        lastAccelB = 0.0f;
    }
void GameCamera::addRotation (float a, float b)
                                       {
        
        
        accelA += a;
        accelB += b;
        
        accelA = accelA / 1.5f;
        accelB = accelB / 1.5f;
        
        rotation[0] += accelA;
        rotation[1] += accelB;
    }
void GameCamera::init ()
                {
        
        accel[0] = 0.0f;
        accel[1] = 0.0f;
        accel[2] = 0.0f;
        
        setRotation(0.0f, 180.0f);
        setUnitPosition(200.0f,200.0f,200.0f);
        
    }
#undef LZZ_INLINE
 
