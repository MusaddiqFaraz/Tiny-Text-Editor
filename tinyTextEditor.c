/*** headers ***/
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/
struct termios orig_termios;


/*** terminal ***/
//call this function wherever any library function call is made, this will print the cause of the error and exit the program with int value 1(non-zero value for failed)
void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) die("tcsetattr");
}

void enableRawMode() {
  if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] =0;
  raw.c_cc[VTIME] =1;

  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


/*** init ***/
int main() {
  /* code */
  enableRawMode();


  while (1) {
    char c = '\0';
    if(read(STDIN_FILENO, &c , 1) == -1 && errno != EAGAIN) die("read");
    if(iscntrl(c)) {
      printf("%d\r\n", c );
    } else {
      printf("%d ('%c')\r\n",c,c);
    }
    if(c == 'q') break;
  }
  return 0;
}
