function onUpdate()
	local pos = transform:getPosition()
	
	pos.y = pos.y + 0.025
	
	if pos.y > 10 then
		pos.y = 0
	end
	
	-- print(tostring(pos))
	transform:setPosition(pos)
end