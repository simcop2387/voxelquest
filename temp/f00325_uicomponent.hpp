
class UIComponent {
private:
	
public:

	Singleton* singleton;

	int uid;
	bool mouseOver;
	bool mouseDown;
	bool wasHit;

	float value;
	float divisions;

	iVector2 align;
	int fillDir;

	iVector2 originPos;
	iVector2 resultDim;

	string text;
	int charScale;
	int charsPerLine;
	int maxLines;
	iVector2 charDim;
	iVector2 spacing;
	std::vector<string> lineVec;
	std::vector< std::vector<string> > wordVec;

	iBoundingBox hitBounds;

	StyleSheet* mainSS;
	StyleSheetResult resSS;


	enum eAlignH {
		E_ALIGN_LEFT,
		E_ALIGN_RIGHT,
		E_ALIGN_CENTER	
	};

	enum eAlignV {
		E_ALIGN_TOP,
		E_ALIGN_MIDDLE,
		E_ALIGN_BOTTOM	
	};

	enum eFillDir {
		E_FILL_HORIZONTAL,
		E_FILL_VERTICAL
	}; 


	UIComponent() {
		
	}

	void init(

				Singleton* _singleton,

				StyleSheet* _mainSS,
				string _text,
				
				int _originPosX,
				int _originPosY,
				int _charsPerLine=40,
				int _maxLines=5,
				
				int _alignX=E_ALIGN_LEFT,
				int _alignY=E_ALIGN_TOP,
				int _fillDir=E_FILL_HORIZONTAL,
				
				int _charScale=1,
				int _spacingX=0,
				int _spacingY=0
				


	) {

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

	void updateValue(int x, int y) {
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

	void testOver(int x, int y) {
		mouseOver = false;

		if (x < hitBounds.xMax && x > hitBounds.xMin && y < hitBounds.yMax && y > hitBounds.yMin) {
			mouseOver = true;
		}

		updateValue(x, y);

	}

	bool testHit(int button, int state, int x, int y) {
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
	

	void setText(string _text) {
		text = _text;

		refresh();
	}

	void refresh() {

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

	inline void renderCharAt(int c, int px, int py) {
		
		iBoundingBox ibbPos;
		fBoundingBox fbbPos;

		fBoundingBox fbbSamplePos;

		ibbPos.xMin = px;
		ibbPos.yMin = py;
		ibbPos.xMax = px+charDim.x*charScale;
		ibbPos.yMax = py+charDim.y*charScale;

		if (c > 32 && c < 126) {
			fbbSamplePos.xMin = ((float)((c-32)*charDim.x - 1))/656.0;
		}
		else {
			fbbSamplePos.xMin = 0.0f;
		}

		fbbSamplePos.xMax = ((float)((c+1-32)*charDim.x - 1))/656.0;
		fbbSamplePos.yMin = 0.0f;
		fbbSamplePos.yMax = 1.0f;

		float x0 = ((float)ibbPos.xMin)/singleton->origWinW;
		float x1 = ((float)ibbPos.xMax)/singleton->origWinW;
		float y0 = ((float)ibbPos.yMin)/singleton->origWinH;
		float y1 = ((float)ibbPos.yMax)/singleton->origWinH;

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		
		//dimensions
		glMultiTexCoord4f(1, charDim.x*charScale, charDim.y*charScale, 0.0f, 0.0f);

		glMultiTexCoord4f(4, 1.0f, 1.0f, 1.0f, 1.0f);
		glMultiTexCoord4f(5, 1.0f, 1.0f, 1.0f, 1.0f);
		//border color
		glMultiTexCoord4f(6, 1.0f, 1.0f, 1.0f, 1.0f);
		//misc
		glMultiTexCoord4f(7, 0.0f, 0.0f, 0.0f, 0.0f);
		

		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOLTEXT1_R], resSS.props[E_SS_BGCOLTEXT1_G], resSS.props[E_SS_BGCOLTEXT1_B], resSS.props[E_SS_BGCOLTEXT1_A]);
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOLTEXT1_R], resSS.props[E_SS_FGCOLTEXT1_G], resSS.props[E_SS_FGCOLTEXT1_B], resSS.props[E_SS_FGCOLTEXT1_A]);
		
		
		glMultiTexCoord4f(0, fbbSamplePos.xMin, fbbSamplePos.yMin, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, fbbSamplePos.xMax, fbbSamplePos.yMin, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOLTEXT0_R], resSS.props[E_SS_BGCOLTEXT0_G], resSS.props[E_SS_BGCOLTEXT0_B], resSS.props[E_SS_BGCOLTEXT0_A]);
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOLTEXT0_R], resSS.props[E_SS_FGCOLTEXT0_G], resSS.props[E_SS_FGCOLTEXT0_B], resSS.props[E_SS_FGCOLTEXT0_A]);		

		glMultiTexCoord4f(0, fbbSamplePos.xMax, fbbSamplePos.yMax, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, fbbSamplePos.xMin, fbbSamplePos.yMax, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );

	}

	inline void renderQuad(int ix0, int iy0, int ix1, int iy1) {


		


		float x0 = ((float)ix0)/singleton->origWinW;
		float x1 = ((float)ix1)/singleton->origWinW;
		float y0 = ((float)iy0)/singleton->origWinH;
		float y1 = ((float)iy1)/singleton->origWinH;

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		
		//dimensions
		glMultiTexCoord4f(1, ix1-ix0, iy1-iy0, resSS.props[E_SS_BORDER], resSS.props[E_SS_CORNERRAD]);

		//glMultiTexCoord4f(4, 1.0f, 1.0f, 1.0f, 1.0f);
		glMultiTexCoord4f(5, 1.0f, 1.0f, 1.0f, 1.0f);
		
		//border color
		glMultiTexCoord4f(6, resSS.props[E_SS_BDCOL_R], resSS.props[E_SS_BDCOL_G], resSS.props[E_SS_BDCOL_B], resSS.props[E_SS_BDCOL_A]);
		//misc
		glMultiTexCoord4f(7, resSS.props[E_SS_ROUNDNESS], value, 1.0f, 1.0f);


		//bg
		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOL1_R], resSS.props[E_SS_BGCOL1_G], resSS.props[E_SS_BGCOL1_B], resSS.props[E_SS_BGCOL1_A]);
		//fg
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOL1_R], resSS.props[E_SS_FGCOL1_G], resSS.props[E_SS_FGCOL1_B], resSS.props[E_SS_FGCOL1_A]);
		//tg
		glMultiTexCoord4f(4, resSS.props[E_SS_TGCOL1_R], resSS.props[E_SS_TGCOL1_G], resSS.props[E_SS_TGCOL1_B], resSS.props[E_SS_TGCOL1_A]);
		
		
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		//bg
		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOL0_R], resSS.props[E_SS_BGCOL0_G], resSS.props[E_SS_BGCOL0_B], resSS.props[E_SS_BGCOL0_A]);
		//fg
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOL0_R], resSS.props[E_SS_FGCOL0_G], resSS.props[E_SS_FGCOL0_B], resSS.props[E_SS_FGCOL0_A]);
		//tg
		glMultiTexCoord4f(4, resSS.props[E_SS_TGCOL0_R], resSS.props[E_SS_TGCOL0_G], resSS.props[E_SS_TGCOL0_B], resSS.props[E_SS_TGCOL0_A]);

		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );


	}

	void render(bool visible) {
		
		

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

};
