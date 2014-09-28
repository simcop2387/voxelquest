// f00325_uicomponent.h
//

#include "f00325_uicomponent.e"
#define LZZ_INLINE inline
UIComponent::UIComponent ()
                      {
		
	}
void UIComponent::init (Singleton * _singleton, int _parentId, int _nodeId, int _index, JSONValue * _jvNodeNoTemplate, string _label, string _uid, string _ss, int _guiClass, float _divisions, bool _hasBackground, bool _singleLine, float _fillRatioX, float _fillRatioY, int _fillDir, int _alignX, int _alignY, float _value, int _layer, int _hoverType, bool _isFloating)
          {

		parent = NULL;
		foundParent = false;
		contOnStack = false;

		singleton = _singleton;
		parentId = _parentId;
		nodeId = _nodeId;
		
		
		
		jvNodeNoTemplate = _jvNodeNoTemplate;
		
		layer = _layer;
		hoverType = _hoverType;
		isFloating = _isFloating;
		
		ss = _ss;
		label = _label;
		text = label;
		
		uid = _uid;
		index = _index;
		
		if (uid.size() > 0) {
			singleton->compMap[uid].nodeId = nodeId;
		}
		
		
		guiClass = _guiClass;
		//guiId = _guiId;
		
		overSelf = false;
		overChild = false;
		isDirty = true;
		visible = (hoverType == E_HT_NORMAL);
		
		hasBackground = _hasBackground;
		fillRatioDim.x = _fillRatioX;
		fillRatioDim.y = _fillRatioY;

		singleLine = _singleLine;
		
		
		curFont = singleton->fontWrappers[EFW_TEXT];
		curFontIcons = singleton->fontWrappers[EFW_ICONS];
		
		wasHit = false;
		value = _value;
		divisions = _divisions;

		mouseDown = false;
		mouseOver = false;

		resSS.init(&(singleton->styleSheetMap[ss]));
		
		
		paddingInPixels = resSS.baseSS->compStates[E_COMP_UP].props[E_SS_PADDING];
		borderInPixels = resSS.baseSS->compStates[E_COMP_UP].props[E_SS_BORDER];
		marginInPixels = resSS.baseSS->compStates[E_COMP_UP].props[E_SS_MARGIN];
		
		
		/////////////////
		
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


		align.x = _alignX;
		align.y = _alignY;
		fillDir = _fillDir;
		
		spacing.x = 0.0f;
		spacing.y = 0.0f;

		
		if (isFloating) {
			resultDimInPixels.x = singleton->guiWinW;
			resultDimInPixels.y = singleton->guiWinH;
			originPos.x = 0.0;
			originPos.y = 0.0;
		}
		
		
	}
void UIComponent::setValue (float _value, bool doEventDispatch, bool preventRefresh)
                                                                                               {
		value = _value;
		if (doEventDispatch) {
			singleton->dispatchEvent(GLUT_LEFT_BUTTON, GLUT_UP, 0, 0, this, true, preventRefresh);
		}
	}
float UIComponent::getValue ()
                         {
		return value;
	}
UIComponent * UIComponent::getParent ()
                                 {
		if (foundParent) {
			
		}
		else {
			// todo: replace with root or id
			parent = singleton->mainGUI->baseComp->findNodeById(parentId);
			foundParent = true;
		}
		
		return parent;
	}
UIComponent * UIComponent::findNodeByString (string _uid)
                                                   {
		int i;
		
		if (_uid.compare(uid) == 0) {
			return this;
		}
		
		for (i = 0; i < children.size(); i++) {
			if (children[i].findNodeByString(_uid) != NULL) {
				return children[i].findNodeByString(_uid);
			}
		}
		
		for (i = 0; i < floatingChildren.size(); i++) {
			if (floatingChildren[i].findNodeByString(_uid) != NULL) {
				return floatingChildren[i].findNodeByString(_uid);
			}
		}
		
		return NULL;
	}
UIComponent * UIComponent::findNodeById (int id)
                                          {
		int i;
		
		if (nodeId == id) {
			return this;
		}
		
		for (i = 0; i < children.size(); i++) {
			if (children[i].findNodeById(id) != NULL) {
				return children[i].findNodeById(id);
			}
		}
		
		for (i = 0; i < floatingChildren.size(); i++) {
			if (floatingChildren[i].findNodeById(id) != NULL) {
				return floatingChildren[i].findNodeById(id);
			}
		}
		
		return NULL;
	}
float UIComponent::getMinWidth ()
                            {
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
		
		for (i = 0; i < children.size(); i++) {
			
			tempMW = children[i].getMinWidth();
			
			if (fillDir == E_FILL_HORIZONTAL) {
				totMW += tempMW;
			}
			else {
				totMW = max(totMW,tempMW);
			}
		}
		
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].getMinWidth();
		}
		
		
		curMW += totMW;
		
		minDimInPixels.x = curMW;
		
		return curMW;
		
	}
float UIComponent::getMinHeight ()
                             {
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
		
		
		for (i = 0; i < children.size(); i++) {
			
			tempMH = children[i].getMinHeight();
			
			if (fillDir == E_FILL_HORIZONTAL) {
				
				totMH = max(totMH,tempMH);
			}
			else {
				totMH += tempMH;
			}
		}
		
		
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].getMinHeight();
		}
		
		curMH += totMH;
		
		minDimInPixels.y = curMH;
		
		return curMH;
		
	}
UIComponent * UIComponent::addChild (int _parentId, int _nodeId, string * stringVals, float * floatVals, bool _isFloating, JSONValue * _jvNodeNoTemplate)
          {
		
		UIComponent tempComp;
		
		int childCount = 0;
		
		if (_isFloating) {
			floatingChildren.push_back(tempComp);
			curComp = &(floatingChildren.back());
			childCount = floatingChildren.size()-1;
			
			
		}
		else {
			children.push_back(tempComp);
			curComp = &(children.back());
			childCount = children.size()-1;
		}
		
		
		
		
		
		curComp->init(
			singleton,
			_parentId,
			_nodeId,
			childCount,
			
			_jvNodeNoTemplate,
			
			stringVals[E_GST_LABEL],
			stringVals[E_GST_UID],
			stringVals[E_GST_SS],
			
			floatVals[E_GFT_TYPE],
			floatVals[E_GFT_DIVISIONS],
			floatVals[E_GFT_HASBACKGROUND],
			floatVals[E_GFT_SINGLELINE],
			floatVals[E_GFT_FILLRATIOX],
			floatVals[E_GFT_FILLRATIOY],
			floatVals[E_GFT_FILLDIR],
			floatVals[E_GFT_ALIGNX],
			floatVals[E_GFT_ALIGNY],
			floatVals[E_GFT_VALUE],
			floatVals[E_GFT_LAYER],
			floatVals[E_GFT_HOVERTYPE],
			_isFloating
		);
		
		//return curComp;
		
		if (_isFloating) {
			return &(floatingChildren.back());
		}
		else {
			return &(children.back());
		}
		
	}
void UIComponent::setOrigPos ()
                          {
		
		int i;
		
		fVector2 curPos = originPos;
		
		
		
		
		fVector2 availSpaceDim = resultDimInPixels;
		fVector2 totVals;
		
		totVals.x = 0.0f;
		totVals.y = 0.0f;
		
		for (i = 0; i < children.size(); i++) {
			
			if (fillDir == E_FILL_HORIZONTAL) {
				totVals.x += children[i].resultDimInPixels.x;
				totVals.y = max(totVals.y,children[i].resultDimInPixels.y);
			}
			else {
				totVals.y += children[i].resultDimInPixels.y;
				totVals.x = max(totVals.x,children[i].resultDimInPixels.x);
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
			for (i = 0; i < children.size(); i++) {
				children[i].originPos.x = curPos.x;
				children[i].originPos.y = curPos.y;
				curPos.x += children[i].resultDimInPixels.x;
			}
		}
		else {
			for (i = 0; i < children.size(); i++) {
				children[i].originPos.x = curPos.x;
				children[i].originPos.y = curPos.y;
				curPos.y += children[i].resultDimInPixels.y;
			}
		}
		
		
		for (i = 0; i < children.size(); i++) {
			children[i].setOrigPos();
		}
		
		
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].setOrigPos();
		}
		
	}
void UIComponent::applyHeight ()
                           {
		fVector2 totalRatios;
		totalRatios.x = 0.0f;
		totalRatios.y = 0.0f;
		float availSpace = resultDimInPixels.y - (marginInPixels+borderInPixels+paddingInPixels)*2.0f;
		int i;
		
		
		//float totVal = 0.0f;
		for (i = 0; i < children.size(); i++) {
			totalRatios.y += children[i].fillRatioDim.y;
			if (fillDir == E_FILL_HORIZONTAL) {
				//totVal = max(totVal,children[i].minDimInPixels.y);
			}
			else {
				availSpace -= children[i].minDimInPixels.y;
			}
		}
		//availSpace -= totVal;
		
		
		if (totalRatios.y == 0.0f) {
			totalRatios.y = 1.0f;
		}
		
		
		for (i = 0; i < children.size(); i++) {
			
			if (fillDir == E_FILL_HORIZONTAL) {
				
				if (children[i].fillRatioDim.y == 0.0f) {
					children[i].resultDimInPixels.y = children[i].minDimInPixels.y;
				}
				else {
					
					// SHOULD BE: 
					// children[i].resultDimInPixels.y = availSpace;
					// ?
					children[i].resultDimInPixels.y = availSpace;
				}
			}
			else {
				children[i].resultDimInPixels.y =
					children[i].minDimInPixels.y +
					(availSpace*children[i].fillRatioDim.y)/totalRatios.y;	
				
			}
		}
		
		for (i = 0; i < children.size(); i++) {
			children[i].applyHeight();	
		}
		
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].applyHeight();
		}
		
	}
void UIComponent::applyWidth ()
                          {
		
		fVector2 totalRatios;
		totalRatios.x = 0.0f;
		totalRatios.y = 0.0f;
		float availSpace = resultDimInPixels.x - (marginInPixels+borderInPixels+paddingInPixels)*2.0f;
		int i;
		
		//float totVal = 0.0f;
		for (i = 0; i < children.size(); i++) {
			totalRatios.x += children[i].fillRatioDim.x;
			if (fillDir == E_FILL_HORIZONTAL) {
				availSpace -= children[i].minDimInPixels.x;
			}
			else {
				//totVal = max(totVal,children[i].minDimInPixels.x);
			}
		}		
		//availSpace -= totVal;
		
		if (totalRatios.x == 0.0f) {
			totalRatios.x = 1.0f;
		}
		
		
		for (i = 0; i < children.size(); i++) {
			
			if (fillDir == E_FILL_HORIZONTAL) {
				
				
				
				children[i].resultDimInPixels.x =
					children[i].minDimInPixels.x +
					(availSpace*children[i].fillRatioDim.x)/totalRatios.x;	
			}
			else {
				
				if (children[i].fillRatioDim.x == 0.0f) {
					children[i].resultDimInPixels.x = children[i].minDimInPixels.x;
				}
				else {
					children[i].resultDimInPixels.x = availSpace;
				}
			}
					
		}
		
		
		for (i = 0; i < children.size(); i++) {
			children[i].applyWidth();	
		}
		
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].applyWidth();
		}
		
	}
void UIComponent::gatherDirty (vector <UIComponent*> * dirtyVec)
                                                          {
		
		int i;
		
		if (isDirty) {
			dirtyVec->push_back(this);
		}
		else {
			for (i = 0; i < children.size(); i++) {
				children[i].gatherDirty(dirtyVec);
			}
			for (i = 0; i < floatingChildren.size(); i++) {
				floatingChildren[i].gatherDirty(dirtyVec);
			}
		}
		
	}
void UIComponent::clearDirty ()
                          {
		
		int i;
		
		isDirty = false;
		
		for (i = 0; i < children.size(); i++) {
			children[i].clearDirty();
		}
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].clearDirty();
		}
	}
void UIComponent::layout ()
                      {
		
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
void UIComponent::updateValue (float x, float y)
                                           {
		
		float hoverBuffer = 4.0f;
		float tempValue;
		
		UIComponent* curParent = getParent();
		
		switch (hoverType) {
			case E_HT_NORMAL:
				if (curParent != NULL) {
					floatOffset = curParent->floatOffset;
					visible = curParent->visible;
					
					
				}
			break;
			case E_HT_TOOLTIP:
			case E_HT_TOOLTIP_VALUE:
				//curParent->floatOffset.x + 
				floatOffset.x = x + hoverBuffer; 
				floatOffset.y = curParent->floatOffset.y + curParent->originPos.y + curParent->resultDimInPixels.y + hoverBuffer;
				visible = curParent->overSelf;
				
				if ((hoverType == E_HT_TOOLTIP_VALUE)&&visible) {
					
					children[0].setText(f__s(curParent->value));
				}
				
			break;
			case E_HT_ONSELECTED:
				//curParent->floatOffset.x + 
				floatOffset.x = curParent->originPos.x + curParent->resultDimInPixels.x;
				floatOffset.y = curParent->floatOffset.y + curParent->originPos.y;
				visible = (curParent->value == 1.0f);
			break;
						
		}
		
		
		float hbxMin = hitBounds.xMin + floatOffset.x;
		float hbxMax = hitBounds.xMax + floatOffset.x;
		
		if (wasHit&&(guiClass == E_GT_SLIDER)) {
			
			if (divisions == 1.0f) {
				// toggle button, do nothing
			}
			else {
				tempValue = clampfZO((x-hbxMin)/(hbxMax-hbxMin));
				if (divisions == 0.0f) {
					setValue(
						tempValue
					);
				}
				else {
					
					setValue(
						floor(tempValue*divisions)/divisions
					);
				}
				
			}
		}
	}
void UIComponent::runReport ()
                         {
		
		int i;
		
		//cout << overSelf << " " << text << "\n";
				
		for (i = 0; i < children.size(); i++) {
			children[i].runReport();
		}
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].runReport();
		}
	}
void UIComponent::clearOver ()
                         {
		int i;
		overSelf = false;
		
		for (i = 0; i < children.size(); i++) {
			children[i].clearOver();
		}
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].clearOver();
		}
		
	}
bool UIComponent::findMaxLayer (float x, float y)
                                            {
		
		
		int i;

		overSelf = (
			(x < (hitBounds.xMax+floatOffset.x)) &&
			(x > (hitBounds.xMin+floatOffset.x)) &&
			(y < (hitBounds.yMax+floatOffset.y)) &&
			(y > (hitBounds.yMin+floatOffset.y))
		) && visible;
		
		overChild = false;
		
		for (i = 0; i < children.size(); i++) {
			overChild = overChild||children[i].findMaxLayer(x,y);
		}
		for (i = 0; i < floatingChildren.size(); i++) {
			overChild = overChild||floatingChildren[i].findMaxLayer(x,y);
		}
		
			
		if (
			overSelf &&
			visible &&
			hasBackground &&
			(!overChild) &&
			(layer >= singleton->maxLayerOver)
		) {
			singleton->maxLayerOver = layer;
		}
		
		return overSelf;
	}
void UIComponent::testOver (float x, float y)
                                        {
		
		int i;
		
		bool lastOver = mouseOver;
		
		for (i = 0; i < children.size(); i++) {
			children[i].testOver(x,y);
		}
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].testOver(x,y);
		}
		
		mouseOver = 
			overSelf &&
			visible &&
			hasBackground &&
			(!overChild) &&
			(layer >= singleton->maxLayerOver);
		
		
		if (mouseOver != lastOver) {
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
			if (wasHit&&(guiClass == E_GT_SLIDER)) {
				
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
bool UIComponent::testHit (int button, int state, float x, float y)
                                                              {
		
		
		
		UIComponent* curParent = getParent();
		
		int i;
		bool hitChild = false;
		float lastValue = value;
		float tempValue;
		
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) { // MOUSE DOWN
				if (mouseOver) {
					
					if (
						// (guiClass == E_GT_SLIDER) ||
						// (guiClass == E_GT_BUTTON)
						(guiClass != E_GT_HOLDER) && visible
						
					) {
						wasHit = true;
					}
					
					
				}

				mouseDown = true;
			}
			else { // MOUSE UP
				if (mouseOver&&wasHit) {
					
					switch (guiClass) {
						case E_GT_HOLDER:
							
						break;
						case E_GT_SLIDER:
							if (divisions == 1.0f) {
								setValue(1.0f-value);
							}
						break;
						case E_GT_BUTTON:
							
						break;
						case E_GT_RADIO:
							
							tempValue = 1.0f-value;
							
							if (tempValue == 1.0f) {
								for (i = 0; i < curParent->children.size(); i++) {
									if (curParent->children[i].guiClass == E_GT_RADIO) {
										curParent->children[i].setValue(0.0f);
									}
								}
							}
							
							setValue(tempValue);
							
						break;
					}
					
				}


				mouseDown = false;
			}
		}
		else {
				
		}

		updateValue(x, y);
		
		for (i = 0; i < children.size(); i++) {
			hitChild = hitChild||children[i].testHit(button, state, x, y);
		}
		
		for (i = 0; i < floatingChildren.size(); i++) {
			hitChild = hitChild||floatingChildren[i].testHit(button, state, x, y);
		}
		
		
		if ( (!hitChild)&&(mouseOver) ) {
			// deepest node
			
			singleton->dispatchEvent(button, state, x, y, this);
		}
		
		bool finalRes = wasHit||hitChild;
		if (state == GLUT_UP) {
			wasHit = false;
		}
		

		return finalRes;
	}
void UIComponent::setText (string _text)
                                   {
		
		UIComponent* curParent = getParent();
		UIComponent* curParentParent;
		
		if (_text.compare(text) == 0) {
			// text unchanged, do nothing
		}
		else {
			text = _text;
			//isDirty = true;
			
			if (curParent != NULL) {
				
				curParentParent = curParent->getParent();
				
				if (curParentParent != NULL) {
					curParentParent->isDirty = true;
				}
				else {
					curParent->isDirty = true;
				}
				
				
				singleton->guiDirty = true;
			}
			
			//thisUICont.locked = true;

			//layout();
			//renderAll();
			//thisUICont.locked = false;
		}
		
		
	}
void UIComponent::updateVecs ()
                          {
		
		
		
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
void UIComponent::renderAll ()
                         {
		int i;
		
		renderText(false);
		
		for (i = 0; i < children.size(); i++) {
			children[i].renderAll();
		}
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].renderAll();
		}
	}
void UIComponent::updateSS ()
                        {
		int i;
		
		resSS.update(mouseOver, mouseDown);
		
		for (i = 0; i < children.size(); i++) {
			children[i].updateSS();
		}
		for (i = 0; i < floatingChildren.size(); i++) {
			floatingChildren[i].updateSS();
		}
		
	}
float UIComponent::getLineOffset (int lineCount)
                                           {
		
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
float UIComponent::lengthOfWord (int i, int j, bool isIcon)
                                                      {
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
int UIComponent::maxCharsForWord (int i, int j)
                                          {
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
void UIComponent::renderText (bool getDimensions)
                                            {
		
		
		
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
			
			
			if (contOnStack) {
				
			}
			else {
				contOnStack = true;
				singleton->guiLayers[layer].push_back(&thisUICont);
				//thisUICont.locked = false;
			}
			
			
			if (thisUICont.charVec.size() > 0) {
				thisUICont.charVec.clear();
			}
			
			
			
			thisUICont.uiComp = this;
			thisUICont.bg.hitBounds = hitBounds;
			thisUICont.bg.cs = NULL;
			thisUICont.bg.fontId = -1;
			
			if (hasBackground) {
				thisUICont.bg.fontId = 0;
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
						thisUICont.charVec.push_back(Singleton::UIQuad());
						curQuad = &(thisUICont.charVec.back());
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
							
							thisUICont.charVec.push_back(Singleton::UIQuad());
							curQuad = &(thisUICont.charVec.back());
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
							
							thisUICont.charVec.push_back(Singleton::UIQuad());
							curQuad = &(thisUICont.charVec.back());
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
#undef LZZ_INLINE
 
