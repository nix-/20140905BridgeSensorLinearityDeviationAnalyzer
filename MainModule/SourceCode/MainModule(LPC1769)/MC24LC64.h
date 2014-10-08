#ifndef __MC24LC64_H
#define __MC24LC64_H

uint32_t MC24LC64_Wr(uint32_t Address, uint8_t *Buffer, uint32_t Length);
uint32_t MC24LC64_Rd(uint32_t Address, uint8_t *Buffer, uint32_t Length);
uint32_t MC24LC64_Init(void);

#endif /* __MC24LC64_H */
