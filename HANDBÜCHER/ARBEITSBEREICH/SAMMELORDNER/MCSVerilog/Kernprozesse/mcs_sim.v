module mcs_sim (
    input wire clk,
    input wire [7:0] cmd,
    input wire [31:0] src_addr,
    input wire [31:0] dst_addr,
    output reg led
);
    always @(posedge clk) begin
        case (cmd)
            8'b00000001: led <= 1; // echo
            8'b00000010: led <= 1; // send
            8'b00000011: led <= 1; // copy
            8'b00000100: led <= 1; // to
            default: led <= 0;
        endcase
    end
endmodule
