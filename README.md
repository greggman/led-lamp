# LED Lamp

Inspired by [this project](http://www.eerkmans.nl/pacman-lamp-with-arduino/) a lamp

[![LED lamp](http://img.youtube.com/vi/5vNteZiAiWw/0.jpg)](http://www.youtube.com/watch?v=5vNteZiAiWw "LED Lamp")

## Notes:

The code was written with a 144 LED 1 meter strip. It might work on a longer strip but it's unlikely to work on a shorter strip.

Unlike the project linked above this one is powered by a USB charger with 2 sockets each able to deliver 2.4 amps. All 144 lamps
on full bright white would require ~6 amps so the code never turns on more than about half the lights and never at full bright white.

The LED strip is inside a [foam tube](https://www.google.com/search?q=foam+tube+white) to diffuse the light

Here is the basic circuit

![](https://cdn-learn.adafruit.com/assets/assets/000/030/892/original/leds_Wiring-Diagram.png)

Except I wired +5V on the Arduino (2 pin left of GND) to the +5V on LED strip. No idea if that is good or bad. The power supply
said it can provide 2.4 amps *per socket* and all LEDs white would be 6amps so I wanted to make sure both sockets made it 
to the strip. I know nothing about electrical engineering so I hope that's okay. I can say running the lamp for hours
nothing is getting hot. Slightly warm but not hot.

Parts I used (sorry for the Japan amazon links but I live in Tokyo)

* [Arduino UNO R3 with case](https://www.amazon.co.jp/gp/product/B0756D3R7K/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)
* [Ikea HOLMÖ Lamp](https://www.ikea.com/us/en/p/holmoe-floor-lamp-with-led-bulb-white-70394126/)
* [E26 light socket to wall socket adaptor](https://www.amazon.com/Comyan-Polarized-Splitter-Standard-Install/dp/B07SQ511XT/ref=sr_1_34?)
* [A RAVPower 24w 2port USB Charger](https://www.amazon.co.jp/gp/product/B075L2LBD6/ref=ppx_yo_dt_b_asin_title_o02_s02?ie=UTF8&psc=1)
* 330Ω resistor
* 1000µF capacitor
* A USB cable I cut and soldered in to the LED strip with the capacitor (see the +5V at bottom of the diagram)
* [1 meter foam tube (optional?)](https://www.google.com/search?q=foam+tube+white) 

So there are 2 USB cables. One from a socket the RAVPower to the Arduino UNO, another from the other socket on the RAVPower supply
to the LED strip. When I'm working on it I still have the RAVPower connected to the LED strip and the other USB in my computer.

I also used some [plastic packaging twine](https://www.daisojapan.com/p-29797-paper-twine-white-26247-ft-10pks.aspx) to tie the
foam tube with the LED in it at various points in the lamp. I had to tape the ends down or they cast shadows. Note that twine
feels like it's made out of plastic grocery store bags which means it's translucent so the LEDs will show through it. Not sure that matter.

I also used a couple of [brass fasteners](https://www.amazon.com/ACCO-Brass-Paper-Fasteners-Plated/dp/B002YK4IHA/ref=sr_1_3) to pinch the ends of the foam tube closed so the LED strip doesn't slide out.

## License: [MIT](LICENSE.md)

