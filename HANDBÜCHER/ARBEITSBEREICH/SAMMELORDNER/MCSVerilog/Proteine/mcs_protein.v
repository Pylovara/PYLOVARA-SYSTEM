module mcs_protein (
    input wire clk,
    input wire [7:0] cmd_in,
    input wire [31:0] src_addr_in,
    input wire [31:0] dst_addr_in,
    input wire [7:0] data_in,
    output reg [7:0] data_out,
    output reg [31:0] src_addr_out,
    output reg [31:0] dst_addr_out,
    input wire executable,
    output reg valid
);

    parameter STATE_IDLE  = 3'b000;
    parameter STATE_CMD   = 3'b001;
    parameter STATE_SRC   = 3'b010;
    parameter STATE_DST   = 3'b011;
    parameter STATE_SYNC  = 3'b100;
    reg [2:0] state = STATE_IDLE; // 2:0=5 zustände 3:0=16 zustände

    reg [7:0] cmd_reg = 8'b00000000;
    reg [31:0] src_addr_reg = 32'h00000000;
    reg [31:0] dst_addr_reg = 32'h00000000;

    always @(posedge clk) begin
        case (state)
            STATE_IDLE: begin
                valid <= 0;
                if (cmd_in != 8'b00000000) begin
                    cmd_reg <= cmd_in;
                    if (src_addr_in != 32'h00000000) src_addr_reg <= src_addr_in;
                    if (dst_addr_in != 32'h00000000) dst_addr_reg <= dst_addr_in;
                    state <= STATE_DST;
                end
            end
            STATE_DST: begin
                if (executable) begin
                    if (cmd_reg == 8'b00000001) begin // echo
                        data_out <= data_in;
                        src_addr_out <= 32'h00000000;
                        dst_addr_out <= 32'h00000000;
                    end else if (cmd_reg == 8'b00000101) begin // sync-timer trigger
                        state <= STATE_SYNC; // Wechsel zu Sync-Zustand
                    end else begin // send/copy/to
                        data_out <= data_in;
                        src_addr_out <= src_addr_reg;
                        dst_addr_out <= dst_addr_reg;
                    end
                    valid <= 1;
                end else begin
                    data_out <= 8'h00;
                    valid <= 0;
                end
                state <= STATE_IDLE; // Zurück nach Ausführung
            end
            STATE_SYNC: begin // Parallele Verarbeitung
                if (executable) begin
                    data_out <= data_in; // Temp-Daten durchreichen
                    src_addr_out <= 32'h00000001; // SensorX
                    dst_addr_out <= 32'h00000002; // Cooler
                    valid <= 1;
                end else begin
                    data_out <= 8'h00;
                    valid <= 0;
                end
                state <= STATE_IDLE; // Zurück nach Ausführung
            end
            default: state <= STATE_IDLE; // Sicherheitsfall
        endcase
    end
endmodule
