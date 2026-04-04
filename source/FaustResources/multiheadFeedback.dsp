import("stdfaust.lib");

// ===== Constants =====
MAXDELAYSAMPLES = int(ma.SR * 4);

// ===== The Delay Head Module =====
delayHead(i) = vgroup("Head %i", toggleOn * fbDelay(fb, dSamples, lpFreq, hpFreq, lpBypass, hpBypass) : sp.panner(pan) : *(level), *(level))
with {
    // Unique sliders for each head
    dTime    = hslider("[0]Time [unit:s]", 0.2 * (i+1), 0, 4, 0.001);
    fb       = hslider("[1]Feedback", 0.3, 0, 0.95, 0.01);
    pan      = hslider("[2]Pan", 0.5, 0, 1, 0.01);
    level    = hslider("[3]Level", 0.5, 0, 1, 0.01);
    toggleOn = checkbox("[4]On");

    // Filter controls
    lpFreq   = hslider("Filters/[0]LPF [unit:Hz]", 20000, 20, 20000, 1) : si.smoo;
    lpBypass = checkbox("Filters/[1]LP Bp");
    hpFreq   = hslider("Filters/[2]HPF [unit:Hz]", 20, 20, 20000, 1) : si.smoo;
    hpBypass = checkbox("Filters/[3]HP Bp");

    dSamples = int(dTime * ma.SR);

    // Helper function for bypassing a process
    // If 'bp' is 0, use 'proc'. If 'bp' is 1, bypass it.
    bypass(bp, proc) = _ <: proc, _ : select2(bp);

    // Feedback Delay with integrated Filters
    fbDelay(f, d, lp, hp, lpB, hpB) = _ : (+ : de.sdelay(MAXDELAYSAMPLES, 1024, d)
                                           : bypass(lpB, fi.lowpass(2, lp))
                                           : bypass(hpB, fi.highpass(2, hp))
                                       ) ~ *(f);
};

// ===== Main Process =====
process = + <: par(i, 4, delayHead(i)) :> _,_;