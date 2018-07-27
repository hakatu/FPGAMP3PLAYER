module adc_dac(
	input clk, reset_n,
	input [31:0] dac_data_in,
	output [31:0] adc_data_out,
	output m_clk, b_clk, dac_lr_clk, adc_lr_clk,
	output dacdat,
	input adcdat,
	output load_done_tick);

//symbolic constants
localparam M_DVSR = 2;
localparam B_DVSR = 3;
localparam LR_DVSR = 5;

//signal declaration
reg [M_DVSR-1:0] m_reg;
reg [B_DVSR-1:0] b_reg;
reg [LR_DVSR-1:0] lr_reg;
reg [31:0] dac_buf_reg, adc_buf_reg;
reg lr_delayed_reg, b_delayed_reg;
wire [M_DVSR-1:0] m_next;
wire [B_DVSR-1:0] b_next;
wire [LR_DVSR-1:0] lr_next;
wire [31:0] dac_buf_next, adc_buf_next;
wire m_12_5m_tick, load_tick, b_neg_tick, b_pos_tick;

//clock signal generator for codec digital audio interface
//registers
always@(posedge clk, posedge reset_n)
if(reset_n)
	begin
		m_reg <= 0;
		b_reg <= 0;
		lr_reg <= 0;
		dac_buf_reg <= 0;
		adc_buf_reg <= 0;
		b_delayed_reg <= 0;
		lr_delayed_reg <= 0;
	end
else
	begin
		m_reg <= m_next;
		b_reg <= b_next;
		lr_reg <= lr_next;
		dac_buf_reg <= dac_buf_next;
		adc_buf_reg <= adc_buf_next;
		b_delayed_reg <= b_reg[B_DVSR-1];
		lr_delayed_reg <= lr_reg[LR_DVSR-1];
	end
// codec 12.5MHz m_clk 
assign m_next = m_reg +1;
assign m_clk = m_reg[M_DVSR-1];
assign m_12_5m_tick = (m_reg==0)?1'b1 : 1'b0;
//b_clk ( m_clk/8 = 2*16*48k)
assign b_next = m_12_5m_tick ? b_reg+1 : b_reg;
assign b_clk = b_reg[B_DVSR-1];
//negedge of b_clk
assign b_neg_tick = b_delayed_reg & ~b_reg[B_DVSR-1];
//posedge of b_clk
assign b_pos_tick = ~b_delayed_reg & b_reg[B_DVSR-1];
//adc/dac_lr_clk ( b_clk/32 = 48k)
assign lr_next = b_neg_tick ? lr_reg +1 : lr_reg;
assign dac_lr_clk = lr_reg[LR_DVSR-1];
assign adc_lr_clk = lr_reg[LR_DVSR-1];
//load DAC tick at 0-1 transition of dac_lr_clk
assign load_tick = ~lr_delayed_reg & lr_reg[LR_DVSR-1];
assign load_done_tick = load_tick;
//DAC buffer to shift out data at negedge b_clk
assign dac_buf_next = load_tick ? dac_data_in :
							 b_neg_tick ? {dac_buf_reg[30:0],1'b0}: dac_buf_reg;
assign dacdat = dac_buf_reg[31];
//ADC buffer to shift in data 
//data shifted out at negedge b_clk from codec
//use posedge to latch in ADC data for stability
assign adc_buf_next = b_pos_tick ? {adc_buf_reg[30:0], adcdat}:adc_buf_reg;
assign adc_data_out = adc_buf_reg;
endmodule




















