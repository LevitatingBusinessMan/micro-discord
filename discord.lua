--package.cpath = package.cpath .. "?.so"
local ffi = require("ffi")
local RPC = ffi.load("./rpc.so") 

local headers_file = io.open("./rpc.h", "r")
local headers = headers_file:read("*a")

ffi.cdef(headers)

RPC.initialize("813116720646455346")
RPC.update_presence("Testfile", "test")

--To stall termination
io.read()
