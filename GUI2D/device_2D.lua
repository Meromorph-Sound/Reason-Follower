format_version = "2.0"




front = { 
  Bg = {{ path = "front" }},
  onoffbypass = { offset = {200,20}, {path="Fader_Bypass_3frames", frames = 3}},
  deviceName = { offset = { 500, 20}, { path = "Tape_Horizontal_1frames", frames = 1 }},
  {
    learningRate = { offset = { 1300, 70 }, { path = "Knob_01_63frames", frames = 63 }},
    threshold = { offset = { 1500, 70 }, { path = "Knob_01_63frames", frames = 63 }},
    thresholdLED = { offset = { 1700, 70 }, { path = "Bicolour_LED_3frames", frames = 3 }},
    
    inGain = { offset = {1900,40}, { path = "Knob_51_63frames", frames=63 }},
    outGain = { offset = {1900,190}, { path = "Knob_51_63frames", frames=63 }},
    
 --   rectification = {
 --     offset = { 950, 100 },
 --     { path = "Button_23_2frames", frames = 2},
  --  },
 --  rectificationDisplay =
  --  {
  --    offset = { 1110, 100 },
  --    { path = "Lamp_18_3frames", frames = 3},
  --  },
  	halfWave = { offset = {1000,50}, { path = "Button_53_2frames", frames = 2 }},
  	fullWave = { offset = {1000,130}, { path = "Button_53_2frames", frames = 2 }},
  	squaring = { offset = {1000,210}, { path = "Button_53_2frames", frames = 2 }},
  	
-- display = { offset = {2200,40}, { path = "Display_20_1frames", frames = 1}}
  }
}
back = { 
  Bg = {{ path = "back" },},
  Placeholder = {offset = { 100, 100 },{ path = "Placeholder" }},
  SignalInputLeft = {offset = {500,100},{ path = "SharedAudioJack", frames = 3}},
  SignalInputRight = {offset = {700,100},{ path = "SharedAudioJack", frames = 3}},
  EnvelopeOutputLeft = {offset = {900,100},{ path = "SharedAudioJack", frames = 3}},
  EnvelopeOutputRight = {offset = {1100,100},{ path = "SharedAudioJack", frames = 3}},
  GateOutputCV = {offset = {1500,110},{ path = "SharedCVJack", frames = 3}},
  deviceName = {offset = { 3280, 50},{ path = "Tape_Horizontal_1frames", frames = 1 }},
}
folded_front = { 
  Bg = {{ path = "Panel_Folded_Front" }}
}
folded_back = { 
  Bg = {{ path = "Panel_Folded_Back" }},
  CableOrigin = {offset = { 1885, 75 }}
}
