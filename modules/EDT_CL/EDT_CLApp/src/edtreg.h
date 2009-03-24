/* #pragma ident "@(#)edtreg.h	1.163 05/26/05 EDT" */

#ifndef EDTREG_H
#define EDTREG_H
/*
 * Bit definitions
 */

#define P11W_ID			0x0000
#define P16D_ID			0x0001
#define PDV_ID			0x0010
#define PCD20_ID		0x0011
#define PCD40_ID		0x0012
#define PCD60_ID		0x0013
#define PDVK_ID			0x0014
#define PDV44_ID		0x0015
#define PDVAERO_ID		0x0016
#define PDVCL_ID		0x0034	/* PMC DV C-Link is using this DID */

#define PGP20_ID		0x0021
#define PGP40_ID		0x0022
#define PGP60_ID		0x0023
#define PDVRGB_ID		0x0024
#define PGP_RGB_ID		0x0024
#define PGP_THARAS_ID		0x0025

#define PGP_ECL_ID		0x0026

#define PCD_16_ID		0x0027

#define P53B_ID			0x0020

#define PDVFOI_ID		0x0030

#define PDVFCI_AIAG_ID		0x0038
#define PDVFCI_USPS_ID		0x0039
#define PCDFCI_SIM_ID		0x003a
#define PCDFCI_PCD_ID		0x003b

#define PSS4_ID			0x0040
#define PSS16_ID		0x0041

#define PCDA_ID			0x0044
#define PCDA16_ID		0x0045
#define PCDCL_ID		0x0046

#define PDVA_ID			0x0048
#define PDVA16_ID		0x0049
#define PDVCL2_ID		0x004a
#define PDVHSS_ID		0x004c
#define PCDHSS_ID		0x004e
#define PDVFOX_ID		PDVHSS_ID /* new names */	/* PMC FOX is using this DID */
#define PCDFOX_ID		PCDHSS_ID /* new names */

#define PGS4_ID			0x0050
#define PGS16_ID		0x0051

#define DMY_ID			0x0080
#define DMYK_ID			0x0081
#define DMYFOI_ID		0x0081
#define DMY44_ID		0x0081

#define USB_ID			0x0060

#define EDT_TRACESIZE		1024	/* in longs */
#define EDT_DEPSIZE		4096	/* in bytes */
#define EDT_IOCTLSIZE		4096	/* in bytes */

/* trace support */
#define	TR_READ_GRAB		0x1001
#define	TR_ISR_START		0x1002
#define	TR_ISR_END		0x1003
#define	TR_DPC_START		0x1004
#define	TR_DPC_END		0x1005
#define	TR_START_BUF		0x1006
#define	TR_DOGRAB		0x1007
#define	TR_WAIT_START		0x1008
#define	TR_NEED_WAIT		0x1009
#define	TR_WAIT_END		0x100b
#define	TR_LOADED		0x100c
#define	TR_DONE			0x100d
#define	TR_SERIAL		0x100e
#define	TR_TIMEOUT		0x100f
#define	TR_SET			0x1010
#define	TR_SER_READ		0x1011
#define	TR_SER_WRITE		0x1012
#define	TR_SER_READ_END		0x1013
#define	TR_SER_WRITE_END	0x1014
#define	TR_ISRGRAB		0x1015
#define	TR_DEVINT		0x1016
#define	TR_SNDMSG		0x1017
#define	TR_TMSTAMP		0x1018
#define	TR_P11W_ATTNINT		0x1019
#define	TR_P11W_CNTINT		0x101a
#define TR_EN_INT           0x101b

#define  TR_WAKEUP              0x101c
#define  TR_LOCK              0x101d
#define  TR_UNLOCK              0x101e
#define	TR_ENDCLR		0x101f
#define	TR_STARTCLR		0x1020
#define	TR_FLUSH		0x1021
#define	TR_SETEVENT		0x1022
#define	TR_EVENTSLP		0x1023
#define	TR_EVENTWK		0x1024
#define	TR_REFTMSTAMP		0x1025

#define TR_SETBUF       0x1026
#define TR_SERWAIT       0x1027
#define TR_SEREND       0x1028
#define TR_CONTSTATE       0x1029

#define TR_STATE_CHANGE	0x1030

#define TR_EN_INTR	0x1031
#define TR_CLEANUP  0x1032
#define TR_ABORT  0x1033
#define	TR_EVENTWARN	0x1034
#define TR_NOT_OUR_INTR 0x1035
#define TR_NO_ACTION 0x1036
#define TR_CHECK_SG_NXT 0x1037
#define TR_ISR_START_RT 0x1038
#define TR_ISR_END_RT   0x1039

#define TR_RT_INTR_START	0x1040
#define TR_RT_INTR_END	0x1041


/* SG next count/control */

/* write */
#define EDT_BURST_EN         0x01000000      /* burst enable */
#define EDT_DMA_MEM_RD       0x02000000      /* dma direction */
#define EDT_DMA_ABORT        0x04000000      /* abort dma */
#define EDT_EN_SG_DONE       0x08000000      /* enable sg dma done int */
#define EDT_EN_MN_DONE       0x10000000      /* enable main dma page int */
#define EDT_DMA_START        0x20000000      /* enable next dma */
#define EDT_EN_RDY           0x80000000      /* enable nxt empty int */

/* read */
#define EDT_NXTPG_INT        0x00010000      /* main nxt page int enabled */
#define EDT_CURPG_INT        0x00020000      /* main cur page int enabled */
#define EDT_PG_INT           0x00040000      /* page int is set */
#define EDT_READ0_0          0x00080000      /* reads 0 */
#define EDT_READ0_1          0x00100000      /* reads 0 */
#define EDT_READ0_2          0x00200000      /* reads 0 */
#define EDT_MN_NXT_EMP       0x00400000      /* main next is empty */
#define EDT_MN_DMA_DONE      0x00800000      /* main dma is done */
#define EDT_MN_BURST_EN      0x01000000      /* main burst enable */
#define EDT_MN_DMA_MEM_RD    0x02000000      /* main dma direction */
#define EDT_READ0_3          0x04000000      /* reads 0 */
/* #define EDT_EN_SG_DONE    0x08000000      */
/* #define EDT_EN_MN_DONE    0x10000000      */
#define EDT_DMA_RDY          0x20000000      /* next sg dma reg empty */
#define EDT_DMA_DONE         0x40000000      /* sg dma done */
/* #define EDT_EN_RDY        0x80000000      */

/* defines for scatter gather control */
#define EDT_LIST_PAGEINT     0x10000


/* dma config/status register */

/* write */
#define	EDT_RMT_ADDR		0x0000007f	/* Remote xilinx reg addr   */
#define	EDT_RMT_AUTO_INCR	0x00000080	/* Remote auto increment    */
#define	EDT_RFIFO_ENB		0x00000100	/* enable read fifo	*/
#define	EDT_WRITE_STROBE	0x00000200	/* give read burst fifo a dummy clock */
#define	EDT_EMPTY_CHAN_FIFO	0x00000800	/* SS16 Channel fifo flush bit*/
#define	EDT_TEMP_EN_INTR	0x00002000	/* GS temp adt7461 enable interrupt  */
#define	EDT_RMT_EN_INTR		0x00004000	/* Remote enable interrupt  */
#define	EDT_PCI_EN_INTR		0x00008000	/* dma enable interrupt     */
#define	EDT_RMT_DATA		0x00010000	/* Remote program data	    */
#define	EDT_RMT_CCLK		0x00020000	/* enable 1 cclk            */
#define	EDT_RMT_PROG		0x00040000	/* Remote program pin	    */
#define	EDT_RMT_INIT		0x00080000	/* Remote init pin	    */
#define	EDT_RMT_DONE		0x00100000	/* Remote done pin 	    */
#define	EDT_RMT_STATE		0x00200000	/* Remote init state pin    */
#define	EDT_FIFO_CNT		0x0f000000	/* number of 32 bit words   */
						/* to fill the burst fifo   */
#define	EDT_EMPTY_CHAN		0x0f000000	/* SS16 Channel fifo to flush */
#define	EDT_FIFO_FLG		0x10000000	/* set if something in fifo */
						/* needed to distinguish 1 and 0 */

#define FOI_EN_TX_INT		0x00001000
#define FOI_EN_RX_INT		0x00002000

#define	EDT_FIFO_SHIFT		24		/* shift for FIFO_CNT */
#define	EDT_EMPTY_CHAN_SHIFT	24	    /* SS16 shift for chan fifo flush */

/* read */
#define FOI_XMT_EMP		0x00000400	/* foi xmit cmd empty */
#define FOI_RCV_RDY		0x00000800	/* foi rcv cmd ready */
#define EDT_TEMP_INTR		0x00000800	/* GS temp ADT7461 intr */
#define	EDT_DMA_INTR		0x00001000	/* Dma interrupt    */
#define	EDT_RMT_INTR		0x00002000	/* Remote xilinx interrupt */
/*#define P11W_CNTINT           0x4000 	 count interrupt pending */
#define	EDT_PCI_INTR		0x00008000	/* PCI interrupt	   */

/*  Clear channel fifo bit */

#define EDT_CL_ZRQ			0x00000800

/*
 * ce error flags for foi
 *
 * clear with bit16 of int_cfg
 * 7 - overflow in mac8100 fifo (16 kbyte)
 * 6 - ignore - may show 1/8 full
 * 5 - ignore
 * 4 - block ram fifo overflow - ignore (2k)
 * 3 - block ram fifo not empty
 * 2 - non sequential data into dma engine - not yet
 * 1 - non sequential data into read data fifo
 * 0 - non sequential data into block ram fifo
 */




#define X_DATA			0x010000
#define X_CCLK			0x020000
#define X_PROG			0x040000
#define X_INIT			0x080000
#define X_DONE			0x100000
#define X_INITSTAT		0x200000
 


/**
 ** Special macro definititons:
 **	24 bit values:
 **	- bytes 0 and 1 contains the register offset within the Xilinx registers
 **	- byte 2 selects the access method of the register (local or remote
 **	  xilinx, etc).
 **	- byte 3 contains the size of the register in bytes.
 ** See the code in edt_set() etc. for usage.
 **/

/* Local Xilinx Register Set */
#define EDT_DMA_CUR_ADDR	0x04030000
#define EDT_DMA_NXT_ADDR	0x04030004
#define EDT_DMA_CUR_CNT		0x04030008
#define EDT_DMA_NXT_CNT		0x0403000c
#define EDT_SG_CUR_ADDR		0x04030010
#define EDT_SG_NXT_ADDR		0x04030014
#define EDT_SG_CUR_CNT		0x04030018
#define EDT_SG_NXT_CNT		0x0403001c
#define EDT_CHAN_OFFSET		0x20		/* offset to next dma channel */
#define EDT_CHAN16_BASE		0x200		/* start of 16 channel dma */

#define EDT_CL_INTFC_OFFSET	0x40		/* offset to next intfc - cl */

#define EDT_GP_OUTPUT       0x040000c0 /* for daughter board programming */


#define EDT_ADT7461_REG		0x01000088
/* bit definitions for EDT_ADT7461_REG */

#define EDT_ADT7461_CLK		0x1
#define EDT_ADT7461_DATA		0x2
#define EDT_ADT7461_TRISTATE	0x4
#define EDT_ADT7461_THERM_RESET	0x8
#define EDT_ADT7461_ALERT		0x10
#define EDT_ADT7461_THERM		0x20

/* register addresses on the ADT7461 */

#define EDT_ADT7461_DEVICE_ADDR			0x4C
#define EDT_ADT7461_ALERT_RESPONSE_ADDR	0x0C
#define EDT_ADT7461_INTERN_TEMP_R		0x00
#define EDT_ADT7461_EXT_TEMP_R		0x01
#define EDT_ADT7461_EXT_TEMP_LOW_BYTE_R		0x10
#define EDT_ADT7461_STATUS_REG_R			0x02
#define EDT_ADT7461_CFG_R                0x03
#define EDT_ADT7461_CFG_W                0x09

#define EDT_ADT7461_EXT_TEMP_HIGH_R	0x07
#define EDT_ADT7461_EXT_TEMP_LOW_R	0x08

#define EDT_ADT7461_EXT_TEMP_HIGH_W	0x0D
#define EDT_ADT7461_EXT_TEMP_LOW_W	0x0E

#define EDT_ADT7461_EXT_THERM_R    0x19
#define EDT_ADT7461_EXT_THERM_W    0x19

#define EDT_ADT7461_THERM_HYSTERESIS 0x21

/* Special SSD16 registers */
#define SSD16_CHENL     0x01010010		/* Channel Enable */
#define SSD16_CHENH     0x01010011

#define SSD16_CHDIRL    0x01010012		/* Channel Direction */
#define SSD16_CHDIRH    0x01010013

#define SSD16_CHEDGEL   0x01010014		/* Channel Clock Edge */
#define SSD16_CHEDGEH   0x01010015


/*
 * SSE (fast serial) definitions.
 */
#define  SSE_BYTESWAP           0x0101000f

#define  SSE_SWAPBYTES		0x01
#define  SSE_SWAPBITS		0x04
#define  SSE_SWAPSHORTS		0x08
#define  SSE_RSTA		0x10
#define  SSE_RSTB		0x20


#define  PCD_PAL0		0x01010010
#define  PCD_PAL1		0x01010011
#define  PCD_PAL2		0x01010012
#define  PCD_PAL3		0x01010013




#define EDT_DMA_CFG		0x040000c4
#define EDT_INTR_CFG		0x020000c4

#define EDT_DMA_INTCFG_ONLY	0x010000c5
#define EDT_DMA_INTCFG		0x020000c4
#define EDT_REMOTE_OFFSET	0x010000c4

#define EDT_DMA_STATUS		0x040000c8
#define EDT_REMOTE_DATA		0x040000cc
#define EDT_FLASHROM_ADDR	0x04000080
#define EDT_FLASHROM_DATA	0x04000084
#define EDT_FLASHROM_LTXHUB(hub) (0xfc000c | (hub << 12))

#define EDT_TEMP		0x04000088
#define EDT_DMA_STOP		0x01000089

#define FOI_DBG_STAT		0x010000ce
#define FOI_DBG_CLR		0x010000c6

/* to address individually */
#define EDT_CHAN0		0x0400001c
#define EDT_CHAN1		0x0400003c
#define EDT_CHAN2		0x0400005c
#define EDT_CHAN3		0x0400007c

/* xilinx programming support */
#define EDT_WRITECMD		0x01000000

/* Remote Xilinx Register Set */
#define PDV_CMD			0x01010000
#define PDV_REV			PDV_CMD
#define PDV_STAT		0x01010001
#define PDV_CFG			0x01010002
#define PDV_SHUTTER		0x01010003
#define PDV_SHUTTER_LEFT	0x01010004
#define PDV_UTIL3		0x01010005	/* was PDV_FIFO_FLAGS which has moved */
#define PDV_DATA_PATH		0x01010006
#define PDV_MODE_CNTL		0x01010007
#define	PDV_EXTMODE		0x01010008	/* extended mode delta design */
#define PDV_DATA_MSB		0x01010008	/* only for picam */
#define PDV_DATA_LSB		0x01010009	/* only for picam */
#define PDV_FIXEDLEN		0x02010008	/* only for xybion */
#define PDV_SERIAL_DATA		0x0101000a
#define PDV_SERIAL_DATA_STAT	0x0101000b
#define PDV_SERIAL_DATA_CNTL	0x0101000c
#define PDV_SERIAL_CNTL2	0x0101000d

/* This one is Bayer Stripe */
#define PDV_KBS_STATIC		0x0101000e

#define PDV_BYTESWAP		0x0101000f
#define PDV_UTILITY		PDV_BYTESWAP
#define PDV_UTIL2		0x01010010
#define PDV_SHIFT		0x01010011	/* defines below */
#define PDV_UTIL24		0x01010011	/* overloaded register, dva aiag24.bit ONLY */
#define PDV_MASK		0x02010012	/* zeros shifted data if bit set */
#define PDV_MASK_LO		0x01010012
#define PDV_MASK_HI		0x01010013

/* Kodak Bayer Stripe Registers */
#define PDV_KBS_CC		0x01010014
#define PDV_KBS_BLKSETUP	0x01010015
#define PDV_KBS_FUT_CPDV_PLLFG		0x01010016

/* Frame Counter Registers (in new aiag -- Bayer Stripe stuff is history) */
#define PDV_FRAME_PERIOD	0x03010014
#define PDV_FRAME_PERIOD0	0x01010014
#define PDV_FRAME_PERIOD1	0x01010015
#define PDV_FRAME_PERIOD2	0x01010016

/* ROI */
#define PDV_ROICTL		0x01010017 /* defines below */
#define PDV_HSKIP		0x02010018 /* wr hskip/rd xilinx opt, 16 BITS */
#define PDV_HACTV		0x0201001a /* write only, 16 BITS */
#define PDV_VSKIP		0x0201001c /* write only, 16 BITS */
#define PDV_VACTV		0x0201001e /* write only, 16 BITS */
#define PDV_XOPT		0x01010018 /* same as HSKIP but read, 16 BITS */

/* PLL */
/* dividers and bits work the same as the PCD. The bits in the
 * FUNCT register of the PCD (bits7-4) are in the same bits of
 * the PLL_CTL register. So use the same bit defines as PCD */

#define PDV_PLL_CTL		0x01010020 /* same as top bits of PCD FUNCT */
#define	PDV_REF_SCALE		0x01010021 /* AV9110 reference prescale */
#define	PDV_OUT_SCALE		0x01010022 /* AV9110 output prescale */

#define	PDV_STATUS2		0x01010023 /* jerry's new one */
#define	PDV_CL_FRMCNT		0x02010020 /* frame count on cl */
#define	PDV_CL_FRMCNT_RESET	0x01010020 /* frame count on cl */

#define PDV_CL_DATA_PATH	0x01010028 /* c-link data path */
#define PDV_CL_CFG		0x01010029 /* c-link config/control */

#define PDV_CL_LINESPERFRAME_LO	0x0101002a /* c-link lines per frame */
#define PDV_CL_LINESPERFRAME_HI	0x0101002b /* (pdvcamlk019 and higher) */
#define PDV_CL_LINESPERFRAME	0x0201002a /* lines per frame, combined */
#define PDV_CL_PIXELSPERLINE_LO	0x0101002c /* c-link pixels per line */
#define PDV_CL_PIXELSPERLINE_HI	0x0101002d /* (pdvcamlk019 and higher) */
#define PDV_CL_PIXELSPERLINE	0x0201002c /* pixels per line, combined */

#define PDV_CL_HMAX_LO		0x0101002e
#define PDV_CL_HMAX_HI		0x0101002f
#define PDV_CL_HMAX		0x0201002e /* linescan deinterleave reg for pir */

/*
 * New LH Strobe registers -- only in c-link so far,
 * but may be ported back into pdv/pdvk eventually
 */
#define PDV_LHS_DELAY		0x01010030 /* delay */
#define PDV_LHS_PERIOD		0x01010031  /* between-strobe per., 100us units */
#define PDV_LHS_COUNT_LOW	0x01010032  /* # strobes, low 8 bits */
#define PDV_LHS_COUNT_HI	0x01010033  /* # strobes, high 4 bits + dmy */
#define PDV_LHS_COUNT		0x02010032  /* # of strobes, combined */
#define PDV_LHS_CONTROL		0x01010034  /* lh strobe control bits */
/* check this out */
#define PDV_CL_JERRYG		0x01010038 /* c-link data path */
#define PDV_CL_JERRYDBG		0x040000d8 /* c-link debug */

/* PDV_CL_DATA_PATH bits */
#define PDV_CL_DATA_PATH_CHANNEL_MSK	0xf0 /* # of channels is high 4 bits */
#define PDV_CL_DATA_PATH_NBITS_MSK	0x0f /* # of bits is low 4 bits */

/* PDV_CL_CFG bits */
#define PDV_CL_CFG_RGB			0x1 /* RGB enable/disable */
#define PDV_CL_CFG_IGNORE_DV		0x2 /* ignore data valid */

/* ALERT: OLD WAY -- jerry's working on a new scheme that doesn't eat so many */
/* register resources */
/* Built in CL counters */
#define PDV_CL_HTOT		0x02010020
#define PDV_CL_HACT		0x02010022
#define PDV_CL_VTOT		0x02010024
#define PDV_CL_VACT		0x02010026

/* new baud rate register */
#define PDV_BRATE		0x1010024

/* Camera Link Simulator Registers */
#define PDV_CLSIM_CFGA		0x01010040
#define PDV_CLSIM_CFGB		0x01010041
#define PDV_CLSIM_CFGC		0x01010042
#define PDV_CLSIM_EXSYNCDLY	0x01010043
#define PDV_CLSIM_FILLA		0x01010044
#define PDV_CLSIM_FILLB		0x01010046
#define PDV_CLSIM_HCNTMAX	0x02010048
#define PDV_CLSIM_VACTV		0x0201004a
#define PDV_CLSIM_VCNTMAX	0x0301004c
#define PDV_CLSIM_HFVSTART	0x02010050
#define PDV_CLSIM_HFVEND	0x02010052
#define PDV_CLSIM_HLVSTART	0x02010054
#define PDV_CLSIM_HLVEND	0x02010056
#define PDV_CLSIM_HRVSTART	0x02010058
#define PDV_CLSIM_HRVEND	0x0201005a
#define PDV_CLSIM_TAP0START	0x02010060
#define PDV_CLSIM_TAP0DELTA	0x02010062
#define PDV_CLSIM_TAP1START	0x02010064
#define PDV_CLSIM_TAP1DELTA	0x02010066
#define PDV_CLSIM_TAP2START	0x02010068
#define PDV_CLSIM_TAP2DELTA	0x0201006a
#define PDV_CLSIM_TAP3START	0x0201006c
#define PDV_CLSIM_TAP3DELTA	0x0201006e

/* Serial Parity - on Aero asynch board */

#define PDV_SERIAL_PARITY	0x1010025
#define PDV_SERIAL_PARITY_NONE	0x0
#define PDV_SERIAL_PARITY_EVEN	0x1
#define PDV_SERIAL_PARITY_ODD	0x3

/* PDV_ROICTL bits */
#define PDV_ROICTL_DALSA_LS	0x80	/* dalsa line scan mode */
#define PDV_ROICTL_ROI_EN	0x40	/* enable roi */
#define PDV_ROICTL_SIM_SYNC	0x20	/* simulator sync ena */
#define PDV_ROICTL_SIM_DAT	0x10	/* simulator data ena */
#define PDV_ROICTL_DUAL_CHAN	0x08	/* dual channel */
#define PDV_ROICTL_PCLKSEL_MSK	0x03	/* pixel clock speed (bottom 3 bits) */
#define PDV_RIOCTL_PCLKSEL_DBL_CAM 0x09 /* dual channel + double rate (kodak) */
/* bit 2 1 0 (Pixel Clock Select bits) */
/*     0 0 0  from camera              */
/*     0 0 1  double rate from camera  */
/*     0 1 0  undefined                */
/*     0 1 1  undefined                */
/*     1 0 0  20 MHz                   */
/*     1 0 1  10 MHz                   */
/*     1 1 0  5 MHz                    */
/*     1 1 1  undefined                */

/*
 * command bits
 * write only, non persistent commands
 */
#define	PDV_RESET_INTFC		0x01
#define	PDV_ENABLE_GRAB		0x02
#define	PDV_AQ_CLR		0x04
#define	PDV_CLR_CONT		0x08
#define	PDV_FV_CLR		0x10
#define PDV_EXP_STROBE  0x20


/* Configuration Register */
#define	PDV_DIS_SHUTTER		0x01
#define	PDV_TRIG		0x02
#define	PDV_INV_SHUTTER		0x04
#define	PDV_FIFO_RESET		0x08
#define PDV_BAUD2		0x10	/* see BAUD0-1 in ser data cntl reg */
#define PDV_RESERVED1		0x20	/* was FILTER_00_ON but no longer */
#define	PDV_EN_DALSA		0x40
#define	PDV_INT_ENAQ		0x80
/*
 * NOTE: PDV_STRIP_2LSB, PDV_AQ_RISE and PDV_EN_DATA_OUT were all
 * previously defined as 0x40, but are not currently used as such 
 * anywhere in the s/w, so ahm taking their defines out -- rwh 2/7/02
 */

/*
 * Data path register  
 */
#define	PDV_EXT_DEPTH		0x01
#define	PDV_DUAL_CHAN		0x02		/* for aiag -  */
#define	PDV_RES0		0x02
#define	PDV_RES1		0x04
#define	PDV_RES_MASK		0x06
#define	PDV_INVERT		0x08
#define	PDV_CONTINUOUS		0x10
#define	PDV_INTERLACED		0x20
#define	PDV_DECADE0		0x40
#define	PDV_DECADE1		0x80

/*
 * The multiplier bits are the 6 and 7 bits of the data path register,
 * and are defined as:
 *		00	1MS
 *		01	10MS
 *		10	100MS
 *		11	undefined.
 */

#define	PDV_MULTIPLIER_MASK	0xc0
#define	PDV_MULTIPLIER_10MS	0x40
#define	PDV_MULTIPLIER_100MS	0x80
#define	PDV_MULTIPLIER_100US	0xc0

/*
 * status read - mostly fifo flags
 */
#define	PDV_OVERRUN		0x01
#define	PDV_FRAME_VALID		0x02
#define	PDV_EXPOSURE		0x04
#define	PDV_FIELD		0x08
#define	PDV_SW_ARMED		PDV_FIELD 	/*  in aiag */
#define	PDV_CHAN_ID0		0x10
#define	PDV_CHAN_ID1		0x20
#define	PDV_GRAB_ARMED		0x40
#define	PDV_AQUIRE_IP		0x80

/* defines for utility (byteswap) register */
#define PDV_BSWAP		0x01
#define PDV_PAD0		0x02
#define PDV_PAD1		0x04
#define PDVCL_DUNCAN30		0x02	/* overloaded PAD0 on PDV C-LINK */
#define PDVCL_RGB30		0x04	/* overloaded PAD1 on PDV C-LINK */
#define PDV_SSWAP		0x08
#define PDV_DISABLEMD		0x10
#define PDV_SKIPFRM		0x20	/* OBSOLETE */
#define PDV_PTRIGEXP            0x20    /* ptrig drives EXP directly to cam */
#define PDV_GENDATA		0x20	/* aiargb ONLY: fake data */
#define PDV_SKIP		0x40	/* skip every other pixel */
#define PDV_VIDDIR		0x80	/* turn video data lines out (for pi) */

/* defines for utility 2 register */
#define PDV_PHOTO_TRIGGER	0x01	/* proto coupler triggers after start */
#define PDV_FLDID_TRIGGER	0x02	/* field id triggers after start */
#define PDV_AQUIRE_MULTIPLE	0x04	/* aquire 1 frame for each h/w trigger */
#define PDV_INT_ENFV		0x08	/* enable frame valid interrupt */
#define PDV_EXP_INT			0x10	/* use expose instead of FV for int */
#define PDV_GND_TRIG_WORD	0x20
#define PDV_DBL_TRIG            0x10	/* enable pulnix double trigger mode */
#define PDV_PULNIX              0x20	/* expose can be asserted during fv, */
                                        /* holds off grab_armed till eo expose*/
#define PDV_MC4			0x40	/* mode code 4 for photonics */
#define PDV_RX232		0x40	/* bit overloaded for RCI -- 232 rcv*/
#define PDV_SEL_MC4		0x80	/* select mode code 4 */

/* defines for status2 register */
#define PHOTO_STAT		0x01
#define TRIG_ES40		0x10

/* defines for Kodak Bayer Stripe CC register */
#define PDV_KBS_CC_RROWOLOD	0x01
#define PDV_KBS_CC_GPIXOLOD	0x02
#define PDV_KBS_CC_FRCPGM	0x04
#define PDV_KBS_CC_CLRN		0x08
#define PDV_KBS_CC_OE		0x10
#define PDV_KBS_CC_PGMCLK	0x20
#define PDV_KBS_CC_CWE0		0x40
#define PDV_KBS_CC_CWE1		0x80
#define PDV_KBS_CC_CWE_MSK	(PDV_KBS_CC_CWE0 | PDV_KBS_CC_CWE1)

/* bit 6 set in xilinx rev indicates kodak bayer stripe */

/* Defines for static register */

#define PDV_KBS_CLIP_ON_2	0x01
#define PDV_KBS_DETAIL_ONLY	0x02
#define PDV_KBS_MED_EDGE	0x04
#define PDV_KBS_SHARP_0		0x08
#define PDV_KBS_SHARP_1		0x10
#define PDV_KBS_BLGN_0		0x40
#define PDV_KBS_BLGN_1		0x80

#define PDV_KBS_BLGN_SHIFT  6
#define PDV_KBS_SHARP_SHIFT 3


#define PDV_REV_TOUCAN		0x8
#define PDV_REV_KBS		0x8

/* defines for shift register */
#define PDV_SFT_MASK		0x0f	/* shift from 0 - 15 (rotate) */
					/* shift 4 to make 12 bits at lsb */
#define PDV_AIA_SWAP		0x10	/* swap 16 bit data end-end for aia */

/* defines for util24 register -- pcidva aiag24.bit ONLY */
#define PDV_PACK24		0x04	/* pack 24 bits into 3 bytes only */
#define PDV_BYTEFLIP24		0x08	/* flip bytes */
#define PDV_BITFLIP24		0x10	/* flip bits */

/* defines for utility 3 register (was FIFO_FLAGS reg which got moved) */
#define PDV_PTRIGINV		0x01	/* invert optical trigger polarity */
#define PDV_LV_INVERT		0x01    /* !WAS inv line valid polarity */
#define PDV_FV_INVERT		0x02	/* !WAS inv frame valid polarity */
#define PDV_FV_INT_INVERT       0x04    /* gen. FV int on rising not falling */
#define PDV_TRIGINT		0x08	/* appropriate FV interrupt, make it TRIG */
#define PDV_MODE16		0x10	/* enable 16-bit packing for RCX/FOX */
#define PDV_FVADJ		0x40	/* enable FV based frame counter */
#define PDV_FRENA		0x80	/* enable continuous frame counter */

/*
 * Mode control register
 */
#define PDV_AIA_MC0		0x01
#define PDV_AIA_MC1		0x02
#define PDV_AIA_MC2		0x04
#define PDV_AIA_MC3		0x08
#define PDV_AIA_MC_MASK		0x0f
#define PDV_EN_SHUTTER0		0x10
#define PDV_EN_SHUTTER1		0x20
#define PDV_EN_SHUTTER2		0x40
#define PDV_EN_SHUTTER3		0x80
#define PDV_EN_SHUTTER_MASK	0xf0

/*
 * LH strobe control register
 */
#define PDV_LHS_RESET		0x01
#define PDV_LHS_ENABLE		0x02
#define PDV_LHS_DAC_LOAD	0x10
#define PDV_LHS_DAC_DATA	0x20
#define PDV_LHS_DAC_CLOCK	0x40

/*
 * extended mode register for delta design camera
 * used to program and enable the delta design camera
 */
#define	PDV_SER_CLK		0x01
#define	PDV_SER_DATA		0x02
#define	PDV_LD_REG		0x04
#define	PDV_DAC_REG_ADD		0x08
#define	PDV_LD_DAC		0x10
#define	PDV_ENEE_PROM		0x20
#define	PDV_ENEE_DATA		0x40
#define	PDV_EN_CAM		0x80

/*
 * read the EE PROM back through PDV_STAT exposure bit
 */
#define	PDV_EE_DATA		PDV_EXPOSURE		
#define	PDV_EE_SHFT_DATA	2

/*
 * the enable test pattern bit for the delta design camera is in the
 * regular mode code register. It is the Data in also when loading
 * the camera xilinx.
 */
#define	PDV_ENDD_PATTERN	PDV_AIA_MC2

/*
 * Functional modes for KODAK AIA
 */
#define PDV_MODE_CONTROLLED	0x00
#define PDV_MODE_TRG		0x02
#define PDV_MODE_CONT		0x03


/*
 * Serial data status register
 */
#define PDV_RECEIVE_RDY		0x01
#define PDV_TRANSMIT_RDY	0x02
#define LHS_DONE		0x04	/* strobe done */
#define LHS_INTERRUPT		0x08	/* strobe interrupt bit */
#define PDV_FVAL_INT		0x10	/* normal int -- after AND gate */
#define PDV_AQUIRE_INT		0x20
#define PDV_FVAL_INTA		0x40	/* int after AND gate */
#define PDV_INTFC_INT		0x80

/*
 * Serial data control register
 */
#define PDV_EN_RX		0x01
#define PDV_EN_TX		0x02
#define PDV_EN_RX_INT		0x04
#define PDV_EN_TX_INT		0x08
#define PDV_EN_DEV_INT		0x10
#define PDV_CLR_RX_INT		0x20
#define PDV_BAUD0		0x40
#define PDV_BAUD1		0x80
#define PDV_BAUD_MASK		0xc0
#define LHS_INTEN		0x80		/* Strobe Interrupt Enable */

/*
 * serial control register 2
 */

#define PDV_BRKENA		0x40		/* enable break (spectral insts) */
#define PDV_BRKSEL		0x80		/* break polarity -- 1 high, 0 low */

/* pdv genericsim register description */
#define SIM_STATUS     		0x01010001
#define SIM_CFG        		0x0101001f
#define SIM_WIDTH      		0x02010010
#define SIM_LDELAY     		0x02010012
#define SIM_HEIGHT     		0x02010014
#define SIM_FDELAY     		0x02010016
#define SIM_SPEED      		0x02010017

/* for pdv foi */
#define FOI_MSG	 		0x040000c0
#define FOI_WR_MSG_DATA         0x010000c0
#define FOI_WR_MSG_STAT         0x010000c1
#define FOI_RD_MSG_DATA         0x010000c2
#define FOI_RD_MSG_STAT         0x010000c3

#define	FOI_TX_FIFO_EMP		0x20
#define FOI_TX_FIFO_FULL	0x10
#define FOI_FIFO_FLUSH		0x02
#define FOI_MSG_SEND		0x01
#define FOI_MSG_BSY		0x01
#define	FOI_DATA_AVAIL		0x80
#define FOI_FIFO_EMPTY		0x40
#define FOI_RXWM1		0x20
#define FOI_WAITEOF		0x10

/* for rci fifo flags reached via PDV_INTFC_FIFO */
/* bits all 0 if empty - then get 1, 11, 111, 1111 as gets full */
/* may not be current - used for edt debug */
#define RCI_FIFO_EMPTY		0x01
#define RCI_FIFO_AEMPTY		0x02
#define RCI_FIFO_AFULL		0x04
#define RCI_FIFO_FULL		0x08

/* for camtest */
#define CAM_CMD			0x01010000
#define CAM_STAT		0x01010002
#define CAM_COMPARE		0x02010006
#define CAM_LSPIXELS		0x02010008
#define CAM_MSPIXELS		0x0101000A
#define CAM_HEIGHT		0x0201000C
#define CAM_WIDTH		0x0201000E
#define CAM_FV			0x80
#define CAM_LV			0x40
#define CAM_PCLK		0x20


/* pcd intfc */
#define PCD_CMD			0x01010000
#define PCD_DATA_PATH_STAT	0x01010001
#define PCD_FUNCT		0x01010002
#define PCD_STAT		0x01010003
#define PCD_STAT_POLARITY	0x01010004
#define PCD_OPTION		0x01010005
#define PCD_DIRA		0x01010006
#define PCD_DIRB		0x01010007
#define PCD_PIO_OUTLO		0x01010008
#define PCD_PIO_OUTHI		0x01010009
#define PCD_CONFIG		0x0101000f
#define PCD_CYCLE_CNT		0x0101000e /* customer special */
#define PCD_LTX_CMD 		0x0101003c /* LTX command reg */
#define	EDT_REF_SCALE		0x0101001d /* AV9110 reference prescale */
#define	EDT_OUT_SCALE		0x0101001e /* AV9110 output prescale */

#define PCD_RTCWORD		0x04010020
#define PCD_RTCADDR		0x04010024

/* ssdio intfc */
/* written to PCD_FUNCT */
#define SSDIO_SHFT_RIGHT	0x1	/* lsb first bit in byte */
#define SSDIO_STROBE		0x2	/* low to high forces input bit */
#define SSDIO_STOP_CLK		0x4	/* no output clock when no data */
#define SSDIO_ENABLE_RDQ	0x8	/* enable receive data qualifier */
#define SSDIO_EN_EXT_CLK	0x80 /* enable receive clock as xmit clk */
/* ssdio status */
/* read from PCD_STAT */
#define SSDIO_LAST_BIT		0x1 /* set when next input bit last bit */
#define SSDIO_BYTE_CNT0		0x2 /* CNT0 and CNT1 show which byte filling */
#define SSDIO_BYTE_CNT1		0x4 
#define SSDIO_BYTECNT_MSK	(SSDIO_BYTE_CNT0 | SSDIO_BYTE_CNT1)
#define SSDIO_BYTECNT_SHFT	1
/* ssdio idle */
/* pattern output when no data to xmit if STOP_CLK 0 */
#define SSDIO_IDLE		0x02010018	/* 16 bit idle pattern */

#define	REF_XTAL		30000000.0
/* fields in PCD_OUT_SCALE */
#define	EDT_FAST_DIV		0x03
#define	EDT_FAST_DIV1		0x00
#define	EDT_FAST_DIV3		0x01
#define	EDT_FAST_DIV5		0x02
#define	EDT_FAST_DIV7		0x03
#define EDT_X_DIVN		0xfc
#define EDT_X_DIVN_SHFT		2

/* pcd command bits */
#define	PCD_DIR			0x01
#define	PCD_BNR_ENABLE		0x02 /*	force BNR */
#define	PCD_DATA_INV		0x04
#define	PCD_ENABLE		0x08
#define	PCD_STAT_INT_EN_1	0x10
#define	PCD_STAT_INT_EN_2	0x20
#define	PCD_STAT_INT_EN_3	0x40
#define	PCD_STAT_INT_EN_4	0x80
#define	PCD_STAT_INT_EN_X	0xf0

/* pcd status bits */
#define	PCD_STAT_1		0x01
#define	PCD_STAT_2		0x02
#define	PCD_STAT_3		0x04
#define	PCD_STAT_4		0x08
#define	PCD_STAT_INT_1		0x10
#define	PCD_STAT_INT_2		0x20
#define	PCD_STAT_INT_3		0x40
#define	PCD_STAT_INT_4		0x80
#define	PCD_STAT_INT_X		0xf0

/*
 * Special purpose bits in polarity register.
 */
#define PCD_STAT_INT_ENA	0x10
#define PCD_EN_BACK_IO 0x40

/* pcd datapath status bits */
#define PCD_OUT_EMP		0x01
#define PCD_UNUSED		0x02
#define PCD_UNDERFLOW		0x04
#define PCD_OVERFLOW		0x08
#define PCD_IN_FULL		0x10
#define PCD_SPARE0		0x20 
#define PCD_IN_ALMOST_FULL	0x40
#define PCD_SPARE		0x80

/* Aptix data path status bits */
 
#define APTIX_UNDERFLOW		0x04
#define APTIX_OVERFLOW		0x08
#define APTIX_IDLE		0x10
#define APTIX_WAIT_FOR_WRITE	0x20 
#define APTIX_WAIT_FOR_READ	0x40
#define APTIX_READING		0x80

/* pcd intfc config bits */
#define PCD_BYTESWAP		0x01
#define PCD_SEL_RXT		0x02
#define EDT_SEL_RXT		0x02
#define PCD_SHORTSWAP		0x08
#define PCD_EN_IDVDLY		0x80



/* xtest offsets in intfc */
#define XTEST_CMD		0x0
#define XTEST_CMD0		0x0
#define XTEST_CMD1		0x1
#define XTEST_STAT		0x2
#define XTEST_NOTUSED		0x3
#define XTEST_DIRREG		0x4
#define XTEST_DIRA		0x4
#define XTEST_DIRB		0x5
#define XTEST_COUNT1		0x6
#define XTEST_COUNT0		0x7
#define XTEST_RDDO		0x8
#define XTEST_RDDO0_7		0x8
#define XTEST_RDDO8_15		0x9
#define XTEST_RDCO		0xa
#define XTEST_RDCO0_7		0xa
#define XTEST_RDCO8_15		0xb
#define XTEST_RDDI		0xc
#define XTEST_RDDI0_7		0xc
#define XTEST_RDDI8_15		0xd
#define XTEST_RDCI		0xe
#define XTEST_RDCI0_7		0xe
#define XTEST_RDCI8_15		0xf

/*
 * command/config bits 
 */
#define	XTEST_EN_DATA_03_811	0x0001
#define	XTEST_EN_DATA_47_1215	0x0002
#define	XTEST_EN_CTL_03_811	0x0004
#define	XTEST_EN_CTL_47_1215	0x0008
#define	XTEST_ASSRT_DEV_INT	0x0010
#define XTEST_EN_BACK_IO 0x0020
/*
 * dma data generation - set directio then enable
 * fifo will be filled for DMA write (device read) with psuedo
 * fifo will be emptied and check for DMA READ
 */
#define	XTEST_DEV_READ		0x100
#define XTEST_EN_DATA		0x200
#define	XTEST_UN_RESET_FIFO	0x400
#define	XTEST_BIT32		0x1000
#define	XTEST_SWAPWORDS		0x2000
#define	XTEST_INVERT		0x4000
#define	XTEST_SWAPBYTES		0x8000

/*
 * status read - mostly fifo flags
 */
#define	XTEST_NOT_OUT_FIFO_1_EMPTY	0x1
#define	XTEST_NOT_OUT_FIFO_0_EMPTY	0x2
#define	XTEST_NOT_IN_FIFO_1_FULL	0x4
#define	XTEST_NOT_IN_FIFO_0_FULL	0x8
#define	XTEST_NOT_IN_FIFO_A_FULL	0x10
/*
 * from pcdtest.h
 */
/*
 * command bits 
 */
#define	PCDTEST_ENCHK		0x1
#define	PCDTEST_ENCMP		0x2
#define	PCDTEST_ENUNDER		0x4
#define	PCDTEST_ENABLE		0x8	
#define	PCDTEST_SETDNR		0x10
#define	PCDTEST_IGNORE_BNR	0x20
#define PCDTEST_EN_BACK_IO  0x40        

/*
 * status registers (read back scd funct bits 
 */
#define	PCDTEST_STAT_MSK	0xf

/*
 * functio register - sets stat lines for scd
 */
#define	PCDTEST_FUNCT_MSK	0xf

/* bits in the FUNCT register to serial program AV9110 */
#define	EDT_FUNCT_PLLCLK	0x80
#define	EDT_FUNCT_SELAV		0x40
#define	EDT_FUNCT_DATA		0x20
#define	EDT_FUNCT_CLK		0x10

/*
 * data path status
 */
#define	PCDTEST_OFIFO_NE	0x1
#define	PCDTEST_IFIFO_EMP	0x2
#define	PCDTEST_UNDER		0x4
#define	PCDTEST_IFIFO_NF	0x10
#define	PCDTEST_IFIFO_NAF	0x40
#define	PCDTEST_SP_IN		0x80

/*
 * intfc offsets 
 */
#define PCDT_CMD		0
#define PCDT_DPSTAT		1
#define PCDT_STAT		2
#define PCDT_FUNCT		3
#define PCDT_POLARITY		4
#define PCDT_DIVIDE		5
#define PCDT_START		6
#define PCDT_BITE		8
#define PCDT_IODIR		0xa

/*
 * ssd test 
 */
#define SSDT_FUNC		1

/*
 * SSD16 registers
 */
 
#define SSD16_CHEN	0x02010010
#define SSD16_CHENL     0x01010010 /* Channel Enable */
#define SSD16_CHENH     0x01010011

#define SSD16_CHDIR	0x02010012
#define SSD16_CHDIRL    0x01010012 /* Channel Direction */
#define SSD16_CHDIRH    0x01010013

/* for memtest - 1 inc counter on clock - 0 inc counter on read */
#define SSD16_CHEDGE    0x02010014
#define SSD16_CHEDGEL   0x01010014 /* Channel Clock Data Latch Edge */
#define SSD16_CHEDGEH   0x01010015

/* for memtest - 0 - 16 bit, 1 - 12 bit with channel in upper nibble */
#define SSD16_LSB	0x02010016
#define SSD16_LSBL	0x01010016 /* Channel Clock Data Latch Edge */
#define SSD16_LSBH	0x01010017

#define SSD16_UNDER	0x02010018 /* fifo underflow on output */
#define SSD16_OVER	0x0201001a /* fifo overflow on input */

#define SSD16_CHINVERT      0x0201001c /* Channel data invert */
#define SSD16_CHINVERT_L    0x0101001c /* Channel data invert, low byte*/
#define SSD16_CHINVERT_H    0x0101001d /* Channel data invert, high byte */
#define SSD16_IDLE_PAT      0x0401002c /* Idle pattern */
#define SSD16_IDLE_PAT_L0   0x0101002c /* Idle pattern, byte 0 */
#define SSD16_IDLE_PAT_L1   0x0101002d /* Idle pattern, byte 1 */
#define SSD16_IDLE_PAT_L2   0x0101002e /* Idle pattern, byte 2 */
#define SSD16_IDLE_PAT_L3   0x0101002f /* Idle pattern, byte 3 */
#define COMBO_DIFF_CLK_POL  0x01010023 /* Channel clock polarity */
#define COMBO_DIFF_DATA_INV 0x0101002c /* Channel data invert */


/* 
 * pcd intfc offsets
 */
#define PCD_DIRREG		6

/* 
 * p16d register descriptions
 */
#define P16_COMMAND		0x020000c4
#define P16_CONFIG		0x020000c6
#define P16_STATUS		0x020000c8
#define P16_DATA		0x020000ca
/*
 * p16d command  bits
 */
#define	P16_EN_INT		0x8000	/* enable pci interrupt         */
#define	P16_EN_DINT		0x2000	/* enable device interrupt      */
#define	P16_FCLK		0x1000	/* Force clock			*/
#define	P16_ODDSTART		0x0200	/* DMA starts on odd word	*/
#define	P16_BCLR		0x0100	/* Board clear			*/
#define	P16_INIT		0x0080	/* Initialize remote device	*/
#define	P16_FNCT3		0x0040	/* Function 3			*/
#define	P16_FNCT2		0x0020	/* Function 2			*/
#define	P16_FNCT1		0x0010	/* Function 1			*/
#define	P16_FLUSH		0x0004	/* Flush dma register 		*/

/*
 * p16d configuration bits
 */
#define	P16_SWAP		0x0100	/* swap half-words		*/
#define	P16_DFRST		0x0008	/* disable fifo reset		*/
#define	P16_CLKP		0x0002	/* clock polarity select	*/
#define	P16_DACKP		0x0001	/* data ack select	*/

/* 
 * p16d status bits
 */
#define	P16_INT			0x8000	/* pci interrupt asserted       */
#define	P16_DEVINT		0x2000	/* device interrupt asserted	*/
#define	P16_DMA_INT		0x1000	/* dma interrupt asserted	*/
#define	P16_DINT_S		0x0800	/* device interrupt state	*/
#define	P16_STATC_S		0x0400	/* stat_c state			*/
#define	P16_STATB_S		0x0200	/* stat_b state			*/
#define	P16_STATA_S		0x0100	/* stat_a state			*/
#define	P16_D_INIT_S		0x0080	/* device init output state	*/
#define	P16_FNCT3_S		0x0040	/* Function 3 state		*/
#define	P16_FNCT2_S		0x0020	/* Function 2 state		*/
#define	P16_FNCT1_S		0x0010	/* Function 1 state		*/
#define	P16_ENDINT_S	 	0x0004	/* 1's compl. of ENDINT	in cmd	*/
#define	P16_DIR_S		0x0002	/* 1's compl. of DIR in cmd	*/

/* 
 * p11w register descriptions
 */

/* SGI has backward addressing for these */

#ifdef sgi

#define P11_COMMAND		0x020000c6
#define P11_CONFIG		0x020000c4
#define P11_STATUS		0x020000ca
#define P11_DATA		0x020000c8
/* This might be wrong... FIX ... */
#define P11_COUNT		0x040000cc

#else

#define P11_COMMAND		0x020000c4
#define P11_CONFIG		0x020000c6
#define P11_STATUS		0x020000c8
#define P11_DATA		0x020000ca
#define P11_COUNT		0x040000cc

#endif

/*
 * command register defines
 */
#define P11W_GO	  	0x0001  /* go bit - start fifos */
#define P11W_BLKM       0x0002  /* block mode - direction constant during */
#define P11W_DIRS0      0x0004  /* 00 use latc1, X1 - use DMA cmd MEM_RD */
#define P11W_DIRS1      0x0008  /* 10 use funct1 */
#define P11W_FNCT1      0x0010  /* Function control bits */
#define P11W_FNCT2      0x0020
#define P11W_FNCT3      0x0040
#define P11W_INIT       0x0080  /* Initialize Device - not a pulse        */
#define P11W_BCLR       0x0100  /* Board clear - aborts DMA in process    */
#define P11W_ODDSTART   0x0200  /* Odd word start			    */
#define P11W_FCYC       0x1000  /* force cycle request - used in link     */
#define P11W_EN_ATT     0x2000  /* enable attention interrupt */
#define P11W_EN_CNT     0x4000  /* enable end of dr11 cnt interrupt */
#define P11W_EN_INT     0x8000  /* global interrupt enable */

/* convienent defines */
#define P11W_RESET (P11W_BCLR)

/* 
 * status register defines
 */
/* this is regs_p->status */
#define P11W_RDY_S      0x0001  /* 0 - dma in progress 1 - idle */
#define P11W_BLKM_S     0x0002  /* burst enabled */
#define P11W_FNCT1_S    0x0010  /* read back as written in command */
#define P11W_FNCT2_S    0x0020
#define P11W_FNCT3_S    0x0040
#define P11W_INIT_S     0x0080  /* read back as written in command */
#define P11W_STATA_S    0x0100  /* state of status lines on DR11 interface */
#define P11W_STATB_S    0x0200
#define P11W_STATC_S    0x0400

#define P11W_ATTN       0x0800  /* state of attn - not latched */
#define P11W_DMAINT     0x1000  /* dma interrupt pending */
#define P11W_ATTNINT    0x2000  /* attention interrupt pending */
#define P11W_CNTINT     0x4000  /* count interrupt pending */
#define P11W_REOD     	0x4000  /* ALERT: from S11W -- port ok or no ??? */
#define P11W_INT        0x8000  /* interrupt pending */

/* 
 * configuration register defines
 */
/* this is regs_p->config */

#define P11W_BSYP       0x0001  /* active low  BUSY if set */
#define P11W_CYCP       0x0002  /* positive edge cycle request if set */
#define P11W_ENBB       0x0004  /* Enable cycle request B       */
#define P11W_OUTSK0     0x0008  /* out skew */
#define P11W_OUTSK1     0x0010
#define P11W_INSK0      0x0020  /* in skew */
#define P11W_INSK1      0x0040
#define P11W_RDYT       0x0080  /* ready end of cycle */
#define P11W_SWAP       0x0100  /* swap bytes */
#define P11W_INV        0x0200  /* invert data */
#define P11W_NCOA       0x0800  /* No clear on ATTN */
#define P11W_PFCT2      0x1000  /* Pulse FCT2 */

/*
 * defines for descriptions for edt_get, edt_set
 */
#define LOCAL_XILINX_TYPE	0x00
#define REMOTE_XILINX_TYPE	0x01
#define MAC8100_TYPE		0x02
#define LOCAL_DMA_TYPE		0x03
#define REG_FILE_TYPE       	0x04
#define LOCAL_USB_TYPE		0x05
#define REMOTE_USB_TYPE		0x06
#define INTFC_BYTE		(EDT_MAKE_TYPE(REMOTE_XILINX_TYPE)|EDT_MAKE_SIZE(1))
#define INTFC_WORD		(EDT_MAKE_TYPE(REMOTE_XILINX_TYPE)|EDT_MAKE_SIZE(2))
#define INTFC_32		(EDT_MAKE_TYPE(REMOTE_XILINX_TYPE)|EDT_MAKE_SIZE(4))
#define MAC8100_WORD		(EDT_MAKE_TYPE(MAC8100_TYPE)|EDT_REG_SIZE(2))

#define EDT_REG_ADDR(x)		(x & 0xffff)
#define EDT_REG_TYPE(x)		((x >> 16) & 0xff)
#define EDT_REG_SIZE(x)		((x >> 24) & 0xff)
#define EDT_MAKE_TYPE(x)	((x & 0xff) << 16)
#define EDT_MAKE_SIZE(x)	((x & 0xff) << 24)
#define USB_CMD_TYPE(x)		((x >> 8) & 0xff)

	 /* PCI SS PLL control - ICS307 */

#define EDT_SS_PLL_CTL     0x01010020
#define EDT_SS_CLK_SEL     0x01010021
#define EDT_SS_DIFFLOOP    0x01010022
#define EDT_SS_PLL0_CLK    0x01010024
#define EDT_SS_PLL0_X      0x01010025
#define EDT_SS_PLL1_CLK    0x01010026
#define EDT_SS_PLL1_X      0x01010027
#define EDT_SS_PLL2_CLK    0x01010028
#define EDT_SS_PLL2_X      0x01010029
#define EDT_SS_PLL3_CLK    0x0101002A
#define EDT_SS_PLL3_X      0x0101002B

	 /* ICS 307 control bits */

#define EDT_SS_PLL_STROBE0 0x01
#define EDT_SS_PLL_STROBE1 0x02
#define EDT_SS_PLL_STROBE2 0x04
#define EDT_SS_PLL_STROBE3 0x08

#define EDT_SS_PLL_DATA    0x40
#define EDT_SS_PLL_CLK     0x80

#define PCDA_ODV_DELAY	    0x01010028
#define PCDA_LED_CTL	    0x01010029

#define SSMEM_ADDR	0x03010064 
#define SSMEM_DATA	0x04010068 
#define SSMEM_READ0	0x0401006c 
#define SSMEM_READ1	0x04010070

#define PDMA_OFFSET	0x40000000
#define PDMA_SIZE	0x2000

    	/* USB 8051 command methods */
#define EDT_USB_8051_CMD     0x01050000	/* 8051 Command Methods          */
#define EDT_USB_8051_RFIFO   0x01050001	/* 8051 Read Fifo		 */
#define EDT_USB_8051_WFIFO   0x01050002	/* 8051 Write Fifo		 */
#define EDT_USB_8051_RREG    0x01050003	/* 8051 Read FPGA Register       */
#define EDT_USB_8051_WREG    0x01050004	/* 8051 Write FPGA Register      */

    	/* USB Analyzer registers and control bits */

#define EDT_USB_ANL_CMD     0x01060000	/* Analyzer Command Register          */
#define     ANL_ENABLE	        0x01		/* Analyzer enable bit        */
#define     ANL_DDR	        0x02		/* Double data rate           */
#define     ANL_GLITCH	        0x04		/* Enable glitch detection    */

#define EDT_USB_ANL_STATUS  0x01060001	/* Analyzer Status Register           */
#define     ANL_CAPTURE	        0x01		/* Capture in progress        */
#define     ANL_TRIGGER_A       0x02		/* Trigger A true             */
#define     ANL_TRIGGER_B       0x04		/* Trigger B true             */

#define EDT_USB_ANL_ICSREG  0x01060002	/* Analyzer ICS clock chip            */
#define     ANL_ICSDAT	        0x01		/* ICS data bit               */
#define     ANL_ICSCCK          0x02		/* ICS clock bit              */
#define     ANL_ICSSTB          0x04		/* ICS strobe bit             */
#define     ANL_LED2            0x40		/* 2nd LED                    */
#define     ANL_LED             0x80		/* LED - set to turn on       */

#define EDT_USB_ANL_TSCALE  0x0106000d	/* Analyzer timebase register         */
#define     ANL_DOWNCNT	        0x0f		/* Timebase downcount by n+1: */
    						/* 0,1,4 gives x1, x2, x5.    */
#define     ANL_DECADE          0xf0		/* Timebase decade            */
    						/* 0-7 gives 5ns,10ns,100ns   */
    						/* 1us,10us,100us,1ms,10ms.   */

#define EDT_USB_ANL_DELAY   0x0206000e	/* Trigger delay register (16 bits)   */
					    /* Delay from trigger to end of   */
    					    /* capture in timebase units.     */

#define EDT_USB_ANL_MASK_A  0x02060010	/* Trigger A mask register (16 bits)  */
				        /* Trigger A fires when:              */
				     /* (data & MASK_A) == (MASK_A & MATCH_A) */

#define EDT_USB_ANL_MASK_B  0x02060012	/* Trigger B mask register (16 bits)  */
				        /* Trigger B fires when:              */
				     /* (data & MASK_B) == (MASK_B & MATCH_B) */

#define EDT_USB_ANL_MATCH_A 0x02060014	/* Trigger A match register (16 bits) */
					        /* After trigger A fires,     */
    					        /* then wait for trigger B.   */

#define EDT_USB_ANL_MATCH_B 0x02060016	/* Trigger B match register (16 bits) */
					    /* After trigger B fires,         */
    					    /* then wait for delay timebase   */
    					    /* ticks before stopping capture. */

#endif /* EDTREG_H */

