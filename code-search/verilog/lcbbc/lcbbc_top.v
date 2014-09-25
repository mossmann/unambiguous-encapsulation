
module lcbbc_top(

input	wire			clk_50,

// Hex LEDs
output	wire	[6:0]	HEX0,
output	wire	[6:0]	HEX1,
output	wire	[6:0]	HEX2,
output	wire	[6:0]	HEX3,
output	wire	[6:0]	HEX4,
output	wire	[6:0]	HEX5,
output	wire	[6:0]	HEX6,
output	wire	[6:0]	HEX7,

input		wire	[3:0]	KEY,
input		wire 	[17:0] SW,
output	wire	[17:0] LEDR,
output   wire	[8:0] LEDG

);

uart_debug uart_debug_0 (
	.clk_50		( clk_50 ),
	.reset      ( uart_reset ),
	.mii_clk	( phy0_gm_rx_clk ),
	.mii_in		( phy0_gm_rxd ), 
	.mii_en		( phy0_gm_rx_dv ),
	.uart_tx	( DAISHO_RS232_B_RXD )
);

endmodule

