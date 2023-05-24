import cocotb
from cocotb.triggers import *
from cocotb.clock import Clock
from cocotbext.axi import *
from cocotbext.eth import RgmiiSource, RgmiiSink, GmiiFrame
import random
from scapy.all import Ether, IP, UDP


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


async def asend_msg_to_mac(rgmii_source):
    cnt = 0
    packet = Ether() / IP() / UDP(sport=50008) / "hello111"
    while True:
        await Timer(random.randint(200, 2000), "ns")
        await rgmii_source.send(GmiiFrame.from_payload(packet.build())) 
        await rgmii_source.wait()
        cnt += 1


@cocotb.test()
async def cocotb_entrypoint(dut):
    cocotb.start_soon(Clock(dut.clk_in1_0, 10.0, 'ns').start())
    cocotb.start_soon(Clock(dut.axi_pcie_0.REFCLK, 10.0, "ns").start())
    cocotb.start_soon(Clock(dut.tri_mode_ethernet_mac_0.rgmii_rxc, 8.0, "ns").start())
    await generate_reset(dut)    
    
    dma_axil_master = AxiLiteMaster(AxiLiteBus.from_prefix(dut.axi_dma_0, "S_AXI_LITE"), dut.axi_dma_0.s_axi_lite_aclk, dut.axi_dma_0.axi_resetn, reset_active_level=False)
    
    gpio_axil_master = AxiLiteMaster(AxiLiteBus.from_prefix(dut.axi_gpio_0, "S_AXI"), dut.axi_gpio_0.s_axi_aclk, dut.axi_gpio_0.s_axi_aresetn, reset_active_level=False)
    await gpio_axil_master.write(0x00, 0x0000c358.to_bytes(4, 'little'))
    
    mac = dut.tri_mode_ethernet_mac_0
    rgmii_source = RgmiiSource(mac.rgmii_rxd, mac.rgmii_rx_ctl, mac.rgmii_rxc, mac.rx_reset)

    await dma_axil_master.write(0x30, 0x00007001.to_bytes(4, 'little'))
    await dma_axil_master.write(0x48, 0x00001000.to_bytes(4, 'little'))
    await dma_axil_master.write(0x58, 0x00001000.to_bytes(4, 'little'))

    cocotb.start_soon(asend_msg_to_mac(rgmii_source))

    # dut.data_process_0.ctrl_reg.value = 0x01c358
    
    pkt_cnt = 0
    await Timer(300, "ns")
    while True:
        data = await dma_axil_master.read(0x34, 4)
        if int.from_bytes(data, 'little') & 0x1000 != 0:
            pkt_cnt += 1
            data = await dma_axil_master.read(0x58, 4)
            pkt_len = int.from_bytes(data, 'little')
            dut._log.info(f'recv packet({pkt_len})')
            await dma_axil_master.write(0x34, 0x00001000.to_bytes(4, 'little'))  
            await Timer(10, "ns")    
            await dma_axil_master.write(0x58, 0x00001000.to_bytes(4, 'little'))
            await Timer(300, "ns")
            if (pkt_cnt > 10):
                break
        else:
            await Timer(50, "ns")

    await Timer(3, "us")

    

