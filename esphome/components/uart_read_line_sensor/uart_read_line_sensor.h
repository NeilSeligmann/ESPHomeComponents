#pragma once

// #include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/output/float_output.h"

namespace esphome
{
	namespace uart_read_line_sensor
	{
		class UartReadLineSensor : public Component, public UARTDevice, public TextSensor
		{
		public:
			UartReadLineSensor(UARTComponent *parent) : UARTDevice(parent) {}

			void setup() override
			{
				// nothing to do here
			}

			int readline(int readch, char *buffer, int len)
			{
				static int pos = 0;
				int rpos;

				if (readch > 0)
				{
					switch (readch)
					{
					case '\n': // Ignore new-lines
						break;
					case '\r': // Return on CR
						rpos = pos;
						pos = 0; // Reset position index ready for next time
						return rpos;
					default:
						if (pos < len - 1)
						{
							buffer[pos++] = readch;
							buffer[pos] = 0;
						}
					}
				}
				// No end of line has been found, so return -1.
				return -1;
			}

			void loop() override
			{
				const int max_line_length = 80;
				static char buffer[max_line_length];
				while (available())
				{
					if (readline(read(), buffer, max_line_length) > 0)
					{
						publish_state(buffer);
					}
				}
			}
		};

	}
}