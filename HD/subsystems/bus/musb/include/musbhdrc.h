#ifndef MUSBHDRC_H_
#define MUSBHDRC_H_




/*
 *     MUSBMHDRC Register map 
 */

/* Common USB registers */

#define MGC_O_HDRC_FADDR    0x00    /* 8-bit */
#define MGC_O_HDRC_POWER    0x01    /* 8-bit */

#define MGC_O_HDRC_INTRTX   0x02    /* 16-bit */
#define MGC_O_HDRC_INTRRX       0x04
#define MGC_O_HDRC_INTRTXE      0x06  
#define MGC_O_HDRC_INTRRXE      0x08  
#define MGC_O_HDRC_INTRUSB      0x0A   /* 8 bit */
#define MGC_O_HDRC_INTRUSBE     0x0B   /* 8 bit */
#define MGC_O_HDRC_FRAME        0x0C  
#define MGC_O_HDRC_INDEX        0x0E   /* 8 bit */
#define MGC_O_HDRC_TESTMODE     0x0F   /* 8 bit */ 

/* Get offset for a given FIFO */
#define MGC_FIFO_OFFSET(_bEnd) ((_bEnd+8) << 2) //(0x20 + (_bEnd * 4)) 

/* Additional Control Registers */

#define MGC_O_HDRC_DEVCTL   0x60       /* 8 bit */


/* offsets to registers in flat model */
#define MGC_O_HDRC_TXMAXP   0x00
#define MGC_O_HDRC_TXCSR    0x02
#define MGC_O_HDRC_CSR0 MGC_O_HDRC_TXCSR    /* re-used for EP0 */
#define MGC_O_HDRC_RXMAXP   0x04
#define MGC_O_HDRC_RXCSR    0x06
#define MGC_O_HDRC_RXCOUNT  0x08
#define MGC_O_HDRC_COUNT0   MGC_O_HDRC_RXCOUNT  /* re-used for EP0 */
#define MGC_O_HDRC_TXTYPE   0x0A
#define MGC_O_HDRC_TYPE0    MGC_O_HDRC_TXTYPE   /* re-used for EP0 */
#define MGC_O_HDRC_TXINTERVAL   0x0B
#define MGC_O_HDRC_NAKLIMIT0    MGC_O_HDRC_TXINTERVAL   /* re-used for EP0 */
#define MGC_O_HDRC_RXTYPE   0x0C
#define MGC_O_HDRC_RXINTERVAL   0x0D
#define MGC_O_HDRC_FIFOSIZE 0x0F
#define MGC_O_HDRC_CONFIGDATA   MGC_O_HDRC_FIFOSIZE /* re-used for EP0 */ 



/* "bus control" registers */
#define MGC_O_HDRC_TXFUNCADDR   0x00
#define MGC_O_HDRC_TXHUBADDR    0x02
#define MGC_O_HDRC_TXHUBPORT    0x03

#define MGC_O_HDRC_RXFUNCADDR   0x04
#define MGC_O_HDRC_RXHUBADDR    0x06
#define MGC_O_HDRC_RXHUBPORT    0x07

#define MGC_BUSCTL_OFFSET(_bEnd, _bOffset)  (((_bEnd+0x10) << 3) + _bOffset)

/*
 *     MUSBHDRC Register bit masks
 */

/* POWER */

#define MGC_M_POWER_ISOUPDATE   0x80 
#define MGC_M_POWER_SOFTCONN    0x40
#define MGC_M_POWER_HSENAB  0x20
#define MGC_M_POWER_HSMODE  0x10
#define MGC_M_POWER_RESET       0x08
#define MGC_M_POWER_RESUME      0x04
#define MGC_M_POWER_SUSPENDM    0x02
#define MGC_M_POWER_ENSUSPEND   0x01 

/* INTRUSB */
#define MGC_M_INTR_SUSPEND    0x01
#define MGC_M_INTR_RESUME     0x02
#define MGC_M_INTR_RESET      0x04
#define MGC_M_INTR_BABBLE     0x04
#define MGC_M_INTR_SOF        0x08 
#define MGC_M_INTR_CONNECT    0x10
#define MGC_M_INTR_DISCONNECT 0x20
#define MGC_M_INTR_SESSREQ    0x40
#define MGC_M_INTR_VBUSERROR  0x80   /* FOR SESSION END */
#define MGC_M_INTR_EP0      0x01  /* FOR EP0 INTERRUPT */

/* DEVCTL */
#define MGC_M_DEVCTL_BDEVICE    0x80   
#define MGC_M_DEVCTL_FSDEV      0x40
#define MGC_M_DEVCTL_LSDEV      0x20
#define MGC_M_DEVCTL_VBUS       0x18
#define MGC_S_DEVCTL_VBUS       3
#define MGC_M_DEVCTL_HM         0x04
#define MGC_M_DEVCTL_HR         0x02
#define MGC_M_DEVCTL_SESSION    0x01

/* TESTMODE */

#define MGC_M_TEST_FORCE_HOST   0x80
#define MGC_M_TEST_FIFO_ACCESS  0x40
#define MGC_M_TEST_FORCE_FS     0x20
#define MGC_M_TEST_FORCE_HS     0x10
#define MGC_M_TEST_PACKET       0x08
#define MGC_M_TEST_K            0x04
#define MGC_M_TEST_J            0x02
#define MGC_M_TEST_SE0_NAK      0x01

/* CSR0 */
#define MGC_M_CSR0_FLUSHFIFO      0x0100
#define MGC_M_CSR0_TXPKTRDY       0x0002
#define MGC_M_CSR0_RXPKTRDY       0x0001

/* CSR0 in Peripheral mode */
#define MGC_M_CSR0_P_SVDSETUPEND  0x0080
#define MGC_M_CSR0_P_SVDRXPKTRDY  0x0040
#define MGC_M_CSR0_P_SENDSTALL    0x0020
#define MGC_M_CSR0_P_SETUPEND     0x0010
#define MGC_M_CSR0_P_DATAEND      0x0008
#define MGC_M_CSR0_P_SENTSTALL    0x0004

/* CSR0 in Host mode */
#define MGC_M_CSR0_H_NO_PING      0x0800
#define MGC_M_CSR0_H_WR_DATATOGGLE   0x0400 /* set to allow setting: */
#define MGC_M_CSR0_H_DATATOGGLE     0x0200  /* data toggle control */
#define MGC_M_CSR0_H_NAKTIMEOUT   0x0080
#define MGC_M_CSR0_H_STATUSPKT    0x0040
#define MGC_M_CSR0_H_REQPKT       0x0020
#define MGC_M_CSR0_H_ERROR        0x0010
#define MGC_M_CSR0_H_SETUPPKT     0x0008
#define MGC_M_CSR0_H_RXSTALL      0x0004

/* TxType/RxType */
#define MGC_M_TYPE_SPEED    0xc0
#define MGC_S_TYPE_SPEED    6
#define MGC_TYPE_SPEED_HIGH 1
#define MGC_TYPE_SPEED_FULL 2
#define MGC_TYPE_SPEED_LOW  3
#define MGC_M_TYPE_PROTO    0x30
#define MGC_S_TYPE_PROTO    4
#define MGC_M_TYPE_REMOTE_END   0xf

/* CONFIGDATA */

#define MGC_M_CONFIGDATA_MPRXE      0x80    /* auto bulk pkt combining */
#define MGC_M_CONFIGDATA_MPTXE      0x40    /* auto bulk pkt splitting */
#define MGC_M_CONFIGDATA_BIGENDIAN  0x20
#define MGC_M_CONFIGDATA_HBRXE      0x10    /* HB-ISO for RX */
#define MGC_M_CONFIGDATA_HBTXE      0x08    /* HB-ISO for TX */
#define MGC_M_CONFIGDATA_DYNFIFO    0x04    /* dynamic FIFO sizing */
#define MGC_M_CONFIGDATA_SOFTCONE   0x02    /* SoftConnect */
#define MGC_M_CONFIGDATA_UTMIDW     0x01   /* data width 0 => 8bits, 1 => 16bits */ 

/* TXCSR in Peripheral and Host mode */

#define MGC_M_TXCSR_AUTOSET       0x8000
#define MGC_M_TXCSR_ISO           0x4000
#define MGC_M_TXCSR_MODE          0x2000
#define MGC_M_TXCSR_DMAENAB       0x1000
#define MGC_M_TXCSR_FRCDATATOG    0x0800
#define MGC_M_TXCSR_DMAMODE       0x0400
#define MGC_M_TXCSR_CLRDATATOG    0x0040
#define MGC_M_TXCSR_FLUSHFIFO     0x0008
#define MGC_M_TXCSR_FIFONOTEMPTY  0x0002
#define MGC_M_TXCSR_TXPKTRDY      0x0001

/* TXCSR in Peripheral mode */

#define MGC_M_TXCSR_P_INCOMPTX    0x0080
#define MGC_M_TXCSR_P_SENTSTALL   0x0020
#define MGC_M_TXCSR_P_SENDSTALL   0x0010
#define MGC_M_TXCSR_P_UNDERRUN    0x0004

/* TXCSR in Host mode */

#define MGC_M_TXCSR_H_WR_DATATOGGLE   0x0200
#define MGC_M_TXCSR_H_DATATOGGLE      0x0100
#define MGC_M_TXCSR_H_NAKTIMEOUT  0x0080
#define MGC_M_TXCSR_H_RXSTALL     0x0020
#define MGC_M_TXCSR_H_ERROR       0x0004

/* RXCSR in Peripheral and Host mode */

#define MGC_M_RXCSR_AUTOCLEAR     0x8000
#define MGC_M_RXCSR_DMAENAB       0x2000
#define MGC_M_RXCSR_DISNYET       0x1000
#define MGC_M_RXCSR_DMAMODE       0x0800
#define MGC_M_RXCSR_INCOMPRX      0x0100
#define MGC_M_RXCSR_CLRDATATOG    0x0080
#define MGC_M_RXCSR_FLUSHFIFO     0x0010
#define MGC_M_RXCSR_DATAERROR     0x0008
#define MGC_M_RXCSR_FIFOFULL      0x0002
#define MGC_M_RXCSR_RXPKTRDY      0x0001

/* RXCSR in Peripheral mode */

#define MGC_M_RXCSR_P_ISO         0x4000
#define MGC_M_RXCSR_P_SENTSTALL   0x0040
#define MGC_M_RXCSR_P_SENDSTALL   0x0020
#define MGC_M_RXCSR_P_OVERRUN     0x0004

/* RXCSR in Host mode */

#define MGC_M_RXCSR_H_AUTOREQ     0x4000
#define MGC_M_RXCSR_H_WR_DATATOGGLE   0x0400
#define MGC_M_RXCSR_H_DATATOGGLE        0x0200
#define MGC_M_RXCSR_H_RXSTALL     0x0040
#define MGC_M_RXCSR_H_REQPKT      0x0020
#define MGC_M_RXCSR_H_ERROR       0x0004










#endif /*MUSBHDRC_H_*/
