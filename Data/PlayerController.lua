local speed = 5
local physics

function Start ()
	print ("Player initialized")
end

function Update ()
	physics = GetSceneManager ():findActor ("Player"):getPhysicsComponent ("PhysicsComponent")
	local velocity = physics:getVelocity();

	--Move
	velocity.x = GetInputSystem ():getAxis (1) * speed

	--Jump
	if GetInputSystem ():getButtonDown (0) then
		velocity.y = 10
	end

	physics:setVelocity (velocity)
end