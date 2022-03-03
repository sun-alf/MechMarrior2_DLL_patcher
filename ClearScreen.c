#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>

void ClearScreen()
{
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
}

void WaitAnyKey()
{
	system("pause");
}

#else // !_WIN32
#include <unistd.h>
#include <curses.h>
#include <term.h>

void ClearScreen()
{
  if (!cur_term)
  {
     int result;
     setupterm( NULL, STDOUT_FILENO, &result );
     if (result <= 0) return;
  }

   putp( tigetstr( "clear" ) );
}

void WaitAnyKey()
{
	struct termios term;
	char buf;
	tcflag_t old_lflag;

	printf("\nPress any key to continue ...\n");
	
	tcgetattr (0, &term);
	old_lflag = term.c_lflag;
	term.c_lflag &= ~(ECHO | ICANON);	
	tcsetattr (0, TCSANOW, &term);		
	read (0, &buf, 1);
	term.c_lflag = old_lflag;
	tcsetattr (0, TCSANOW, &term);
}

#endif  // ifdef _WIN32
