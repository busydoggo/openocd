/**
 * @brief OpenOCD 中针对 Xilinx FPGA 内部 JTAG (BSCAN) 支持 OpenRISC 处理器调试的实现
 *
 * Xilinx BSCAN (Boundary Scan)
 *
 * 与之前看到的 Altera vJTAG 类似，Xilinx 也提供了类似的内部 JTAG 访问功能，但实现更加简单。
 * Xilinx FPGA 的边界扫描功能，允许通过 FPGA 的 JTAG 端口访问内部用户逻辑. 在 Vivado/IP
 * Catalog 中叫 BSCANE2(7-series)
 * FPGA 内部有一个特殊的 USER 指令，可以连接到用户自定义逻辑
 */


// SPDX-License-Identifier: GPL-2.0-or-later

/***************************************************************************
 *   Copyright (C) 2013 by Sergio Chico                                    *
 *   sergio.chico@gmail.com                                                *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "or1k_tap.h"
#include "or1k.h"

#include <jtag/jtag.h>

#define OR1K_XILINX_TAP_INST_USER1	0x02

static int or1k_tap_xilinx_bscan_init(struct or1k_jtag *jtag_info)
{
	LOG_DEBUG("Initialising Xilinx Internal JTAG TAP");

	/* 确保 TAP 处于复位状态 - 可能不是必须的 */
	jtag_add_tlr();  // 添加 Test-Logic-Reset 操作

	struct jtag_tap *tap = jtag_info->tap;  // 获取物理 JTAG TAP
	struct scan_field field;
	uint8_t ir_value = OR1K_XILINX_TAP_INST_USER1;  // USER1 指令

	// 配置扫描字段
	field.num_bits = tap->ir_length;  // IR 寄存器的位数
	field.out_value = &ir_value;      // 要发送的数据
	field.in_value = NULL;            // 不读取返回数据

	// 添加 IR 扫描操作
	jtag_add_ir_scan(tap, &field, TAP_IDLE);

	// 执行 JTAG 队列
	return jtag_execute_queue();
}

static struct or1k_tap_ip xilinx_bscan_tap = {
	.name = "xilinx_bscan",
	.init = or1k_tap_xilinx_bscan_init,
};

int or1k_tap_xilinx_bscan_register(void)
{
	list_add_tail(&xilinx_bscan_tap.list, &tap_list);
	return 0;
}
