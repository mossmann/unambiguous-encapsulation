// ICNBC functions
// Copyright (c) 2014 Dominic Spill, Tariq Bashir
// This file is part of Unambiguous Encapsulation
// License: GPL v2
//

`timescale 1ns/1ps

module tb_icnbc;
 
  parameter N = 8;
  parameter MIN_LD = 2;
  parameter MEM_DEPTH=256;

  reg clk;
  reg rst;
  reg [N-1:0] n;
  reg         start;
  reg [N-1:0] min_ld;
  wire [N-1:0] codes;

  initial
    begin
      clk <= 1'b0;
      rst = 1'b1;
    end
  
  always
    #5 clk = ~clk;


  initial
    begin
      repeat(10) @(negedge clk);
      rst = 1'b0; //disable reset
      @(negedge clk);
      start = 1;
      n = N;
      min_ld = MIN_LD;
    end
  
  
  /* icnbc AUTO_TEMPLATE
   (
   
   );
  */

  icnbc #(.N(N),.depth(MEM_DEPTH),.width(N)) LCBBC_INST
    (/*AUTOINST*/
     // Outputs
     .codes                             (codes[N-1:0]),
     // Inputs
     .clk                               (clk),
     .rst                               (rst),
     .n                                 (n[N-1:0]),
     .min_ld                            (min_ld[N-1:0]),
     .start                             (start));
  
  
/* -----\/----- EXCLUDED -----\/-----
  initial
    begin
      #15000;
      $finish;
    end

 -----/\----- EXCLUDED -----/\----- */
  initial
    begin
      $monitor("codeword = %b\t ",codes);
    end
  

endmodule // tb_icnbc
