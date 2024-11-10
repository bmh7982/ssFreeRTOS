;-------------------------------------------------------------------------------
; This file is used for configure option setting, Area 1/2/3.
;-------------------------------------------------------------------------------
;// <<< Use Configuration Wizard in Context Menu >>>
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2008 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e> Configure Option Page 1
CONFIGURE_OPTION_1         EQU      1

;// <h> Read Protection Configuration
;//    <o.0..2> Read Protection
;//       <3=> Level 0 <2=> Level 1 <0=> Level 2
;// </h>
RPCNFIG                    EQU      0x69C8A273

;// <h> Extra User Data
;//   <o0> Word 0 <0x0000-0xFFFF>
;//   <o1> Word 1 <0x0000-0xFFFF>
;// </h>
EX0CNFIG                   EQU      0xFFFF
EX1CNFIG                   EQU      0xFFFF
nEX0CNFIG                  EQU      EX0CNFIG:EOR:0xFFFF
nEX1CNFIG                  EQU      EX1CNFIG:EOR:0xFFFF

;// <h> Watch-Dog Timer Configuration
;//    <o.4..15> WDTRC Enable Selection
;//       <0x96D=> By S/W (CLKSRCR Register) <0x2A7=> Enable but disable at deep sleep <0xFFF=> Always Enable
;//    <o.2> Watch-Dog Timer Clock Selection
;//       <0=> By S/W (PPCLKSR Register) <1=> WDTRC
;//    <o.1> Watch-Dog Timer Reset Enable Selection
;//       <0=> Enable WDT Reset <1=> By S/W (WDTCR Register)
;//    <o.0> Watch-Dog Timer Counter Enable Selection
;//       <0=> Enable WDT Counter <1=> By S/W (WDTCR Register)
;// </h>
WDTCNFIG                   EQU      0xFFFF96DB

;// <h> Low Voltage Reset Configuration
;//    <o.8..15> LVR Operation Control Selection
;//       <0xAA=> By S/W (LVRCR Register) <0xFF=> Always Enable
;//    <o.0..3> LVR Voltage Selection
;//       <15=> 1.62V <11=> 2.00V <10=> 2.13V <9=> 2.28V <8=> 2.46V <7=> 2.67V <6=> 3.04V <5=> 3.20V <4=> 3.55V <3=> 3.75V <2=> 3.99V <1=> 4.25V <0=> 4.55V
;// </h>
LVRCNFIG                   EQU      0xFFFFAAFF

;// <h> Configure Option Page Erase/Write Protection
;//    <o.0> Option Page 1, 0x1FFFF200 to 0x1FFFF27F
;//    <o.1> Option Page 2, 0x1FFFF400 to 0x1FFFF47F
;//    <o.2> Option Page 3, 0x1FFFF600 to 0x1FFFF67F
;// </h>
OPTIONPAGE                 EQU      0x00000000
CNFIGWTP1                  EQU      ~OPTIONPAGE

;// <h> Flash Memory Erase/Write Protection
;//    <o.0> Sector 0, 0x10000000 to 0x100007FF
;//    <o.1> Sector 1, 0x10000800 to 0x10000FFF
;//    <o.2> Sector 2, 0x10001000 to 0x100017FF
;//    <o.3> Sector 3, 0x10001800 to 0x10001FFF
;//    <o.4> Sector 4, 0x10002000 to 0x100027FF
;//    <o.5> Sector 5, 0x10002800 to 0x10002FFF
;//    <o.6> Sector 6, 0x10003000 to 0x100037FF
;//    <o.7> Sector 7, 0x10003800 to 0x10003FFF
;//    <o.8> Sector 8, 0x10004000 to 0x100047FF
;//    <o.9> Sector 9, 0x10004800 to 0x10004FFF
;//    <o.10> Sector 10, 0x10005000 to 0x100057FF
;//    <o.11> Sector 11, 0x10005800 to 0x10005FFF
;//    <o.12> Sector 12, 0x10006000 to 0x100067FF
;//    <o.13> Sector 13, 0x10006800 to 0x10006FFF
;//    <o.14> Sector 14, 0x10007000 to 0x100077FF
;//    <o.15> Sector 15, 0x10007800 to 0x10007FFF
;//    <o.16> Sector 16, 0x10008000 to 0x100087FF
;//    <o.17> Sector 17, 0x10008800 to 0x10008FFF
;//    <o.18> Sector 18, 0x10009000 to 0x100097FF
;//    <o.19> Sector 19, 0x10009800 to 0x10009FFF
;//    <o.20> Sector 20, 0x1000A000 to 0x1000A7FF
;//    <o.21> Sector 21, 0x1000A800 to 0x1000AFFF
;//    <o.22> Sector 22, 0x1000B000 to 0x1000B7FF
;//    <o.23> Sector 23, 0x1000B800 to 0x1000BFFF
;//    <o.24> Sector 24, 0x1000C000 to 0x1000C7FF
;//    <o.25> Sector 25, 0x1000C800 to 0x1000CFFF
;//    <o.26> Sector 26, 0x1000D000 to 0x1000D7FF
;//    <o.27> Sector 27, 0x1000D800 to 0x1000DFFF
;//    <o.28> Sector 28, 0x1000E000 to 0x1000E7FF
;//    <o.29> Sector 29, 0x1000E800 to 0x1000EFFF
;//    <o.30> Sector 30, 0x1000F000 to 0x1000F7FF
;//    <o.31> Sector 31, 0x1000F800 to 0x1000FFFF
;// </h>
FLASHSECTOR                EQU      0x00000000
FMWTP0                     EQU      ~FLASHSECTOR
;// </e>

;// <e> Configure Option Page 2
CONFIGURE_OPTION_2         EQU      1

;// <h> User Data Area 0
;//   <o0> Double 0 <0x00000000-0xFFFFFFFF>
;//   <o1> Double 1 <0x00000000-0xFFFFFFFF>
;//   <o2> Double 2 <0x00000000-0xFFFFFFFF>
;//   <o3> Double 3 <0x00000000-0xFFFFFFFF>
;//   <o4> Double 4 <0x00000000-0xFFFFFFFF>
;//   <o5> Double 5 <0x00000000-0xFFFFFFFF>
;//   <o6> Double 6 <0x00000000-0xFFFFFFFF>
;//   <o7> Double 7 <0x00000000-0xFFFFFFFF>
;//   <o8> Double 8 <0x00000000-0xFFFFFFFF>
;//   <o9> Double 9 <0x00000000-0xFFFFFFFF>
;//   <o10> Double 10 <0x00000000-0xFFFFFFFF>
;//   <o11> Double 11 <0x00000000-0xFFFFFFFF>
;//   <o12> Double 12 <0x00000000-0xFFFFFFFF>
;//   <o13> Double 13 <0x00000000-0xFFFFFFFF>
;//   <o14> Double 14 <0x00000000-0xFFFFFFFF>
;//   <o15> Double 15 <0x00000000-0xFFFFFFFF>
;//   <o16> Double 16 <0x00000000-0xFFFFFFFF>
;//   <o17> Double 17 <0x00000000-0xFFFFFFFF>
;//   <o18> Double 18 <0x00000000-0xFFFFFFFF>
;//   <o19> Double 19 <0x00000000-0xFFFFFFFF>
;//   <o20> Double 20 <0x00000000-0xFFFFFFFF>
;//   <o21> Double 21 <0x00000000-0xFFFFFFFF>
;//   <o22> Double 22 <0x00000000-0xFFFFFFFF>
;//   <o23> Double 23 <0x00000000-0xFFFFFFFF>
;//   <o24> Double 24 <0x00000000-0xFFFFFFFF>
;//   <o25> Double 25 <0x00000000-0xFFFFFFFF>
;//   <o26> Double 26 <0x00000000-0xFFFFFFFF>
;//   <o27> Double 27 <0x00000000-0xFFFFFFFF>
;//   <o28> Double 28 <0x00000000-0xFFFFFFFF>
;//   <o29> Double 29 <0x00000000-0xFFFFFFFF>
;//   <o30> Double 30 <0x00000000-0xFFFFFFFF>
;//   <o31> Double 31 <0x00000000-0xFFFFFFFF>
;// </h>
AREA0_0                    EQU      0xe2e2e2e2
AREA0_1                    EQU      0xFFFFFFFF
AREA0_2                    EQU      0xFFFFFFFF
AREA0_3                    EQU      0xFFFFFFFF
AREA0_4                    EQU      0xFFFFFFFF
AREA0_5                    EQU      0xFFFFFFFF
AREA0_6                    EQU      0xFFFFFFFF
AREA0_7                    EQU      0xFFFFFFFF
AREA0_8                    EQU      0xFFFFFFFF
AREA0_9                    EQU      0xFFFFFFFF
AREA0_10                   EQU      0xFFFFFFFF
AREA0_11                   EQU      0xFFFFFFFF
AREA0_12                   EQU      0xFFFFFFFF
AREA0_13                   EQU      0xFFFFFFFF
AREA0_14                   EQU      0xFFFFFFFF
AREA0_15                   EQU      0xFFFFFFFF
AREA0_16                   EQU      0xFFFFFFFF
AREA0_17                   EQU      0xFFFFFFFF
AREA0_18                   EQU      0xFFFFFFFF
AREA0_19                   EQU      0xFFFFFFFF
AREA0_20                   EQU      0xFFFFFFFF
AREA0_21                   EQU      0xFFFFFFFF
AREA0_22                   EQU      0xFFFFFFFF
AREA0_23                   EQU      0xFFFFFFFF
AREA0_24                   EQU      0xFFFFFFFF
AREA0_25                   EQU      0xFFFFFFFF
AREA0_26                   EQU      0xFFFFFFFF
AREA0_27                   EQU      0xFFFFFFFF
AREA0_28                   EQU      0xFFFFFFFF
AREA0_29                   EQU      0xFFFFFFFF
AREA0_30                   EQU      0xFFFFFFFF
AREA0_31                   EQU      0xe2e2e2e2
;// </e>

;// <e> Configure Option Page 3
CONFIGURE_OPTION_3         EQU      1

;// <h> User Data Area 1
;//   <o0> Double 0 <0x00000000-0xFFFFFFFF>
;//   <o1> Double 1 <0x00000000-0xFFFFFFFF>
;//   <o2> Double 2 <0x00000000-0xFFFFFFFF>
;//   <o3> Double 3 <0x00000000-0xFFFFFFFF>
;//   <o4> Double 4 <0x00000000-0xFFFFFFFF>
;//   <o5> Double 5 <0x00000000-0xFFFFFFFF>
;//   <o6> Double 6 <0x00000000-0xFFFFFFFF>
;//   <o7> Double 7 <0x00000000-0xFFFFFFFF>
;//   <o8> Double 8 <0x00000000-0xFFFFFFFF>
;//   <o9> Double 9 <0x00000000-0xFFFFFFFF>
;//   <o10> Double 10 <0x00000000-0xFFFFFFFF>
;//   <o11> Double 11 <0x00000000-0xFFFFFFFF>
;//   <o12> Double 12 <0x00000000-0xFFFFFFFF>
;//   <o13> Double 13 <0x00000000-0xFFFFFFFF>
;//   <o14> Double 14 <0x00000000-0xFFFFFFFF>
;//   <o15> Double 15 <0x00000000-0xFFFFFFFF>
;//   <o16> Double 16 <0x00000000-0xFFFFFFFF>
;//   <o17> Double 17 <0x00000000-0xFFFFFFFF>
;//   <o18> Double 18 <0x00000000-0xFFFFFFFF>
;//   <o19> Double 19 <0x00000000-0xFFFFFFFF>
;//   <o20> Double 20 <0x00000000-0xFFFFFFFF>
;//   <o21> Double 21 <0x00000000-0xFFFFFFFF>
;//   <o22> Double 22 <0x00000000-0xFFFFFFFF>
;//   <o23> Double 23 <0x00000000-0xFFFFFFFF>
;//   <o24> Double 24 <0x00000000-0xFFFFFFFF>
;//   <o25> Double 25 <0x00000000-0xFFFFFFFF>
;//   <o26> Double 26 <0x00000000-0xFFFFFFFF>
;//   <o27> Double 27 <0x00000000-0xFFFFFFFF>
;//   <o28> Double 28 <0x00000000-0xFFFFFFFF>
;//   <o29> Double 29 <0x00000000-0xFFFFFFFF>
;//   <o30> Double 30 <0x00000000-0xFFFFFFFF>
;//   <o31> Double 31 <0x00000000-0xFFFFFFFF>
;// </h>
AREA1_0                    EQU      0xe3e3e3e3
AREA1_1                    EQU      0xFFFFFFFF
AREA1_2                    EQU      0xFFFFFFFF
AREA1_3                    EQU      0xFFFFFFFF
AREA1_4                    EQU      0xFFFFFFFF
AREA1_5                    EQU      0xFFFFFFFF
AREA1_6                    EQU      0xFFFFFFFF
AREA1_7                    EQU      0xFFFFFFFF
AREA1_8                    EQU      0xFFFFFFFF
AREA1_9                    EQU      0xFFFFFFFF
AREA1_10                   EQU      0xFFFFFFFF
AREA1_11                   EQU      0xFFFFFFFF
AREA1_12                   EQU      0xFFFFFFFF
AREA1_13                   EQU      0xFFFFFFFF
AREA1_14                   EQU      0xFFFFFFFF
AREA1_15                   EQU      0xFFFFFFFF
AREA1_16                   EQU      0xFFFFFFFF
AREA1_17                   EQU      0xFFFFFFFF
AREA1_18                   EQU      0xFFFFFFFF
AREA1_19                   EQU      0xFFFFFFFF
AREA1_20                   EQU      0xFFFFFFFF
AREA1_21                   EQU      0xFFFFFFFF
AREA1_22                   EQU      0xFFFFFFFF
AREA1_23                   EQU      0xFFFFFFFF
AREA1_24                   EQU      0xFFFFFFFF
AREA1_25                   EQU      0xFFFFFFFF
AREA1_26                   EQU      0xFFFFFFFF
AREA1_27                   EQU      0xFFFFFFFF
AREA1_28                   EQU      0xFFFFFFFF
AREA1_29                   EQU      0xFFFFFFFF
AREA1_30                   EQU      0xFFFFFFFF
AREA1_31                   EQU      0xe3e3e3e3
;// </e>

                           IF      CONFIGURE_OPTION_1 <> 0
                           AREA    |.ARM.__AT_0x1FFFF200|, CODE, READONLY
                           DCD     RPCNFIG
                           DCW     EX0CNFIG, nEX0CNFIG, EX1CNFIG, nEX1CNFIG
                           DCD     WDTCNFIG
                           DCD     LVRCNFIG
                           DCD     CNFIGWTP1
                           FILL    40,0xFF,1
                           DCD     FMWTP0
                           ENDIF

                           IF      CONFIGURE_OPTION_2 <> 0
                           AREA    |.ARM.__AT_0x1FFFF400|, CODE, READONLY
                           DCD     AREA0_0, AREA0_1, AREA0_2, AREA0_3, AREA0_4, AREA0_5, AREA0_6, AREA0_7
                           DCD     AREA0_8, AREA0_9, AREA0_10, AREA0_11, AREA0_12, AREA0_13, AREA0_14, AREA0_15
                           DCD     AREA0_16, AREA0_17, AREA0_18, AREA0_19, AREA0_20, AREA0_21, AREA0_22, AREA0_23
                           DCD     AREA0_24, AREA0_25, AREA0_26, AREA0_27, AREA0_28, AREA0_29, AREA0_30, AREA0_31
                           ENDIF

                           IF      CONFIGURE_OPTION_3 <> 0
                           AREA    |.ARM.__AT_0x1FFFF600|, CODE, READONLY
                           DCD     AREA1_0, AREA1_1, AREA1_2, AREA1_3, AREA1_4, AREA1_5, AREA1_6, AREA1_7
                           DCD     AREA1_8, AREA1_9, AREA1_10, AREA1_11, AREA1_12, AREA1_13, AREA1_14, AREA1_15
                           DCD     AREA1_16, AREA1_17, AREA1_18, AREA1_19, AREA1_20, AREA1_21, AREA1_22, AREA1_23
                           DCD     AREA1_24, AREA1_25, AREA1_26, AREA1_27, AREA1_28, AREA1_29, AREA1_30, AREA1_31
                           ENDIF

                           END

