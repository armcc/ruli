#! /usr/bin/env lua
--
-- $Id: toy.lua,v 1.2 2004/10/26 15:16:38 evertonm Exp $

function dump_srv_list(t)
  for i,v in ipairs(t) do
    print(i, ":")
    for j,w in pairs(v) do
      print(j, "=>", w)
    end
  end
end

t = {}

s = {}
s["target"] = "host1"
s["port"] = 25
t[1] = s

s = {}
s["target"] = "host2"
s["port"] = 80
t[2] = s

dump_srv_list(t)
