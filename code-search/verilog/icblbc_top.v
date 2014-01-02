//
// ICBLBC with usb 2.0 to host,
// for DE2-115 board and SMSC USB3300 PHY
// top-level
//
// Copyright (c) 2013 Dominic Spill
//
// This file is part of Unambiguous Encapsulation.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street,
// Boston, MA 02110-1301, USA.
//

module icblbc_top (
	
input	wire			clk_50,
	
input	wire			usb_ulpi_clk,
inout	wire	[7:0]	usb_ulpi_d,
input	wire			usb_ulpi_dir,
output	wire			usb_ulpi_stp,
input	wire			usb_ulpi_nxt,

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

	wire	[17:0]	led_r;
	assign LEDR = led_r;
	
	assign led_r[17:16] = dbg_linestate;
	assign led_r[15] = usb_connected;
	assign led_r[14] = usb_configured;
	assign led_r[10:0] = dbg_frame_num;
	
	wire 	[8:0]		led_g;
	assign LEDG = led_g;
	
	assign led_g[0] = vend_req_act;
	
	wire	[17:0]	sw = SW;
	wire 	[3:0]		key = ~KEY;
	
	reg			reset;

always @(posedge clk_50) begin
	reset <= key[0];
	count_1 <= count_1 + 1'b1;
end

	reg	[26:0]	count_1;
	reg	[26:0]	count_2;
		
always @(posedge usb_ulpi_clk) begin
	count_2 <= count_2 + 1'b1;
end


////////////////////////////////////////////////////////////
//
// USB 2.0 controller
//
////////////////////////////////////////////////////////////

	wire			usb_reset_n;
	wire			usb_connected;
	wire			usb_configured;
	wire	[1:0]	dbg_linestate;
	wire	[10:0]	dbg_frame_num;

usb2_top	iu2 (
	.ext_clk					( clk_50 ),
	.reset_n					( ~reset ),
	.reset_n_out			( usb_reset_n ),
	
	.opt_disable_all			( 1'b0 ),
	.opt_enable_hs			( 1'b1 ),
	.opt_ignore_vbus			( 1'b0 ),
	.stat_connected		( usb_connected ),
	.stat_configured		( usb_configured ),
	
	.phy_ulpi_clk			( usb_ulpi_clk ),
	.phy_ulpi_d				( usb_ulpi_d ),
	.phy_ulpi_dir			( usb_ulpi_dir ),
	.phy_ulpi_stp			( usb_ulpi_stp ),
	.phy_ulpi_nxt			( usb_ulpi_nxt ),
	
	.buf_in_addr			( buf_in_addr ),
	.buf_in_data			( buf_in_data ),
	.buf_in_wren			( buf_in_wren ),
	.buf_in_ready			( buf_in_ready ),
	.buf_in_commit			( buf_in_commit ),
	.buf_in_commit_len	( buf_in_commit_len ),
	.buf_in_commit_ack	( buf_in_commit_ack ),
	
	.buf_out_addr			( buf_out_addr ),
	.buf_out_q				( buf_out_q ),
	.buf_out_len			( buf_out_len ),
	.buf_out_hasdata		( buf_out_hasdata ),
	.buf_out_arm			( buf_out_arm ),
	.buf_out_arm_ack		( buf_out_arm_ack ),
	
	.vend_req_act			( vend_req_act ),
	.vend_req_request		( vend_req_request ),
	.vend_req_val			( vend_req_val ),
	
	.dbg_linestate			( dbg_linestate ),
	.dbg_frame_num			( dbg_frame_num )
);


/////////////////////////////////////////////
//
// ICBLBC
//
/////////////////////////////////////////////

	reg	[8:0]		buf_in_addr;
	reg	[7:0]		buf_in_data;
	reg				buf_in_wren;
	wire				buf_in_ready;
	reg				buf_in_commit;
	reg	[9:0]		buf_in_commit_len;
	wire				buf_in_commit_ack;
	
	wire	[8:0]		buf_out_addr;
	wire	[7:0]		buf_out_q;
	wire	[9:0]		buf_out_len;
	wire				buf_out_hasdata;
	wire				buf_out_arm;
	wire				buf_out_arm_ack;
	
	wire				vend_req_act;
	wire	[7:0]		vend_req_request;
	wire	[15:0]	vend_req_val;

//io_seg7 is7 (
//	.disp_in	( vend_req_request[15:12] ),
//	.disp_out	( HEX7 )
//);
//
//io_seg7 is6 (
//	.disp_in	( vend_req_request[11:8] ),
//	.disp_out	( HEX6 )
//);

//io_seg7 is5 (
//	.disp_in	( vend_req_request[7:4] ),
//	.disp_out	( HEX5 )
//);

io_seg7 is4 (
	.disp_in	( dist[3:0] ),
	.disp_out	( HEX4 )
);

io_seg7 is3 (
	.disp_in	( vend_req_val[15:12] ),
	.disp_out	( HEX3 )
);

io_seg7 is2 (
	.disp_in	( vend_req_val[11:8] ),
	.disp_out	( HEX2 )
);

io_seg7 is1 (
	.disp_in	( vend_req_val[7:4] ),
	.disp_out	( HEX1 )
);

io_seg7 is0 (
	.disp_in	( vend_req_val[3:0] ),
	.disp_out	( HEX0 )
);

wire [3:0] dist;
reg [7:0] ham_in_a, ham_in_b;

hamming_distance hd (
	.clock ( clk_50 ),
	.val_a ( ham_in_a ),
	.val_b ( ham_in_b ),
	.distance (dist)
);

find_best_iso fbi (
		.clock ( clk_50 )
);

reg			[5:0]	state;
parameter	[5:0]	ST_RST		= 6'h00,
					ST_IDLE		= 6'h01;

reg		buf_in_ready_1;
reg		buf_in_commit_ack_1;
reg		vend_req_act_1, vend_req_act_2;

always @(posedge clk_50) begin
	{vend_req_act_2, vend_req_act_1} <= {vend_req_act_1, vend_req_act};
	buf_in_ready_1 <= buf_in_ready;
	buf_in_commit_ack_1 <= buf_in_commit_ack;
	
	buf_in_wren <= 0;
	buf_in_commit <= 0;
	
	case(state)
	ST_RST: begin
		state <= ST_IDLE;
	end
	
	ST_IDLE: begin
		if( usb_configured & buf_in_ready_1 & vend_req_act_1 & ~vend_req_act_2 ) begin
			{ ham_in_a, ham_in_b } <= vend_req_val;
			state <= 9;
		end
	end
	
	9: begin
		state <= 10;
	end
	
	10: begin
		buf_in_data <= { 1'h0, dist[3:0] };
		buf_in_addr <= 0;
		buf_in_wren <= 1;
		state <= 18;
	end

	18: begin
		buf_in_commit <= 1;
		buf_in_commit_len <= 1;
		if( buf_in_commit_ack_1 ) begin
			state <= 19;
		end
	end
	
	19: begin
		if( buf_in_commit_ack_1 == 0 ) begin
			state <= ST_IDLE;
		end
	end
	
	endcase
end

endmodule
