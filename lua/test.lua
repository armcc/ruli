#! /usr/bin/env lua
--
-- $Id: test.lua,v 1.6 2004/10/26 18:18:35 evertonm Exp $

function dump_srv_list(t)
  for i,v in ipairs(t) do
    print(string.format('  %d:', i))
    for j,w in pairs(v) do
      if (type(w) == "table") then
        print(string.format('    %s =>', j))
        for k,addr in ipairs(w) do
          print(string.format('      %s', addr))
        end
      else
        print(string.format('    %s => %s', j, w))
      end
    end
  end
end

function show(label, srv_list)
  print(label)
  dump_srv_list(srv_list)
end

require("ruli")

t = ruli.ruli_sync_query("_sip._tcp", "gnu.org", -1, 0)
show("SIP: gnu.org", t)

t = ruli.ruli_sync_smtp_query("registro.br", 0)
show("SMTP: registro.br", t)

t = ruli.ruli_sync_http_query("6bone.net", -1, 0)
show("HTTP: 6bone.net", t)

t = ruli.ruli_sync_http_query("bad.tld", -1, 0)
show("HTTP: bad.tld", t)

