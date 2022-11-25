// ������ ��� ������ � ����������� layered.dll v 0.2.1.0 (09.10.2020)

[code]
#ifndef UNICODE
  #error REQUIRES UNICODE INNO SETUP!
#endif

const
  SCENE_BRUSH = -1;
  SCENE_FONT  = -1;
  // ��������� ����� ���������� � ����� �������, ��� � �������� ��������� ����������� ������� Brush ��� Font (����� lFontDelete � lBrushDelete)
  // � ���� ������ ������������ ����������� ����� � ����� ��������������� �� ������ �� ������� ���������� ������.

type
  TArrayOfWChar = array [0..511] of WideChar;  // �� ������ ������ !!!

  Scene = integer;
  Texture = integer;
  Brush = integer;
  Font = integer;
  Img = integer;
  Str = integer;
  Window = integer;

// --- Init  --- \\

function lStartup(): bool; external 'lStartup@{tmp}\layered.dll stdcall delayload';
// ���������

procedure lShutdown(); external 'lShutdown@{tmp}\layered.dll stdcall delayload';
// ���������. ������� ��������� ������� �� ������

procedure lMoving(handle: hwnd); external 'lMoving@{tmp}\layered.dll stdcall delayload';
// ReleaseCapture � SendMessage(handle,$0112,$F012,0)

// --- Texture  --- \\

function lTextureCreateW(const filename: string): Texture; external 'lTextureCreateW@{tmp}\layered.dll stdcall delayload';
// ��������� �������� �� �����

function lTextureWidth(texture: Texture): uint; external 'lTextureWidth@{tmp}\layered.dll stdcall delayload';
function lTextureHeight(texture: Texture): uint; external 'lTextureHeight@{tmp}\layered.dll stdcall delayload';
// ���������� ������ ��������

function lTextureDelete(texture: Texture): integer; external 'lTextureDelete@{tmp}\layered.dll stdcall delayload';
// ������� �������� �� ������

// --- Scene  --- \\

function lSceneCreate(handle: hwnd): Scene; external 'lSceneCreate@{tmp}\layered.dll stdcall delayload';
// ������� ����� �� ����

function lSceneDelete(scene: Scene): integer; external 'lSceneDelete@{tmp}\layered.dll stdcall delayload';
// ������� ����� �� ������ (��� �������� ��� �� ��������� ��� �����������, ����� � ���� ��������� �� ������ �����)

function lSceneAddlayer(scene: Scene): integer; external 'lSceneAddlayer@{tmp}\layered.dll stdcall delayload';
// ��������� ���� ��������� �� �����

procedure lSceneApply(scene: Scene); external 'lSceneApply@{tmp}\layered.dll stdcall delayload';
// ��������� ��������� �� �����

const
  // lSceneSetLong index
  SSL_PAINTEVENT = 1;
  SSL_BRUSH = 2;
  SSL_FONT = 3;
  SSL_HITTEST_TITLEX = 4;
  SSL_HITTEST_TITLEY = 5;

procedure lSceneSetLong(scene: Scene; index: integer; newlong: longword); external 'lSceneSetLong@{tmp}\layered.dll stdcall delayload';
// ������������� �������������� ��������� �����

procedure lSceneSetBrush(scene: Scene; brush: Brush); begin lSceneSetLong(scene, SSL_BRUSH, brush); end;
// ������������� ����� ����� ��� ������ ****

procedure lSceneSetFont(scene: Scene; font: Font); begin lSceneSetLong(scene, SSL_FONT, font); end;
// ������������� ����� ����� ��� ������ ***

procedure lSceneMoving(scene: Scene; left, top, right, bottom: integer);
begin
  lSceneSetLong(Scene, SSL_HITTEST_TITLEX, left or right shl 16);
  lSceneSetLong(Scene, SSL_HITTEST_TITLEY, top or bottom shl 16);
end;
// ������������� ������� ����� �� ������� ����� ���������� ���� ����� (� ������� �� ������ ������� �������� ����������, ����� ��������� ��� ��� �� ��������������)

// --- Brush  --- \\

function lSolidBrushCreate(color: dword): Brush; external 'lSolidBrushCreate@{tmp}\layered.dll stdcall delayload';
// ������� �������� ����� *

function lBrushDelete(brush: Brush): integer; external 'lBrushDelete@{tmp}\layered.dll stdcall delayload';
// ������� ����� �� ������

// --- Font  --- \\

function lFontCreate(hfont: longint): Font; external 'lFontCreate@{tmp}\layered.dll stdcall delayload';
// ������� ����� (�� TFont.Handle)

function lFontDelete(font: Font): integer; external 'lFontDelete@{tmp}\layered.dll stdcall delayload';
// ������� ����� �� ������

// --- Image  --- \\ **

function lDrawImage(texture: Texture; x, y: integer): bool; external 'lDrawImage@{tmp}\layered.dll stdcall delayload';
function lDrawImageRect(texture: Texture; x, y, cx, cy: integer): bool; external 'lDrawImageRect@{tmp}\layered.dll stdcall delayload';
function lDrawImageRectRect(texture: Texture; x, y, cx, cy, srcx, srcy, srccx, srccy: integer): bool; external 'lDrawImageRectRect@{tmp}\layered.dll stdcall delayload';
// ������ �����������

// --- String  --- \\ **

const
  // ������������ ������
  SA_NAER   = 0; // �� ������ ����     // ������
  SA_CENTER = 1; // �� ������          // �� ������
  SA_FAR    = 2; // �� ������� ����    // �����

function lStringAlignment(value: uint): bool; external 'lStringAlignment@{tmp}\layered.dll stdcall delayload';
function lStringLineAlignment(value: uint): bool; external 'lStringLineAlignment@{tmp}\layered.dll stdcall delayload';
// �������������� � ������������ ������������ ������

function lDrawString(const caption: string; x, y: integer; var cx, cy: integer; font: Font; brush: Brush): bool; external 'lDrawString@{tmp}\layered.dll stdcall delayload';
// ������ ����� (� ����������)

function lDrawStringRect(const caption: string; x, y, cx, cy: integer; font: Font; brush: Brush): bool; external 'lDrawStringRect@{tmp}\layered.dll stdcall delayload';
// ������ ����� 
// ���� � ������� ������������ ������ �������� ������

function lMeasureString(const caption: string; font: Font; var cx, cy: integer): bool; external 'lMeasureString@{tmp}\layered.dll stdcall delayload';
// ��������� ������ ������������� ������

// --- Img  --- \\

function lImgLoad(scene: Scene; texture: Texture; left, top, width, height: integer): Img; external 'lImgLoad@{tmp}\layered.dll stdcall delayload';
// ��������� �����������

function lImgDelete(img: Img): integer; external 'lImgDelete@{tmp}\layered.dll stdcall delayload';
// ������� ����������� �� ������

procedure lImgSetVisibility(img: Img; value: bool); external 'lImgSetVisibility@{tmp}\layered.dll stdcall delayload';
// ������������� �������� ��������� �����������

function lImgGetVisibility(img: Img): bool; external 'lImgGetVisibility@{tmp}\layered.dll stdcall delayload';
// ���������� �������� ��������� �����������

procedure lImgSetVisiblePart(img: Img; left, top, width, height: integer); external 'lImgSetVisiblePart@{tmp}\layered.dll stdcall delayload';
// ������������� ������� ����� �����������

procedure lImgGetVisiblePart(img: Img; var left, top, width, height: integer); external 'lImgGetVisiblePart@{tmp}\layered.dll stdcall delayload';
// ���������� ������� ����� �����������

procedure lImgSetPosition(img: Img; left, top, width, height: integer); external 'lImgSetPosition@{tmp}\layered.dll stdcall delayload';
// ������������� ������� �����������

procedure lImgGetPosition(img: Img; var left, top, width, height: integer); external 'lImgGetPosition@{tmp}\layered.dll stdcall delayload';
// ���������� ������� �����������

// --- Str --- \\

function lStrLoad(scene: Scene; const caption: string; left, top, width, height: integer; autosize: bool): Str; external 'lStrLoad@{tmp}\layered.dll stdcall delayload';
// ��������� �������

function lStrDelete(str: Str): integer; external 'lStrDelete@{tmp}\layered.dll stdcall delayload';
// ������� ������� �� ������

procedure lStrSetCaption(str: Str; const caption: string); external 'lStrSetCaption@{tmp}\layered.dll stdcall delayload';
// ������������� ����� �������

procedure lStrGetCaption_(str: Str; var caption: TArrayOfWChar); external 'lStrGetCaption@{tmp}\layered.dll stdcall delayload';
// ���������� ����� �������

function lStrGetLength(str: Str): integer; external 'lStrLength@{tmp}\layered.dll stdcall delayload';
// ������ ����� �������

procedure lStrSetVisibility(str: Str; value: bool); external 'lStrSetVisibility@{tmp}\layered.dll stdcall delayload';
// ������������� �������� ��������� �������

function lStrGetVisibility(str: Str): bool; external 'lStrGetVisibility@{tmp}\layered.dll stdcall delayload';
// ���������� �������� ��������� �������

procedure lStrSetPosition(str: Str; left, top, width, height: integer); external 'lStrSetPosition@{tmp}\layered.dll stdcall delayload';
// ������������� ������� �������

procedure lStrGetPosition(str: Str; var left, top, width, height: integer); external 'lStrGetPosition@{tmp}\layered.dll stdcall delayload';
// ���������� ������� �������

procedure lStrSetBrush(str: Str; brush: Brush); external 'lStrSetBrush@{tmp}\layered.dll stdcall delayload';
// ������������� ����� ��� ������� ****

procedure lStrSetFont(str: Str; font: Font); external 'lStrSetFont@{tmp}\layered.dll stdcall delayload';
// ������������� ����� ��� ������� ***

procedure lStrSetAlignment(str: Str; value: integer); external 'lStrSetAlignment@{tmp}\layered.dll stdcall delayload';
// �������������� ������������ ������ �������

procedure lStrSetLineAlignment(str: Str; value: integer); external 'lStrSetLineAlignment@{tmp}\layered.dll stdcall delayload';
// ������������ ������������ ������ �������

// --- ���������� --- \\

//========================================================================================================================================================================================================
// *    - ���� �������� � ������� ARGB
//        ������: $FFFF0000 - ������� ��� ������������; $7F00FF00 - ������� � 50% �������������
//========================================================================================================================================================================================================
// **   - ������� ����� �������� ������ � ������� onpaint �����
//========================================================================================================================================================================================================
// ***  - ����� ��������� �����������: Name := "Lucida Console"; Height := 12; Weight := FW_NORMAL; Italic := false; Underline := false; StrikeOut := false; CharSet := ANSI_CHARSET; Quality := DEFAULT_QUALITY;
//        ���� ����� ������������ �� ���������
//========================================================================================================================================================================================================
// **** - ����������� �����: �������� - $ff000000 (������ ��� ������������)
//        ��� ����� ������������ �� ���������
//========================================================================================================================================================================================================

// --- Window --- \\

type
  TPos = record
    Left, Top, Width, Height: Integer;
  end;

const
  // ����� ���� lCreateWindow
  CW_BTNPUSH = $1; // ������
  CW_BTNCHECK = $2; // ������ ������
  CW_PROGRESS = $4; // ��������� ���������
  CW_EDIT = $8; // ���� �����

function lCreateWindow(scene: Scene; style: dword; left, top, width, height: integer; const caption: string; texture: Texture): Window; external 'lCreateWindow@{tmp}\layered.dll stdcall delayload';
// ������� ���� ****

procedure lDestroyWindow(window: Window); external 'lDestroyWindow@{tmp}\layered.dll stdcall delayload';
// ������� ���� �� ������

procedure lSetWindowText(window: Window; const caption: string); external 'lSetWindowText@{tmp}\layered.dll stdcall delayload';
// ������������� ����� ��������� ��� ����

function lGetWindowTextLength(window: Window): integer; external 'lGetWindowTextLength@{tmp}\layered.dll stdcall delayload';
// ������ ������ ��������� ����

procedure lGetWindowText_(window: Window; var caption: TArrayOfWChar); external 'lGetWindowText@{tmp}\layered.dll stdcall delayload';
// ������ ��������� ����

procedure lSetWindowPos(window: Window; left, top: integer); external 'lSetWindowPos@{tmp}\layered.dll stdcall delayload';
// ������������� ������� ����

procedure lGetWindowPos_(window: Window; var left, top, width, height: integer); external 'lGetWindowPos@{tmp}\layered.dll stdcall delayload';
// ������ ������� ����

const
  L_FALSE = 0;
  L_TRUE = 1;

  // progress state
  L_NORMAL = 1;
  L_PAUSE = 2;
  L_ERROR = 3;

  // lSetWindowLong index *
  SWL_EVENTONCLICK = 1;  // ������� ����� �����
  SWL_EVENTONMOUSELEAVE = 2; // ���� �������� ������� ����
  SWL_EVENTONMOUSEENTER = 3; // ���� ����� � ������� ����
  SWL_EVENTCHANGE = 13; // ���������� ��� ��������� ������ ����

  SWL_BRUSHNORMAL = 4; // ����� ��� ������ � ���������� ��������� *****
  SWL_BRUSHFOCUSED = 5; // �����, ����� ���� � ������
  SWL_BRUSHDISABLED = 6; // �����, ����� ���� ����������
  SWL_ALIGNMENT = 10;  // ������������ ������ ***
  SWL_LINEALIGNMENT = 11; // ������������ ������������ ***
  SWL_FONT = 12; // �����

  // lSetWindowLong/lGetWindowLong index
  SWL_ENABLE = 7; // ����������� *
  SWL_VISIBILITY = 8; // ���������
  SWL_CHECKED = 9; // �������� �� ������� (��� CW_BTNCHECK) *  **
  SWL_PROGRESSVALUE = 1; // �������� ��������� (��� CW_PROGRESS)
  SWL_PROGRESSMAX = 2; // ����������� TProgressBar.Max
  SWL_PROGRESSSTATE = 3; // ��������� ������������ (L_NORMAL, L_PAUSE, L_ERROR)

  // lGetWindowLong index
  GWL_POSX = -1; // ���������� � ������ ����
  GWL_POSY = -2;
  GWL_POSCX = -3;
  GWL_POSCY = -4;

procedure lSetWindowLong(window: Window; index: integer; newlong: longint); external 'lSetWindowLong@{tmp}\layered.dll stdcall delayload';
// ������������� �������������� ��������� ����

function lGetWindowLong(window: Window; index: integer): longint; external 'lGetWindowLong@{tmp}\layered.dll stdcall delayload';
// ������ ��������� ��������� ����

function lGetWindowPos(window: Window): TPos; // ������ ������� ����
begin
  Result.Left := lGetWindowLong(window, GWL_POSX);
  Result.Top := lGetWindowLong(window, GWL_POSY);
  Result.Width := lGetWindowLong(window, GWL_POSCX);
  Result.Height := lGetWindowLong(window, GWL_POSCY);
end;

procedure lSetWindowVisibility(window: Window; value: bool); // ������������� �������� ��������� ����
begin
  if value then
    lSetWindowLong(window, SWL_VISIBILITY, L_TRUE)
  else
    lSetWindowLong(window, SWL_VISIBILITY, L_FALSE);
end;

function lGetWindowVisibility(window: Window): bool; var r: integer; // ������ �������� ��������� ����
begin
  r := lGetWindowLong(window, SWL_VISIBILITY);
  if r = L_FALSE then RESULT := FALSE else RESULT := TRUE;
end;

procedure lSetWindowEnabled(window: Window; value: bool); // ������������� ����������� ����
begin
  if value then
    lSetWindowLong(window, SWL_ENABLE, L_TRUE)
  else
    lSetWindowLong(window, SWL_ENABLE, L_FALSE);
end;

function lGetWindowEnabled(window: Window): bool; var r: integer; // ������ �������� ����������� ����
begin
  r := lGetWindowLong(window, SWL_ENABLE);
  if r = L_FALSE then RESULT := FALSE else RESULT := TRUE;
end;

procedure lSetBtnChecked(window: Window; value: bool); // �������������/������� ������� (��� CW_BTNCHECK) **
begin
  if value then
    lSetWindowLong(window, SWL_CHECKED, L_TRUE)
  else
    lSetWindowLong(window, SWL_CHECKED, L_FALSE);
end;

function lGetBtnChecked(window: Window): bool; var r: integer; // �������� �� ������� (��� CW_BTNCHECK)
begin
  r := lGetWindowLong(window, SWL_CHECKED);
  if r = L_FALSE then RESULT := FALSE else RESULT := TRUE;
end;

procedure lSetProgressValue(window: Window; value: integer); // ������������� �������� ���������� ���������
begin
  lSetWindowLong(window, SWL_PROGRESSVALUE, value);
end;

function lGetProgressValue(window: Window): integer; // ������ �������� ���������� ���������
begin
  RESULT := lGetWindowLong(window, SWL_PROGRESSVALUE);
end;

procedure lSetProgressMax(window: Window; value: integer); // ������������� ������������ �������� ���������� ���������
begin
  lSetWindowLong(window, SWL_PROGRESSMAX, value);
end;

function lGetProgressMax(window: Window): integer; // ������ ������������ �������� ���������� ���������
begin
  RESULT := lGetWindowLong(window, SWL_PROGRESSMAX);
end;

procedure lSetProgressState(window: Window; value: integer); // ������������� ��������� ���������� ���������
begin
  lSetWindowLong(window, SWL_PROGRESSSTATE, value);
end;

function lGetProgressState(window: Window): integer; // ������ ��������� ���������� ���������
begin
  RESULT := lGetWindowLong(window, SWL_PROGRESSSTATE);
end;

// --- ���������� --- \\

//========================================================================================================================================================================================================
// *    - �� ��������� ��� ���� �� ������ CW_PROGRESS
//========================================================================================================================================================================================================
// **   - ��� CW_BTNPUSH ����������� ����� �� ����(��� ������ �������� newlong)
//========================================================================================================================================================================================================
// ***  - ��������� ������ ��� CW_BTNPUSH
//========================================================================================================================================================================================================
// **** - �������� ��������� ����������� ��� ������� ����� ����.
//        ��������� ���� �� ����������
//========================================================================================================================================================================================================
// ***** - ���� �� ������ CW_EDIT ������������ ������ �������� ����� (lSolidBrushCreate)
//========================================================================================================================================================================================================

function ArrayOfWideCharToString(a: TArrayOfWChar): String; // ����� �� botva2.iss. ������� South
var
  i: integer;
begin
  i := 0;
  Result := '';
  while a[i] <> #0 do begin
    Result := Result + a[i];
    i := i + 1;
  end;
end;

function lStrGetCaption(str: Str): string;
var
  a: TArrayOfWChar;
begin
  lStrGetCaption_(str, a);
  Result := ArrayOfWideCharToString(a);
end;

function lGetWindowText(window: Window): string;
var
  a: TArrayOfWChar;
begin
  lGetWindowText_(window, a);
  Result := ArrayOfWideCharToString(a);
end;

////////////////////// ������ Taskbar //////////////

const
  TBPF_NOPROGRESS = 0; // ����
  TBPF_INDETERMINATE = 1; // ������������ �������
  TBPF_NORMAL = 2; // �������
  TBPF_ERROR = 4; // �������
  TBPF_PAUSED = 8; // ������

procedure SetupPreview(handle: hwnd); external 'SetupPreview@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarProgressValue(value: integer); external 'SetTaskBarProgressValue@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarProgressState(value: integer); external 'SetTaskBarProgressState@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarTitle(const caption: String); external 'SetTaskBarTitle@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarThumbnailTooltip(const Caption: String); external 'SetTaskBarThumbnailTooltip@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarOverlayIcon(hIcon: HICON); external 'SetTaskBarOverlayIcon@{tmp}\layered.dll stdcall delayload';

////////////////////// ����� Taskbar ///////////////
