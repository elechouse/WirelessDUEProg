#WirelessDUEProg#

-----------------
Summary
-------
WirelessDUEProg is a project which aims to implement a wireless serial adapter, user can use this adapter to upload their program to Arduino DUE or even other Arduino boards.

Hardware
--------
* **Master**
	>Use nRF24LU1([Nordic Semiconductor](http://www.nordicsemi.com/ "Nordic")\)to emulate a USB CDC device. Arduino user access this Wireless USB serial adapter like any other Wired Serial tools(set/clear DTR/CTS, set baudrate, and send/recieve data ).The master is like a Dongle thing.

* **Slave**
	>Use Atmega168pa([ATMEL](http://www.atmel.com/ "Atmel"))+nRF24L01([Nordic Semiconductor](http://www.nordicsemi.com/ "Nordic")\). The slave module connect with DUE directly through ERASE pin, REEST pin, GND pin, 5V pin, TX and RX pin. 

Software
--------
* **Master**
	>Developed with KEIL C51

* **Slave**
	>Developed with IAR for AVR(will porting to gcc-avr soon)

Performance
-----------
Our team had tried our best to make WirelessDUEProg to be stable and work in furhter distance, but, unfortunately, the result is disappointing.

>Reliable communication distance : about 5m(that means, you can download your DUE with the distance less than 5m)

<table width="500" align="center" border="0" cellspacing="1" bgcolor="#000000">
	<tr>
		<td bgcolor = "#FFFFFF" align="center">Distance</td>
		<td bgcolor = "#FFFFFF" align="center">Success Rate(about)</td>
	</tr>
	<tr>
		<td bgcolor = "#FFFFFF" align="center">5m</td>
		<td bgcolor = "#FFFFFF" align="center">30%</td>
	</tr>
	<tr>
		<td bgcolor = "#FFFFFF" align="center">4m</td>
		<td bgcolor = "#FFFFFF" align="center">35%</td>
	</tr>
	<tr>
		<td bgcolor = "#FFFFFF" align="center">3m</td>
		<td bgcolor = "#FFFFFF" align="center">50%</td>
	</tr>
	<tr>
		<td bgcolor = "#FFFFFF" align="center">2m</td>
		<td bgcolor = "#FFFFFF" align="center">65%</td>
	</tr>
	<tr>
		<td bgcolor = "#FFFFFF" align="center">1m</td>
		<td bgcolor = "#FFFFFF" align="center">70%</td>
	</tr>
	<tr>
		<td bgcolor = "#FFFFFF" align="center">0.5m</td>
		<td bgcolor = "#FFFFFF" align="center">85%</td>
	</tr>
</table>
 