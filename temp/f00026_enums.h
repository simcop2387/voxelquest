

enum E_GUI_DATA_STRINGS {
	E_GDS_DATA_SOURCE,
	E_GDS_DATA_FILE,
	//E_GDS_CHILD_TYPE,
	E_GDS_CHILD_NAME,
	E_GDS_MATERIAL,
	E_GDS_LAST_KEY,
	E_GDS_LENGTH
};

enum E_PERFORMANCE_PROFILE {
	E_PP_FAST_LOWRES,
	E_PP_SCROLLING,
	E_PP_HIGHQUALITY,
	E_PP_FAST_MODIFICATION,
	E_PP_TERRAIN_MODIFICATION,
	E_PP_ENTITY,
	E_PP_LENGTH
};



enum E_PRIM_TYPE_TER {
	E_PTT_TER,
	E_PTT_WAT,
	E_PTT_LST,
	E_PTT_EMP,
	E_PTT_LENGTH
};

enum E_THREAD_TYPE {
	E_TT_GENPATHS,
	E_TT_GENLIST,
	E_TT_LENGTH
};

enum E_PRIM_TYPE_EXT {
	E_PTT_IDE,
	E_PTT_STB,
	E_PTT_BLD,
	E_PTT_RS1,
	E_EXT_LENGTH
};

enum E_DRAG_TYPE {
	E_DT_NOTHING, // can drag to, but not from
	E_DT_WORLD_OBJECT,
	E_DT_INV_OBJECT,
	E_DT_INV_OBJECT_PARENT,
	E_DT_LENGTH
};

enum E_VOCAB {
	E_VC_FLOAT,
	E_VC_IFTHEN,
	E_VC_ALL,
	E_VC_ANY,
	E_VC_NONE,
	E_VC_LIST,
	E_VC_EQUAL,
	E_VC_NOTEQUAL,
	E_VC_ISBETWEEN,
	E_VC_LENGTH	
};

enum E_VOCAB_TYPE {
	
	E_VCT_FLOAT, // terminal
	E_VCT_NOUN, // terminal
	E_VCT_OPERATOR,
	E_VCT_VARIABLE,
	E_VCT_PREDICATE,
	E_VCT_LENGTH
};

struct FluidPlane {
	std::vector<int> planeIds;
};

struct FluidStruct {
	int minX;
	int minY;
	int minZ;
	
	int maxX;
	int maxY;
	int maxZ;
	
	int id;
	long long int collectedWater;
	
	bool didCollectWater;
	
	int fidBegInd;
	int fidEndInd;
	
	int iciBegInd;
	int iciEndInd;
	
};

struct VToken
{
	int index;
	int type;
	int maxParam;
	float floatValue;
	string stringValue;
};

enum E_FLUID_IDS {
	//E_FID_SML,
	E_FID_BIG,
	//E_FID_MIP,
	E_FID_LENGTH	
};

string globString;


int NUM_POLY_STRINGS = 0;
string polyFBOStrings[] = {
	"polyFBO0",
	"polyFBO1",
	"polyFBO2",
	"polyFBO3",
	"polyFBO4"
};

string dragStrings[] = {
	"E_DT_NOTHING",
	"E_DT_WORLD_OBJECT",
	"E_DT_INV_OBJECT",
	"E_DT_INV_OBJECT_PARENT",
	"E_DT_LENGTH"
};


const static int NA_SIZE_IN_BYTES = 32;

struct NetworkAction {
	char data[NA_SIZE_IN_BYTES];
};

enum E_NET_OPS {
	E_NO_TERMINAL,
	E_NO_KEY_ACTION,
	E_NO_ADD_ENT,
	E_NO_REM_ENT,
	E_NO_DRAG_ENT,
	E_NO_LENGTH	
};


enum E_ITEM_CLASS {
	E_IC_NONE,
	E_IC_EQ_HEAD,
	E_IC_EQ_NECK,
	E_IC_EQ_BACK,
	E_IC_EQ_TORSO,
	E_IC_WAIST,
	E_IC_EQ_WRISTS,
	E_IC_EQ_HANDS,
	E_IC_EQ_FINGERS,
	E_IC_EQ_HOLDING,
	E_IC_EQ_FEET,
	E_IC_RESOURCE,
	E_IC_CONSUMABLE,
	E_IC_MISC,
	E_IC_LENGTH,
};

// enum E_TOOL_CLASS {
// 	E_TC_NONE,
// 	E_TC_SWORD,
// 	E_TC_AXE,
// 	E_TC_WAND,
// 	E_TC_BOW,
// 	E_TC_,
// 	E_TC_,
// 	E_TC_,
// 	E_TC_,
// 	E_TC_,
// 	E_TC_,
// 	E_TC_,
// 	E_TC_,
// 	E_TC_,
// 	E_IC_LENGTH,
// };

enum E_VOLUME_WRAPPERS {
	//E_VW_TERGEN,
	E_VW_HOLDER,
	E_VW_WORLD,
	E_VW_VORO,
	E_VW_LENGTH
};

// string volumeWrapperStrings[] = {
// 	"E_VW_TERGEN",
// 	"E_VW_HOLDER",
// 	"E_VW_VORO",
// 	"E_VW_LENGTH"
// };

// do not reorder
enum E_GUI_CHILD_TYPES {
	E_GCT_INV_ITEM,			// 0
	E_GCT_SHADER_PARAM,		// 1
	E_GTC_GENERIC, 			// 2
	E_GTC_CONTAINER,		// 3
	E_GTC_CONTAINER_PARENT, // 4
	E_GCT_LENGTH			// 5
};

enum E_FONT_WRAPPERS {
	EFW_TEXT,
	EFW_ICONS,
	EFW_LENGTH
};

enum E_MOVE_TYPES {
	E_MT_NONE,
	E_MT_RELATIVE,
	E_MT_TRACKBALL,
	E_MT_LENGTH
};

enum eProgramState {
	E_PS_MENU,
	E_PS_IN_GAME,
	E_PS_SIZE
};

enum eProgramAction {
	E_PA_QUIT,
	E_PA_TOGGLE_FULLSCREEN,
	E_PA_REFRESH,
	E_PA_SIZE
};

enum E_TEX_TYPE {
	E_TEX_TYPE_NOISE
};

enum E_RENDER_METHODS {
	E_RENDER_NONE,
	E_RENDER_VOL,
	E_RENDER_LENGTH
};

enum E_KEYMAP {
	KEYMAP_UP,
	KEYMAP_DOWN,
	KEYMAP_FORWARD,
	KEYMAP_BACKWARD,
	KEYMAP_LEFT,
	KEYMAP_RIGHT,
	KEYMAP_FIRE_PRIMARY,
	KEYMAP_GRAB_THROW,
	KEYMAP_LENGTH
};

enum ENT_TYPE {
	E_ENTTYPE_OBJ,
	E_ENTTYPE_MONSTER,
	E_ENTTYPE_NPC,
	E_ENTTYPE_WEAPON,
	E_ENTTYPE_BULLET,
	E_ENTTYPE_TRACE,
	E_ENTTYPE_DEBRIS,
	E_ENTTYPE_LENGTH
};

enum E_STATES {
	E_STATE_INIT_LAUNCH,
	E_STATE_INIT_BEG,
	E_STATE_INIT_END,
	E_STATE_CREATESIMPLEXNOISE_LAUNCH,
	E_STATE_CREATESIMPLEXNOISE_BEG,
	E_STATE_CREATESIMPLEXNOISE_END,
	E_STATE_NEIGHBORSREADY_LAUNCH,
	E_STATE_NEIGHBORSREADY_BEG,
	E_STATE_NEIGHBORSREADY_END,
	E_STATE_COPYTOTEXTURE_LAUNCH,
	E_STATE_COPYTOTEXTURE_BEG,
	E_STATE_COPYTOTEXTURE_END,
	E_STATE_GENERATEVOLUME_LAUNCH,
	E_STATE_GENERATEVOLUME_BEG,
	E_STATE_GENERATEVOLUME_END,
	E_STATE_WAIT,
	E_STATE_LENGTH

};

enum EVENT_OPS {
	EV_COLLISION,
	EV_LENGTH	
};

// enum E_UI_LIST {
// 	E_UL_FB_MEM,
// 	E_UL_CHUNK_MEM,
// 	E_UL_TOT_MEM,
// 	E_UL_LENGTH
// };

// enum E_UI_LIST {
// 	E_UL_FB_MEM,
// 	E_UL_CHUNK_MEM,
// 	E_UL_TOT_MEM,
// 	E_UL_LENGTH
// };


// enum E_ENTPOOL {
// 	E_ENTPOOL_BULLET,
// 	E_ENTPOOL_TRACE,
// 	E_ENTPOOL_LENGTH
// };

struct DebrisStruct {
	btVector3 pos;
};

struct ExplodeStruct {
	btVector3 pos;
	float radius;
	float power;	
};

struct EntPool {
	int curIndex;
	int maxCount;
	int entType;
	std::vector<int> entIds;
};

struct KeyStackEvent {
	unsigned char key;
	bool keyDown;
	int x;
	int y;
};

enum E_TER_TYPE {
	E_TER_GROUNDLEVEL,
	E_TER_UNDERGROUND,
	E_TER_AIR,
	E_TER_LENGTH
};

enum E_LAYER_TYPE {
	E_LAYER_SOLID,
	E_LAYER_TRANS,
	E_LAYER_NULL, // must be last
	E_LAYER_LENGTH
};


enum E_MOUSE_STATE {
	E_MOUSE_STATE_MOVE,
	E_MOUSE_STATE_BRUSH,
	E_MOUSE_STATE_PICKING,
	E_MOUSE_STATE_LENGTH
};
string mouseStateStrings[] = {
	"E_MOUSE_STATE_MOVE",
	"E_MOUSE_STATE_BRUSH",
	"E_MOUSE_STATE_PICKING",
	"E_MOUSE_STATE_LENGTH"
};

enum E_BRUSH {
	E_BRUSH_ADD,
	E_BRUSH_SUB,
	E_BRUSH_REF,
	E_BRUSH_MOVE,
	E_BRUSH_LENGTH
};

enum E_HOLDER_ACTION {
	E_HOLDER_ACTION_RENDER,
	E_HOLDER_ACTION_REDRAW_CHILDREN,
	E_HOLDER_ACTION_RESET,
	E_HOLDER_ACTION_LENGTH
};

enum E_PLANE_TYPES {
	E_PLANE_XY,
	E_PLANE_YZ,
	E_PLANE_XZ,
	E_PLANE_LENGTH
};

// enum E_GRASS_STATE {
//     E_GRASS_STATE_OFF,
//     E_GRASS_STATE_ON,
//     E_GRASS_STATE_ANIM,
//     E_GRASS_STATE_LENGTH
// };

enum E_FILL_STATE {
	E_FILL_STATE_EMPTY,
	E_FILL_STATE_PARTIAL,
	E_FILL_STATE_FULL,
};

enum E_PLANT_TYPES {
	E_PT_OAK_TRUNK,
	E_PT_OAK_ROOTS,
	E_PT_OAK2_TRUNK,
	E_PT_OAK2_ROOTS,
	E_PT_BARE_OAK_TRUNK,
	E_PT_BARE_OAK_ROOTS,
	E_PT_LENGTH
};

enum E_OBJ {
	E_OBJ_CAMERA,
	//E_OBJ_LIMBTARG0,
	//E_OBJ_FOG,
	//E_OBJ_CUTAWAY,
	//E_OBJ_HUMAN,
	//E_OBJ_P0,
	//E_OBJ_P1,
	//E_OBJ_P2,
	E_OBJ_LIGHT0,
	E_OBJ_LIGHT1,
	E_OBJ_LIGHT2,
	E_OBJ_LIGHT3,
	E_OBJ_LIGHT4,
	E_OBJ_LIGHT5,
	E_OBJ_LENGTH // LIGHTS MUST BE LAST ENTRIES
};

enum E_MAT_SUBPARAM {
	E_MAT_SUBPARAM_NONE,
	E_MAT_SUBPARAM_TUDOR,
	E_MAT_SUBPARAM_DOCK,
	E_MAT_SUBPARAM_BRICK,
	E_MAT_SUBPARAM_BRICK_ARCH,
	E_MAT_SUBPARAM_LENGTH
};


enum E_MAT_PARAM {
	E_MAT_PARAM_FOUNDATION,
	E_MAT_PARAM_ROOF,
	//E_MAT_PARAM_WALKWAY_TOP,
	//E_MAT_PARAM_DOCK,
	E_MAT_PARAM_BUILDING,
	//E_MAT_PARAM_WALKWAY,
	E_MAT_PARAM_LANTERN,
	E_MAT_PARAM_DOORWAY,
	E_MAT_PARAM_DOOR,
	//E_MAT_PARAM_STAIRS,
	E_MAT_PARAM_TREE, // TREES MUST BE LAST ENTRY
	E_MAT_PARAM_LENGTH
};


enum E_ORGTYPE {
	E_ORGTYPE_HUMAN,
	E_ORGTYPE_WEAPON,
	E_ORGTYPE_LENGTH	
};

enum E_ORG_SUBPARAM {
	E_ORG_SUBPARAM_NOT_SEL,
	E_ORG_SUBPARAM_SEL,
	E_ORG_SUBPARAM_LENGTH
};

enum E_ORG_PARAM {
	E_ORG_PARAM_GEOM,
	E_ORG_PARAM_LINES,
	E_ORG_PARAM_LENGTH
};


// enum E_CONNECTION_SUBTYPES {
//     E_CST_NULL,
//     E_CST_TUDOR,
//     E_CST_BRICK,
//     E_CST_LENGTH
// };

enum E_BLOCK_GEN {
	E_BG_ROADS_AND_BUILDINGS,
	//E_BG_STAIRS,
	//E_BG_REMOVE_REDUNDANT_Z_SEGMENTS,
	E_BG_ROOFS,
	//E_BG_BASEMENTS,
	E_BG_WING_TIPS,
	E_BG_DOORS,
	E_BG_FILL_TERRAIN,
	E_BG_LENGTH
};

enum E_CONNECTION_TYPE_CLASSES {
	E_CTC_NOTHING,
	E_CTC_ROAD,
	E_CTC_ROOF,
	E_CTC_ROOM,
	E_CTC_STAIRS,
	E_CTC_LANTERN,
	E_CTC_LENGTH
};

enum E_CONNECTION_TYPES {
	E_CT_NULL,
	E_CT_CONNECTED,
	E_CT_FOUNDATION,
	E_CT_ROAD,
	E_CT_DOCK,
	E_CT_MAINHALL,
	E_CT_WING,
	E_CT_ROOF,
	E_CT_WALKWAY_TOP,
	E_CT_ROOM_BRICK,
	E_CT_ROOM_TUDOR,
	E_CT_WALKWAY,
	E_CT_STAIRS,
	E_CT_LANTERN,
	E_CT_WINDOWFRAME,
	E_CT_WINDOW,
	E_CT_DOORWAY,
	E_CT_DOOR,
	E_CT_OBJECT,
	E_CT_LINES,
	E_CT_TREE,
	E_CT_LENGTH
};


// enum E_HOLDER_STATE {
// 	EHS_INIT_BEG,
// 	EHS_INIT_END,
// 	EHS_
// }

// enum E_BUILDING_TYPE {
// 	E_BT_NULL,
// 	E_BT_ROAD,
// 	E_BT_DOCK,
// 	E_BT_MAINHALL,
// 	E_BT_WING,
// 	// E_BT_BALCONY,
// 	// E_BT_TOWER,
// 	// E_BT_LANTERN,
// 	// E_BT_DOORWAY,
// 	// E_BT_WINDOWFRAME,
// 	// E_BT_DOOR,
// 	// E_BT_WINDOW,
// 	//E_BT_OBJ,
// 	//E_BT_TREE,
// 	E_BT_LENGTH
// };


enum E_BONES_HUMAN {
	
	E_BONE_L_BEG, ///////////////
	E_BONE_L_SHOULDER,
	E_BONE_L_UPPERARM,
	E_BONE_L_LOWERARM,
	E_BONE_L_METACARPALS,
	E_BONE_L_PROXIMAL0, // pinky
	E_BONE_L_PROXIMAL1, // ring
	E_BONE_L_PROXIMAL2, // middle
	E_BONE_L_PROXIMAL3, // index
	E_BONE_L_PROXIMAL4, // thumb
	E_BONE_L_INTER0, // pinky
	E_BONE_L_INTER1, // ring
	E_BONE_L_INTER2, // middle
	E_BONE_L_INTER3, // index
	E_BONE_L_INTER4, // thumb
	E_BONE_L_DISTAL0, // pinky
	E_BONE_L_DISTAL1, // ring
	E_BONE_L_DISTAL2, // middle
	E_BONE_L_DISTAL3, // index
	E_BONE_L_DISTAL4, // thumb
	E_BONE_L_HIP,
	E_BONE_L_UPPERLEG,
	E_BONE_L_LOWERLEG,
	E_BONE_L_TALUS,
	E_BONE_L_PHALANGE,
	E_BONE_L_END, //////////////
	E_BONE_R_BEG, //////////////
	E_BONE_R_SHOULDER,
	E_BONE_R_UPPERARM,
	E_BONE_R_LOWERARM,
	E_BONE_R_METACARPALS,
	E_BONE_R_PROXIMAL0, // pinky
	E_BONE_R_PROXIMAL1, // ring
	E_BONE_R_PROXIMAL2, // middle
	E_BONE_R_PROXIMAL3, // index
	E_BONE_R_PROXIMAL4, // thumb
	E_BONE_R_INTER0, // pinky
	E_BONE_R_INTER1, // ring
	E_BONE_R_INTER2, // middle
	E_BONE_R_INTER3, // index
	E_BONE_R_INTER4, // thumb
	E_BONE_R_DISTAL0, // pinky
	E_BONE_R_DISTAL1, // ring
	E_BONE_R_DISTAL2, // middle
	E_BONE_R_DISTAL3, // index
	E_BONE_R_DISTAL4, // thumb
	E_BONE_R_HIP,
	E_BONE_R_UPPERLEG,
	E_BONE_R_LOWERLEG,
	E_BONE_R_TALUS,
	E_BONE_R_PHALANGE,
	E_BONE_R_END, /////////////////
	E_BONE_C_BEG, /////////////////
	E_BONE_C_BASE,
	E_BONE_C_SPINE0, // tail
	E_BONE_C_SPINE1,
	E_BONE_C_SPINE2,
	E_BONE_C_SPINE3,
	E_BONE_C_SPINE4, // neck
	E_BONE_C_SKULL,
	
	E_BONE_WEAPON_BASE,
	E_BONE_WEAPON_END,
	E_BONE_WEAPON_0,
	E_BONE_WEAPON_1,
	E_BONE_WEAPON_2,
	E_BONE_WEAPON_3,
	E_BONE_WEAPON_4,
	E_BONE_WEAPON_5,
	E_BONE_WEAPON_6,
	E_BONE_WEAPON_7,
	E_BONE_WEAPON_8,
	
	E_BONE_C_END //////////////////
	
};

enum E_WEAPON_STATES {
	E_WEAPON_STATE_IDLE,
	E_WEAPON_STATE_BEG,
	E_WEAPON_STATE_END,
	E_WEAPON_STATE_LENGTH	
};

enum E_WEAPON_POS {
	E_WEAPON_POS_RELAXED,
	
	E_WEAPON_POS_FORWARD,
	
	E_WEAPON_POS_LEFT,
	E_WEAPON_POS_RIGHT,
	E_WEAPON_POS_UP,
	E_WEAPON_POS_DOWN,
	
	E_WEAPON_POS_UP_LEFT,
	E_WEAPON_POS_UP_RIGHT,
	E_WEAPON_POS_DOWN_LEFT,
	E_WEAPON_POS_DOWN_RIGHT,
	
	E_WEAPON_POS_LENGTH
};

enum E_GUI {
	E_GUI_LENGTH
};

enum eAlignH {
	E_ALIGNH_LEFT,
	E_ALIGNH_CENTER,
	E_ALIGNH_RIGHT,
};

enum eAlignV {
	E_ALIGNV_TOP,
	E_ALIGNV_MIDDLE,
	E_ALIGNV_BOTTOM	
};

enum eFillDir {
	E_FILL_HORIZONTAL,
	E_FILL_VERTICAL
};

// do not reorder
enum E_ORG_VECS {
	E_OV_TANGENT,
	E_OV_BITANGENT,
	E_OV_NORMAL,
	E_OV_TBNRAD0,
	E_OV_TBNRAD1,
	E_OV_THETAPHIRHO,
	E_OV_TPRORIG,
	E_OV_MATPARAMS,
	E_OV_LENGTH
};


string boneStrings[] = {
	
	"E_BONE_L_BEG", ///////////////
	"E_BONE_L_SHOULDER",
	"E_BONE_L_UPPERARM",
	"E_BONE_L_LOWERARM",
	"E_BONE_L_METACARPALS",
	"E_BONE_L_PROXIMAL0", // pinky
	"E_BONE_L_PROXIMAL1", // ring
	"E_BONE_L_PROXIMAL2", // middle
	"E_BONE_L_PROXIMAL3", // index
	"E_BONE_L_PROXIMAL4", // thumb
	"E_BONE_L_INTER0", // pinky
	"E_BONE_L_INTER1", // ring
	"E_BONE_L_INTER2", // middle
	"E_BONE_L_INTER3", // index
	"E_BONE_L_INTER4", // thumb
	"E_BONE_L_DISTAL0", // pinky
	"E_BONE_L_DISTAL1", // ring
	"E_BONE_L_DISTAL2", // middle
	"E_BONE_L_DISTAL3", // index
	"E_BONE_L_DISTAL4", // thumb
	"E_BONE_L_HIP",
	"E_BONE_L_UPPERLEG",
	"E_BONE_L_LOWERLEG",
	"E_BONE_L_TALUS",
	"E_BONE_L_PHALANGE",
	"E_BONE_L_END", //////////////
	"E_BONE_R_BEG", //////////////
	"E_BONE_R_SHOULDER",
	"E_BONE_R_UPPERARM",
	"E_BONE_R_LOWERARM",
	"E_BONE_R_METACARPALS",
	"E_BONE_R_PROXIMAL0", // pinky
	"E_BONE_R_PROXIMAL1", // ring
	"E_BONE_R_PROXIMAL2", // middle
	"E_BONE_R_PROXIMAL3", // index
	"E_BONE_R_PROXIMAL4", // thumb
	"E_BONE_R_INTER0", // pinky
	"E_BONE_R_INTER1", // ring
	"E_BONE_R_INTER2", // middle
	"E_BONE_R_INTER3", // index
	"E_BONE_R_INTER4", // thumb
	"E_BONE_R_DISTAL0", // pinky
	"E_BONE_R_DISTAL1", // ring
	"E_BONE_R_DISTAL2", // middle
	"E_BONE_R_DISTAL3", // index
	"E_BONE_R_DISTAL4", // thumb
	"E_BONE_R_HIP",
	"E_BONE_R_UPPERLEG",
	"E_BONE_R_LOWERLEG",
	"E_BONE_R_TALUS",
	"E_BONE_R_PHALANGE",
	"E_BONE_R_END", /////////////////
	"E_BONE_C_BEG", /////////////////
	"E_BONE_C_BASE",
	"E_BONE_C_SPINE0", // tail
	"E_BONE_C_SPINE1",
	"E_BONE_C_SPINE2",
	"E_BONE_C_SPINE3",
	"E_BONE_C_SPINE4",
	"E_BONE_C_SPINE5",
	"E_BONE_C_SPINE6", // neck
	"E_BONE_C_SKULL",
	
	"E_BONE_WEAPON_BASE",
	"E_BONE_WEAPON_END",
	"E_BONE_WEAPON_0",
	"E_BONE_WEAPON_1",
	"E_BONE_WEAPON_2",
	"E_BONE_WEAPON_3",
	"E_BONE_WEAPON_4",
	"E_BONE_WEAPON_5",
	"E_BONE_WEAPON_6",
	"E_BONE_WEAPON_7",
	"E_BONE_WEAPON_8",
	
	"E_BONE_C_END" //////////////////
	
	
	
};


// NEVER REORDER
enum E_EQUIPMENT_SLOTS {
	E_ES_HELMET,	// 0
	E_ES_NECKLACE,	// 1
	E_ES_CLOAK,		// 2
	E_ES_ARMOR,		// 3
	E_ES_GLOVES,	// 4
	E_ES_PRIMARY,	// 5
	E_ES_SECONDARY,	// 6
	E_ES_BOOTS,		// 7
	E_ES_LENGTH 	// 8
};

// // NEVER REORDER

// enum E_ITEM_IDS {
// 	E_II_IRON_LONG_SWORD,
// 	E_II_IRON_KITE_SHIELD,
// 	E_II_IRON_PLATED_MAIL,
// 	E_II_LENGTH
	
// }

enum E_COMMON_PARAMS {
	E_CP_VISMININPIXELST,
	E_CP_VISMAXINPIXELST,
	E_CP_LENGTH
};

// enum E_ACTOR_PARAMS {
// 	E_AC_RESERVED0,
// 	E_AC_RESERVED1,
// 	E_AC_UNUSED00,
// 	E_AC_UNUSED01,
// 	E_AC_UNUSED1,
// 	E_AC_UNUSED2,
// 	E_AC_UNUSED3,
// 	E_AC_UNUSED4,
// 	E_AC_UNUSED5,
// 	E_AC_UNUSED6, // must be last
// 	E_AC_LENGTH
// };

enum E_LIGHT_PARAMS {
	E_LP_RESERVED0,
	E_LP_RESERVED1,
	E_LP_POSITION,
	E_LP_RADIUS,
	E_LP_COLOR,
	E_LP_UNUSED2,
	E_LP_UNUSED3,
	E_LP_UNUSED4,
	E_LP_UNUSED5,
	E_LP_UNUSED6, // must be last
	E_LP_LENGTH
};

enum E_TREE_PARAMS {
	E_TP_RESERVED0,
	E_TP_RESERVED1,
	E_TP_P0,
	E_TP_P1,
	E_TP_P2,
	E_TP_POWERVALS,
	E_TP_POWERVALS2,
	E_TP_THICKVALS,
	E_TP_UNUSED,
	E_TP_MATPARAMS, // must be last
	E_TP_LENGTH
};

enum E_LINES_PARAMS {
	E_AP_RESERVED0,
	E_AP_RESERVED1,
	E_AP_ORG, // origin
	E_AP_TAN, // tangent (not normalized)
	E_AP_BIT, // bitangent (normalized)
	E_AP_NOR, // normal (normalized)
	E_AP_RAD0, // radius
	E_AP_RAD1,
	E_AP_UNUSED1,
	E_AP_MATPARAMS, // must be last
	E_AP_LENGTH
};

enum E_GEOM_PARAMS {
	E_GP_RESERVED0, //visMin
	E_GP_RESERVED1, //visMax
	E_GP_BOUNDSMININPIXELST,
	E_GP_BOUNDSMAXINPIXELST,
	E_GP_CORNERDISINPIXELS,
	E_GP_POWERVALS,
	E_GP_POWERVALS2,
	E_GP_THICKVALS,
	E_GP_UNUSED,
	E_GP_MATPARAMS, // must be last
	E_GP_LENGTH
};

enum E_PRIMTEMP {
	E_PRIMTEMP_VISMIN,
	E_PRIMTEMP_VISMAX,
	E_PRIMTEMP_BOUNDSMIN,
	E_PRIMTEMP_BOUNDSMAX,
	E_PRIMTEMP_CORNERDIS, //E_PRIMTEMP_CORNERDISTHICKNESSPOWER
	E_PRIMTEMP_MATPARAMS,
	E_PRIMTEMP_LENGTH
};

const static int FLOATS_PER_TEMPLATE = ((int)E_PRIMTEMP_LENGTH)*4;
const static float defaultTemplate[FLOATS_PER_TEMPLATE] = {
	-2.0,-2.0,-2.0, 0.0,
	 2.0, 2.0, 2.0, 0.0,
	-2.0,-2.0,-2.0, 0.0,
	 2.0, 2.0, 2.0, 0.0,
	 1.0, 1.0, 1.0, 1.0,
	 0.0, 0.0, 0.0, 0.0
};

string primTempStrings[] = {
	"visMin",
	"visMax",
	"boundsMin",
	"boundsMax",
	"cornerDis",
	"matParams"
};



enum E_GEOM_POINTS {
	E_GEOM_POINTS_ORIGIN,
	E_GEOM_POINTS_RAD_XYZ,
	E_GEOM_POINTS_OFFSET,
	E_GEOM_POINTS_CORNER,
	E_GEOM_POINTS_POWER_VALS,
	E_GEOM_POINTS_NEG_RAD_XY,
	E_GEOM_POINTS_POS_RAD_XY,
	E_GEOM_POINTS_NEG_RAD_Z,
	E_GEOM_POINTS_POS_RAD_Z,
	E_GEOM_POINTS_THICKNESS,
	E_GEOM_POINTS_LENGTH
};

enum E_GEOM_POINTS_TEMP {
	E_GEOM_POINTS_TEMP_ORIGIN,
	E_GEOM_POINTS_TEMP_OFFSET,
	E_GEOM_POINTS_TEMP_LENGTH
};

string E_GEOM_POINTS_STR[] = {
	"E_GEOM_POINTS_ORIGIN",
	"E_GEOM_POINTS_RAD_XYZ",
	"E_GEOM_POINTS_OFFSET",
	"E_GEOM_POINTS_CORNER",
	"E_GEOM_POINTS_POWER_VALS",
	"E_GEOM_POINTS_NEG_RAD_XY",
	"E_GEOM_POINTS_POS_RAD_XY",
	"E_GEOM_POINTS_NEG_RAD_Z",
	"E_GEOM_POINTS_POS_RAD_Z",
	"E_GEOM_POINTS_THICKNESS",
	"E_GEOM_POINTS_LENGTH"
};

enum E_PRIM_LAYERS {
	E_PL_TERRAIN,
	//E_PL_PRIMIDS,
	E_PL_LENGTH
};

enum E_PUSH_MOD {
	E_PM_EXPLODE_BULLET,
	E_PM_MODIFY_UNIT,
	E_PM_PLACE_TEMPLATE,
	E_PM_LENGTH
};



enum E_ENT_TYPES {
	E_ET_GEOM,
	//E_ET_ACTOR,
	E_ET_LIGHT,
	E_ET_LENGTH
	
};

enum E_ENT_PARAM {
	
	E_EP_CURCENTERPOSINPIXELS,
	E_EP_RADIUSINPIXELS,
	E_EP_CURMINVISBOUNDSINPIXELS,
	E_EP_CURMAXVISBOUNDSINPIXELS,
	E_EP_MOVEMINBOUNDSINPIXELS,
	E_EP_MOVEMAXBOUNDSINPIXELS,
	E_EP_LENGTH
};


enum E_NODE_TYPE {
	E_NT_CONNECTIONPROP,
	E_NT_SHORTPROP,
	E_NT_DYNPROP,
	E_NT_CENTERPROP,
	E_NT_LENGTH
};


enum E_ALIGN {
	E_ALIGN_BOTTOM,
	E_ALIGN_MIDDLE,
	E_ALIGN_TOP
};

enum E_MAT_CODE {
	E_MC_DEFAULT,
	E_MC_HSV,
	E_MC_MATERIAL,
	E_MC_LENGTH
};




// enum E_PATH_INDEX_TYPE {
// 	E_PIT_TYPE,
// 	E_PIT_RES1,
// 	E_PIT_RES2,
// 	E_PIT_RES3,
// 	E_PIT_LENGTH
// };

enum E_PATH_VALUE_TYPE {
	E_PVT_INVALID = 1,
	E_PVT_LAND_BELOW = 2,
	E_PVT_LENGTH = 128
};


// enum E_CELL_DATA {
// 	E_CD_EMPTY,
// 	E_CD_WATER,
// 	E_CD_SOLID,
// 	//E_CD_PORTAL, // cost of moving through a door is very high, but less than a wall
// 	E_CD_LENGTH	
// };

// dont use enums, not cast to uint
const static uint E_CD_UNKNOWN = 0;
const static uint E_CD_EMPTY = 1;
const static uint E_CD_WATER = 2;
const static uint E_CD_SOLID = 4;

struct ConnectingNodeStruct {
	
	int blockIdFrom;
	int holderIdFrom;
	
	int blockIdTo;
	int holderIdTo;
	
	int groupIdFrom;
	int groupIdTo;
	
	int cellIndFrom;
	int cellIndTo;
	
	int totCost;
};

struct GroupIdStruct {
	int ind;
	int groupId;
	
	
	int cameFromInd;
	int pathCost;
};

struct GroupInfoStruct {
	//bool touchesFace[6];
	int begInd;
	int endInd;
	int centerInd;
	
	int visitId;
};


struct PathResult {
	
	ConnectingNodeStruct conNode;
	
	int blockId;
	int holderId;
	int groupId;
	
	int lastBlockId;
	int lastHolderId;
	int lastGroupId;
	
	int lastIndex;
};

enum E_PATH_FILL_OPS {
	E_PFO_CLEAR_GROUPS,
	E_PFO_SEARCH_GROUPS,
	E_PFO_LENGTH
};



enum E_COL_TYPES {
    COL_NOTHING = 0,
    COL_STATIC = 1,
    COL_DYN = 2,
    COL_BODY = 4,
    COL_MARKER = 8,
    COL_WEAPON = 16
};

const static int terCollidesWith = COL_STATIC|COL_DYN|COL_BODY|COL_MARKER|COL_WEAPON;
const static int bodyCollidesWith = COL_STATIC|COL_DYN;//COL_NOTHING;//
const static int markerCollidesWith = COL_STATIC|COL_DYN;
const static int dynCollidesWith = COL_STATIC|COL_DYN|COL_BODY|COL_MARKER|COL_WEAPON;
const static int weaponCollidesWith = COL_STATIC|COL_DYN;// |COL_WEAPON;



struct BodyStruct {
	btRigidBody* body;
	btVector3 lastVel;
	btVector3 totAV;
	btVector3 totLV;
	
	float mass;
	int boneId;
	bool isBall;
	
	bool isVisible;
	bool inWater;
	bool isFalling;
	bool hasContact;
	bool isInside;
};

struct ActorJointStruct {
	int boneId;
	bool isBall;
	float rad;
	float length;
	
	
	
	btVector3 begOrig;
	btVector3 midOrig;
	btVector3 endOrig;
	btVector3 targAlignT;
	btVector3 targAlignB;
	btVector3 targAlignN;
	btQuaternion quat;
	btMatrix3x3 basis;
	//btVector3 pivotAxis;
	
	int jointId;
	int parentId;
	std::vector<int> children;
	btCollisionShape* shape;
	btRigidBody* body;
	btTypedConstraint* joint;
};

// struct PathNode {
// 	// uint flags;
// 	// int cameFromInd;
// 	// int pathCost;
// 	// int groupId;
	
// 	giIndex
	
// };


enum E_POSE_GROUPS {
	E_PG_JUMP,
	E_PG_IDLE,
	E_PG_WALK,
	E_PG_LENGTH	
};

enum E_POSE_KEYS {
	E_PK_T_POSE,
	E_PK_JUMP,
	E_PK_IDLE_LOW,
	E_PK_IDLE_HIGH,
	E_PK_L_FORWARD,
	E_PK_R_CROSS,
	E_PK_R_FORWARD,
	E_PK_L_CROSS,
	E_PK_LENGTH
	
};

string poseStrings[] = {
	"E_PK_T_POSE",
	"E_PK_JUMP",
	"E_PK_IDLE_LOW",
	"E_PK_IDLE_HIGH",
	"E_PK_L_FORWARD",
	"E_PK_R_CROSS",
	"E_PK_R_FORWARD",
	"E_PK_L_CROSS",
	"E_PK_LENGTH"
};

struct materialNode {
	float h;
	float s;
	float l;
	float r;
	float g;
	float b;
	float power;
	float ratio;
};

struct iVector4 {
	int x;
	int y;
	int z;
	int w;
};
struct fVector4 {
	float x;
	float y;
	float z;
	float w;
};

struct iVector3 {
	int x;
	int y;
	int z;
};
struct fVector3 {
	float x;
	float y;
	float z;
};


struct iVector2 {
	int x;
	int y;
};
struct fVector2 {
	float x;
	float y;
};

struct iBoundingBox {
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

struct fBoundingBox {
	float xMin;
	float yMin;
	float xMax;
	float yMax;
};





struct intPair {
	int v0;
	int v1;
};

bool operator==(const intPair& lhs, const intPair& rhs)
{
    return (lhs.v0 == rhs.v0)&&(lhs.v1==rhs.v1);
}

bool operator>(const intPair& lhs, const intPair& rhs)
{
	long lhs2 = lhs.v1;
	long rhs2 = rhs.v1;

	lhs2 = lhs2 << 16;
	rhs2 = rhs2 << 16;

	lhs2 |= lhs.v0;
	rhs2 |= rhs.v0;


	return (lhs2) > (rhs2);
}

bool operator<(const intPair& lhs, const intPair& rhs)
{
    long lhs2 = lhs.v1;
	long rhs2 = rhs.v1;

	lhs2 = lhs2 << 16;
	rhs2 = rhs2 << 16;

	lhs2 |= lhs.v0;
	rhs2 |= rhs.v0;

	return (lhs2) < (rhs2);
}

struct intPairVec {
	std::vector<intPair> data;	
};
struct intVec {
	std::vector<int> data;	
};

struct floatAndIndex {
	float value;
	int index1;
	int index2;
};

struct coordAndIndex {
	float x;
	float y;
	float xTen;
	float yTen;
	int index;
};

struct PlantRules
{
	float numChildren[2];
	float divergenceAngleV[2];
	float begThickness;
	float endThickness;
	float curLength[MAX_PLANT_GEN];
	float sphereGen;
	float sphereRad;
	//float baseLength;
	//float nodeLengthMultiplier;
	float numGenerations;
	float angleUniformityU;
	float isInit;


};

struct FloatVec {
	std::vector<float> data;	
};

struct TerTexture {
	int usedByBlockId;
	GLuint texId;
	bool alreadyBound;
};


// struct PathNode {
// 	int index;
// 	int blockId;
// 	int x;
// 	int y;
// 	int z;
// };
// bool operator==(const PathNode& lhs, const PathNode& rhs)
// {
//     return (lhs.index == rhs.index)&&(lhs.blockId==rhs.blockId);
// }

// struct PathHolder {
// 	std::vector<PathNode> pathList;
// 	std::vector<PathNode> visitedList;
// };


struct MapNode {
	int connectionProps[TOT_MAP_DIRS];
	int terHeight;
	int adjustedHeight;
	int houseHeight;
	int id;
};

const static unsigned int BC_FLAG_INSIDE = 1;
const static unsigned int BC_FLAG_WING_BEG = 2;
const static unsigned int BC_FLAG_WING_END = 4;

struct BuildingCon {
	int conType;
	unsigned int nodeFlags;
	//bool isWingBeg;
	//bool isWingEnd;
	float wingMult;
	float wallRadInCells;
	int heightDelta;
	int direction; 	// -1: negative,
					//  0: neutral,
					//  1: positve
};


struct BuildingNode {

	BuildingCon con[TOT_NODE_VALS];

	// int con[TOT_NODE_VALS]; // connections
	// bool isWingBeg[TOT_NODE_VALS];
	// bool isWingEnd[TOT_NODE_VALS];

	//std::vector<ConnectionProp> connections;

	int mazeIndex;
	int id;
	int visited;
	float powerValU;
	float powerValV;
	bool nearTerrain;
	bool nearAir;

	// float terHeight;
	// bool isWingTip;

	// int centerType;
	// int connectionTypes[4];
	// int shortTypes[4];

	// int centerHeight;
	// int connectionHeights[4];
	// int shortHeights[4];


};



const int GLUT_NO_BUTTON = 99;
const int GLUT_OVER = 8;
const int GLUT_OUT = 9;
const int GLUT_CHANGING = 10;


enum E_MUSIC_LIST {
	EML_BIRDSONG0,
	EML_CRICKETS0,
	EML_OCEANWAVES0,
	EML_UNDERWATER0,
	EML_LENGTH
};



string colorStrings[] = {
	"Red",
	"Orange",
	"Yellow",
	"Green",
	"Aqua",
	"Blue",
	"Purple",
	"Pink",
	"Brown",
	"Black",
	"Gray",
	"White",
	"Empty",
	"Broken"
};
string gemStrings[] = {
	"Ruby",
	"Citrine",
	"Topaz",
	"Emerald",
	"Aquamarine",
	"Sapphire",
	"Amethyst",
	"Quartz",
	"Beryl",
	"Onyx",
	"Moonstone",
	"Diamond"
};
string metalStrings[] = {
	"Copper",
	"Silver",
	"Gold",
	"Platinum",
	"Meteorite"
};
string weaponStrings[] = {
	"Sword",
	"Axe",
	"Spear",
	"Dagger",
	"Mace",
	"Staff",
	"Wand",
	"Throwing Star",
	"Bow",
	"Knuckles",
	"Punch Dagger"
};
string elementStrings[] = {
	"Normal",
	"Fire",
	"Ice",
	"Poison",
	"Earth",
	"Electric",
	"Light",
	"Dark",
	"Holy",
	"Unholy",
	"Crystal",
	"Artifact",
	"Artifact",
	"Artifact",
	"Artifact",
	"Artifact"
};

string musicStrings[] = {
	"birdsong0",
	"crickets0",
	"oceanwaves0",
	"underwater0"
};





// enum E_GUI_SECTIONS {
// 	E_GS_MAINMENU,
// 	E_GS_CONTEXTMENU,
// 	E_GS_LENGTH
// };

// do not reorder!
enum GUI_TYPES {
	E_GT_HOLDER,  // 0
	E_GT_SLIDER,  // 1 also a toggle if 1 division
	E_GT_BUTTON,  // 2
	E_GT_RADIO,   // 3
	E_GT_MENUBAR, // 4
	E_GT_DRAGPAD, // 5
	E_GT_CHECK,   // 6
	E_GT_LENGTH
};

enum GUI_STRING_TYPES {
	E_GST_LABEL,
	E_GST_UID,
	E_GST_SS,
	E_GST_DATAREF,
	E_GST_DATAFILE,
	E_GST_DATAKEY,
	E_GST_VALREF,
	E_GST_LENGTH
};

char* guiStringTypes[] = {
	"label",
	"uid",
	"ss",
	"dataRef",
	"dataFile",
	"dataKey",
	"valRef"
};

enum E_HOVER_TYPES {
	E_HT_NORMAL,		// 0
	E_HT_TOOLTIP,		// 1
	E_HT_ONSELECTED,	// 2
	E_HT_TOOLTIP_VALUE, // 3
	E_HT_ROOT,			// 4
	E_HT_LENGTH
};

enum GUI_FLOAT_TYPES {
	E_GFT_TYPE,
	E_GFT_DIVISIONS,
	E_GFT_VALUE,
	E_GFT_HASBACKGROUND,
	E_GFT_SINGLELINE,
	E_GFT_FILLRATIOX,
	E_GFT_FILLRATIOY,
	E_GFT_FILLDIR,
	E_GFT_ALIGNX,
	E_GFT_ALIGNY,
	E_GFT_LAYER,
	E_GFT_HOVERTYPE, // 0: normal, 1: tooltip, 2: show when parent is selected
	E_GFT_MAXDIMX,
	E_GFT_MAXDIMY,
	E_GFT_MINDIMX,
	E_GFT_MINDIMY,
	E_GFT_FLAGS,
	E_GFT_VALUE0,
	E_GFT_VALUE1,
	E_GFT_VALUE2,
	E_GFT_VALUE3,
	E_GFT_MATCODE,
	E_GFT_LENGTH
};
char* guiFloatTypes[] = {
	"type",
	"divisions",
	"value",
	"hasBackground",
	"singleLine",
	"fillRatioX",
	"fillRatioY",
	"fillDir",
	"alignX",
	"alignY",
	"layer",
	"hoverType",
	"maxDimX",
	"maxDimY",
	"minDimX",
	"minDimY",
	"flags",
	"value0",
	"value1",
	"value2",
	"value3",
	"matCode"
	
	
};

enum E_GUI_FLAGS {
	E_GF_X = 1,
	E_GF_Y = 2,
	E_GF_Z = 4,
	E_GF_W = 8,
	E_GF_MAT = 16
};

string guiStringValues[E_GST_LENGTH];
double guiFloatValues[E_GFT_LENGTH];

enum E_DIR_SPECS {
	E_DIR_X,
	E_DIR_Y,
	E_DIR_Z,
	E_DIR_LENGTH
};



enum eSSProperties {

	E_SS_BGCOL0_R,
	E_SS_BGCOL0_G,
	E_SS_BGCOL0_B,
	E_SS_BGCOL0_A,

	E_SS_BGCOL1_R,
	E_SS_BGCOL1_G,
	E_SS_BGCOL1_B,
	E_SS_BGCOL1_A,

	E_SS_FGCOL0_R,
	E_SS_FGCOL0_G,
	E_SS_FGCOL0_B,
	E_SS_FGCOL0_A,

	E_SS_FGCOL1_R,
	E_SS_FGCOL1_G,
	E_SS_FGCOL1_B,
	E_SS_FGCOL1_A,

	E_SS_BGCOLTEXT0_R,
	E_SS_BGCOLTEXT0_G,
	E_SS_BGCOLTEXT0_B,
	E_SS_BGCOLTEXT0_A,

	E_SS_BGCOLTEXT1_R,
	E_SS_BGCOLTEXT1_G,
	E_SS_BGCOLTEXT1_B,
	E_SS_BGCOLTEXT1_A,

	E_SS_FGCOLTEXT0_R,
	E_SS_FGCOLTEXT0_G,
	E_SS_FGCOLTEXT0_B,
	E_SS_FGCOLTEXT0_A,

	E_SS_FGCOLTEXT1_R,
	E_SS_FGCOLTEXT1_G,
	E_SS_FGCOLTEXT1_B,
	E_SS_FGCOLTEXT1_A,

	E_SS_TGCOL0_R,
	E_SS_TGCOL0_G,
	E_SS_TGCOL0_B,
	E_SS_TGCOL0_A,

	E_SS_TGCOL1_R,
	E_SS_TGCOL1_G,
	E_SS_TGCOL1_B,
	E_SS_TGCOL1_A,

	E_SS_BDCOL_R,
	E_SS_BDCOL_G,
	E_SS_BDCOL_B,
	E_SS_BDCOL_A,

	E_SS_PADDING,
	E_SS_BORDER,
	E_SS_MARGIN,
	E_SS_CORNERRAD,
	E_SS_ROUNDNESS,

	E_SS_PROPS_SIZE
};

enum E_FIELD_CALLBACKS {
	E_FC_SAVEORG,
	E_FC_LOADORG,
	E_FC_LENGTH
};

enum eCompStates {
	E_COMP_UP,
	E_COMP_OVER,
	E_COMP_DOWN,
	E_COMP_TOTAL
};

struct CharStruct {
	float consumedW;
	float consumedH;
	
	float offsetX;
	float offsetY;
	
	float sampX;
	float sampY;
	float sampW;
	float sampH;
};



class StyleSheetState {
private:

public:

	float props[E_SS_PROPS_SIZE];

	StyleSheetState() {

	}

	void setVal(int offsetVal, float x, float y, float z, float w) {
		props[offsetVal] = x;
		props[offsetVal + 1] = y;
		props[offsetVal + 2] = z;
		props[offsetVal + 3] = w;

	}

	void init() {

		int i;

		for (i = 0; i < E_SS_PROPS_SIZE; i++) {
			props[i] = 0.0f;
		}
	}

	void copyFrom(StyleSheetState *cf) {
		int i;

		for (i = 0; i < E_SS_PROPS_SIZE; i++) {
			props[i] = cf->props[i];
		}
	}


};

class StyleSheet {
private:

public:
	StyleSheetState compStates[E_COMP_TOTAL];

	StyleSheet() {

	}

	void copyFrom(StyleSheet* fromSS) {
		compStates[E_COMP_UP].copyFrom(&(fromSS->compStates[E_COMP_UP]));
		compStates[E_COMP_OVER].copyFrom(&(fromSS->compStates[E_COMP_OVER]));
		compStates[E_COMP_DOWN].copyFrom(&(fromSS->compStates[E_COMP_DOWN]));
	}

	void init() {
		int i;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			compStates[i].init();
		}

	}


};

class StyleSheetResult: public StyleSheetState {
private:

public:
	StyleSheet *baseSS;
	float incVal;
	float stateTotal;
	float stateAmounts[E_COMP_TOTAL];

	typedef StyleSheetState super;

	StyleSheetResult() {

	}

	void init(StyleSheet *_baseSS) {
		baseSS = _baseSS;
		super::init();

		int i;

		incVal = 0.1f;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			stateAmounts[i] = 0.0f;
		}

		stateAmounts[E_COMP_UP] = 1.0f;
	}


	void incrementVal(int enumVal) {
		int i;
		for (i = 0; i < E_COMP_TOTAL; i++) {
			stateAmounts[i] -= incVal;
		}

		stateAmounts[enumVal] += 2.0 * incVal;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			if (stateAmounts[i] < 0.0f) {
				stateAmounts[i] = 0.0f;
			}
			if (stateAmounts[i] > 1.0f) {
				stateAmounts[i] = 1.0f;
			}
		}

		float tot = 0.0f;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			tot += stateAmounts[i];
		}
		stateTotal = tot;

	}

	void update(bool mouseOver, bool mouseDown) {


		if (mouseOver) {
			if (mouseDown) {
				incrementVal(E_COMP_DOWN);
			}
			else {
				incrementVal(E_COMP_OVER);
			}
		}
		else {
			incrementVal(E_COMP_UP);
		}


		int i;
		int j;

		float bval;

		for (i = 0; i < E_SS_PROPS_SIZE; i++) {

			props[i] = 0.0f;
			for (j = 0; j < E_COMP_TOTAL; j++) {
				bval = baseSS->compStates[j].props[i];
				props[i] += bval * stateAmounts[j];
			}
			props[i] /= stateTotal;
		}

	}
};





////////////////////////////////////////////////////

//    DONT FORGET SEMICOLONS!

////////////////////////////////////////////////////
 
