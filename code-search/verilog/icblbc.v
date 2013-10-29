//
// ICBLBC functions
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

module hamming_distance (
input wire clock,
input wire [7:0] val_a, val_b,
output reg [7:0] distance
);

wire [7:0] bit_diff;
assign bit_diff = val_a ^ val_b;

always @(posedge clock) begin
	distance <= bit_diff[0] + bit_diff[1] + bit_diff[2] + bit_diff[3]
		+ bit_diff[4] + bit_diff[5] + bit_diff[6] + bit_diff[7];
end
endmodule

//module find_codes (
//
//);
//
//hamming_distance hd (
//	.clock ( clk_50),
//	.val_a ( ham_in_a ),
//	.val_b ( ham_in_b ),
//	.distance (dist)
//);
//
//endmodule