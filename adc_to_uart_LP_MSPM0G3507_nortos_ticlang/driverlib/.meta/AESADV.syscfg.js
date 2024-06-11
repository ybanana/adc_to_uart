/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 *  ======== AESADV.syscfg.js ========
 */
"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");

/* generic configuration parameters for AESADV instances */
let aesadvConfig = []; /* nothing (yet) beyond generic driver configs */

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    return;
}


/*
 *  ======== base ========
 *  Define the base AESADV properties and methods
 */
let base = {
    displayName: "AESADV",
    description: "AESADV Module for MSPM0",
    longDescription: `
The **Advanced Encryption Standard (AESADV) accelerator module** accelerator
module performs encryption and decryption of 128-bit data blocks with a 128-bit
or 256-bit key in hardware according to the advanced encryption standard
specified in FIPS PUB 197.

Under *Basic Configuration* users can:
- Configure AESADV key length
- Configure the operation type
- Configure cipher mode

Check the datasheet of your selected device for more detailed information.
    `,
    defaultInstanceName: "AESADV",
    initPriority: -1,
    uiAdd: "staticOnly",

    moduleStatic: {
        config: aesadvConfig,
    },

    /* override device-specific templates */
    templates: {
        boardc: "/ti/driverlib/aesadv/AESADV.Board.c.xdt",
        boardh: "/ti/driverlib/aesadv/AESADV.Board.h.xdt",
        Call: true,
        Reset: true,
        Power: true,
        GPIO: false,
        Function: true,
    },

    _getPinResources: _getPinResources // Ignore
};

let deviceAESADV = system.getScript("/ti/driverlib/aesadv/AESADVMSPM0");

exports = deviceAESADV.extend(base);
