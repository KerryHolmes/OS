#define PRINTS(x) interrupt(33,0,x,0,0)
#define PRINTN(x) interrupt(33,13,x,0,0)
#define SCANS(x)  interrupt(33,1,x,0,0)
#define SCANN(x)  interrupt(33,14,&x,0,0)
#define CLEAR     interrupt(33,12,8,10,0)
#define RUN(x, y) interrupt(33,4,x,y,0)
#define END       interrupt(33,5,0,0,0)
#define READSEC(x, y)   interrupt(33,2, &x, y, 0 );
