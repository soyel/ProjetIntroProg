#ifndef GETCH_TOOL_H_INCLUDED
#define GETCH_TOOL_H_INCLUDED

void initTermios(int echo);
void resetTermios(void);
char getch();

#endif
