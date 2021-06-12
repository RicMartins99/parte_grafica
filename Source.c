#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"
#include <wingdi.h>


#define botao_adiciona 1
#define botao_suspende 2
#define botao_ativa 3
/* ===================================================== */
/* Programa base (esqueleto) para aplica��es Windows     */
/* ===================================================== */
// Cria uma janela de nome "Janela Principal" e pinta fundo de branco
// Modelo para programas Windows:
//  Composto por 2 fun��es: 
//	WinMain()     = Ponto de entrada dos programas windows
//			1) Define, cria e mostra a janela
//			2) Loop de recep��o de mensagens provenientes do Windows
//     TrataEventos()= Processamentos da janela (pode ter outro nome)
//			1) � chamada pelo Windows (callback) 
//			2) Executa c�digo em fun��o da mensagem recebida

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);

// Nome da classe da janela (para programas de uma s� janela, normalmente este nome � 
// igual ao do pr�prio programa) "szprogName" � usado mais abaixo na defini��o das 
// propriedades do objecto janela
TCHAR szProgName[] = TEXT("Base");

HWND button_adiciona, button_suspende,button_ativa;

// ============================================================================
// FUN��O DE IN�CIO DO PROGRAMA: WinMain()
// ============================================================================
// Em Windows, o programa come�a sempre a sua execu��o na fun��o WinMain()que desempenha
// o papel da fun��o main() do C em modo consola WINAPI indica o "tipo da fun��o" (WINAPI
// para todas as declaradas nos headers do Windows e CALLBACK para as fun��es de
// processamento da janela)
// Par�metros:
//   hInst: Gerado pelo Windows, � o handle (n�mero) da inst�ncia deste programa 
//   hPrevInst: Gerado pelo Windows, � sempre NULL para o NT (era usado no Windows 3.1)
//   lpCmdLine: Gerado pelo Windows, � um ponteiro para uma string terminada por 0
//              destinada a conter par�metros para o programa 
//   nCmdShow:  Par�metro que especifica o modo de exibi��o da janela (usado em  
//        	   ShowWindow()

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;		// hWnd � o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;		// MSG � uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX � uma estrutura cujos membros servem para 
			  // definir as caracter�sticas da classe da janela

	// ============================================================================
	// 1. Defini��o das caracter�sticas da janela "wcApp" 
	//    (Valores dos elementos da estrutura "wcApp" do tipo WNDCLASSEX)
	// ============================================================================
	wcApp.cbSize = sizeof(WNDCLASSEX);      // Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;		         // Inst�ncia da janela actualmente exibida 
								   // ("hInst" � par�metro de WinMain e vem 
										 // inicializada da�)
	wcApp.lpszClassName = szProgName;       // Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = TrataEventos;       // Endere�o da fun��o de processamento da janela
											// ("TrataEventos" foi declarada no in�cio e
											// encontra-se mais abaixo)
	wcApp.style = CS_HREDRAW | CS_VREDRAW;  // Estilo da janela: Fazer o redraw se for
											// modificada horizontal ou verticalmente

	wcApp.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON2));   // "hIcon" = handler do �con normal
										   // "NULL" = Icon definido no Windows
										   // "IDI_AP..." �cone "aplica��o"
	wcApp.hIconSm = NULL;				 // "hIconSm" = handler do �con pequeno
										   // "NULL" = Icon definido no Windows
										   // "IDI_INF..." �con de informa��o
	wcApp.hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));	// "hCursor" = handler do cursor (rato) 
							  // "NULL" = Forma definida no Windows
							  // "IDC_ARROW" Aspecto "seta" 
	wcApp.lpszMenuName = (TCHAR *)IDR_MENU1;			// Classe do menu que a janela pode ter
							  // (NULL = n�o tem menu)
	wcApp.cbClsExtra = 0;				// Livre, para uso particular
	wcApp.cbWndExtra = 0;				// Livre, para uso particular
	wcApp.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	// "hbrBackground" = handler para "brush" de pintura do fundo da janela. Devolvido por
	// "GetStockObject".Neste caso o fundo ser� branco

	// ============================================================================
	// 2. Registar a classe "wcApp" no Windows
	// ============================================================================
	if (!RegisterClassEx(&wcApp))
		return(0);

	// ============================================================================
	// 3. Criar a janela
	// ============================================================================
	hWnd = CreateWindow(
		szProgName,			// Nome da janela (programa) definido acima
		TEXT("Exemplo de Janela Principal em C"),// Texto que figura na barra do t�tulo
		WS_OVERLAPPEDWINDOW,	// Estilo da janela (WS_OVERLAPPED= normal)
		CW_USEDEFAULT,		// Posi��o x pixels (default=� direita da �ltima)
		CW_USEDEFAULT,		// Posi��o y pixels (default=abaixo da �ltima)
		CW_USEDEFAULT,		// Largura da janela (em pixels)
		CW_USEDEFAULT,		// Altura da janela (em pixels)
		(HWND)HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir de
						// outra) ou HWND_DESKTOP se a janela for a primeira, 
						// criada a partir do "desktop"
		(HMENU)NULL,			// handle do menu da janela (se tiver menu)
		(HINSTANCE)hInst,		// handle da inst�ncia do programa actual ("hInst" � 
						// passado num dos par�metros de WinMain()
		0);				// N�o h� par�metros adicionais para a janela
	  // ============================================================================
	  // 4. Mostrar a janela
	  // ============================================================================
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido por 
					  // "CreateWindow"; "nCmdShow"= modo de exibi��o (p.e. 
					  // normal/modal); � passado como par�metro de WinMain()
	UpdateWindow(hWnd);		// Refrescar a janela (Windows envia � janela uma 
					  // mensagem para pintar, mostrar dados, (refrescar)� 
	// ============================================================================
	// 5. Loop de Mensagens
	// ============================================================================
	// O Windows envia mensagens �s janelas (programas). Estas mensagens ficam numa fila de
	// espera at� que GetMessage(...) possa ler "a mensagem seguinte"	
	// Par�metros de "getMessage":
	// 1)"&lpMsg"=Endere�o de uma estrutura do tipo MSG ("MSG lpMsg" ja foi declarada no  
	//   in�cio de WinMain()):
	//			HWND hwnd		handler da janela a que se destina a mensagem
	//			UINT message		Identificador da mensagem
	//			WPARAM wParam		Par�metro, p.e. c�digo da tecla premida
	//			LPARAM lParam		Par�metro, p.e. se ALT tamb�m estava premida
	//			DWORD time		Hora a que a mensagem foi enviada pelo Windows
	//			POINT pt		Localiza��o do mouse (x, y) 
	// 2)handle da window para a qual se pretendem receber mensagens (=NULL se se pretendem
	//   receber as mensagens para todas as
	// janelas pertencentes � thread actual)
	// 3)C�digo limite inferior das mensagens que se pretendem receber
	// 4)C�digo limite superior das mensagens que se pretendem receber

	// NOTA: GetMessage() devolve 0 quando for recebida a mensagem de fecho da janela,
	// 	  terminando ent�o o loop de recep��o de mensagens, e o programa 
	HACCEL tabela = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, tabela, &lpMsg)) {
			TranslateMessage(&lpMsg);	// Pr�-processamento da mensagem (p.e. obter c�digo 
						   // ASCII da tecla premida)
			DispatchMessage(&lpMsg);	// Enviar a mensagem traduzida de volta ao Windows, que
						   // aguarda at� que a possa reenviar � fun��o de 
						   // tratamento da janela, CALLBACK TrataEventos (abaixo)
		}
	}

	// ============================================================================
	// 6. Fim do programa
	// ============================================================================
	return((int)lpMsg.wParam);	// Retorna sempre o par�metro wParam da estrutura lpMsg
}

// ============================================================================
// FUN��O DE PROCESSAMENTO DA JANELA
// Esta fun��o pode ter um nome qualquer: Apenas � neces�rio que na inicializa��o da
// estrutura "wcApp", feita no in�cio de // WinMain(), se identifique essa fun��o. Neste
// caso "wcApp.lpfnWndProc = WndProc"
//
// WndProc recebe as mensagens enviadas pelo Windows (depois de lidas e pr�-processadas
// no loop "while" da fun��o WinMain()
// Par�metros:
//		hWnd	O handler da janela, obtido no CreateWindow()
//		messg	Ponteiro para a estrutura mensagem (ver estrutura em 5. Loop...
//		wParam	O par�metro wParam da estrutura messg (a mensagem)
//		lParam	O par�metro lParam desta mesma estrutura
//
// NOTA:Estes par�metros est�o aqui acess�veis o que simplifica o acesso aos seus valores
//
// A fun��o EndProc � sempre do tipo "switch..." com "cases" que descriminam a mensagem
// recebida e a tratar.
// Estas mensagens s�o identificadas por constantes (p.e. 
// WM_DESTROY, WM_CHAR, WM_KEYDOWN, WM_PAINT...) definidas em windows.h
// ============================================================================
BOOL CALLBACK TrataEventosCaixa(HWND h, UINT eve, WPARAM w, LPARAM l) {
	switch (eve) {

	case WM_CLOSE:
		EndDialog(h, 0);

		break;

	case WM_COMMAND:
		if (LOWORD(w) == IDCLOSE || LOWORD(w) == IDCANCEL)
		{
			EndDialog(h, LOWORD(w));
		}
		break;

		if (LOWORD(w) == IDOK)
		{
			
		}

	}
	return FALSE;
}


LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, aux;
	static HDC ddc;
    static TCHAR letra = ' ';
	static int maxX, maxY;
	 int x = 0, i;
	 int altura = 600, largura = 600;
	static TCHAR texto[100] =  TEXT("");
	static HBITMAP bmp = NULL, foto = NULL;
	PAINTSTRUCT paint;
	int cmd_suspende1 = 1;
	int cmd_ativa1 = 0;
	switch (messg) {

	case WM_CREATE:

		bmp = LoadBitmap((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));

		button_adiciona = CreateWindow(_T("BUTTON"), _T("Adiciona Aeroporto"), WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 600, 200, 20, hWnd, (HMENU)1, NULL, NULL);
		button_suspende= CreateWindow(_T("BUTTON"), _T("Aceita Novos Passageiros - Suspende"), WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 600, 200, 20, hWnd, (HMENU)2, NULL, NULL);
		button_ativa= CreateWindow(_T("BUTTON"), _T("Aceita Novos Passageiros - Ativa"), WS_VISIBLE | WS_CHILD | WS_BORDER, 600, 600, 200, 20, hWnd, (HMENU)3, NULL, NULL);


		hdc = GetDC(hWnd);
		maxX = GetSystemMetrics(SM_CXSCREEN);
		maxX = GetSystemMetrics(SM_CYSCREEN);

		ddc = CreateCompatibleDC(hdc);

		foto = CreateCompatibleBitmap(hdc, maxX, maxY);
		SelectObject(ddc, foto);

		SelectObject(ddc, GetStockObject(GRAY_BRUSH));
		PatBlt(ddc, 0, 0, maxX, maxY, PATCOPY);
		ReleaseDC(hWnd, hdc);
		DeleteObject(foto);




		break;
		//novos eventos
	/*case WM_CHAR:
		if (wParam >= 32 && wParam <= 154) {
			letra = wParam;
			_stprintf_s(texto, 100, TEXT("%s%c"), texto, letra);
			MessageBox(hWnd, texto, _T("Evento da Tecla"), MB_OK);

		}
		break;
*/
	case WM_KEYDOWN:

		if (wParam == VK_BACK)
			MessageBox(hWnd, TEXT("Backspace premido"), TEXT("Evento KEYDOWN"), MB_OK);

		break;
/*
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		SetBkMode(hdc, TRANSPARENT);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		TextOut(hdc, LOWORD(lParam), HIWORD(lParam), &letra, 1);
		
		ReleaseDC(hWnd, hdc);
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &paint);

		SetBkMode(hdc, TRANSPARENT);
		
		TextOut(hdc, x, y, &letra, 1);

		EndPaint(hWnd, &paint);

		break;*/
	case WM_COMMAND:

		if (LOWORD(wParam) == botao_adiciona) {
			//MessageBox(hWnd, _T("Tem a certeza?"), _T("Titulo"), MB_YESNO);
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, TrataEventosCaixa);
		}
		if (LOWORD(wParam) == botao_suspende) {
			
			if (cmd_suspende1 == 1) { //esta ativo
				MessageBox(hWnd, TEXT("ACEITA NOVOS PASSAGEIROS [SUSPENSO]"), TEXT("SUSPENDE"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("SUSPENDIDO ANTERIORMENTE [SUSPENSO]"), TEXT("SUSPENDE"), MB_OK);

		}
		
		if(LOWORD(wParam) == botao_ativa){

			if(cmd_ativa1==0){  //esta suspenso
				MessageBox(hWnd, TEXT("ACEITA NOVOS AVIOES [ATIVO]"), TEXT("ATIVA"), MB_OK);
			}
			else
				MessageBox(hWnd, TEXT("ACEITE ANTERIORMENTE [ATIVO]"), TEXT("ATIVA"), MB_OK);
			
		}

		if (LOWORD(wParam == ID_SOBRE)) {
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, TrataEventosCaixa);
		}

		if (LOWORD(wParam == ID_FICHEIRO_SAIR)) {
			DestroyWindow(hWnd);
		}
		

		break;
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &paint);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 10, 10, _T("Lembram-se disto?"), _tcslen(_T("Lembram-se disto?")));
		aux = CreateCompatibleDC(hdc);
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		Rectangle(hdc, 200, 200, 300, 300);
		//SelectObject(aux, bmp);

		
		//TransparentBlt(hdc, x, 10, 134, 134, aux, 0, 0, 134, 134, RGB(255, 255, 255));

		SelectObject(aux, bmp);
		BitBlt(hdc, 700, 50, 134, 134, aux, 0, 0, SRCCOPY);
		
		DeleteDC(aux);

		EndPaint(hdc, &paint);
		break;
	case WM_DESTROY:
		
		DeleteObject(bmp);
		PostQuitMessage(0);
		break;

	case WM_CLOSE:	// Destruir a janela e terminar o programa 
						// "PostQuitMessage(Exit Status)"	

		//hdc = GetDC(hWnd);
		//SetBkMode(hdc, TRANSPARENT);
		//TextOut(hdc, 10, 10, _T("Bem-Vindo"), _tcslen(_T("Bem-Vindo")));
		//TextOut(hdc, 50, 50, &letra, 1);
		//TextOut(hdc, 100, 100, texto, _tcslen(texto));
		//ReleaseDC(hWnd, hdc);

		DeleteDC(ddc);

		if(MessageBox(hWnd, _T("Tem a certeza?"), _T("Titulo"), MB_YESNO) == IDYES)
			PostQuitMessage(0);
		break;
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar","maximizar","restaurar")
		// n�o � efectuado nenhum processamento, apenas se segue o "default" do Windows
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;  // break tecnicamente desnecess�rio por causa do return
	}
	return(0);
}