addon_environment = 
{
	print = print
}

setmetatable(addon_environment, {__index = _G})