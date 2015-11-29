/*
 * gdbstub.c
 *
 * Copyright(c) 2007-2015 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot/gdbstub.h>

struct arm64_state_t {
	uint64_t xregs[32];
	uint64_t pc;
	uint32_t pstate;
};

static inline int cpu_get_reg32(char * buf , uint32_t reg)
{
    memcpy(buf, &reg, 4);
    return 4;
}

static inline int cpu_get_reg64(char * buf, uint64_t reg)
{
    memcpy(buf, &reg, 8);
    return 8;
}

static inline int cpu_set_reg32(uint64_t * reg, char * buf)
{
	memcpy(reg, buf, 4);
	return 4;
}

static inline int cpu_set_reg64(uint64_t * reg, char * buf)
{
	memcpy(reg, buf, 8);
	return 8;
}

static int cpu_read_register(struct gdb_cpu_t * cpu, char * buf, int reg)
{
	struct arm64_state_t * env = (struct arm64_state_t *)cpu->env;

	if(reg < 31)
		return cpu_get_reg64(buf, env->xregs[reg]);
	switch(reg)
	{
	case 31:
		return cpu_get_reg64(buf, env->xregs[31]);
	case 32:
		return cpu_get_reg64(buf, env->pc);
	case 33:
		return cpu_get_reg32(buf, env->pstate);
	default:
		break;
	}
	return 0;
}

static int cpu_write_register(struct gdb_cpu_t * cpu, char * buf, int reg)
{
	struct arm64_state_t * env = (struct arm64_state_t *)cpu->env;

	if(reg < 31)
		return cpu_set_reg64(&env->xregs[reg], buf);
	switch(reg)
	{
	case 31:
		return cpu_set_reg64(&env->xregs[31], buf);
	case 32:
		return cpu_set_reg64(&env->pc, buf);
	case 33:
		return cpu_set_reg32(&env->pstate, buf);
	default:
		break;
	}
	return 0;
}

static int cpu_set_pc(struct gdb_cpu_t * cpu, virtual_addr_t addr)
{
	struct arm64_state_t * env = (struct arm64_state_t *)cpu->env;

	env->pc = (uint64_t)addr;
	return 1;
}

static int cpu_mem_access(struct gdb_cpu_t * cpu, virtual_addr_t addr, virtual_size_t size, int rw)
{
	return 1;
}

static struct arm64_state_t __arm64_env;

static struct gdb_cpu_t __arm64_gdb_cpu = {
	.nregs	= 34,
	.read_register = cpu_read_register,
	.write_register = cpu_write_register,
	.set_pc = cpu_set_pc,
	.mem_access = cpu_mem_access,
	.env = &__arm64_env,
};

struct gdb_cpu_t * arch_gdb_cpu(void)
{
	return &__arm64_gdb_cpu;
}
