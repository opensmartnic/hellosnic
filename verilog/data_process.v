`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/03/2023 07:17:12 PM
// Design Name: 
// Module Name: data_process
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module data_process(
    input wire clk, 
    input wire rst_n,
    input wire[31:0] ctrl_reg,  
    input wire[7:0] data_in,
    input wire t_valid, 
    input wire t_ready,
    input wire t_last, 
    output reg[7:0] data_out
    );
 
 assign enable = ctrl_reg[16];
 reg start_process = 1'b0;
 reg ip_matched_0 = 1'b0;
 reg ip_matched_1 = 1'b0;
 reg udp_matched = 1'b0;
 reg port_matched_0 = 1'b0;
 reg port_matched_1 = 1'b0;
 assign port_matched = ip_matched_0 & ip_matched_1 & udp_matched & port_matched_0 & port_matched_1;
 
 reg[12:0] byte_counter = 13'd0;
 always@(posedge clk) begin
    if(!rst_n) begin
        byte_counter <= 13'd0;
        start_process <= 1'b0;
        ip_matched_0 <= 1'b0;
        ip_matched_1 <= 1'b0;
        udp_matched <= 1'b0;
        port_matched_0 <= 1'b0;
        port_matched_1 <= 1'b0;
    end
    else begin
        if(t_valid && t_ready && t_last) begin
            //tlast信号拉高的下一时钟为新数据
            byte_counter <= 13'd0;
            start_process <= 1'b0;
            ip_matched_0 <= 1'b0;
            ip_matched_1 <= 1'b0;
            udp_matched <= 1'b0;
            port_matched_0 <= 1'b0;
            port_matched_1 <= 1'b0;
        end
        else if(t_valid && t_ready) begin     
            case(byte_counter)
                13'd12: ip_matched_0 <= data_in == 8'h08;
                13'd13: ip_matched_1 <= data_in == 8'h00;
                13'd23: udp_matched <= data_in == 8'h11;
                13'd34: port_matched_0 <= data_in == ctrl_reg[15:8];
                13'd35: port_matched_1 <= data_in == ctrl_reg[7:0];
                13'd41: start_process <= 1;
            endcase
            byte_counter <= byte_counter + 13'd1;
        end
    end
end

always @* begin
    if (enable && start_process && port_matched)
        data_out = data_in + 1;
    else
        data_out = data_in;
end 
 
endmodule
