# Unique Doorbell in Swiss Style
A funny and extraordinary Swiss version of a normal doorbell. Utilizing an older but still functioning three-tone horn harvested from a retired post bus that probably found his way to the scrap metal press already. If you want to watch a real Swiss post bus driving along windy snowy roads and using it's horn to warn oncoming traffic then click [here](https://www.youtube.com/watch?v=wMWEQdxMhdA).

## :information_source: Description

A while ago I got an Email from a then unknown client asking the innocent question how to control a simple 4-relais board. After digging deeper he eventually revealed his plan to build a rather unusual doorbell for his medium sized home which turned out to be located in a small and remote village (less than 100 inhabitants) in the french part of the Jura Mountains.  

The Jura mainly demarcates a long stretch of the French–Swiss border, is not as high as the Swiss Alps (only 1720m max) and nevertheless absolutely ideal for memorable moto-bike tours. So my interest was rising and since he didn't have much experience in electronics I offered him the full package including design, building the circuit board, coding the MCU and subsequently deliverage, installation and of course extensive final testing at his home in person. He happily agreed and an absolut unique and in the end almost hilarious project got started.  

Since I didn't have any clue as to the correct melody of a post bus horn he eventually sent me a valuable mp3 sound sample. The file was imported into Audacity and analyzed as to the lenght of each tone and pauses. Below are the findings.

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Sound%20Analysis.jpg)

Many Emails and Signal messenger sessions later all necessary informations (e.g. physical distance between bell button, circuit board, horns, the operating voltage of the valves, etc.) were gathered and creating a simple schematic was now possible. 

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Schematic.jpg) 

Based on the above schematic and an old plastic housing I found in a dark corner of my office the circuit board was put together with various components I had at hand. I used a Sparkfun Pro Micro board with ATmega32U4 MCU since I had a few of them with no other use for them at that time. I guess any other Arduino board would have done the job as well.  

Only part to be ordered was the bi-directional voltage suppressor diode which is supposed to kill possible voltage spikes/transients coming in on the ~12m wire connecting the board with the bell button and acting like a big antenna. Likely sources for creating transients would be the air compressor at startup, high power devices in the house getting switched on/off or even lightning in the vicinity of the house. 

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Circuit%20Board%20Top.jpg)  

When the software was done, somewhat tested and the weather forecast didn't issue any warnings for the week to come we fixed a date for installation. It took place a few days later on a sunny and dry Saturday. At arrival this sight in his garage was greeting me.

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Post%20Horn%20Tubes%20with%20Valves%20and%20Air%20Tube.jpg)  

The 3-tone horn appeared to be still in good shap despite being worn and torn for probably many years on the roads. My client had already fixed the 3 horns on a piece of wood together with an air splitter, had attached 1 valve to each horn and even mounted an air tube running to an air compressor standing next to the whole ensemble. 

Naturally the 3 horns require a minimum amount of air pressure from the attached compressor in order to work properly, and after some testing a pressure of approx. 2.5 bar seemed to be sufficient for this job.

After having connected all electrical wires between board and valves our ears almost were blewn off while conducting the first test. That 3-horn device made a hell of a noise and the melody sounded somewhat strange too. Puzzled and obviously lacking musical ears a while later our better halves got asked and they immediately uncovered the truth. By mistake the valves had been connected in the wrong order. 

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Under%20Construction%202.jpg)  

After sipping coffee, swapping cables, handing out earmuffs and playing with the timings for about 20 minutes everybody was happy. 
Now the vintage bell button at the frontdoor got wired to the controller board and we were ready for the [moment of truth.](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Final%20Test%20with%20Bell%20Button%20connected.mp4)

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Bell%20Button.jpg)

Well, the smallest horn responsible for producing the highest tone sounded somewhat hoarse but al in all the door bell was working and according to everybody's opinion will, when activated at night, not let anybody sleep who happens to have the incredibly bad luck to live nearby.

After all work done getting invited for coffee plus superb self made Cantuccinis and then riding back home through Jura was definitely the icing on the cake. But it got better. The very next day the proud owner of the noisy doorbell got approached by a neighbour from fairly down the road and asked if he had seen the post busses driving past the other day. The guy reckoned he had missed every single bus that went past despite trying to get a glimpse of one. When my client told me the story we both were cracking up with laughter.

## :mag: Software details

All coding was done with VSCode/PlatformIO. The programm and platformio.ini files are available in folder [**software**](https://github.com/yellobyte/Swiss-Style-Doorbell/tree/main/software).

The code starts with some definitions for better reading. The input lines IN1...IN4 on the relaisboard are active low, that means in order to activate a relais the assigned line must go low. Only 3 relais get used and ports A8, A9 and A10 will control them.

When the bell button is pressed (actually pulled in our case) then the line on port A0 will go low and after releasing the button will go high again. However, after detecting a high->low transition the code waits another 50ms and then probes the line again just to make sure short spikes managing to get through the hardware filter are neglected.

```c
#define alarmPort    A0
#define valveNone    NULL
#define valveOne     A8
#define valveTwo     A9
#define valveThree   A10

#define valveOpen   LOW
#define valveClosed HIGH
```

The melody of the doorbell is coded as shown below. This array makes it possible to add/delete tones, change their timings or re-assign valves in no time.

```c
...
struct tone_t 
{
  const int valve;      // 0...3: valve 1...3, or 0 for none (all valves closed)
  const int delay;      // ms: for how long the valve is activated (valve is open)
};
...
...
tone_t tone_array[] = {
  { .valve = valveOne,   .delay = 400 },      // first tone
  { .valve = valveTwo,   .delay = 1050 },     // second tone
  { .valve = valveThree, .delay = 1050 },     // third tone
  { .valve = valveNone,  .delay = 130 },      // pause, all valves closed
  { .valve = valveOne,   .delay = 990 },      // forth tone
  { .valve = valveTwo,   .delay = 1020 },     // fifth ...
  { .valve = valveThree, .delay = 1040 },     // ...
  { .valve = valveNone,  .delay = 90 },       // pause, all valves closed
  { .valve = valveOne,   .delay = 970 },      //
  { .valve = valveTwo,   .delay = 920 },      //
}; 
...
Vector<tone_t> melody(tone_array, sizeof(tone_array) / 4);
...
```

The code configures Timer1 of the Atmega and uses an attached interrupt routine for letting the LEDs blink if needed. The internal watchdog gets set and activated as well.

Funktion loop() checks the bell button getting pulled and if true will open/close the valves as defined above to generate the melody. 
```c
void loop() {
  ...
  ... 
  if (!digitalRead(alarmPort) && activated) {
    // wait some ms to make sure it's not a random spike
    delay(50);
    if (digitalRead(alarmPort)) return;
    // button still active, so we start the action
    activated = false;
    for (int i = 0; i < (int)melody.size(); i++) {
      if (melody.at(i).valve) {
        // activate valve for a defined period
        digitalWrite(melody.at(i).valve, valveOpen);
        delay(melody.at(i).delay);
        digitalWrite(melody.at(i).valve, valveClosed);
      }
      else {
        // pause, all valves stay closed
        delay(melody.at(i).delay);
      }
    }
  }
  ...
  ...
}
````

Serial output has been coded as well and allows to easily debug the code or simply watch it running.

## :relaxed: Postscript: 

If you have questions or miss some information, feel free to create an issue or contact me. However, my response time might be slow at times. 

