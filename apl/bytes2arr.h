/*! \file bytes2arr.h
    \brief Defines macros to get/put values from/to byte array.
	This header file defines macros to put 8 bit, 16 bit and 32 bit values
	into byte array; also defines macros to get 8 bit, 16 bit and 32 bit values
	out of a byte array.
	Separate macros available for Little endian and Big endian architectures.
	\author pramod.ranade@sedemac.com
	\version v1.10
	\date 3 Apr 2017
	\copyright SEDEMAC Mechatronics Pvt. Ltd. (www.sedemac.com)
*/

#ifndef BYTES2ARR
#define	BYTES2ARR

/** \cond */

#ifdef __BYTE_ORDER__
	// this is a predefined macro in (most versions of) gcc.
	// if it is already defined, we check it's value here
#else
	// in case this macro is not predefined, you may define it here
	// and also define the two possible values for it.
	#define	__ORDER_LITTLE_ENDIAN__	0
	#define	__ORDER_BIG_ENDIAN__	1

	#define	__BYTE_ORDER__	__ORDER_LITTLE_ENDIAN__
#endif

// Little Endian
#define	PUT_U8_IN_U8ARR_LE(arr,index,val)	arr[index]=val
#define	PUT_U16_IN_U8ARR_LE(arr,index,val)	arr[index]=val;arr[(index)+1]=(val)>>8
#define	PUT_U32_IN_U8ARR_LE(arr,index,val)	arr[index]=val;arr[(index)+1]=(val)>>8;arr[(index)+2]=(val)>>16;arr[(index)+3]=(val)>>24

#define	GET_U8_FROM_U8ARR_LE(arr,index)		(arr[index])
#define	GET_U16_FROM_U8ARR_LE(arr,index)	((((uint16_t)(arr[(index)+1]))<<8)+(arr[index]))
#define	GET_U32_FROM_U8ARR_LE(arr,index)	((((uint32_t)(arr[(index)+3]))<<24)+(((uint32_t)(arr[(index)+2]))<<16)+(((uint32_t)(arr[(index)+1]))<<8)+(arr[index]))

// Big Endian
#define	PUT_U8_IN_U8ARR_BE(arr,index,val)	arr[index]=val
#define	PUT_U16_IN_U8ARR_BE(arr,index,val)	arr[(index)+1]=(uint8_t)val;arr[index]=(uint8_t)((val)>>8)
#define	PUT_U32_IN_U8ARR_BE(arr,index,val)	arr[(index)+3]=val;arr[(index)+2]=(val)>>8;arr[(index)+1]=(val)>>16;arr[index]=(val)>>24

#define	GET_U8_FROM_U8ARR_BE(arr,index)		(arr[index])
#define	GET_U16_FROM_U8ARR_BE(arr,index)	((((uint16_t)(arr[index]))<<8)+(arr[(index)+1]))
#define	GET_U32_FROM_U8ARR_BE(arr,index)	((((uint32_t)(arr[index]))<<24)+(((uint32_t)(arr[(index)+1]))<<16)+(((uint32_t)(arr[(index)+2]))<<8)+(arr[(index)+3]))

/** \endcond */

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
	/**Put uint8_t value in byte array.*/
	#define	PUT_U8_IN_U8ARR(arr,index,val)	PUT_U8_IN_U8ARR_LE(arr,index,val)
	/**Put uint16_t value in byte array.*/
	#define	PUT_U16_IN_U8ARR(arr,index,val)	PUT_U16_IN_U8ARR_LE(arr,index,val)
	/**Put uint32_t value in byte array.*/
	#define	PUT_U32_IN_U8ARR(arr,index,val)	PUT_U32_IN_U8ARR_LE(arr,index,val)
	/**Get uint8_t value from byte array.*/
	#define	GET_U8_FROM_U8ARR(arr,index)	GET_U8_FROM_U8ARR_LE(arr,index)
	/**Get uint16_t value from byte array.*/
	#define	GET_U16_FROM_U8ARR(arr,index)	GET_U16_FROM_U8ARR_LE(arr,index)
	/**Get uint32_t value from byte array.*/
	#define	GET_U32_FROM_U8ARR(arr,index)	GET_U32_FROM_U8ARR_LE(arr,index)
#else
	/**Put uint8_t value in byte array.*/
	#define	PUT_U8_IN_U8ARR(arr,index,val)	PUT_U8_IN_U8ARR_BE(arr,index,val)
	/**Put uint16_t value in byte array.*/
	#define	PUT_U16_IN_U8ARR(arr,index,val)	PUT_U16_IN_U8ARR_BE(arr,index,val)
	/**Put uint32_t value in byte array.*/
	#define	PUT_U32_IN_U8ARR(arr,index,val)	PUT_U32_IN_U8ARR_BE(arr,index,val)
	/**Get uint8_t value from byte array.*/
	#define	GET_U8_FROM_U8ARR(arr,index)	GET_U8_FROM_U8ARR_BE(arr,index)
	/**Get uint16_t value from byte array.*/
	#define	GET_U16_FROM_U8ARR(arr,index)	GET_U16_FROM_U8ARR_BE(arr,index)
	/**Get uint32_t value from byte array.*/
	#define	GET_U32_FROM_U8ARR(arr,index)	GET_U32_FROM_U8ARR_BE(arr,index)
#endif



#endif // !BYTES2ARR
