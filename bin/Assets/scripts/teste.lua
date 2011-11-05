-- yooo lua comment

function onUpdate()
	local pos = transform:getPosition()
	
	pos.y = pos.y + 0.1
	
	if pos.y > 20 then
		pos.y = 0
	end
	
	-- print(tostring(pos))
	transform:setPosition(pos)
end