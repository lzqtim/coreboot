/**
 * @file
 *
 * Install of build option: Message-Based C1e
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
 * @e \$Revision: 44323 $   @e \$Date: 2010-12-22 01:24:58 -0700 (Wed, 22 Dec 2010) $
 */
/*****************************************************************************
 *
 * Copyright (c) 2011, Advanced Micro Devices, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Advanced Micro Devices, Inc. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 ***************************************************************************/

#ifndef _OPTION_MSG_BASED_C1E_INSTALL_H_
#define _OPTION_MSG_BASED_C1E_INSTALL_H_

#include "cpuMsgBasedC1e.h"

/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */
#define OPTION_MSG_BASED_C1E_FEAT
#define F10_MSG_BASED_C1E_SUPPORT
#if OPTION_MSG_BASED_C1E == TRUE
  #if (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_RESUME == TRUE)
    #ifdef OPTION_FAMILY10H
      #if OPTION_FAMILY10H == TRUE
        #if OPTION_FAMILY10H_HY == TRUE
          extern CONST CPU_FEATURE_DESCRIPTOR ROMDATA CpuFeatureMsgBasedC1e;
          #undef OPTION_MSG_BASED_C1E_FEAT
          #define OPTION_MSG_BASED_C1E_FEAT &CpuFeatureMsgBasedC1e,
          extern CONST MSG_BASED_C1E_FAMILY_SERVICES ROMDATA F10MsgBasedC1e;
          #undef F10_MSG_BASED_C1E_SUPPORT
          #define F10_MSG_BASED_C1E_SUPPORT {AMD_FAMILY_10_HY, &F10MsgBasedC1e},
        #endif
      #endif
    #endif
    CONST CPU_SPECIFIC_SERVICES_XLAT ROMDATA MsgBasedC1eFamilyServiceArray[] =
    {
      F10_MSG_BASED_C1E_SUPPORT
      {0, NULL}
    };
    CONST CPU_FAMILY_SUPPORT_TABLE ROMDATA MsgBasedC1eFamilyServiceTable =
    {
      (sizeof (MsgBasedC1eFamilyServiceArray) / sizeof (CPU_SPECIFIC_SERVICES_XLAT)),
      &MsgBasedC1eFamilyServiceArray[0]
    };
  #endif
#endif

#endif  // _OPTION_MSG_BASED_C1E_INSTALL_H_
