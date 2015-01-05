#ifndef _UART_H_
#define _UART_H_

/*! \brief Called by CDC interface
 * Callback running when CDC device have received data
 */
void cdc_rx_notify(uint8_t port);

/*! \brief Configures communication line
 *
 * \param cfg      line configuration
 */
void cdc_config(uint8_t port, usb_cdc_line_coding_t * cfg);

/*! \brief Opens communication line
 */
void cdc_open(uint8_t port);

/*! \brief Closes communication line
 */
void cdc_close(uint8_t port);

#endif // _UART_H_
