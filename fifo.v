module fifo
  #(
    parameter DATA_WIDTH = 8, // number of bits in a word
    		  ADDR_WIDTH = 4  // number of address bits
  )
  (
    input clk, reset_n, rd, wr,
    input [DATA_WIDTH-1:0] w_data,
    output empty, full,
    output [DATA_WIDTH-1:0] r_data);
  
  // signal declaration
  wire [ADDR_WIDTH-1:0] w_addr, r_addr;
  wire wr_en, full_tmp;
  
  //body
  //write enabled only when FIFO is not full
  assign wr_en = wr & ~ full_tmp;
  assign full = full_tmp;
  
  //instantiate FIFO control unit
  fifo_ctrl #(.ADDR_WIDTH(ADDR_WIDTH)) c_unit
  (.clk(clk), .reset_n(reset_n), .rd(rd), .wr(wr), .empty(empty),
   .full(full_tmp), .w_addr(w_addr), .r_addr(r_addr), .r_addr_next());
  
  //instantitate register file
  reg_file #(.DATA_WIDTH(DATA_WIDTH), .ADDR_WIDTH(ADDR_WIDTH)) r_unit
  (.clk(clk), .wr_en(wr_en), .w_addr(w_addr), .r_addr(r_addr), 
   .w_data(w_data), .r_data(r_data));
endmodule