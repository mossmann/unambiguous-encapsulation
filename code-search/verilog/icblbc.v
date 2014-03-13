//
// ICBLBC functions
//
// Copyright (c) 2013-2014 Dominic Spill
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

module hamming_distance (
input wire clock,
input wire [7:0] val_a, val_b,
output reg [3:0] distance
);

wire [7:0] bit_diff;
assign bit_diff = val_a ^ val_b;

always @(posedge clock) begin
	// Unless I misunderstood him, Marshall said this should work
	distance = bit_diff[0] + bit_diff[1] + bit_diff[2] + bit_diff[3]
			 + bit_diff[4] + bit_diff[5] + bit_diff[6] + bit_diff[7];
end
endmodule


module populate_candidates (
input wire clock,
input wire [7:0] code,
input wire [7:0] base_candidate_addr,
output reg [7:0] addr_candidates,
input wire [7:0] read_candidates,
input wire [7:0] candidate_len,
input wire [7:0] base_next_cand_addr,
output reg [7:0] addr_next_cand,
output reg [7:0] data_next_cand,
output wire wren_next_cand,
output reg [7:0] next_cand_len,
input wire [3:0] min_dist,
input wire start,
output wire complete
);

wire [3:0] distance;
reg [7:0] ham_in_a, ham_in_b;

hamming_distance hd (
	.clock ( clock),
	.val_a ( ham_in_a ),
	.val_b ( ham_in_b ),
	.distance ( distance )
);

reg done;
assign complete = (done == 1 || state == ST_DONE);
reg write_cand;
assign wren_next_cand = write_cand;
reg [7:0] icand, inext;
reg start_1, start_2;
reg			[5:0]	state;
parameter	[5:0]	ST_RST		= 6'h00,
					ST_IDLE		= 6'h01,
					ST_DONE		= 6'h07;

always @(posedge clock) begin
	{start_2, start_1} <= {start_1, start};

	case(state)
	ST_RST: begin
		done <= 0;
		state <= ST_IDLE;
	end
	
	ST_IDLE: begin
			if( start_2 ) begin
				ham_in_a <= code;
				icand <= 0;
				inext <= 0;
				state <= 3;
				done <= 0;
			end
			
	end
	
	// Read next candidate
	3: begin
		addr_candidates <= base_candidate_addr + icand;
		state <= 4;
	end
	
	// 
	4: begin
		ham_in_b <= read_candidates;
		state <= 5;
	end
	
	5: begin
		if ( distance >= min_dist ) begin
			data_next_cand <= read_candidates;
			addr_next_cand <= base_next_cand_addr + inext;
			write_cand <= 1;
		end
		state <= 6;
	end
	
	6: begin
		// Unsure if I need to wait until here to increment inext
		if ( distance >= min_dist ) begin
			inext <= inext + 1;
		end

		write_cand <= 0;
		icand <= icand + 1;
		
		if ( icand > candidate_len ) begin
			done <= 1;
			next_cand_len <= inext;
			state <= 7;
		end else begin
			state <= 3;
		end
	end
	
	ST_DONE: begin
		state <= ST_IDLE;
	end
	endcase
end
endmodule


module find_iso (
input clock,
input wire start,
output wire complete
);

reg start_1, start_2;
reg			[5:0]	state;
parameter	[5:0]	ST_RST		= 6'h00,
					ST_IDLE		= 6'h01;

always @(posedge clock) begin
	{start_2, start_1} <= {start_1, start};

	case(state)
	ST_RST: begin
		state <= ST_IDLE;
	end
	
	ST_IDLE: begin
			if( start_2 ) begin
				state <= 3;
			end
	end
	
	3: begin
		state <= 4;
	end
	
	4: begin
		state <= 5;
	end
	
	5: begin
		state <= 6;
	end
	
	6: begin
		state <= ST_IDLE;
	end
	endcase
end
endmodule



module find_best_iso (
input wire clock,
input wire [7:0] min_hd,
input wire [7:0] min_iso,
input wire start_process,
output reg complete
);

parameter n = 5;
parameter MAX_N = 8;
parameter MAX_CAND = 2**MAX_N;

reg			[5:0]	state;
parameter	[5:0]	ST_RST		= 6'h00,
					ST_IDLE		= 6'h01;

reg [7:0] count, icand;

reg [7:0] a_len, min_b_len;


reg start_process_1, start_process_2;

// Storage for sets of codes
wire wren_codes;
wire [7:0] read_codes;
wire [10:0] addr_codes;
wire [7:0] data_codes;

icblbc_ram codes (
	.address ( addr_codes ),
	.clock ( clock ),
	.data ( data_codes ),
	.wren ( wren_codes ),
	.q ( read_codes )
);

reg fi_en;

find_iso fi (
	.clock ( clock ),
	.start ( fi_en ),
	.complete ( fi_done )
	
);

always @(posedge clock) begin
	{start_process_2, start_process_1} <= {start_process_1, start_process};

	case(state)
	ST_RST: begin
		state <= ST_IDLE;
	end
	
	ST_IDLE: begin
			if( start_process_2 ) begin
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
	
	// Wait for find_iso() to finish
	10: begin
		fi_en <= 1;
		state <= 11;
	end
	
	11: begin
		if( fi_done ) begin
			state <= 12;
		end
	end
	
	12: begin
		state <= 13;
	end
	
	20: begin
		state <= ST_IDLE;
	end
	
	endcase
end
endmodule
