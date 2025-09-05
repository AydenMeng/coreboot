void copy_mem(int);
int GetExceptionType (void);

int GetInterruptType (void);

void DumpCpuContext (int);
void DumpImageAndCpuContent (int);
void IpiInterruptHandler (int);
void CommonExceptionHandler(int);
