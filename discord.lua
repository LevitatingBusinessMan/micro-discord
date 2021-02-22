local fmt = import("fmt")
local micro = import("micro")

--[[ 
	local ffi = require("ffi")
	local RPC = ffi.load("./rpc.so") 

	local headers_file = io.open("./rpc.h", "r")
	local headers = headers_file:read("*a")

	ffi.cdef(headers)

	RPC.initialize("813116720646455346")
	RPC.update_presence("Testfile", "test")

	--To stall termination
	io.read()
]]--

function preinit()
	--Run if not running
	ensure_daemon()
end

function onBufferOpen(buf)

	--Run it again in case another instance closed it
	--ensure_daemon()

	local file = buf.Path
	local filetype = buf:FileType()
	if file == 'Log' or file =='' then
		return
	end

	if filetype == nil then
		filetype = "unknown"
	end

	local cmd = fmt.Sprintf("./micro_rpc '%s' '%s' >/dev/null 2>&1", file, buf:FileType())
	os.execute(cmd)
end


--I haven't found a good way to switching the focus to a different tab or pane yet
function onSave(pane)
	if pane.buf ~= nil then
		onBufferOpen(pane.buf)
	end
end

function ensure_daemon()
	os.execute("pidof micro_rpc >/dev/null || ./micro_rpc -d >/dev/null &")
end
