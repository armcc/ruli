-- $Id: ruli.lua,v 1.4 2004/10/25 22:41:10 evertonm Exp $
--
-- This is the load file for the RULI extension for
-- the Lua language.
--
-- RULI site is at: http://www.nongnu.org/ruli/
-- Lua site is at: http://www.lua.org/
--
-- This file is usually stored under the search
-- path of Lua interpreter. See the lua manual
-- for details.
--
-- See the README file from the tarball distribution
-- for detailed installation instructions.

local r = assert(loadlib("liblua-ruli.so", "luaopen_ruli"))
r()
