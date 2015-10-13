class GameEnt {
private:

	FIVector4 boundsMinInPixels;
	FIVector4 boundsMaxInPixels;
	FIVector4 visMinInPixels;
	FIVector4 visMaxInPixels;
	
	
	
	
	
public:
	
	int templateId;
	FIVector4 templatePos;
	
	int entType;
	//int toggleState;
	//int maxToggleStates;
	
	bool toggled;
	bool visible;
	//bool isFalling;
	
	float camDistance;

	
	
	FIVector4 geomParams[E_GP_LENGTH];

	FIVector4 anchorPointInPixels;
	FIVector4 moveMinInPixels;
	FIVector4 moveMaxInPixels;

	GameEnt *light;

	
	
	bool hasAnchor;

	//   1
	// 2   0
	//   3


	int rotDir;
	int minRot;
	int maxRot;
	int curRot;
	
	int buildingType;


	FIVector4 tempVec1;
	FIVector4 tempVec2;
	FIVector4 tempVec3;
	FIVector4 tempVec4;
	FIVector4 tempVec5;
	

	void setLightPos(FIVector4* newPos) {
		geomParams[E_LP_POSITION].copyFrom(newPos);
		geomParams[E_CP_VISMININPIXELST].copyFrom(newPos);
		geomParams[E_CP_VISMAXINPIXELST].copyFrom(newPos);
		
		geomParams[E_CP_VISMININPIXELST].addXYZRef(&(geomParams[E_LP_RADIUS]),-1.0f);
		geomParams[E_CP_VISMAXINPIXELST].addXYZRef(&(geomParams[E_LP_RADIUS]),1.0f);
		
		
		moveMinInPixels.setFXYZRef(&(geomParams[E_CP_VISMININPIXELST]));
		moveMaxInPixels.setFXYZRef(&(geomParams[E_CP_VISMAXINPIXELST]));
		
	}

	void initLight(
		FIVector4 *position,
		FIVector4 *color,
		float radius
	) {
		
		geomParams[E_LP_COLOR].copyFrom(color);
		geomParams[E_LP_RADIUS].setFXYZ(radius,radius,radius);
		
		setLightPos(position);
		
	}
	
	
	
	
	
	
	
	
	
	

	
	FIVector4 *getVisMinInPixelsT() {
		return &geomParams[E_CP_VISMININPIXELST];
	}
	FIVector4 *getVisMaxInPixelsT() {
		return &geomParams[E_CP_VISMAXINPIXELST];
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
					curRot = maxRot - 1;
					rotDir *= -1;
				}
				if (curRot < minRot) {
					curRot = minRot + 1;
					rotDir *= -1;

				}

			}


		}
		else {
			cout << "Attemped to rotate without anchor.\n";
		}


	}
	
	
	
	
	
	
	GameEnt() {
		
		templateId = 0;
		
		light = NULL;
		//toggleState = 0;
		//maxToggleStates = 2;
		toggled = false;
		visible = true;
		//isFalling = false;
	}
	
	

	void initBounds(
		int _buildingType,
		int alignBottomMiddleTop,

		float _zOffset,

		FIVector4 *p1,
		FIVector4 *p2,
		FIVector4 *rad,
		FIVector4 *_cornerDisInPixels,
		FIVector4 *_visInsetFromMin,
		FIVector4 *_visInsetFromMax,
		FIVector4 *_powerVals,
		FIVector4 *_powerVals2,
		FIVector4 *_thickVals,
		FIVector4 *_matParams,
		FIVector4 *_anchorPoint,
		int _minRot,
		int _maxRot

	) {
		buildingType = _buildingType;
		float temp;
		float zOffset = _zOffset;
		
		

		curRot = 0;
		rotDir = 1;
		hasAnchor = false;

		anchorPointInPixels.setFXYZ(0.0f, 0.0f, 0.0f);


		boundsMinInPixels.setFXYZRef(p1);
		boundsMaxInPixels.setFXYZRef(p2);

		FIVector4::normalizeBounds(&boundsMinInPixels, &boundsMaxInPixels);




		boundsMinInPixels.addXYZRef(rad, -1.0f);
		boundsMaxInPixels.addXYZRef(rad, 1.0f);

		switch (alignBottomMiddleTop) {

		case E_ALIGN_BOTTOM: // bottom _$_
			zOffset += (rad->getFZ() - _visInsetFromMin->getFZ());
			break;
		case E_ALIGN_MIDDLE: // middle -$-
			zOffset += 0.0f;
			break;
			//               ___
		case E_ALIGN_TOP: // top  $
			zOffset += -(rad->getFZ() - _visInsetFromMax->getFZ());
			break;


		}

		boundsMinInPixels.addXYZ(0.0f, 0.0f, zOffset);
		boundsMaxInPixels.addXYZ(0.0f, 0.0f, zOffset);


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
		geomParams[E_CP_VISMININPIXELST].setFXYZRef(&visMinInPixels);
		geomParams[E_CP_VISMAXINPIXELST].setFXYZRef(&visMaxInPixels);


		if (_minRot != _maxRot) {
			initAnchorPoint(_anchorPoint, _minRot, _maxRot);
		}

	}


	void initLines(
		int _buildingType,
		float scale,
		
		FIVector4 *_offset,

		FIVector4 *_orgVec,
		FIVector4 *_tanVec, // already scaled
		FIVector4 *_bitVec,
		FIVector4 *_norVec,
		FIVector4 *_radVec0,
		FIVector4 *_radVec1,
		//FIVector4 *_radVecScale0,
		//FIVector4 *_radVecScale1,
		FIVector4 *_matParams



	) {
		buildingType = _buildingType;
		float temp;
		float radMax;
		
		

		curRot = 0;
		rotDir = 1;
		hasAnchor = false;

		anchorPointInPixels.setFXYZ(0.0f, 0.0f, 0.0f);

		tempVec1.setFXYZRef(_orgVec);
		tempVec2.setFXYZRef(_radVec0);
		tempVec3.setFXYZRef(_radVec1);
		
		//tempVec2.multXYZ(_radVecScale0);
		//tempVec3.multXYZ(_radVecScale1);
		
		tempVec1.multXYZ(scale);
		tempVec2.multXYZ(scale);
		tempVec3.multXYZ(scale);
		
		tempVec1.addXYZRef(_offset);
		

		boundsMinInPixels.setFXYZRef(&tempVec1);
		boundsMaxInPixels.setFXYZRef(&tempVec1);
		
		tempVec4.copyFrom(_tanVec);
		tempVec5.copyFrom(_tanVec);
		tempVec4.multXYZ(-_radVec0->getFX()*scale);
		tempVec5.multXYZ(_radVec1->getFX()*scale);
		
		boundsMinInPixels.addXYZRef(&tempVec4);
		boundsMaxInPixels.addXYZRef(&tempVec5);
		
		FIVector4::normalizeBounds(&boundsMinInPixels, &boundsMaxInPixels);

		radMax = max(
			max(
				max(tempVec2[0], tempVec2[1]),
				max(tempVec3[0], tempVec3[1])
			),
			max(tempVec2[2], tempVec3[2])
		);
		
		boundsMinInPixels.addXYZ(-radMax);
		boundsMaxInPixels.addXYZ(radMax);

		visMinInPixels.setFXYZRef(&boundsMinInPixels);
		visMaxInPixels.setFXYZRef(&boundsMaxInPixels);

		moveMinInPixels.setFXYZRef(&boundsMinInPixels);
		moveMaxInPixels.setFXYZRef(&boundsMaxInPixels);




		geomParams[E_AP_ORG].setFXYZRef(&tempVec1);
		geomParams[E_AP_TAN].setFXYZRef(_tanVec);
		geomParams[E_AP_BIT].setFXYZRef(_bitVec);
		geomParams[E_AP_NOR].setFXYZRef(_norVec);
		geomParams[E_AP_RAD0].setFXYZRef(&tempVec2);
		geomParams[E_AP_RAD1].setFXYZRef(&tempVec3);
		geomParams[E_AP_MATPARAMS].setFXYZRef(_matParams);
		geomParams[E_CP_VISMININPIXELST].setFXYZRef(&visMinInPixels);
		geomParams[E_CP_VISMAXINPIXELST].setFXYZRef(&visMaxInPixels);


	}


	void initTree(
		int _buildingType,

		// p0, p1 = start, end
		// p2 = control point or tangent

		FIVector4 *p0,
		FIVector4 *p1,
		FIVector4 *p2,

		float radP0,
		float radP1,
		float sphereRad,

		FIVector4 *_matParams



	) {
		buildingType = _buildingType;
		float temp;

		float radMax = max(max(radP0, radP1), sphereRad);

		curRot = 0;
		rotDir = 1;
		hasAnchor = false;

		anchorPointInPixels.setFXYZ(0.0f, 0.0f, 0.0f);


		boundsMinInPixels.setFXYZRef(p0);
		boundsMaxInPixels.setFXYZRef(p1);

		//boundsMinInPixels.addXYZ(-radMax);
		//boundsMaxInPixels.addXYZ(radMax);

		FIVector4::normalizeBounds(&boundsMinInPixels, &boundsMaxInPixels);

		FIVector4::growBoundary(&boundsMinInPixels, &boundsMaxInPixels, p2, p2);

		boundsMinInPixels.addXYZ(-radMax);
		boundsMaxInPixels.addXYZ(radMax);



		visMinInPixels.setFXYZRef(&boundsMinInPixels);
		visMaxInPixels.setFXYZRef(&boundsMaxInPixels);


		geomParams[E_TP_P0].setFXYZRef(p0);
		geomParams[E_TP_P1].setFXYZRef(p1);
		geomParams[E_TP_P2].setFXYZRef(p2);
		geomParams[E_TP_THICKVALS].setFXYZ(radP0, radP1, sphereRad);

		geomParams[E_TP_MATPARAMS].setFXYZRef(_matParams);

		moveMinInPixels.setFXYZRef(&boundsMinInPixels);
		moveMaxInPixels.setFXYZRef(&boundsMaxInPixels);
		geomParams[E_CP_VISMININPIXELST].setFXYZRef(&visMinInPixels);
		geomParams[E_CP_VISMAXINPIXELST].setFXYZRef(&visMaxInPixels);


	}

	void toggle() {
		// toggleState++;
		// cout << "toggleState " << toggleState << "\n";
		// if (toggleState >= maxToggleStates) {
		// 	toggleState = 0;
		// }
		// cout << "toggleState " << toggleState << "\n";
		
		toggled = !toggled;
	}

	void toggleTransform() {
		toggle();
		applyTransform(rotDir, false);
	}

	void applyTransform(int rotMod, bool ignoreConstraints) {

		rotate(rotMod, ignoreConstraints);

		geomParams[E_GP_BOUNDSMININPIXELST].setFXYZRef(&boundsMinInPixels);
		geomParams[E_GP_BOUNDSMAXINPIXELST].setFXYZRef(&boundsMaxInPixels);
		geomParams[E_CP_VISMININPIXELST].setFXYZRef(&visMinInPixels);
		geomParams[E_CP_VISMAXINPIXELST].setFXYZRef(&visMaxInPixels);

		geomParams[E_GP_BOUNDSMININPIXELST].addXYZRef(&anchorPointInPixels, -1.0f);
		geomParams[E_GP_BOUNDSMAXINPIXELST].addXYZRef(&anchorPointInPixels, -1.0f);
		geomParams[E_CP_VISMININPIXELST].addXYZRef(&anchorPointInPixels, -1.0f);
		geomParams[E_CP_VISMAXINPIXELST].addXYZRef(&anchorPointInPixels, -1.0f);

		geomParams[E_GP_BOUNDSMININPIXELST].rotate90(getClampedRot());
		geomParams[E_GP_BOUNDSMAXINPIXELST].rotate90(getClampedRot());
		geomParams[E_CP_VISMININPIXELST].rotate90(getClampedRot());
		geomParams[E_CP_VISMAXINPIXELST].rotate90(getClampedRot());

		geomParams[E_GP_BOUNDSMININPIXELST].addXYZRef(&anchorPointInPixels, 1.0f);
		geomParams[E_GP_BOUNDSMAXINPIXELST].addXYZRef(&anchorPointInPixels, 1.0f);
		geomParams[E_CP_VISMININPIXELST].addXYZRef(&anchorPointInPixels, 1.0f);
		geomParams[E_CP_VISMAXINPIXELST].addXYZRef(&anchorPointInPixels, 1.0f);

		FIVector4::normalizeBounds(&geomParams[E_GP_BOUNDSMININPIXELST], &geomParams[E_GP_BOUNDSMAXINPIXELST]);
		FIVector4::normalizeBounds(&geomParams[E_CP_VISMININPIXELST], &geomParams[E_CP_VISMAXINPIXELST]);

		FIVector4::growBoundary(&moveMinInPixels, &moveMaxInPixels, &geomParams[E_CP_VISMININPIXELST], &geomParams[E_CP_VISMAXINPIXELST]);
	}

	void initAnchorPoint(FIVector4 *_anchorPointInPixels, int _minRot, int _maxRot) {

		int i;


		hasAnchor = true;
		anchorPointInPixels.setFXYZRef(_anchorPointInPixels);
		minRot = _minRot;
		maxRot = _maxRot;

		for (i = 0; i < 4; i++) {
			applyTransform(1, true);
		}

	}
	
	
	
	
	
	
	
	
	
	
};

struct EntVec {
	std::vector<GameEnt> data;	
};



class EntSelection {
public:
	int selEntListInd;
	std::vector<GameEnt *> selEntList;
	std::map<intPair, int> selEntMap;
	
	EntSelection() {
		selEntListInd = 0;
	}
	
	GameEnt* getSelectedEnt() {
		if (selEntList.size() > 0) {
			return selEntList[selEntListInd];
		}
		else {
			return NULL;
		}
	}
	
	void cycleEnts() {
		selEntListInd++;
		if (selEntListInd >= selEntList.size()) {
			selEntListInd = 0;
		}
	}
};



class DynObject {

public:

	FIVector4 pos;
	FIVector4 color;
	FIVector4 posRel;
	FIVector4 posTrackball;

	int r;
	int g;
	int b;

	//bool isTrackball;
	//bool isRelative;
	int moveType;
	bool doRender;
	bool firstRun;

	float radius;
	float lightRadius;

	GameEnt childLight;


	DynObject() {
		firstRun = true;
	}

	GameEnt *getLight() {
		
		if (firstRun) {
			childLight.toggled = true;
			firstRun = false;
		}
		childLight.initLight(
			&pos,
			&color,
			lightRadius
		);
		
		
		return &childLight;
	}

	void init(
		int _x, int _y, int _z,
		int _r, int _g, int _b,
		bool _doRender, int _moveType,//bool _isRelative, bool _isTrackball,
		FIVector4 *_cameraPos,
		float _radius,
		float _lightRadius=0.0f
	) {
		//isRelative = _isRelative;
		//isTrackball = _isTrackball;
		doRender = _doRender;
		moveType = _moveType;
		
		lightRadius = _lightRadius;

		if (moveType == E_MT_RELATIVE) {
			posRel.setIXYZ(_x, _y, _z);
			pos.setFXYZRef(_cameraPos);
			pos.addXYZRef(&posRel);
		}
		else {
			pos.setIXYZ(_x, _y, _z);
			posRel.setIXYZ(0, 0, 0);
		}

		radius = _radius;

		r = _r;
		g = _g;
		b = _b;

		color.setFXYZ(
			((float)r),
			((float)g),
			((float)b)
		);

		
	}


};

