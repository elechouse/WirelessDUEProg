#include "nrf_app.h"

const u8 nrf_addr[5]={0x37, 0x75, 0x59, 0x96, 0x6A};

void nrf_app_init(void)
{	
	/** clear irq flag */
	hal_nrf_get_clear_irq_flags();
	
	/** empty FIFO */
	hal_nrf_flush_rx();
	hal_nrf_flush_tx();
	
	/** rf parameters */
	hal_nrf_set_output_power(HAL_NRF_0DBM);
	hal_nrf_set_rf_channel(52);
	hal_nrf_set_datarate(HAL_NRF_1MBPS);
	
	/** set address */
	hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);
	hal_nrf_set_address(HAL_NRF_PIPE0, nrf_addr);
	hal_nrf_set_address(HAL_NRF_TX, nrf_addr);
	
	/* dynamic payload length, */
	hal_nrf_enable_dynamic_payload(true);	//FEATRUE register 
	hal_nrf_enable_ack_payload(false);
	hal_nrf_enable_dynamic_ack(true);
	hal_nrf_setup_dynamic_payload(DPL_P0);	//DYNPD register
	
	/** EN_AA, EN_RXADDR  */
	hal_nrf_close_pipe(HAL_NRF_PIPE5);
	hal_nrf_close_pipe(HAL_NRF_PIPE4);
	hal_nrf_close_pipe(HAL_NRF_PIPE3);
	hal_nrf_close_pipe(HAL_NRF_PIPE2);
	hal_nrf_close_pipe(HAL_NRF_PIPE1);
	hal_nrf_open_pipe(HAL_NRF_PIPE0, true);
					   
	/** auto retransmmit */
	hal_nrf_set_auto_retr(10, 0x100);
	
	/** config */
	hal_nrf_set_irq_mode(HAL_NRF_MAX_RT,true);/**< Max retries interrupt */
    hal_nrf_set_irq_mode(HAL_NRF_TX_DS,true); /**< TX data sent interrupt */
    hal_nrf_set_irq_mode(HAL_NRF_RX_DR,true);/**< RX data received interrupt */	
	hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);
	hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
//	hal_nrf_set_operation_mode(HAL_NRF_PRX);
}

void nrf_app_sta_handle(void)
{

}