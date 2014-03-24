//
// ICBLBC stack controller
//
// Copyright (c) 2014 Dominic Spill
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


// The stack consists of two parts, the codes and the results. We can trigger a
// copy from code to results when we have identified a full code.
// The main code is stored in a stack (with a counter to store size)
module icblbc_stack(
input wire clock,
input wire start,
output wire complete,
input wire next_code,
input wire copy_results,
input wire [7:0] val_a, val_b,
output reg [3:0] distance
);

reg			[6:0]	state;
parameter	[6:0]	ST_RST		= 6'h00,
					ST_IDLE		= 6'h01;

reg [7:0] stack_size;

icblbc_ram code (
	.address ( addr_codes ),
	.clock ( clock ),
	.data ( data_codes ),
	.wren ( wren_codes ),
	.q ( read_codes )
);

icblbc_ram results (
	.address ( addr_results ),
	.clock ( clock ),
	.data ( data_results ),
	.wren ( wren_results ),
	.q ( read_results )
);

reg start_1, start_2;

always @(posedge clock) begin
	{start_2, start_1} <= {start_1, start};

	case(state)
	ST_RST: begin
		state <= ST_IDLE;
	end
	
	ST_IDLE: begin
			if( start_process_2 ) begin
				if ( copy_results ) begin
				state <= 3;
				a_len <= 1<<(n-1);
				min_b_len <= 2;
			end
			
	end
	
	3: begin
		state <= 4;
	end
	
	4: begin
		state <= 5;
	end
	
	5: begin
		state <= 10;
	end
	

	
	20: begin
		state <= ST_IDLE;
	end
	
	endcase
end

endmodule




