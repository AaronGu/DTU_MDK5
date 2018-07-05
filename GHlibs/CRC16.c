#include "CRC16.h"

#define false (0)
#define true  (1)

static uint8_t crc_tab16_init = false;
static uint16_t crc_tab16[256];


static void init_crc16_tab(void)
{

	uint16_t i;
	uint16_t j;
	uint16_t crc;
	uint16_t c;

	for (i = 0; i < 256; i++)
	{
		crc = 0;
		c = i;
		for (j = 0; j < 8; j++)
		{
			if ((crc ^ c) & 0x0001)
				crc = (crc >> 1) ^ 0xA001;
			else
				crc = crc >> 1;
			c = c >> 1;
		}
		crc_tab16[i] = crc;
	}
	crc_tab16_init = true;
} /* init_crc16_tab */


uint16_t crc_modbus(const uint8_t *input_str, uint8_t num_bytes)
{

	uint16_t crc;
	uint16_t tmp;
	uint16_t short_c;
	const uint8_t *ptr;
	uint8_t a;

	if (!crc_tab16_init)
		init_crc16_tab();

	crc = 0xFFFF;
	ptr = input_str;

	if (ptr != NULL)
		for (a = 0; a < num_bytes; a++)
		{

			short_c = 0x00FF & (uint16_t)*ptr;
			tmp = crc ^ short_c;
			crc = (crc >> 8) ^ crc_tab16[tmp & 0xFF];
			ptr++;
		}
	return crc;
}
