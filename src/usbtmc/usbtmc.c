/**
 * @file usbtmc.c
 * @author Huang Dong (dohuang@borgwarner.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-26
 * 
 * @copyright Copyright (c) 2023 MIT
 * 
 */
/********************************************************************/
/*                Read and Write to a USBTMC Instrument             */
/*                                                                  */
/* This code demonstrates sending synchronous read & write commands */
/* to an USB Test & Measurement Class (USBTMC) instrument using     */
/* NI-VISA                                                          */
/* The example writes the "*IDN?\n" string to all the USBTMC        */
/* devices connected to the system and attempts to read back        */
/* results using the write and read functions.                      */
/*                                                                  */
/* The general flow of the code is                                  */
/*    Open Resource Manager                                         */
/*    Open VISA Session to an Instrument                            */
/*    Write the Identification Query Using viWrite                  */
/*    Try to Read a Response With viRead                            */
/*    Close the VISA Session                                        */
/********************************************************************/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
/* Functions like strcpy are technically not secure because they do */
/* not contain a 'length'. But we disable this warning for the VISA */
/* examples since we never copy more than the actual buffer size.   */
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "visa.h"
#include "minilogger.h"

static ViSession defaultRM;
static ViSession instr;
static ViUInt32 numInstrs;
static ViFindList findList;
static ViUInt32 retCount;
static ViUInt32 writeCount;
static ViStatus status;
static char instrResourceString[VI_FIND_BUFLEN];

static unsigned char buffer[100];
static char stringinput[512];

/**
 * @brief Deprecated. This func control an ITECH DC power's output.
 * 
 * @param state Output state.
 *              0: close;
 *              1: open.
 * @return int 
 */
int ItechDcPowerOutput(unsigned char state)
{
    int i;

    /*
     * First we must call viOpenDefaultRM to get the manager
     * handle.  We will store this handle in defaultRM.
     */
    FileLoggerInit("capldlllog");
    status = viOpenDefaultRM(&defaultRM);
    if (status < VI_SUCCESS)
    {
        LOG_ERROR("Could not open a session to the VISA Resource Manager!");
        exit(EXIT_FAILURE);
    }

    /* Find all the USB TMC VISA resources in our system and store the  */
    /* number of resources in the system in numInstrs.                  */
    status = viFindRsrc(defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);

    if (status < VI_SUCCESS)
    {
        LOG_ERROR("An error occurred while finding resources.");
        viClose(defaultRM);
        return status;
    }

    /*
     * Now we will open VISA sessions to all USB TMC instruments.
     * We must use the handle from viOpenDefaultRM and we must
     * also use a string that indicates which instrument to open.  This
     * is called the instrument descriptor.  The format for this string
     * can be found in the function panel by right clicking on the
     * descriptor parameter. After opening a session to the
     * device, we will get a handle to the instrument which we
     * will use in later VISA functions.  The AccessMode and Timeout
     * parameters in this function are reserved for future
     * functionality.  These two parameters are given the value VI_NULL.
     */

    for (i = 0; i < numInstrs; i++)
    {
        if (i > 0)
            viFindNext(findList, instrResourceString);

        status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);
        LOG_INFO("%s",instrResourceString);

        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Cannot open a session to the device %d.", i + 1);
            continue;
        }

        /*
         * At this point we now have a session open to the USB TMC instrument.
         * We will now use the viWrite function to send the device the string "*IDN?\n",
         * asking for the device's identification.
         */
        strcpy(stringinput, "*IDN?\n");
        status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error writing to the device %d.", i + 1);
            status = viClose(instr);
            continue;
        }

        /*
         * Now we will attempt to read back a response from the device to
         * the identification query that was sent.  We will use the viRead
         * function to acquire the data.  We will try to read back 100 bytes.
         * This function will stop reading if it finds the termination character
         * before it reads 100 bytes.
         * After the data has been read the response is displayed.
         */
        status = viRead(instr, buffer, 100, &retCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error reading a response from the device %d.", i + 1);
        }
        else
        {
            LOG_INFO("Device %d: %*s", i + 1, retCount, buffer);
        }

        if(state)
        strcpy(stringinput, "OUTP 1\n");
        else
        strcpy(stringinput, "OUTP 0\n");

        status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error writing to the device %d.", i + 1);
            status = viClose(instr);
            continue;
        }

        status = viClose(instr);
    }

    /*
     * Now we will close the session to the instrument using
     * viClose. This operation frees all system resources.
     */
    status = viClose(defaultRM);

    return 0;
}

/**
 * @brief Write a standard SIPC command to an ITECH DC power supply.
 * 
 * @param command SIPC command string.
 * @return int 
 */
int ItechDcPowerWrite(char* command)
{
    int i;

    /*
     * First we must call viOpenDefaultRM to get the manager
     * handle.  We will store this handle in defaultRM.
     */
    FileLoggerInit("capldlllog");
    status = viOpenDefaultRM(&defaultRM);
    if (status < VI_SUCCESS)
    {
        LOG_ERROR("Could not open a session to the VISA Resource Manager!");
        exit(EXIT_FAILURE);
    }

    /* Find all the USB TMC VISA resources in our system and store the  */
    /* number of resources in the system in numInstrs.                  */
    status = viFindRsrc(defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);

    if (status < VI_SUCCESS)
    {
        LOG_ERROR("An error occurred while finding resources.");
        viClose(defaultRM);
        return status;
    }

    /*
     * Now we will open VISA sessions to all USB TMC instruments.
     * We must use the handle from viOpenDefaultRM and we must
     * also use a string that indicates which instrument to open.  This
     * is called the instrument descriptor.  The format for this string
     * can be found in the function panel by right clicking on the
     * descriptor parameter. After opening a session to the
     * device, we will get a handle to the instrument which we
     * will use in later VISA functions.  The AccessMode and Timeout
     * parameters in this function are reserved for future
     * functionality.  These two parameters are given the value VI_NULL.
     */

    for (i = 0; i < numInstrs; i++)
    {
        if (i > 0)
            viFindNext(findList, instrResourceString);

        status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);
        LOG_INFO("%s",instrResourceString);

        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Cannot open a session to the device %d.", i + 1);
            continue;
        }

        /*
         * At this point we now have a session open to the USB TMC instrument.
         * We will now use the viWrite function to send the device the string "*IDN?\n",
         * asking for the device's identification.
         */
        strcpy(stringinput, "*IDN?\n");
        status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error writing to the device %d.", i + 1);
            status = viClose(instr);
            continue;
        }

        /*
         * Now we will attempt to read back a response from the device to
         * the identification query that was sent.  We will use the viRead
         * function to acquire the data.  We will try to read back 100 bytes.
         * This function will stop reading if it finds the termination character
         * before it reads 100 bytes.
         * After the data has been read the response is displayed.
         */
        status = viRead(instr, buffer, 100, &retCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error reading a response from the device %d.", i + 1);
        }
        else
        {
            LOG_INFO("Device %d: %*s", i + 1, retCount, buffer);
        }

        strcpy(stringinput, command);

        status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error writing to the device %d.", i + 1);
            status = viClose(instr);
            continue;
        }

        status = viClose(instr);
    }

    /*
     * Now we will close the session to the instrument using
     * viClose. This operation frees all system resources.
     */
    status = viClose(defaultRM);

    return 0;
}

/**
 * @brief Write a query command to an ITECH DC power supply and read back its reply.
 * 
 * @param command SIPC query command string.
 * @param resultString Buffer to save a reply string.
 * @param result Numberic reply will also be saved in this buffer.
 * @return int 
 */
int ItechDcPowerQuery(char* command, char *resultString,  double *result)
{
    int i;

    /*
     * First we must call viOpenDefaultRM to get the manager
     * handle.  We will store this handle in defaultRM.
     */
    FileLoggerInit("capldlllog");
    status = viOpenDefaultRM(&defaultRM);
    if (status < VI_SUCCESS)
    {
        LOG_ERROR("Could not open a session to the VISA Resource Manager!");
        exit(EXIT_FAILURE);
    }

    /* Find all the USB TMC VISA resources in our system and store the  */
    /* number of resources in the system in numInstrs.                  */
    status = viFindRsrc(defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);

    if (status < VI_SUCCESS)
    {
        LOG_ERROR("An error occurred while finding resources.");
        viClose(defaultRM);
        return status;
    }

    /*
     * Now we will open VISA sessions to all USB TMC instruments.
     * We must use the handle from viOpenDefaultRM and we must
     * also use a string that indicates which instrument to open.  This
     * is called the instrument descriptor.  The format for this string
     * can be found in the function panel by right clicking on the
     * descriptor parameter. After opening a session to the
     * device, we will get a handle to the instrument which we
     * will use in later VISA functions.  The AccessMode and Timeout
     * parameters in this function are reserved for future
     * functionality.  These two parameters are given the value VI_NULL.
     */

    for (i = 0; i < numInstrs; i++)
    {
        if (i > 0)
            viFindNext(findList, instrResourceString);

        status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);
        LOG_INFO("%s",instrResourceString);

        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Cannot open a session to the device %d.", i + 1);
            continue;
        }

        /*
         * At this point we now have a session open to the USB TMC instrument.
         * We will now use the viWrite function to send the device the string "*IDN?\n",
         * asking for the device's identification.
         */
        strcpy(stringinput, "*IDN?\n");
        status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error writing to the device %d.", i + 1);
            status = viClose(instr);
            continue;
        }

        /*
         * Now we will attempt to read back a response from the device to
         * the identification query that was sent.  We will use the viRead
         * function to acquire the data.  We will try to read back 100 bytes.
         * This function will stop reading if it finds the termination character
         * before it reads 100 bytes.
         * After the data has been read the response is displayed.
         */
        status = viRead(instr, buffer, 100, &retCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error reading a response from the device %d.", i + 1);
        }
        else
        {
            LOG_INFO("Device %d: %*s", i + 1, retCount, buffer);
        }

        strcpy(stringinput, command);

        status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error writing to the device %d.", i + 1);
            status = viClose(instr);
            continue;
        }

        status = viRead(instr, buffer, 100, &retCount);
        if (status < VI_SUCCESS)
        {
            LOG_ERROR("Error reading a response from the device %d.", i + 1);
        }
        else
        {
            sscanf(buffer,"%lf",result);
            LOG_INFO("Measured value: %lf",result);
            strncpy(resultString, buffer,retCount);
        }
        status = viClose(instr);
    }

    /*
     * Now we will close the session to the instrument using
     * viClose. This operation frees all system resources.
     */
    status = viClose(defaultRM);

    return 0;
}