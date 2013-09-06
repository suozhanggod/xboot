/*
 * framework/stopwatch/l-stopwatch.c
 *
 * Copyright(c) 2007-2013 jianjun jiang <jerryjianjun@gmail.com>
 * official site: http://xboot.org
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

#include <framework/stopwatch/l-stopwatch.h>

struct stopwatch_t {
	double __start_time;
};

static double __time_stamp(void)
{
	return (double)jiffies / HZ;
}

static int l_new(lua_State * L)
{
	struct stopwatch_t * stopwatch = lua_newuserdata(L, sizeof(struct stopwatch_t));
	stopwatch->__start_time = __time_stamp();
	luaL_setmetatable(L, MT_NAME_STOPWATCH);
	return 1;
}

static const luaL_Reg l_stopwatch[] = {
	{"new", l_new},
	{NULL, NULL}
};

static int m_reset(lua_State * L)
{
	struct stopwatch_t * stopwatch = luaL_checkudata(L, 1, MT_NAME_STOPWATCH);
	stopwatch->__start_time = __time_stamp();
	return 0;
}

static int m_elapsed(lua_State * L)
{
	struct stopwatch_t * stopwatch = luaL_checkudata(L, 1, MT_NAME_STOPWATCH);
	lua_pushnumber(L, __time_stamp() - stopwatch->__start_time);
	return 1;
}

static const luaL_Reg m_stopwatch[] = {
	{"reset", m_reset},
	{"elapsed", m_elapsed},
	{NULL, NULL}
};

int luaopen_stopwatch(lua_State * L)
{
	luaL_newlib(L, l_stopwatch);
	luahelper_create_metatable(L, MT_NAME_STOPWATCH, m_stopwatch);
	return 1;
}