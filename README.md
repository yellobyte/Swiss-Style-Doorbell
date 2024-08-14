# Unique Doorbell in Swiss Style
A funny and extraordinary version of a normal doorbell. Here in typical swiss style useing an old swiss post car horn.

## Description



## Technical details


The melody of the doorbell is coded as follows:
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
