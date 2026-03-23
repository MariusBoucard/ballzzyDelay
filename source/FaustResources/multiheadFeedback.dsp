

// AUTRE FAUST §§§

import("stdfaust.lib");

// ===== Constants =====
MAXDELAYSAMPLES = int(ma.SR * 4);

// ===== The Delay Head Module =====
delayHead(i) = toogleOn * fbDelay(fb, dSamples, lpFreq, hpFreq) : sp.panner(pan) : *(level), *(level)
with {
    // Unique sliders for each head
    dTime    = hslider("Head %i[v: Head]/Time [unit:s]", 0.2 * (i+1), 0, 4, 0.001);
    pan      = hslider("Head %i[v: Head]/Pan", 0.5, 0, 1, 0.01);
    fb       = hslider("Head %i[v: Head]/Feedback", 0.3, 0, 0.95, 0.01);
    level    = hslider("Head %i[v: Head]/Level", 0.5, 0, 1, 0.01);
    toogleOn = checkbox("Head %i[v: Head]/on");

    // Filter controls per head
    lpFreq   = hslider("Head %i[v: Head]/LPF [unit:Hz]", 20000, 20, 20000, 1) : si.smoo;
    hpFreq   = hslider("Head %i[v: Head]/HPF [unit:Hz]", 20, 20, 20000, 1) : si.smoo;

    dSamples = int(dTime * ma.SR);

    // Feedback Delay with integrated Filters
    // Filters are placed inside the loop so echoes get darker/thinner over time
    fbDelay(f, d, lp, hp) = _ : (+ : de.sdelay(MAXDELAYSAMPLES, 1024, d)
                                   : fi.lowpass(2, lp)
                                   : fi.highpass(2, hp)) ~ *(f);
};

// ===== Main Process =====
// Input is now stereo (L, R)
// We sum the input to mono (+) before sending it to the 4 parallel delay heads
process = + <: par(i, 4, delayHead(i)) :> _,_;