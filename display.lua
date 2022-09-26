format_version = "1.0"

local Axes = { r=180,g=180,b=180 }
local Rect = { r=80,g=80,b=80,a=128 }
local Graph = { r=0,g=255,b=0  }
local Cutoff = { r=255,g=0,b=0 }

function clip(value)
  return math.max(0,math.min(1,value))
end



function log2(x)
	return math.log(x)/math.log(2)
end

function response(rho,x)
  return rho / (1 - x*(1-rho))
end


function drawFilter(property_values,display_info,dirty_rect)
  local a = clip(property_values[1])
  local rho = log2(1+0.5*a)
  
  local b = clip(property_values[2])
  local threshold = log2(1+b)
  
  local w = display_info.width
  local h = display_info.height
  
  local fMin = 0
  local fMax = 1
  local xScale = 1/w
  

  
    
    local yMax = 1 -- valueOf(rate,1,exponent,fMin)
    local yMin = 0
    local yScale = (yMax-yMin)/h;
    jbox.trace("yMax ="..yMax)
   
    local points={}
    jbox.trace("Rho ="..rho)
    jbox.trace("Threshold ="..threshold)
    
    -- lines
    --local yAlpha = h*valueOf(rate,alpha,0,fMin)
    --jbox_display.draw_rect({left= 0, top= h-yAlpha, right= w-1, bottom= h-1},Rect)
    
    
    
    if rho > 0 then
    
    	for x = 0,w,5 do
      		local freq=x*xScale
      		local y=response(rho,freq)/yScale
      		
      		table.insert(points,{x=x,y=y})
    	end
    	table.insert(points,{x=w,y=h})
    	table.insert(points,{x=0,y=h})
    	jbox_display.draw_polygon(points,Graph)
    	
    	local cutoff =  -math.log(1-rho)/(2*math.pi)
    	local xOff = math.floor(cutoff*w)
    	jbox.trace("cut="..cutoff.." xO="..xOff)
    	jbox_display.draw_rect({left= xOff, top= 0, right= w-1, bottom= h-1},Rect)
    	
    	local tY = (1-threshold)/yScale
    	jbox_display.draw_rect({left=0, top= tY-2, right= w-1, bottom= tY+2},Cutoff)
    end
    
    
    
 
  
   -- axes
  jbox_display.draw_lines({ {x=0,y=0},{x=w-1,y=0},{x=w-1,y=h-1},{x=0,y=h-1} },Axes,"closed") 
end


