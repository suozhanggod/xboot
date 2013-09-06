local M = Class(DisplayObject)

function M:init(on, off)
	DisplayObject.init(self)

	self.on = on
	self.off = off
	self.ison = false
	self.focus = nil

	self:addChild(self.on)
	self:addChild(self.off)
	self:updateVisualState(self.ison)

	self:addEventListener(Event.MOUSE_DOWN, self.onMouseDown, self)
	self:addEventListener(Event.MOUSE_MOVE, self.onMouseMove, self)
	self:addEventListener(Event.MOUSE_UP, self.onMouseUp, self)

	self:addEventListener(Event.TOUCHES_BEGIN, self.onTouchesBegin, self)
	self:addEventListener(Event.TOUCHES_MOVE, self.onTouchesMove, self)
	self:addEventListener(Event.TOUCHES_END, self.onTouchesEnd, self)
	self:addEventListener(Event.TOUCHES_CANCEL, self.onTouchesCancel, self)
end

function M:onMouseDown(e)
	if self.focus == nil and self:hitTest(e.info.x, e.info.y) then
		self.focus = 0
		self.ison = not self.ison
		self:updateVisualState(self.ison)
		self:dispatchEvent(Event:new("Toggled", {on = self.ison}))
		e:stopPropagation()
	end
end

function M:onMouseMove(e)
	if self.focus == 0 then
		if not self:hitTest(e.info.x, e.info.y) then	
			self.focus = nil
		end
		e:stopPropagation()
	end
end

function M:onMouseUp(e)
	if self.focus == 0 then
		self.focus = nil
		e:stopPropagation()
	end
end

function M:onTouchesBegin(e)
	if self.focus == nil and self:hitTest(e.info.x, e.info.y) then
		self.focus = e.info.id
		self.ison = not self.ison
		self:updateVisualState(self.ison)
		self:dispatchEvent(Event:new("Toggled", {on = self.ison}))
		e:stopPropagation()
	end
end

function M:onTouchesMove(e)
	if self.focus == e.info.id then
		if not self:hitTest(e.info.x, e.info.y) then
			self.focus = nil
		end
		e:stopPropagation()
	end
end

function M:onTouchesEnd(e)
	if self.focus == e.info.id then
		self.focus = nil
		e:stopPropagation()
	end
end

function M:onTouchesCancel(e)
	if self.focus == e.info.id then
		self.focus = nil;
		e:stopPropagation()
	end
end

function M:updateVisualState(state)
	if state then
		self.on:visible(true)
		self.off:visible(false)
		self.width = self.off.width
		self.height = self.off.height
	else
		self.on:visible(false)
		self.off:visible(true)
		self.width = self.on.width
		self.height = self.on.height
	end
end

return M