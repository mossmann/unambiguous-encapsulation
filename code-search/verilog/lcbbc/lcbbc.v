//AUTHOR: Tariq Bashir
//tariqbashir@gmail.com
//for unambiguous codes (lcbbc)
//released into the public under GPL


`timescale 1ns/1ps

module lcbbc
  #(parameter N = 3,
    parameter depth=1024,
    parameter width=N,   //same as N
    parameter addr_sz=$clog2(depth)
    )
  (input clk,
   input              rst, //positive reset
   input [N-1:0]      n,
   input [N/2-1:0]      min_hd,
   input              start,
   output reg [N-1:0] codes
   );
  

  localparam [2:0]
    IDLE = 0,
    CALC_HD = 1,
    CLOCK_CALC_HD = 2,
    FIND_CODE = 3,
    RD_MEM = 4,
    OUTPUT_CODES = 5,
    CLOCK_OUTPUT_CODES = 6,
    DONE = 7;
    

  reg [2:0]           state, nxt_state;
  reg [N-1:0]         counterl, nxt_counterl, counterh, nxt_counterh;
  
  reg [addr_sz-1:0]   codelength, nxt_codelength;
  reg [addr_sz-1:0]   count, nxt_count;
  
  //sequential block
  always @(posedge clk)
    if(rst)
      begin
        state         <= #1 IDLE;
        counterl      <= #1 0;
        codelength    <= #1 0;
	count         <= #1 0;
        counterh      <= #1 0;
      end
    else
      begin
        state         <= #1 nxt_state;
        counterl      <= #1 nxt_counterl;
        counterh      <= #1 nxt_counterh; 
        codelength    <= #1 nxt_codelength;
	count         <= #1 nxt_count; 
      end
  
  

 
  reg [N-1:0]         summation;
  reg [N-1:0]         candidate;
  
  reg                 wr_en, rd_en;
  reg [width-1:0]     d_in;
  reg [addr_sz-1:0]   wr_addr;
  reg [width-1:0]     d_out;
  reg [addr_sz-1:0]   rd_addr, r_addr;
  
  reg [width-1:0]     code_array[0:depth-1]; 
  
  
  //memory
  always @(posedge clk)
    begin
      if (wr_en)
        begin
          code_array[wr_addr] <= #1 d_in;
        end
      else if (rd_en)
        begin
          d_out <= #1 code_array[rd_addr];
        end
    end // always @ (posedge clk)

  
  //Finite STATE MACHINE
  always@*
    begin
      //defaults
      nxt_state         = state;
      nxt_counterl      = counterl;
      nxt_counterh      = counterh;
      nxt_count         = count;
      nxt_codelength    = codelength;
      
      case(state)
        IDLE:
          begin
            if(start)
              begin
                wr_addr = 0;
                wr_en = 1;
                d_in = 0;
                nxt_codelength = 1;
                nxt_state = CALC_HD;
              end
          end
        
        CALC_HD:
          begin
            wr_en = 0;
            if( (counterh == 1 ) || (codelength == depth -1) )
              nxt_state = OUTPUT_CODES;
            else
              begin
                if(counterl == 2**N-1)
                  nxt_counterh = counterh + 1;
                
               	summation = sum(counterl ^ counterh); //function call
		if( summation >= min_hd )
		  begin
		    //HD[counterl] = counterl;
		    candidate = counterl;
		    nxt_counterl = counterl + 1;
		    nxt_state = FIND_CODE;
		  end
		else
		  begin
		    nxt_counterl = counterl + 1;
                    nxt_state = CLOCK_CALC_HD;
                  end
	      end // else: !if( (counterh == 1 ) || (codelength == depth -1) )
            
          end // case: CALC_HD

        CLOCK_CALC_HD:
          begin
            nxt_state = CALC_HD;
          end
        
        FIND_CODE:
          begin
            if(count < codelength)
              begin
                rd_en = 1;
                rd_addr = count;
                nxt_state = RD_MEM;
              end
	    else if(count == codelength)
	      begin
		wr_en = 1;
		d_in = candidate;
		wr_addr = codelength;
		nxt_codelength = codelength + 1;
		nxt_count = 0;
		nxt_state = CALC_HD;
	      end
          end  //FIND_CODE

       
        RD_MEM:
          begin
            rd_en = 0;
            summation = sum(d_out ^ candidate);
            if (summation >= min_hd)
              begin
                nxt_count = count + 1;
		nxt_state = FIND_CODE;
              end
	    else
	      begin
             	nxt_count = 0;
		nxt_state = CALC_HD;
	      end
          end
        
        OUTPUT_CODES:
          begin
            if( count < codelength)
              begin
		rd_en = 1;
		rd_addr = count;
		nxt_count = count + 1;
		nxt_state = CLOCK_OUTPUT_CODES;
	      end
            else
              nxt_state = DONE;
          end // case: OUTPUT_CODE
        
        
        CLOCK_OUTPUT_CODES:
          begin
	    rd_en = 0;
	    codes = d_out; 
            nxt_state = OUTPUT_CODES;
          end

        DONE:
          begin
            $display($time,": Done \n");
            $finish;
          end

      endcase // case (state)
      
    end // always@ *
  
  
  
  
  function [N-1:0] sum(input [N-1:0] input_vector);
    integer       k;
    reg [N-1:0]   temp;
    
    begin
      temp = 0;
      for(k=0; k < N; k=k+1)
        begin
          temp = temp + input_vector[k];
        end
      sum = temp;
    end
    
  endfunction // sum
  

  

  
endmodule // lcbbc
