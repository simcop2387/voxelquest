// f00340_gameplantnode.h
//

#include "f00340_gameplantnode.e"
#define LZZ_INLINE inline
GamePlantNode::GamePlantNode ()
                        {
		parent = NULL;
		children = NULL;
		upVec.setFXYZ(0.0f,0.0f,1.0f);
	}
void GamePlantNode::updateTangent (float angleInRadians)
                                                 {
		tangent.setFXYZRef(&endPoint);
		tangent.addXYZRef(&begPoint,-1.0f);
		tangent.normalize();
		baseShoot.setFXYZRef(&tangent);
		baseShoot.rotate(angleInRadians, E_PLANE_XZ);
		baseShoot.normalize();

		shootLength = begPoint.distance(&endPoint);

		// ctrPoint.copyFrom(&begPoint);
		// if (parent == NULL) {
		// 	ctrPoint.addXYZRef(&endPoint);
		// 	ctrPoint.multXYZ(0.5f);
		// }
		// else {
		// 	ctrPoint.addXYZRef(&(parent->tangent), shootLength/2.0f);
		// }
		

	}
void GamePlantNode::init (GamePlantNode * _parent, int _maxChildren, int _numChildren)
          {
		parent = _parent;
		
		maxChildren = _maxChildren;	
		numChildren = _numChildren;

		if (maxChildren > 0) {

			if (children == NULL) {
				children = new GamePlantNode[maxChildren];
			}
			else {

			}

			
		}
		
	}
#undef LZZ_INLINE
 
