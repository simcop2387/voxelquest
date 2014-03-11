
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

enum E_MOUSE_STATE {
    E_MOUSE_STATE_MOVE,
    E_MOUSE_STATE_OBJECTS,
    E_MOUSE_STATE_BRUSH,
    E_MOUSE_STATE_MEASURE,
    E_MOUSE_STATE_LENGTH
};

enum E_BRUSH {
    E_BRUSH_ADD,
    E_BRUSH_SUB,
    E_BRUSH_MOVE,
    E_BRUSH_LENGTH
};

enum E_HOLDER_ACTION {
    E_HOLDER_ACTION_RENDER,
    E_HOLDER_ACTION_RESET,
    E_HOLDER_ACTION_LENGTH
};

enum E_PLANE_TYPES {
    E_PLANE_XY,
    E_PLANE_YZ,
    E_PLANE_XZ,
    E_PLANE_LENGTH
};

enum E_GRASS_STATE {
    E_GRASS_STATE_OFF,
    E_GRASS_STATE_ON,
    E_GRASS_STATE_ANIM,
    E_GRASS_STATE_LENGTH
};

enum E_FILL_STATE {
    E_FILL_STATE_EMPTY,
    E_FILL_STATE_PARTIAL,
    E_FILL_STATE_FULL,
};

enum E_PLANT_TYPES {
    E_PT_OAK_TRUNK,
    E_PT_OAK_ROOTS,
    E_PT_LENGTH
};

enum E_OBJ {
    E_OBJ_CAMERA,
    E_OBJ_FOG,
    E_OBJ_CUTAWAY,
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

enum E_MAT_PARAM {
    //E_MAT_PARAM_TER,
    E_MAT_PARAM_ROAD,
    E_MAT_PARAM_BUILDING,
    E_MAT_PARAM_DOORWAY,
    E_MAT_PARAM_DOOR,
    E_MAT_PARAM_WINDOW,
    E_MAT_PARAM_SLATS,
    E_MAT_PARAM_TREE, // TREES MUST BE LAST ENTRY
    E_MAT_PARAM_LENGTH
};

enum E_BUILDING_TYPE {
    E_BT_NULL,
    E_BT_ROAD,
    E_BT_MAINHALL,
    E_BT_WING,
    E_BT_BALCONY,
    E_BT_TOWER,
    E_BT_DOORWAY,
    E_BT_WINDOWFRAME,
    E_BT_DOOR,
    E_BT_WINDOW,
    E_BT_TREE,
    E_BT_LENGTH
};


enum E_TREE_PARAMS {
    E_TP_VISMININPIXELST,
    E_TP_VISMAXINPIXELST,
    E_TP_P0,
    E_TP_P1,
    E_TP_P2,
    E_TP_POWERVALS,
    E_TP_POWERVALS2,
    E_TP_THICKVALS,
    E_TP_MATPARAMS,
    E_TP_LENGTH
};

enum E_GEOM_PARAMS {
    E_GP_VISMININPIXELST,
    E_GP_VISMAXINPIXELST,
    E_GP_BOUNDSMININPIXELST,
    E_GP_BOUNDSMAXINPIXELST,
    E_GP_CORNERDISINPIXELS,
    E_GP_POWERVALS,
    E_GP_POWERVALS2,
    E_GP_THICKVALS,
    E_GP_MATPARAMS,
    E_GP_LENGTH
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
    float baseLength;
    float nodeLengthMultiplier;
    float numGenerations;
    float angleUniformityU;
    float isInit;


};




struct BuildingNodeProp {
    int begHeight;
    int endHeight;
    int typeVal;
};

struct BuildingNode {
    
    BuildingNodeProp centerProp;
    BuildingNodeProp connectionProps[4];
    BuildingNodeProp shortProps[4*MAX_FLOORS];
    BuildingNodeProp dynProps[4*MAX_FLOORS];

    int id;
    float powerValU;
    float powerValV;
    float terHeight;

    bool isWingTip;

    // int centerType;
    // int connectionTypes[4];
    // int shortTypes[4];

    // int centerHeight;
    // int connectionHeights[4];
    // int shortHeights[4];

    
};



////////////////////////////////////////////////////

//    DONT FORGET SEMICOLONS!

////////////////////////////////////////////////////
 
