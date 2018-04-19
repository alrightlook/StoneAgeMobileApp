#include "../systeminc/version.h"
#include "../systeminc/talkwindow.h"
#include "../systeminc/main.h"
//#include "../resource.h"
#include "../systeminc/loadrealbin.h"
#include "../oft/work.h"
#include "../systeminc/font.h"
#include "../systeminc/mouse.h"
#include "../systeminc/tool.h"

#ifdef _TALK_WINDOW

int g_iCursorCount = -1;
extern HFONT hFont;

#ifdef _CHANNEL_MODIFY
extern char g_szChannelTitle[6][13];
extern int TalkMode;
#endif

CTalkWindow::CTalkWindow() {};
CTalkWindow::~CTalkWindow()
{
	Release();
};

void CTalkWindow::Init(HWND hWnd,HINSTANCE hInstance)
{
	int i;
	ChatBufferLink *pCBL;

	m_hdcBackBuffer = 0;
	m_hbmpOldBackBuffer = 0;
	m_hbmpBackBuffer = 0;
	m_bScroll = false;
	m_bUpArrowHit = false;
	m_bDownArrowHit = false;
	m_bInit = false;
	m_hTalkWindow = NULL;
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	m_iline = 0;

	for(i=0;i<SKIN_KIND;i++){
		m_hSkin[i] = 0;
		m_hdcSkin[i] = 0;
		m_hOldSkin[i] = 0;
	}
	pCBL = m_pCBLView = m_pCBLViewBottom = m_pCBLString = m_pCBLHead = (ChatBufferLink*)MALLOC(sizeof(ChatBufferLink));
	if(pCBL == NULL){
		//MessageBox(hWnd,TEXT("CTalkWindow::Init()记忆体配置失败(1)!!"),TEXT("确定"),MB_OK);
		return;
	}
	memset(pCBL,0,sizeof(ChatBufferLink));
	for(i=0;i<TALK_WINDOW_MAX_CHAT_LINE;i++){
		pCBL->next = (ChatBufferLink*)MALLOC(sizeof(ChatBufferLink));
		if(pCBL == NULL){
			//MessageBox(hWnd,TEXT("CTalkWindow::Init()记忆体配置失败(2)!!"),TEXT("确定"),MB_OK);
			Release();
			return;
		}
		memset(pCBL->next,0,sizeof(ChatBufferLink));
		pCBL->next->prev = pCBL;
		pCBL = pCBL->next;
	}
	pCBL->next = NULL;
	m_pCBLTail = pCBL;
	m_bInit = true;
	LoadSkin("data\\skin\\default");
#ifdef _DEBUG
	m_iSymbolCount = 0;
	memset(m_fsFaceSymbol,0,sizeof(m_fsFaceSymbol));
	memset(m_ssStoreSymbol,0,sizeof(m_ssStoreSymbol));
	ReadFaceSymbolFile();
#endif
};

void CTalkWindow::Release(void)
{
	ChatBufferLink *pCBL = m_pCBLHead,*pCBLTemp;

	for(int i=0;i<SKIN_KIND;i++){
		if(m_hSkin[i]){
			SelectObject(m_hdcSkin[i],m_hOldSkin[i]);
			DeleteObject(m_hSkin[i]);
		}
		if(m_hdcSkin[i]) DeleteDC(m_hdcSkin[i]);
	}
	if(m_hbmpBackBuffer){
		SelectObject(m_hdcBackBuffer,m_hbmpOldBackBuffer);
		DeleteObject(m_hbmpBackBuffer);
	}
	if(m_hdcBackBuffer) DeleteDC(m_hdcBackBuffer);	
	KillTimer(m_hTalkWindow,1);
	for(int i=0;i<TALK_WINDOW_MAX_CHAT_LINE;i++){
		if(pCBL->next){
			if(pCBL->next->next){
				pCBLTemp = pCBL->next->next;
				FREE(pCBL->next);
				pCBL->next = pCBLTemp;
			}
			else FREE(pCBL->next);
		}
	}
	FREE(m_pCBLHead);
#ifdef _DEBUG
	ReleaseFaceSymbol();
#endif
};

void CTalkWindow::Create()
{
	RECT rect;
	WNDCLASS wndclass;

	if(!m_bInit) return;
	if(m_hTalkWindow){
		ShowWindow(m_hTalkWindow,SW_SHOW);
		SetFocus(hWnd);
		return;
	}
	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = TalkWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInstance;
  wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(m_hInstance ,MAKEINTRESOURCE(SA_MOUSE));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName= NULL;
	wndclass.lpszClassName = "TALK";
	RegisterClass(&wndclass);
	GetWindowRect(hWnd,&rect);
	m_hTalkWindow = CreateWindow("TALK","TalkWIndow",WS_OVERLAPPED,
															rect.left,rect.top + rect.bottom,
															SKIN_WIDTH,SKIN_HEIGHT,hWnd,NULL,m_hInstance,NULL);
	if(m_hTalkWindow){
		unsigned long dwStyle;
		HRGN hRegion;
		
		ShowWindow(m_hTalkWindow,SW_SHOW);
		UpdateWindow(m_hTalkWindow);
		hRegion = CreateRectRgn(0,0,SKIN_WIDTH,SKIN_HEIGHT+GetSystemMetrics(SM_CYSIZE)+GetSystemMetrics(SM_CXFIXEDFRAME));
		SetWindowRgn(m_hTalkWindow,hRegion,true);
		DeleteObject(hRegion);
		dwStyle = GetWindowLong(m_hTalkWindow,GWL_STYLE);
		dwStyle &= ~(WS_CAPTION|WS_SIZEBOX);
		SetWindowLong(m_hTalkWindow,GWL_STYLE,dwStyle);
		InvalidateRect(m_hTalkWindow,NULL,true);
		SetWindowPos(m_hTalkWindow,NULL,0,0,SKIN_WIDTH,SKIN_HEIGHT+GetSystemMetrics(SM_CYSIZE),SWP_NOMOVE|SWP_NOZORDER);
		SetTimer(m_hTalkWindow,1,500,(TIMERPROC)NULL);
		TalkWindow.Update();
		SetFocus(hWnd);
	}
}

void CTalkWindow::Update()
{
	if(g_bTalkWindow) SendMessage(m_hTalkWindow,WM_UPDATE_SKIN,0,0);
}

void CTalkWindow::LoadSkin(char *szSkinPath)
{
	char szFileName[5][32] = 
	{ "\\base.bmp","\\up_arrow_g.bmp","\\up_arrow_r.bmp","\\down_arrow_g.bmp","\\down_arrow_r.bmp"};
	char szTemp[128];
	// 读入skin的图
	for(int i=0;i<SKIN_KIND;i++){
		sprintf(szTemp,"%s%s",szSkinPath,szFileName[i]);
		m_hSkin[i] = LoadImage(NULL,szTemp,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if(!m_hSkin[i]) return;
		m_hdcSkin[i] = CreateCompatibleDC(NULL);
		m_hOldSkin[i] = SelectObject(m_hdcSkin[i],m_hSkin[i]);
	}
	m_hdcBackBuffer = CreateCompatibleDC(NULL);
	m_hbmpBackBuffer = CreateCompatibleBitmap(m_hdcSkin[0],SKIN_WIDTH,SKIN_HEIGHT);
	m_hbmpOldBackBuffer = SelectObject(m_hdcBackBuffer,m_hbmpBackBuffer);
}

void CTalkWindow::DrawSkin(bool bShowCursor)
{
	int j;
	char szBuffer[STR_BUFFER_SIZE + 1];
	unsigned char color;
	HFONT hOldFont;
	HDC hdc;
	ChatBufferLink *pCBL;
	
	// 显示skin到backbuffer dc
	BitBlt(m_hdcBackBuffer,0,0,SKIN_WIDTH,SKIN_HEIGHT,m_hdcSkin[0],0,0,SRCCOPY);
	if(m_bUpArrowHit) BitBlt(m_hdcBackBuffer,620,8,SKIN_WIDTH,SKIN_HEIGHT,m_hdcSkin[2],0,0,SRCCOPY);
	else BitBlt(m_hdcBackBuffer,620,8,SKIN_WIDTH,SKIN_HEIGHT,m_hdcSkin[1],0,0,SRCCOPY);
	if(m_bDownArrowHit) BitBlt(m_hdcBackBuffer,620,98,SKIN_WIDTH,SKIN_HEIGHT,m_hdcSkin[4],0,0,SRCCOPY);
	else BitBlt(m_hdcBackBuffer,620,98,SKIN_WIDTH,SKIN_HEIGHT,m_hdcSkin[3],0,0,SRCCOPY);
	
	SetBkMode(m_hdcBackBuffer,TRANSPARENT);
	hOldFont = (HFONT)SelectObject(m_hdcBackBuffer,hFont);
	j = 0;
	// 显示输出的文字
	pCBL = m_pCBLView;
	for(int i=0;i<MAX_TALK_WINDOW_LINE;i++){
		if(pCBL != NULL){
#ifdef _DEBUG
			SetToFaceSymbolString(szBuffer,pCBL,TALK_WINDOW_SXO,TALK_WINDOW_SYO + j * 20);
#else
			strcpy(szBuffer,pCBL->ChatBuffer.buffer);
#endif
			color = pCBL->ChatBuffer.color;
			if(pCBL->bUse){
				SetTextColor(m_hdcBackBuffer,0);
				TextOut(m_hdcBackBuffer,TALK_WINDOW_SXO + 1,TALK_WINDOW_SYO + 1 + j * 18,szBuffer,(int)strlen(szBuffer)); 
				SetTextColor(m_hdcBackBuffer,FontPal[color]);
				TextOut(m_hdcBackBuffer,TALK_WINDOW_SXO,TALK_WINDOW_SYO + j * 18,szBuffer,(int)strlen(szBuffer)); 
				j++;
			}
		}
		if(pCBL->next == NULL) pCBL = m_pCBLHead;
		else pCBL = pCBL->next;
	}
	// 显示输入的文字
	strcpy(szBuffer,MyChatBuffer.buffer);
	color = MyChatBuffer.color;
	SetTextColor(m_hdcBackBuffer,0);
#ifdef _CHANNEL_MODIFY
	TextOut(m_hdcBackBuffer,TALK_WINDOW_SXI - 25,TALK_WINDOW_SYI+1,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode]));  // 显示频道
#endif
	TextOut(m_hdcBackBuffer,TALK_WINDOW_SXI + 1,TALK_WINDOW_SYI + 1,szBuffer,(int)strlen(szBuffer)); 
	SetTextColor(m_hdcBackBuffer,FontPal[color]);
#ifdef _CHANNEL_MODIFY
	TextOut(m_hdcBackBuffer,TALK_WINDOW_SXI - 26,TALK_WINDOW_SYI,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode])); // 显示频道
#endif
	TextOut(m_hdcBackBuffer,TALK_WINDOW_SXI,TALK_WINDOW_SYI,szBuffer,(int)strlen(szBuffer)); 
	
	// 显示游标
	if(bShowCursor){
		int x;

		x = TALK_WINDOW_SXI + MyChatBuffer.cursor * (FONT_SIZE>>1);
		SetTextColor(m_hdcBackBuffer,0);
		TextOut(m_hdcBackBuffer,x + 1,TALK_WINDOW_SYI + 1,"_",1);
		SetTextColor(m_hdcBackBuffer,FontPal[color]);
		TextOut(m_hdcBackBuffer,x,TALK_WINDOW_SYI,"_",1);
	}
	SelectObject(m_hdcBackBuffer,hOldFont);
#ifdef _DEBUG
	ShowFaceSymbol();
#endif
	// draw to window dc
	hdc = GetDC(m_hTalkWindow);
	BitBlt(hdc,0,0,SKIN_WIDTH,SKIN_HEIGHT,m_hdcBackBuffer,0,0,SRCCOPY);
	ReleaseDC(m_hTalkWindow,hdc);
}

void CTalkWindow::AddString(char *szString,int color)
{
	if(m_hTalkWindow){
		// 游戏一开始没字串,所以要先把 m_iline 累加到 MAX_TALK_WINDOW_LINE 才进行显示框的移动
		if(m_iline <= MAX_TALK_WINDOW_LINE) m_iline++;
		strcpy(m_pCBLString->ChatBuffer.buffer,szString);
		m_pCBLString->ChatBuffer.color = color;
		m_pCBLString->bUse = true;
		// 游戏一开始都没有字串,所以要当 m_iline 值大于等于 MAX_TALK_WINDOW_LINE 时才进行显示框的移动
		// 正处于卷动状态时不进行显示框的移动
		if(!m_bScroll && m_iline > MAX_TALK_WINDOW_LINE){
			if(m_pCBLView->next != NULL) m_pCBLView = m_pCBLView->next;
			else m_pCBLView = m_pCBLHead;
		}
		// 不处于卷动状态时进行显示框的移动
		if(!m_bScroll){
			if(m_pCBLViewBottom->next != NULL) m_pCBLViewBottom = m_pCBLViewBottom->next;
			else m_pCBLViewBottom = m_pCBLHead;
		}
		// 当 m_pCBLString->next 时,表示已到 list 尾,指回 list 头
		if(m_pCBLString->next == NULL) m_pCBLString = m_pCBLHead;
		else m_pCBLString = m_pCBLString->next;
	}
}

// 上卷
void CTalkWindow::UpArrowHit(bool bHit)
{ 
	m_bUpArrowHit = bHit;
	if(bHit){
		m_bScroll = false;
		// 若 m_pCBLView 和 m_pCBLString 不相等,表示目前显示范围还没超过 m_pCBLString
		if(m_pCBLView != m_pCBLString){
			// 若 m_pCBLView->prev 为 NULL,表示上卷到底了
			if(m_pCBLView->prev == NULL){
				// 若list尾部有在使用的话,把 m_pCBLView 指向 m_pCBLTail
				if(m_pCBLTail->bUse){
					m_pCBLView = m_pCBLTail;
					m_bScroll = true;
					m_pCBLViewBottom = m_pCBLViewBottom->prev;	// 移动显示框
					if(m_pCBLViewBottom == NULL) m_pCBLViewBottom = m_pCBLTail;
				}
			}
			// 未卷到底
			else if(m_pCBLView->prev->bUse){
				m_pCBLView = m_pCBLView->prev;
				m_bScroll = true;
				m_pCBLViewBottom = m_pCBLViewBottom->prev;
				if(m_pCBLViewBottom == NULL) m_pCBLViewBottom = m_pCBLTail;
			}
		}
	}
}

// 下卷
void CTalkWindow::DownArrowHit(bool bHit)
{ 
	m_bDownArrowHit = bHit;
	if(bHit){
		m_bScroll = false;
		// 若 m_pCBLViewBottom 和 m_pCBLString 不相等,表示目前显示范围还没超过 m_pCBLString
		if(m_pCBLViewBottom != m_pCBLString){
			// 若 m_pCBLView->next 为 NULL,表示下卷到底了
			if(m_pCBLView->next == NULL){
				// 若list尾部有在使用的话,把 m_pCBLView 指向 m_pCBLHead
				if(m_pCBLTail->bUse){
					m_pCBLView = m_pCBLHead;
					m_bScroll = true;
					m_pCBLViewBottom = m_pCBLViewBottom->next;	// 移动显示框
					if(m_pCBLViewBottom == NULL) m_pCBLViewBottom = m_pCBLHead;
				}
			}
			// 未卷到底
			else if(m_pCBLView->next->bUse){
				m_pCBLView = m_pCBLView->next;
				m_bScroll = true;
				m_pCBLViewBottom = m_pCBLViewBottom->next;
				if(m_pCBLViewBottom == NULL) m_pCBLViewBottom = m_pCBLHead;
			}
		}
	}
}

void CTalkWindow::ClearChatBuffer(void)
{
	ChatBufferLink *pCBL = m_pCBLHead;

	while(pCBL != NULL){
		memset(&pCBL->ChatBuffer,0,sizeof(CHAT_BUFFER));
		pCBL->bUse = false;
		pCBL = pCBL->next;
	}
	m_pCBLView = m_pCBLViewBottom = m_pCBLString = m_pCBLHead;
	m_iline = 0;
}

void CTalkWindow::Visible(bool flag)
{
	ShowWindow(m_hTalkWindow,SW_HIDE);
	::ClearChatBuffer();
}

#ifdef _DEBUG
void CTalkWindow::ReadFaceSymbolFile(void)
{
	FILE *pfFaceSymbolFile = NULL;
	char szReadBuffer[32];
	int iStrlen;

	pfFaceSymbolFile = fopen("data\\facesymbol.ini","r");
	if(pfFaceSymbolFile){
		for(int i=0;i<FACE_SYMBOL_NUM;i++){
			memset(szReadBuffer,0,sizeof(szReadBuffer));
			if(fgets(szReadBuffer,sizeof(szReadBuffer),pfFaceSymbolFile) == NULL) break;
			getStringToken(szReadBuffer,'=',1,sizeof(m_fsFaceSymbol[i].szSymbol),m_fsFaceSymbol[i].szSymbol);
			getStringToken(szReadBuffer,'=',2,sizeof(m_fsFaceSymbol[i].szFaceName),m_fsFaceSymbol[i].szFaceName);
			chop(m_fsFaceSymbol[i].szFaceName);
			iStrlen = strlen(m_fsFaceSymbol[i].szFaceName);
			if(iStrlen <= 0) break;
			m_fsFaceSymbol[i].bUse = true;
		}
		fclose(pfFaceSymbolFile);
	}
	InitFaceSymbol(RGB(0,0,0));
}

void CTalkWindow::InitFaceSymbol(COLORREF MaskColor)
{
	char szFileName[128];
	HANDLE hTemp;

	for(int i=0;i<FACE_SYMBOL_NUM;i++){
		if(m_fsFaceSymbol[i].bUse){
			sprintf(szFileName,"data\\skin\\facesymbol\\%s",m_fsFaceSymbol[i].szFaceName);
			m_fsFaceSymbol[i].hLoadBMP = LoadImage(NULL,szFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_DEFAULTSIZE);
			if(m_fsFaceSymbol[i].hLoadBMP == NULL){
				m_fsFaceSymbol[i].bUse = false;
				continue;
			}
			hTemp = CopyImage(m_fsFaceSymbol[i].hLoadBMP,IMAGE_BITMAP,SYMBOL_WIDTH,SYMBOL_HEIGHT,LR_COPYDELETEORG); // 如果原图比19 19大,则会自动缩小
			DeleteObject(m_fsFaceSymbol[i].hLoadBMP);
			if(hTemp == NULL){
				m_fsFaceSymbol[i].bUse = false;
				continue;
			}
			m_fsFaceSymbol[i].hLoadBMP = hTemp;
			m_fsFaceSymbol[i].hDraw = CreateCompatibleDC(NULL);
			m_fsFaceSymbol[i].hOldLoadBMP = SelectObject(m_fsFaceSymbol[i].hDraw,m_fsFaceSymbol[i].hLoadBMP);
			m_fsFaceSymbol[i].hDrawMask = CreateCompatibleDC(m_fsFaceSymbol[i].hDraw);
			// 产生单色通透图
			m_fsFaceSymbol[i].hbmpMaskBMP = CreateBitmap(SYMBOL_WIDTH,SYMBOL_HEIGHT,1,1,NULL);
			m_fsFaceSymbol[i].hOldMaskBMP = SelectObject(m_fsFaceSymbol[i].hDrawMask,m_fsFaceSymbol[i].hbmpMaskBMP);
			// 设定透明色
			SetBkColor(m_fsFaceSymbol[i].hDraw,MaskColor);
			SetTextColor(m_fsFaceSymbol[i].hDraw,RGB(0,0,0));
			// 产生透明区域为白色,其他区域为黑色的单色图
			BitBlt(m_fsFaceSymbol[i].hDrawMask,0,0,SYMBOL_WIDTH,SYMBOL_HEIGHT,m_fsFaceSymbol[i].hDraw,0,0,SRCCOPY);
			// 把原图换成透明区域为黑色,其他区域不变的图
			SetBkColor(m_fsFaceSymbol[i].hDraw,RGB(0,0,0));
			SetTextColor(m_fsFaceSymbol[i].hDraw,RGB(255,255,255));
			BitBlt(m_fsFaceSymbol[i].hDraw,0,0,SYMBOL_WIDTH,SYMBOL_HEIGHT,m_fsFaceSymbol[i].hDrawMask,0,0,SRCAND);
		}
	}
	SetBkColor(m_hdcBackBuffer,RGB(255,255,255));
	SetTextColor(m_hdcBackBuffer,RGB(0,0,0));
}

void CTalkWindow::ReleaseFaceSymbol(void)
{
	for(int i=0;i<FACE_SYMBOL_NUM;i++){
		if(m_fsFaceSymbol[i].bUse){
			if(m_fsFaceSymbol[i].hLoadBMP){
				SelectObject(m_fsFaceSymbol[i].hDraw,m_fsFaceSymbol[i].hOldLoadBMP);
				DeleteObject(m_fsFaceSymbol[i].hLoadBMP);
			}
			if(m_fsFaceSymbol[i].hDraw){
				SelectObject(m_fsFaceSymbol[i].hDraw,m_fsFaceSymbol[i].hOldLoadBMP);
				DeleteDC(m_fsFaceSymbol[i].hDraw);
			}
			if(m_fsFaceSymbol[i].hDrawMask){
				SelectObject(m_fsFaceSymbol[i].hDrawMask,m_fsFaceSymbol[i].hOldMaskBMP);
				DeleteDC(m_fsFaceSymbol[i].hDrawMask);
			}
		}
	}
}

void CTalkWindow::SetToFaceSymbolString(char *szDestString,ChatBufferLink *pCBL,int x,int y)
{
	int iCount = 0,k,iStoreX,iCheck,iSymbolNum = 0;
	static char szSourString[STR_BUFFER_SIZE + 1],szTemp[STR_BUFFER_SIZE + 1];
	int iStrlen;
	bool bBreak;

	memcpy(szSourString,pCBL->ChatBuffer.buffer,sizeof(szSourString));
	memset(szTemp,0,sizeof(szTemp));
	iStrlen = strlen(szSourString);
	for(int i=0;i<iStrlen;i++){
		bBreak = false;
		iStoreX = i;
		int j;
		for(j=0;j<FACE_SYMBOL_NUM && !bBreak;j++){
			k = 0;
			if(m_fsFaceSymbol[j].bUse){
				iCheck = 0;
				while(1){
					if(szSourString[i] == m_fsFaceSymbol[j].szSymbol[k]){
						k++;iCheck++;
						if(m_fsFaceSymbol[j].szSymbol[k] == '\0'){
							// 显示的图 x 座标超过了最右边,把接下来的字串拼入到下一行
							if((x + (iStoreX + iSymbolNum) * (FONT_SIZE>>1)) > 610){
								if(pCBL->next != NULL){
									sprintf(szTemp,"%s%s",&szSourString[iStoreX],pCBL->next->ChatBuffer.buffer);
									sprintf(pCBL->next->ChatBuffer.buffer,"%s",szTemp);
								}else{
									sprintf(szTemp,"%s%s",&szSourString[iStoreX],m_pCBLHead->ChatBuffer.buffer);
									sprintf(m_pCBLHead->ChatBuffer.buffer,"%s",szTemp);
								}
								// 把原先的字串分离成二行
								memcpy(szTemp,szSourString,iStoreX);
								sprintf(szSourString,"%s",szTemp);
								sprintf(pCBL->ChatBuffer.buffer,"%s",szTemp);
								// 设定 i 为 STR_BUFFER_SIZE + 1 是为了直接离开 i 那层 loop
								i = STR_BUFFER_SIZE + 1;
								bBreak = true;
								break;
							}
							szDestString[iCount++] = ' ';
							szDestString[iCount++] = ' ';
							szDestString[iCount++] = ' ';
							// 记录要在那个位置显示表情符号
							m_ssStoreSymbol[m_iSymbolCount].bUse = true;
							m_ssStoreSymbol[m_iSymbolCount].hDraw = m_fsFaceSymbol[j].hDraw;
							m_ssStoreSymbol[m_iSymbolCount].hDrawMask = m_fsFaceSymbol[j].hDrawMask;
							m_ssStoreSymbol[m_iSymbolCount].x = x + (iStoreX + iSymbolNum) * (FONT_SIZE>>1);
							m_ssStoreSymbol[m_iSymbolCount].y = y;
							m_iSymbolCount++;
							bBreak = true;
							iSymbolNum++;
							break;
						}
						i++;
					}
					else{
						i = iStoreX;
						break;
					}
				}
			}
			//if(bBreak) break;
		}
		if(j == FACE_SYMBOL_NUM){
			szDestString[iCount++] = szSourString[iStoreX];
		}
	}
	szDestString[iCount] = '\0';
}

void CTalkWindow::ShowFaceSymbol(void)
{
	SetTextColor(m_hdcBackBuffer,0);
	for(int i=0;i<STORE_SYMBOL_NUM;i++){
		if(m_ssStoreSymbol[i].bUse){
			BitBlt(m_hdcBackBuffer,m_ssStoreSymbol[i].x,m_ssStoreSymbol[i].y,SKIN_WIDTH,SKIN_HEIGHT,m_ssStoreSymbol[i].hDrawMask,0,0,SRCAND);
			BitBlt(m_hdcBackBuffer,m_ssStoreSymbol[i].x,m_ssStoreSymbol[i].y,SKIN_WIDTH,SKIN_HEIGHT,m_ssStoreSymbol[i].hDraw,0,0,SRCPAINT);
		}
		// 因为 m_ssStoreSymbol 内容的产生是照顺序产生的,所以只要有一个 m_ssStoreSymbol 的 bUse 为 false 就可以直接离开 loop
		else break;
	}
	memset(m_ssStoreSymbol,0,sizeof(m_ssStoreSymbol));
	m_iSymbolCount = 0;
}
#endif

long CALLBACK TalkWindowProc(HWND hWnd,unsigned int Message,WPARAM wParam,LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static bool bShowCursor = false;

	switch(Message){
		case WM_MOUSEMOVE:
			if(g_iCursorCount < 0) g_iCursorCount = ShowCursor(true);
			break;
		case WM_KEYDOWN:
			switch(wParam){
			case VK_LEFT:
				KeyboardLeft();
				break;
			case VK_RIGHT:
				KeyboardRight();
				break;
			case VK_RETURN:
				KeyboardReturn();
				break;
			case VK_BACK:
				if(joy_con[1] & JOY_RSHIFT || joy_con[1] & JOY_LSHIFT){
					if(pNowStrBuffer != NULL){
						pNowStrBuffer->cnt = 0;
						pNowStrBuffer->buffer[0] = NULL;
					}
				}else KeyboardBackSpace();
				break;
			case VK_DELETE:
				TalkWindow.ClearChatBuffer();
				break;
			case VK_CONTROL:
				if(di_key[DIK_V] & 0x80) GetClipboad();
				break;
			case VK_V:
				if(GetKeyState(VK_CONTROL) & 0xff00) GetClipboad();
				break;
			case VK_UP:
				joy_auto[0] |= JOY_UP;
				ChatProc();
				break;
			case VK_DOWN:
				joy_auto[0] |= JOY_DOWN;
				ChatProc();
				break;
			}
			TalkWindow.Update();
			break;
		case WM_CHAR:
			StockStrBufferChar((char)wParam);
			break;
		case WM_TIMER:
			bShowCursor = !bShowCursor;
			TalkWindow.DrawSkin(bShowCursor);
			break;
		case WM_PAINT:
			BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);
		case WM_UPDATE_SKIN:
			TalkWindow.DrawSkin(bShowCursor);
			break;
		case WM_MOVE:
			RECT rect;

			GetWindowRect(::hWnd,&rect);
			SetWindowPos(hWnd,NULL,rect.left,rect.bottom,0,0,SWP_NOZORDER|SWP_NOSIZE);
			break;
		case WM_LBUTTONDOWN:
			if(MAKEPOINTS(lParam).x >= 620 && MAKEPOINTS(lParam).x <= 638 &&
				 MAKEPOINTS(lParam).y >= 8 && MAKEPOINTS(lParam).y <= 26){
				TalkWindow.UpArrowHit(true);
				TalkWindow.DrawSkin(bShowCursor);
			}
			if(MAKEPOINTS(lParam).x >= 620 && MAKEPOINTS(lParam).x <= 638 &&
				 MAKEPOINTS(lParam).y >= 98 && MAKEPOINTS(lParam).y <= 116){
				TalkWindow.DownArrowHit(true);
				TalkWindow.DrawSkin(bShowCursor);
			}
			break;
		case WM_LBUTTONUP:
			TalkWindow.UpArrowHit(false);
			TalkWindow.DownArrowHit(false);
			TalkWindow.DrawSkin(bShowCursor);
			break;
		default:
			return DefWindowProc( hWnd, Message, wParam, lParam );
	}
	return 0;
}
#endif
