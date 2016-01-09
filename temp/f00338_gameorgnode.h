// f00338_gameorgnode.h
//

#include "f00338_gameorgnode.e"
#define LZZ_INLINE inline
GameOrgNode::GameOrgNode (GameOrgNode * _parent, int _nodeName, float _material, float _rotThe, float _rotPhi, float _rotRho, float _tanLengthInCells0, float _bitLengthInCells0, float _norLengthInCells0, float _tanLengthInCells1, float _bitLengthInCells1, float _norLengthInCells1, float _tanX, float _tanY, float _tanZ, float _bitX, float _bitY, float _bitZ, float _norX, float _norY, float _norZ)
          {
		orgVecs[E_OV_POWVALS].setFXYZW(2.0f,2.0f,2.0f,0.5f);
		orgVecs[E_OV_TBNOFFSET].setFXYZW(0.0f,0.0f,0.0f,0.0f);
		
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
		
		
		(orgVecs[E_OV_TANGENT]).setFXYZ(_tanX,_tanY,_tanZ);
		(orgVecs[E_OV_BITANGENT]).setFXYZ(_bitX,_bitY,_bitZ);
		(orgVecs[E_OV_NORMAL]).setFXYZ(_norX,_norY,_norZ);
		(orgVecs[E_OV_TANGENT]).normalize();
		(orgVecs[E_OV_BITANGENT]).normalize();
		(orgVecs[E_OV_NORMAL]).normalize();
		
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
void GameOrgNode::setTangent (float newVal)
                                      {
		int i;
		
		orgVecs[E_OV_TANGENT].setFX(newVal);
		
		for (i = 0; i < children.size(); i++) {
			children[i]->setTangent(newVal);
		}
	}
void GameOrgNode::doTransform (Singleton * singleton, GameOrgNode * tempParent)
          {
		
		int i;
		int j;
		int popCount = 0;
		int modCount;
		
		GameOrgNode* resultParent;
		
		if (tempParent == NULL) {
			resultParent = parent;
		}
		else {
			resultParent = tempParent;
		}
		
		// start
		if (resultParent == NULL) {
			orgTrans[0].setFXYZ(0.0f,0.0f,0.0f);
		}
		else {
			orgTrans[0].setFXYZRef(&(resultParent->orgTrans[2]));
		}
		
		
		/*
		E_OV_TANGENT,
		E_OV_BITANGENT,
		E_OV_NORMAL,
		*/
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
		orgTrans[1].multXYZ(orgVecs[E_OV_TBNRAD0].getFX());
		orgTrans[1].addXYZRef(&(orgTrans[0]));
		
		// end
		orgTrans[2].setFXYZRef(&(tbnRotC[0]));
		orgTrans[2].multXYZ(
			orgVecs[E_OV_TBNRAD0].getFX() +
			orgVecs[E_OV_TBNRAD1].getFX()
		);
		orgTrans[2].addXYZRef(&(orgTrans[0]));
		
		
		
		if (orgVecs[E_OV_TBNOFFSET].any()) {
			tbnOffset = orgVecs[E_OV_TBNOFFSET].getBTV();
			tempOffset = 
				tbnOffset.getX()*tbnRotC[0].getBTV() + 
				tbnOffset.getY()*tbnRotC[1].getBTV() + 
				tbnOffset.getZ()*tbnRotC[2].getBTV();
			tempFI.setBTV(tempOffset);
			for (i = 0; i < 3; i++) {
				orgTrans[i].addXYZRef(&tempFI);
			}
		}
		
		
		
		
		
		for (i = 0; i < 3; i++) {
			(tbnTrans[i]).setFXYZRef(&(tbnRotC[i]));
			//(tbnTrans[i]).multXYZ(orgVecs[E_OV_TBNRAD0][i]); //*tbnRadScale0[i]
			(tbnTrans[i]).addXYZRef(&(orgTrans[1]));
		}
		
		
		
		for (i = 0; i < children.size(); i++) {
			children[i]->doTransform(singleton, NULL);
		}
		
		
		for (i = 0; i < popCount; i++) {
			singleton->rotMatStack.pop_back();
			//singleton->transStack.pop_back();
			//singleton->rotStack.pop_back();
		}
		
	}
#undef LZZ_INLINE
 
