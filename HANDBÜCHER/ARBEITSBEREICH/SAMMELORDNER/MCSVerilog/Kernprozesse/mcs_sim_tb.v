module mcs_sim_tb;
    reg clk;
    reg [7:0] cmd;
    reg [31:0] src_addr;
    reg [31:0] dst_addr;
    reg [7:0] data_in;
    reg [0:0] executable;
    wire [7:0] data_out;
    wire [31:0] src_addr_out;
    wire [31:0] dst_addr_out;
    wire valid;
    wire led;

    mcs_sim dut (.clk(clk), .cmd(cmd), .src_addr(src_addr), .dst_addr(dst_addr), .led(led));
    mcs_protein protein (.clk(clk), .cmd_in(cmd), .src_addr_in(src_addr), .dst_addr_in(dst_addr),
                         .data_in(data_in), .data_out(data_out), .src_addr_out(src_addr_out),
                         .dst_addr_out(dst_addr_out), .executable(executable), .valid(valid));

    initial begin
        clk = 0;
        cmd = 8'b00000000;
        src_addr = 32'h00000000;
        dst_addr = 32'h00000000;
        data_in = 8'h00;
        executable = 1'b1;

        // Lade echo-cmd.protein-core:
        #0 cmd = 8'b00000001; src_addr = 32'h00000000; dst_addr = 32'h00000000; data_in = 8'h48; executable = 1'b1;
        // Lade send-cmd.protein-core
        #25 cmd = 8'b00000010; src_addr = 32'h00000000; dst_addr = 32'h00000010; data_in = 8'hAA; executable = 1'b1;
        // Lade copy-cmd.protein-core
        #20 cmd = 8'b00000011; src_addr = 32'h00000002; dst_addr = 32'h00000012; data_in = 8'hCC; executable = 1'b1;
        // Lade to-cmd.protein-core
        #20 cmd = 8'b00000100; src_addr = 32'h00000004; dst_addr = 32'h00000014; data_in = 8'hDD; executable = 1'b1;
        // Lade sync-timer für sync-timer.test-micro :
        #25 cmd = 8'b00000101; src_addr = 32'h00000000; dst_addr = 32'h00000000; data_in = 8'h19; executable = 1'b1; // 0x19 = 25 (Temp)

        forever #5 clk = ~clk;
    end

    initial begin
        $dumpfile("mcs_sim.vcd");
        $dumpvars(0, mcs_sim_tb);
        #120 $finish;
    end
endmodule
