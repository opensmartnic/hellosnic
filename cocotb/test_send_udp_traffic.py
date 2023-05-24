import cocotb
from cocotb.triggers import *
from cocotb.clock import Clock
from cocotbext.axi import *
from cocotbext.eth import RgmiiSource, RgmiiSink, GmiiFrame
import random


async def generate_reset(dut):
    dut.clk_wiz_0.reset.setimmediatevalue(0)
    await RisingEdge(dut.clk_in1_0)
    await RisingEdge(dut.clk_in1_0)
    dut.clk_wiz_0.reset.value = 1
    dut.pcie_rstn.value = 0
    await Timer(200, "ns")
    dut.clk_wiz_0.reset.value = 0
    dut.pcie_rstn.value = 1
    await Timer(500, "ns")

    
@cocotb.test()
async def cocotb_entrypoint(dut):
    cocotb.start_soon(Clock(dut.clk_in1_0, 10.0, 'ns').start())
    cocotb.start_soon(Clock(dut.axi_pcie_0.REFCLK, 10.0, "ns").start())
    await generate_reset(dut)
    
    dma_axil_master = AxiLiteMaster(AxiLiteBus.from_prefix(dut.axi_dma_0, "S_AXI_LITE"), dut.axi_dma_0.s_axi_lite_aclk, dut.axi_dma_0.axi_resetn, reset_active_level=False)

    await dma_axil_master.write(0x00, 0x00007001.to_bytes(4, 'little'))
    await dma_axil_master.write(0x18, 0x00000000.to_bytes(4, 'little'))

    await dma_axil_master.write(0x28, 0x00000040.to_bytes(4, 'little'))

    await Timer(3, "us")

    

