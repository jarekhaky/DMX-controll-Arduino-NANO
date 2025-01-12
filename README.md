# DMX-controll-Arduino-NANO
Controlling 2 PAR RGBW Lights for a Podcast
System Components
Potentiometers:

Potentiometer A0: Controls the overall brightness (master brightness) of both PAR lights.
Potentiometer A1: Controls the RGB mix, i.e., the color of the light.
Potentiometer A2: Controls the balance between the RGB mix and the white LED.
Switches:

Switch 1 and 2: They are of type ON-OFF-ON and are used to select the controlled light (PAR 1, PAR 2, or both lights simultaneously).
Starting DMX Channels
PAR 1:

Ch1: Master Brightness
Ch2: Red
Ch3: Green
Ch4: Blue
Ch5: White
PAR 2:

Ch10: Master Brightness
Ch11: Red
Ch12: Green
Ch13: Blue
Ch14: White
System Control
Selecting the Controlled Light:

Both lights simultaneously: Switch 1 and Switch 2 are in the OFF position.
PAR 1: Switch 1 is in the ON position, Switch 2 is in the OFF position.
PAR 2: Switch 1 is in the OFF position, Switch 2 is in the ON position.
Setting Brightness (Potentiometer A0):

Turning the potentiometer A0 sets the overall brightness of the selected light or both lights simultaneously.
Turn the potentiometer to the right to increase brightness.
Turn the potentiometer to the left to decrease brightness.
Setting Color (Potentiometer A1):

Turning the potentiometer A1 sets the color of the light using the RGB mix.
The color cycles through the entire spectrum (red, green, blue, and their combinations).
Setting Balance (Potentiometer A2):

Turning the potentiometer A2 sets the balance between the RGB mix and the white LED.
For values A2 ≤ 512: The white LED is at full intensity, and the RGB mix increases from 0 to 255.
For values A2 > 512: The RGB mix is at full intensity, and the white LED decreases from 255 to 0.
Example Usage
Setting Brightness for Both Lights:

Ensure that both switches are in the OFF position.
Turn the potentiometer A0 to the desired brightness level.
Setting Different Colors for Each Light:

Set Switch 1 to the ON position and Switch 2 to the OFF position.
Turn the potentiometer A1 to the desired color for PAR 1.
Set Switch 1 to the OFF position and Switch 2 to the ON position.
Turn the potentiometer A1 to the desired color for PAR 2.
Setting Balance for Both Lights:

Ensure that both switches are in the OFF position.
Turn the potentiometer A2 to the desired balance level between the RGB mix and the white LED.
Tips and Tricks
Before starting your podcast recording, try different combinations of brightness and colors to achieve the best lighting.
If you need consistent lighting, set both switches to the OFF position and control both lights simultaneously.
For dramatic effects or to highlight a specific part of the scene, you can set different colors and balances for each light individually.
