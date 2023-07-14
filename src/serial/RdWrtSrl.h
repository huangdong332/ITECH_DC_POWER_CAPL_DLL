
#ifndef RDWRTSRL_H
#define URDWRTSRL_H
#ifdef __cplusplus
extern "C" {
#endif
int ItechDcPowerWriteSerial(char* command);
int ItechDcPowerQuerySerial(char* command, char *resultString,  double *result);
#ifdef __cplusplus
}
#endif
#endif