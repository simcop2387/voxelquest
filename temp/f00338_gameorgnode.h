// f00338_gameorgnode.h
//

#include "f00338_gameorgnode.e"
#define LZZ_INLINE inline
GameOrgNode::GameOrgNode (GameOrgNode * _parent, int _nodeName, float _material, float _rotThe, float _rotPhi, float _rotRho, float _tanLengthInCells0, float _bitLengthInCells0, float _norLengthInCells0, float _tanLengthInCells1, float _bitLengthInCells1, float _norLengthInCells1, float _tanX, float _tanY, float _tanZ, float _bitX, float _bitY, float _bitZ, float _norX, float _norY, float _norZ)
          {
		
		
		
		orgVecs[E_OV_MATPARAMS].setFX(_material);//8.0;
		
		parent = _parent;
		nodeName = _nodeName;
		
		orgVecs[E_OV_THETAPHIRHO].setFXYZ(_rotThe,_rotPhi,_rotRho);
		orgVecs[E_OV_TPRORIG].setFXYZ(_rotThe,_rotPhi,_rotRho);
		
		// rotThe = _rotThe;
		// rotPhi = _rotPhi;
		// rotRho = _rotRho;

		

		//boneLengthHalf = _boneLength*0.5f;// *multiplier;

		orgVecs[E_OV_TBNRAD0].setFXYZ(
			_tanLengthInCells0, // *multiplier,
			_bitLengthInCells0, // *multiplier,
			_norLengthInCells0 // *multiplier
		);
		orgVecs[E_OV_TBNRAD1].setFXYZ(
			_tanLengthInCells1, // *multiplier,
			_bitLengthInCells1, // *multiplier,
			_norLengthInCells1 // *multiplier
		);
		//orgVecs[E_OV_TBNRAD1].setFXYZRef(&orgVecs[E_OV_TBNRAD0]);
		//tbnRadScale0.setFXYZ(1.0f,1.0f,1.0f);
		//tbnRadScale1.setFXYZ(1.0f,1.0f,1.0f);
		//boneLengthScale = 1.0f;
		
		
		(orgVecs[0]).setFXYZ(_tanX,_tanY,_tanZ);
		(orgVecs[1]).setFXYZ(_bitX,_bitY,_bitZ);
		(orgVecs[2]).setFXYZ(_norX,_norY,_norZ);
		(orgVecs[0]).normalize();
		(orgVecs[1]).normalize();
		(orgVecs[2]).normalize();
		
	}
GameOrgNode * GameOrgNode::addChild (int _nodeName, float _material, float _rotThe, float _rotPhi, float _rotRho, float _tanLengthInCells0, float _bitLengthInCells0, float _norLengthInCells0, float _tanLengthInCells1, float _bitLengthInCells1, float _norLengthInCells1, float _tanX, float _tanY, float _tanZ, float _bitX, float _bitY, float _bitZ, float _norX, float _norY, float _norZ)
          {
		
		//if (_nodeName >= E_BONE_C_END) {
		//	cout << "nodeName: " << _nodeName << "\n";
		//}
		
		children.push_back(
			new GameOrgNode(
				this,
				
				_nodeName,
				//_boneLength,
				
				_material,
				
				_rotThe,
				_rotPhi,
				_rotRho,
				
				_tanLengthInCells0,
				_bitLengthInCells0,
				_norLengthInCells0,
				
				_tanLengthInCells1,
				_bitLengthInCells1,
				_norLengthInCells1,
				
				_tanX, _tanY, _tanZ,
				_bitX, _bitY, _bitZ,
				_norX, _norY, _norZ
			)
		);
		
		return children.back();
	}
GameOrgNode * GameOrgNode::getNode (int _nodeName)
                                            {
		int i;
		
		// cout << "getNode("<< _nodeName << "): " << nodeName << "\n";
		
		
		if (nodeName == _nodeName) {
			return this;
		}
		
		for (i = 0; i < children.size(); i++) {
			
			if (children[i]->getNode(_nodeName) != NULL) {
				return children[i]->getNode(_nodeName);
			}
		}
		
		return NULL;
	}
void GameOrgNode::doTransform (Singleton * singleton)
          {
		
		int i;
		int j;
		int popCount = 0;
		int modCount;
		
		
		// start
		if (parent == NULL) {
			orgTrans[0].setFXYZ(0.0f,0.0f,0.0f);
		}
		else {
			orgTrans[0].setFXYZRef(&(parent->orgTrans[2]));
		}
		
		
		
		for (i = 0; i < 3; i++) {
			tbnBaseTrans[i].copyFrom(&(orgVecs[i]));
			//tbnBaseTrans[i].addXYZRef(&(orgTrans[0]));
		}
		
		
		readTBN = tbnBaseTrans;
		writeTBN = tbnRotA;
		
		
		modCount = 0;
		
		
		if (orgVecs[E_OV_THETAPHIRHO].getFZ() != 0.0f) {
			singleton->rotMatStack.push_back(RotationInfo());
			singleton->rotMatStack.back().basePoint.copyFrom(&(orgTrans[0]));
			singleton->rotMatStack.back().axisAngle.copyFrom(&(readTBN[2]));
			singleton->rotMatStack.back().axisAngle.setFW(orgVecs[E_OV_THETAPHIRHO].getFZ());
			axisRotationInstance.buildRotMatrix(&(singleton->rotMatStack.back()));
			
			popCount++;
		}
		if (orgVecs[E_OV_THETAPHIRHO].getFX() != 0.0f) {
			singleton->rotMatStack.push_back(RotationInfo());
			singleton->rotMatStack.back().basePoint.copyFrom(&(orgTrans[0]));
			singleton->rotMatStack.back().axisAngle.copyFrom(&(readTBN[1]));
			singleton->rotMatStack.back().axisAngle.setFW(orgVecs[E_OV_THETAPHIRHO].getFX());
			axisRotationInstance.buildRotMatrix(&(singleton->rotMatStack.back()));
			popCount++;
		}
		if (orgVecs[E_OV_THETAPHIRHO].getFY() != 0.0f) {
			singleton->rotMatStack.push_back(RotationInfo());
			singleton->rotMatStack.back().basePoint.copyFrom(&(orgTrans[0]));
			singleton->rotMatStack.back().axisAngle.copyFrom(&(readTBN[0]));
			singleton->rotMatStack.back().axisAngle.setFW(orgVecs[E_OV_THETAPHIRHO].getFY());
			axisRotationInstance.buildRotMatrix(&(singleton->rotMatStack.back()));
			popCount++;
		}
		
		
		for (i = singleton->rotMatStack.size()-(1); i >= 0; i--) {
			
			axisRotationInstance.applyRotation(
				writeTBN,
				readTBN,
				&(singleton->rotMatStack[i])
			);
			
			if ((modCount%2) == 0) {
				readTBN = tbnRotA;
				writeTBN = tbnRotB;
			}
			else {
				readTBN = tbnRotB;
				writeTBN = tbnRotA;
			}
			
			
			modCount++;
			
		}
		
		
		for (j = 0; j < 3; j++) {
			tbnRotC[j].setFXYZRef(&( readTBN[j] ));
			//tbnRotC[j].addXYZRef(&(orgTrans[0]),-1.0f);
			tbnRotC[j].normalize();
		}
		
		
		// middle
		orgTrans[1].setFXYZRef(&(tbnRotC[0]));
		orgTrans[1].multXYZ(orgVecs[E_OV_TBNRAD0].getFX()); //*boneLengthScale
		orgTrans[1].addXYZRef(&(orgTrans[0]));
		
		// end
		orgTrans[2].setFXYZRef(&(tbnRotC[0]));
		orgTrans[2].multXYZ(
			orgVecs[E_OV_TBNRAD0].getFX() +
			orgVecs[E_OV_TBNRAD1].getFX()
		); //*boneLengthScale
		orgTrans[2].addXYZRef(&(orgTrans[0]));
		
		for (i = 0; i < 3; i++) {
			(tbnTrans[i]).setFXYZRef(&(tbnRotC[i]));
			(tbnTrans[i]).multXYZ(orgVecs[E_OV_TBNRAD0][i]); //*tbnRadScale0[i]
			(tbnTrans[i]).addXYZRef(&(orgTrans[1]));
		}
		
		
		
		for (i = 0; i < children.size(); i++) {
			children[i]->doTransform(singleton);
		}
		
		
		for (i = 0; i < popCount; i++) {
			singleton->rotMatStack.pop_back();
			//singleton->transStack.pop_back();
			//singleton->rotStack.pop_back();
		}
		
	}
#undef LZZ_INLINE
 
