--
-- Public domain
--
require("socket")
require("ssl")

print("Use Ctrl+S and Ctrl+Q to suspend and resume the server.")

local params = {
   mode = "server",
   protocol = "sslv3",
   key = "../certs/serverAkey.pem",
   certificate = "../certs/serverA.pem",
   cafile = "../certs/rootA.pem",
   verify = {"peer", "fail_if_no_peer_cert"},
   options = {"all", "no_sslv2"},
}


-- [[ SSL context
local ctx = assert(ssl.newcontext(params))
--]]

local server = socket.tcp()
server:setoption('reuseaddr', true)
assert( server:bind("127.0.0.1", 8888) )
server:listen()

local peer = server:accept()

-- [[ SSL wrapper
   peer = assert( ssl.wrap(peer, ctx) )
   assert( peer:dohandshake() )
--]]

while true do
   local str = peer:receive("*l")
   print(str)
end
peer:close()
