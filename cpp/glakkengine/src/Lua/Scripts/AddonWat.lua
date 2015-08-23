function Wat_initialize()
	local val = kke.FNV1aHash.hash_rt("asdahey")
	print(val)
	local man = kke.LuaManager:Instance()
	local ref = man:RegisterReference("Wat_sex")
	local event = kke.EventData:new_local(kke.EventHasherRT("fucker"), 102.0)
	print(event:GetTime())
	local ev = kke.EventManager:Instance()
	ev:Trigger(event)
	
	print("Init")
end

function Wat_sex()
	print("sex")
end

function Wat_terminate()
	print("Term")
	Wat_initialize = nil
	Wat_terminate = nil
end