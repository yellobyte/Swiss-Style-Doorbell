# Unique Doorbell in Swiss Style
A funny and extraordinary Swiss version of a normal doorbell. Utilizing an older but still functioning three-tone horn harvested from a retired post bus that probably found his way to the scrap metal press already. If you want to watch a real Swiss post bus driving along windy snowy roads and using it's horn to warn oncoming traffic then click [here](https://www.youtube.com/watch?v=wMWEQdxMhdA).

## Description

A while ago I got an Email from a then unknown person asking the innocent question how to control a simple 4-relais board. After digging deeper he revealed his plan to build a rather unusual door-bell for his medium sized home which turned out to be located in a small and remote village (less than 100 inhabitants) in the french part of the Jura Mountains.  

The Jura mainly demarcates a long stretch of the French–Swiss border, is not as high as the Swiss Alps (only 1720m max) and nevertheless absolutely ideal for memorable moto-bike tours. So my interest was rising and since he didn't have much experience in electronics I offered him the full package including design, building the circuit board, coding the MCU and subsequently deliverage, installation and of course final extensive testing at his home in person. He happily agreed and an absolut unique and in the end almost hilarious project was born.  

Since I didn't have no clue as to the correct melody of a post bus horn he quickly sent me a valuable mp3 sound sample. The file was imported into Audacity and analyzed as to the lenght of each tone and pauses. Below are the findings.

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Sound%20Analysis.jpg)

After many Emails and Signal messenger sessions later all necessary informations (e.g. distance between bell button and circuit board, btw. CB and horns, operating voltage of valves, etc) were gathered and the design of a simple schematic was possible. I used a Sparkfun Pro Micro board with ATmega32U4 MCU since I had no other use for them at that time. I guess any other Arduino board or similar would have done the job though.

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/........jpg) 

Based on the above schematic and an old plastic housing I found in a dark corner of my office the circuit board was put together with various components I had at hand. Only part I needed to order was the voltage suppressor diode which kills possible voltage spikes delivered by ~12m wire connecting the board with the bell button.

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Circuit%20Board%20Top.jpg)  

When coding was done as well and the weather forecast didn't issue any warnings for the week to come we fixed a date for installation. It took place a few days later on a biker proof sunny and dry Saturday. At arrival this sight in his garage was greeting me.

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Post%20Horn%20Tubes%20with%20Valves%20and%20Air%20Tube.jpg)  

The 3-tone horn to be controlled appeared to be still in good shap despite being worn and torn for probably many years on the roads. The proud owner had already fixed the 3 horns on a piece of wood, as well as an air splitter, 3 valves, one for each of the three horns  and even attached an air tube running to his compressor nearby. 

After having connected all electrical wires between board and valves our ears almost were blewn off while conducting a first test. That 3-horn device made a hell of a noise and the melody was somewhat strange too. Since we both lack a musical ear our better halves got asked and they finally uncovered the truth. By mistake we had the valves connected in wrong order. After having a coffee, fixing that detail, handing out earmuffs and playing with the timings finally everybody was happy. 

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Under%20Construction%202.jpg)  

Now the vintage bell button at the frontdoor got wired to the controller and we were ready for the [moment of truth.](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Final%20Test%20with%20Bell%20Button%20connected.mp4)

![github](https://github.com/yellobyte/Swiss-Style-Doorbell/raw/main/doc/Bell%20Button.jpg)

Well, the smallest horn responsible for producing the highest tone sounded quite a bit hoarse but the door bell was working and according to anybody's opinion will when activated not let anybody sleep who has the incredibly bad luck to live in it's vicinity.

The slightly extended moto-bike tour through Jura was definitely the icing on the cake. But it got better. The very next day the new owner of the noisy door-bell got approached by a neighbour from fairly down the road and asked if he had seen a post bus driving past quite a couple of times the other day. The guy reckoned he had missed it every single time it went past. When I was told the story we both were cracking up with laughter.

## Technical details

All coding was done with VSCode/PlatformIO. Sources and platformio.ini files are available in folder [**software**](https://github.com/yellobyte/Swiss-Style-Doorbell/tree/main/software).

The melody of the doorbell is coded as follows and therefore can be changed easily:
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
```
The code uses an interrupt timer to issue the 8 tones so that their timing is constant and accurate. Changing the above array would allow to add/delete tones, change their timings or re-assign valves in no time.

Added serial output allows to debug the code or simply watch it running.

## Postscript: 

If you have questions or miss some information, feel free to create an issue or contact me. However, my response time might be slow at times. 

