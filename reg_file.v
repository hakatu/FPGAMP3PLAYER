module reg_file
  #(parameter DATA_WIDTH = 8, ADDR_WIDTH = 2)
  (
  input clk, wr_en,
    input [ADDR_WIDTH-1:0] w_addr, r_addr,
    input [DATA_WIDTH-1:0] w_data,
    output [DATA_WIDTH-1:0] r_data);
//signal declaration
  reg [DATA_WIDTH-1:0] array_reg [2**ADDR_WIDTH-1:0];
// body
// write operation
  always@(posedge clk)
    if (wr_en)
      array_reg[w_addr] <= w_data;
// read operation
  assign r_data = array_reg[r_addr];
endmodule
  