/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)


/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec blue_led = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

static const uint16_t NUM_OF_LEDS = 3u;

static const struct gpio_dt_spec* leds[] =
{
	&red_led,
	&green_led,
	&blue_led
};

static uint8_t m_u8LedIndex = 0;

typedef enum
{
	LED_ON,
	LED_OFF
}eLedState_t;

static void initLed(const struct gpio_dt_spec* led)
{
	if(led != NULL)
	{
		if(device_is_ready(led->port))
		{
			gpio_pin_configure_dt(led, GPIO_OUTPUT_ACTIVE);
		}
	}
}

static void setLed(uint8_t u8LedIndex, eLedState_t ledState)
{
	if(u8LedIndex < NUM_OF_LEDS)
	{
		switch(ledState)
		{
		case LED_ON:
			gpio_pin_set_dt(leds[u8LedIndex], 1);
			break;
		case LED_OFF:
			gpio_pin_set_dt(leds[u8LedIndex], 0);
			break;
		}
	}
}

static void increaseLedIndex(uint8_t* u8LedIndex)
{
	if(u8LedIndex != NULL)
	{
		(*u8LedIndex)++;

		(*u8LedIndex) %= NUM_OF_LEDS;
	}
}

static void setNextLedOn()
{
	setLed(m_u8LedIndex, LED_OFF);
	increaseLedIndex(&m_u8LedIndex);
	setLed(m_u8LedIndex, LED_ON);
}

void main(void)
{
	for(uint8_t i = 0u; i < NUM_OF_LEDS; i++)
	{
		initLed(leds[i]);
	}


	while (1) {
		setNextLedOn();
		k_msleep(SLEEP_TIME_MS);
	}
}
