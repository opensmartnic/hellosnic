# This file is public domain, it can be freely copied without restrictions.
# SPDX-License-Identifier: CC0-1.0

# Makefile

# defaults
SIM ?= modelsim
TOPLEVEL_LANG ?= verilog

# VERILOG_SOURCES += /E/fpga/Vivado/2019.2/data/verilog/src/unisims/*.v
VERILOG_SOURCES += /E/fpga/Vivado/2019.2/data/verilog/src/glbl.v
PROJECT_SRC_HOME = /E/Share/01MK7160-V2020/08_myprojs/hellosnic/hellosnic.srcs

VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/synth/*.v
VHDL_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_bram_ctrl_0_0/synth/*.vhd
VHDL_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_dma_0_0/synth/*.vhd
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_pcie_0_0/synth/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_pcie_0_0/hdl/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/synth/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/synth/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_0/synth/*.v
VHDL_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_1/synth/*.vhd
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_2/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_3/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_4/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_5/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_6/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_7/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_8/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_9/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_10/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_11/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_12/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_13/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_14/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_15/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_16/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_17/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_18/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_19/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_20/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_21/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_22/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_23/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_24/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_25/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_26/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_27/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_28/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_29/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_30/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_31/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_32/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_33/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_34/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_35/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_36/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_37/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_38/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_39/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_40/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_41/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_42/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_43/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_44/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_45/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_46/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_47/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_48/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_smc_0/bd_0/ip/ip_49/synth/*.sv
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axis_data_fifo_0_0/synth/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axis_data_fifo_1_0/synth/*.v
VHDL_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_blk_mem_gen_0_0/synth/*.vhd
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_clk_wiz_0_0/system_i_clk_wiz_0_0.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_clk_wiz_0_0/system_i_clk_wiz_0_0_clk_wiz.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_data_process_0_0/synth/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/new/*.v
VHDL_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_rst_axi_pcie_0_125M_0/synth/*.vhd
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_tri_mode_ethernet_mac_0_0/synth/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_tri_mode_ethernet_mac_0_0/synth/common/*.v
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_tri_mode_ethernet_mac_0_0/synth/physical/*.v
VHDL_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_util_ds_buf_0_0/synth/*.vhd
VERILOG_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_xlconstant_0_0/synth/*.v
VHDL_SOURCES += $(PROJECT_SRC_HOME)/sources_1/bd/system_i/ip/system_i_axi_gpio_0_0/synth/*.vhd

# VERILOG_SOURCES += /E/Share/01MK7160-V2020/08_myprojs/cocotb_use_xilinx_ip/cocotb_use_xilinx_ip.srcs/sources_1/bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0.v
# VERILOG_SOURCES += /E/Share/01MK7160-V2020/08_myprojs/cocotb_use_xilinx_ip/cocotb_use_xilinx_ip.srcs/sources_1/bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0_clk_wiz.v
# VHDL_SOURCES += /E/Share/01MK7160-V2020/08_myprojs/cocotb_use_xilinx_ip/cocotb_use_xilinx_ip.srcs/sources_1/bd/design_1/ip/design_1_blk_mem_gen_0_0/synth/*.vhd
# VHDL_SOURCES += /E/Share/01MK7160-V2020/08_myprojs/cocotb_use_xilinx_ip/cocotb_use_xilinx_ip.srcs/sources_1/bd/design_1/ip/design_1_axi_bram_ctrl_0_0/synth/*.vhd
# VERILOG_SOURCES += /E/Share/01MK7160-V2020/08_myprojs/cocotb_use_xilinx_ip/cocotb_use_xilinx_ip.srcs/sources_1/bd/design_1/ip/design_1_axi_protocol_convert_0_0/synth/*.v
# VERILOG_SOURCES += /E/Share/01MK7160-V2020/08_myprojs/cocotb_use_xilinx_ip/cocotb_use_xilinx_ip.srcs/sources_1/bd/design_1/ipshared/c4a6/hdl/*.v


# use VHDL_SOURCES for VHDL files

# TOPLEVEL is the name of the toplevel module in your Verilog or VHDL file
TOPLEVEL = system_i

# MODULE is the basename of the Python test file
MODULE ?= test_recv_udp_traffic

SIM_ARGS = -L unisims_ver -L unimacro_ver -L secureip -L xpm \
		   -L axi_bram_ctrl_v4_1_2 -L axi_dma_v7_1_21 -L axi_pcie_v2_9_2 \
		   -L axis_data_fifo_v2_0_2 -L blk_mem_gen_v8_4_4 -L lib_cdc_v1_0_2 -L axis_infrastructure_v1_1_0 \
		   -L smartconnect_v1_0 -L xlconstant_v1_1_6 -L axi_protocol_converter_v2_1_20 \
		   -L tri_mode_ethernet_mac_v9_0_15 \
		   work.glbl 
# SIM_ARGS = -L blk_mem_gen_v8_4_4 -L axi_bram_ctrl_v4_1_2 -L unisims_ver -L unimacro_ver -L secureip -L xpm -L smartconnect_v1_0 -L xlconstant_v1_1_6 work.glbl 
# SIM_ARGS = work.glbl -voptargs=+acc=rn -L blk_mem_gen_v8_4_4
# GUI=1

# include cocotb's make rules to take care of the simulator setup
include $(shell cocotb-config --makefiles)/Makefile.sim
