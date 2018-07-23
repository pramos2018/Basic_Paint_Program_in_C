#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>


typedef struct {
    float R;
    float G;
    float B;
} fRGB;

typedef struct {
    int R;
    int G;
    int B;
} iRGB;
typedef struct {
    float getX;
    float getY;
} fPoint;
typedef struct  {
    int getX;
    int getY;
} iPoint;
typedef struct  {
    int family;
    int style;
    int angle;
    int size;
} Font;

typedef struct {
    int flag;//1 - Set, 0 - Empty
    int shape;
    int x;
    int y;
    int w;
    int h;
    int fFill;
    float pen;
    fPoint xlp1;
    fRGB color;
    int idxText;//pointer
    Font xFont;
}shapeX;

//Prototyping
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
//-----------------------------------------------------
void drawHandle(HDC hDC);

void drawAll();
void drawTriangle();
void drawTriangle3d();
void drawTriangleDemo();
void drawSquare();
void drawCube();
void DrawCircle(float cx, float cy, float r, int num_segments);
void fillCircle(float cx, float cy, float r, int num_segments);

void drawLine(float x1, float y1, float x2, float y2);
void drawRectangle(float x1, float y1, float x2, float y2);
void drawText(float x, float y, void *font, char* string);
void fillRectangle(float x1, float y1, float x2, float y2);
void drawBitmap(float x, float y, void *font, char* string);
void loadFile();

//Absolute locations on the screen
float getX(int x);
float getY(int y);
float txtX (int row);
float txtY (int col);
float getPixH(float px);
float getPixV(float px);

fPoint p1, p2, lp1, lp2;
iPoint p3;
int flip = 0;
fRGB fColor;//Text
fRGB bColor;//Text

//Menus / DropDowns
void printMenus();
void drawRectMenus(int y, int x, int w, float cl, int ct);
void selectMenuItem();
//-------------Global variables---------
const int VIDEO_W = 640;//Video Width
const int VIDEO_H = 480;//Video Height
const int TXT_ROWS = 25;
const int TXT_COLS = 80;

int mouseV=4;//Velocity
int mouseX=320;
int mouseY=240;

int flagMouseDown = 0;
int flagFill = 0;
int selItem = 0;
int flagP1 = 0;
int flagPaint = 0;

float theta = 0.0f;
BOOL bQuit = FALSE;
int shape = 0;//Selected Shape
int shape_lmt = 17; // Shape Limit
float penSize = 1.0f, pen = 1.0f;

int ctrText = 0;
char listText[100][50];
fRGB g1 [600][400]; //600 x 400 //Bitmap

unsigned char g2 [600*360*3]; //600 x 400 //Bitmap
shapeX currShape;
//unsigned char g2 [590][360][3]; //600 x 400 //Bitmap


//--------- Screen Array ------------
int shapeCtr;
int shapeLMT = 1000;
shapeX screenArray[1001];
//------------------------------------
int exMenu = 0;

char xData[245*900];
int iHeader = 180; // Bitmap Header
int menuSelected[10] = {0,0,0,0,1,2,10,4,0,0};

int size, sz_w, sz_h; //Size Variables
int menuSel = 0;


int pos_x = 0;//x to the Print routines - Max 80
int pos_y = 0;//y to the Print routines - Max 25

char dText[50] = "This is a testing Text";
// ============ InputBox and MessageBox ==============
char title[80];
char msg[80*15];
int flagInputBox = 0;
int flagMsgBox = 0;
char input[80];
int iBox=0;
int inpBoxCTR = 0;
int flagCap = 0;
int flagColorPicker = 0;
int selectedColor = 0;
int flagFontPicker = 0;

int selectedFont[4] = {1, 1, 1, 6};
int selectFontCTR = 0;
int lf_font[4];
fRGB dColor;
Font dFont;

int flagShape = 0;
int selectedShape = 1;
int selectMode = 0;
int flagCut = 0, flagCopy = 0, flagPaste=0;
//====================================================
char currPath[50] = "c:\\temp\\bpaint\\";
char currFile[20] = "cpaint1.txt";
//====================================================



    //Goal to Create the Menus: File, Edit, Help and drop-downs: Shape, Size, PenSize;
    char *menuFile[]= {"File", "New File", "Open File", "Save File", "Save As", "Exit"};
    char *menuEdit[]= {"Edit", "Set Text", "Clear Screen", "Delete", "Cut", "Copy", "Paste", "Change Font", "Change Color"};
    char *menuHelp[]= {"Help", "Show Help", "About"};
    char *ddShapes[]= {"Shapes", "Drawing", "Rectangle (p1, p2)", "Circle (p1, p2)", "Square", "Rectangle (H)", "Rectangle (V)", "Circle", "Line (p1, p2)", "Text"};
    char *ddFill[]= {"Type", "Fill", "Stroke"};
    char *ddSizes[] = {"Sizes","10", "20", "30", "40", "50", "60", "70", "80", "90", "100", "120", "140", "160", "180", "200", "225", "250", "275", "300", "350", "400", "450", "500"};
    char *ddPenSizes[]= {"PenSizes", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "12", "14", "16", "18", "20", "25", "30"};

    char *fontFamily[] = {"Family","Font #1"};
    char *fontStyle[] = {"Style", "normal", "italic", "bold", "bold-italic"};
    char *fontInclination[] = {"Angle", "  0"," 45", " 90", "135", "180", "225", "270", "315"};
    char *fontSize[] = {"Size","5", "6", "7", "8", "9", "10", "12", "14", "16", "18", "20", "24", "28", "32", "38", "44", "48", "54", "60", "66", "72", "80", "90","100"};


char charCode[128][8] =  {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},
{0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},
{0x18, 0x7E, 0x03, 0x3E, 0x60, 0x3F, 0x18, 0x00},
{0x00, 0x63, 0x30, 0x18, 0x0C, 0x06, 0x63, 0x00},
{0x1C, 0x36, 0x1C, 0x6E, 0x33, 0x33, 0x6E, 0x00},
{0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},
{0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},
{0x00, 0x77, 0x3E, 0x7F, 0x3E, 0x77, 0x00, 0x00},
{0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x00},
{0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00},
{0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},
{0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},
{0x18, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00},
{0x3E, 0x63, 0x30, 0x18, 0x0C, 0x66, 0x7F, 0x00},
{0x3E, 0x63, 0x60, 0x3C, 0x60, 0x63, 0x3E, 0x00},
{0x30, 0x38, 0x3C, 0x36, 0x7F, 0x30, 0x30, 0x00},
{0x7F, 0x03, 0x3F, 0x60, 0x60, 0x63, 0x3E, 0x00},
{0x3E, 0x63, 0x03, 0x3F, 0x63, 0x63, 0x3E, 0x00},
{0x7F, 0x63, 0x60, 0x30, 0x18, 0x18, 0x18, 0x00},
{0x3E, 0x63, 0x63, 0x3E, 0x63, 0x63, 0x3E, 0x00},
{0x3E, 0x63, 0x63, 0x7E, 0x60, 0x63, 0x3E, 0x00},
{0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00},
{0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0C},
{0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00},
{0x00, 0x00, 0x7F, 0x00, 0x00, 0x7F, 0x00, 0x00},
{0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},
{0x3E, 0x63, 0x60, 0x30, 0x18, 0x00, 0x18, 0x00},
{0x3E, 0x63, 0x63, 0x7B, 0x3B, 0x03, 0x7E, 0x00},
{0x1C, 0x36, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x00},
{0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},
{0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},
{0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},
{0x7F, 0x43, 0x03, 0x1F, 0x03, 0x43, 0x7F, 0x00},
{0x7F, 0x46, 0x06, 0x3E, 0x06, 0x06, 0x0F, 0x00},
{0x3E, 0x63, 0x03, 0x03, 0x7B, 0x63, 0x3E, 0x00},
{0x63, 0x63, 0x63, 0x7F, 0x63, 0x63, 0x63, 0x00},
{0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00},
{0x3C, 0x18, 0x18, 0x18, 0x1B, 0x1B, 0x0E, 0x00},
{0x63, 0x33, 0x1B, 0x0F, 0x1B, 0x33, 0x63, 0x00},
{0x0F, 0x06, 0x06, 0x06, 0x06, 0x46, 0x7F, 0x00},
{0x63, 0x77, 0x7F, 0x6B, 0x6B, 0x63, 0x63, 0x00},
{0x63, 0x67, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x00},
{0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00},
{0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},
{0x3E, 0x63, 0x63, 0x63, 0x63, 0x6B, 0x3E, 0x60},
{0x3F, 0x63, 0x63, 0x3F, 0x1B, 0x33, 0x63, 0x00},
{0x3E, 0x63, 0x03, 0x3E, 0x60, 0x63, 0x3E, 0x00},
{0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00},
{0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00},
{0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x08, 0x00},
{0x63, 0x63, 0x6B, 0x6B, 0x7F, 0x77, 0x63, 0x00},
{0x63, 0x36, 0x1C, 0x1C, 0x1C, 0x36, 0x63, 0x00},
{0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x00},
{0x7F, 0x61, 0x30, 0x18, 0x0C, 0x46, 0x7F, 0x00},
{0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x00},
{0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},
{0x3E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3E, 0x00},
{0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
{0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00},
{0x07, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3F, 0x00},
{0x00, 0x00, 0x3E, 0x63, 0x03, 0x63, 0x3E, 0x00},
{0x38, 0x30, 0x3E, 0x33, 0x33, 0x33, 0x7E, 0x00},
{0x00, 0x00, 0x3E, 0x63, 0x7F, 0x03, 0x3E, 0x00},
{0x38, 0x6C, 0x0C, 0x3F, 0x0C, 0x0C, 0x1E, 0x00},
{0x00, 0x00, 0x6E, 0x73, 0x63, 0x7E, 0x60, 0x3E},
{0x07, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x67, 0x00},
{0x18, 0x00, 0x1C, 0x18, 0x18, 0x18, 0x3C, 0x00},
{0x30, 0x00, 0x38, 0x30, 0x30, 0x30, 0x33, 0x1E},
{0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},
{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x00},
{0x00, 0x00, 0x36, 0x7F, 0x6B, 0x6B, 0x63, 0x00},
{0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x66, 0x00},
{0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x3E, 0x00},
{0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},
{0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},
{0x00, 0x00, 0x3B, 0x66, 0x06, 0x06, 0x0F, 0x00},
{0x00, 0x00, 0x3E, 0x03, 0x3E, 0x60, 0x3E, 0x00},
{0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x6C, 0x38, 0x00},
{0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},
{0x00, 0x00, 0x63, 0x63, 0x36, 0x1C, 0x08, 0x00},
{0x00, 0x00, 0x63, 0x63, 0x6B, 0x7F, 0x36, 0x00},
{0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},
{0x00, 0x00, 0x63, 0x63, 0x73, 0x6E, 0x60, 0x3E},
{0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},
{0x70, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x70, 0x00},
{0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},
{0x0E, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0E, 0x00},
{0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

//--------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;



    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "Basic Paint Program [OpenGL]",
                          WS_OVERLAPPEDWINDOW,//WS_POPUPWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          VIDEO_W,
                          VIDEO_H,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    start();
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            drawHandle(hDC);

        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            if (wParam==VK_ESCAPE) {bQuit = GL_TRUE;return;}
            if (flagShape==1) {changeShapeKeys(wParam);return;}
            if (flagInputBox==1) {getInput(wParam);return;}
            if (flagColorPicker==1) {changeColorKeys(wParam);return;}
            if (flagFontPicker==1) {changeFontKeys(wParam);return;}
            if (flagMsgBox==1) {msgBoxKeys(wParam);return;}


            switch (wParam)
            {
                case VK_ESCAPE:
                    //PostQuitMessage(0);
                    bQuit = GL_TRUE;//New Code
                break;
                case VK_DOWN:
                    if (shape==15) {
                        menuSel ++;
                        //printf ("\nSelect Menu: %i", menuSel);
                    }
                    if (shape==16) {//mousePointer
                            mouseY+=mouseV;
                            mousePointer();
                    }

                break;

                case VK_UP:
                    if (shape==15) {
                        menuSel --;
                    }
                    if (shape==16) {//mousePointer
                            mouseY-=mouseV;
                            mousePointer();
                    }
                break;
                case VK_RIGHT:
                    if (shape==16) {//mousePointer
                            mouseX+=mouseV;
                            mousePointer();
                    }
                    if (shape==15) {
                        exMenu++;
                        menuSel = 0;
                    }

                break;
                case VK_LEFT:
                    if (shape==16) {//mousePointer
                            mouseX-=mouseV;
                            mousePointer();
                    }
                    if (shape==15) {
                        exMenu--;
                        menuSel = 0;
                    }

                break;


                case VK_ADD:
                    shape = shape-1;
                    if (shape<0) shape = shape_lmt-1;

                break;

                case VK_SUBTRACT:
                    shape = shape+1;
                    if (shape>=shape_lmt) shape =0;
                break;
                case VK_SPACE:
                    if (shape==15) {//MenuSelection
                        selectMenuItem();
                    }
                    if (shape==16) {//MouseMove
                        mouseClick();
                    }
                    break;
                case 13:
                    if (shape==15) {//MenuSelection
                        selectMenuItem();
                    }
                    break;

                case VK_F1://Test
                    exMenu ++;//Expand Menu Test
                    shape = 15;
                    int i = 0;
                    menuSel = 0;
                    flagP1 = 1;
                break;
                case VK_F2:
                    shape = 16;
                    exMenu = 0;
                    //mouseX = 320; mouseY = 240;
                    flagP1 = 1;
                    break;
                case VK_F3:
                    clearScreen();
                    break;
                case VK_F4:
                    CloseBox();
                    break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);

    glEnable(GL_DEPTH_TEST);//Tutorial 5 - Z-Buffer
    glDepthMask(GL_TRUE);//Tutorial 5 - Z-Buffer

}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

//========================== NEW CODE =========================================
void drawHandle(HDC hDC) {
            /* OpenGL animation code goes here */

            //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            //glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Tutorial 5

            glPushMatrix();

            //glRotatef(theta, 0.0f, 0.0f, 1.0f);
            //Cube
            if (shape== 4) glRotatef(theta, 0.0f, 1.0f, 1.0f);//Tutorial 6 - A cube

            drawAll();//Draw all the shapes

            glPopMatrix();

            SwapBuffers(hDC);

            //theta += 1.0f;
            //Cube
            if (shape== 4) theta += 0.5f;//Rotation velocity
            //Sleep (1);//Pause


}
//---------------------------------------------------------------------------------
void drawAll() {
            //Centrallized Variables

            loop();

            glColor3f(0.0f,0.0f, 1.0f);//Color to Draw;
            glLineWidth(0.5f);
            //-----------------------------------------

            if(shape==0) drawTriangle() ;
            if(shape==1) drawTriangleDemo();
            if(shape==2) drawTriangle3d();
            if(shape==3) drawSquare();
            if(shape==4) drawCube();

            if(shape==5) drawLine(0.0,0.0, 1.0, 1.0);
            if(shape==6) drawRectangle(0.25,0.25, 0.75, 0.75);
            if(shape==7) fillRectangle(0.25,0.25, 0.75, 0.75);
            if(shape==8) fillCircle(0.5, 0.5, 0.25, 36);
            if(shape==9) DrawCircle(0.5, 0.5, 0.25, 36);


            if(shape==10) fillRectangle(-0.5,0.0, 0.0, 0.5);
            if(shape==11) drawRectangle(-0.5,0.0, 0.0, 0.5);

            if(shape==13) drawLine(0.0f, 0.0f, 0.5, 0.5);

            if(shape==14) drawBitmapX(0, 0, "Test", "Test");

}

//============================ STANDARD SHAPES ==============================

void drawTriangleDemo() {
            glBegin(GL_TRIANGLES);
                glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f,   1.0f);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f,  -0.5f);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);
            glEnd();

}
void drawTriangle() {
            glBegin(GL_TRIANGLES);
                glVertex2f(-0.75f, 0.0f); //glVertex2f(x, y); Proportional to the screen;
                glVertex2f(0.75f, 0.0f);
                glVertex2f(0.0f, 0.75f);
            glEnd();
}
void drawTriangle3d() {
            glBegin(GL_TRIANGLES);
                glVertex3f(-0.75f, 0.0f, -1.0f);
                glVertex3f(0.75f, 0.0f, -1.0f);
                glVertex3f(0.0f, 0.75f, -1.0f);
            glEnd();
}

void drawSquare() {
            glBegin(GL_QUADS);
                glVertex3f(0.75f, 0.75f, 1.0f);
                glVertex3f(0.75f, -0.75f, 1.0f);
                glVertex3f(-0.75f, -0.75f, 1.0f);
                glVertex3f(-0.75f, 0.75f, 1.0f);
            glEnd();
            //glColor3f(0.0f, 0.0f, 0.0f);//reset the color
}

void drawCube() {
            glBegin(GL_QUADS);
                glColor3f(1.0f, 0.0f, 0.0f);//set the color

                glVertex3f(0.5f, 0.5f, -0.5f);
                glVertex3f(0.5f, -0.5f, -0.5f);
                glVertex3f(-0.5f, -0.5f, -0.5f);
                glVertex3f(-0.5f, 0.5f, -0.5f);

                glColor3f(0.0f, 1.0f, 0.0f);//set the color

                glVertex3f(0.5f, 0.5f, 0.5f);
                glVertex3f(0.5f, 0.5f, -0.5f);
                glVertex3f(-0.5f, 0.5f, -0.5f);
                glVertex3f(-0.5f, 0.5f, 0.5f);

                glColor3f(0.0f, 0.0f, 0.5f);//set the color

                glVertex3f(0.5f, -0.5f, 0.5f);
                glVertex3f(0.5f, -0.5f, -0.5f);
                glVertex3f(-0.5f, -0.5f, -0.5f);
                glVertex3f(-0.5f, -0.5f, 0.5f);

                glColor3f(1.0f, 1.0f, 0.0f);//set the color

                glVertex3f(0.5f, -0.5f, 0.5f);
                glVertex3f(0.5f, -0.5f, -0.5f);
                glVertex3f(-0.5f, -0.5f, -0.5f);
                glVertex3f(-0.5f, 0.5f, 0.5f);

                glColor3f(1.0f, 1.0f, 0.5f);//set the color

                glVertex3f(0.5f, 0.5f, 0.5f);
                glVertex3f(0.5f, -0.5f, 0.5f);
                glVertex3f(0.5f, -0.5f, -0.5f);
                glVertex3f(0.5f, 0.5f, -0.5f);

                glColor3f(0.0f, 1.0f, 1.0f);//set the color

                glVertex3f(-0.5f, 0.5f, 0.5f);
                glVertex3f(-0.5f, -0.5f, 0.5f);
                glVertex3f(-0.5f, -0.5f, -0.5f);
                glVertex3f(-0.5f, 0.5f, -0.5f);


            glEnd();

}

void DrawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    int ii = 0;
    float cyx = getPixV(1.0f)/getPixH(1.0f);


    for(ii = 0; ii < num_segments; ii++)
    {
        float th = 2.0f * 3.1415926f * (float) ii / (float) num_segments;//get the current angle

        float x = r * cosf(th);//calculate the x component
        float y = (r * sinf(th))*cyx;//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();
}
void fillCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_POLYGON);
    int ii = 0;
    float cyx = getPixV(1.0f)/getPixH(1.0f);


    for(ii = 0; ii < num_segments; ii++)
    {
        float th = 2.0f * 3.1415926f * (float) ii / (float) num_segments;//get the current angle

        float x = r * cosf(th);//calculate the x component
        float y = (r * sinf(th))*cyx;//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();
}
void fillCircleBk(float x, float y, float radius, int num_segments)
{
    int i;
    int triangleAmount = 360;
    float twicePi = 2.0f * 3.141592653;

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0);

    glBegin(GL_LINES);
    for(i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( x, y);
        glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}


void drawLine(float x1, float y1, float x2, float y2)
{

    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);//output vertex
        glVertex2f(x2, y2);//output vertex
    glEnd();
}
void drawRectangle(float x1, float y1, float x2, float y2)
{
    drawLine(x1, y1, x1, y2);
    drawLine(x1, y2, x2, y2);

    drawLine(x2, y2, x2, y1);
    drawLine(x2, y1, x1, y1);
}
void fillRectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x1, y2);
        glVertex2f(x2, y2);
        glVertex2f(x2, y1);
    glEnd();
}


//===========================================================================
//*********************** DRAW BITMAP FILES ****************************
void drawBitmap(float x, float y, void *font, char* string) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)

        int i, j, c, r;
        float cx = 0.00f;
        float fct = (float) 1/200;
        float rx = 0.0f;

        for (c = 0; c<=200; c++) {
            cx = cx+fct;
            rx = 0.0f;
            for (r=0; r<=200;r++) {
                    rx = rx + fct;
                    fillRectangle(rx, cx, rx+fct, cx+fct);//Scalable dot
            }
        }

}
void drawBitmapFonts() {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        int i, j, c, r;
        float cx = 0.00f;
        int scl = 900;
        float fct = (float) 2/scl;
        float rx = -1.0f;
        int ctx = 0;
        char ch;
        float px = 0;

        glLineWidth(0.5f);
        for (c = 0; c<245; c++) {
            cx = cx+fct;
            rx = -1.0f;
            for (r=0; r<scl;r++) {
                    rx = rx + fct;
                    ctx = (c*900)+r;
                    ch = xData[ctx];

                    px = (float) 255/ch;
                    px = 1.0f - px;


                    glColor3f(px, px, px);

                    fillRectangle(rx, cx, rx+fct, cx+fct);//Scalable dot
            }
        }

}


//****************** Draw Text / Print Text *******************************
const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "X" : " ");
    }

    return b;
}

void drawCharCode() {
    int i = 0;
    int ch = 0;
    for (i=0; i<7; i++) {
        ch = (int) charCode[65+3][i];
        printf("\n%s", byte_to_binary(ch));
    }
}

void drawDot(float x, float y) {
    glBegin(GL_POINTS);
        //glVertex3f(x, y, -0.25f);
        glVertex2f(x, y);
    glEnd( );
}
void drawChar(char str, float x, float y, float fct_x, float fct_y) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
       //----------- Printing the Character on the Screen ----------
        float rx, cx;
        int r, c;
        glLineWidth(0.5f);

        int ch = 0; int ctx = (int) str;
        char bt[9];
        char ch1;

        rx = y;
        for (r = 0; r<8; r++) {
            cx = x;
            ch = (int) charCode[ctx][7-r];
            strcpy(bt, byte_to_binary(ch));
            for (c=0; c<8;c++) {
                    if (bt[7-c]==' ' ) {
                        //glColor3f(1.0f, 1.0f, 1.f);//BackGround
                        setColorRGB(bColor);
                    } else {
                        glColor3f(0.0f, 0.0f, 0.f);//ForeColor
                        setColorRGB(fColor);
                    }
                    cx = cx + fct_x;
                    fillRectangle(cx, rx, cx+fct_x, rx+fct_y);//Scalable dot

            }
            rx = rx+fct_y;
        }
        //---------------------------------------------------------

}
void drawCharF(char str, float x, float y, float fct_x, float fct_y, Font f, fRGB xColor) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
       //----------- Printing the Character on the Screen ----------
        float rx, cx;
        int r, c;
        glLineWidth(0.5f);

        int ch = 0; int ctx = (int) str;
        char bt[9];
        char ch1;
        float dsl_x = 0.00f;

        rx = y;
        for (r = 0; r<8; r++) {
            cx = x+dsl_x;
            ch = (int) charCode[ctx][7-r];
            strcpy(bt, byte_to_binary(ch));
            for (c=0; c<8;c++) {
                    cx = cx + fct_x;
                    if (bt[7-c]==' ' ) {
                    } else {
                        glColor3f(0.0f, 0.0f, 0.f);//ForeColor
                        setColorRGB(xColor);
                        fillRectangle(cx, rx, cx+fct_x, rx+fct_y);//Scalable dot
                        //fontStyle[] = {"Style", "normal", "italic", "bold", "bold-italic"};
                        if (f.style==3 || f.style == 4) {//Bold
                            fillRectangle(cx, rx, cx+fct_x*1.25, rx+fct_y*1.25);//Scalable dot
                        }
                    }

            }
            rx = rx+fct_y;
                        if (f.style==2 || f.style == 4) {dsl_x= dsl_x+fct_x*1.25f;}//Italic


        }
        //---------------------------------------------------------

}

void drawStringF(char * text, float x, float y, Font f, fRGB xColor) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)

        //fontInclination[] = {"Angle", "  0"," 45", " 90", "135", "180", "225", "270", "315"};

        float fct_x = (float) (2.0f/VIDEO_W * f.size/12.0f);
        float fct_y = (float) (2.0f/VIDEO_H * f.size/12.0f);

        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            ch = text[i];
            drawCharF(ch, x, y, fct_x, fct_y*2, f, xColor);
            if (f.angle==1) {x = x + 8*fct_x;}//0dg
            if (f.angle==5) {x = x - 8*fct_x;}//180dg

            //Font Inclination

            if (f.angle==2) {x = x + 8*fct_x; y = y + 8*fct_y*1;}//45dg
            if (f.angle==8) {x = x + 8*fct_x; y = y - 8*fct_y*1;}//315dg

            if (f.angle==3) {y = y + 8*fct_y*2;}//90dg
            if (f.angle==7) {y = y - 8*fct_y*2;}//270dg

            if (f.angle==4) {x = x - 8*fct_x; y = y + 8*fct_y*1;}//135dg
            if (f.angle==6) {x = x - 8*fct_x;y = y - 8*fct_y*1;}//270dg

        }

}


void drawTextX(char * text, float x, float y) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        float scl_w = (float) 2/VIDEO_W;
        float scl_h = (float) 2/VIDEO_H;

        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            ch = text[i];
            drawChar(ch, x, y, scl_w, scl_h);
            x = x + 8*scl_w;
        }
}
void drawString(char * text, float x, float y, int sz_x) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        float fct = (float) (2.0f/VIDEO_H * sz_x/100.0f);
        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            ch = text[i];
            drawChar(ch, x, y, fct, fct*2);
            x = x + 8*fct;
        }
}

void drawBitmapX(float x, float y, void *font, char* string) {
    int l = 1;
    float lx = -1.0f;
    for (l = 0; l<25; l++) {
        drawTextX("Testing 1234 - ABCDEFGHIJKLMNOPQRSTUVXWYZ 123456 abcdefghijklmnoprstuvxz01234567890", -1.0f, lx);
        lx += (float) 2/25;
    }

}
void printRow(char * text, int fixCol, int maxCol) {
    //Will be used to Draw Bitmaps and Text (BMP Characters)
        float scl_x = (float) (2.0f/VIDEO_W);
        float scl_y = (float) (2.0f/VIDEO_H);

        float x, y;

        int i = 0;  char ch = 0;
        for (i = 0; i <strlen(text); i++ )
        {
            x = -1.0f + ((float)pos_x/TXT_COLS*2);
            y = 1.0f  -  ((float)pos_y/TXT_ROWS*2);
            ch = text[i];
            if (ch==0) {break;}

            if (ch==10 || ch==13) {
                pos_x = fixCol-1; pos_y++;
            } else {
                drawChar(ch, x, y, scl_x, scl_y*1.5f);
            }
            //----- Incrementing the Text Position on the Screen -----
            pos_x++;if (pos_x>=maxCol) {pos_x = fixCol;pos_y++;}

            if (pos_y>=25) {pos_y = 0;}
            //----------------------------------------------------------
        }

}

void printText(char *string) {
    //Prints Text
    printRow(string, 0, 80);
}
void printTextNL(char *string) {
    //Prints Text and adds a NewLine at the end
    printRow(string, 0, 80);
    //New Line
    pos_y++; pos_x = 0;
    if (pos_y>=25) {pos_y = 0;}

}

void printTextRC(int row, int col, char *string) {
    pos_x = col; pos_y = row;
    printRow(string, col, 80);
}
void printTextRCmax(int row, int col, int maxCol, char *string) {
    pos_x = col; pos_y = row;
    printRow(string, col, maxCol);
}

//****************** Load BitMap File *******************************
void loadFile() {
    //system("cls");
    //char cFile[]= "C:\\TEMP\\FONT3.BMP" ; //108 x 1386
    //char *cName = "C:\\TEMP\\FONT2.BMP";//55 x 695
    //char cFile[]= "C:\\TEMP\\FONT1.BMP" ; //55 x 695
    char cFile[]= "C:\\TEMP\\FONT4.BMP" ; //245 x 900
    drawCharCode();

    int h = 245;
    int w = 900;

    int i, ii, j, jj;
    char ch;

    FILE *file2 = fopen(cFile, "r"); //Open File


    for (i=0; i<iHeader;i++) {//Read the Header
        fscanf(file2, "%c", &ch);
    }

    int ctx = 0;
    for (i=0;i<(h*w);i++) {
            fscanf(file2, "%c", &ch);
            xData[ctx] = ch; ctx++;
    }

    fclose(file2);//Close File

    shape = 14; //ShowBitmap
}
//****************************************************************

//========================== PAINT PROJECT ==================================
//===========================================================================
void printMenus() {
    //ExpandMenu
    if (exMenu>=8) {exMenu=0;}
    if (exMenu<0) {exMenu=7;}
    int lf = 0, i = 0, ix = 0, iy = 0;

    //PrintMenus
    printTextRC(1,3, "File");
    printTextRC(1,12, "Edit");
    printTextRC(1,22, "Help");
    printTextRC(1,32, "Shapes");
    printTextRC(1,42, "Type");
    printTextRC(1,52, "Sizes");
    printTextRC(1,62, "PenSizes");

    //Expand Menus
    switch (exMenu) {
        case 1:
             lf = sizeof(menuFile)/sizeof(menuFile[0]);
             ix = 3;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,10,0.82f, i);printTextNL(menuFile[i]);pos_x = ix;}
            break;
        case 2:
             lf = sizeof(menuEdit)/sizeof(menuEdit[0]);
             ix = 12;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,12,0.82f, i);printTextNL(menuEdit[i]);pos_x = ix;}
            break;
        case 3:
             lf = sizeof(menuHelp)/sizeof(menuHelp[0]);
             ix = 22;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,10,0.82f, i);printTextNL(menuHelp[i]);pos_x = ix;}
            break;

        case 4:
             lf = sizeof(ddShapes)/sizeof(ddShapes[0]);
             ix = 32;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,18,0.82f, i);printTextNL(ddShapes[i]);pos_x = ix;}
            break;
        case 5:
             lf = sizeof(ddFill)/sizeof(ddFill[0]);
             ix = 42;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,10,0.82f, i);printTextNL(ddFill[i]);pos_x = ix;}
            break;
        case 6:
             lf = sizeof(ddSizes)/sizeof(ddSizes[0]);
             ix = 52;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,5,0.82f,i);printTextNL(ddSizes[i]);pos_x = ix;}
            break;
        case 7:
             lf = sizeof(ddPenSizes)/sizeof(ddPenSizes[0]);
             ix = 62;pos_y=1;pos_x= ix; for (i=0; i<lf; i++) {drawRectMenus(pos_y,pos_x,5,0.82f, i);printTextNL(ddPenSizes[i]);pos_x = ix;}
            break;
        default:
            exMenu = 0;
            break;
    }
    glColor3f(0.0f, 0.0f, 0.0f); //black;
    glLineWidth(2.0f);
    drawRectangle(getX(20), getY(30), getX(620), getY(420));

}

void drawRectMenus(int y, int x, int w, float cl, int ct) {
    if (ct ==0) return;

    if (menuSel==ct) {
        printTextRC(y, x, ">>");//Indicator
        //x = x+2;
        w = w +2;
    }

    float x1 = txtX(x);
    float x2 = txtX(x+w);

    float y1 = txtY(y-1)-getPixH(2.0);
    float y2 = txtY(y) -getPixH(2.0);

    glColor3f(cl,cl, cl);//Color to Draw;
    glLineWidth(0.5f);
    //fillRectangle(x1, y1, x2, y2);
    cl = 0.0f;
    glColor3f(cl,cl, cl);//Color to Draw;
    drawRectangle(x1,y1, x2, y2);//Border
}
void selectMenuItem() {
    menuSelected[exMenu] = menuSel;
    exMenu = 0;//Exit Menu
    setShape();

    //char *menuFile[]= {"File", "New File", "Open File", "Save File", "Save As", "Exit"};
    //char *menuEdit[]= {"Edit", "Set Text", "Clear Screen", "Cut", "Copy", "Paste", "Change Font", "Change Color"};
    //char *menuHelp[]= {"Help", "Show Help", "About"};

    //-------------- Menu File -----------------------
    if (menuSelected[1]==1) {clearScreen();setShape();} //New File;
    if (menuSelected[1]==2) {openFileDialog();} //Open File;
    if (menuSelected[1]==3) {saveFileDialog();} //Save File;
    if (menuSelected[1]==4) {saveFileAsDialog();} //Save As;
    if (menuSelected[1]==5) {bQuit = TRUE;} //Exit;
    menuSelected[1] = 0;//Resets

    //-------------- Menu Edit -----------------------
    if (menuSelected[2]==1) {setText();} //Set Text
    if (menuSelected[2]==2) {clearScreen();} //Clear Screen
    if (menuSelected[2]==3) {delete_shape();} //Delete
    if (menuSelected[2]==4) {cut_shape();} //Cut
    if (menuSelected[2]==5) {copy_shape();} //Copy
    if (menuSelected[2]==6) {paste_shape();} //Paste
    if (menuSelected[2]==7) {flagFontPicker=1; changeFont();} //Change Font
    if (menuSelected[2]==8) {flagColorPicker=1; changeColor();} //Change Color

    menuSelected[2] = 0;//Resets
    //-------------- Menu Help -----------------------
    if (menuSelected[3]==1) {help();} //Show Help
    if (menuSelected[3]==2) {about();} //About
    menuSelected[3] = 0;//Resets
}

//=============== Essential Translations  and Variables =====================

float getX(int x) {
    float r = -1.0f + (((float) x/ VIDEO_W)*2.0f);
    return r;
}
float getY(int y) {
    float r = 1.0f - (((float) y/ VIDEO_H)*2.0f);
    return r;
}

float txtX (int row) {
    float r = -1.0f + ((float)row/TXT_COLS*2);
    return r;
}
float txtY (int col) {
    float r = 1.0f  -  ((float)col/TXT_ROWS*2);
    return r;
}
float getPixH(float px) {
    float r = (px/VIDEO_W)*2.0f;
    return r;
}
float getPixV(float px) {
    float r = (px/ VIDEO_H)*2.0f;
    return r;
}

fRGB setfRGB(float r, float g, float b) {
    fRGB t;
    t.R = r;
    t.G = g;
    t.B = b;
    return t;
}
void setColorRGB(fRGB color) {
    glColor3f(color.R, color.G, color.B);
}
fPoint setfPoint(float x, float y) {
    fPoint t;
    t.getX = x;
    t.getY = y;
    return t;
}
//===========================================================================
void mousePointer() {
    if (mouseX <=0) mouseX = 0;
    if (mouseX >=VIDEO_W) mouseX = VIDEO_W;
    if (mouseY <=0) mouseY = 0;
    if (mouseY >=VIDEO_H) mouseY = VIDEO_H;

    float x0 = getX(mouseX);
    float y0 = getY(mouseY);
    float dh = getPixH(6.0f);
    float dv = getPixV(4.0f);

    float x1 = x0+dh;
    float y1 = y0+dv;
    float x2 = x0-dh;
    float y2 = y0-dv;

    glColor3f(0.0f, 0.0f, 1.0f); //blue;
    drawLine(x0, y0+dh, x0, y0-dh);
    drawLine(x0+dv, y0, x0-dv, y0);
    drawRectangle(x1, y1, x2, y2);
}
void updateStatus() {
    printTextRC(23,3, "Shape: ");
    printText(ddShapes[menuSelected[4]]);

    printText(", Type: ");
    printText(ddFill[menuSelected[5]]);

    printText(", Size: ");
    printText(ddSizes[menuSelected[6]]);

    printText(", Pen: ");
    printText(ddPenSizes[menuSelected[7]]);

    char str1[15];
    sprintf(str1, "%i", mouseX);
    char str2[15];
    sprintf(str2, "%i", mouseY);

    printText (", P(");
    printText (str1);
    printText (",");
    printText (str2);
    printText (")");

    printText(" Sel[");
    char str3[15];
    sprintf(str3, "%i", shape);
    printText (str3);
    printText ("]");


    printTextRC(24,3, "Press: F1 - Menu, Down + Space To Select, F2 - Mouse, +/- Other Screens");
}


//===========================================================================
void addShapeX(int shape, int x, int y, int w, int h, fRGB xColor, float xPen, int xfFill, fPoint xlp1, char xText, Font xFont) {

    shapeX t;
    t.flag = 1;
    t.shape = shape;
    t.x = x;
    t.y = y;
    t.w = w;
    t.h = h;
    t.color = xColor;
    t.pen = xPen;
    t.fFill = xfFill;
    t.xlp1 = xlp1;
    t.xFont = xFont;

    if (shape==9) {//Text
        t.idxText = ctrText;
        strcpy(listText[ctrText], dText);
        ctrText++;
        if (ctrText>=100) ctrText=0;
    }

    screenArray[shapeCtr] = t;
    shapeCtr++;

    if (shapeCtr>=shapeLMT) shapeCtr = 0;
    //printf("\nSeq %i - Shape: %i, flagFill = %i", shapeCtr, t.shape, t.fFill);
    //printf("\nColor: IN (%f, %f, %f)  OUT (%f, %f, %f)", xColor.R,xColor.G,xColor.B, t.color.R, t.color.G, t.color.B);
    //printf ("\n %i %i %i %i %i %f %f %f %f %i %f %f", shape, x, y, w, h, xColor.R, xColor.G, xColor.B, xPen, xfFill, xlp1.getX, xlp1.getY);
    //printf ("\n %i %i %i %i %i %f %f %f %f %i %f %f", t.shape, t.x, t.y, t.w, t.h, t.color.R, t.color.G, t.color.B, t.pen, t.fFill, t.xlp1.getX, t.xlp1.getY);
}
void drawShapeX(shapeX sx) {
                if (sx.flag==0) return;
                //Basic Setup
                setColorRGB(sx.color);
                glLineWidth(sx.pen);
                float fx1 = getX(sx.x), fy1 = getY(sx.y), rd;

                int sp = sx.shape;
                fPoint xlp1 = sx.xlp1;
                if (sp>=4 && sp<=7) {xlp1.getX = getX(sx.x+sx.w); xlp1.getY = getY(sx.y+sx.h);}
                if (sp==8) {rd = abs(fx1- xlp1.getX)/2.0f;}
                if (sp==7) {rd = getPixH(sx.w)/2.0f;}

                if (sp==8) {drawLine(xlp1.getX, xlp1.getY, fx1, fy1); return;}//Line
                if ((sx.fFill==1) && (sp==3 || sp==7)) {fillCircle(fx1, fy1, rd, 360);return;}//Circle
                if ((sx.fFill==0) && (sp==3 || sp==7)) {DrawCircle(fx1, fy1, rd, 360);return;}//Circle
                if (sp==9) {drawStringF(listText[sx.idxText], fx1, fy1, sx.xFont, sx.color);return;}//Text
                if (sx.fFill==0) {drawRectangle(xlp1.getX, xlp1.getY, fx1, fy1);return;}//Rectangle
                if (sx.fFill==1) {fillRectangle(xlp1.getX, xlp1.getY, fx1, fy1);return;}//Rectangle

}
void drawScreen() {
    if (shapeCtr==0) return;
    //Backup
    fPoint l1 =lp1;
    int i = 0;
    shapeX sx;
    for (i=0;i<shapeCtr;i++) {
       sx = screenArray[i];
       if (sx.flag ==1) {
            drawShapeX(sx);
       }

    }
    //Recover
    lp1 = l1;
}

shapeX clearShapeX() {
    shapeX t;
    t.flag = 0;
    t.shape = 0;
    t.x = 0;
    t.y = 0;
    t.w = 0;
    t.h = 0;
    t.color = setfRGB(0.0f, 0.0f, 0.0f);
    t.pen = 0.0f;
    t.fFill = 0;
    t.xlp1 = setfPoint(0.0, 0.0);
    return t;
}

void clearScreen() {
    int i = 0;
    for (i=0;i<shapeLMT;i++) {
            screenArray[i] = clearShapeX();
    }
    shapeCtr = 0;
    ctrText = 0;
    clearCanvas();//Drawing
}

//==================== DRAWING ROUTINES =====================================
void drawShape(int opt, int x, int y, int w, int h, float xPen, fRGB xColor, int fgFill, int fgP1, char *xText, Font xFont) {
    setColorRGB(xColor);
    glLineWidth(xPen);
    //glPointSize(xPen);

    float fx1, fy1, fx2, fy2;
    fx1 = getX(x); fy1 = getY(y);
    fx2 = getX(x+w); fy2 = getY(y+h);


    switch (opt) {
        case 1://Drawing
            break;
        case 2://Rectangle(p1, p2)
            if (fgP1==0) {
                lp2 = setfPoint(getX(x), getY(y));
                if (fgFill==1) {
                    fillRectangle(lp1.getX, lp1.getY, lp2.getX, lp2.getY);
                } else {
                    drawRectangle(lp1.getX, lp1.getY, lp2.getX, lp2.getY);
                }
            }
            break;
        case 3://Circle (p1, p2)
            if (fgP1==0) {
                lp2 = setfPoint(getX(x), getY(y));
                float rd = abs((lp2.getX - lp1.getX))/2.0f;
                if (fgFill==1) {
                    fillCircle(lp1.getX, lp1.getY, rd, 360);
                } else {
                    DrawCircle(lp1.getX, lp1.getY, rd, 360);
                }
            }

            break;
        case 4://Square
            if (fgFill==1) {
                fillRectangle(fx1, fy1, fx2, fy2);
            } else {
                drawRectangle(fx1, fy1, fx2, fy2);
            }
            break;
        case 5://Rectangle (H)
            if (fgFill==1) {
                fillRectangle(fx1, fy1, fx2, fy2);
            } else {
                drawRectangle(fx1, fy1, fx2, fy2);
            }
            break;
        case 6://Rectangle (V)
            if (fgFill==1) {
                fillRectangle(fx1, fy1, fx2, fy2);
            } else {
                drawRectangle(fx1, fy1, fx2, fy2);
            }
            break;
        case 7://Circle
            if (fgFill==1) {
                float rd = getPixH(w)/2.0f;
                fillCircle(fx1, fy1, rd, 360);
            } else {
                float rd = getPixH(w)/2.0f;
                DrawCircle(fx1, fy1, rd, 360);
            }
            break;
        case 8://Line (p1, p2)
            if (fgP1==0) {
                lp2 = setfPoint(getX(x), getY(y));
                drawLine(lp1.getX, lp1.getY, lp2.getX, lp2.getY);
            }
            break;
        case 9://Text
            //drawString(xText, fx1, fy1, h);
            drawStringF(dText, fx1, fy1, xFont, xColor);
            break;

        case 10://Delete Shape //Erase
            break;
        case 11://Erase Range
            break;
         //----------- Edit Options -------------
        case 12://Cut
            break;
        case 13://Copy
            break;
        case 14://Paste
            break;
         //--------------------------------------
        default: break;
    }
}
//==========================[Canvas g1 and g2] 600x400x3=====================
void clearCanvas() {
    int i, j;
    int cx, rx;
    char cl[3];
    char ch;
    fRGB color = setfRGB(1.0f, 1.0f, 1.0f);
    for (rx=0; rx<360; rx++) {
        for (cx = 0; cx<600; cx++) {
              g2[rx*590*3+cx*3+0] = 255;
              g2[rx*590*3+cx*3+1] = 255;
              g2[rx*590*3+cx*3+2] = 255;

        }
    }
}
void drawCanvas() {
    unsigned char data[] = {255/*b*/, 0 /*g*/, 0/*r*/, 0/*b*/, 0/*g*/, 255/*r*/};
    glRasterPos2f(getX(20), getY(420));  // <-- commented out
    glDrawPixels(600, 360, GL_RGB, GL_UNSIGNED_BYTE, g2);
}
//================Basic Drawings on the Canvas ===================
void drawPointCanvas(int x, int y, fRGB color) {
    //x = x -35; y = y -22;
    //x = x+128 ;
    y = 400-y ;
    x = x -28;
    if (x<0 || y <0) {return;}
    g2[y*600*3+x*3+0] = 0;
    g2[y*600*3+x*3+1] = 0;
    g2[y*600*3+x*3+2] = 255;

}
void drawLineCanvas(int x1, int y1, int x2, int y2, fRGB color) {
    int rx, cx;
    for (rx=x1; rx<=x2; rx++) {
        for (cx=y1; cx<=y2; cx++) {
                g1[cx][rx] = color;
        }
    }
}
//============================[Message Box and Input Box]===============================================


void drawBox(float x1, float y1, float x2, float y2, float cl1, float cl2, float bd) {

    float clx;

    clx = cl1;
    glLineWidth(bd);
    glColor3f(clx,clx, clx);//Color to Draw;
    drawRectangle(x1,y1, x2, y2);//Border

    clx = cl2;
    glColor3f(clx,clx, clx);//Color to Draw;
    fillRectangle(x1, y1, x2, y2);

}
void msgBox() {
    pos_x = 20;pos_y = 7;

    char txt[65] = {0};
    char ch;
    int i = 0, rw =1, cl = 0, w = 0, h =0;
    //Get number of rows and columns
    int len = strlen(msg);
    rw = 1;
    for (i=0; i<len; i++) {
       ch = msg[i];
       if (ch==13 || ch == 10 || cl >=60) {
            rw++; cl = 0;
       }
       cl++; if (cl>w) w = cl;//max
    }

    //Print Title
    bColor = setfRGB(0.82f, 0.82f, 0.82f);

    int mid = w/2;
    printTextRC(7, 20+w/2-strlen(title)/2, title);
    printTextRCmax(9, 20, 70, msg);
    //Print Close
    printTextRC(9+rw+1, 20+w/2-5,"[Close F4]" );

    bColor = setfRGB(1.0f, 1.0f, 1.0f);
    //Border

    float x1 = txtX(19);
    float x2 = txtX(20+w);
    float y1 = txtY(6)-getPixH(2.0);
    float y2 = txtY(9+rw+1) -getPixH(2.0);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);

}

void inputBox() {
    pos_x = 20;pos_y = 7;

    char txt[65] = {0};
    char ch;
    int i = 0, rw =1, cl = 0, w = 0, h =0;
    //Get number of rows and columns
    int len = strlen(msg);
    rw = 1;
    for (i=0; i<len; i++) {
       ch = msg[i];
       if (ch==13 || ch == 10 || cl >=60) {
            rw++; cl = 0;
       }
       cl++; if (cl>w) w = cl;//max
    }

    //Print Title
    bColor = setfRGB(0.82f, 0.82f, 0.82f);

    int mid = w/2;
    printTextRC(7, 20+w/2-strlen(title)/2, title);
    printTextRCmax(9, 20, 70, msg);
    //Print Close

    printTextRC(9+rw+3, 20+w/2-5,"[ENTER]" );


    bColor = setfRGB(1.0f, 1.0f, 1.0f);
    //Border


    float x1, x2, y1, y2;
    //Input Box
    printTextRC(9+rw+1, 22,input);

    x1 = txtX(20);
    x2 = txtX(20+w-1);

    y1 = txtY(9+rw)-getPixH(2.0);
    y2 = txtY(9+rw+1) -getPixH(2.0);
    drawBox(x1, y1, x2, y2, 0.0f, 1.00f, 0.5f);

    //----------------------------------
    x1 = txtX(19);
    x2 = txtX(20+w+1);

    y1 = txtY(6)-getPixH(2.0);
    y2 = txtY(9+rw+3) -getPixH(2.0);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);

}

void getInput(unsigned char key) {
    if (inpBoxCTR>=50) return;
    if (key==27) {iBox = 0; flagInputBox = 0;}
    if (key==13 || key == 10) {CloseBox();}
    if (key==VK_SHIFT) {if (flagCap==1) {flagCap=0;} else {flagCap=1;}}
    if (key==VK_BACK) {input[inpBoxCTR]=0; inpBoxCTR--; if (inpBoxCTR<0) inpBoxCTR=0;};

    if (key==VK_DECIMAL) key = '.';
    if (key==VK_ADD) key = '+';
    if (key==VK_SUBTRACT) key = '-';
    if (key==VK_MULTIPLY) key = '*';
    if (key==VK_DIVIDE) key = '/';



    if (key >= 32 && key <= 127) {
        if (flagCap==0) {
            if (key>='A' && key<= 'Z') key = key+32;
        }
        input[inpBoxCTR]=key;
        inpBoxCTR++;
        input[inpBoxCTR]=0;
    }

}


void about() {
 char *t = "About";
 char *m = "Basic Paint Program \nMade as Part of a Training Program \nBy P. Ramos - Jul/2016";
 showMsgBox(t, m);
}

void help() {
 char *t = "Help";
 char *m = " Basic Paint Program - Made by Paulo Ramos @2016"
            "\n [F1] - Access the Menus"
            "\n [F2] - Drawing Mode"
            "\n [F4] - Close DialogBoxes"
            "\n [F3] - Clear the Screen"
            "\n Menu Navigation: [F1] [up/down] [left/right]"
            "\n Menu Selection: with '>>' pointed press [SPACE]"
            ;
showMsgBox(t, m);
}

void showMsgBox(char *t, char *m) {
 strcpy(title, t);
 strcpy(msg, m);
 flagInputBox=0;
 flagMsgBox = 1;
}

void showInputBox(char *t, char *m) {
 strcpy(title, t);
 strcpy(msg, m);
 inpBoxCTR = 0;
 input[0]=0; input[1]=0;
 flagMsgBox = 0;
 flagInputBox=1;
}
void setText() {
 char *t = "Set Text";
 char *m = "Enter the Text To be Displayed ";
  iBox = 101; //Set Text Code
 showInputBox(t, m);
}
//================== Change Font and Change Color =================================
void changeColor() {
    int c1, c2, c3;
    int c=0, r=0;

    printTextRC(6, 20,  "[       Select a Color           ]");
    printTextRC(18, 20, "[Selected Color]        [Close F4]");

    float x = 160;
    float y = 120;
    float dsl_x = getX(10);
    float dsl_y = getY(8);
    float x1, y1, x2, y2;
    int ctx =0;

    fRGB xColor;
    for (c1=0; c1<=3; c1++) {
        for (c2=0; c2<=3; c2++) {
            for (c3=0; c3<=3; c3++) {

                x1 = getX(x); y1 = getY(y);
                x2 = getX(x+25); y2 = getY(y+20);


                if (ctx == selectedColor) {
                    dColor = setfRGB((float)c1/3,(float)c2/3,(float) c3/3);
                    glColor3f(1.0f - (float)c1/3,1.0f -(float)c2/3,1.0f -(float) c3/3);
                    drawLine(x1, y1, x2, y2);
                    drawLine(x1, y2, x2, y1);
                }
                ctx ++;



                glColor3f((float)c1/3,(float)c2/3,(float) c3/3);
                fillRectangle(x1, y1, x2, y2);

                glLineWidth(2.0f);
                glColor3f(1.0f - (float)c1/3,1.0f -(float)c2/3,1.0f -(float) c3/3);
                drawRectangle(x1, y1, x2, y2);

                x = x+35;
                c++;
                if (c>=8) {
                    x = 160; y = y +25;
                    c = 0;
                }

            }
        }
    }


                x=160; y = 350;
                x1 = getX(x); y1 = getY(y);
                x2 = getX(x+130); y2 = getY(y+20);

                setColorRGB (dColor);
                fillRectangle(x1, y1, x2, y2);

                glColor3f(0.0f, 0.0f, 0.0f);
                drawRectangle(x1, y1, x2, y2);


                x=145; y = 95;
                x1 = getX(x); y1 = getY(y);
                x2 = getX(x+300); y2 = getY(y+285);

                glColor3f(0.0f, 0.0f, 0.0f);
                drawRectangle(x1, y1, x2, y2);




}
void msgBoxKeys(unsigned char key) {
    if (key==VK_F4 || key==13 || key==VK_SPACE) CloseBox();
}
void changeColorKeys(unsigned char key) {
    if (key==VK_F4  || key==13 || key==VK_SPACE) CloseBox();
    if (key==VK_RIGHT) selectedColor++;
    if (key==VK_LEFT) selectedColor--;
    if (key==VK_UP) selectedColor = selectedColor-8;
    if (key==VK_DOWN) selectedColor = selectedColor+8;
    if (selectedColor <0) selectedColor = 0;
    if (selectedColor >64) selectedColor = 64;
}

void drawFontListBox(int idx, int row, int col, int w) {
    float x1, y1, x2, y2;
    int i, lf, r, dsl, dslx;
    //----------------------------------------------------
    lf = lf_font[idx];
    r = row;
    dslx = 0;
    if (selectedFont[idx]> 6) dslx = selectedFont[idx]-6;
    for (i=0; i<lf; i++) {
        if (i==0) {bColor= setfRGB(1.0f, 1.0f, 1.0f);} else {bColor= setfRGB(0.82f, 0.82f, 0.82f);}
        pos_y = r; pos_x = col;
        if (i==0 && selectFontCTR==idx) printText(">>");
        if (i>0) {
                if ((i+dsl)==selectedFont[idx]) {
                    printText("(*)");
                }else {
                    printText("( )");
                }
        }

        dsl = dslx;
        if (i==0) dsl = 0;
        if (idx==0) printText(fontFamily[i+dsl]);
        if (idx==1) printText(fontStyle[i+dsl]);
        if (idx==2) printText(fontInclination[i+dsl]);
        if (idx==3) printText(fontSize[i+dsl]);

        r++;
        if (i>=6) break;
    }
    if (lf>6) {//Scrolling

        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row); y2 = txtY(row+6);
        float px = (y2-y1)/lf, rp = ((float)selectedFont[idx]/lf)*(y2-y1);
        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row)+rp; y2 = txtY(row)+rp+px;
        drawBox(x1, y1, x2, y2, 0.0f, 0.0f, 2.0f);

        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row); y2 = txtY(row+6);
        drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);
    }
    x1 = txtX(col); x2 = txtX(col+w); y1 = txtY(5); y2 = txtY(5+6);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);
    //----------------------------------------------------

}
Font setFont(int family, int style, int angle, int size) {
    Font t;
    t.family = family;
    t.style = style;
    t.angle = angle;
    t.size = size;
    return t;
}
void changeFont() {
    int i, j, lf, r, c, st_r=6;
    float x1, y1, x2, y2;

    bColor = setfRGB(0.82f, 0.82f, 0.82f);

    printTextRC(4, 20,  "             Select the Font           ");
    x1 = txtX(20); x2 = txtX(67); y1 = txtY(3); y2 = txtY(4);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);

    printTextRC(18, 20, "[Selected Font]            [Close F4]");
    x1 = txtX(20); x2 = txtX(67); y1 = txtY(17); y2 = txtY(18);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);


    //-------------Draw Font List Box ---------------------
    drawFontListBox(0, 5, 20, 14);//Font Family
    drawFontListBox(1, 5, 35, 14);//Font Style
    drawFontListBox(2, 5, 50, 8);//Font
    drawFontListBox(3, 5, 59, 8);//Font Size
    //----------------------------------------------------
    bColor = setfRGB(1.0f, 1.0f, 1.0f);


    //-------------------------------------------

    int ff, ft, fa, fs;
    ff = selectedFont[0];//Font Family
    ft = selectedFont[1];//Font Style
    fa = selectedFont[2];//Font Angle
    fs = atoi(fontSize[selectedFont[3]]);//Font Size
    Font fx = setFont(ff, ft, fa, fs);
    x1 = txtX(22);   y1 = txtY(15);
    //drawString("Font Preview", x1, y1, 100);
    drawStringF("Font Preview", x1, y1, fx, fColor);
    dFont = fx;

    //-------------------------------------------

    printTextRC(12, 20, "Preview:");
    x1 = txtX(20); x2 = txtX(67); y1 = txtY(12); y2 = txtY(17);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);

    //-----------------------------------------------
    x1 = txtX(19); x2 = txtX(68); y1 = txtY(2); y2 = txtY(19);
    drawBox(x1, y1, x2, y2, 0.0f, 1.0f, 2.0f);
}

void changeFontKeys(unsigned char key) {
    //int selectedFont[4] = {1, 1, 1, 1};
    //int selectFontCTR = 0;

    if (key==VK_F4  || key==13 || key==VK_SPACE) CloseBox();
    if (key==VK_RIGHT) selectFontCTR++;
    if (key==VK_LEFT) selectFontCTR--;
    if (key==VK_UP) selectedFont[selectFontCTR]--;
    if (key==VK_DOWN) selectedFont[selectFontCTR]++;


    if (selectFontCTR <0) selectedColor = 2;
    if (selectFontCTR >2) selectedColor = 0;

    if (selectedFont[selectFontCTR]<=1) selectedFont[selectFontCTR] = 1;

    if (selectedFont[selectFontCTR]>= lf_font[selectFontCTR]) selectedFont[selectFontCTR] = lf_font[selectFontCTR]-1;//Family


}

//==========================[SAVE FILE / OPEN FILE] =========================

void openFile(char *cFile) {
    char tmpf[200];
    clearScreen();
    //****************** Opening the File ******************
    int r, c, a, b, ch;
    float f;
    if (strlen(cFile) <=0) {
         char *t2 = "Error Saving File" ;
         char *m = "Inform a valid name to the File!";
         showMsgBox(t2, m);
    }


    FILE *file2 = fopen(cFile, "r"); //

    fscanf(file2,"%[^\n]%*c",&tmpf);
    if (strcmp(tmpf,"BasicPaintProject:")==0) {


    shapeCtr = 0;ctrText = 0;
    int flag, shape, x, y, w, h;


    while (tmpf!=EOF) {
        //printf("%s\n", tmpf);//testing
        if (strcmp(tmpf,"Data:")==0) {
            shapeX t;
            fscanf(file2, "%i %i ", &t.flag, &t.shape);
            fscanf(file2, "%i %i %i %i ", &t.x, &t.y, &t.w, &t.h);
            fscanf(file2, "%i %i %f ", &t.fFill, &t.idxText, &t.pen);
            fscanf(file2, "%f %f ", &t.xlp1.getX, &t.xlp1.getY);
            fscanf(file2, "%f %f %f ", &t.color.R, &t.color.G, &t.color.B);
            fscanf(file2, "%i %i %i %i \n",&t.xFont.family, &t.xFont.style, &t.xFont.angle, &t.xFont.size);
            screenArray[shapeCtr] = t;
            shapeCtr++;
        }
        if (strcmp(tmpf,"Text:")==0) {
            fscanf(file2,"%[^\n]%*c",&tmpf);
            strcpy(listText[ctrText], tmpf);
            ctrText ++;
        }
        if (strcmp(tmpf,"End:")==0) {
                char *t2 = "Opening File" ;
                char *m = "File Opened Successfully!";
                showMsgBox(t2, m);
                break;
        }
        fscanf(file2,"%[^\n]%*c",&tmpf);
    }
    }
    fclose(file2);

}
void saveFile(char *cFile) {
    if (strlen(cFile) <=0) {
         char *t = "Error Saving File" ;
         char *m = "Inform a valid name to the File!";
         showMsgBox(t, m);
    }

    FILE *file1 = fopen(cFile, "w"); //
    int i, j, r, c;

    //------------- Print Data to the File --------------------
    fprintf(file1, "BasicPaintProject:\n");

    shapeX t;
    for (i=0; i<shapeCtr; i++) {
        t= screenArray[i];
        if (t.flag==1) {
            fprintf(file1, "Data:\n");
            fprintf(file1, "%i %i ", t.flag, t.shape);
            fprintf(file1, "%i %i %i %i ", t.x, t.y, t.w, t.h);
            fprintf(file1, "%i %i %f ", t.fFill, t.idxText, t.pen);
            fprintf(file1, "%f %f ", t.xlp1.getX, t.xlp1.getY);
            fprintf(file1, "%f %f %f ", t.color.R, t.color.G, t.color.B);
            fprintf(file1, "%i %i %i %i ",t.xFont.family, t.xFont.style, t.xFont.angle, t.xFont.size);
            fprintf(file1, "\n");
        }
    }

    for (i=0; i<ctrText; i++) {
        fprintf(file1, "Text:\n");
        fprintf(file1,"%s\n", listText[i]);
    }
    fprintf(file1, "End:\n");

    //----------------------------------------------------------
         char *t2 = "Saving File" ;
         char *m = "File Saved Successfully!";
         showMsgBox(t2, m);
    fclose(file1);
}
void saveFileDialog() {
    if (strlen(currFile)>0) {
        char cFile[100];
        strcpy(cFile, currPath);
        strcat(cFile, currFile);
        saveFile(cFile);

    } else {
        saveFileAsDialog();
    }
}
void saveFileAsDialog() {
    printf("\n Inform the new name for the file: %s ", currFile);

    char m [300];
    char *t = "Save File As";
    char *m1 = "Press. Enter to Open the File [";
    char *m2 = "]\nor Inform the name of the new File to be saved:";

    strcpy(m, m1);
    strcat(m, currFile);
    strcat(m, m2);

    iBox = 201;
    showInputBox(t, m);
}
void openFileDialog() {
    char m [300];
    char *t = "Open File";
    char *m1 = "Press. Enter to Open The File [";
    char *m2 = "]\nor Inform the name of the new File to be opened:";


    strcpy(m, m1);
    strcat(m, currFile);
    strcat(m, m2);


    iBox = 202;
    showInputBox(t, m);
}

void CloseBox() {
    flagInputBox = 0 ;
    flagMsgBox = 0;
    flagColorPicker = 0;
    flagFontPicker = 0;
    if (iBox==101) {if (strlen(input)>0) {strcpy(dText, input);}}//setText;
    if (iBox==201) {//Save File as
        if (strlen(input)>0) {
                strcpy(currFile, input);
        }
        if (strlen(currFile)>0) {
            char cFile[100];
            strcpy(cFile, currPath);
            strcat(cFile, currFile);
            saveFile(cFile);

        }
    }
    if (iBox==202) {//OpenFile
        if (strlen(input)>0) {
                strcpy(currFile, input);
        }
        char cFile[100];
        strcpy(cFile, currPath);
        strcat(cFile, currFile);
        if (strlen(currFile)>0) {
            openFile(cFile);
        }
    }
    iBox = 0;
}

//===========================================================================
void shapeSelection() {
    if (selectMode==0) {//Delete
        screenArray[selectedShape].flag = 0;//Simple deletion
    }
    if (selectMode==1) {//Cut
        currShape = screenArray[selectedShape];
        screenArray[selectedShape].flag = 0;//Simple deletion
        flagPaste = 1;
    }
    if (selectMode==2) {//Copy
        currShape = screenArray[selectedShape];
        flagPaste = 1;
    }
    if (selectMode==3) {//Paste
        flagPaste = 1;
    }
}
void changeShapeKeys(unsigned char key) {
    //int selectedFont[4] = {1, 1, 1, 1};
    //int selectFontCTR = 0;
    if (key==VK_F4) flagShape = 0;

    if (flagPaste==1) {
        if (key==VK_RIGHT) mouseX+=4;
        if (key==VK_LEFT) mouseX-=4;
        if (key==VK_UP) mouseY-=4;;
        if (key==VK_DOWN) mouseY+=4;;
        if (key==13 || key==VK_SPACE) {
                screenArray[shapeCtr] = currShape;
                shapeCtr++;
                flagShape =0; flagPaste=0;
        }

        return;
    }


    if (key==13 || key==VK_SPACE) shapeSelection();

    if (key==VK_RIGHT) selectMode++;
    if (key==VK_LEFT) selectMode--;
    if (key==VK_UP) selectedShape--;
    if (key==VK_DOWN) selectedShape++;

    if (selectMode <0) selectMode = 2;
    if (selectMode >2) selectMode = 0;
    if (selectedShape<=0) selectedShape = 0;
    if (selectedShape >= shapeCtr-1) selectedShape = shapeCtr-1;


}

void drawEditShapeBox() {
    int idx, row, col, w;
    float x1, y1, x2, y2;
    int i, lf, r, dsl, dslx;

    row = 3;
    col = 3;
    idx = 1;
    w = 9;
    //----------------------------------------------------
    lf = shapeCtr;
    r = row;
    dsl = 0;
    if (selectedShape> 17) dsl = selectedShape-17;

    shapeX s;
    pos_y = r; pos_x = col;
    bColor= setfRGB(1.0f, 1.0f, 1.0f);
    if (selectMode==0)  printText("Delete Shape");
    if (selectMode==1)  printText("Cut Shape");
    if (selectMode==2)  printText("Copy Shape");
    if (selectMode==3)  printText("Paste Shape");

    bColor= setfRGB(0.82f, 0.82f, 0.82f);
    r++;
    for (i=0; i<lf; i++) {
        pos_y = r; pos_x = col;
        s = screenArray[i+dsl];
        if ((i+dsl)==selectedShape) {
            printText("(*)");
            s.color = setfRGB(0.0f, 0.0f, 1.0f);//Blue
            drawShapeX(s);
        } else {
            printText("( )");
        }
        if (s.flag==1) {
            switch (s.shape) {
                case 3: printText("Circ");break;
                case 7: printText("Circ");break;
                case 8: printText("Line");break;
                case 9: printText("Text");break;
                default: printText("Rect"); break;
            }
        } else {
            printText("----");
        }

        r++;
        if (i >= 17) break;
    }
    if (lf>17) {//Scrolling


        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row); y2 = txtY(row+18);
        float px = (y2-y1)/lf, rp = ((float)selectedShape/lf)*(y2-y1);
        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row)+rp; y2 = txtY(row)+rp+px;
        drawBox(x1, y1, x2, y2, 0.0f, 0.0f, 2.0f);

        x1 = txtX(col+w-1); x2 = txtX(col+w); y1 = txtY(row); y2 = txtY(row+18);
        drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);


    }
    x1 = txtX(col); x2 = txtX(col+w); y1 = txtY(row); y2 = txtY(row+18);
    drawBox(x1, y1, x2, y2, 0.0f, 0.82f, 2.0f);
    //----------------------------------------------------
    bColor= setfRGB(1.0f, 1.0f, 1.0f);

    if (currShape.flag==1 && flagPaste==1) {
            float x1, y1, dx, dy;
            x1 = getX(currShape.x);y1 = getY(currShape.y);
            dx = x1 -currShape.xlp1.getX; dy = y1 - currShape.xlp1.getY;

            //New Position x / y and new xlp1
            currShape.x = mouseX; currShape.y = mouseY;
            x1 = getX(currShape.x);y1 = getY(currShape.y);
            currShape.xlp1.getX = x1 + dx;
            currShape.xlp1.getY = y1 + dy;

            drawShapeX(currShape);
    }

}

void cut_shape() {
   flagShape = 1; flagCut = 1;
   selectMode=1;flagPaste=0;
}
void copy_shape() {
   flagShape = 1; flagCopy = 1;
   selectMode=2;;flagPaste=0;
}
void paste_shape() {
   flagShape= 1; flagPaste = 1;
   selectMode=3;
}
void delete_shape() {
    flagShape = 1;
    selectMode = 0;
}
//===========================================================================

//==========================[Mouse Emulation]================================
void mouseClick() {
    if (flagP1 == 1) {
            lp1 = setfPoint(getX(mouseX), getY(mouseY));
            p1 = setfPoint(getX(mouseX), getY(mouseY));
    } else {


        int opt = menuSelected[4];//Shapes;
        if (opt>1) {
            flagPaint = 0;
            addShapeX(opt, mouseX, mouseY, sz_w, sz_h, dColor, pen, flagFill, lp1, dText, dFont);//Saving
        }
    }
    if (flagP1 == 1) {flagP1 = 0;} else {flagP1 = 1;}//flipFlop;
    if (menuSelected[4]==1) {//Drawing
        if (flagPaint == 1) {flagPaint=0;} else {flagPaint=1;}//flipFlop;
    }

}
void mouseMove() {
    float pen = penSize/2.0f;
    int opt = menuSelected[4];//Shapes;

    drawShape(opt, mouseX,mouseY, sz_w, sz_h, pen, dColor, flagFill, flagP1, dText, dFont);

    if (flagPaint ==1 && menuSelected[4]==1) {
        int t = 0;
        if (t==1) {//Testing canvas
               drawPointCanvas(mouseX, mouseY, dColor) ;
        } else {
            p2 = setfPoint(getX(mouseX), getY(mouseY));
            drawShape(8, mouseX,mouseY, sz_w, sz_h, pen, dColor, flagFill, flagP1, dText, dFont);
            if (lp1.getX != p2.getX && lp1.getY != p2.getY) {
                addShapeX(8, p3.getX, p3.getY, sz_w, sz_h,dColor, pen, flagFill, lp1, dText, dFont);//Save Line
                lp1 = setfPoint(getX(p3.getX), getY(p3.getY));
            }
            p3.getX = mouseX;
            p3.getY = mouseY;
        }
    }
}
void mouseDown() {//Shift Down
}
void mouseUp() {//Shift Up
}


//============================ Basic Sequence =====================================
void setShape() {
        if (menuSelected[5]==1) {//FlagFill
            flagFill = 1;
        } else  {
            flagFill = 0;
        }
        size = atoi(ddSizes[menuSelected[6]]);
        sz_w = size; sz_h = size;
        penSize = (float) atoi(ddPenSizes[menuSelected[7]]);
        pen = penSize/2.0f;
        selItem = menuSelected[4];//Shapes;

        if (selItem==5) {sz_w = size*2; sz_h = size;} //Rectangle (H)
        if (selItem==6) {sz_w = size; sz_h = size*2;} //Rectangle (V)
        flagP1 = 1; flagPaint = 0;
}

void start() {

    bColor = setfRGB(1.0f, 1.0f, 1.0f);
    fColor = setfRGB(0.0f, 0.0f, 0.0f);
    dColor = setfRGB(0.0f, 0.0f, 0.0f);


    lf_font[0] = sizeof(fontFamily)/sizeof(fontFamily[0]);;
    lf_font[1] = sizeof(fontStyle)/sizeof(fontStyle[0]);;
    lf_font[2] = sizeof(fontInclination)/sizeof(fontInclination[0]);;
    lf_font[3] = sizeof(fontSize)/sizeof(fontSize[0]);;

    int ff, ft, fa, fs;
    ff = selectedFont[0];//Font Family
    ft = selectedFont[1];//Font Style
    fa = selectedFont[2];//Font Angle
    fs = atoi(fontSize[selectedFont[3]]);//Font Size
    dFont = setFont(ff, ft, fa, fs);

    shape = 16;exMenu = 0;
    clearScreen();
    menuSelected[4]=1; //drawing
    setShape();


}

void loop() {
    printMenus();
    if (flagMsgBox==1 || flagInputBox==1 || flagColorPicker==1 || flagFontPicker==1) {
            if (flagMsgBox==1) msgBox();
            if (flagInputBox==1)inputBox();
            if (flagColorPicker==1) changeColor();
            if (flagFontPicker==1) changeFont();
    } else {
        if (flagShape == 1) drawEditShapeBox();//testing
        mousePointer();
        mouseMove();
        drawScreen();
    }
    //drawCanvas();
    updateStatus();

}
//===========================================================================
