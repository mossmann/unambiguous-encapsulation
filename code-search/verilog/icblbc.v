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
output reg [3:0] distance
);

wire [7:0] bit_diff;
assign bit_diff = val_a ^ val_b;

always @(posedge clock) begin
	case (clock)
		0:  	distance <= 5'h0;
		1:  	distance <= 5'h1;
		2:  	distance <= 5'h1;
		3:  	distance <= 5'h2;
		4:  	distance <= 5'h1;
		5:  	distance <= 5'h2;
		6:  	distance <= 5'h2;
		7:  	distance <= 5'h3;
		8:  	distance <= 5'h1;
		9:  	distance <= 5'h2;
		10: 	distance <= 5'h2;
		11: 	distance <= 5'h3;
		12: 	distance <= 5'h2;
		13: 	distance <= 5'h3;
		14: 	distance <= 5'h3;
		15: 	distance <= 5'h4;
		16: 	distance <= 5'h1;
		17: 	distance <= 5'h2;
		18: 	distance <= 5'h2;
		19: 	distance <= 5'h3;
		20: 	distance <= 5'h2;
		21: 	distance <= 5'h3;
		22: 	distance <= 5'h3;
		23: 	distance <= 5'h4;
		24: 	distance <= 5'h2;
		25: 	distance <= 5'h3;
		26: 	distance <= 5'h3;
		27: 	distance <= 5'h4;
		28: 	distance <= 5'h3;
		29: 	distance <= 5'h4;
		30: 	distance <= 5'h4;
		31: 	distance <= 5'h5;
		32: 	distance <= 5'h1;
		33: 	distance <= 5'h2;
		34: 	distance <= 5'h2;
		35: 	distance <= 5'h3;
		36: 	distance <= 5'h2;
		37: 	distance <= 5'h3;
		38: 	distance <= 5'h3;
		39: 	distance <= 5'h4;
		40: 	distance <= 5'h2;
		41: 	distance <= 5'h3;
		42: 	distance <= 5'h3;
		43: 	distance <= 5'h4;
		44: 	distance <= 5'h3;
		45: 	distance <= 5'h4;
		46: 	distance <= 5'h4;
		47: 	distance <= 5'h5;
		48: 	distance <= 5'h2;
		49: 	distance <= 5'h3;
		50: 	distance <= 5'h3;
		51: 	distance <= 5'h4;
		52: 	distance <= 5'h3;
		53: 	distance <= 5'h4;
		54: 	distance <= 5'h4;
		55: 	distance <= 5'h5;
		56: 	distance <= 5'h3;
		57: 	distance <= 5'h4;
		58: 	distance <= 5'h4;
		59: 	distance <= 5'h5;
		60: 	distance <= 5'h4;
		61: 	distance <= 5'h5;
		62: 	distance <= 5'h5;
		63: 	distance <= 5'h6;
		64: 	distance <= 5'h1;
		65: 	distance <= 5'h2;
		66: 	distance <= 5'h2;
		67: 	distance <= 5'h3;
		68: 	distance <= 5'h2;
		69: 	distance <= 5'h3;
		70: 	distance <= 5'h3;
		71: 	distance <= 5'h4;
		72: 	distance <= 5'h2;
		73: 	distance <= 5'h3;
		74: 	distance <= 5'h3;
		75: 	distance <= 5'h4;
		76: 	distance <= 5'h3;
		77: 	distance <= 5'h4;
		78: 	distance <= 5'h4;
		79: 	distance <= 5'h5;
		80: 	distance <= 5'h2;
		81: 	distance <= 5'h3;
		82: 	distance <= 5'h3;
		83: 	distance <= 5'h4;
		84: 	distance <= 5'h3;
		85: 	distance <= 5'h4;
		86: 	distance <= 5'h4;
		87: 	distance <= 5'h5;
		88: 	distance <= 5'h3;
		89: 	distance <= 5'h4;
		90: 	distance <= 5'h4;
		91: 	distance <= 5'h5;
		92: 	distance <= 5'h4;
		93: 	distance <= 5'h5;
		94: 	distance <= 5'h5;
		95: 	distance <= 5'h6;
		96: 	distance <= 5'h2;
		97: 	distance <= 5'h3;
		98: 	distance <= 5'h3;
		99: 	distance <= 5'h4;
		100:	distance <= 5'h3;
		101:	distance <= 5'h4;
		102:	distance <= 5'h4;
		103:	distance <= 5'h5;
		104:	distance <= 5'h3;
		105:	distance <= 5'h4;
		106:	distance <= 5'h4;
		107:	distance <= 5'h5;
		108:	distance <= 5'h4;
		109:	distance <= 5'h5;
		110:	distance <= 5'h5;
		111:	distance <= 5'h6;
		112:	distance <= 5'h3;
		113:	distance <= 5'h4;
		114:	distance <= 5'h4;
		115:	distance <= 5'h5;
		116:	distance <= 5'h4;
		117:	distance <= 5'h5;
		118:	distance <= 5'h5;
		119:	distance <= 5'h6;
		120:	distance <= 5'h4;
		121:	distance <= 5'h5;
		122:	distance <= 5'h5;
		123:	distance <= 5'h6;
		124:	distance <= 5'h5;
		125:	distance <= 5'h6;
		126:	distance <= 5'h6;
		127:	distance <= 5'h7;
		128:	distance <= 5'h1;
		129:	distance <= 5'h2;
		130:	distance <= 5'h2;
		131:	distance <= 5'h3;
		132:	distance <= 5'h2;
		133:	distance <= 5'h3;
		134:	distance <= 5'h3;
		135:	distance <= 5'h4;
		136:	distance <= 5'h2;
		137:	distance <= 5'h3;
		138:	distance <= 5'h3;
		139:	distance <= 5'h4;
		140:	distance <= 5'h3;
		141:	distance <= 5'h4;
		142:	distance <= 5'h4;
		143:	distance <= 5'h5;
		144:	distance <= 5'h2;
		145:	distance <= 5'h3;
		146:	distance <= 5'h3;
		147:	distance <= 5'h4;
		148:	distance <= 5'h3;
		149:	distance <= 5'h4;
		150:	distance <= 5'h4;
		151:	distance <= 5'h5;
		152:	distance <= 5'h3;
		153:	distance <= 5'h4;
		154:	distance <= 5'h4;
		155:	distance <= 5'h5;
		156:	distance <= 5'h4;
		157:	distance <= 5'h5;
		158:	distance <= 5'h5;
		159:	distance <= 5'h6;
		160:	distance <= 5'h2;
		161:	distance <= 5'h3;
		162:	distance <= 5'h3;
		163:	distance <= 5'h4;
		164:	distance <= 5'h3;
		165:	distance <= 5'h4;
		166:	distance <= 5'h4;
		167:	distance <= 5'h5;
		168:	distance <= 5'h3;
		169:	distance <= 5'h4;
		170:	distance <= 5'h4;
		171:	distance <= 5'h5;
		172:	distance <= 5'h4;
		173:	distance <= 5'h5;
		174:	distance <= 5'h5;
		175:	distance <= 5'h6;
		176:	distance <= 5'h3;
		177:	distance <= 5'h4;
		178:	distance <= 5'h4;
		179:	distance <= 5'h5;
		180:	distance <= 5'h4;
		181:	distance <= 5'h5;
		182:	distance <= 5'h5;
		183:	distance <= 5'h6;
		184:	distance <= 5'h4;
		185:	distance <= 5'h5;
		186:	distance <= 5'h5;
		187:	distance <= 5'h6;
		188:	distance <= 5'h5;
		189:	distance <= 5'h6;
		190:	distance <= 5'h6;
		191:	distance <= 5'h7;
		192:	distance <= 5'h2;
		193:	distance <= 5'h3;
		194:	distance <= 5'h3;
		195:	distance <= 5'h4;
		196:	distance <= 5'h3;
		197:	distance <= 5'h4;
		198:	distance <= 5'h4;
		199:	distance <= 5'h5;
		200:	distance <= 5'h3;
		201:	distance <= 5'h4;
		202:	distance <= 5'h4;
		203:	distance <= 5'h5;
		204:	distance <= 5'h4;
		205:	distance <= 5'h5;
		206:	distance <= 5'h5;
		207:	distance <= 5'h6;
		208:	distance <= 5'h3;
		209:	distance <= 5'h4;
		210:	distance <= 5'h4;
		211:	distance <= 5'h5;
		212:	distance <= 5'h4;
		213:	distance <= 5'h5;
		214:	distance <= 5'h5;
		215:	distance <= 5'h6;
		216:	distance <= 5'h4;
		217:	distance <= 5'h5;
		218:	distance <= 5'h5;
		219:	distance <= 5'h6;
		220:	distance <= 5'h5;
		221:	distance <= 5'h6;
		222:	distance <= 5'h6;
		223:	distance <= 5'h7;
		224:	distance <= 5'h3;
		225:	distance <= 5'h4;
		226:	distance <= 5'h4;
		227:	distance <= 5'h5;
		228:	distance <= 5'h4;
		229:	distance <= 5'h5;
		230:	distance <= 5'h5;
		231:	distance <= 5'h6;
		232:	distance <= 5'h4;
		233:	distance <= 5'h5;
		234:	distance <= 5'h5;
		235:	distance <= 5'h6;
		236:	distance <= 5'h5;
		237:	distance <= 5'h6;
		238:	distance <= 5'h6;
		239:	distance <= 5'h7;
		240:	distance <= 5'h4;
		241:	distance <= 5'h5;
		242:	distance <= 5'h5;
		243:	distance <= 5'h6;
		244:	distance <= 5'h5;
		245:	distance <= 5'h6;
		246:	distance <= 5'h6;
		247:	distance <= 5'h7;
		248:	distance <= 5'h5;
		249:	distance <= 5'h6;
		250:	distance <= 5'h6;
		251:	distance <= 5'h7;
		252:	distance <= 5'h6;
		253:	distance <= 5'h7;
		254:	distance <= 5'h7;
		255:	distance <= 5'h8;
	endcase
end
endmodule


module populate_candidates (
input wire clock,
input wire [7:0] code,
output reg [7:0] addr_candidates,
input wire [7:0] read_candidates,
input wire [7:0] candidate_len,
output reg [7:0] addr_next_cand,
output reg [7:0] data_next_cand,
output reg [7:0] wren_next_cand,
input wire [3:0] min_dist,
input wire start,
output reg complete
);

wire [3:0] distance;
reg [7:0] ham_in_a, ham_in_b;

hamming_distance hd (
	.clock ( clock),
	.val_a ( ham_in_a ),
	.val_b ( ham_in_b ),
	.distance ( distance )
);

reg count = 0;
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
				ham_in_a <= code;
				state <= 3;
			end
			
	end
	
	// Read next candidate
	3: begin
		addr_candidates <= count;
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
			addr_next_cand <= count;
			wren_next_cand <= 1;
		end
		state <= 6;
	end
	
	6: begin
		wren_next_cand <= 0;
		count <= count + 1;
		
		if ( count >= candidate_len ) begin
			complete <= 1;
			state <= 7;
		end else begin
			state <= 3;
		end
	end
	
	7: begin
		complete <= 0;
		state <= ST_IDLE;
	end
	endcase
end
endmodule


module find_comp (
input clock
);


endmodule


module find_iso (
input clock
);


endmodule



module find_iso_from_start (
input wire clock,
input wire [7:0] start,
input wire [7:0] n,
input wire [7:0] min_hd,
input wire [7:0] min_iso,
input wire [7:0] a_len,
input wire [7:0] min_b_len,
input wire start_process,
output reg complete
);

parameter MAX_N = 8;
parameter MAX_CAND = 2**MAX_N;

reg			[5:0]	state;
parameter	[5:0]	ST_RST		= 6'h00,
					ST_IDLE		= 6'h01;


wire [3:0] distance;
reg [7:0] ham_in_a, ham_in_b;
reg [7:0] count, icand;

reg start_process_1, start_process_2;

// Storage for sets of codes
reg wren_codes, wren_next_cand, wren_next_b_cand;
wire [7:0] read_codes, read_next_cand, read_next_b_cand;
reg [7:0] addr_codes, addr_next_cand, addr_next_b_cand;
reg [7:0] data_codes, data_next_cand, data_next_b_cand;

icblbc_ram codes (
	.address ( addr_codes ),
	.clock ( clock ),
	.data ( data_codes ),
	.wren ( wren_codes ),
	.q ( read_codes )

);

icblbc_ram next_candidates (
	.address ( addr_next_cand ),
	.clock ( clock ),
	.data ( data_next_cand ),
	.wren ( wren_next_cand ),
	.q ( read_next_cand )

);

icblbc_ram next_b_candidates (
	.address ( addr_next_b_cand ),
	.clock ( clock ),
	.data ( data_next_b_cand ),
	.wren ( wren_next_b_cand ),
	.q ( read_next_b_cand )

);

reg find_iso_en;
wire find_iso_done;

find_iso fi (
	.clock ( clock )
//	.start_process ( find_iso_en ),
//	.complete ( find_iso_done )
	
);

always @(posedge clock) begin
	{start_process_2, start_process_1} <= {start_process_1, start_process};

	case(state)
	ST_RST: begin
		state <= ST_IDLE;
	end
	
	ST_IDLE: begin
			if( start_process_2 ) begin
				count <= 0;
				icand <= 2**n;
				state <= 3;
				addr_next_cand <= 0;
				addr_next_b_cand <= 0;
				ham_in_a <= start;
			end
			
	end
	
	3: begin
		ham_in_b <= count;
		state <= 4;
	end
	
	4: begin
		// Delay for hamming distance to finish
		state <= 5;
	end
	
	5: begin
		if ( distance >= min_hd ) begin
			data_next_cand <= count;
			addr_next_cand <= addr_next_b_cand + 1;
			wren_next_cand <= 1;
		end
		if ( distance >= min_iso ) begin
			data_next_b_cand <= count;
			addr_next_b_cand <= addr_next_b_cand + 1;
			wren_next_b_cand <= 1;
		end
		state <= 6;
	end
	
	6: begin
		wren_next_cand <= 0;
		wren_next_b_cand <= 0;
		count <= count + 1;
		
		if ( count >= icand ) begin
			state <= 7;
			find_iso_en <= 1;
		end else begin
			state <= 3;
		end
	end
	
	7: begin
		// Wait for find_iso() to complete
		if ( find_iso_done ) begin
			state <= 8;
		end
	end
	
	8: begin
		// Tidy up output (as needed) and return it to the parent
		state <= 9;
	end
	
	9: begin
		complete <= 1;
		state <= ST_IDLE;
	end
	
	
	endcase
end
endmodule
