// LineAndCircle.cpp : Defines the entry point for the application.
//
#include "Globals.h"
#include "basicgraphic.h"
#include "Projection.h"
#include "Transform.h"
#include "Utils.h"
#include "Lighting.h"
#include "Texture.h"
#include "Rast.h"
#include "ZBuffer.h"

static CCamera g_cam;
static CRenderObjectsManager g_RenderManager;
static CLightManager g_lights;
POINT3D cam_pos(0.0,0.0,0.0);
Vector4D cam_dir(0.0,0.0,0.0);
static  float g_rotx=0.0,g_roty=0.0,g_rotz=0.0, g_tranz = 0.0;
int g_fps = 0;

HWND g_hwnd;
HDC g_hdc;
BITMAP_FILE bitmap;

LRESULT CALLBACK MyWndProc(HWND,UINT, WPARAM ,LPARAM );
void Render();
void Init();
void main_logic(HWND hWindowHandle);

//HBITMAP CreateMemDC(HDC hdc,int cx,int cy)
//{
//	
//	g_hMemdc = ::CreateCompatibleDC(NULL);
//	HBITMAP hcompatiblebitmap = ::CreateCompatibleBitmap(hdc,cx,cy);
//	HBITMAP holdbitmap = (HBITMAP)::SelectObject(g_hMemdc,hcompatiblebitmap);
//	HBRUSH hbrush = GetStockBrush(WHITE_BRUSH);
//	HBRUSH holdbrush =(HBRUSH) ::SelectObject(g_hMemdc,hbrush);
//	::Rectangle(g_hMemdc,-1,-1,cx+1,cy+1);
//	::SelectObject(g_hMemdc,holdbrush);
//	return hcompatiblebitmap;
//
//	/*SetBkColor(g_hMemdc,RGB(255,0,255));*///find the reason
//}
#ifdef _DEBUG
void TestVector()
{
	Vector4D a(1.0,2.0,3.0);
	Vector4D b(2.0,3.0,4.0);

	Vector4D c= a+b;
	Vector4D d = a-b;
	c +=a;
	c -=a;
	float e = a.DotProduct(b);
	Vector4D f = a.CrossProduct(b);

}
void TestMatrix()
{
	float  arr[] = {
		         1.0,0.0,0.0,0.0,
				 0.0, 1,0,0,
				 0,0,1,0,
				 0,0,0,1};
	Matrix m1(arr);
	Matrix m2 = m1 *m1;
	Matrix m3 = m1+m1;
	m2.Identity();
	m2 = m1*4;
}
#endif

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	WNDCLASS  cs;
	cs.cbClsExtra =0;
	cs.cbWndExtra = 0;
	cs.hbrBackground = (HBRUSH)(COLOR_WINDOW +1);//°×É«±³¾°
	cs.hCursor       = LoadCursor(NULL,IDC_CROSS);
	cs.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
	cs.hInstance  = hInstance;
	cs.lpszClassName = "CircleLine";
	cs.lpszMenuName = NULL;
	cs.style = CS_OWNDC;
	cs.lpfnWndProc =(WNDPROC) MyWndProc;
	if(!RegisterClass(&cs))
	{
		MessageBox(NULL,"Register window class error","Error",MB_OK);
	}
    int  x = 	GetSystemMetrics(SM_CXSCREEN);
	int  y =    GetSystemMetrics(SM_CYSCREEN);
   HWND hWindowHandle =	CreateWindowEx(WS_EX_CLIENTEDGE|WS_EX_OVERLAPPEDWINDOW,"CircleLine","Lab1",
	              WS_OVERLAPPEDWINDOW ,x/2,y/2,800,600,NULL,NULL,NULL,NULL);
   _ASSERT(hWindowHandle != NULL);
   g_hwnd = hWindowHandle;
   g_hdc = GetDC(g_hwnd);

   Init();

   ShowWindow(hWindowHandle,nCmdShow);
   UpdateWindow(hWindowHandle);
#ifdef _DEBUG
   TestVector();
   TestMatrix();
#endif
  
   MSG  msg;
   while(1)
   {
	   if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	   { 
		   // test if this is a quit
		   if (msg.message == WM_QUIT)
			   break;

		   // translate any accelerator keys
		   TranslateMessage(&msg);

		   // send the message to the window proc
		   DispatchMessage(&msg);
	   } // end if

	   // main game processing goes here
	  // Render();
	   //LOCAL rotation
	   main_logic(hWindowHandle);

   } // end whi
  
 
	return msg.wParam;
}

static char buff[255];

LRESULT CALLBACK MyWndProc(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam)
{

	PAINTSTRUCT ps;
	switch(msg)
	{
	case WM_LBUTTONDOWN:
		{
	    


		}
		return 0;
	case WM_TIMER:
		{
			
			sprintf(buff,"Press F1 to test no light version,up/down to change z position,home/end to open/close light,speed:%d fps",g_fps);
			::OutputDebugStr(buff);
			g_fps = 0;
		}
	case WM_KEYUP:
		{
			
          
		}
		break;
	case WM_CREATE:
		{

		}
		break;
	case WM_KEYDOWN:
		{
			
		}
		break;
	case WM_LBUTTONUP:
		{
			
			
		}
		break;
	case 0x020A:
		{
			RECT rc;
			GetClientRect(g_hwnd,&rc);
			int cx = rc.right -rc.left;
			int cy = rc.bottom-rc.top;
			int dir = HIWORD(wParam);
			if(dir>0)//forward
			{
               cam_pos += Vector4D(0,0,0.5);
			}
			else
			{
				cam_pos += Vector4D(0,0,-0.5);
			}
			
			g_cam.Init(cam_pos,cam_dir,60,50,500,cx,cy);
		}
	case WM_MOUSEMOVE:
		
		return 0;
	case WM_SIZE:
		{
		
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			g_cam.Init(cam_pos,cam_dir,60,50,500,width,height);
            
			delete g_pCurSurface;
			g_pCurSurface = NULL;
			g_pCurSurface = new CSurface;
			SetClearColor(RGB(255,255,255));//white background

			g_pCurSurface->Destroy();
			g_pCurSurface->Create(g_hdc,width,height);
			ZBuffer::GetInstance()->Create(width,height);
		

			
		}
	case WM_PAINT:
		{
			 BeginPaint(hwnd,&ps);
			
			/*line(510,500,360,270,RGB(0,0,0));

			DrawTopTriangle(50,50,160,50,100,100,RGB(255,0,0));
			DrawBottomTriangle(100,100,40,140,80,140,0xff0000);
			Triangle(150,150,400,200,300,230,0x00ff00);*/
			
			//HDC hdc  = ::GetDC(hwnd);
			//CreateMemDC(hdc);

			//Render();
			//RECT rc;
			//GetClientRect(hwnd,&rc);
			//int width = rc.right - rc.left;
			//int height = rc.bottom - rc.top;
			//::BitBlt(hdc,0,0,width,height,g_hMemdc,0,0,SRCCOPY);
			  EndPaint(hwnd,&ps);
			//::ReleaseDC(hwnd,hdc);
			//::DeleteDC(g_hMemdc);
			//::DeleteObject(g_hComatiblebitmap);
		}

		return 0;
	case WM_DESTROY:
		{	
		    
			delete g_pCurSurface;
			g_pCurSurface = NULL;
			ZBuffer::GetInstance()->Destroy();
            KillTimer(g_hwnd,100);
			PostQuitMessage(0);
	        return 0;
		}
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}
void Init()
{
    //
	::SetTimer(g_hwnd,100,1000,NULL);
   

	bool bret = LoadBitmapFile(&bitmap,"wall.bmp");

	int bitmapwidth = bitmap.bitmapinfoheader.biWidth;
	int bitmapheight = bitmap.bitmapinfoheader.biHeight;

	CRenderObject *pobj = new CRenderObject;
	std::vector<Vertex> pointlist;
	pointlist.push_back(Vertex(POINT3D(10,10,10) ));
	pointlist.push_back(Vertex(POINT3D(10,10,-10)));
	pointlist.push_back(Vertex(POINT3D(-10,10,-10)));
	pointlist.push_back(Vertex(POINT3D(-10,10,10)));
	pointlist.push_back(Vertex(POINT3D(10,-10,10)));
	pointlist.push_back(Vertex(POINT3D(-10,-10,10)));
	pointlist.push_back(Vertex(POINT3D(-10,-10,-10)));
	pointlist.push_back(Vertex(POINT3D(10,-10,-10)));
    
	pobj->m_translateverticesList = pointlist;
	pobj->m_verticesList = pointlist;
  

	//Set texture pointlist
	std::vector<POINT2D> texturelist;
	texturelist.push_back(POINT2D(0,0));
	texturelist.push_back(POINT2D(bitmapwidth-1,0));
	texturelist.push_back(POINT2D(bitmapwidth-1,bitmapheight-1));
	texturelist.push_back(POINT2D(0,bitmapheight-1));

	pobj->m_texturecoordinatesList = texturelist;


	int tempindices[36]=
	{
		0,1,2, 0,2,3,
		0,7,1, 0,4,7,
		1,7,6, 1,6,2,
		2,6,5, 2,3,5,
		0,5,4, 0,3,5,
		5,6,7, 4,5,7
	};
	int textindices[36] =
	{
        2,1,0, 2,0,3,
		2,0,1, 2,3,0,
		1,0,3, 1,3,0,
		0,1,2, 2,3,0,
		2,0,1, 2,3,0,
		2,1,0, 3,2,0
	};

	for(int i =0; i <12;++i)
	{

    POLYGON temp ;
	temp.v[0]  = tempindices[i*3+0];
	temp.v[1]  = tempindices[i*3+1];
	temp.v[2]  = tempindices[i*3 +2];

	temp.text[0] = textindices[i*3+0];
	temp.text[1] = textindices[i*3+1];
	temp.text[2] = textindices[i*3+2];


    temp.state |= OBJECT_HAS_TEXTURE|SHADE_MODEL_CONSTANT;
	temp.color = RGBA(RGB(128,123,140));
	temp.texture = &bitmap;
	temp.pointlist = &pobj->m_verticesList;
	temp.texturecoordinatelist = &pobj->m_texturecoordinatesList;
	pobj->m_PolyGonList.push_back(temp);
   }
	pobj->world_pos = POINT3D(-10,0,50);
	CRenderObject *pobj2 = new CRenderObject(*pobj);
	pobj2->world_pos = POINT3D(15,0,50);
	CRenderObject* pobj3 =new CRenderObject(*pobj);
	pobj3->world_pos = POINT3D(0,10,80);
	CRenderObject *pobj4  = new CRenderObject(*pobj);
	pobj4->world_pos = POINT3D(20,-10,100);

	g_RenderManager.Add(pobj);
	g_RenderManager.Add(pobj2);
	g_RenderManager.Add(pobj3);
	g_RenderManager.Add(pobj4);

  //Set lights
  CLight *pLight = new CLight;
  pLight->InitLight(CLight::kAmbientLight,RGBA(RGB(255,0,0)),Black_Color,Black_Color);
  g_lights.AddLight(pLight);
 
  CLight *pLight2 = new CLight;
  pLight2->InitLight(CLight::kInfiniteLigtht,Black_Color,RGBA(255,0,0),Black_Color,ZeroVector,
	               Vector4D(-1.0,0.0,1.0));
   g_lights.AddLight(pLight2);

  CLight *pLight3 = new  CLight;
  pLight3->InitLight(CLight::kPointLight,Black_Color,RGBA(0,255,0),Black_Color,Vector4D(0,10,0),
	                 ZeroVector,1.0,2.0,4.0);
  g_lights.AddLight(pLight3);

  CLight *pLight4 = new CLight;
  pLight4->InitLight(CLight::kSpotLight,Black_Color,RGBA(255,255,255),Black_Color,Vector4D(0,20,10),
	                Vector4D(0.0,0.0,1.0),0.0,0.01,0.0,30.0,60.0,1.0);
  g_lights.AddLight(pLight4);


  g_RenderManager.SetCamera(&g_cam);
  g_RenderManager.SetLightManager(&g_lights);
}
void  Render()
{
   Matrix mat,mat1,mat2;
   Bulid_XYZ_RotationMatrix(mat1,0.0,0.0,g_roty);
   BuildTranslateMatrix(mat2,0.0,0.0,g_tranz);
   static bool bDrawNonTextureCube=false;
   if(KEY_DOWN(VK_F1))
   {
	   //Draw the color cube. don't use texture
	   bDrawNonTextureCube = !bDrawNonTextureCube;
   }
 
   int icount = g_RenderManager.RenderObjectSize();
   for(int i = 0; i < icount;++i)
   {
       CRenderObject *pobj = g_RenderManager[i];
	   POLYGONLIST &polygonlist = pobj->m_PolyGonList;
	   int polygonsize = polygonlist.size();
	   for( int j = 0; j < polygonsize;++j)
	   {
		   POLYGON &polygon = polygonlist[j];
		   if(bDrawNonTextureCube)
			   polygon.state &= ~OBJECT_HAS_TEXTURE;
		   else 
			   polygon.state |= OBJECT_HAS_TEXTURE;
	   }
	  
	   pobj->Transform(mat1);//transform local
	   pobj->world_pos *= mat2;
	  
   }
   //local->world,world->camera,camera->screen
   static bool bEnableLighting = false;
   static bool bEableBackfaceRemove =false;
   static bool bEnableInvZBuffer = false;
   if(KEY_DOWN(VK_HOME))
   {
	   //user press p
	   for(int i=0; i< g_lights.GetLightsCount();++i)
	   {
		   CLight* pLight = g_lights.GetLight(i);
		    pLight->EnableLight();
		 /*  if(pLight->GetLightType() == CLight::kAmbientLight)
		   {
			   pLight->EnableLight();
			   break;
		   }*/
	   }
	   bEnableLighting = true;
	   
   }
   else if(KEY_DOWN(VK_END))
   {
	   bEnableLighting = false;
   }
   else if(KEY_DOWN(VK_F2))
   {
	   bEableBackfaceRemove = true;
   }
   else if(KEY_DOWN(VK_F3))
   {
	   bEableBackfaceRemove = false;
   }
   else if(KEY_DOWN(VK_F4))
   {
	   bEnableInvZBuffer = true;
   }
   else if(KEY_DOWN(VK_F5))
   {
	   bEnableInvZBuffer = false;
   }
   if(bEnableInvZBuffer)
    ZBuffer::GetInstance()->Clear(0);
   else
    ZBuffer::GetInstance()->Clear((1<<30)-1);

   g_RenderManager.Render(bEnableLighting,bEableBackfaceRemove,!bEnableInvZBuffer);
   g_fps++;
}

void main_logic(HWND hWindowHandle)
{
	
	if(KEY_DOWN(VK_LEFT))
	{
		g_roty = -0.05;

	}
	else if(KEY_DOWN(VK_RIGHT))
	{
		g_roty = 0.1;
	}
	else if(KEY_DOWN(VK_UP))
	{
		g_tranz = 0.1;
	}
	else if(KEY_DOWN(VK_DOWN))
	{
		g_tranz = -0.1;
	}
	else
	{
		g_roty = 0.0;
		g_tranz = 0.0;
	}
	g_roty = 0.1;
	if(++g_roty>360)g_roty =0.0;

	if(!g_pCurSurface) return;
	
	g_pCurSurface->Clear();

	static char tip1[]="Press F2 to enable back face remove,press F3 to disable back face remove";
	static char tip2[]="Press F4 to enable 1/z buffer,press F5 to use z buffer(default)";
	TextOut(g_pCurSurface->dib_hDC,0,0,buff,strlen(buff));
	TextOut(g_pCurSurface->dib_hDC,0,20,tip1,strlen(tip1));
	TextOut(g_pCurSurface->dib_hDC,0,40,tip2,strlen(tip2));
	
	Render();

	g_pCurSurface->Draw();
	//::GdiFlush();
	
}