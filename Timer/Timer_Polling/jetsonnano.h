#ifndef __JETSONNANO_H
#define __JETSONNANO_H

#define _I	volatile const	/*!< defines 'read only' permissions>!*/
#define _O	volatile	/*!< defines 'write only' permissions>!*/
#define _IO	volatile	/*!< defines 'read / write' permissions>!*/

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef struct
{
	_IO uint32_t PTV;
	_IO uint32_t PCR;
}TMR_TypeDef;

typedef struct
{
	_I uint32_t CNTR_1US;
	_IO uint32_t USEC_CFG;
	uint32_t RESERVED[6];
	_IO uint32_t CNTR_FREEZE;
}TMRUS_TypeDef;

typedef struct
{
    _IO uint32_t CNF;
    uint32_t RESERVED1[3];
    _IO uint32_t OE;
    uint32_t RESERVED2[3];
    _O uint32_t OUT;
    uint32_t RESERVED3[3];
    _I uint32_t IN;
    uint32_t RESERVED4[3];
    _IO uint32_t INT_STA;
    uint32_t RESERVED5[3];
    _IO uint32_t INT_ENB;
    uint32_t RESERVED6[3];
    _IO uint32_t INT_LVL;
    uint32_t RESERVED7[3];
    _IO uint32_t INT_CLR;
    uint32_t RESERVED8[3];
}GPIO_TypeDef;

//Define macro for gpio registers
#define GPIO_1_BASE	0x6000D000
#define GPIO_2_BASE	0x6000D100
#define GPIO_3_BASE	0x6000D200
#define GPIO_4_BASE	0x6000D300
#define GPIO_5_BASE	0x6000D400
#define GPIO_6_BASE	0x6000D500
#define GPIO_7_BASE	0x6000D600
#define GPIO_8_BASE	0x6000D700

//GPIO declaration
#define GPIO_A		(GPIO_1_BASE + 0x0000)
#define GPIO_B		(GPIO_1_BASE + 0x0004)
#define GPIO_C		(GPIO_1_BASE + 0x0008)
#define GPIO_D		(GPIO_1_BASE + 0x000C)

#define GPIO_E		(GPIO_2_BASE + 0x0000)
#define GPIO_F		(GPIO_2_BASE + 0x0004)
#define GPIO_G		(GPIO_2_BASE + 0x0008)
#define GPIO_H		(GPIO_2_BASE + 0x000C)

#define GPIO_I			(GPIO_3_BASE + 0x0000)
#define GPIO_J			(GPIO_3_BASE + 0x0004)
#define GPIO_K		(GPIO_3_BASE + 0x0008)
#define GPIO_L		(GPIO_3_BASE + 0x000C)

#define GPIO_M		(GPIO_4_BASE + 0x0000)
#define GPIO_N		(GPIO_4_BASE + 0x0004)
#define GPIO_O		(GPIO_4_BASE + 0x0008)
#define GPIO_P		(GPIO_4_BASE + 0x000C)

#define GPIO_Q		(GPIO_5_BASE + 0x0000)
#define GPIO_R		(GPIO_5_BASE + 0x0004)
#define GPIO_S		(GPIO_5_BASE + 0x0008)
#define GPIO_T		(GPIO_5_BASE + 0x000C)

#define GPIO_U		(GPIO_6_BASE + 0x0000)
#define GPIO_V		(GPIO_6_BASE + 0x0004)
#define GPIO_W		(GPIO_6_BASE + 0x0008)
#define GPIO_X		(GPIO_6_BASE + 0x000C)

#define GPIO_Y		(GPIO_7_BASE + 0x0000)
#define GPIO_Z		(GPIO_7_BASE + 0x0004)
#define GPIO_AA		(GPIO_7_BASE + 0x0008)
#define GPIO_BB		(GPIO_7_BASE + 0x000C)

#define GPIO_CC		(GPIO_8_BASE + 0x0000)
#define GPIO_DD		(GPIO_8_BASE + 0x0004)
#define GPIO_EE		(GPIO_8_BASE + 0x0008)


#define TMR_BASE		0x60005000

//Define macro for timer registers
#define TMR0		 	(TMR_BASE + 0x0088)
#define TMR1		 	(TMR_BASE + 0x0000)
#define TMR2			(TMR_BASE + 0x0008)
#define TMR3			(TMR_BASE + 0x0050)
#define TMR4			(TMR_BASE + 0x0058)
#define TMR5			(TMR_BASE + 0x0060)
#define TMR6			(TMR_BASE + 0x0068)
#define TMR7			(TMR_BASE + 0x0070)
#define TMR8			(TMR_BASE + 0x0078)
#define TMR9			(TMR_BASE + 0x0080)
#define TMR10		 	(TMR_BASE + 0x0090)
#define TMR11		 	(TMR_BASE + 0x0098)
#define TMR12		 	(TMR_BASE + 0x00A0)
#define TMR13		 	(TMR_BASE + 0x00A8)

//TIMERUS_USEC_CFG
#define TMRUS	 	(TMR_BASE + 0x0010)

//Interrupts
#define ARB_PRI_BASE				0x60003000
#define PRI_ICTLR_BASE				0x60004000
#define PRI_APR_GNT_ICTLR_BASE		0x60004040
#define SEC_ICTLR_BASE				0x60004100
#define TRI_ICTLR_BASE				0x60004200
#define QUAD_ICTLR_BASE			0x60004300
#define PENTA_ICTLR_BASE			0x60004400
#define HEXA_ICTLR_BASE			0x60004500
#define COM_ICTLR_BASE				0x60004800

#endif