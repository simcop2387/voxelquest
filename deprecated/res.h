struct MEMBER_DEF_STRUCT {
	int memberID;
	int memberClassType;
	int parentClassType;
	char* label;

};

enum CLASS_DEF_TYPES {
	E00xTYPE_FLOAT,
	E01xTYPE_COLOR,
	E02xTYPE_STYLESHEETSTATE,
	E03xTYPE_STYLESHEET,
	E04xTYPE_RES,
	E05xTYPE_RES,
	E06xTYPE_RES,
	E07xTYPE_RES,
	E08xTYPE_RES,
	E09xTYPE_RES,
	E10xTYPE_RES,
	E11xTYPE_RES,
	E12xTYPE_RES,
	E13xTYPE_RES,
	E14xTYPE_RES,
	E15xTYPE_RES,
	ExxxTYPE_SIZEOF
};

enum CD_FLOAT_MEMBERS {
	E00xFLOAT_VAL,
	ExxxFLOAT_SIZEOF
};

enum CD_COLOR_MEMBERS {
	E00xCOLOR_R,
	E01xCOLOR_G,
	E02xCOLOR_B,
	E03xCOLOR_A,
	ExxxCOLOR_SIZEOF
};

enum CD_STYLESHEETSTATE_MEMBERS {
	
	E00xSSS_BGCOL0,
	E01xSSS_BGCOL1,
	E02xSSS_FGCOL0,
	E03xSSS_FGCOL1,
	E04xSSS_BGCOLTEXT0,
	E05xSSS_BGCOLTEXT1,
	E06xSSS_FGCOLTEXT0,
	E07xSSS_FGCOLTEXT1,
	E08xSSS_TGCOL0,
	E09xSSS_TGCOL1,
	E10xSSS_BDCOL,
	E11xSSS_PADDING,
	E12xSSS_BORDER,
	E13xSSS_MARGIN,
	E14xSSS_CORNERRAD,
	E15xSSS_ROUNDNESS,
	ExxxSSS_SIZEOF
};

enum CD_STYLESHEET_MEMBERS {
	E00xSS_UP,
	E01xSS_OVER,
	E02xSS_DOWN,
	ExxxSS_SIZE
}


class ClassDef {
public:
	int classDefType;
	string className;
	vector<string> memberLabels;
	vector<int> memberTypes;

	DefNode() {

	}

	void init() {

	}
};

class ClassInst {
public:
	int classDefType;
	vector<float> values;

	InstNode() {
		
	}

	void init() {
		
	}
};