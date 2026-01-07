// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <stdlib.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <helper/log.h>
#include <helper/time_support.h>
#include "target/target.h"
#include "target/algorithm.h"
#include "target/target_type.h"
#include <target/smp.h>
#include "jtag/jtag.h"
#include "target/register.h"
#include "target/breakpoints.h"
#include "helper/base64.h"
#include "helper/time_support.h"
#include "rtos/rtos.h"
#include <helper/bits.h>
#include "ambarella.h"
#include "../semihosting_common.h"


/* Wall-clock timeout for a command/access. Settable via RISC-V Target commands.*/
static int amba_command_timeout_sec_value = DEFAULT_COMMAND_TIMEOUT_SEC;
/* DEPRECATED Wall-clock timeout after reset. Settable via RISC-V Target commands.*/
static int amba_reset_timeout_sec = DEFAULT_COMMAND_TIMEOUT_SEC;

static unsigned int amba_xlen_nonconst(struct target *target)
{
	// return amba_xlen(target);
    return 0;
}

static
int amba_create_target(struct target *target)
{
	return ERROR_OK;
}

static unsigned int amba_data_bits(struct target *target)
{
	return 0;
}

static
int amba_jim_configure(struct target *target,
		                      struct jim_getopt_info *goi)
{
    return JIM_ERR;
}

static int amba_init_target(struct command_context *cmd_ctx,
		struct target *target)
{
	LOG_TARGET_DEBUG(target, "amba_init_target()");

	return ERROR_OK;
}

static
void amba_deinit_target(struct target *target)
{
	LOG_TARGET_DEBUG(target, "amba_deinit_target()");
}

static
int amba_examine(struct target *target)
{
    return ERROR_OK;
}

static
int old_or_new_amba_poll(struct target *target)
{
    return ERROR_OK;
}

int amba_halt(struct target *target)
{
    return ERROR_OK;
}

static
int amba_target_resume(struct target *target, bool current,
		               target_addr_t address,
                       bool handle_breakpoints,
                       bool debug_execution)
{
	if (target->state != TARGET_HALTED) {
		LOG_TARGET_ERROR(target, "Not halted.");
		return ERROR_TARGET_NOT_HALTED;
	}
	return ERROR_OK;
}

static
int old_or_new_amba_step(struct target *target,
                         bool           current,
		                 target_addr_t  address,
                         bool           handle_breakpoints)
{
	return 0;
}

static int amba_assert_reset(struct target *target)
{
	LOG_TARGET_DEBUG(target, "");
    return 0;
}

static int amba_deassert_reset(struct target *target)
{
	LOG_TARGET_DEBUG(target, "");

    return ERROR_FAIL;
}

static
int amba_read_memory(struct target *target,
                     target_addr_t address,
		             uint32_t size,
                     uint32_t count,
                     uint8_t *buffer)
{
	return ERROR_OK;
}

static
int amba_write_memory(struct target *target,
                      target_addr_t address,
		              uint32_t size,
                      uint32_t count,
                      const uint8_t *buffer)
{
	return ERROR_OK;
}

static
int amba_read_phys_memory(struct target *target,
                          target_addr_t phys_address,
			              uint32_t size,
                          uint32_t count,
                          uint8_t *buffer)
{
	return 0;
}

static
int amba_write_phys_memory(struct target *target,
                           target_addr_t phys_address,
			               uint32_t size,
                           uint32_t count,
                           const uint8_t *buffer)
{

	return 0;
}

static
int amba_checksum_memory(struct target *target,
		                 target_addr_t address,
                         uint32_t count,
		                 uint32_t *checksum)
{
    return ERROR_OK;
}

static
int amba_mmu(struct target *target, bool *enabled)
{
    return ERROR_OK;
}

static
int amba_virt2phys(struct target *target, target_addr_t virtual, target_addr_t *physical)
{
    return ERROR_OK;
}

static
const char *amba_get_gdb_arch(const struct target *target)
{
	return NULL;
}

static
int amba_get_gdb_reg_list(struct target *target,
		                  struct reg **reg_list[],
                          int *reg_list_size,
		                  enum target_register_class reg_class)
{
	return 0;
}

static
int amba_get_gdb_reg_list_noread(struct target *target,
		                         struct reg **reg_list[],
                                 int *reg_list_size,
		                         enum target_register_class reg_class)
{
	return 0;
}

static int amba_add_breakpoint(struct target *target, struct breakpoint *breakpoint)
{
    return ERROR_OK;
}

static
int amba_remove_breakpoint(struct target *target,
		                   struct breakpoint *breakpoint)
{
    return ERROR_OK;
}

int amba_add_watchpoint(struct target *target, struct watchpoint *watchpoint)
{
    return ERROR_OK;
}

int amba_remove_watchpoint(struct target *target,
		struct watchpoint *watchpoint)
{
    return ERROR_OK;
}

static
int amba_hit_watchpoint(struct target *target, struct watchpoint **hit_watchpoint)
{
    return ERROR_OK;
}

static int amba_arch_state(struct target *target)
{
    return ERROR_OK;
}

static
int amba_run_algorithm(struct target *target,
                       int num_mem_params,
		               struct mem_param *mem_params,
                       int num_reg_params,
		               struct reg_param *reg_params,
                       target_addr_t entry_point,
		               target_addr_t exit_point,
                       unsigned int timeout_ms,
                       void *arch_info)
{
    return ERROR_OK;
}


/* Command Handlers */
COMMAND_HANDLER(amba_set_command_timeout_sec)
{
	if (CMD_ARGC != 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	int timeout = atoi(CMD_ARGV[0]);
	if (timeout <= 0) {
		LOG_ERROR("%s is not a valid integer argument for command.", CMD_ARGV[0]);
		return ERROR_FAIL;
	}

	amba_command_timeout_sec_value = timeout;

	return ERROR_OK;
}

COMMAND_HANDLER(amba_set_reset_timeout_sec)
{
	LOG_WARNING("The command 'riscv set_reset_timeout_sec' is deprecated! Please, use 'riscv set_command_timeout_sec'.");
	if (CMD_ARGC != 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	int timeout = atoi(CMD_ARGV[0]);
	if (timeout <= 0) {
		LOG_ERROR("%s is not a valid integer argument for command.", CMD_ARGV[0]);
		return ERROR_FAIL;
	}

	amba_reset_timeout_sec = timeout;
	return ERROR_OK;
}

COMMAND_HANDLER(amba_set_mem_access)
{
	return ERROR_OK;
}

#if 0
static int parse_reg_ranges_impl(struct list_head *ranges, char *args,
		const char *reg_type, unsigned int max_val, char ** const name_buffer)
{
	/* For backward compatibility, allow multiple parameters within one TCL
	 * argument, separated by ',' */
	for (char *arg = strtok(args, ","); arg; arg = strtok(NULL, ",")) {
		unsigned int low = 0;
		unsigned int high = 0;
		char *name = NULL;

		char *dash = strchr(arg, '-');
		char *equals = strchr(arg, '=');

		if (!dash && !equals) {
			/* Expecting single register number. */
			if (!parse_csr_address(arg, &low)) {
				LOG_ERROR("Failed to parse single register number from '%s'.", arg);
				return ERROR_COMMAND_SYNTAX_ERROR;
			}
		} else if (dash && !equals) {
			/* Expecting register range - two numbers separated by a dash: ##-## */
			*dash = '\0';
			if (!parse_csr_address(arg, &low)) {
				LOG_ERROR("Failed to parse '%s' - not a valid decimal or hexadecimal number.",
					arg);
				return ERROR_COMMAND_SYNTAX_ERROR;
			}
			const char *high_num_in = dash + 1;
			if (!parse_csr_address(high_num_in, &high)) {
				LOG_ERROR("Failed to parse '%s' - not a valid decimal or hexadecimal number.",
					high_num_in);
				return ERROR_COMMAND_SYNTAX_ERROR;
			}
			if (high < low) {
				LOG_ERROR("Incorrect range encountered [%u, %u].", low, high);
				return ERROR_FAIL;
			}
		} else if (!dash && equals) {
			/* Expecting single register number with textual name specified: ##=name */
			*equals = '\0';
			if (!parse_csr_address(arg, &low)) {
				LOG_ERROR("Failed to parse '%s' - not a valid decimal or hexadecimal number.",
					arg);
				return ERROR_COMMAND_SYNTAX_ERROR;
			}

			const char * const reg_name_in = equals + 1;
			const size_t reg_type_len = strlen(reg_type);
			/* format is: <reg_type>_<reg_name_in>\0 */
			*name_buffer = calloc(1, strlen(reg_name_in) + reg_type_len + 2);
			name = *name_buffer;
			if (!name) {
				LOG_ERROR("Out of memory");
				return ERROR_FAIL;
			}
			strcpy(name, reg_type);
			name[reg_type_len] = '_';

			unsigned int scanned_chars;
			char *scan_dst = name + strlen(reg_type) + 1;
			if (sscanf(reg_name_in, "%[_a-zA-Z0-9]%n", scan_dst, &scanned_chars) != 1 ||
				scanned_chars != strlen(reg_name_in)) {
				LOG_ERROR("Invalid characters in register name '%s'.", reg_name_in);
				return ERROR_COMMAND_SYNTAX_ERROR;
			}
		} else {
			LOG_ERROR("Invalid argument '%s'.", arg);
			return ERROR_COMMAND_SYNTAX_ERROR;
		}

		high = MAX(high, low);

		if (high > max_val) {
			LOG_ERROR("Cannot expose %s register number 0x%x, maximum allowed value is 0x%x.",
				reg_type, high, max_val);
			return ERROR_FAIL;
		}

		/* Check for overlap, name uniqueness. */
		range_list_t *entry;
		list_for_each_entry(entry, ranges, list) {
			if (entry->low <= high && low <= entry->high) {
				if (low == high)
					LOG_WARNING("Duplicate %s register number - "
							"Register %u has already been exposed previously", reg_type, low);
				else
					LOG_WARNING("Overlapping register ranges - Register range starting from %u overlaps "
							"with already exposed register/range at %u.", low, entry->low);
			}

			if (entry->name && name && (strcasecmp(entry->name, name) == 0)) {
				LOG_ERROR("Duplicate register name \"%s\" found.", name);
				return ERROR_FAIL;
			}
		}

		range_list_t *range = calloc(1, sizeof(range_list_t));
		if (!range) {
			LOG_ERROR("Out of memory");
			return ERROR_FAIL;
		}

		range->low = low;
		range->high = high;
		range->name = name;
		/* ownership over name_buffer contents is transferred to list item here */
		*name_buffer = NULL;
		list_add(&range->list, ranges);
	}

	return ERROR_OK;
}
#endif

/*

static int parse_reg_ranges(struct list_head *ranges, const char *tcl_arg,
		const char *reg_type, unsigned int max_val)
{
	char *args = strdup(tcl_arg);
	if (!args) {
		LOG_ERROR("Out of memory");
		return ERROR_FAIL;
	}
	char *name_buffer = NULL;
	int result = parse_reg_ranges_impl(ranges, args, reg_type, max_val, &name_buffer);
	free(name_buffer);
	free(args);
	return result;
}
*/

COMMAND_HANDLER(amba_set_expose_csrs)
{

	int ret = ERROR_OK;

	return ret;
}

COMMAND_HANDLER(amba_set_expose_custom)
{
	if (CMD_ARGC == 0)
		return ERROR_COMMAND_SYNTAX_ERROR;


	return 0;
}

COMMAND_HANDLER(amba_hide_csrs)
{
	if (CMD_ARGC == 0)
		return ERROR_COMMAND_SYNTAX_ERROR;


	return 0;
}

COMMAND_HANDLER(amba_authdata_read)
{
	return ERROR_FAIL;
}

COMMAND_HANDLER(amba_authdata_write)
{
	return 0;
}

uint32_t amba_get_dmi_address(const struct target *target, uint32_t dm_address)
{
	return 0;
}

/*

static int amba_dmi_read(struct target *target, uint32_t *value, uint32_t address)
{
	return 0;
}
*/

static int amba_dmi_write(struct target *target, uint32_t dmi_address, uint32_t value)
{
	if (!target) {
		LOG_ERROR("target is NULL!");
		return ERROR_FAIL;
	}

	return 0;
}

COMMAND_HANDLER(handle_amba_dmi_read)
{
	if (CMD_ARGC != 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return 0;
}

COMMAND_HANDLER(handle_amba_dmi_write)
{
	if (CMD_ARGC != 2)
		return ERROR_COMMAND_SYNTAX_ERROR;

	uint32_t dmi_address, value;
	COMMAND_PARSE_NUMBER(u32, CMD_ARGV[0], dmi_address);
	COMMAND_PARSE_NUMBER(u32, CMD_ARGV[1], value);

	struct target * const target = get_current_target(CMD_CTX);
	return amba_dmi_write(target, dmi_address, value);
}

COMMAND_HANDLER(handle_amba_dm_read)
{
	if (CMD_ARGC != 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return 0;
}

COMMAND_HANDLER(handle_amba_dm_write)
{
	if (CMD_ARGC != 2)
		return ERROR_COMMAND_SYNTAX_ERROR;

	uint32_t dm_address, value;
	COMMAND_PARSE_NUMBER(u32, CMD_ARGV[0], dm_address);
	COMMAND_PARSE_NUMBER(u32, CMD_ARGV[1], value);

	struct target * const target = get_current_target(CMD_CTX);
	return amba_dmi_write(target, amba_get_dmi_address(target, dm_address),
					value);
}

COMMAND_HANDLER(amba_reset_delays)
{
	if (CMD_ARGC > 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return ERROR_OK;
}

COMMAND_HANDLER(amba_set_ir)
{
	if (CMD_ARGC != 2)
		return ERROR_COMMAND_SYNTAX_ERROR;

	uint32_t value;
	COMMAND_PARSE_NUMBER(u32, CMD_ARGV[1], value);

	return ERROR_OK;
}

COMMAND_HANDLER(amba_resume_order)
{
	if (CMD_ARGC != 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return ERROR_OK;
}

COMMAND_HANDLER(amba_use_bscan_tunnel)
{
	return ERROR_OK;
}

COMMAND_HANDLER(amba_set_bscan_tunnel_ir)
{
	if (CMD_ARGC > 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return ERROR_OK;
}

COMMAND_HANDLER(amba_set_maskisr)
{
	return ERROR_OK;
}

COMMAND_HANDLER(amba_set_autofence)
{
	return ERROR_COMMAND_SYNTAX_ERROR;
}

COMMAND_HANDLER(amba_set_ebreakm)
{
	return 0;
}

COMMAND_HANDLER(amba_set_ebreaks)
{
	return 0;
}

COMMAND_HANDLER(amba_set_ebreaku)
{
	return 0;
}

COMMAND_HELPER(amba_clear_trigger, int trigger_id, const char *name)
{
	return 0;
}

COMMAND_HANDLER(amba_itrigger)
{
	if (CMD_ARGC < 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return ERROR_OK;
}

COMMAND_HANDLER(amba_icount)
{
	if (CMD_ARGC < 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return ERROR_OK;
}

COMMAND_HANDLER(amba_etrigger)
{
	if (CMD_ARGC < 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return ERROR_OK;
}

COMMAND_HANDLER(handle_repeat_read)
{
	return 0;
}

COMMAND_HANDLER(handle_memory_sample_command)
{
	if (CMD_ARGC < 2)
		return ERROR_COMMAND_SYNTAX_ERROR;

	return ERROR_OK;
}

COMMAND_HANDLER(handle_dump_sample_buf_command)
{
	return 0;
}

static COMMAND_HELPER(amba_print_info_line_if_available, const char *section,
		const char *key, unsigned int value, bool is_available)
{
	char full_key[80];
	snprintf(full_key, sizeof(full_key), "%s.%s", section, key);
	if (is_available)
		command_print(CMD, "%-21s %3d", full_key, value);
	else
		command_print(CMD, "%-21s unavailable", full_key);
	return 0;
}

COMMAND_HELPER(amba_print_info_line, const char *section, const char *key,
			   unsigned int value)
{
	return CALL_COMMAND_HANDLER(amba_print_info_line_if_available, section,
			key, value, /*is_available*/ true);
}

COMMAND_HANDLER(handle_info)
{
	return 0;
}

COMMAND_HANDLER(amba_exec_progbuf)
{
	return ERROR_OK;
}

COMMAND_HANDLER(amba_set_enable_trigger_feature)
{
	return ERROR_OK;
}

/*

static COMMAND_HELPER(report_reserved_triggers, struct target *target)
{
	return ERROR_OK;
}
*/

COMMAND_HANDLER(handle_reserve_trigger)
{
	return ERROR_OK;
}

COMMAND_HANDLER(handle_amba_virt2phys_mode)
{
	return ERROR_OK;
}

static const struct command_registration amba_exec_command_handlers[] = {
	{
		.name = "dump_sample_buf",
		.handler = handle_dump_sample_buf_command,
		.mode = COMMAND_ANY,
		.usage = "[base64]",
		.help = "Print the contents of the sample buffer, and clear the buffer."
	},
	{
		.name = "info",
		.handler = handle_info,
		.mode = COMMAND_ANY,
		.usage = "",
		.help = "Displays some information OpenOCD detected about the target."
	},
	{
		.name = "memory_sample",
		.handler = handle_memory_sample_command,
		.mode = COMMAND_ANY,
		.usage = "bucket address|clear [size=4]",
		.help = "Causes OpenOCD to frequently read size bytes at the given address."
	},
	{
		.name = "repeat_read",
		.handler = handle_repeat_read,
		.mode = COMMAND_ANY,
		.usage = "count address [size=4]",
		.help = "Repeatedly read the value at address."
	},
	{
		.name = "set_command_timeout_sec",
		.handler = amba_set_command_timeout_sec,
		.mode = COMMAND_ANY,
		.usage = "sec",
		.help = "Set the wall-clock timeout (in seconds) for individual commands"
	},
	{
		.name = "set_reset_timeout_sec",
		.handler = amba_set_reset_timeout_sec,
		.mode = COMMAND_ANY,
		.usage = "sec",
		.help = "DEPRECATED. Use 'riscv set_command_timeout_sec' instead."
	},
	{
		.name = "set_mem_access",
		.handler = amba_set_mem_access,
		.mode = COMMAND_ANY,
		.usage = "method1 [method2] [method3]",
		.help = "Set which memory access methods shall be used and in which order "
			"of priority. Method can be one of: 'progbuf', 'sysbus' or 'abstract'."
	},
	{
		.name = "expose_csrs",
		.handler = amba_set_expose_csrs,
		.mode = COMMAND_CONFIG,
		.usage = "n0[-m0|=name0][,n1[-m1|=name1]]...[,n15[-m15|=name15]]",
		.help = "Configure a list of inclusive ranges for CSRs to expose in "
				"addition to the standard ones. This must be executed before "
				"`init`."
	},
	{
		.name = "expose_custom",
		.handler = amba_set_expose_custom,
		.mode = COMMAND_CONFIG,
		.usage = "n0[-m0|=name0][,n1[-m1|=name1]]...[,n15[-m15|=name15]]",
		.help = "Configure a list of inclusive ranges for custom registers to "
			"expose. custom0 is accessed as abstract register number 0xc000, "
			"etc. This must be executed before `init`."
	},
	{
		.name = "hide_csrs",
		.handler = amba_hide_csrs,
		.mode = COMMAND_CONFIG,
		.usage = "{n0|n-m0}[,n1|n-m1]......",
		.help = "Configure a list of inclusive ranges for CSRs to hide from gdb. "
			"Hidden registers are still available, but are not listed in "
			"gdb target description and `reg` command output. "
			"This must be executed before `init`."
	},
	{
		.name = "authdata_read",
		.handler = amba_authdata_read,
		.usage = "[index]",
		.mode = COMMAND_ANY,
		.help = "Return the 32-bit value read from authdata or authdata0 "
				"(index=0), or authdata1 (index=1)."
	},
	{
		.name = "authdata_write",
		.handler = amba_authdata_write,
		.mode = COMMAND_ANY,
		.usage = "[index] value",
		.help = "Write the 32-bit value to authdata or authdata0 (index=0), "
				"or authdata1 (index=1)."
	},
	{
		.name = "dmi_read",
		.handler = handle_amba_dmi_read,
		.mode = COMMAND_ANY,
		.usage = "address",
		.help = "Read and return 32-bit value from the given address on the "
				"RISC-V DMI bus."
	},
	{
		.name = "dmi_write",
		.handler = handle_amba_dmi_write,
		.mode = COMMAND_ANY,
		.usage = "address value",
		.help = "Write a 32-bit value to the given address on the RISC-V DMI bus."
	},
	{
		.name = "dm_read",
		.handler = handle_amba_dm_read,
		.mode = COMMAND_ANY,
		.usage = "reg_address",
		.help = "Read and return 32-bit value from a debug module's register "
				"at reg_address."
	},
	{
		.name = "dm_write",
		.handler = handle_amba_dm_write,
		.mode = COMMAND_ANY,
		.usage = "reg_address value",
		.help = "Write a 32-bit value to the debug module's register at "
				"reg_address."
	},
	{
		.name = "reset_delays",
		.handler = amba_reset_delays,
		.mode = COMMAND_ANY,
		.usage = "[wait]",
		.help = "OpenOCD learns how many Run-Test/Idle cycles are required "
			"between scans to avoid encountering the target being busy. This "
			"command resets those learned values after `wait` scans. It's only "
			"useful for testing OpenOCD itself."
	},
	{
		.name = "resume_order",
		.handler = amba_resume_order,
		.mode = COMMAND_ANY,
		.usage = "normal|reversed",
		.help = "Choose the order that harts are resumed in when `hasel` is not "
			"supported. Normal order is from lowest hart index to highest. "
			"Reversed order is from highest hart index to lowest."
	},
	{
		.name = "set_ir",
		.handler = amba_set_ir,
		.mode = COMMAND_ANY,
		.usage = "idcode|dtmcs|dmi value",
		.help = "Set IR value for specified JTAG register."
	},
	{
		.name = "use_bscan_tunnel",
		.handler = amba_use_bscan_tunnel,
		.mode = COMMAND_CONFIG,
		.usage = "value [type]",
		.help = "Enable or disable use of a BSCAN tunnel to reach DM."
	},
	{
		.name = "set_bscan_tunnel_ir",
		.handler = amba_set_bscan_tunnel_ir,
		.mode = COMMAND_CONFIG,
		.usage = "[value]",
		.help = "Specify the JTAG TAP IR used to access the bscan tunnel. "
			"By default it is 0x23 << (ir_length - 6), which map some "
			"Xilinx FPGA (IR USER4)"
	},
	{
		.name = "set_maskisr",
		.handler = amba_set_maskisr,
		.mode = COMMAND_EXEC,
		.help = "mask riscv interrupts",
		.usage = "['off'|'steponly']",
	},
	{
		.name = "set_ebreakm",
		.handler = amba_set_ebreakm,
		.mode = COMMAND_ANY,
		.usage = "[on|off]",
		.help = "DEPRECATED! use '<target_name> configure -ebreak' or "
			"'<target_name> cget -ebreak'"
	},
	{
		.name = "set_ebreaks",
		.handler = amba_set_ebreaks,
		.mode = COMMAND_ANY,
		.usage = "[on|off]",
		.help = "DEPRECATED! use '<target_name> configure -ebreak' or "
			"'<target_name> cget -ebreak'"
	},
	{
		.name = "set_ebreaku",
		.handler = amba_set_ebreaku,
		.mode = COMMAND_ANY,
		.usage = "[on|off]",
		.help = "DEPRECATED! use '<target_name> configure -ebreak' or "
			"'<target_name> cget -ebreak'"
	},
	{
		.name = "etrigger",
		.handler = amba_etrigger,
		.mode = COMMAND_EXEC,
		.usage = "set [vs] [vu] [m] [s] [u] <exception_codes>|clear",
		.help = "Set or clear a single exception trigger."
	},
	{
		.name = "icount",
		.handler = amba_icount,
		.mode = COMMAND_EXEC,
		.usage = "set [vs] [vu] [m] [s] [u] [pending] <count>|clear",
		.help = "Set or clear a single instruction count trigger."
	},
	{
		.name = "itrigger",
		.handler = amba_itrigger,
		.mode = COMMAND_EXEC,
		.usage = "set [vs] [vu] [nmi] [m] [s] [u] <mie_bits>|clear",
		.help = "Set or clear a single interrupt trigger."
	},
	{
		.name = "exec_progbuf",
		.handler = amba_exec_progbuf,
		.mode = COMMAND_EXEC,
		.usage = "instr1 [instr2 [... instr16]]",
		.help = "Execute a sequence of 32-bit instructions using the program buffer. "
			"The final ebreak instruction is added automatically, if needed."
	},
	{
		.name = "set_enable_trigger_feature",
		.handler = amba_set_enable_trigger_feature,
		.mode = COMMAND_ANY,
		.usage = "[('eq'|'napot'|'ge_lt'|'all') ('wp'|'none')]",
		.help = "Control whether OpenOCD is allowed to use certain RISC-V trigger features for watchpoints."
	},
	{
		.name = "reserve_trigger",
		.handler = handle_reserve_trigger,
		/* TODO: Move this to COMMAND_ANY */
		.mode = COMMAND_EXEC,
		.usage = "[index ('on'|'off')]",
		.help = "Controls which RISC-V triggers shall not be touched by OpenOCD.",
	},
	{
		.name = "virt2phys_mode",
		.handler = handle_amba_virt2phys_mode,
		.mode = COMMAND_ANY,
		.usage = "['sw'|'hw'|'off']",
		.help = "Configure the virtual address translation mode: "
				"sw - translate vaddr to paddr by manually traversing page tables, "
				"hw - translate vaddr to paddr by hardware, "
				"off - no address translation."
	},
	{
		.name = "autofence",
		.handler = amba_set_autofence,
		.mode = COMMAND_ANY,
		.usage = "[on|off]",
		.help = "When on (default), OpenOCD will automatically execute fence instructions in some situations. "
			"When off, users need to take care of memory coherency themselves, for example by using "
			"`riscv exec_progbuf` to execute fence or CMO instructions."
	},
	{
		.chain = smp_command_handlers
	},
	COMMAND_REGISTRATION_DONE
};

/*
 * To be noted that RISC-V targets use the same semihosting commands as
 * ARM targets.
 *
 * The main reason is compatibility with existing tools. For example the
 * Eclipse OpenOCD/SEGGER J-Link/QEMU plug-ins have several widgets to
 * configure semihosting, which generate commands like `arm semihosting
 * enable`.
 * A secondary reason is the fact that the protocol used is exactly the
 * one specified by ARM. If RISC-V will ever define its own semihosting
 * protocol, then a command like `riscv semihosting enable` will make
 * sense, but for now all semihosting commands are prefixed with `arm`.
 */

static const struct command_registration amba_command_handlers[] = {
	{
		.name = "riscv",
		.mode = COMMAND_ANY,
		.help = "RISC-V Command Group",
		.usage = "",
		.chain = amba_exec_command_handlers
	},
	{
		.name = "arm",
		.mode = COMMAND_ANY,
		.help = "ARM Command Group",
		.usage = "",
		.chain = semihosting_common_handlers
	},
	COMMAND_REGISTRATION_DONE
};

struct target_type ambarella_target = {
	.name = "ambarella",

	.target_create = amba_create_target,
	.target_jim_configure = amba_jim_configure,
	.init_target = amba_init_target,
	.deinit_target = amba_deinit_target,
	.examine = amba_examine,

	/* poll current target status */
	.poll = old_or_new_amba_poll,

	.halt = amba_halt,
	.resume = amba_target_resume,
	.step = old_or_new_amba_step,

	.assert_reset = amba_assert_reset,
	.deassert_reset = amba_deassert_reset,

	.read_memory = amba_read_memory,
	.write_memory = amba_write_memory,
	.read_phys_memory = amba_read_phys_memory,
	.write_phys_memory = amba_write_phys_memory,

	.checksum_memory = amba_checksum_memory,

	.mmu = amba_mmu,
	.virt2phys = amba_virt2phys,

	.get_gdb_arch = amba_get_gdb_arch,
	.get_gdb_reg_list = amba_get_gdb_reg_list,
	.get_gdb_reg_list_noread = amba_get_gdb_reg_list_noread,

	.add_breakpoint = amba_add_breakpoint,
	.remove_breakpoint = amba_remove_breakpoint,

	.add_watchpoint = amba_add_watchpoint,
	.remove_watchpoint = amba_remove_watchpoint,
	.hit_watchpoint = amba_hit_watchpoint,

	.arch_state = amba_arch_state,

	.run_algorithm = amba_run_algorithm,

	.commands = amba_command_handlers,

	.address_bits = amba_xlen_nonconst,
	.data_bits = amba_data_bits
};
