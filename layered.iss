// Модуль для работы с библиотекой layered.dll v 0.2.1.0 (09.10.2020)

[code]
#ifndef UNICODE
  #error REQUIRES UNICODE INNO SETUP!
#endif

const
  SCENE_BRUSH = -1;
  SCENE_FONT  = -1;
  // константу можно подставить в любую функцию, где в качестве параметра принимаются объекты Brush или Font (кроме lFontDelete и lBrushDelete)
  // в этом случае используются стандартные кисть и шрифт ассоциированные со сценой на которой расположен объект.

type
  TArrayOfWChar = array [0..511] of WideChar;  // не менять размер !!!

  Scene = integer;
  Texture = integer;
  Brush = integer;
  Font = integer;
  Img = integer;
  Str = integer;
  Window = integer;

// --- Init  --- \\

function lStartup(): bool; external 'lStartup@{tmp}\layered.dll stdcall delayload';
// запускает

procedure lShutdown(); external 'lShutdown@{tmp}\layered.dll stdcall delayload';
// отключает. удаляет созданные объекты из памяти

procedure lMoving(handle: hwnd); external 'lMoving@{tmp}\layered.dll stdcall delayload';
// ReleaseCapture и SendMessage(handle,$0112,$F012,0)

// --- Texture  --- \\

function lTextureCreateW(const filename: string): Texture; external 'lTextureCreateW@{tmp}\layered.dll stdcall delayload';
// загружает текстуру из файла

function lTextureWidth(texture: Texture): uint; external 'lTextureWidth@{tmp}\layered.dll stdcall delayload';
function lTextureHeight(texture: Texture): uint; external 'lTextureHeight@{tmp}\layered.dll stdcall delayload';
// возвращает размер текстуры

function lTextureDelete(texture: Texture): integer; external 'lTextureDelete@{tmp}\layered.dll stdcall delayload';
// удаляет текстуру из памяти

// --- Scene  --- \\

function lSceneCreate(handle: hwnd): Scene; external 'lSceneCreate@{tmp}\layered.dll stdcall delayload';
// создает сцену из окна

function lSceneDelete(scene: Scene): integer; external 'lSceneDelete@{tmp}\layered.dll stdcall delayload';
// удаляет сцену из пямяти (при удалении так же удаляются все изображения, текст и окна созданные на данной сцене)

function lSceneAddlayer(scene: Scene): integer; external 'lSceneAddlayer@{tmp}\layered.dll stdcall delayload';
// добавляет слой рисования на сцену

procedure lSceneApply(scene: Scene); external 'lSceneApply@{tmp}\layered.dll stdcall delayload';
// применяет изменения на сцене

const
  // lSceneSetLong index
  SSL_PAINTEVENT = 1;
  SSL_BRUSH = 2;
  SSL_FONT = 3;
  SSL_HITTEST_TITLEX = 4;
  SSL_HITTEST_TITLEY = 5;

procedure lSceneSetLong(scene: Scene; index: integer; newlong: longword); external 'lSceneSetLong@{tmp}\layered.dll stdcall delayload';
// устанавливает дополнительные параметры сцены

procedure lSceneSetBrush(scene: Scene; brush: Brush); begin lSceneSetLong(scene, SSL_BRUSH, brush); end;
// устанавливает новую кисть для текста ****

procedure lSceneSetFont(scene: Scene; font: Font); begin lSceneSetLong(scene, SSL_FONT, font); end;
// устанавливает новый шрифт для текста ***

procedure lSceneMoving(scene: Scene; left, top, right, bottom: integer);
begin
  lSceneSetLong(Scene, SSL_HITTEST_TITLEX, left or right shl 16);
  lSceneSetLong(Scene, SSL_HITTEST_TITLEY, top or bottom shl 16);
end;
// устанавливает область сцены за которую можно перетащить окно мышью (в область не должны попасть элементы управления, иначе сообщения для них не обрабатываются)

// --- Brush  --- \\

function lSolidBrushCreate(color: dword): Brush; external 'lSolidBrushCreate@{tmp}\layered.dll stdcall delayload';
// создает сплошную кисть *

function lBrushDelete(brush: Brush): integer; external 'lBrushDelete@{tmp}\layered.dll stdcall delayload';
// удаляет кисть из памяти

// --- Font  --- \\

function lFontCreate(hfont: longint): Font; external 'lFontCreate@{tmp}\layered.dll stdcall delayload';
// создает шрифт (из TFont.Handle)

function lFontDelete(font: Font): integer; external 'lFontDelete@{tmp}\layered.dll stdcall delayload';
// удаляет шрифт из памяти

// --- Image  --- \\ **

function lDrawImage(texture: Texture; x, y: integer): bool; external 'lDrawImage@{tmp}\layered.dll stdcall delayload';
function lDrawImageRect(texture: Texture; x, y, cx, cy: integer): bool; external 'lDrawImageRect@{tmp}\layered.dll stdcall delayload';
function lDrawImageRectRect(texture: Texture; x, y, cx, cy, srcx, srcy, srccx, srccy: integer): bool; external 'lDrawImageRectRect@{tmp}\layered.dll stdcall delayload';
// рисует изображение

// --- String  --- \\ **

const
  // выравнивание текста
  SA_NAER   = 0; // по левому краю     // сверху
  SA_CENTER = 1; // по центру          // по центру
  SA_FAR    = 2; // по правому краю    // снизу

function lStringAlignment(value: uint): bool; external 'lStringAlignment@{tmp}\layered.dll stdcall delayload';
function lStringLineAlignment(value: uint): bool; external 'lStringLineAlignment@{tmp}\layered.dll stdcall delayload';
// горизонтальное и вертикальное выравнивание текста

function lDrawString(const caption: string; x, y: integer; var cx, cy: integer; font: Font; brush: Brush): bool; external 'lDrawString@{tmp}\layered.dll stdcall delayload';
// рисует текст (в разработке)

function lDrawStringRect(const caption: string; x, y, cx, cy: integer; font: Font; brush: Brush): bool; external 'lDrawStringRect@{tmp}\layered.dll stdcall delayload';
// рисует текст 
// цвет и уровень прозрачности текста задается кистью

function lMeasureString(const caption: string; font: Font; var cx, cy: integer): bool; external 'lMeasureString@{tmp}\layered.dll stdcall delayload';
// вычисляет размер нарисованного текста

// --- Img  --- \\

function lImgLoad(scene: Scene; texture: Texture; left, top, width, height: integer): Img; external 'lImgLoad@{tmp}\layered.dll stdcall delayload';
// загружает изображение

function lImgDelete(img: Img): integer; external 'lImgDelete@{tmp}\layered.dll stdcall delayload';
// удаляет изображение из памяти

procedure lImgSetVisibility(img: Img; value: bool); external 'lImgSetVisibility@{tmp}\layered.dll stdcall delayload';
// устанавливает значение видимости изображения

function lImgGetVisibility(img: Img): bool; external 'lImgGetVisibility@{tmp}\layered.dll stdcall delayload';
// возвращает значение видимости изображения

procedure lImgSetVisiblePart(img: Img; left, top, width, height: integer); external 'lImgSetVisiblePart@{tmp}\layered.dll stdcall delayload';
// устанавливает видимую часть изображения

procedure lImgGetVisiblePart(img: Img; var left, top, width, height: integer); external 'lImgGetVisiblePart@{tmp}\layered.dll stdcall delayload';
// возвращает видимую часть изображения

procedure lImgSetPosition(img: Img; left, top, width, height: integer); external 'lImgSetPosition@{tmp}\layered.dll stdcall delayload';
// устанавливает позицию изображения

procedure lImgGetPosition(img: Img; var left, top, width, height: integer); external 'lImgGetPosition@{tmp}\layered.dll stdcall delayload';
// возвращает позицию изображения

// --- Str --- \\

function lStrLoad(scene: Scene; const caption: string; left, top, width, height: integer; autosize: bool): Str; external 'lStrLoad@{tmp}\layered.dll stdcall delayload';
// загружает надпись

function lStrDelete(str: Str): integer; external 'lStrDelete@{tmp}\layered.dll stdcall delayload';
// удаляет надпись из памяти

procedure lStrSetCaption(str: Str; const caption: string); external 'lStrSetCaption@{tmp}\layered.dll stdcall delayload';
// устанавливает текст надписи

procedure lStrGetCaption_(str: Str; var caption: TArrayOfWChar); external 'lStrGetCaption@{tmp}\layered.dll stdcall delayload';
// возвращает текст надписи

function lStrGetLength(str: Str): integer; external 'lStrLength@{tmp}\layered.dll stdcall delayload';
// вернет длину надписи

procedure lStrSetVisibility(str: Str; value: bool); external 'lStrSetVisibility@{tmp}\layered.dll stdcall delayload';
// устанавливает значение видимости надписи

function lStrGetVisibility(str: Str): bool; external 'lStrGetVisibility@{tmp}\layered.dll stdcall delayload';
// возвращает значение видимости надписи

procedure lStrSetPosition(str: Str; left, top, width, height: integer); external 'lStrSetPosition@{tmp}\layered.dll stdcall delayload';
// устанавливает позицию надписи

procedure lStrGetPosition(str: Str; var left, top, width, height: integer); external 'lStrGetPosition@{tmp}\layered.dll stdcall delayload';
// возвращает позицию надписи

procedure lStrSetBrush(str: Str; brush: Brush); external 'lStrSetBrush@{tmp}\layered.dll stdcall delayload';
// устанавливает кисть для надписи ****

procedure lStrSetFont(str: Str; font: Font); external 'lStrSetFont@{tmp}\layered.dll stdcall delayload';
// устанавливает шрифт для надписи ***

procedure lStrSetAlignment(str: Str; value: integer); external 'lStrSetAlignment@{tmp}\layered.dll stdcall delayload';
// горизонтальное выравнивание текста надписи

procedure lStrSetLineAlignment(str: Str; value: integer); external 'lStrSetLineAlignment@{tmp}\layered.dll stdcall delayload';
// вертикальное выравнивание текста надписи

// --- Примечания --- \\

//========================================================================================================================================================================================================
// *    - цвет задается в формате ARGB
//        пример: $FFFF0000 - красный без прозрачности; $7F00FF00 - зеленый с 50% прозрачностью
//========================================================================================================================================================================================================
// **   - функции можно вызывать только в событии onpaint сцены
//========================================================================================================================================================================================================
// ***  - шрифт созданный библиотекой: Name := "Lucida Console"; Height := 12; Weight := FW_NORMAL; Italic := false; Underline := false; StrikeOut := false; CharSet := ANSI_CHARSET; Quality := DEFAULT_QUALITY;
//        этот шрифт используется по умолчанию
//========================================================================================================================================================================================================
// **** - стандартная кисть: сплошная - $ff000000 (черный без прозрачности)
//        эта кисть используется по умолчанию
//========================================================================================================================================================================================================

// --- Window --- \\

type
  TPos = record
    Left, Top, Width, Height: Integer;
  end;

const
  // стили окон lCreateWindow
  CW_BTNPUSH = $1; // кнопка
  CW_BTNCHECK = $2; // кнопка выбора
  CW_PROGRESS = $4; // индикатор прогресса
  CW_EDIT = $8; // поле ввода

function lCreateWindow(scene: Scene; style: dword; left, top, width, height: integer; const caption: string; texture: Texture): Window; external 'lCreateWindow@{tmp}\layered.dll stdcall delayload';
// создает окно ****

procedure lDestroyWindow(window: Window); external 'lDestroyWindow@{tmp}\layered.dll stdcall delayload';
// удаляет окно из памяти

procedure lSetWindowText(window: Window; const caption: string); external 'lSetWindowText@{tmp}\layered.dll stdcall delayload';
// устанавливает новый заголовок для окна

function lGetWindowTextLength(window: Window): integer; external 'lGetWindowTextLength@{tmp}\layered.dll stdcall delayload';
// вернет длинну заголовка окна

procedure lGetWindowText_(window: Window; var caption: TArrayOfWChar); external 'lGetWindowText@{tmp}\layered.dll stdcall delayload';
// вернет заголовок окна

procedure lSetWindowPos(window: Window; left, top: integer); external 'lSetWindowPos@{tmp}\layered.dll stdcall delayload';
// устанавливает позицию окна

procedure lGetWindowPos_(window: Window; var left, top, width, height: integer); external 'lGetWindowPos@{tmp}\layered.dll stdcall delayload';
// вернет позицию окна

const
  L_FALSE = 0;
  L_TRUE = 1;

  // progress state
  L_NORMAL = 1;
  L_PAUSE = 2;
  L_ERROR = 3;

  // lSetWindowLong index *
  SWL_EVENTONCLICK = 1;  // событие клика мышью
  SWL_EVENTONMOUSELEAVE = 2; // мышь покинула пределы окна
  SWL_EVENTONMOUSEENTER = 3; // мышь вошла в область окна
  SWL_EVENTCHANGE = 13; // вызывается при изменении текста окна

  SWL_BRUSHNORMAL = 4; // кисть для текста в нормальном состоянии *****
  SWL_BRUSHFOCUSED = 5; // кисть, когда окно в фокусе
  SWL_BRUSHDISABLED = 6; // кисть, когда окно недоступно
  SWL_ALIGNMENT = 10;  // выравнивание текста ***
  SWL_LINEALIGNMENT = 11; // вертикальное выравнивание ***
  SWL_FONT = 12; // шрифт

  // lSetWindowLong/lGetWindowLong index
  SWL_ENABLE = 7; // доступность *
  SWL_VISIBILITY = 8; // видимость
  SWL_CHECKED = 9; // отмечена ли галочка (для CW_BTNCHECK) *  **
  SWL_PROGRESSVALUE = 1; // значение прогресса (для CW_PROGRESS)
  SWL_PROGRESSMAX = 2; // равносильно TProgressBar.Max
  SWL_PROGRESSSTATE = 3; // состояние прогрессбара (L_NORMAL, L_PAUSE, L_ERROR)

  // lGetWindowLong index
  GWL_POSX = -1; // координаты и размер окна
  GWL_POSY = -2;
  GWL_POSCX = -3;
  GWL_POSCY = -4;

procedure lSetWindowLong(window: Window; index: integer; newlong: longint); external 'lSetWindowLong@{tmp}\layered.dll stdcall delayload';
// устанавливает дополнительные параметры окна

function lGetWindowLong(window: Window; index: integer): longint; external 'lGetWindowLong@{tmp}\layered.dll stdcall delayload';
// вернет некоторые параметры окна

function lGetWindowPos(window: Window): TPos; // вернет позицию окна
begin
  Result.Left := lGetWindowLong(window, GWL_POSX);
  Result.Top := lGetWindowLong(window, GWL_POSY);
  Result.Width := lGetWindowLong(window, GWL_POSCX);
  Result.Height := lGetWindowLong(window, GWL_POSCY);
end;

procedure lSetWindowVisibility(window: Window; value: bool); // устанавливает значение видимости окна
begin
  if value then
    lSetWindowLong(window, SWL_VISIBILITY, L_TRUE)
  else
    lSetWindowLong(window, SWL_VISIBILITY, L_FALSE);
end;

function lGetWindowVisibility(window: Window): bool; var r: integer; // вернет значение видимости окна
begin
  r := lGetWindowLong(window, SWL_VISIBILITY);
  if r = L_FALSE then RESULT := FALSE else RESULT := TRUE;
end;

procedure lSetWindowEnabled(window: Window; value: bool); // устанавливает доступность окна
begin
  if value then
    lSetWindowLong(window, SWL_ENABLE, L_TRUE)
  else
    lSetWindowLong(window, SWL_ENABLE, L_FALSE);
end;

function lGetWindowEnabled(window: Window): bool; var r: integer; // вернет значение доступности окна
begin
  r := lGetWindowLong(window, SWL_ENABLE);
  if r = L_FALSE then RESULT := FALSE else RESULT := TRUE;
end;

procedure lSetBtnChecked(window: Window; value: bool); // устанавливает/снимает галочку (для CW_BTNCHECK) **
begin
  if value then
    lSetWindowLong(window, SWL_CHECKED, L_TRUE)
  else
    lSetWindowLong(window, SWL_CHECKED, L_FALSE);
end;

function lGetBtnChecked(window: Window): bool; var r: integer; // отмечена ли галочка (для CW_BTNCHECK)
begin
  r := lGetWindowLong(window, SWL_CHECKED);
  if r = L_FALSE then RESULT := FALSE else RESULT := TRUE;
end;

procedure lSetProgressValue(window: Window; value: integer); // устанавливает значение индикатора прогресса
begin
  lSetWindowLong(window, SWL_PROGRESSVALUE, value);
end;

function lGetProgressValue(window: Window): integer; // вернет значение индикатора прогресса
begin
  RESULT := lGetWindowLong(window, SWL_PROGRESSVALUE);
end;

procedure lSetProgressMax(window: Window; value: integer); // устанавливает максимальное значение индикатора прогресса
begin
  lSetWindowLong(window, SWL_PROGRESSMAX, value);
end;

function lGetProgressMax(window: Window): integer; // вернет максимальное значение индикатора прогресса
begin
  RESULT := lGetWindowLong(window, SWL_PROGRESSMAX);
end;

procedure lSetProgressState(window: Window; value: integer); // устанавливает состояние индикатора прогресса
begin
  lSetWindowLong(window, SWL_PROGRESSSTATE, value);
end;

function lGetProgressState(window: Window): integer; // вернет состояние индикатора прогресса
begin
  RESULT := lGetWindowLong(window, SWL_PROGRESSSTATE);
end;

// --- Примечания --- \\

//========================================================================================================================================================================================================
// *    - не действует для окна со стилем CW_PROGRESS
//========================================================================================================================================================================================================
// **   - для CW_BTNPUSH равносильно клику по окну(для любого значения newlong)
//========================================================================================================================================================================================================
// ***  - действует только для CW_BTNPUSH
//========================================================================================================================================================================================================
// **** - текстуру загражать обязательно для каждого стиля окон.
//        заголовок окна не обязателен
//========================================================================================================================================================================================================
// ***** - окно со стилем CW_EDIT поддерживает только сплошную кисть (lSolidBrushCreate)
//========================================================================================================================================================================================================

function ArrayOfWideCharToString(a: TArrayOfWChar): String; // взято из botva2.iss. Спасибо South
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

////////////////////// Начало Taskbar //////////////

const
  TBPF_NOPROGRESS = 0; // нету
  TBPF_INDETERMINATE = 1; // переливается зеленым
  TBPF_NORMAL = 2; // обычный
  TBPF_ERROR = 4; // красный
  TBPF_PAUSED = 8; // желтый

procedure SetupPreview(handle: hwnd); external 'SetupPreview@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarProgressValue(value: integer); external 'SetTaskBarProgressValue@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarProgressState(value: integer); external 'SetTaskBarProgressState@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarTitle(const caption: String); external 'SetTaskBarTitle@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarThumbnailTooltip(const Caption: String); external 'SetTaskBarThumbnailTooltip@{tmp}\layered.dll stdcall delayload';
procedure SetTaskBarOverlayIcon(hIcon: HICON); external 'SetTaskBarOverlayIcon@{tmp}\layered.dll stdcall delayload';

////////////////////// Конец Taskbar ///////////////
