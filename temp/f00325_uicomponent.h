// f00325_uicomponent.h
//

#include "f00325_uicomponent.e"
#define LZZ_INLINE inline
UIComponent::UIComponent ()
                      {
		
	}
void UIComponent::init (Singleton * _singleton, StyleSheet * _mainSS, string _text, int _originPosX, int _originPosY, int _charsPerLine, int _maxLines, int _alignX, int _alignY, int _fillDir, int _charScale, int _spacingX, int _spacingY)
          {

		singleton = _singleton;

		wasHit = false;
		value = 0.0f;
		divisions = 0.0f;

		uid = giGUI_IDS;
		giGUI_IDS++;

		mouseDown = false;
		mouseOver = false;

		mainSS = _mainSS;
		resSS.init(mainSS);
		originPos.x = _originPosX;
		originPos.y = _originPosY;

		hitBounds.xMin = 0;
		hitBounds.xMax = 0;
		hitBounds.yMin = 0;
		hitBounds.yMax = 0;


		align.x = _alignX;
		align.y = _alignY;
		fillDir = _fillDir;
		charScale = _charScale;

		spacing.x = _spacingX;
		spacing.y = _spacingY;

		text = _text;
		
		

		charsPerLine = _charsPerLine;
		maxLines = _maxLines;
		
		charDim.x = 7;
		charDim.y = 12;

		refresh();
	}
void UIComponent::updateValue (int x, int y)
                                       {
		if (wasHit) {
			if (divisions == 0.0f) {
				value = ((float)(x-hitBounds.xMin))/((float)(hitBounds.xMax-hitBounds.xMin));
			}
			else {
				if (divisions == 1.0f) {
					
				}
				else {
					value = floor(((float)(x-hitBounds.xMin))/((float)(hitBounds.xMax-hitBounds.xMin))*divisions)/divisions;
				}
			}
		}
	}
void UIComponent::testOver (int x, int y)
                                    {
		mouseOver = false;

		if (x < hitBounds.xMax && x > hitBounds.xMin && y < hitBounds.yMax && y > hitBounds.yMin) {
			mouseOver = true;
		}

		updateValue(x, y);

	}
bool UIComponent::testHit (int button, int state, int x, int y)
                                                          {
		wasHit = false;

		testOver(x,y);

		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) {
				if (mouseOver) {
					wasHit = true;
				}

				mouseDown = true;
			}
			else {
				if (mouseOver) {
					if (divisions == 1.0f) {
						value = 1.0f-value;
					}
				}


				mouseDown = false;
			}
		}
		else {
				
		}

		updateValue(x, y);

		return wasHit;
	}
void UIComponent::setText (string _text)
                                   {
		text = _text;

		refresh();
	}
void UIComponent::refresh ()
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

		render(false);
	}
void UIComponent::render (bool visible)
                                  {
		
		

		int i;
		int j;
		int k;
		int curChar;

		iVector3 caretPos;
		
		if (visible) {
			resSS.update(mouseOver, mouseDown);
		}
		
		int carriage = originPos.x + resSS.props[E_SS_MARGIN] + resSS.props[E_SS_BORDER] + resSS.props[E_SS_PADDING];

		caretPos.x = carriage;
		caretPos.y = originPos.y + resSS.props[E_SS_MARGIN] + resSS.props[E_SS_BORDER] + resSS.props[E_SS_PADDING];

		int charCount = 0;

		int maxSize;

		int bufVal = resSS.props[E_SS_BORDER] + resSS.props[E_SS_PADDING];

		if (visible) {
			hitBounds.xMin = originPos.x - bufVal;
			hitBounds.xMax = resultDim.x + bufVal*2;
			hitBounds.yMin = originPos.y - bufVal;
			hitBounds.yMax = resultDim.y + bufVal*2;
			renderQuad(hitBounds.xMin, hitBounds.yMin, hitBounds.xMax, hitBounds.yMax);
		}

		//lines
		for (i = 0; i < wordVec.size(); i++) {

			//words in line
			for (j = 0; j < wordVec[i].size(); j++) {

				//if word won't fit, increment line
				if ( (charCount + wordVec[i][j].size()) > charsPerLine ) {
					charCount = 0;
					caretPos.x = carriage;
					caretPos.y += (charDim.y + spacing.y)*charScale;
				}
				//if word won't fit on line, limit chars
				maxSize = min ( (int)wordVec[i][j].size() , charsPerLine);
				
				//chars in word
				for (k = 0; k < maxSize; k++) {
					curChar = wordVec[i][j][k];
					if (visible) {
						renderCharAt(curChar, caretPos.x, caretPos.y);
					}
					caretPos.x += (charDim.x + spacing.x)*charScale;
					charCount++;
				}

				
				curChar = ' ';
				if (visible) {
					if (charCount+1 < charsPerLine) {
						renderCharAt(curChar, caretPos.x, caretPos.y);
					} 
					
				}
				caretPos.x += (charDim.x + spacing.x)*charScale;
				charCount++;

			}
			charCount = 0;
			caretPos.x = carriage;
			caretPos.y += (charDim.y + spacing.y)*charScale;

		}

		resultDim.x = carriage + charsPerLine*(charDim.x + spacing.x)*charScale;
		resultDim.y = caretPos.y-( (spacing.y)*charScale );

	}
#undef LZZ_INLINE
 
