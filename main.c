/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * This version include INT1 and INT2 for shackle input for BLE advertisement and locking/unlocking
 * 
 */
 
#include <stdint.h>
#include <string.h>
#include "nrf51.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_advdata.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "ble_advertising.h"
#include "app_error.h"
#include "boards.h"
#include "device_manager.h"
#include "pstorage.h"
#include "bsp.h"
#include "bsp_btn_ble.h" 


#define GPIOTE_CHANNEL_0 0 																				/**< Using channel 0 of GPTIOE. */


#ifdef SVCALL_AS_NORMAL_FUNCTION
#include "ser_phy_debug_app.h"
#endif

/**
 * @brief BLE function prototypes
 *
 * Definitions for the declared prototypes can be found in BLE.c
 * 
 */
 
extern void ble_evt_dispatch(ble_evt_t * p_ble_evt);
extern void advertisement_evt_cb(ble_adv_evt_t ble_adv_evt);
extern void softdevice_init(void);
extern void gap_params_init(void);
extern void advertising_init(void);


/**
 * @brief GPIOTE Interrupt Handler
 *
 * When interrupt from Shacke Movement is received, this handler is called for advertising and wakeup
 * 
 */
void in1_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{		

		// Start fast advertisement
		ble_advertising_start(BLE_ADV_MODE_FAST);
		
		// Toggle LED_1 
    nrf_drv_gpiote_out_toggle(LED_1);
	nrf_drv_gpiote_in_event_enable(BUTTON_2, true);
}

void in2_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{		

		// Toggle LED_2 
    nrf_drv_gpiote_out_toggle(LED_2);
		nrf_drv_gpiote_in_event_disable(BUTTON_2);
}

/**
 * @brief Initialization of GPIOTE pins
 *
 * LED is attached on LED_1 and INT_1 is configured as input pin for interrupt from LIS3DH
 * 
 */
static void init_gpio(void)
{
		ret_code_t err_code;
		
		// Initializing GPIOTE
	  err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);
		
		// Setting LED_1 as OUTPUT
    err_code = nrf_drv_gpiote_out_init(LED_1, &out_config);
    APP_ERROR_CHECK(err_code);
	
		// Setting LED_2 as OUTPUT
    err_code = nrf_drv_gpiote_out_init(LED_2, &out_config);
    APP_ERROR_CHECK(err_code);
		
		// Configuring Interrupt detection on Button 1 as edge triggered event (Hi to Low)
		nrf_drv_gpiote_in_config_t in1_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true); 
    in1_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(BUTTON_1, &in1_config, in1_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(BUTTON_1, true);
	
		// Configuring Interrupt detection on Button 2 as edge triggered event (Hi to Low)
		nrf_drv_gpiote_in_config_t in2_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true); 
    in2_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(BUTTON_2, &in2_config, in2_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(BUTTON_2, true);
	
		

}

/**@brief Function for application main entry.
 */

int main(void){

	
	// Initialize.

	init_gpio();
	
	softdevice_init();
	gap_params_init();
	advertising_init();
	

	// Enter main loop.
	while(1)	
	{
			
		
		// Put device in power saving mode until an event is triggered 
		sd_app_evt_wait();
		
	}
	 

    
}

