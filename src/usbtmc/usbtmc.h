#ifndef USBTMC_H
#define USBTMC_H
#ifdef __cplusplus
extern "C" {
#endif
int ItechDcPowerOutput(unsigned char state);
int ItechDcPowerWrite(char* command);
int ItechDcPowerQuery(char* command, char *resultString,  double *result);
#ifdef __cplusplus
}
#endif
#endif