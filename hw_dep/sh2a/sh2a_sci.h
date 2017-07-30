//
// Copyright (c) 2017, Kentaro Sekimoto
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  -Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//  -Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef SH2A_SCI_H_
#define SH2A_SCI_H_

#ifdef __cplusplus
extern "C" {
#endif

struct st_scif { /* struct SCIF  */
    union { /* SCSMR        */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short :8; /*              */
            unsigned short CA :1; /*   C/A        */
            unsigned short CHR :1; /*   CHR        */
            unsigned short PE :1; /*   PE         */
            unsigned short OE :1; /*   O/E        */
            unsigned short STOP :1; /*   STOP       */
            unsigned short :1; /*              */
            unsigned short CKS :2; /*   CKS        */
        } BIT; /*              */
    } SCSMR; /*              */
    unsigned char wk0[2]; /*              */
    union { /* SCBRR        */
        unsigned char BYTE; /*  Byte Access */
        struct { /*  Bit Access  */
            unsigned char D :8; /*   D          */
        } BIT; /*              */
    } SCBRR; /*              */
    unsigned char wk1[3]; /*              */
    union { /* SCSCR        */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short :8; /*              */
            unsigned short TIE :1; /*   TIE        */
            unsigned short RIE :1; /*   RIE        */
            unsigned short TE :1; /*   TE         */
            unsigned short RE :1; /*   RE         */
            unsigned short REIE :1; /*   REIE       */
            unsigned short :1; /*              */
            unsigned short CKE :2; /*   CKE        */
        } BIT; /*              */
    } SCSCR; /*              */
    unsigned char wk2[2]; /*              */
    union { /* SCFTDR       */
        unsigned char BYTE; /*  Byte Access */
        struct { /*  Bit Access  */
            unsigned char D :8; /*   D          */
        } BIT; /*              */
    } SCFTDR; /*              */
    unsigned char wk3[3]; /*              */
    union { /* SCFSR        */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short PERN :4; /*   PERN       */
            unsigned short FERN :4; /*   FERN       */
            unsigned short ER :1; /*   ER         */
            unsigned short TEND :1; /*   TEND       */
            unsigned short TDFE :1; /*   TDFE       */
            unsigned short BRK :1; /*   BRK        */
            unsigned short FER :1; /*   FER        */
            unsigned short PER :1; /*   PER        */
            unsigned short RDF :1; /*   RDF        */
            unsigned short DR :1; /*   DR         */
        } BIT; /*              */
    } SCFSR; /*              */
    unsigned char wk4[2]; /*              */
    union { /* SCFRDR       */
        unsigned char BYTE; /*  Byte Access */
        struct { /*  Bit Access  */
            unsigned char D :8; /*   D          */
        } BIT; /*              */
    } SCFRDR; /*              */
    unsigned char wk5[3]; /*              */
    union { /* SCFCR        */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short :5; /*              */
            unsigned short RSTRG :3; /*   RSTRG      */
            unsigned short RTRG :2; /*   RTRG       */
            unsigned short TTRG :2; /*   TTRG       */
            unsigned short MCE :1; /*   MCE        */
            unsigned short TFRST :1; /*   TFRST      */
            unsigned short RFRST :1; /*   RFRST      */
            unsigned short LOOP :1; /*   LOOP       */
        } BIT; /*              */
    } SCFCR; /*              */
    unsigned char wk6[2]; /*              */
    union { /* SCFDR        */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short :3; /*              */
            unsigned short T :5; /*   T          */
            unsigned short :3; /*              */
            unsigned short R :5; /*   R          */
        } BIT; /*              */
    } SCFDR; /*              */
    unsigned char wk7[2]; /*              */
    union { /* SCSPTR       */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short :12; /*              */
            unsigned short SCKIO :1; /*   SCKIO      */
            unsigned short SCKDT :1; /*   SCKDT      */
            unsigned short SPB2IO :1; /*   SPB2IO     */
            unsigned short SPB2DT :1; /*   SPB2DT     */
        } BIT; /*              */
    } SCSPTR; /*              */
    unsigned char wk8[2]; /*              */
    union { /* SCLSR        */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short :15; /*              */
            unsigned short ORER :1; /*   ORER       */
        } BIT; /*              */
    } SCLSR; /*              */
    unsigned char wk9[2]; /*              */
    union { /* SCEMR        */
        unsigned short WORD; /*  Word Access */
        struct { /*  Bit Access  */
            unsigned short :8; /*              */
            unsigned short BGDM :1; /*   BGDM       */
            unsigned short :6; /*              */
            unsigned short ABCS :1; /*   ABCS       */
        } BIT; /*              */
    } SCEMR; /*              */
};
/*              */

#define SH7262_SCI_TX_PRIORITY 6
#define SH7262_SCI_RX_PRIORITY 5

void SCI_Init(int ch, int baud);
unsigned char SCI_Rx(int ch);
void SCI_Tx(int ch, unsigned char c);
void SCI_TxStr(int ch, unsigned char *p);

#ifdef __cplusplus
}
#endif

#endif /* SH2A_SCI_H_ */
