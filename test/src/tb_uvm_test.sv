`timescale 1ns/1ps
interface sample_if(input logic clk, rstz);
    logic[7:0] addr,data;
    logic valid;
endinterface


module tb_uvm_test;
    `include "uvm_macros.svh"
    import uvm_pkg::*;

    class sample_seq_item extends uvm_sequence_item;
      rand logic [7:0] addr, data;
      `uvm_object_utils_begin(sample_seq_item)
        `uvm_field_int (addr, UVM_DEFAULT)
        `uvm_field_int (data, UVM_DEFAULT)
      `uvm_object_utils_end
      function new (string name = "sample_seq_item_inst");
        super.new(name);
      endfunction : new
    endclass

    class sample_sequencer extends uvm_sequencer #(sample_seq_item);
        `uvm_component_utils(sample_sequencer)
        `uvm_new_func
        task run_phase(uvm_phase phase);
            uvm_report_info("SEQR","Hi I am Sequencer");
        endtask
    endclass

    class sample_driver extends uvm_driver #(sample_seq_item);
        `uvm_component_utils(sample_driver)
        virtual sample_if vif;
        function new (string name ="sample_driver", uvm_component parent);
            super.new(name, parent);
        endfunction

        function void build_phase(uvm_phase phase);
            super.build_phase(phase);
            if(!uvm_config_db#(virtual sample_if)::get(this, "", "vif", vif))
                `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".vif"});
        endfunction: build_phase

        task run_phase(uvm_phase phase);
            sample_seq_item trans_item;
            vif.valid = 1'b0;
            uvm_report_info("DRIVER", "Hi! I am sample_driver");
            @(posedge vif.rstz);// wait negate reset
            forever begin
                seq_item_port.get_next_item(trans_item);
                // get several value from trans_item and drive
                // signals, receive signals via virtual interface
                @(posedge vif.clk);
                vif.valid <= 1'b1;
                vif.addr  <= trans_item.addr;
                vif.data  <= trans_item.data;
                @(posedge vif.clk) vif.valid <= 1'b0;
                seq_item_port.item_done();
            end
        endtask
    endclass

    class sample_monitor extends uvm_monitor;
        `uvm_component_utils(sample_monitor)
        virtual sample_if vif;
        function new(string name="sample_monitor", uvm_component parent);
            super.new(name, parent);
        endfunction
        function void build_phase(uvm_phase phase);
            super.build_phase(phase);
            if(!uvm_config_db#(virtual sample_if)::get(this, "", "vif", vif))
                `uvm_fatal("NOVIF",{"virtual interface must be set for:", get_full_name(), ".vif"});
        endfunction: build_phase
        task run_phase(uvm_phase phase);
            uvm_report_info("MONITOR","Hi! I am sample_monitor");
            fork
                //check_clock;
                check_trans;
            join
        endtask
        task check_trans;
            forever begin
                @(posedge vif.valid) uvm_report_info("MON", $sformatf("addr=%02Xh, data=%02Xh", vif.addr, vif.data));
            end
        endtask;
        task check_clock;
            forever begin
                wait(vif.clk===1'b0);
                uvm_report_info("MON", "fall clock");
                wait(vif.clk===1'b1);
                uvm_report_info("MON", "rise clock");
            end
        endtask
    endclass
    class sample_agent extends uvm_agent;
        `uvm_component_utils(sample_agent)
        `uvm_new_func
        sample_driver       driver;
        sample_sequencer    sequencer;
        sample_monitor      monitor;
        function void build_phase(uvm_phase phase);
            driver = sample_driver::type_id::create("driver",this);
            sequencer = sample_sequencer::type_id::create("sequencer",this);
            monitor = sample_monitor::type_id::create("monitor",this);
        endfunction
        function void connect_phase(uvm_phase phase);
            if(get_is_active() == UVM_ACTIVE) begin
                driver.seq_item_port.connect(sequencer.seq_item_export);
            end
        endfunction
        task run_phase(uvm_phase phase);
            uvm_report_info("AGENT", "Hi! I am Agent");
        endtask
    endclass
    class sample_env extends uvm_env;
        `uvm_component_utils(sample_env)
        `uvm_new_func
        sample_agent agent;
        function void build_phase(uvm_phase phase);
            super.build_phase(phase);
            agent = sample_agent::type_id::create("agent",this);
        endfunction
    endclass
    virtual class sample_base_sequence extends uvm_sequence #(sample_seq_item);
        function new(string name="sample_base_seq");
            super.new(name);
        endfunction
        virtual task pre_body();
            if(starting_phase != null) begin
                `uvm_info(get_type_name(),
                          $sformatf("%s pre_body() raising %s objection",
                          starting_phase.get_name()), UVM_MEDIUM);
                starting_phase.raise_objection(this);
            end
        endtask
        // Drop the objection in the post_body so the objection is removed when
        // the root sequence is complete.
        virtual task post_body();
            if(starting_phase != null) begin
                `uvm_info(get_type_name(),
                          $sformatf("%s post_bodu() dropping %s objection",
                                    get_sequence_path(),
                                    starting_phase.get_name()), UVM_MEDIUM);
            starting_phase.drop_objection(this);
            end
        endtask
    endclass
    class issue_one_trans_seq extends sample_base_sequence;
        `uvm_object_utils(issue_one_trans_seq)
        function new(string name="issue_one_trans_seq");
            super.new(name);
        endfunction
      virtual task body();
        sample_seq_item trans_item;
        $display("I am issue_one_trans_seq");
        `uvm_create(trans_item)
        trans_item.addr = 8'h00;
        trans_item.data = 8'h10;
        `uvm_send(trans_item)
        #1000;
      endtask
   endclass
   class test extends uvm_test;
        `uvm_component_utils(test)
        `uvm_new_func
        sample_env env;
        virtual function void build_phase(uvm_phase phase);
           env=sample_env::type_id::create("env",this);
            super.build_phase(phase);
        endfunction

        task run_phase(uvm_phase phase);
            issue_one_trans_seq seq = issue_one_trans_seq::type_id::create("seq");
            uvm_report_info("TEST", "Hello World from class test");
            phase.raise_objection(this);
            seq.start(env.agent.sequencer);
            phase.drop_objection(this);
        endtask
    endclass
    class sample_test extends uvm_test;
        `uvm_component_utils(sample_test)
        sample_env env;
        function new (string name="sample_test", uvm_component parent=null);
            super.new(name,parent);
        endfunction
        function void build_phase(uvm_phase phase);
            super.build_phase(phase);
            env = sample_env::type_id::create("env",this);
        endfunction
        task run_phase(uvm_phase phase);
            uvm_report_info("TEST", "Hello World from sample_test");
        endtask
    endclass

    logic clk, rstz;
    sample_if sif(clk, rstz);// interface
    initial begin
        fork
            begin
                clk = 1'b1;
                #100;
                forever #50 clk = ~clk;
            end
            begin
                rstz = 1'b0;
                #100;
                rstz = 1'b1;
            end
        join
    end
    initial begin
        `uvm_info("info", "Hello World from initial block", UVM_LOW)
        uvm_config_db#(virtual sample_if)::set(uvm_root::get(), "*", "vif", sif);
        run_test("test");
    end
endmodule
