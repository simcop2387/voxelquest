
class UIComponent {
private:
	UIComponent* valuePtr;
	
	float privValueX;
	float privValueY;
	
	std::vector<int> _children;
	std::vector<int> _floatingChildren;
	
public:
	
	

	Singleton* singleton;

	Singleton::UICont uiCont;

	string uid;
	string ss;
	string text; // resulting text
	string label; // base label
	string dataFile;
	string dataRef;
	string dataKey;
	string valRef;
	
	int matCode;
	int parentId;
	int nodeId;
	int index;
	int fillDir;
	int layer;
	int layerId;
	int hoverType;
	int guiClass;
	//int guiId;
	
	bool forceDragUpdate;
	bool selected;
	bool foundParent;
	bool foundValuePtr;
	bool visible;
	bool enabled;
	
	iVector2 align;

	FIVector4 valVec;
	//FIVector4* valVecPtr;
	FIVector4 valVecMask;
	
	


	JSONValue* jvNodeNoTemplate;

	fVector2 scrollMaskY;
	fVector2 dragStart;
	fVector2 lastDrag;
	fVector2 dragOffset;
	
	
	fVector2 totOffset;
	fVector2 targScrollOffset;
	fVector2 scrollOffset;
	fVector2 floatOffset;
	fVector2 originPos;
	fVector2 resultDimInPixels;
	fVector2 textDimInPixels;
	fVector2 rmDimInPixels; // resulting minimum dim
	fVector2 minDimInPixels;
	fVector2 maxDimInPixels;
	fVector2 fillRatioDim;
	FontWrapper* curFont;
	FontWrapper* curFontIcons;

	bool dragging;
	bool overChild;
	bool overSelf;
	bool singleLine;	
	bool hasBackground;
	bool mouseOver;
	bool mouseDown;
	bool wasHit;
	bool isDirty;
	bool isFloating;
	bool dataLoaded;
	
	uint flags;
	
	float divisions;
	float paddingInPixels; // inner
	float borderInPixels;
	float marginInPixels; // outer
	float spaceForCharsInPixels;

	
	iVector2 spacing;
	std::vector<string> lineVec;
	std::vector< std::vector<string> > wordVec;
	std::vector<float> linePitchVec;

	fBoundingBox hitBounds;
	StyleSheetResult resSS;
	
	
	
	


	UIComponent() {
		singleton = NULL;
		valuePtr = NULL;
	}


	

	void init(

		Singleton* _singleton,
		
		int _parentId,
		int _nodeId,
		int _index,
		
		JSONValue* _jvNodeNoTemplate, // original node without template applied to it
		bool _isFloating,
		
		
		string* stringVals,
		double* floatVals
		
	) {
		
		enabled = true;
		
		_children.clear();
		_floatingChildren.clear();
		
		dataLoaded = false;
		
		//valVec = NULL;
		
		privValueY = 0.0f;
		selected = false;
		forceDragUpdate = false;
		
		
		//valVecPtr = NULL;
		valuePtr = NULL;
		foundParent = false;
		foundValuePtr = false;
		

		singleton = _singleton;
		parentId = _parentId;
		nodeId = _nodeId;
		
		flags = floatVals[E_GFT_FLAGS];
		
		
		
		if ((flags&E_GF_X) == 0) {
			valVecMask.setFX(0.0f);
		}
		else {
			valVecMask.setFX(1.0f);
		}
		if ((flags&E_GF_Y) == 0) {
			valVecMask.setFY(0.0f);
		}
		else {
			valVecMask.setFY(1.0f);
		}
		if ((flags&E_GF_Z) == 0) {
			valVecMask.setFZ(0.0f);
		}
		else {
			valVecMask.setFZ(1.0f);
		}
		if ((flags&E_GF_W) == 0) {
			valVecMask.setFW(0.0f);
		}
		else {
			valVecMask.setFW(1.0f);
		}
		
		matCode = floatVals[E_GFT_MATCODE];
		
		valVec.setFXYZW(
			floatVals[E_GFT_VALUE0],
			floatVals[E_GFT_VALUE1],
			floatVals[E_GFT_VALUE2],
			floatVals[E_GFT_VALUE3]
		);
		
		
		
		layerId = -1;
		
		jvNodeNoTemplate = _jvNodeNoTemplate;
		
		layer = floatVals[E_GFT_LAYER];
		hoverType = floatVals[E_GFT_HOVERTYPE];
		isFloating = _isFloating;
		
		ss = stringVals[E_GST_SS];
		label = stringVals[E_GST_LABEL];
		text = label;
		dataRef = stringVals[E_GST_DATAREF];
		dataFile = stringVals[E_GST_DATAFILE];
		dataKey = stringVals[E_GST_DATAKEY];
		valRef = stringVals[E_GST_VALREF];
		
		
		uid = stringVals[E_GST_UID];
		index = _index;
		
		if (uid.size() > 0) {
			
			if (uid[0] == '#') {
				
			}
			else {
				singleton->compMap[uid].nodeId = nodeId;
			}
			
			
		}
		
		
		
		maxDimInPixels.x = floatVals[E_GFT_MAXDIMX];
		maxDimInPixels.y = floatVals[E_GFT_MAXDIMY];
		
		minDimInPixels.x = floatVals[E_GFT_MINDIMX];
		minDimInPixels.y = floatVals[E_GFT_MINDIMY];
		
		
		guiClass = floatVals[E_GFT_TYPE];
		//guiId = _guiId;
		
		
		dragging = false;
		overSelf = false;
		overChild = false;
		isDirty = true;
		visible = false;//(hoverType == E_HT_NORMAL);
		
		hasBackground = floatVals[E_GFT_HASBACKGROUND];
		fillRatioDim.x = floatVals[E_GFT_FILLRATIOX];
		fillRatioDim.y = floatVals[E_GFT_FILLRATIOY];

		singleLine = floatVals[E_GFT_SINGLELINE];
		
		
		curFont = singleton->fontWrappers[EFW_TEXT];
		curFontIcons = singleton->fontWrappers[EFW_ICONS];
		
		wasHit = false;
		privValueX = floatVals[E_GFT_VALUE];
		divisions = floatVals[E_GFT_DIVISIONS];

		mouseDown = false;
		mouseOver = false;

		resSS.init(&(singleton->styleSheetMap[ss]));
		
		
		paddingInPixels = resSS.baseSS->compStates[E_COMP_UP].props[E_SS_PADDING];
		borderInPixels = resSS.baseSS->compStates[E_COMP_UP].props[E_SS_BORDER];
		marginInPixels = resSS.baseSS->compStates[E_COMP_UP].props[E_SS_MARGIN];
		
		
		/////////////////
		
		totOffset.x = 0.0f;
		totOffset.y = 0.0f;
		
		
		targScrollOffset.x = 0.0f;
		targScrollOffset.y = 0.0f;
		
		scrollOffset.x = 0.0f;
		scrollOffset.y = 0.0f;
		
		scrollMaskY.x = 0.0;
		scrollMaskY.y = 0.0;
		
		dragStart.x = 0.0f;
		dragStart.y = 0.0f;
		
		lastDrag.x = 0.0f;
		lastDrag.y = 0.0f;
		
		dragOffset.x = 0.0f;
		dragOffset.y = 0.0f;
		
		floatOffset.x = 0.0f;
		floatOffset.y = 0.0f;
		
		originPos.x = 0.0f;
		originPos.y = 0.0f;
		spaceForCharsInPixels = 200.0f;
		
		/////////////////


		hitBounds.xMin = 0.0f;
		hitBounds.xMax = 0.0f;
		hitBounds.yMin = 0.0f;
		hitBounds.yMax = 0.0f;


		align.x = floatVals[E_GFT_ALIGNX];
		align.y = floatVals[E_GFT_ALIGNY];
		fillDir = floatVals[E_GFT_FILLDIR];
		
		spacing.x = 0.0f;
		spacing.y = 0.0f;

		
		if (isFloating) {
			resultDimInPixels.x = singleton->guiWinW;
			resultDimInPixels.y = singleton->guiWinH;
			originPos.x = 0.0;
			originPos.y = 0.0;
		}
		
		//
		
		
	}
	
	
	void clearChildren() {
		_children.clear();
		_floatingChildren.clear();
	}
	
	UIComponent* getChild(int ind) {
		
		int curInd = _children[ind];
		
		//if (singleton->compStack[curInd].isValid) {
			return (singleton->compStack[curInd].data);
		// }
		// else {
		// 	return NULL;
		// }
	}
	UIComponent* getFloatingChild(int ind) {
		
		int curInd = _floatingChildren[ind];
		
		//if (singleton->compStack[curInd].isValid) {
			return (singleton->compStack[curInd].data);
		// }
		// else {
		// 	return NULL;
		// }
	}
	int getChildCount() {
		return _children.size();
	}
	int getFloatingChildCount() {
		return _floatingChildren.size();
	}
	
	
	
	
	
	float getDimYClamped(float val) {
		if (!enabled) {
			return 0.0f;
		}
		
		if (maxDimInPixels.y == 0) {
			return max(val,minDimInPixels.y);
		}
		else {
			return max(min(maxDimInPixels.y, val),minDimInPixels.y);
		}
	}
	
	float getResultDimYClamped() {
		if (!enabled) {
			return 0.0f;
		}
		
		if (maxDimInPixels.y == 0) {
			return max(resultDimInPixels.y,minDimInPixels.y);
		}
		else {
			return max(min(maxDimInPixels.y, resultDimInPixels.y),minDimInPixels.y);
		}
	}
	
	// float getMinDimYClamped() {
	// 	if (maxDimInPixels.y == 0) {
	// 		return minDimInPixels.y;
	// 	}
	// 	else {
	// 		return min(maxDimInPixels.y, minDimInPixels.y);
	// 	}
	// }
	
	
	string findKeyString(int valEnum) {
		string resString = "";
		UIComponent* curParent = getParent();
		
		
		while (
			(curParent != NULL)
		) {
			
			switch (valEnum) {
				case E_GST_DATAREF:
					if (curParent->dataRef.compare("") != 0) {
						return curParent->dataRef;
					}
				break;
				case E_GST_DATAFILE:
					if (curParent->dataFile.compare("") != 0) {
						return curParent->dataFile;
					}
				break;
			}
			
			curParent = curParent->getParent();
		}
		
		return "";
	}
	
	void updateLinkedValues(bool isRead = false) {
		int k;
		
		JSONValue* jvFileBase = NULL;
		JSONValue* jvNodeBase = NULL;
		
		
		
		
		if (dataKey.compare("") == 0) {
			
		}
		else {
			
			if (dataFile.compare("") == 0) {
				dataFile = findKeyString(E_GST_DATAFILE);
			}
			
			if (dataRef.compare("") == 0) {
				dataRef = findKeyString(E_GST_DATAREF);
			}
			
			if (
				(dataFile.compare("") == 0) ||
				(dataRef.compare("") == 0)
			) {
				cout << "missing dataFile or dataRef";
			}
			else {
				
				
				jvFileBase = singleton->fetchJSONData(dataFile, false);
				singleton->getJVNodeByString(jvFileBase,&jvNodeBase,dataRef + "." + dataKey);
				
				// if (isRead) {
				// 	cout << "DF " << dataFile << "\n";
				// }
				
				
				if (jvNodeBase != NULL) {
					
					if (isRead) {
						
						//cout << "yay\n";
						
						if (jvNodeBase->IsNumber()) {
							setValue(jvNodeBase->number_value);
						}
						if (jvNodeBase->IsArray()) {
							for (k = 0; k < jvNodeBase->array_value.size(); k++) {
								valVec.setIndex(k,jvNodeBase->array_value[k]->number_value);
							}
						}
					}
					else {
						if (jvNodeBase->IsNumber()) {
							jvNodeBase->number_value = getValue();
						}
						if (jvNodeBase->IsArray()) {
							for (k = 0; k < jvNodeBase->array_value.size(); k++) {
								jvNodeBase->array_value[k]->number_value = valVec[k];
							}
						}
					}
					
					
					
				}
				
			}
			
			
			
		}
	}
	
	void setValueIndex(int ind, float val) {
		valVec.setIndex(ind,val);
		updateLinkedValues();	
	}
	float getValueIndex(int ind) {
		
		if (!dataLoaded) {
			dataLoaded = true;
			updateLinkedValues(true);
		}
		
		return valVec[ind];
	}
	float getValueIndexPtr(int ind) {
		
		UIComponent* curValuePtr = getValuePtr();
		if (curValuePtr != NULL) {
			return curValuePtr->getValueIndex(ind);
		}
		else {
			return getValueIndex(ind);
		}
		
		
	}
	
	void setValue(float _value, bool doEventDispatch = false, bool preventRefresh = false) {
		UIComponent* curValuePtr = getValuePtr();
		
		if (curValuePtr != NULL) {
			
			if ((flags&E_GF_X) != 0) {
				curValuePtr->setValueIndex(0,_value);
			}
			else if ((flags&E_GF_Y) != 0) {
				curValuePtr->setValueIndex(1,_value);
			}
			else if ((flags&E_GF_Z) != 0) {
				curValuePtr->setValueIndex(2,_value);
			}
			else if ((flags&E_GF_W) != 0) {
				curValuePtr->setValueIndex(3,_value);
			}
			
			
		}
		else {
			privValueX = _value;
			
			updateLinkedValues();
			
			if (doEventDispatch) {
				singleton->dispatchEvent(GLUT_LEFT_BUTTON, GLUT_UP, 0, 0, this, true, preventRefresh);
			}
		}
		
		
	}
	float getValue() {
		
		if (!dataLoaded) {
			dataLoaded = true;
			updateLinkedValues(true);
		}
		
		UIComponent* highestCont = NULL;
		UIComponent* highestContPar = NULL;
		
		if (matCode == E_MC_MATERIAL) {
			highestCont = getHighestCont(this, 0);
			if (highestCont != NULL) {
				highestContPar = highestCont->getParent();
				if (highestContPar != NULL) {
					return (((float)(highestContPar->index)) + 0.5f)/255.0f;
				}
				
			}
			else {
				return 1.0;
			}
			
		}
		
		
		UIComponent* curValuePtr = getValuePtr();
		if (curValuePtr != NULL) {
			if ((flags&E_GF_X) != 0) {
				return curValuePtr->getValueIndex(0);
			}
			else if ((flags&E_GF_Y) != 0) {
				return curValuePtr->getValueIndex(1);
			}
			else if ((flags&E_GF_Z) != 0) {
				return curValuePtr->getValueIndex(2);
			}
			else if ((flags&E_GF_W) != 0) {
				return curValuePtr->getValueIndex(3);
			}
		}
		else {
			return privValueX;
		}
		
		return 0.0f;
		
	}
	
	void setValueY(float _value, bool doEventDispatch = false, bool preventRefresh = false) {
		UIComponent* curValuePtr = getValuePtr();
		
		int count = 0;
		
		if (curValuePtr != NULL) {
			if ((flags&E_GF_X) != 0) {
				if (count == 1) {
					curValuePtr->setValueIndex(0,_value);
				}
				count++;
			}
			if ((flags&E_GF_Y) != 0) {
				if (count == 1) {
					curValuePtr->setValueIndex(1,_value);
				}
				count++;
			}
			if ((flags&E_GF_Z) != 0) {
				if (count == 1) {
					curValuePtr->setValueIndex(2,_value);
				}
				count++;
			}
			if ((flags&E_GF_W) != 0) {
				if (count == 1) {
					curValuePtr->setValueIndex(3,_value);
				}
				count++;
			}
		}
		else {
			privValueY = _value;
		}
	}
	float getValueY() {
		int count = 0;
		UIComponent* curValuePtr = getValuePtr();
		
		if (!dataLoaded) {
			dataLoaded = true;
			updateLinkedValues(true);
		}
		
		if (curValuePtr != NULL) {
			if ((flags&E_GF_X) != 0) {
				if (count == 1) {
					return curValuePtr->getValueIndex(0);
				}
				count++;
			}
			if ((flags&E_GF_Y) != 0) {
				if (count == 1) {
					return curValuePtr->getValueIndex(1);
				}
				count++;
			}
			if ((flags&E_GF_Z) != 0) {
				if (count == 1) {
					return curValuePtr->getValueIndex(2);
				}
				count++;
			}
			if ((flags&E_GF_W) != 0) {
				if (count == 1) {
					return curValuePtr->getValueIndex(3);
				}
				count++;
			}
			
			return 0.0f;
			
		}
		else {
			return privValueY;
		}
		
	}
	
	
	UIComponent* getParent() {
		if (nodeId <= 0) {
			return NULL;
		}
		else {
			return (singleton->compStack[parentId].data);
		}
	}
	
	
	UIComponent* findParentByUID(string parUID) {
		string resString = "";
		UIComponent* curParent = getParent();
		
		
		while (curParent != NULL) {
			
			
			if(curParent->uid.compare(parUID) == 0) {
				return curParent;
			}
			
			curParent = curParent->getParent();
		}
		
		return NULL;
	}
	
	
	UIComponent* getValuePtr() {
		if (foundValuePtr) {
			
		}
		else {
			
			if (valRef.compare("") != 0) {
				valuePtr = findParentByUID(valRef);
			}
			else {
				valuePtr = NULL;
			}
			
			foundValuePtr = true;
		}
		
		return valuePtr;
	}
	
	
	UIComponent* findNodeByString(string _uid) {
		int i;
		
		if (_uid.compare(uid) == 0) {
			return this;
		}
		
		for (i = 0; i < getChildCount(); i++) {
			if (getChild(i)->findNodeByString(_uid) != NULL) {
				return getChild(i)->findNodeByString(_uid);
			}
		}
		
		for (i = 0; i < getFloatingChildCount(); i++) {
			if (getFloatingChild(i)->findNodeByString(_uid) != NULL) {
				return getFloatingChild(i)->findNodeByString(_uid);
			}
		}
		
		return NULL;
	}
	
	float getMinWidth() {
		int i;
		
		updateVecs();
		
		if (singleLine) {
			spaceForCharsInPixels = 99999.0f;
			renderText(true);
			
		}
		else {
			textDimInPixels.x = 0;
			textDimInPixels.y = 0;
		}
		
		
		
		float curMW = (marginInPixels+borderInPixels+paddingInPixels)*2.0f + 
			textDimInPixels.x;
		
		
		float totMW = 0.0f;
		float tempMW = 0.0f;
		
		for (i = 0; i < getChildCount(); i++) {
			
			tempMW = getChild(i)->getMinWidth();
			
			if (fillDir == E_FILL_HORIZONTAL) {
				totMW += tempMW;
			}
			else {
				totMW = max(totMW,tempMW);
			}
		}
		
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->getMinWidth();
		}
		
		
		curMW += totMW;
		
		rmDimInPixels.x = max(curMW,minDimInPixels.x);
		
		return rmDimInPixels.x;
		
	}
	
	
	float getMinHeight() {
		int i;
		
		if (!singleLine) {
			spaceForCharsInPixels = resultDimInPixels.x;
			renderText(true);
		}
		else {
			// already determined
		}
		
		
		
		float curMH = (marginInPixels+borderInPixels+paddingInPixels)*2.0f + 
			textDimInPixels.y;
		
		float totMH = 0.0f;
		float tempMH = 0.0f;
		
		
		for (i = 0; i < getChildCount(); i++) {
			
			tempMH = getChild(i)->getMinHeight();
			
			if (fillDir == E_FILL_HORIZONTAL) {
				
				totMH = max(totMH,tempMH);
			}
			else {
				totMH += tempMH;
			}
		}
		
		
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->getMinHeight();
		}
		
		curMH += totMH;
		
		rmDimInPixels.y = max(curMH,minDimInPixels.y);
		
		return getDimYClamped(curMH);
		
	}
	
	
	
	int addChild(
		int _lastIndex,
		int _parentId,
		string* stringVals,
		double* floatVals,
		bool _isFloating,
		JSONValue* _jvNodeNoTemplate
		
	) {
		
		int _nodeId;
		int childCount = 0;
		
		_nodeId = singleton->placeInStack();
		
		
		
		if (_isFloating) {
			
			if (_lastIndex < 0) {
				_floatingChildren.push_back(_nodeId);
				childCount = getFloatingChildCount()-1;
			}
			else {
				_floatingChildren[_lastIndex] = _nodeId;
				childCount = _lastIndex;
			}
			
			
		}
		else {
			
			if (_lastIndex < 0) {
				_children.push_back(_nodeId);
				childCount = getChildCount()-1;
			}
			else {
				_children[_lastIndex] = _nodeId;
				childCount = _lastIndex;
			}
			
		}
		
		singleton->compStack[_nodeId].data->init(
			singleton,
			_parentId,
			_nodeId,
			childCount,
			
			_jvNodeNoTemplate,
			_isFloating,
			
			stringVals,
			floatVals
		);
		
		return _nodeId;
		
	}
	
	void setOrigPos() {
		
		int i;
		
		fVector2 curPos = originPos;
		
		
		
		
		fVector2 availSpaceDim = resultDimInPixels;
		fVector2 totVals;
		
		totVals.x = 0.0f;
		totVals.y = 0.0f;
		
		for (i = 0; i < getChildCount(); i++) {
			
			if (fillDir == E_FILL_HORIZONTAL) {
				totVals.x += getChild(i)->resultDimInPixels.x;
				totVals.y = max(totVals.y,getChild(i)->resultDimInPixels.y);
			}
			else {
				totVals.y += getChild(i)->resultDimInPixels.y;
				totVals.x = max(totVals.x,getChild(i)->resultDimInPixels.x);
			}
			
			
		}
		
		availSpaceDim.x -= totVals.x;
		availSpaceDim.y -= totVals.y;
		
		switch (align.x) {
			case E_ALIGNH_LEFT:
				curPos.x += marginInPixels + borderInPixels + paddingInPixels;
			break;
			case E_ALIGNH_CENTER:
				curPos.x += availSpaceDim.x/2.0f;
			break;
			case E_ALIGNH_RIGHT:
				curPos.x += availSpaceDim.x;
				curPos.x -= marginInPixels + borderInPixels + paddingInPixels;
			break;
		}
		
		switch (align.y) {
			case E_ALIGNV_TOP:
				curPos.y += marginInPixels + borderInPixels + paddingInPixels;
			break;
			case E_ALIGNV_MIDDLE:
				curPos.y += availSpaceDim.y/2.0f;
			break;
			case E_ALIGNV_BOTTOM:
				curPos.y += availSpaceDim.y;
				curPos.y -= marginInPixels + borderInPixels + paddingInPixels;
			break;
		}
		
		if (fillDir == E_FILL_HORIZONTAL) {
			for (i = 0; i < getChildCount(); i++) {
				getChild(i)->originPos.x = curPos.x;
				getChild(i)->originPos.y = curPos.y;
				curPos.x += getChild(i)->resultDimInPixels.x;
			}
		}
		else {
			for (i = 0; i < getChildCount(); i++) {
				getChild(i)->originPos.x = curPos.x;
				getChild(i)->originPos.y = curPos.y;
				curPos.y += getChild(i)->resultDimInPixels.y;
			}
		}
		
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->setOrigPos();
		}
		
		
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->setOrigPos();
		}
		
	}
	
	void applyHeight() {
		fVector2 totalRatios;
		totalRatios.x = 0.0f;
		totalRatios.y = 0.0f;
		float availSpace = resultDimInPixels.y - (marginInPixels+borderInPixels+paddingInPixels)*2.0f;
		int i;
		
		

		for (i = 0; i < getChildCount(); i++) {
			totalRatios.y += getChild(i)->fillRatioDim.y;
			if (fillDir == E_FILL_HORIZONTAL) {
				
			}
			else {
				availSpace -= getChild(i)->rmDimInPixels.y;
			}
		}
		
		availSpace = max(availSpace,0.0f);
		
		
		if (totalRatios.y == 0.0f) {
			totalRatios.y = 1.0f;
		}
		
		
		for (i = 0; i < getChildCount(); i++) {
			
			if (getChild(i)->enabled == false) {
				getChild(i)->resultDimInPixels.y = 0.0;
			}
			else {
				if (fillDir == E_FILL_HORIZONTAL) {
					
					if (getChild(i)->fillRatioDim.y == 0.0f) {
						getChild(i)->resultDimInPixels.y = getChild(i)->rmDimInPixels.y;
					}
					else {
						
						
						getChild(i)->resultDimInPixels.y = availSpace;
					}
				}
				else {
					getChild(i)->resultDimInPixels.y = ceil(
						getChild(i)->rmDimInPixels.y + 
						(availSpace*getChild(i)->fillRatioDim.y)/totalRatios.y
					);
					
					
				}
			}
			
			
		}
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->applyHeight();	
		}
		
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->applyHeight();
		}
		
	}
	
	void applyWidth() {
		
		fVector2 totalRatios;
		totalRatios.x = 0.0f;
		totalRatios.y = 0.0f;
		float availSpace = resultDimInPixels.x - (marginInPixels+borderInPixels+paddingInPixels)*2.0f;
		int i;
		

		for (i = 0; i < getChildCount(); i++) {
			totalRatios.x += getChild(i)->fillRatioDim.x;
			if (fillDir == E_FILL_HORIZONTAL) {
				availSpace -= getChild(i)->rmDimInPixels.x;
			}
			else {
				
			}
		}		
		availSpace = max(availSpace,0.0f);
		
		if (totalRatios.x == 0.0f) {
			totalRatios.x = 1.0f;
		}
		
		
		for (i = 0; i < getChildCount(); i++) {
			
			
			if (getChild(i)->enabled == false) {
				getChild(i)->resultDimInPixels.x = 0.0;
			}
			else {
				if (fillDir == E_FILL_HORIZONTAL) {
					
					
					
					getChild(i)->resultDimInPixels.x = ceil(
						max(
							getChild(i)->rmDimInPixels.x +
							(availSpace*getChild(i)->fillRatioDim.x)/totalRatios.x,
							getChild(i)->minDimInPixels.x
						)
					);
					
						
				}
				else {
					
					if (getChild(i)->fillRatioDim.x == 0.0f) {
						getChild(i)->resultDimInPixels.x =
							max(
								getChild(i)->rmDimInPixels.x,
								getChild(i)->minDimInPixels.x
							);
						
					}
					else {
						getChild(i)->resultDimInPixels.x =
							max(
								availSpace,
								getChild(i)->minDimInPixels.x
							);
						
					}
				}
			}
			
			
					
		}
		
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->applyWidth();	
		}
		
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->applyWidth();
		}
		
	}
	
	void gatherDirty(vector<UIComponent*>* dirtyVec ) {
		
		int i;
		
		if (isDirty) {
			dirtyVec->push_back(this);
		}
		else {
			for (i = 0; i < getChildCount(); i++) {
				getChild(i)->gatherDirty(dirtyVec);
			}
			for (i = 0; i < getFloatingChildCount(); i++) {
				getFloatingChild(i)->gatherDirty(dirtyVec);
			}
		}
		
	}
	
	void clearDirty() {
		
		int i;
		
		isDirty = false;
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->clearDirty();
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->clearDirty();
		}
	}
	
	
	void alignToComp(UIComponent* myComp) {
		UIComponent* myComp2 = myComp->getParent();
		
		
		if (
			(myComp->fillDir == 0) &&
			(myComp2 != NULL)
		) {
			floatOffset.x = myComp2->floatOffset.x + myComp2->dragOffset.x + myComp2->originPos.x + myComp2->resultDimInPixels.x;
			floatOffset.y = myComp2->floatOffset.y + myComp2->dragOffset.y + myComp2->originPos.y;
		}
		else {
			floatOffset.x = myComp->floatOffset.x + myComp->dragOffset.x + myComp->originPos.x + myComp->resultDimInPixels.x;
			floatOffset.y = myComp->floatOffset.y + myComp->dragOffset.y + myComp->originPos.y;
		}
		
		
		
		
	}
	
	
	void layout() {
		
		int i;
		
		
		// if (isDirty) {
		// 	isDirty = false;
		// }
		// else {
		// 	return;
		// }
		
		
		
		getMinWidth();
		applyWidth();
		getMinHeight();
		applyHeight();
		setOrigPos();
		
		
		
	}
	
	void updateOffset() {
		totOffset.x = floatOffset.x + dragOffset.x + scrollOffset.x;
		totOffset.y = floatOffset.y + dragOffset.y + scrollOffset.y;
		return;
	}

	void updateValue(float x, float y) {
		
		int i;
		
		float hoverBuffer = 4.0f;
		float tempValue;
		float tempValueY;
		
		UIComponent* curParent = getParent();
		
		
		UIComponent* highestCont;
		
		if (dragging||forceDragUpdate) {
			highestCont = getHighestCont(this, 0);
			
			if (forceDragUpdate) {
				highestCont->dragOffset.x = lastDrag.x;
				highestCont->dragOffset.y = lastDrag.y;
			}
			else {
				highestCont->dragOffset.x = lastDrag.x + (x - dragStart.x); //
				highestCont->dragOffset.y = lastDrag.y + (y - dragStart.y); //
			}
			
			
			forceDragUpdate = false;
		}
		
		if (curParent == NULL) {
			
		}
		else {
			if (
				(curParent->getFloatingChildCount() > 0)
			) {
				
			}
			else {
				//if (curParent->hoverType != E_HT_ROOT) {
					dragOffset = curParent->dragOffset;
				//}			
			}
		}
		
		
		
		
		
		
		
		if ((maxDimInPixels.y == 0)&&(curParent != NULL)) {
			
			if (curParent->getFloatingChildCount() == 0) {
				scrollOffset.y = curParent->scrollOffset.y;
			}
			
			
		}
		else {
			scrollOffset.y += (targScrollOffset.y-scrollOffset.y)/16.0f;
		}
		
		
		
		
		
		
		
		
		
		switch (hoverType) {
			case E_HT_NORMAL:
				if (curParent != NULL) {
					floatOffset = curParent->floatOffset;					
				}
			break;
			case E_HT_ROOT:
				
			break;
			case E_HT_TOOLTIP:
			case E_HT_TOOLTIP_VALUE:
				floatOffset.x = x + hoverBuffer - dragOffset.x; 
				floatOffset.y = curParent->totOffset.y + curParent->originPos.y + curParent->resultDimInPixels.y + hoverBuffer;
				visible = curParent->overSelf && curParent->enabled;
				
				if ((hoverType == E_HT_TOOLTIP_VALUE)&&visible&&enabled) {
					
					getChild(0)->setText(f__s(curParent->getValue()));
				}
				
			break;
			case E_HT_ONSELECTED:
				if (curParent != NULL) {
					alignToComp(curParent);
					
					//visible = (curParent->selected)&&(curParent->visible);
				}
				
				
				
				
			break;

		}
		
		for (i = 0; i < getChildCount(); i++) {
			
			switch(getChild(i)->hoverType) {
				case E_HT_NORMAL:
					getChild(i)->visible = visible&&enabled;
				break;
				
				case E_HT_ROOT:
					
				break;
				
				case E_HT_TOOLTIP:
				case E_HT_TOOLTIP_VALUE:
				
				break;
				
				case E_HT_ONSELECTED:
					getChild(i)->visible = visible&&selected&&enabled;
				break;
			}
			
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			
			switch(getFloatingChild(i)->hoverType) {
				case E_HT_NORMAL:
					getFloatingChild(i)->visible = visible&&enabled;
				break;
				
				case E_HT_ROOT:
					
				break;
				
				case E_HT_TOOLTIP:
				case E_HT_TOOLTIP_VALUE:
				
				break;
				
				case E_HT_ONSELECTED:
					getFloatingChild(i)->visible = visible&&selected&&enabled;
				break;
			}
		}
		
		
		updateOffset();
		
		
		if (curParent != NULL) {
			if (curParent->getFloatingChildCount() == 0) {
				if (maxDimInPixels.y == 0) {
					scrollMaskY = curParent->scrollMaskY;
				}
				else {
					scrollMaskY.x = originPos.y + marginInPixels;
					scrollMaskY.y = originPos.y +  getResultDimYClamped() - marginInPixels;
					
					scrollMaskY.x += totOffset.y-scrollOffset.y;
					scrollMaskY.y += totOffset.y-scrollOffset.y;
					
					scrollMaskY.x /= singleton->guiWinH;
					scrollMaskY.y /= singleton->guiWinH;
					scrollMaskY.x = ((1.0f-scrollMaskY.x) - 0.5f)*2.0f;
					scrollMaskY.y = ((1.0f-scrollMaskY.y) - 0.5f)*2.0f;			
				}
			}
		}
		
		
		
		
		
		
		
		float hbxMin = hitBounds.xMin + totOffset.x;
		float hbxMax = hitBounds.xMax + totOffset.x;
		
		float hbyMin = hitBounds.yMin + totOffset.y;
		float hbyMax = hitBounds.yMax + totOffset.y;
		
		if (wasHit&&(guiClass == E_GT_SLIDER)) {
			
			if (divisions == 1.0f) {
				// toggle button, do nothing
			}
			else {
				tempValue = clampfZO((x-hbxMin)/(hbxMax-hbxMin));
				tempValueY = clampfZO(1.0f - (y-hbyMin)/(hbyMax-hbyMin));
				
				if (divisions == 0.0f) {
					setValue(
						tempValue
					);
					setValueY(
						tempValueY	
					);
				}
				else {
					
					setValue(
						floor(tempValue*divisions)/divisions
					);
					setValueY(
						floor(tempValueY*divisions)/divisions
					);
				}
				
			}
		}
	}
	
	
	void runReport() {
		
		int i;
		
		//cout << overSelf << " " << text << "\n";
				
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->runReport();
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->runReport();
		}
	}
	
	
	
	void clearOver() {
		int i;
		overSelf = false;
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->clearOver();
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->clearOver();
		}
		
	}

	bool findMaxLayer(float x, float y, float xTransformed, float yTransformed) {
		
		
		int i;

		overSelf = (
			(x < (hitBounds.xMax+totOffset.x)) &&
			(x > (hitBounds.xMin+totOffset.x)) &&
			(y < (hitBounds.yMax+totOffset.y)) &&
			(y > (hitBounds.yMin+totOffset.y))
		) && visible && enabled;// && hasBackground;
		
		
		if (
			(maxDimInPixels.y == 0) ||
			((abs(scrollMaskY.x) + abs(scrollMaskY.y)) <= 1.0f)
		) {
			
		}
		else {
			if (
					(yTransformed > scrollMaskY.x) || 
				  (yTransformed < scrollMaskY.y)
			) {
				overSelf = false;
				overChild = false;
				return false;
			}
		}
		
		
		
		
		
		
		
		overChild = false;
		
		for (i = 0; i < getChildCount(); i++) {
			overChild = overChild||getChild(i)->findMaxLayer(x,y,xTransformed,yTransformed);
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			overChild = overChild||getFloatingChild(i)->findMaxLayer(x,y,xTransformed,yTransformed);
		}
		
		if (
			overSelf &&
			visible &&
			enabled &&
			hasBackground &&
			(!overChild) &&
			(layer >= singleton->maxLayerOver)
		) {
			singleton->maxLayerOver = layer;
		}
		
		return overSelf;
	}
	
	

	void testOver(float x, float y) {
		
		int i;
		
		bool lastOver = mouseOver;
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->testOver(x,y);
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->testOver(x,y);
		}
		
		mouseOver = 
			overSelf &&
			visible &&
			enabled &&
			(hasBackground) && //||(guiClass == E_GT_DRAGPAD)
			(!overChild) &&
			(layer >= singleton->maxLayerOver);
		
		
		
		if ((mouseOver != lastOver)&&(!(singleton->dragging))) {
			if (mouseOver) {
				singleton->dispatchEvent(
					GLUT_NO_BUTTON,
					GLUT_OVER,
					x,
					y,
					this
				);
				
				
			}
			else {
				singleton->dispatchEvent(
					GLUT_NO_BUTTON,
					GLUT_OUT,
					x,
					y,
					this
				);
			}
		}
		else {
			if (
				wasHit && 
				(
					(guiClass == E_GT_SLIDER) ||
					(guiClass == E_GT_DRAGPAD)	
				)
				
			) {
				
				if (divisions == 1.0f) {
					// toggle button, do nothing
				}
				else {
					singleton->dispatchEvent(
						GLUT_NO_BUTTON,
						GLUT_CHANGING,
						x,
						y,
						this
					);
				}
			}
		}
		
		
		
		
		updateValue(x, y);

	}

	bool testHit(int button, int state, float x, float y) {
		
		bool hitDeepest = false;		
		float wheelDelta = 0.0f;
		
		
		UIComponent* curParent = getParent();
		UIComponent* curParent2 = NULL;
		
		UIComponent* selParent = NULL;
		
		if (curParent != NULL) {
			curParent2 = curParent->getParent();
		}
		
		
		int i;
		int j;
		bool hitChild = false;
		float lastValue = getValue();
		bool tempValue;
		
		
		
		
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) { // MOUSE DOWN
				if (mouseOver) {
					
					if (enabled && visible) {
						
						
					
						
						if (
							(guiClass != E_GT_HOLDER)
						) {
							wasHit = true;
						}
						
						dragging = (guiClass == E_GT_MENUBAR);
						if (dragging) {
							singleton->dragging = true;
						}
						
						if (dragging||(guiClass == E_GT_DRAGPAD)) {
							dragStart.x = x;
							dragStart.y = y;
						}
						
						
						
					}
					
					
				}

				mouseDown = true;
			}
			else { // MOUSE UP
				
				
				if (dragging) {
					lastDrag.x += (x - dragStart.x);
					lastDrag.y += (y - dragStart.y);
					dragStart.x = 0.0f;
					dragStart.y = 0.0f;
					
					
				}
				dragging = false;
				singleton->dragging = false;
				
				if (mouseOver&&wasHit) {
					
					switch (guiClass) {
						case E_GT_HOLDER:
							
						break;
						case E_GT_SLIDER:
							if (divisions == 1.0f) {
								setValue(1.0f-getValue());
							}
						break;
						case E_GT_BUTTON:
							
						break;
						case E_GT_CHECK:
							selected = !selected;
						break;
						
						case E_GT_RADIO:
							tempValue = !selected;
							
							if (singleton->bCtrl) {
								
							}
							else {
								
								if (singleton->bShift) {
									
								}
								else {
									
								}
								
								if (curParent2 == NULL) {
									for (i = 0; i < curParent->getChildCount(); i++) {
										if (curParent->getChild(i)->guiClass == E_GT_RADIO) {
											curParent->getChild(i)->selected = false;
											//curParent->getChild(i)->setValue(0.0f);
										}
									}
								}
								else {
									
									for (j = 0; j < curParent2->getChildCount(); j++) {
										selParent = curParent2->getChild(j);
										
										for (i = 0; i < selParent->getChildCount(); i++) {
											if (selParent->getChild(i)->guiClass == E_GT_RADIO) {
												//selParent->getChild(i)->setValue(0.0f);
												selParent->getChild(i)->selected = false;
											}
										}
									}
									
									
									
									
								}
								
								
							}
							selected = tempValue;
							//setValue(tempValue);
							
						break;
					}
					
				}


				mouseDown = false;
			}
		}
		else {
				if ( (button == 3) || (button == 4) ) {
					
					
					if (enabled && visible && overSelf) {
						if (maxDimInPixels.y != 0) {
							
							if (button == 3) {
								wheelDelta = 1.0f;
								// wheel up
							}
							else {
								wheelDelta = -1.0f;
								// wheel down
							}
							
							if (resultDimInPixels.y > maxDimInPixels.y) {
								targScrollOffset.y += wheelDelta*20.0f;
								targScrollOffset.y = clampf(targScrollOffset.y, -(resultDimInPixels.y-maxDimInPixels.y), 0.0f);
								
							}
							
							
							return true;
						}	
					}
				}
		}

		updateValue(x, y);
		
		for (i = 0; i < getChildCount(); i++) {
			hitChild = hitChild||getChild(i)->testHit(button, state, x, y);
		}
		
		for (i = 0; i < getFloatingChildCount(); i++) {
			hitChild = hitChild||getFloatingChild(i)->testHit(button, state, x, y);
		}
		
		hitDeepest = (!hitChild)&&(mouseOver);
		
		if ( hitDeepest ) {
			// deepest node
			
			singleton->dispatchEvent(button, state, x, y, this);
		}
		
		bool finalRes = (hitDeepest||hitChild); //(wasHit)
		if ((state == GLUT_UP)) { //&&(wheelDelta==0.0f)
			wasHit = false;
		}
		

		return finalRes;
	}
	
	
	UIComponent* getHighestCont(UIComponent* curNode, int genCount) {
		UIComponent* curParent = curNode->getParent();
		
		if (curParent == NULL) {
			return curNode;
		}
		else {
			if (curParent->getFloatingChildCount() > 0) {
				return curNode;
			}
			else {
				return getHighestCont(curParent, genCount+1);
			}
		}
		
		
		
	}

	void setText(string _text) {
		
		UIComponent* highestCont;
		
		if (_text.compare(text) == 0) {
			// text unchanged, do nothing
		}
		else {
			text = _text;
			
			highestCont = getHighestCont(this, 0);
			highestCont->isDirty = true;
			singleton->guiDirty = true;
			
		}
		
		
	}
	
	
	void updateVecs() {
		
		
		
		lineVec.clear();
		wordVec.clear();

		lineVec = split(text, '\n');

		int i;
		int j;

		for (i = 0; i < lineVec.size(); i++) {
			wordVec.push_back(std::vector<string>());
			wordVec[i] = split(lineVec[i], ' ');
		}
		
	}

	void renderAll() {
		int i;
		
		renderText(false);
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->renderAll();
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->renderAll();
		}
	}
	
	
	void updateSS() {
		int i;
		
		resSS.update(mouseOver, mouseDown);
		
		for (i = 0; i < getChildCount(); i++) {
			getChild(i)->updateSS();
		}
		for (i = 0; i < getFloatingChildCount(); i++) {
			getFloatingChild(i)->updateSS();
		}
		
	}
	

	float getLineOffset(int lineCount) {
		
		switch (align.x) {
			case E_ALIGNH_LEFT:
				return 0.0f;
			break;
			case E_ALIGNH_CENTER:
				return (resultDimInPixels.x - (linePitchVec[lineCount]+(marginInPixels+borderInPixels+paddingInPixels)*2.0f) )/2.0f;
			break;
			case E_ALIGNH_RIGHT:
				return (
					resultDimInPixels.x -
					(
						(linePitchVec[lineCount]) +
						(marginInPixels+borderInPixels+paddingInPixels)
					)
				);
			break;
			default:
				return 0.0f;
			break;
		}
		
		
	}

	float lengthOfWord(int i, int j, bool isIcon) {
		int numChars = wordVec[i][j].size();
		int k;
		float tot = 0.0f;
		
		if (isIcon) {
			return curFontIcons->maxWidth;
		}
		
		for (k = 0; k < numChars; k++) {
			tot += curFont->charVals[ wordVec[i][j][k] ].consumedW*curFont->fontScale;
		}
		
		return tot;
		
	}
	
	int maxCharsForWord(int i, int j) {
		int numChars = wordVec[i][j].size();
		
		float tot = 0.0f;
		
		int k = 0;
		while (
			(tot < spaceForCharsInPixels) &&
			(k < wordVec[i][j].size())
		) {
			tot += curFont->charVals[ wordVec[i][j][k] ].consumedW;
			k++;
		}
		
		return k;
	}

	void renderText(bool getDimensions) {
		
		
		
		int curSize = 0;
		int i;
		int j;
		int k;
		int curChar;
		int lineCount = 0;
		
		bool isRendering = !getDimensions;
		bool isIcon = false;

		fVector2 caretPos;
		fVector2 offsetPos;
		
		float vspace = resultDimInPixels.y - textDimInPixels.y;
		
		Singleton::UIQuad* curQuad;
		
		caretPos.x = 0.0f;
		caretPos.y = 0.0f;
		
		if (getDimensions) {
			linePitchVec.clear();
		}
		else {
			offsetPos.x = originPos.x + marginInPixels + borderInPixels + paddingInPixels;
			
			switch (align.y) {
				case E_ALIGNV_TOP:
					offsetPos.y = originPos.y + marginInPixels + borderInPixels + paddingInPixels;
				break;
				case E_ALIGNV_MIDDLE:
					offsetPos.y = originPos.y + vspace/2.0f;
					
				break;
				case E_ALIGNV_BOTTOM:
					offsetPos.y = originPos.y + vspace - (marginInPixels + borderInPixels + paddingInPixels);
				break;
			}
		}
		
		
		float maxCaretPos = 0;

		//float charCount = 0;

		float maxSize;

		if (isRendering) {
			hitBounds.xMin = originPos.x + marginInPixels;
			hitBounds.xMax = originPos.x + resultDimInPixels.x - marginInPixels;
			hitBounds.yMin = originPos.y + marginInPixels;
			hitBounds.yMax = originPos.y + resultDimInPixels.y - marginInPixels;
			
			
			if (layerId == -1) {
				layerId = singleton->placeInLayer(nodeId, layer);
				//singleton->guiLayers[layer].push_back(&uiCont);
			}
			
			
			if (uiCont.charVec.size() > 0) {
				uiCont.charVec.clear();
			}
			
			
			
			//uiCont.uiComp = this;
			uiCont.bg.hitBounds = hitBounds;
			uiCont.bg.cs = NULL;
			uiCont.bg.fontId = -1;
			
			if (hasBackground) {
				uiCont.bg.fontId = 0;
			}
			
		}

		//lines
		for (i = 0; i < wordVec.size(); i++) {
			
			caretPos.x = 0.0f;
			if (isRendering) {
				caretPos.x += getLineOffset(lineCount);
			}
			lineCount++;

			//words in line
			for (j = 0; j < wordVec[i].size(); j++) {
				curSize = wordVec[i][j].size();
				
				if (curSize > 0) {
					isIcon = (wordVec[i][j][curSize-1] == '&') && (curSize >= 2);
					
					//if word won't fit, increment line
					if ( (caretPos.x + lengthOfWord(i,j,isIcon)) + curFont->maxWidth >= spaceForCharsInPixels ) {
						//charCount = 0;
						
						if (getDimensions) {
							linePitchVec.push_back(caretPos.x);
						}
						maxCaretPos = max(caretPos.x, maxCaretPos);
						caretPos.x = 0.0f;
						if (isRendering) {
							caretPos.x += getLineOffset(lineCount);
						}
						
						lineCount++;
						caretPos.y += (curFont->fontHeight*curFont->fontScale + spacing.y);
					}
					
					
					
					
					if (isIcon) {
						// is an icon
						
						curChar = atoi(wordVec[i][j].c_str());
						
						if (isRendering) {
							uiCont.charVec.push_back(Singleton::UIQuad());
							curQuad = &(uiCont.charVec.back());
							curQuad->cs = &(curFontIcons->charVals[ curChar ]);
							curQuad->fontId = EFW_ICONS;
							curQuad->hitBounds.xMin = caretPos.x+offsetPos.x;
							curQuad->hitBounds.yMin = caretPos.y+offsetPos.y;
						}
						
						
						
						caretPos.x += curFontIcons->maxWidth*curFontIcons->fontScale;
						
						
						
						// renderCharAt(
						// 	&(curFontIcons->charVals[ curChar ]),
						// 	curFontIcons,
						// 	caretPos.x+offsetPos.x,
						// 	caretPos.y+offsetPos.y
						// );
						
					}
					else {
						// is characters
						
						//if word won't fit on line, limit chars
						maxSize = maxCharsForWord(i,j);
						
						for (k = 0; k < maxSize; k++) {
							curChar = wordVec[i][j][k];
							if (isRendering) {
								
								uiCont.charVec.push_back(Singleton::UIQuad());
								curQuad = &(uiCont.charVec.back());
								curQuad->fontId = EFW_TEXT;
								curQuad->cs = &(curFont->charVals[ curChar ]);
								curQuad->hitBounds.xMin = caretPos.x+offsetPos.x;
								curQuad->hitBounds.yMin = caretPos.y+offsetPos.y;
								
								// renderCharAt(
								// 	&(curFont->charVals[ curChar ]),
								// 	curFont,
								// 	caretPos.x+offsetPos.x,
								// 	caretPos.y+offsetPos.y
								// );
							}
							caretPos.x += (curFont->charVals[ curChar ].consumedW*curFont->fontScale + spacing.x);
							//charCount++;
						}
					
					}
					
					//chars in word
					
					
					
					// render a space
					
					curChar = ' ';
					if (
						//(i == wordVec.size()-1) && 
						(j == wordVec[i].size()-1)
					) {
						//end of line, no space
					}
					else {
						if (caretPos.x + curFont->maxWidth*curFont->fontScale < spaceForCharsInPixels) {
							
							if (isRendering) {
								
								uiCont.charVec.push_back(Singleton::UIQuad());
								curQuad = &(uiCont.charVec.back());
								curQuad->fontId = EFW_TEXT;
								curQuad->cs = &(curFont->charVals[ curChar ]);
								curQuad->hitBounds.xMin = caretPos.x+offsetPos.x;
								curQuad->hitBounds.yMin = caretPos.y+offsetPos.y;
								
								// renderCharAt(
								// 	&(curFont->charVals[ curChar ]),
								// 	curFont,
								// 	caretPos.x+offsetPos.x,
								// 	caretPos.y+offsetPos.y
								// );
							}
							
							caretPos.x += (curFont->charVals[ curChar ].consumedW*curFont->fontScale + spacing.x);
							// /charCount++;
							
						} 
					}
					
				}
				
				

			}
			//charCount = 0;
			
			if (getDimensions) {
				linePitchVec.push_back(caretPos.x);
			}
			
			maxCaretPos = max(caretPos.x, maxCaretPos);
			caretPos.y += (curFont->fontHeight*curFont->fontScale + spacing.y);

		}

		if (getDimensions) {
			textDimInPixels.x = maxCaretPos - spacing.x;
			textDimInPixels.y = caretPos.y - spacing.y;
		}
		

	}

};
