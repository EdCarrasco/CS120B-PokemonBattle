/*
 * bit.h
 *
 * Created: 5/19/2019 8:31:53 PM
 *  Author: Edward-Laptop
 */ 

#ifndef BIT_H_
#define BIT_H_

unsigned char SetBit(unsigned char port, unsigned char bit, unsigned char value) {
	return (value ? port | (0x01 << bit) : port & ~(0x01 << bit));
}

unsigned char GetBit(unsigned char pin, unsigned char bit) {
	return pin & (0x01 << bit);
}

#endif /* BIT_H_ */